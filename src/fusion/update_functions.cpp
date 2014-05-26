/*
 * update_functions.cpp
 *
 *  Created on: May 5, 2013
 *      Author: steinbrf
 */



void updateVarMulLoopSimSingle
(
		const cv::Mat &image, std::vector<cv::Mat> rgb,
		int imageWidth, int imageHeight,
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float fx, float fy, float cx, float cy,
		float scale, float distanceThreshold,
		volumetype nLeavesQueued,
		volumetype *_leafNumber,
		sidetype3 *_leafPos,
		sidetype *_leafScale,
		float *_distance,
		weighttype *_weights,
		colortype3 *_color,
		sidetype _brickLength
)
{
	float *data = (float*)image.data;

	volumetype brickSize = _brickLength*_brickLength*_brickLength;

	for(volumetype l=0;l<nLeavesQueued;l++){
		volumetype brickIdx = _leafNumber[l];
		sidetype3 o = _leafPos[brickIdx];
		sidetype leafScale = _leafScale[brickIdx];
		for(sidetype z=0;z<_brickLength;z++){
			float pz = (o.z+z*leafScale)*scale;
			float zmulx = m13*pz+m14;
			float zmuly = m23*pz+m24;
			float zmulz = m33*pz+m34;
			for(sidetype y=0;y<_brickLength;y++){
				int threadOffsetY = (z*_brickLength+y)*_brickLength;
				float py = (o.y+y*leafScale)*scale;
				float ymulx = m12*py+zmulx;
				float ymuly = m22*py+zmuly;
				float ymulz = m32*py+zmulz;
				for(sidetype x=0;x<_brickLength;x+=4){

					__attribute__ ((aligned (16))) int threadOffset[4] = {
							threadOffsetY + x + 0, threadOffsetY + x + 1,
							threadOffsetY + x + 2, threadOffsetY + x + 3
					};

					__attribute__ ((aligned (16))) float px[4] = {
							(o.x+(x+0)*leafScale)*scale, (o.x+(x+1)*leafScale)*scale,
							(o.x+(x+2)*leafScale)*scale, (o.x+(x+3)*leafScale)*scale
					};

					__attribute__ ((aligned (16))) float qx[4] = {
								m11*px[0]+ymulx,
								m11*px[1]+ymulx,
								m11*px[2]+ymulx,
								m11*px[3]+ymulx
						};

					__attribute__ ((aligned (16))) float qy[4] = {
								m21*px[0]+ymuly,
								m21*px[1]+ymuly,
								m21*px[2]+ymuly,
								m21*px[3]+ymuly
						};

					__attribute__ ((aligned (16))) float qz[4] = {
								m31*px[0]+ymulz,
								m31*px[1]+ymulz,
								m31*px[2]+ymulz,
								m31*px[3]+ymulz
						};

					float length[4] = {
							sqrtf(qx[0]*qx[0]+qy[0]*qy[0]+qz[0]*qz[0]),
							sqrtf(qx[1]*qx[1]+qy[1]*qy[1]+qz[1]*qz[1]),
							sqrtf(qx[2]*qx[2]+qy[2]*qy[2]+qz[2]*qz[2]),
							sqrtf(qx[3]*qx[3]+qy[3]*qy[3]+qz[3]*qz[3])
					};






					__attribute__ ((aligned (16))) int imx[4] = {
							(int)floor(qx[0]/qz[0]*fx+cx),
							(int)floor(qx[1]/qz[1]*fx+cx),
							(int)floor(qx[2]/qz[2]*fx+cx),
							(int)floor(qx[3]/qz[3]*fx+cx)
					};


					__attribute__ ((aligned (16))) int imy[4] = {
							(int)floor(qy[0]/qz[0]*fy+cy),
							(int)floor(qy[1]/qz[1]*fy+cy),
							(int)floor(qy[2]/qz[2]*fy+cy),
							(int)floor(qy[3]/qz[3]*fy+cy)
					};

					float h[4] = {
							data[std::max(std::min(imy[0],imageHeight-1),0)*imageWidth+
							     std::max(std::min(imx[0],imageWidth-1),0)],
							data[std::max(std::min(imy[1],imageHeight-1),0)*imageWidth+
									 std::max(std::min(imx[1],imageWidth-1),0)],
							data[std::max(std::min(imy[2],imageHeight-1),0)*imageWidth+
									 std::max(std::min(imx[2],imageWidth-1),0)],
							data[std::max(std::min(imy[3],imageHeight-1),0)*imageWidth+
									 std::max(std::min(imx[3],imageWidth-1),0)]
					};

					float distance[4] = {
							length[0] - length[0]/qz[0]*h[0],
							length[1] - length[1]/qz[1]*h[1],
							length[2] - length[2]/qz[2]*h[2],
							length[3] - length[3]/qz[3]*h[3]
					};

					for(int i=0;i<4;i++)
					if(qz[i] > 0.0f && std::isfinite(distance[i]) && imx[i]>=0 && imy[i]>=0 &&
							imx[i]<imageWidth && imy[i]<imageHeight){
						volumetype idx = brickIdx*brickSize+threadOffset[i];
						weighttype weight = getDistanceWeight_AoS_variables_h(distance[i],WEIGHT_FACTOR*distanceThreshold*leafScale,DISTANCEWEIGHTEPSILON,DISTANCEWEIGHTSIGMA);
						weighttype prevWeight = _weights[idx];
						float factor = 1.0f/((float)(prevWeight+weight)+(float)(prevWeight+weight==0));
						imx[i] = std::max(std::min(imx[i],imageWidth-1),0);
						imy[i] = std::max(std::min(imy[i],imageHeight-1),0);
					  _distance[idx] = (_distance[idx]*prevWeight + processDistanceValue_AoS_h(distance[i],distanceThreshold*leafScale)*weight)*factor;
						_weights[idx] = prevWeight+weight;
						_color[idx].x = ((float)(_color[idx].x)*prevWeight + (float)rgb[2].at<uchar>(imy[i],imx[i])*COLOR_MULTIPLICATOR*weight)*factor;
						_color[idx].y = ((float)(_color[idx].y)*prevWeight + (float)rgb[1].at<uchar>(imy[i],imx[i])*COLOR_MULTIPLICATOR*weight)*factor;
						_color[idx].z = ((float)(_color[idx].z)*prevWeight + (float)rgb[0].at<uchar>(imy[i],imx[i])*COLOR_MULTIPLICATOR*weight)*factor;
					}
				}
			}
		}
	}

}









void update8NaiveFloat
(
		const float *depth, const uchar *red, const uchar *green, const uchar *blue,
		int imageWidth, int imageHeight,
		float &m11, float &m12, float &m13, float &m14,
		float &m21, float &m22, float &m23, float &m24,
		float &m31, float &m32, float &m33, float &m34,
		float &fx, float &fy, float &cx, float &cy,
		float &scale, float &distanceThreshold,
		volumetype &brickIdx,
		sidetype3 &o,
		sidetype &leafScale,
		float *_distance,
		weighttype *_weights,
		colortype3 *_color
)
{
	volumetype threadOffset = brickIdx*512;
	float fleafScale = (float)(leafScale)*scale;
	float ox = (m11*o.x+m12*o.y+m13*o.z)*scale + m14;
	float oy = (m21*o.x+m22*o.y+m23*o.z)*scale + m24;
	float oz = (m31*o.x+m32*o.y+m33*o.z)*scale + m34;
	int x,y,z;

	float d11 = m11*fleafScale;
	float d21 = m21*fleafScale;
	float d31 = m31*fleafScale;

	float d12 = m12*fleafScale;
	float d22 = m22*fleafScale;
	float d32 = m32*fleafScale;

	float d13 = m13*fleafScale;
	float d23 = m23*fleafScale;
	float d33 = m33*fleafScale;

	for(z=0;z<8;z++){
		for(y=0;y<8;y++){
			for(x=0;x<8;x++){
				volumetype idx = threadOffset++;
				float pxx = ox + d11*(float)x + d12*(float)y + d13*(float)z;
				float pxy = oy + d21*(float)x + d22*(float)y + d23*(float)z;
				float pxz = oz + d31*(float)x + d32*(float)y + d33*(float)z;

				if(pxz > 0.0f){
					int imx = (int)floor(pxx/pxz*fx+cx);
					int imy = (int)floor(pxy/pxz*fy+cy);
					if(imx>=0 && imy>=0 && imx<imageWidth && imy<imageHeight){
						int imageIndex = imy*imageWidth + imx;
						float h = depth[imageIndex];
						float length = sqrtf(pxx*pxx+pxy*pxy+pxz*pxz);
						float distance = length - length/pxz*h;
						if(std::isfinite(distance)){
							weighttype weight = getDistanceWeight_AoS_variables_h(distance,WEIGHT_FACTOR*distanceThreshold*leafScale,DISTANCEWEIGHTEPSILON,DISTANCEWEIGHTSIGMA);
							weighttype prevWeight = _weights[idx];
							float factor = 1.0f/((float)(prevWeight+weight)+(float)(prevWeight+weight==0));
							_distance[idx] = (_distance[idx]*prevWeight + processDistanceValue_AoS_h(distance,distanceThreshold*leafScale)*weight)*factor;
							_weights[idx] = prevWeight+weight;
							_color[idx].x = ((float)(_color[idx].x)*prevWeight + red[imageIndex]  *COLOR_MULTIPLICATOR*weight)*factor;
							_color[idx].y = ((float)(_color[idx].y)*prevWeight + green[imageIndex]*COLOR_MULTIPLICATOR*weight)*factor;
							_color[idx].z = ((float)(_color[idx].z)*prevWeight + blue[imageIndex] *COLOR_MULTIPLICATOR*weight)*factor;
						}
					}
				}
			}
		}
	}
}


void update8NaiveInteger
(
		const ushort *depth, float scaling, float maxcamdistance, const uchar *rgb,
		int imageWidth, int imageHeight,
		float &m11, float &m12, float &m13, float &m14,
		float &m21, float &m22, float &m23, float &m24,
		float &m31, float &m32, float &m33, float &m34,
		float &fx, float &fy, float &cx, float &cy,
		float &scale, float &distanceThreshold,
		volumetype &brickIdx,
		sidetype3 &o,
		sidetype &leafScale,
		float *_distance,
		weighttype *_weights,
		colortype3 *_color
)
{
	volumetype threadOffset = brickIdx*512;
	float fleafScale = (float)(leafScale)*scale;
	float ox = (m11*o.x+m12*o.y+m13*o.z)*scale + m14;
	float oy = (m21*o.x+m22*o.y+m23*o.z)*scale + m24;
	float oz = (m31*o.x+m32*o.y+m33*o.z)*scale + m34;
	int x,y,z;

	float d11 = m11*fleafScale;
	float d21 = m21*fleafScale;
	float d31 = m31*fleafScale;

	float d12 = m12*fleafScale;
	float d22 = m22*fleafScale;
	float d32 = m32*fleafScale;

	float d13 = m13*fleafScale;
	float d23 = m23*fleafScale;
	float d33 = m33*fleafScale;

	for(z=0;z<8;z++){
		for(y=0;y<8;y++){
			for(x=0;x<8;x++){
				volumetype idx = threadOffset++;
				float pxx = ox + d11*(float)x + d12*(float)y + d13*(float)z;
				float pxy = oy + d21*(float)x + d22*(float)y + d23*(float)z;
				float pxz = oz + d31*(float)x + d32*(float)y + d33*(float)z;

				float pxx1 = ox + d11*(float)(x+1) + d12*(float)y + d13*(float)z;
				float pxy1 = oy + d21*(float)(x+1) + d22*(float)y + d23*(float)z;
				float pxz1 = oz + d31*(float)(x+1) + d32*(float)y + d33*(float)z;

				float pxx2 = ox + d11*(float)x + d12*(float)(y+1) + d13*(float)z;
				float pxy2 = oy + d21*(float)x + d22*(float)(y+1) + d23*(float)z;
				float pxz2 = oz + d31*(float)x + d32*(float)(y+1) + d33*(float)z;

				float pxx3 = ox + d11*(float)(x+1) + d12*(float)(y+1) + d13*(float)z;
				float pxy3 = oy + d21*(float)(x+1) + d22*(float)(y+1) + d23*(float)z;
				float pxz3 = oz + d31*(float)(x+1) + d32*(float)(y+1) + d33*(float)z;

				float pxx4 = ox + d11*(float)x + d12*(float)y + d13*(float)(z+1);
				float pxy4 = oy + d21*(float)x + d22*(float)y + d23*(float)(z+1);
				float pxz4 = oz + d31*(float)x + d32*(float)y + d33*(float)(z+1);

				float pxx5 = ox + d11*(float)(x+1) + d12*(float)y + d13*(float)(z+1);
				float pxy5 = oy + d21*(float)(x+1) + d22*(float)y + d23*(float)(z+1);
				float pxz5 = oz + d31*(float)(x+1) + d32*(float)y + d33*(float)(z+1);

				float pxx6 = ox + d11*(float)x + d12*(float)(y+1) + d13*(float)(z+1);
				float pxy6 = oy + d21*(float)x + d22*(float)(y+1) + d23*(float)(z+1);
				float pxz6 = oz + d31*(float)x + d32*(float)(y+1) + d33*(float)(z+1);

				float pxx7 = ox + d11*(float)(x+1) + d12*(float)(y+1) + d13*(float)(z+1);
				float pxy7 = oy + d21*(float)(x+1) + d22*(float)(y+1) + d23*(float)(z+1);
				float pxz7 = oz + d31*(float)(x+1) + d32*(float)(y+1) + d33*(float)(z+1);

				if(pxz > 0.0f){
					int imx = (int)floor(pxx/pxz*fx+cx);
					int imy = (int)floor(pxy/pxz*fy+cy);

					int imx1 = (int)floor(pxx1/pxz1*fx+cx);
					int imy1 = (int)floor(pxy1/pxz1*fy+cy);

					int imx2 = (int)floor(pxx2/pxz2*fx+cx);
					int imy2 = (int)floor(pxy2/pxz2*fy+cy);

					int imx3 = (int)floor(pxx3/pxz3*fx+cx);
					int imy3 = (int)floor(pxy3/pxz3*fy+cy);

					int imx4 = (int)floor(pxx4/pxz4*fx+cx);
					int imy4 = (int)floor(pxy4/pxz4*fy+cy);

					int imx5 = (int)floor(pxx5/pxz5*fx+cx);
					int imy5 = (int)floor(pxy5/pxz5*fy+cy);

					int imx6 = (int)floor(pxx6/pxz6*fx+cx);
					int imy6 = (int)floor(pxy6/pxz6*fy+cy);

					int imx7 = (int)floor(pxx7/pxz7*fx+cx);
					int imy7 = (int)floor(pxy7/pxz7*fy+cy);


					if(imx>=0 && imy>=0 && imx<imageWidth && imy<imageHeight
							&& imx1>=0 && imy1>=0 && imx1<imageWidth && imy1<imageHeight
							&& imx2>=0 && imy2>=0 && imx2<imageWidth && imy2<imageHeight
							&& imx3>=0 && imy3>=0 && imx3<imageWidth && imy3<imageHeight
							&& imx4>=0 && imy4>=0 && imx4<imageWidth && imy4<imageHeight
							&& imx5>=0 && imy5>=0 && imx5<imageWidth && imy5<imageHeight
							&& imx6>=0 && imy6>=0 && imx6<imageWidth && imy6<imageHeight
							&& imx7>=0 && imy7>=0 && imx7<imageWidth && imy7<imageHeight
					){
						int imageIndex = imy*imageWidth + imx;
						float h = (float)(depth[imageIndex])*scaling;
						float length = sqrtf(pxx*pxx+pxy*pxy+pxz*pxz);
						float distance = length - length/pxz*h;
						if(h>0.0f && h<maxcamdistance){
							weighttype weight = getDistanceWeight_AoS_variables_h(distance,WEIGHT_FACTOR*distanceThreshold*leafScale,DISTANCEWEIGHTEPSILON,DISTANCEWEIGHTSIGMA);
							weighttype prevWeight = _weights[idx];
							float factor = 1.0f/((float)(prevWeight+weight)+(float)(prevWeight+weight==0));
							_distance[idx] = (_distance[idx]*prevWeight + processDistanceValue_AoS_h(distance,distanceThreshold*leafScale)*weight)*factor;
							_weights[idx] = prevWeight+weight;
							_color[idx].x = ((float)(_color[idx].x)*prevWeight + rgb[3*imageIndex+2]*COLOR_MULTIPLICATOR*weight)*factor;
							_color[idx].y = ((float)(_color[idx].y)*prevWeight + rgb[3*imageIndex+1]*COLOR_MULTIPLICATOR*weight)*factor;
							_color[idx].z = ((float)(_color[idx].z)*prevWeight + rgb[3*imageIndex+0]*COLOR_MULTIPLICATOR*weight)*factor;
						}
					}
				}
			}
		}
	}
}



void update8AddLoopLoopSingleFloat
(
		const float *depth, const uchar *red, const uchar *green, const uchar *blue,
		int imageWidth, int imageHeight,
		float &m11, float &m12, float &m13, float &m14,
		float &m21, float &m22, float &m23, float &m24,
		float &m31, float &m32, float &m33, float &m34,
		float &fx, float &fy, float &cx, float &cy,
		float &scale, float &distanceThreshold,
		volumetype &brickIdx,
		sidetype3 &o,
		sidetype &leafScale,
		float *_distance,
		weighttype *_weights,
		colortype3 *_color
)
{
	volumetype threadOffset = brickIdx*512;
	float fleafScale = (float)(leafScale)*scale;
	float ox = (m11*o.x+m12*o.y+m13*o.z)*scale + m14;
	float oy = (m21*o.x+m22*o.y+m23*o.z)*scale + m24;
	float oz = (m31*o.x+m32*o.y+m33*o.z)*scale + m34;
	float pxx, pxy, pxz, pyx, pyy, pyz, pzx, pzy, pzz;
	int x,y,z;


	float d11 = m11*fleafScale;
	float d21 = m21*fleafScale;
	float d31 = m31*fleafScale;

	float d12 = m12*fleafScale;
	float d22 = m22*fleafScale;
	float d32 = m32*fleafScale;

	float d13 = m13*fleafScale;
	float d23 = m23*fleafScale;
	float d33 = m33*fleafScale;

	for(z=0,pzx=ox,pzy=oy,pzz=oz;z<8;z++,pzx+=d13,pzy+=d23,pzz+=d33){
		for(y=0,pyx=pzx,pyy=pzy,pyz=pzz;y<8;y++,pyx+=d12,pyy+=d22,pyz+=d32){
			for(x=0,pxx=pyx,pxy=pyy,pxz=pyz;x<8;x++,pxx+=d11,pxy+=d21,pxz+=d31){
				volumetype idx = threadOffset++;
				if(pxz > 0.0f){
					int imx = (int)floor(pxx/pxz*fx+cx);
					int imy = (int)floor(pxy/pxz*fy+cy);
					if(imx>=0 && imy>=0 && imx<imageWidth && imy<imageHeight){
						int imageIndex = imy*imageWidth + imx;
						float h = depth[imageIndex];
						float length = sqrtf(pxx*pxx+pxy*pxy+pxz*pxz);
						float distance = length - length/pxz*h;
						if(std::isfinite(distance)){
							weighttype weight = getDistanceWeight_AoS_variables_h(distance,WEIGHT_FACTOR*distanceThreshold*leafScale,DISTANCEWEIGHTEPSILON,DISTANCEWEIGHTSIGMA);
							weighttype prevWeight = _weights[idx];
							float factor = 1.0f/((float)(prevWeight+weight)+(float)(prevWeight+weight==0));
							_distance[idx] = (_distance[idx]*prevWeight + processDistanceValue_AoS_h(distance,distanceThreshold*leafScale)*weight)*factor;
							_weights[idx] = prevWeight+weight;
							_color[idx].x = ((float)(_color[idx].x)*prevWeight + red[imageIndex]  *COLOR_MULTIPLICATOR*weight)*factor;
							_color[idx].y = ((float)(_color[idx].y)*prevWeight + green[imageIndex]*COLOR_MULTIPLICATOR*weight)*factor;
							_color[idx].z = ((float)(_color[idx].z)*prevWeight + blue[imageIndex] *COLOR_MULTIPLICATOR*weight)*factor;
						}
					}
				}
			}
		}
	}
}



void update8AddLoopLoopSingle_noJumpsFloat
(
		const float *depth, const uchar *red, const uchar *green, const uchar *blue,
		int imageWidth, int imageHeight,
		float &m11, float &m12, float &m13, float &m14,
		float &m21, float &m22, float &m23, float &m24,
		float &m31, float &m32, float &m33, float &m34,
		float &fx, float &fy, float &cx, float &cy,
		float &scale, float &distanceThreshold,
		volumetype &brickIdx,
		sidetype3 &o,
		sidetype &leafScale,
		float *_distance,
		weighttype *_weights,
		colortype3 *_color
)
{
	volumetype threadOffset = brickIdx*512;
	float fleafScale = (float)(leafScale)*scale;
	float ox = (m11*o.x+m12*o.y+m13*o.z)*scale + m14;
	float oy = (m21*o.x+m22*o.y+m23*o.z)*scale + m24;
	float oz = (m31*o.x+m32*o.y+m33*o.z)*scale + m34;
	float pxx, pxy, pxz, pyx, pyy, pyz, pzx, pzy, pzz;


	float d11 = m11*fleafScale;
	float d21 = m21*fleafScale;
	float d31 = m31*fleafScale;

//#define USE_SSE_HERE


	float d12 = m12*fleafScale;
	float d22 = m22*fleafScale;
	float d32 = m32*fleafScale;

	float d13 = m13*fleafScale;
	float d23 = m23*fleafScale;
	float d33 = m33*fleafScale;

	pzx=ox;pzy=oy;pzz=oz;
	for(int z=0;z<8;z++,pzx+=d13,pzy+=d23,pzz+=d33){
		pyx=pzx;pyy=pzy;pyz=pzz;
		for(int y=0;y<8;y++,pyx+=d12,pyy+=d22,pyz+=d32){

			pxx=pyx;pxy=pyy;pxz=pyz;


			for(int x=0;x<8;x++,pxx+=d11,pxy+=d21,pxz+=d31){
				volumetype idx = threadOffset++;
				float length = sqrtf(pxx*pxx+pxy*pxy+pxz*pxz);
				int imx = (int)floor(pxx/pxz*fx+cx);
				int imy = (int)floor(pxy/pxz*fy+cy);
				int imageIndex = imy*(imy>=0 && imy<imageHeight)*imageWidth + imx*(imx>=0 && imx<imageWidth);

				float h = depth[imageIndex];
				float dInc = length - length/pxz*h;
				float rInc = (float)(red[imageIndex]*COLOR_MULTIPLICATOR);
				float gInc = (float)(green[imageIndex]*COLOR_MULTIPLICATOR);
				float bInc = (float)(blue[imageIndex]*COLOR_MULTIPLICATOR);

				float dPrev = _distance[idx];
				weighttype wPrev = _weights[idx];
				float rPrev = (float)(_color[idx].x);
				float gPrev = (float)(_color[idx].y);
				float bPrev = (float)(_color[idx].z);

				unsigned int mask = (pxz > 0.0f && imx>=0 && imy>=0 && imx<imageWidth && imy<imageHeight && std::isfinite(dInc))*4294967295;
				float threshold = WEIGHT_FACTOR*distanceThreshold*leafScale;
				weighttype wInc = mask ? ((float)(dInc<DISTANCEWEIGHTEPSILON) +
						(threshold-dInc)/(threshold-DISTANCEWEIGHTEPSILON)*
						(float)(dInc>=DISTANCEWEIGHTEPSILON && dInc<threshold)) : 0;

					float factor = 1.0f/((float)(wPrev+wInc)+(float)(wPrev+wInc==0));
					_distance[idx] = (dPrev*wPrev +
							(mask ? std::max(-distanceThreshold*leafScale,std::min(distanceThreshold*leafScale,dInc))*wInc : 0))*factor;
					_weights[idx] = wPrev+wInc;
    			_color[idx].x = (rPrev*wPrev + rInc*wInc)*factor;
					_color[idx].y = (gPrev*wPrev + gInc*wInc)*factor;
					_color[idx].z = (bPrev*wPrev + bInc*wInc)*factor;
			}

		}
	}
}


void update8AddLoopSIMDSingleFloat
(
		const float *depth, const uchar *red, const uchar *green, const uchar *blue,
		int imageWidth, int imageHeight,
		float &m11, float &m12, float &m13, float &m14,
		float &m21, float &m22, float &m23, float &m24,
		float &m31, float &m32, float &m33, float &m34,
		float &fx, float &fy, float &cx, float &cy,
		float &scale, float &distanceThreshold,
		volumetype &brickIdx,
		sidetype3 &o,
		sidetype &leafScale,
		float *_distance,
		weighttype *_weights,
		colortype3 *_color
)
{
	volumetype threadOffset = brickIdx*512;
	float fleafScale = (float)(leafScale)*scale;
	float ox = (m11*o.x+m12*o.y+m13*o.z)*scale + m14;
	float oy = (m21*o.x+m22*o.y+m23*o.z)*scale + m24;
	float oz = (m31*o.x+m32*o.y+m33*o.z)*scale + m34;
	float pxx, pxy, pxz, pyx, pyy, pyz, pzx, pzy, pzz;


	float d11 = m11*fleafScale;
	float d21 = m21*fleafScale;
	float d31 = m31*fleafScale;

//#define USE_SSE_HERE


	float d12 = m12*fleafScale;
	float d22 = m22*fleafScale;
	float d32 = m32*fleafScale;

	float d13 = m13*fleafScale;
	float d23 = m23*fleafScale;
	float d33 = m33*fleafScale;

	pzx=ox;pzy=oy;pzz=oz;
	for(int z=0;z<8;z++,pzx+=d13,pzy+=d23,pzz+=d33){
		pyx=pzx;pyy=pzy;pyz=pzz;
		for(int y=0;y<8;y++,pyx+=d12,pyy+=d22,pyz+=d32){

			pxx=pyx;pxy=pyy;pxz=pyz;

			for(int x=0;x<2;x++,pxx+=d11*4.0f,pxy+=d21*4.0f,pxz+=d31*4.0f){

				float pxx4[4] = {
						(pxx+d11*0.0f),
						(pxx+d11*1.0f),
						(pxx+d11*2.0f),
						(pxx+d11*3.0f)};
				float pxy4[4] = {
						(pxy+d21*0.0f),
						(pxy+d21*1.0f),
						(pxy+d21*2.0f),
						(pxy+d21*3.0f)};
				float pxz4[4] = {
						(pxz+d31*0.0f),
						(pxz+d31*1.0f),
						(pxz+d31*2.0f),
						(pxz+d31*3.0f)};

				volumetype idx = threadOffset; threadOffset += 4;
				float length[4] = {
						sqrtf(pxx4[0]*pxx4[0] + pxy4[0]*pxy4[0] + pxz4[0]*pxz4[0]),
						sqrtf(pxx4[1]*pxx4[1] + pxy4[1]*pxy4[1] + pxz4[1]*pxz4[1]),
						sqrtf(pxx4[2]*pxx4[2] + pxy4[2]*pxy4[2] + pxz4[2]*pxz4[2]),
						sqrtf(pxx4[3]*pxx4[3] + pxy4[3]*pxy4[3] + pxz4[3]*pxz4[3])
				};
				int imx[4] = {
						(int)floor(pxx4[0]/pxz4[0]*fx+cx),
						(int)floor(pxx4[1]/pxz4[1]*fx+cx),
						(int)floor(pxx4[2]/pxz4[2]*fx+cx),
						(int)floor(pxx4[3]/pxz4[3]*fx+cx)
				};
				int imy[4] = {
						(int)floor(pxy4[0]/pxz4[0]*fy+cy),
						(int)floor(pxy4[1]/pxz4[1]*fy+cy),
						(int)floor(pxy4[2]/pxz4[2]*fy+cy),
						(int)floor(pxy4[3]/pxz4[3]*fy+cy)
				};


				int imageIndex[4] = {
						(imy[0]>=0 ? (imy[0]<imageHeight ? imy[0] : imageHeight-1) : 0)*imageWidth + (imx[0]>=0 ? (imx[0]<imageWidth ? imx[0] : imageWidth-1) : 0),
						(imy[1]>=0 ? (imy[1]<imageHeight ? imy[1] : imageHeight-1) : 0)*imageWidth + (imx[1]>=0 ? (imx[1]<imageWidth ? imx[1] : imageWidth-1) : 0),
						(imy[2]>=0 ? (imy[2]<imageHeight ? imy[2] : imageHeight-1) : 0)*imageWidth + (imx[2]>=0 ? (imx[2]<imageWidth ? imx[2] : imageWidth-1) : 0),
						(imy[3]>=0 ? (imy[3]<imageHeight ? imy[3] : imageHeight-1) : 0)*imageWidth + (imx[3]>=0 ? (imx[3]<imageWidth ? imx[3] : imageWidth-1) : 0)
				};

				float h[4] = {
						depth[imageIndex[0]],
						depth[imageIndex[1]],
						depth[imageIndex[2]],
						depth[imageIndex[3]]
				};


				float dInc[4] = {
						length[0] - length[0]/pxz4[0]*h[0],
						length[1] - length[1]/pxz4[1]*h[1],
						length[2] - length[2]/pxz4[2]*h[2],
						length[3] - length[3]/pxz4[3]*h[3]
				};
				float rInc[4] = {
						(float)(red[imageIndex[0]]*COLOR_MULTIPLICATOR),
						(float)(red[imageIndex[1]]*COLOR_MULTIPLICATOR),
						(float)(red[imageIndex[2]]*COLOR_MULTIPLICATOR),
						(float)(red[imageIndex[3]]*COLOR_MULTIPLICATOR)
				};
				float gInc[4] = {
						(float)(green[imageIndex[0]]*COLOR_MULTIPLICATOR),
						(float)(green[imageIndex[1]]*COLOR_MULTIPLICATOR),
						(float)(green[imageIndex[2]]*COLOR_MULTIPLICATOR),
						(float)(green[imageIndex[3]]*COLOR_MULTIPLICATOR)
				};
				float bInc[4] = {
						(float)(blue[imageIndex[0]]*COLOR_MULTIPLICATOR),
						(float)(blue[imageIndex[1]]*COLOR_MULTIPLICATOR),
						(float)(blue[imageIndex[2]]*COLOR_MULTIPLICATOR),
						(float)(blue[imageIndex[3]]*COLOR_MULTIPLICATOR)
				};

				float dPrev[4] = {
						_distance[idx+0],
						_distance[idx+1],
						_distance[idx+2],
						_distance[idx+3]
				};
				weighttype wPrev[4] = {
						_weights[idx+0],
						_weights[idx+1],
						_weights[idx+2],
						_weights[idx+3],
				};
				float rPrev[4] = {
						(float)(_color[idx+0].x),
						(float)(_color[idx+1].x),
						(float)(_color[idx+2].x),
						(float)(_color[idx+3].x)
				};
				float gPrev[4] = {
						(float)(_color[idx+0].y),
						(float)(_color[idx+1].y),
						(float)(_color[idx+2].y),
						(float)(_color[idx+3].y)
				};
				float bPrev[4] = {
						(float)(_color[idx+0].z),
						(float)(_color[idx+1].z),
						(float)(_color[idx+2].z),
						(float)(_color[idx+3].z)
				};

				int mask4[4] = {
						(int)((pxz4[0] > 0.0f && imx[0]>=0 && imy[0]>=0 && imx[0]<imageWidth &&
								imy[0]<imageHeight && std::isfinite(dInc[0]))*4294967295),
						(int)((pxz4[1] > 0.0f && imx[1]>=0 && imy[1]>=0 && imx[1]<imageWidth &&
								imy[1]<imageHeight && std::isfinite(dInc[1]))*4294967295),
						(int)((pxz4[2] > 0.0f && imx[2]>=0 && imy[2]>=0 && imx[2]<imageWidth &&
								imy[2]<imageHeight && std::isfinite(dInc[2]))*4294967295),
						(int)((pxz4[3] > 0.0f && imx[3]>=0 && imy[3]>=0 && imx[3]<imageWidth &&
								imy[3]<imageHeight && std::isfinite(dInc[3]))*4294967295)
				};


				float threshold = WEIGHT_FACTOR*distanceThreshold*leafScale;
				weighttype wInc4[4] = {
						mask4[0] ? ((float)(dInc[0]<DISTANCEWEIGHTEPSILON) +
						(threshold-dInc[0])/(threshold-DISTANCEWEIGHTEPSILON)*
						(float)(dInc[0]>=DISTANCEWEIGHTEPSILON && dInc[0]<threshold)) : 0,
						mask4[1] ? ((float)(dInc[1]<DISTANCEWEIGHTEPSILON) +
						(threshold-dInc[1])/(threshold-DISTANCEWEIGHTEPSILON)*
						(float)(dInc[1]>=DISTANCEWEIGHTEPSILON && dInc[1]<threshold)) : 0,
						mask4[2] ? ((float)(dInc[2]<DISTANCEWEIGHTEPSILON) +
						(threshold-dInc[2])/(threshold-DISTANCEWEIGHTEPSILON)*
						(float)(dInc[2]>=DISTANCEWEIGHTEPSILON && dInc[2]<threshold)) : 0,
						mask4[3] ? ((float)(dInc[3]<DISTANCEWEIGHTEPSILON) +
						(threshold-dInc[3])/(threshold-DISTANCEWEIGHTEPSILON)*
						(float)(dInc[3]>=DISTANCEWEIGHTEPSILON && dInc[3]<threshold)) : 0
				};


					float factor[4] = {
							1.0f/((float)(wPrev[0]+wInc4[0])+(float)(wPrev[0]+wInc4[0]==0)),
							1.0f/((float)(wPrev[1]+wInc4[1])+(float)(wPrev[1]+wInc4[1]==0)),
							1.0f/((float)(wPrev[2]+wInc4[2])+(float)(wPrev[2]+wInc4[2]==0)),
							1.0f/((float)(wPrev[3]+wInc4[3])+(float)(wPrev[3]+wInc4[3]==0))
					};
					_distance[idx+0] = (dPrev[0]*wPrev[0] +
							(mask4[0] ? std::max(-distanceThreshold*leafScale,
									std::min(distanceThreshold*leafScale,dInc[0]))*wInc4[0] : 0))*factor[0];
					_distance[idx+1] = (dPrev[1]*wPrev[1] +
							(mask4[1] ? std::max(-distanceThreshold*leafScale,
									std::min(distanceThreshold*leafScale,dInc[1]))*wInc4[1] : 0))*factor[1];
					_distance[idx+2] = (dPrev[2]*wPrev[2] +
							(mask4[2] ? std::max(-distanceThreshold*leafScale,
									std::min(distanceThreshold*leafScale,dInc[2]))*wInc4[2] : 0))*factor[2];
					_distance[idx+3] = (dPrev[3]*wPrev[3] +
							(mask4[3] ? std::max(-distanceThreshold*leafScale,
									std::min(distanceThreshold*leafScale,dInc[3]))*wInc4[3] : 0))*factor[3];

					_weights[idx+0] = wPrev[0]+wInc4[0];
					_weights[idx+1] = wPrev[1]+wInc4[1];
					_weights[idx+2] = wPrev[2]+wInc4[2];
					_weights[idx+3] = wPrev[3]+wInc4[3];

    			_color[idx+0].x = (rPrev[0]*wPrev[0] + rInc[0]*wInc4[0])*factor[0];
    			_color[idx+1].x = (rPrev[1]*wPrev[1] + rInc[1]*wInc4[1])*factor[1];
    			_color[idx+2].x = (rPrev[2]*wPrev[2] + rInc[2]*wInc4[2])*factor[2];
    			_color[idx+3].x = (rPrev[3]*wPrev[3] + rInc[3]*wInc4[3])*factor[3];

    			_color[idx+0].y = (gPrev[0]*wPrev[0] + gInc[0]*wInc4[0])*factor[0];
    			_color[idx+1].y = (gPrev[1]*wPrev[1] + gInc[1]*wInc4[1])*factor[1];
    			_color[idx+2].y = (gPrev[2]*wPrev[2] + gInc[2]*wInc4[2])*factor[2];
    			_color[idx+3].y = (gPrev[3]*wPrev[3] + gInc[3]*wInc4[3])*factor[3];

    			_color[idx+0].z = (bPrev[0]*wPrev[0] + bInc[0]*wInc4[0])*factor[0];
    			_color[idx+1].z = (bPrev[1]*wPrev[1] + bInc[1]*wInc4[1])*factor[1];
    			_color[idx+2].z = (bPrev[2]*wPrev[2] + bInc[2]*wInc4[2])*factor[2];
    			_color[idx+3].z = (bPrev[3]*wPrev[3] + bInc[3]*wInc4[3])*factor[3];

			}
		}
	}
}



void update8AddLoopSSESingleFloat
(
		const float *depth, const uchar *red, const uchar *green, const uchar *blue,
		int imageWidth, int imageHeight,
		float &m11, float &m12, float &m13, float &m14,
		float &m21, float &m22, float &m23, float &m24,
		float &m31, float &m32, float &m33, float &m34,
		float &fx, float &fy, float &cx, float &cy,
		float &scale, float &distanceThreshold,
		volumetype &brickIdx,
		sidetype3 &o,
		sidetype &leafScale,
		float *_distance,
		weighttype *_weights,
		colortype3 *_color
)
{
	volumetype threadOffset = brickIdx*512;
	float fleafScale = (float)(leafScale)*scale;
	float ox = (m11*o.x+m12*o.y+m13*o.z)*scale + m14;
	float oy = (m21*o.x+m22*o.y+m23*o.z)*scale + m24;
	float oz = (m31*o.x+m32*o.y+m33*o.z)*scale + m34;
	float pxx, pxy, pxz, pyx, pyy, pyz, pzx, pzy, pzz;


	float d11 = m11*fleafScale;
	float d21 = m21*fleafScale;
	float d31 = m31*fleafScale;

	__m128 d11_SSE = _mm_setr_ps(0.0f, d11, 2.0f*d11, 3.0f*d11);
	__m128 d21_SSE = _mm_setr_ps(0.0f, d21, 2.0f*d21, 3.0f*d21);
	__m128 d31_SSE = _mm_setr_ps(0.0f, d31, 2.0f*d31, 3.0f*d31);
  __m128 thresholdDistance = _mm_set1_ps(distanceThreshold*leafScale);
  __m128 thresholdWeight = _mm_set1_ps(WEIGHT_FACTOR*distanceThreshold*leafScale);
	d11*=4.0f; d21*=4.0f; d31*=4.0f;

	float d12 = m12*fleafScale;
	float d22 = m22*fleafScale;
	float d32 = m32*fleafScale;

	float d13 = m13*fleafScale;
	float d23 = m23*fleafScale;
	float d33 = m33*fleafScale;

	pzx=ox;pzy=oy;pzz=oz;
	for(int z=0;z<8;z++,pzx+=d13,pzy+=d23,pzz+=d33){
		pyx=pzx;pyy=pzy;pyz=pzz;
		for(int y=0;y<8;y++,pyx+=d12,pyy+=d22,pyz+=d32){

			pxx=pyx;pxy=pyy;pxz=pyz;

			for(int x=0;x<2;x++,pxx+=d11,pxy+=d21,pxz+=d31){
				volumetype idx = threadOffset; threadOffset+=4;
				__m128 pxx_SSE =  _mm_add_ps(_mm_set1_ps(pxx),d11_SSE);
				__m128 pxy_SSE =  _mm_add_ps(_mm_set1_ps(pxy),d21_SSE);
				__m128 pxz_SSE =  _mm_add_ps(_mm_set1_ps(pxz),d31_SSE);

				// float length = sqrtf(pxx*pxx+pxy*pxy+pxz*pxz);
				__m128 length = _mm_sqrt_ps(_mm_add_ps(_mm_add_ps(
						_mm_mul_ps(pxx_SSE,pxx_SSE),_mm_mul_ps(pxy_SSE,pxy_SSE)),_mm_mul_ps(pxz_SSE,pxz_SSE)));

				__m128 reciprocal = _mm_rcp_ps(pxz_SSE);
				// int imx = (int)floor(pxx/pxz*fx+cx);
				__m128i imx = _mm_cvtps_epi32(_mm_add_ps(_mm_set1_ps(cx),_mm_mul_ps(_mm_set1_ps(fx),_mm_mul_ps(pxx_SSE,reciprocal))));
				// int imy = (int)floor(pxy/pxz*fy+cy);
				__m128i imy = _mm_cvtps_epi32(_mm_add_ps(_mm_set1_ps(cy),_mm_mul_ps(_mm_set1_ps(fy),_mm_mul_ps(pxy_SSE,reciprocal))));


				// int imageIndex = imy*(imy>=0 && imy<imageHeight)*imageWidth + imx*(imx>=0 && imx<imageWidth);
				ALIGNED int imageIndex[4];
				 _mm_store_si128((__m128i*)imageIndex,
					 _mm_add_epi32(
						 _mm_mullo_epi32(
							 _mm_set1_epi32(imageWidth),
								 _mm_max_epi32(_mm_min_epi32(imy,_mm_set1_epi32(imageHeight-1)),_mm_setzero_si128())),
								 _mm_max_epi32(_mm_min_epi32(imx,_mm_set1_epi32(imageWidth-1)),_mm_setzero_si128())));

				ALIGNED float h4[4] = {
						depth[imageIndex[0]],
						depth[imageIndex[1]],
						depth[imageIndex[2]],
						depth[imageIndex[3]]};


				ALIGNED float rInc4[4] = {
						(float)(red[imageIndex[0]]),
						(float)(red[imageIndex[1]]),
						(float)(red[imageIndex[2]]),
						(float)(red[imageIndex[3]])};
				ALIGNED float gInc4[4] = {
						(float)(green[imageIndex[0]]),
						(float)(green[imageIndex[1]]),
						(float)(green[imageIndex[2]]),
						(float)(green[imageIndex[3]])};
				ALIGNED float bInc4[4] = {
						(float)(blue[imageIndex[0]]),
						(float)(blue[imageIndex[1]]),
						(float)(blue[imageIndex[2]]),
						(float)(blue[imageIndex[3]])};

				ALIGNED float rAcc4[4] = {
						(float)(_color[idx+0].x),
						(float)(_color[idx+1].x),
						(float)(_color[idx+2].x),
						(float)(_color[idx+3].x)};
				ALIGNED float gAcc4[4] = {
						(float)(_color[idx+0].y),
						(float)(_color[idx+1].y),
						(float)(_color[idx+2].y),
						(float)(_color[idx+3].y)};
				ALIGNED float bAcc4[4] = {
						(float)(_color[idx+0].z),
						(float)(_color[idx+1].z),
						(float)(_color[idx+2].z),
						(float)(_color[idx+3].z)};

				// float dInc = length - length/pxz*h;
				__m128 dInc = _mm_sub_ps(length,_mm_mul_ps(_mm_mul_ps(length,reciprocal),_mm_load_ps(h4)));
				__m128 rInc = _mm_mul_ps(_mm_load_ps(rInc4),_mm_set1_ps(COLOR_MULTIPLICATOR));
				__m128 gInc = _mm_mul_ps(_mm_load_ps(gInc4),_mm_set1_ps(COLOR_MULTIPLICATOR));
				__m128 bInc = _mm_mul_ps(_mm_load_ps(bInc4),_mm_set1_ps(COLOR_MULTIPLICATOR));

				//FIXME
				__m128 wAcc = _mm_load_ps(_weights+idx);
				__m128 dAcc = _mm_load_ps(_distance+idx);
				__m128 rAcc = _mm_load_ps(rAcc4);
				__m128 gAcc = _mm_load_ps(gAcc4);
				__m128 bAcc = _mm_load_ps(bAcc4);

				// (pxz > 0.0f && imx>=0 && imy>=0 && imx<imageWidth && imy<imageHeight && isfinite(dInc))*4294967295
				__m128 mask =
						_mm_and_ps(
							_mm_cmpgt_ps(pxz_SSE,_mm_setzero_ps()),
							_mm_and_ps(
								_mm_castsi128_ps(_mm_and_si128(_mm_cmplt_epi32(imy,_mm_set1_epi32(imageHeight)),_mm_cmpgt_epi32(imy,_mm_set1_epi32(-1)))),
								_mm_and_ps(
									_mm_castsi128_ps(_mm_and_si128(_mm_cmplt_epi32(imx,_mm_set1_epi32(imageWidth)),_mm_cmpgt_epi32(imx,_mm_set1_epi32(-1)))),
									_mm_cmpord_ps(dInc,dInc))));



				// (float)(dInc<DISTANCEWEIGHTEPSILON)
				__m128 maskFront =
						_mm_and_ps(
								_mm_set1_ps(1.0f),
								_mm_cmplt_ps(dInc,_mm_set1_ps(DISTANCEWEIGHTEPSILON)));

				// (float)(dInc>=DISTANCEWEIGHTEPSILON && dInc<thresholdWeight)
				__m128 maskBack =
						_mm_and_ps(
								_mm_set1_ps(1.0f),
								_mm_and_ps(
										_mm_cmpge_ps(dInc,_mm_set1_ps(DISTANCEWEIGHTEPSILON)),
										_mm_cmplt_ps(dInc,thresholdWeight)));

				// (thresholdWeight-dInc)/(thresholdWeight-DISTANCEWEIGHTEPSILON)
				__m128 weightFall =
						_mm_mul_ps(
								_mm_sub_ps(thresholdWeight,dInc),
								_mm_rcp_ps(
										_mm_sub_ps(thresholdWeight,_mm_set1_ps(DISTANCEWEIGHTEPSILON))));

				__m128 wInc =
						_mm_and_ps(
								_mm_add_ps(maskFront,_mm_mul_ps(weightFall,maskBack)),
								mask);


				__m128 wNew = _mm_add_ps(wAcc,wInc);

				// float factor = 1.0f/((float)(wPrev+wInc)+(float)(wPrev+wInc==0));
				__m128 factor = _mm_rcp_ps(
						_mm_add_ps(
								wNew,
								_mm_and_ps(_mm_set1_ps(1.0f),_mm_cmple_ps(wNew,_mm_setzero_ps()))));

				//_distance[idx] = (dPrev*wPrev +
				//		std::max(-distanceThreshold*leafScale,std::min(distanceThreshold*leafScale,dInc))*wInc)*factor;
				_mm_store_ps(_distance+idx,
						_mm_mul_ps(
								_mm_add_ps(
										_mm_mul_ps(dAcc,wAcc),
										_mm_and_ps(mask,
											_mm_mul_ps(
													_mm_max_ps(
															_mm_mul_ps(_mm_set1_ps(-1.0f),thresholdDistance),
															_mm_min_ps(thresholdDistance,dInc)),
													wInc))),
								factor));

				// _weights[idx] = wPrev+wInc;
				_mm_store_ps(_weights+idx,wNew);

				_mm_store_ps(rAcc4,_mm_mul_ps(_mm_add_ps(_mm_mul_ps(rAcc,wAcc),_mm_mul_ps(rInc,wInc)),factor));
				_mm_store_ps(gAcc4,_mm_mul_ps(_mm_add_ps(_mm_mul_ps(gAcc,wAcc),_mm_mul_ps(gInc,wInc)),factor));
				_mm_store_ps(bAcc4,_mm_mul_ps(_mm_add_ps(_mm_mul_ps(bAcc,wAcc),_mm_mul_ps(bInc,wInc)),factor));

				_color[idx+0].x = rAcc4[0];
				_color[idx+1].x = rAcc4[1];
				_color[idx+2].x = rAcc4[2];
				_color[idx+3].x = rAcc4[3];

				_color[idx+0].y = gAcc4[0];
				_color[idx+1].y = gAcc4[1];
				_color[idx+2].y = gAcc4[2];
				_color[idx+3].y = gAcc4[3];

				_color[idx+0].z = bAcc4[0];
				_color[idx+1].z = bAcc4[1];
				_color[idx+2].z = bAcc4[2];
				_color[idx+3].z = bAcc4[3];
			}
		}
	}
}



void update8AddLoopSSESingleInteger
(
		const ushort *depth, float scaling, float maxcamdistance, const uchar *rgb,
		int imageWidth, int imageHeight,
		float &m11, float &m12, float &m13, float &m14,
		float &m21, float &m22, float &m23, float &m24,
		float &m31, float &m32, float &m33, float &m34,
		float &fx, float &fy, float &cx, float &cy,
		float &scale, float &distanceThreshold,
		volumetype &brickIdx,
		sidetype3 &o,
		sidetype &leafScale,
		float *_distance,
		weighttype *_weights,
		colortype3 *_color
)
{
	volumetype threadOffset = brickIdx*512;
	float fleafScale = (float)(leafScale)*scale;
	float ox = (m11*o.x+m12*o.y+m13*o.z)*scale + m14;
	float oy = (m21*o.x+m22*o.y+m23*o.z)*scale + m24;
	float oz = (m31*o.x+m32*o.y+m33*o.z)*scale + m34;
	float pxx, pxy, pxz, pyx, pyy, pyz, pzx, pzy, pzz;


	float d11 = m11*fleafScale;
	float d21 = m21*fleafScale;
	float d31 = m31*fleafScale;

	__m128 d11_SSE = _mm_setr_ps(0.0f, d11, 2.0f*d11, 3.0f*d11);
	__m128 d21_SSE = _mm_setr_ps(0.0f, d21, 2.0f*d21, 3.0f*d21);
	__m128 d31_SSE = _mm_setr_ps(0.0f, d31, 2.0f*d31, 3.0f*d31);
  __m128 thresholdDistance = _mm_set1_ps(distanceThreshold*leafScale);
  __m128 thresholdWeight = _mm_set1_ps(WEIGHT_FACTOR*distanceThreshold*leafScale);
	d11*=4.0f; d21*=4.0f; d31*=4.0f;

	float d12 = m12*fleafScale;
	float d22 = m22*fleafScale;
	float d32 = m32*fleafScale;

	float d13 = m13*fleafScale;
	float d23 = m23*fleafScale;
	float d33 = m33*fleafScale;

	pzx=ox;pzy=oy;pzz=oz;
	for(int z=0;z<8;z++,pzx+=d13,pzy+=d23,pzz+=d33){
		pyx=pzx;pyy=pzy;pyz=pzz;
		for(int y=0;y<8;y++,pyx+=d12,pyy+=d22,pyz+=d32){

			pxx=pyx;pxy=pyy;pxz=pyz;

			for(int x=0;x<2;x++,pxx+=d11,pxy+=d21,pxz+=d31){
				volumetype idx = threadOffset; threadOffset+=4;
				__m128 pxx_SSE =  _mm_add_ps(_mm_set1_ps(pxx),d11_SSE);
				__m128 pxy_SSE =  _mm_add_ps(_mm_set1_ps(pxy),d21_SSE);
				__m128 pxz_SSE =  _mm_add_ps(_mm_set1_ps(pxz),d31_SSE);

				// float length = sqrtf(pxx*pxx+pxy*pxy+pxz*pxz);
				__m128 length = _mm_sqrt_ps(_mm_add_ps(_mm_add_ps(
						_mm_mul_ps(pxx_SSE,pxx_SSE),_mm_mul_ps(pxy_SSE,pxy_SSE)),_mm_mul_ps(pxz_SSE,pxz_SSE)));

				__m128 reciprocal = _mm_rcp_ps(pxz_SSE);
				// int imx = (int)floor(pxx/pxz*fx+cx);
				__m128i imx = _mm_cvtps_epi32(_mm_add_ps(_mm_set1_ps(cx),_mm_mul_ps(_mm_set1_ps(fx),_mm_mul_ps(pxx_SSE,reciprocal))));
				// int imy = (int)floor(pxy/pxz*fy+cy);
				__m128i imy = _mm_cvtps_epi32(_mm_add_ps(_mm_set1_ps(cy),_mm_mul_ps(_mm_set1_ps(fy),_mm_mul_ps(pxy_SSE,reciprocal))));


				// int imageIndex = imy*(imy>=0 && imy<imageHeight)*imageWidth + imx*(imx>=0 && imx<imageWidth);
				ALIGNED int imageIndex[4];
				 _mm_store_si128((__m128i*)imageIndex,
					 _mm_add_epi32(
						 _mm_mullo_epi32(
							 _mm_set1_epi32(imageWidth),
								 _mm_max_epi32(_mm_min_epi32(imy,_mm_set1_epi32(imageHeight-1)),_mm_setzero_si128())),
								 _mm_max_epi32(_mm_min_epi32(imx,_mm_set1_epi32(imageWidth-1)),_mm_setzero_si128())));

				ALIGNED float h4[4] = {
						(float)(depth[imageIndex[0]]),
						(float)(depth[imageIndex[1]]),
						(float)(depth[imageIndex[2]]),
						(float)(depth[imageIndex[3]])};


				ALIGNED float rInc4[4] = {
						(float)(rgb[imageIndex[0]*3+2]),
						(float)(rgb[imageIndex[1]*3+2]),
						(float)(rgb[imageIndex[2]*3+2]),
						(float)(rgb[imageIndex[3]*3+2])};
				ALIGNED float gInc4[4] = {
						(float)(rgb[imageIndex[0]*3+1]),
						(float)(rgb[imageIndex[1]*3+1]),
						(float)(rgb[imageIndex[2]*3+1]),
						(float)(rgb[imageIndex[3]*3+1])};
				ALIGNED float bInc4[4] = {
						(float)(rgb[imageIndex[0]*3+0]),
						(float)(rgb[imageIndex[1]*3+0]),
						(float)(rgb[imageIndex[2]*3+0]),
						(float)(rgb[imageIndex[3]*3+0])};

				ALIGNED float rAcc4[4] = {
						(float)(_color[idx+0].x),
						(float)(_color[idx+1].x),
						(float)(_color[idx+2].x),
						(float)(_color[idx+3].x)};
				ALIGNED float gAcc4[4] = {
						(float)(_color[idx+0].y),
						(float)(_color[idx+1].y),
						(float)(_color[idx+2].y),
						(float)(_color[idx+3].y)};
				ALIGNED float bAcc4[4] = {
						(float)(_color[idx+0].z),
						(float)(_color[idx+1].z),
						(float)(_color[idx+2].z),
						(float)(_color[idx+3].z)};

				// float dInc = length - length/pxz*h;
				__m128 h4SSE = _mm_mul_ps(_mm_set1_ps(scaling),_mm_load_ps(h4));
				__m128 dInc = _mm_sub_ps(length,_mm_mul_ps(_mm_mul_ps(length,reciprocal),h4SSE));
				__m128 rInc = _mm_mul_ps(_mm_load_ps(rInc4),_mm_set1_ps(COLOR_MULTIPLICATOR));
				__m128 gInc = _mm_mul_ps(_mm_load_ps(gInc4),_mm_set1_ps(COLOR_MULTIPLICATOR));
				__m128 bInc = _mm_mul_ps(_mm_load_ps(bInc4),_mm_set1_ps(COLOR_MULTIPLICATOR));

				//FIXME
				__m128 wAcc = _mm_load_ps(_weights+idx);
				__m128 dAcc = _mm_load_ps(_distance+idx);
				__m128 rAcc = _mm_load_ps(rAcc4);
				__m128 gAcc = _mm_load_ps(gAcc4);
				__m128 bAcc = _mm_load_ps(bAcc4);

				// (pxz > 0.0f && imx>=0 && imy>=0 && imx<imageWidth && imy<imageHeight && isfinite(dInc))*4294967295
				__m128 mask =
						_mm_and_ps(
							_mm_cmpgt_ps(pxz_SSE,_mm_setzero_ps()),
							_mm_and_ps(
								_mm_castsi128_ps(_mm_and_si128(_mm_cmplt_epi32(imy,_mm_set1_epi32(imageHeight)),_mm_cmpgt_epi32(imy,_mm_set1_epi32(-1)))),
								_mm_and_ps(
									_mm_castsi128_ps(_mm_and_si128(_mm_cmplt_epi32(imx,_mm_set1_epi32(imageWidth)),_mm_cmpgt_epi32(imx,_mm_set1_epi32(-1)))),
									_mm_cmplt_ps(h4SSE,_mm_set1_ps(maxcamdistance)))));



				// (float)(dInc<DISTANCEWEIGHTEPSILON)
				__m128 maskFront =
						_mm_and_ps(
								_mm_set1_ps(1.0f),
								_mm_cmplt_ps(dInc,_mm_set1_ps(DISTANCEWEIGHTEPSILON)));

				// (float)(dInc>=DISTANCEWEIGHTEPSILON && dInc<thresholdWeight)
				__m128 maskBack =
						_mm_and_ps(
								_mm_set1_ps(1.0f),
								_mm_and_ps(
										_mm_cmpge_ps(dInc,_mm_set1_ps(DISTANCEWEIGHTEPSILON)),
										_mm_cmplt_ps(dInc,thresholdWeight)));

				// (thresholdWeight-dInc)/(thresholdWeight-DISTANCEWEIGHTEPSILON)
				__m128 weightFall =
						_mm_mul_ps(
								_mm_sub_ps(thresholdWeight,dInc),
								_mm_rcp_ps(
										_mm_sub_ps(thresholdWeight,_mm_set1_ps(DISTANCEWEIGHTEPSILON))));

				__m128 wInc =
						_mm_and_ps(
								_mm_add_ps(maskFront,_mm_mul_ps(weightFall,maskBack)),
								mask);


				__m128 wNew = _mm_add_ps(wAcc,wInc);

				// float factor = 1.0f/((float)(wPrev+wInc)+(float)(wPrev+wInc==0));
				__m128 factor = _mm_rcp_ps(
						_mm_add_ps(
								wNew,
								_mm_and_ps(_mm_set1_ps(1.0f),_mm_cmple_ps(wNew,_mm_setzero_ps()))));

				//_distance[idx] = (dPrev*wPrev +
				//		std::max(-distanceThreshold*leafScale,std::min(distanceThreshold*leafScale,dInc))*wInc)*factor;
				_mm_store_ps(_distance+idx,
						_mm_mul_ps(
								_mm_add_ps(
										_mm_mul_ps(dAcc,wAcc),
										_mm_and_ps(mask,
											_mm_mul_ps(
													_mm_max_ps(
															_mm_mul_ps(_mm_set1_ps(-1.0f),thresholdDistance),
															_mm_min_ps(thresholdDistance,dInc)),
													wInc))),
								factor));

				// _weights[idx] = wPrev+wInc;
				_mm_store_ps(_weights+idx,wNew);

//				_mm_store_ps(rAcc4,_mm_mul_ps(_mm_add_ps(_mm_mul_ps(rAcc,wAcc),_mm_mul_ps(rInc,wInc)),factor));
//				_mm_store_ps(gAcc4,_mm_mul_ps(_mm_add_ps(_mm_mul_ps(gAcc,wAcc),_mm_mul_ps(gInc,wInc)),factor));
//				_mm_store_ps(bAcc4,_mm_mul_ps(_mm_add_ps(_mm_mul_ps(bAcc,wAcc),_mm_mul_ps(bInc,wInc)),factor));
				//New: Thresholding to prevent Overexposure-Artefacts
				__m128 col_max = _mm_set1_ps(255.0f*COLOR_MULTIPLICATOR);
				_mm_store_ps(rAcc4,_mm_min_ps(_mm_mul_ps(_mm_add_ps(_mm_mul_ps(rAcc,wAcc),_mm_mul_ps(rInc,wInc)),factor),col_max));
				_mm_store_ps(gAcc4,_mm_min_ps(_mm_mul_ps(_mm_add_ps(_mm_mul_ps(gAcc,wAcc),_mm_mul_ps(gInc,wInc)),factor),col_max));
				_mm_store_ps(bAcc4,_mm_min_ps(_mm_mul_ps(_mm_add_ps(_mm_mul_ps(bAcc,wAcc),_mm_mul_ps(bInc,wInc)),factor),col_max));

				_color[idx+0].x = rAcc4[0];
				_color[idx+1].x = rAcc4[1];
				_color[idx+2].x = rAcc4[2];
				_color[idx+3].x = rAcc4[3];

				_color[idx+0].y = gAcc4[0];
				_color[idx+1].y = gAcc4[1];
				_color[idx+2].y = gAcc4[2];
				_color[idx+3].y = gAcc4[3];

				_color[idx+0].z = bAcc4[0];
				_color[idx+1].z = bAcc4[1];
				_color[idx+2].z = bAcc4[2];
				_color[idx+3].z = bAcc4[3];
			}
		}
	}
}


#ifdef OWNAVX
#pragma message "Compiling AVX and AVX2 voxel update function"
void update8AddLoopAVXSingleInteger
(
		const ushort *depth, float scaling, float maxcamdistance, const uchar *rgb,
		int imageWidth, int imageHeight,
		float &m11, float &m12, float &m13, float &m14,
		float &m21, float &m22, float &m23, float &m24,
		float &m31, float &m32, float &m33, float &m34,
		float &fx, float &fy, float &cx, float &cy,
		float &scale, float &distanceThreshold,
		volumetype &brickIdx,
		sidetype3 &o,
		sidetype &leafScale,
		float *_distance,
		weighttype *_weights,
		colortype3 *_color
)
{
	volumetype threadOffset = brickIdx*512;
	float fleafScale = (float)(leafScale)*scale;
	float ox = (m11*o.x+m12*o.y+m13*o.z)*scale + m14;
	float oy = (m21*o.x+m22*o.y+m23*o.z)*scale + m24;
	float oz = (m31*o.x+m32*o.y+m33*o.z)*scale + m34;
//	float pxx, pxy, pxz, pyx, pyy, pyz, pzx, pzy, pzz;
	float pyx, pyy, pyz, pzx, pzy, pzz;


	float d11 = m11*fleafScale;
	float d21 = m21*fleafScale;
	float d31 = m31*fleafScale;

	__m256 d11_AVX = _mm256_setr_ps(0.0f, d11, 2.0f*d11, 3.0f*d11, 4.0f*d11, 5.0f*d11, 6.0f*d11, 7.0f*d11);
	__m256 d21_AVX = _mm256_setr_ps(0.0f, d21, 2.0f*d21, 3.0f*d21, 4.0f*d21, 5.0f*d21, 6.0f*d21, 7.0f*d21);
	__m256 d31_AVX = _mm256_setr_ps(0.0f, d31, 2.0f*d31, 3.0f*d31, 4.0f*d31, 5.0f*d31, 6.0f*d31, 7.0f*d31);
  __m256 thresholdDistance = _mm256_set1_ps(distanceThreshold*leafScale);
  __m256 thresholdWeight = _mm256_set1_ps(WEIGHT_FACTOR*distanceThreshold*leafScale);

	float d12 = m12*fleafScale;
	float d22 = m22*fleafScale;
	float d32 = m32*fleafScale;

	float d13 = m13*fleafScale;
	float d23 = m23*fleafScale;
	float d33 = m33*fleafScale;

	pzx=ox;pzy=oy;pzz=oz;
	for(int z=0;z<8;z++,pzx+=d13,pzy+=d23,pzz+=d33){
		pyx=pzx;pyy=pzy;pyz=pzz;
		for(int y=0;y<8;y++,pyx+=d12,pyy+=d22,pyz+=d32){


			volumetype idx = threadOffset; threadOffset+=8;
			__m256 pxx_AVX =  _mm256_add_ps(_mm256_set1_ps(pyx),d11_AVX);
			__m256 pxy_AVX =  _mm256_add_ps(_mm256_set1_ps(pyy),d21_AVX);
			__m256 pxz_AVX =  _mm256_add_ps(_mm256_set1_ps(pyz),d31_AVX);

			// float length = sqrtf(pxx*pxx+pxy*pxy+pxz*pxz);
			__m256 length = _mm256_sqrt_ps(_mm256_add_ps(_mm256_add_ps(
					_mm256_mul_ps(pxx_AVX,pxx_AVX),_mm256_mul_ps(pxy_AVX,pxy_AVX)),_mm256_mul_ps(pxz_AVX,pxz_AVX)));

			__m256 reciprocal = _mm256_rcp_ps(pxz_AVX);
			// int imx = (int)floor(pxx/pxz*fx+cx);
			__m256i imx = _mm256_cvtps_epi32(_mm256_add_ps(_mm256_set1_ps(cx),_mm256_mul_ps(_mm256_set1_ps(fx),_mm256_mul_ps(pxx_AVX,reciprocal))));
			// int imy = (int)floor(pxy/pxz*fy+cy);
			__m256i imy = _mm256_cvtps_epi32(_mm256_add_ps(_mm256_set1_ps(cy),_mm256_mul_ps(_mm256_set1_ps(fy),_mm256_mul_ps(pxy_AVX,reciprocal))));


			// int imageIndex = imy*(imy>=0 && imy<imageHeight)*imageWidth + imx*(imx>=0 && imx<imageWidth);
			ALIGNED int imageIndex[8];
			 _mm256_store_si256((__m256i*)imageIndex,
				 _mm256_add_epi32(
					 _mm256_mullo_epi32(
						 _mm256_set1_epi32(imageWidth),
							 _mm256_max_epi32(_mm256_min_epi32(imy,_mm256_set1_epi32(imageHeight-1)),_mm256_setzero_si256())),
								 _mm256_max_epi32(_mm256_min_epi32(imx,_mm256_set1_epi32(imageWidth-1)),_mm256_setzero_si256())));

			ALIGNED float h8[8] = {
					(float)(depth[imageIndex[0]]),
					(float)(depth[imageIndex[1]]),
					(float)(depth[imageIndex[2]]),
					(float)(depth[imageIndex[3]]),
					(float)(depth[imageIndex[4]]),
					(float)(depth[imageIndex[5]]),
					(float)(depth[imageIndex[6]]),
					(float)(depth[imageIndex[7]])};


			ALIGNED float rInc8[8] = {
					(float)(rgb[imageIndex[0]*3+2]),
					(float)(rgb[imageIndex[1]*3+2]),
					(float)(rgb[imageIndex[2]*3+2]),
					(float)(rgb[imageIndex[3]*3+2]),
					(float)(rgb[imageIndex[4]*3+2]),
					(float)(rgb[imageIndex[5]*3+2]),
					(float)(rgb[imageIndex[6]*3+2]),
					(float)(rgb[imageIndex[7]*3+2])};
			ALIGNED float gInc8[8] = {
					(float)(rgb[imageIndex[0]*3+1]),
					(float)(rgb[imageIndex[1]*3+1]),
					(float)(rgb[imageIndex[2]*3+1]),
					(float)(rgb[imageIndex[3]*3+1]),
					(float)(rgb[imageIndex[4]*3+1]),
					(float)(rgb[imageIndex[5]*3+1]),
					(float)(rgb[imageIndex[6]*3+1]),
					(float)(rgb[imageIndex[7]*3+1])};
			ALIGNED float bInc8[8] = {
					(float)(rgb[imageIndex[0]*3+0]),
					(float)(rgb[imageIndex[1]*3+0]),
					(float)(rgb[imageIndex[2]*3+0]),
					(float)(rgb[imageIndex[3]*3+0]),
					(float)(rgb[imageIndex[4]*3+0]),
					(float)(rgb[imageIndex[5]*3+0]),
					(float)(rgb[imageIndex[6]*3+0]),
					(float)(rgb[imageIndex[7]*3+0])};

			ALIGNED float rAcc8[8] = {
					(float)(_color[idx+0].x),
					(float)(_color[idx+1].x),
					(float)(_color[idx+2].x),
					(float)(_color[idx+3].x),
					(float)(_color[idx+4].x),
					(float)(_color[idx+5].x),
					(float)(_color[idx+6].x),
					(float)(_color[idx+7].x)};
			ALIGNED float gAcc8[8] = {
					(float)(_color[idx+0].y),
					(float)(_color[idx+1].y),
					(float)(_color[idx+2].y),
					(float)(_color[idx+3].y),
					(float)(_color[idx+4].y),
					(float)(_color[idx+5].y),
					(float)(_color[idx+6].y),
					(float)(_color[idx+7].y)};
			ALIGNED float bAcc8[8] = {
					(float)(_color[idx+0].z),
					(float)(_color[idx+1].z),
					(float)(_color[idx+2].z),
					(float)(_color[idx+3].z),
					(float)(_color[idx+4].z),
					(float)(_color[idx+5].z),
					(float)(_color[idx+6].z),
					(float)(_color[idx+7].z)};

			// float dInc = length - length/pxz*h;
			__m256 h8AVX = _mm256_mul_ps(_mm256_set1_ps(scaling),_mm256_load_ps(h8));
			__m256 dInc = _mm256_sub_ps(length,_mm256_mul_ps(_mm256_mul_ps(length,reciprocal),h8AVX));
			__m256 rInc = _mm256_mul_ps(_mm256_load_ps(rInc8),_mm256_set1_ps(COLOR_MULTIPLICATOR));
			__m256 gInc = _mm256_mul_ps(_mm256_load_ps(gInc8),_mm256_set1_ps(COLOR_MULTIPLICATOR));
			__m256 bInc = _mm256_mul_ps(_mm256_load_ps(bInc8),_mm256_set1_ps(COLOR_MULTIPLICATOR));

			//FIXME
			__m256 wAcc = _mm256_load_ps(_weights+idx);
			__m256 dAcc = _mm256_load_ps(_distance+idx);
			__m256 rAcc = _mm256_load_ps(rAcc8);
			__m256 gAcc = _mm256_load_ps(gAcc8);
			__m256 bAcc = _mm256_load_ps(bAcc8);

			// (pxz > 0.0f && imx>=0 && imy>=0 && imx<imageWidth && imy<imageHeight && isfinite(dInc))*4294967295
			__m256 mask =
					_mm256_and_ps(
						_mm256_cmp_ps(pxz_AVX,_mm256_setzero_ps(),_CMP_GT_OS),
						_mm256_and_ps(
							_mm256_castsi256_ps(_mm256_and_si256(_mm256_cmpgt_epi32(_mm256_set1_epi32(imageHeight),imy),_mm256_cmpgt_epi32(imy,_mm256_set1_epi32(-1)))),
							_mm256_and_ps(
								_mm256_castsi256_ps(_mm256_and_si256(_mm256_cmpgt_epi32(_mm256_set1_epi32(imageWidth),imx),_mm256_cmpgt_epi32(imx,_mm256_set1_epi32(-1)))),
								_mm256_cmp_ps(h8AVX,_mm256_set1_ps(maxcamdistance),_CMP_LT_OS))));



			// (float)(dInc<DISTANCEWEIGHTEPSILON)
			__m256 maskFront =
					_mm256_and_ps(
							_mm256_set1_ps(1.0f),
							_mm256_cmp_ps(dInc,_mm256_set1_ps(DISTANCEWEIGHTEPSILON),_CMP_LT_OS));

			// (float)(dInc>=DISTANCEWEIGHTEPSILON && dInc<thresholdWeight)
			__m256 maskBack =
					_mm256_and_ps(
							_mm256_set1_ps(1.0f),
							_mm256_and_ps(
									_mm256_cmp_ps(dInc,_mm256_set1_ps(DISTANCEWEIGHTEPSILON),_CMP_GE_OS),
									_mm256_cmp_ps(dInc,thresholdWeight,_CMP_LT_OS)));

			// (thresholdWeight-dInc)/(thresholdWeight-DISTANCEWEIGHTEPSILON)
			__m256 weightFall =
					_mm256_mul_ps(
							_mm256_sub_ps(thresholdWeight,dInc),
							_mm256_rcp_ps(
									_mm256_sub_ps(thresholdWeight,_mm256_set1_ps(DISTANCEWEIGHTEPSILON))));

			__m256 wInc =
					_mm256_and_ps(
							_mm256_add_ps(maskFront,_mm256_mul_ps(weightFall,maskBack)),
							mask);


//			__m128 wNew = _mm_add_ps(wAcc,wInc);
			__m256 wNew = _mm256_add_ps(wAcc,wInc);

			// float factor = 1.0f/((float)(wPrev+wInc)+(float)(wPrev+wInc==0));
			__m256 factor = _mm256_rcp_ps(
					_mm256_add_ps(
							wNew,
							_mm256_and_ps(_mm256_set1_ps(1.0f),_mm256_cmp_ps(wNew,_mm256_setzero_ps(),_CMP_LE_OS))));

			//_distance[idx] = (dPrev*wPrev +
			//		std::max(-distanceThreshold*leafScale,std::min(distanceThreshold*leafScale,dInc))*wInc)*factor;
			_mm256_store_ps(_distance+idx,
					_mm256_mul_ps(
							_mm256_add_ps(
									_mm256_mul_ps(dAcc,wAcc),
									_mm256_and_ps(mask,
										_mm256_mul_ps(
												_mm256_max_ps(
														_mm256_mul_ps(_mm256_set1_ps(-1.0f),thresholdDistance),
														_mm256_min_ps(thresholdDistance,dInc)),
												wInc))),
							factor));

			// _weights[idx] = wPrev+wInc;
			_mm256_store_ps(_weights+idx,wNew);

			//New: Thresholding to prevent Overexposure-Artefacts
			__m256 col_max = _mm256_set1_ps(255.0f*COLOR_MULTIPLICATOR);
			_mm256_store_ps(rAcc8,_mm256_min_ps(_mm256_mul_ps(_mm256_add_ps(_mm256_mul_ps(rAcc,wAcc),_mm256_mul_ps(rInc,wInc)),factor),col_max));
			_mm256_store_ps(gAcc8,_mm256_min_ps(_mm256_mul_ps(_mm256_add_ps(_mm256_mul_ps(gAcc,wAcc),_mm256_mul_ps(gInc,wInc)),factor),col_max));
			_mm256_store_ps(bAcc8,_mm256_min_ps(_mm256_mul_ps(_mm256_add_ps(_mm256_mul_ps(bAcc,wAcc),_mm256_mul_ps(bInc,wInc)),factor),col_max));


			_color[idx+0].x = rAcc8[0];
			_color[idx+1].x = rAcc8[1];
			_color[idx+2].x = rAcc8[2];
			_color[idx+3].x = rAcc8[3];
			_color[idx+4].x = rAcc8[4];
			_color[idx+5].x = rAcc8[5];
			_color[idx+6].x = rAcc8[6];
			_color[idx+7].x = rAcc8[7];

			_color[idx+0].y = gAcc8[0];
			_color[idx+1].y = gAcc8[1];
			_color[idx+2].y = gAcc8[2];
			_color[idx+3].y = gAcc8[3];
			_color[idx+4].y = gAcc8[4];
			_color[idx+5].y = gAcc8[5];
			_color[idx+6].y = gAcc8[6];
			_color[idx+7].y = gAcc8[7];

			_color[idx+0].z = bAcc8[0];
			_color[idx+1].z = bAcc8[1];
			_color[idx+2].z = bAcc8[2];
			_color[idx+3].z = bAcc8[3];
			_color[idx+4].z = bAcc8[4];
			_color[idx+5].z = bAcc8[5];
			_color[idx+6].z = bAcc8[6];
			_color[idx+7].z = bAcc8[7];

		}
	}
}

#endif
















typedef struct SDFUpdateParameterFloat_ {
	const float *depth;
	const uchar *red;
	const uchar *green;
	const uchar *blue;
	int imageWidth; int imageHeight;
	float m11; float m12; float m13; float m14;
	float m21; float m22; float m23; float m24;
	float m31; float m32; float m33; float m34;
	float fx; float fy; float cx; float cy;
	float scale; float distanceThreshold;
	volumetype *_leafNumber; sidetype3 *_leafPos; sidetype *_leafScale;
	float *_distance; weighttype *_weights; colortype3 *_color;
	sidetype brickLength;
	SDFUpdateParameterFloat_(
			const float *depth,
			const uchar *red,
			const uchar *green,
			const uchar *blue,
			int imageWidth, int imageHeight,
			float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float fx, float fy, float cx, float cy,
			float scale, float distanceThreshold,
			volumetype *_leafNumber, sidetype3 *_leafPos, sidetype *_leafScale,
			float *_distance, weighttype *_weights, colortype3 *_color,
			sidetype brickLength):
			depth(depth), red(red), green(green), blue(blue),
			imageWidth(imageWidth), imageHeight(imageHeight),
			m11(m11), m12(m12), m13(m13), m14(m14),
			m21(m21), m22(m22), m23(m23), m24(m24),
			m31(m31), m32(m32), m33(m33), m34(m34),
			fx(fx), fy(fy), cx(cx), cy(cy),
			scale(scale), distanceThreshold(distanceThreshold),
			_leafNumber(_leafNumber), _leafPos(_leafPos), _leafScale(_leafScale),
			_distance(_distance), _weights(_weights), _color(_color),
			brickLength(brickLength)
			{}
} SDFUpdateParameterFloat;

typedef struct SDFUpdateParameterInteger_ {
	const ushort *depth; float scaling; float maxcamdistance;
	const uchar *rgb;
	int imageWidth; int imageHeight;
	float m11; float m12; float m13; float m14;
	float m21; float m22; float m23; float m24;
	float m31; float m32; float m33; float m34;
	float fx; float fy; float cx; float cy;
	float scale; float distanceThreshold;
	volumetype *_leafNumber; sidetype3 *_leafPos; sidetype *_leafScale;
	float *_distance; weighttype *_weights; colortype3 *_color;
	sidetype brickLength;
	SDFUpdateParameterInteger_(
			const ushort *depth, float scaling, float maxcamdistance,
			const uchar *rgb,
			int imageWidth, int imageHeight,
			float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float fx, float fy, float cx, float cy,
			float scale, float distanceThreshold,
			volumetype *_leafNumber, sidetype3 *_leafPos, sidetype *_leafScale,
			float *_distance, weighttype *_weights, colortype3 *_color,
			sidetype brickLength):
			depth(depth), scaling(scaling), maxcamdistance(maxcamdistance), rgb(rgb),
			imageWidth(imageWidth), imageHeight(imageHeight),
			m11(m11), m12(m12), m13(m13), m14(m14),
			m21(m21), m22(m22), m23(m23), m24(m24),
			m31(m31), m32(m32), m33(m33), m34(m34),
			fx(fx), fy(fy), cx(cx), cy(cy),
			scale(scale), distanceThreshold(distanceThreshold),
			_leafNumber(_leafNumber), _leafPos(_leafPos), _leafScale(_leafScale),
			_distance(_distance), _weights(_weights), _color(_color),
			brickLength(brickLength)
			{}
} SDFUpdateParameterInteger;





void updateWrapperFloat
(
		SDFUpdateParameterFloat param,
		volatile volumetype * _nLeavesQueued,
		volatile bool *_threadValid,
		volumetype startLeaf
)
{
	const float *depth = param.depth;
	const uchar *red = param.red;
	const uchar *green = param.green;
	const uchar *blue = param.blue;
	int imageWidth = param.imageWidth;
	int imageHeight = param.imageHeight;
	float m11 = param.m11;
	float m12 = param.m12;
	float m13 = param.m13;
	float m14 = param.m14;
	float m21 = param.m21;
	float m22 = param.m22;
	float m23 = param.m23;
	float m24 = param.m24;
	float m31 = param.m31;
	float m32 = param.m32;
	float m33 = param.m33;
	float m34 = param.m34;
	float fx = param.fx;
	float fy = param.fy;
	float cx = param.cx;
	float cy = param.cy;
	float scale = param.scale;
	float distanceThreshold = param.distanceThreshold;
	volumetype *_leafNumber = param._leafNumber;
	sidetype3 *_leafPos = param._leafPos;
	sidetype *_leafScale = param._leafScale;
	float *_distance = param._distance;
	weighttype *_weights = param._weights;
	colortype3 *_color = param._color;
//	sidetype _brickLength = param.brickLength;

//	volumetype brickSize = _brickLength*_brickLength*_brickLength;

	volumetype l1 = startLeaf;


  unsigned int rnd_mode = _MM_GET_ROUNDING_MODE();
  if(rnd_mode != _MM_ROUND_TOWARD_ZERO) _MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);

	while(*_threadValid || l1 < *_nLeavesQueued){
		volumetype nLeavesQueued = *_nLeavesQueued;
		for(volumetype l=l1;l<nLeavesQueued;l++){
			volumetype brickIdx = _leafNumber[l];
			sidetype3 o = _leafPos[brickIdx];
			sidetype leafScale = _leafScale[brickIdx];

			update8NaiveFloat(depth,red,green,blue,imageWidth,imageHeight,
					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
					scale,distanceThreshold,brickIdx,o,leafScale,
					_distance,_weights,_color);

//			update8AddLoopLoopSingleFloat(depth,red,green,blue,imageWidth,imageHeight,
//					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
//					scale,distanceThreshold,brickIdx,o,leafScale,
//					_distance,_weights,_color);

//			update8AddLoopLoopSingle_noJumpsFloat(depth,red,green,blue,imageWidth,imageHeight,
//					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
//					scale,distanceThreshold,brickIdx,o,leafScale,
//					_distance,_weights,_color);

//			update8AddLoopSIMDSingleFloat(depth,red,green,blue,imageWidth,imageHeight,
//					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
//					scale,distanceThreshold,brickIdx,o,leafScale,
//					_distance,_weights,_color);

			update8AddLoopSSESingleFloat(depth,red,green,blue,imageWidth,imageHeight,
					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
					scale,distanceThreshold,brickIdx,o,leafScale,
					_distance,_weights,_color);

		}
		l1 = nLeavesQueued;
	}

  if(rnd_mode != _MM_ROUND_TOWARD_ZERO) _MM_SET_ROUNDING_MODE(rnd_mode);
}


void updateWrapperInteger
(
		SDFUpdateParameterInteger param,
		volatile volumetype * _nLeavesQueued,
		volatile bool *_threadValid,
		volumetype startLeaf
)
{
	const ushort *depth = param.depth;
	float scaling = param.scaling;
	float maxcamdistance = param.maxcamdistance;
	const uchar *rgb = param.rgb;
	int imageWidth = param.imageWidth;
	int imageHeight = param.imageHeight;
	float m11 = param.m11;
	float m12 = param.m12;
	float m13 = param.m13;
	float m14 = param.m14;
	float m21 = param.m21;
	float m22 = param.m22;
	float m23 = param.m23;
	float m24 = param.m24;
	float m31 = param.m31;
	float m32 = param.m32;
	float m33 = param.m33;
	float m34 = param.m34;
	float fx = param.fx;
	float fy = param.fy;
	float cx = param.cx;
	float cy = param.cy;
	float scale = param.scale;
	float distanceThreshold = param.distanceThreshold;
	volumetype *_leafNumber = param._leafNumber;
	sidetype3 *_leafPos = param._leafPos;
	sidetype *_leafScale = param._leafScale;
	float *_distance = param._distance;
	weighttype *_weights = param._weights;
	colortype3 *_color = param._color;
//	sidetype _brickLength = param.brickLength;

//	volumetype brickSize = _brickLength*_brickLength*_brickLength;

	volumetype l1 = startLeaf;


  unsigned int rnd_mode = _MM_GET_ROUNDING_MODE();
  if(rnd_mode != _MM_ROUND_TOWARD_ZERO) _MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);

	while(*_threadValid || l1 < *_nLeavesQueued){
		volumetype nLeavesQueued = *_nLeavesQueued;
		for(volumetype l=l1;l<nLeavesQueued;l++){
			volumetype brickIdx = _leafNumber[l];
			sidetype3 o = _leafPos[brickIdx];
			sidetype leafScale = _leafScale[brickIdx];

//			update8NaiveInteger(depth,scaling,maxcamdistance,rgb,imageWidth,imageHeight,
//					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
//					scale,distanceThreshold,brickIdx,o,leafScale,
//					_distance,_weights,_color);

			//TODO: Noch nicht implementiert
//			update8AddLoopLoopSingleInteger(depth,red,green,blue,imageWidth,imageHeight,
//					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
//					scale,distanceThreshold,brickIdx,o,leafScale,
//					_distance,_weights,_color);

			//TODO: Noch nicht implementiert
//			update8AddLoopLoopSingle_noJumpsInteger(depth,red,green,blue,imageWidth,imageHeight,
//					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
//					scale,distanceThreshold,brickIdx,o,leafScale,
//					_distance,_weights,_color);

			//TODO: Noch nicht implementiert
//			update8AddLoopSIMDSingleInteger(depth,red,green,blue,imageWidth,imageHeight,
//					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
//					scale,distanceThreshold,brickIdx,o,leafScale,
//					_distance,_weights,_color);

#ifdef OWNAVX
#pragma message "Compiling with AVX2 support"
			update8AddLoopAVXSingleInteger(depth,scaling,maxcamdistance,rgb,imageWidth,imageHeight,
					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
					scale,distanceThreshold,brickIdx,o,leafScale,
					_distance,_weights,_color);
#else
#pragma message "Compiling without AVX2 support"
			update8AddLoopSSESingleInteger(depth,scaling,maxcamdistance,rgb,imageWidth,imageHeight,
					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
					scale,distanceThreshold,brickIdx,o,leafScale,
					_distance,_weights,_color);
#endif



		}
		l1 = nLeavesQueued;
	}

  if(rnd_mode != _MM_ROUND_TOWARD_ZERO) _MM_SET_ROUNDING_MODE(rnd_mode);
}




