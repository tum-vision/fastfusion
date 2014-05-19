/*
 * loopclosure.cpp
 *
 *  Created on: Aug 25, 2013
 *      Author: steinbrf
 */

#include "loopclosure.hpp"
#include <xmmintrin.h> // SSE intrinsics
#include <emmintrin.h> // SSE2 intrinsics
#include <pmmintrin.h> // SSE3 intrinsics
#include <smmintrin.h>


void update8AddLoopSSESingleDeleteloop
(
		const float *depth, const uchar *red, const uchar *green, const uchar *blue,
		int imageWidth, int imageHeight,
		float &m11, float &m12, float &m13, float &m14,
		float &m21, float &m22, float &m23, float &m24,
		float &m31, float &m32, float &m33, float &m34,
		float &fx, float &fy, float &cx, float &cy,
		float &scale, float &distanceThreshold,
		volumetype &threadOffset,
		sidetype3 &o,
		sidetype &leafScale,
		float *_distance,
		float *_weights,
		colortype3 *_color
)
{
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
								//ATTENTION: Here the negative sign is implemented!
//								_mm_sub_ps(thresholdWeight,dInc),
								_mm_sub_ps(dInc,thresholdWeight),
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

bool updateWrapper_deleteloop
(
		threadparameterLoop param,
		size_t _nLeavesQueued,
		volumetype *_leafMapLoop,
		volumetype _nLeavesTotal,
		volumetype *_nLeavesMapped
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
	float *_weights = param._weights;
	colortype3 *_color = param._color;

	volumetype l1 = 0;


  unsigned int rnd_mode = _MM_GET_ROUNDING_MODE();
  if(rnd_mode != _MM_ROUND_TOWARD_ZERO) _MM_SET_ROUNDING_MODE(_MM_ROUND_TOWARD_ZERO);

  fprintf(stderr,"\nLeaves mapped before queue: %i",*_nLeavesMapped);
	while(l1 < _nLeavesQueued){
		volumetype nLeavesQueued = _nLeavesQueued;
		for(volumetype l=l1;l<nLeavesQueued;l++){
			fprintf(stderr," %i:[",l);
//			fprintf(stderr,"\nUpdating Leaf %i",l);
			volumetype brickIdx = _leafNumber[l];
			volumetype threadOffset = _leafMapLoop[brickIdx];
			if(threadOffset>=*_nLeavesMapped){
				if(*_nLeavesMapped>=_nLeavesTotal) return false;
				//TODO: Optimieren
				threadOffset = _leafMapLoop[brickIdx] = (*_nLeavesMapped); (*_nLeavesMapped)++;
			}
			fprintf(stderr,"%i %i]",brickIdx,threadOffset);
			//TODO: Optimieren mit Shift
			threadOffset*=512;
			sidetype3 o = _leafPos[brickIdx];
			sidetype leafScale = _leafScale[brickIdx];

			fprintf(stderr,"?");
			update8AddLoopSSESingleDeleteloop(depth,red,green,blue,imageWidth,imageHeight,
					m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,fx,fy,cx,cy,
					scale,distanceThreshold,threadOffset,o,leafScale,
					_distance,_weights,_color);
			fprintf(stderr,"!");

		}
		l1 = nLeavesQueued;
	}
  fprintf(stderr,"\nLeaves mapped after queue: %i",*_nLeavesMapped);

  if(rnd_mode != _MM_ROUND_TOWARD_ZERO) _MM_SET_ROUNDING_MODE(rnd_mode);
  return true;
}
