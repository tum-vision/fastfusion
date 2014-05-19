#include "geometryfusion_mipmap_cpu.hpp"
#include "mesh_interleaved.hpp"
#include "mesh_interleaved_meshcell.hpp"

#include <pmmintrin.h>

//#define WEIGHT_MINIMUM

void addBrickInterior_indexed_incremental_optimized
(
		volumetype lastleaf, const ParentArray &leafParent,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		treeinfo info,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
//	offset.x = offset.y = offset.z = 0.0;
//	scale = 1.0;
	MeshInterleaved &mesh = *pmesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	float *w = new float[brickSize];
	VertexColor *c = new VertexColor[brickSize];
//	float *c = new float[brickSize*3];


	ALIGNED weighttype wf[8]; ALIGNED float df[8];
//	ALIGNED colortype3 cf[8];
	ALIGNED float cf[24];

//	MarchingCubesIndexed mc(bl,bl);
	unsigned int *indices = new unsigned int[brickSize*3];

	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	sidetype sizeMin = leafScale[lastleaf];

	if(sizeMin>size){
		fprintf(stderr,"\nWARNING: The Leaf is too large: %i > %i",sizeMin,size);
		return;
	}

	if(size/sizeMin>bl){
		fprintf(stderr,"\nERROR: The leaf is too small! (%i %i %i | %i): %i",ox,oy,oz,size,sizeMin);
		return;
	}

	if(ox+size>leafPos[lastleaf].x+leafScale[lastleaf]*bl || ox<leafPos[lastleaf].x ||
		 oy+size>leafPos[lastleaf].y+leafScale[lastleaf]*bl || oy<leafPos[lastleaf].y ||
		 oz+size>leafPos[lastleaf].z+leafScale[lastleaf]*bl || oz<leafPos[lastleaf].z){
		fprintf(stderr,"\nERROR: The interior Cell [%i %i %i]%i does not fit in the Leaf [%i %i %i]%i*%i",
				ox,oy,oz,size,leafPos[lastleaf].x,leafPos[lastleaf].y,leafPos[lastleaf].z,sizeMin,bl);
		return;
	}

//	fprintf(stderr,"\nContents of the last leaf:");
//	for(unsigned int z=0;z<bl;z++){
//		fprintf(stderr,"\n");
//		for(unsigned int y=0;y<bl;y++){
//			fprintf(stderr,"\n");
//			for(unsigned int x=0;x<bl;x++){
//				fprintf(stderr,"(%+.3f %+.3f)",distance[lastleaf*brickSize+(z*bl+y)*bl+x],
//						weights[lastleaf*brickSize+(z*bl+y)*bl+x]);
//			}
//		}
//	}





	for(volumetype idx=0;idx<brickSize;idx++) w[idx] = 0;


	for(sidetype z=oz;z<oz+size;z+=sizeMin){
//		fprintf(stderr,"\n");
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
//			fprintf(stderr,"\n");
			sidetype by = (y-oy)/sizeMin;
			for(sidetype x=ox;x<ox+size;x+=sizeMin){
				sidetype bx = (x-ox)/sizeMin;
				volumetype idx = (bz*bl+by)*bl+bx;
				w[idx] = 0;
				int depth = -1;
				for(volumetype leaf=lastleaf;leaf<BRANCHINIT
#ifdef ADD_WEIGHTS_TRANSITION_140424
				&& w[idx]<=MIN_WEIGHT_FOR_SURFACE
#else
				&& w[idx]<=0.0f
#endif
				;
				leaf=leafParent[leaf]
//				,leaf=BRANCHINIT
				                ){
					depth++;

					if(ox+size>leafPos[leaf].x+leafScale[leaf]*bl || ox<leafPos[leaf].x ||
						 oy+size>leafPos[leaf].y+leafScale[leaf]*bl || oy<leafPos[leaf].y ||
						 oz+size>leafPos[leaf].z+leafScale[leaf]*bl || oz<leafPos[leaf].z){
						fprintf(stderr,"\nERROR: The interior Cell [%i %i %i]%i does not fit in the large Leaf [%i %i %i]%i*%i",
								ox,oy,oz,size,leafPos[leaf].x,leafPos[leaf].y,leafPos[leaf].z,sizeMin,bl);
						fprintf(stderr,"\nStarted from leaf [%i %i %i]%i*%i",
								leafPos[lastleaf].x,leafPos[lastleaf].y,leafPos[lastleaf].z,leafScale[lastleaf],bl);
						return;
					}

					volumetype start = leaf*brickSize;
					sidetype3 lo = leafPos[leaf];
					sidetype ls = leafScale[leaf];


					sidetype lxl = (x-lo.x)/ls;
					sidetype lxr = (x-lo.x)%ls;
					float rx = (float)lxr/(float)ls;
					float rxn = 1.0f-rx;
					sidetype lxh = lxl+(rx>0.0 && lxl<bl-1);

					sidetype lyl = (y-lo.y)/ls;
					sidetype lyr = (y-lo.y)%ls;
					float ry = (float)lyr/(float)ls;
					float ryn = 1.0f-ry;
					sidetype lyh = lyl+(ry>0.0 && lyl<bl-1);

					sidetype lzl = (z-lo.z)/ls;
					sidetype lzr = (z-lo.z)%ls;
					float rz = (float)lzr/(float)ls;
					float rzn = 1.0f-rz;
					sidetype lzh = lzl+(rz>0.0 && lzl<bl-1);

					ALIGNED float f[8] = {
							rzn*ryn*rxn,
							rzn*ryn*rx,
							rzn*ry *rxn,
							rzn*ry *rx,
							rz *ryn*rxn,
							rz *ryn*rx,
							rz *ry *rxn,
							rz *ry *rx
					};

					if(leaf==lastleaf && (rx || ry || rz)) fprintf(stderr,"\nERROR Interior: Finest Size does not match: "
							"(%i %i %i):(%i %i) (%i %i %i): %i  - (%i %i %i) (%f %f %f)",
							ox,oy,oz,size,sizeMin,lo.x,lo.y,lo.z,ls,x,y,z,rx,ry,rz);

					volumetype idxLeaf[8] = {
							(volumetype)((lzl*bl+lyl)*bl+lxl),
							(volumetype)((lzl*bl+lyl)*bl+lxh),
							(volumetype)((lzl*bl+lyh)*bl+lxl),
							(volumetype)((lzl*bl+lyh)*bl+lxh),
							(volumetype)((lzh*bl+lyl)*bl+lxl),
							(volumetype)((lzh*bl+lyl)*bl+lxh),
							(volumetype)((lzh*bl+lyh)*bl+lxl),
							(volumetype)((lzh*bl+lyh)*bl+lxh)
					};

					for(volumetype i=0;i<8;i++) if(idxLeaf[i]>brickSize)
						fprintf(stderr,"\nERROR: Index out of bound!");

					for(volumetype i=0;i<8;i++) {

						wf[i] = (float)weights[start+idxLeaf[i]];
						df[i] = distance[start+idxLeaf[i]];
//						if(color) cf[i] = color[start+idxLeaf[i]];
						if(color) {
							cf[i] = color[start+idxLeaf[i]].x;
							cf[i+8] = color[start+idxLeaf[i]].y;
							cf[i+16] = color[start+idxLeaf[i]].z;
						}
					}

					__m128 fSSE0 = _mm_load_ps(f);
					__m128 fSSE1 = _mm_load_ps(f+4);
					__m128 wsum = _mm_add_ps(_mm_mul_ps(fSSE0,_mm_load_ps(wf)),_mm_mul_ps(fSSE1,_mm_load_ps(wf+4)));
					ALIGNED float temp1[4];
					_mm_store_ps(temp1,_mm_hadd_ps(wsum,wsum));
//					w[idx] = temp1[0]+temp1[1];
//					w[idx] = f[0]*wf[0] + f[1]*wf[1] + f[2]*wf[2] + f[3]*wf[3] +
//							     f[4]*wf[4] + f[5]*wf[5] + f[6]*wf[6] + f[7]*wf[7];
#ifdef ADD_WEIGHTS_TRANSITION_140424
					w[idx] += wf[0];
#else
					w[idx] = wf[0];
#endif
//					for(uchar i=1;i<8;i++) if(w[idx]>wf[i]) w[idx] = wf[i];

					wsum = _mm_add_ps(_mm_mul_ps(fSSE0,_mm_load_ps(df)),_mm_mul_ps(fSSE1,_mm_load_ps(df+4)));
					_mm_store_ps(temp1,_mm_hadd_ps(wsum,wsum));
					d[idx] = temp1[0]+temp1[1];
//					d[idx] = f[0]*df[0] + f[1]*df[1] + f[2]*df[2] + f[3]*df[3] +
//							     f[4]*df[4] + f[5]*df[5] + f[6]*df[6] + f[7]*df[7];

					if(color){
						ALIGNED float temp2[4]; ALIGNED float temp3[4];
						wsum = _mm_add_ps(_mm_mul_ps(fSSE0,_mm_load_ps(cf)),_mm_mul_ps(fSSE1,_mm_load_ps(cf+4)));
						_mm_store_ps(temp1,_mm_hadd_ps(wsum,wsum));
						wsum = _mm_add_ps(_mm_mul_ps(fSSE0,_mm_load_ps(cf+8)),_mm_mul_ps(fSSE1,_mm_load_ps(cf+12)));
						_mm_store_ps(temp2,_mm_hadd_ps(wsum,wsum));
						wsum = _mm_add_ps(_mm_mul_ps(fSSE0,_mm_load_ps(cf+16)),_mm_mul_ps(fSSE1,_mm_load_ps(cf+20)));
						_mm_store_ps(temp3,_mm_hadd_ps(wsum,wsum));
#ifndef BRICKVISUALIZATION
#ifndef COLORINVERSION
						c[idx] = VertexColor(temp1[0]+temp1[1],temp2[0]+temp2[1],temp3[0]+temp3[1]);
#else
						c[idx] = VertexColor(temp3[0]+temp3[1],temp2[0]+temp2[1],temp1[0]+temp1[1]);
#endif
#else
						c[idx] = VertexColor(65280.0f,65280.0f,65280.0f);
#endif
					}

//					w[idx] = f[0]*wf[0]+
//							     f[1]*wf[1]+
//							     f[2]*wf[2]+
//							     f[3]*wf[3]+
//							     f[4]*wf[4]+
//							     f[5]*wf[5]+
//							     f[6]*wf[6]+
//							     f[7]*wf[7];
//
//					d[idx] = f[0]*df[0]+
//							     f[1]*df[1]+
//							     f[2]*df[2]+
//							     f[3]*df[3]+
//							     f[4]*df[4]+
//							     f[5]*df[5]+
//							     f[6]*df[6]+
//							     f[7]*df[7];
//
//					if(color) c[idx] = VertexColor(
//						f[0]*cf[0 ]+f[1]*cf[1 ]+f[2]*cf[2 ]+f[3]*cf[3 ]+f[4]*cf[4 ]+f[5]*cf[5 ]+f[6]*cf[6 ]+f[7]*cf[7 ],
//						f[0]*cf[8 ]+f[1]*cf[9 ]+f[2]*cf[10]+f[3]*cf[11]+f[4]*cf[12]+f[5]*cf[13]+f[6]*cf[14]+f[7]*cf[15],
//						f[0]*cf[16]+f[1]*cf[17]+f[2]*cf[18]+f[3]*cf[19]+f[4]*cf[20]+f[5]*cf[21]+f[6]*cf[22]+f[7]*cf[23]
//					);

				}
//				fprintf(stderr," %.2i",depth);
			}
		}
	}


	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
				sidetype bx = (x-ox)/sizeMin;
				volumetype idx = (bz*bl+by)*bl+bx;

				if(weightInfluence(minWeight,
						w[idx],w[idx+1],
						w[idx+bl+1],w[idx+bl],
						w[idx+bs],w[idx+bs+1],
						w[idx+bs+bl+1],w[idx+bs+bl])){
					int *table = mc.offsetTable[mc.getCubeIndex(
							d[idx],d[idx+1],
							d[idx+bl+1],d[idx+bl],
							d[idx+bs],d[idx+bs+1],
							d[idx+bs+bl+1],d[idx+bs+bl],
							w[idx],w[idx+1],
							w[idx+bl+1],w[idx+bl],
							w[idx+bs],w[idx+bs+1],
							w[idx+bs+bl+1],w[idx+bs+bl])];

					for (unsigned int i=0;table[i]!=-1;i+=3) {
						faceIsSet[3*idx+table[i  ]] = true;
						faceIsSet[3*idx+table[i+1]] = true;
						faceIsSet[3*idx+table[i+2]] = true;
					}
					tables[idx] = table;
				}
			}
		}
	}

	size_t runningIndex = mesh.vertices.size();
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
//		fprintf(stderr,"\n");
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
//			fprintf(stderr,"\n");
			sidetype by = (y-oy)/sizeMin;
			for(sidetype x=ox;x<ox+size;x+=sizeMin){
				sidetype bx = (x-ox)/sizeMin;
				volumetype idx = (bz*bl+by)*bl+bx;


//				fprintf(stderr,"(%+.3f %+.3f)",d[idx],w[idx]);

				if(
						faceIsSet[3*idx+0] //&&
//						w[idx] && w[idx+1] &&
//						((d[idx]<0)!=(d[idx+1]<0))
						){
//				if(x<ox+size-sizeMin && w[idx] && w[idx+1] && d[idx]*d[idx+1]<0.0f){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+(x      )*scale,offset.y+y*scale,offset.z+z*scale),
							Vertex3f(offset.x+(x+sizeMin)*scale,offset.y+y*scale,offset.z+z*scale),
							d[idx],d[idx+1]);
					if(idx+1>=brickSize) fprintf(stderr,"\nERROR:Interior X idx=%i",idx);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
//					mesh.r.push_back(0);
//					mesh.g.push_back(255);
//					mesh.b.push_back(0);
					indices[3*idx+0] = runningIndex++;
					vertexIsSet[3*idx+0] = true;
				}
				if(
						faceIsSet[3*idx+1] &&
						w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))){
//				if(y<oy+size-sizeMin && w[idx] && w[idx+bl] && d[idx]*d[idx+bl]<0.0f){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+(y        )*scale,offset.z+z*scale),
							Vertex3f(offset.x+x*scale,offset.y+(y+sizeMin)*scale,offset.z+z*scale),
							d[idx],d[idx+bl]);
					if(idx+bl>=brickSize) fprintf(stderr,"\nERROR:Interior Y idx=%i",idx);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
//					mesh.r.push_back(0);
//					mesh.g.push_back(255);
//					mesh.b.push_back(0);
					indices[3*idx+1] = runningIndex++;
					vertexIsSet[3*idx+1] = true;
				}
				if(
						faceIsSet[3*idx+2] &&
						w[idx] && w[idx+bs] && ((d[idx]<0)!=(d[idx+bs]<0))){
//				if(z<oz+size-sizeMin && w[idx] && w[idx+bs] && d[idx]*d[idx+bs]<0.0f){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+y*scale,offset.z+(z        )*scale),
							Vertex3f(offset.x+x*scale,offset.y+y*scale,offset.z+(z+sizeMin)*scale),
							d[idx],d[idx+bs]);
					if(idx+bs>=brickSize) fprintf(stderr,"\nERROR:Interior Z idx=%i",idx);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
//					mesh.r.push_back(0);
//					mesh.g.push_back(255);
//					mesh.b.push_back(0);
					indices[3*idx+2] = runningIndex++;
					vertexIsSet[3*idx+2] = true;
				}
			}
		}
	}

//	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
//		sidetype bz = (z-oz)/sizeMin;
//		for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
//			sidetype by = (y-oy)/sizeMin;
//			for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
//				sidetype bx = (x-ox)/sizeMin;
//				volumetype idx = (bz*bl+by)*bl+bx;
//
//				int *table = tables[idx];
//				if(table){
//					for (unsigned int i=0;table[i]!=-1;i+=3) {
//						mesh.faces.push_back(indices[3*idx+table[i  ]]);
//						mesh.faces.push_back(indices[3*idx+table[i+1]]);
//						mesh.faces.push_back(indices[3*idx+table[i+2]]);
//					}
//				}
//			}
//		}
//	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
				sidetype bx = (x-ox)/sizeMin;
				volumetype idx = (bz*bl+by)*bl+bx;

				if(weightInfluence(minWeight,
						w[idx],w[idx+1],
						w[idx+bl+1],w[idx+bl],
						w[idx+bs],w[idx+bs+1],
						w[idx+bs+bl+1],w[idx+bs+bl])){
					int *table = mc.offsetTable[mc.getCubeIndex(
							d[idx],d[idx+1],
							d[idx+bl+1],d[idx+bl],
							d[idx+bl*bl],d[idx+bl*bl+1],
							d[idx+bl*bl+bl+1],d[idx+bl*bl+bl],
							w[idx],w[idx+1],
							w[idx+bl+1],w[idx+bl],
							w[idx+bl*bl],w[idx+bl*bl+1],
							w[idx+bl*bl+bl+1],w[idx+bl*bl+bl])];

					for (unsigned int i=0;table[i]!=-1;i+=3) {
						mesh.faces.push_back(indices[3*idx+table[i  ]]);
						mesh.faces.push_back(indices[3*idx+table[i+1]]);
						mesh.faces.push_back(indices[3*idx+table[i+2]]);
						if(indices[3*idx+table[i  ]]==indices[3*idx+table[i+1]] ||
						   indices[3*idx+table[i+1]]==indices[3*idx+table[i+2]] ||
						   indices[3*idx+table[i+2]]==indices[3*idx+table[i  ]]){
							fprintf(stderr,"\nERROR: Face has degenerate Indices [%i %i %i]",
									indices[3*idx+table[i  ]],indices[3*idx+table[i+1]],indices[3*idx+table[i+2]]);
						}
						if(indices[3*idx+table[i  ]]>=runningIndex){
							fprintf(stderr,"\nInterior Index Error: %i >= %li",indices[3*idx+table[i  ]],runningIndex);
							fprintf(stderr,"\nFace %i is %s",3*idx+table[i  ],faceIsSet[3*idx+table[i  ]] ? "set" : "not set");
						}
						if(indices[3*idx+table[i+1]]>=runningIndex){
							fprintf(stderr,"\nInterior Index Error: %i >= %li",indices[3*idx+table[i+1]],runningIndex);
							fprintf(stderr,"\nFace %i is %s",3*idx+table[i+1],faceIsSet[3*idx+table[i+1]] ? "set" : "not set");
						}
						if(indices[3*idx+table[i+2]]>=runningIndex){
							fprintf(stderr,"\nInterior Index Error: %i >= %li",indices[3*idx+table[i+2]],runningIndex);
							fprintf(stderr,"\nFace %i is %s",3*idx+table[i+2],faceIsSet[3*idx+table[i+2]] ? "set" : "not set");
						}
					}
				}


			}
		}
	}



	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i] in Wall X",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i in Interior",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);

	if(wrongIndices){
		for(unsigned int z=0;z<bl;z++){
			fprintf(stderr,"\n");
			for(unsigned int y=0;y<bl;y++){
				fprintf(stderr,"\n");
				for(unsigned int x=0;x<bl;x++){
					fprintf(stderr,"(%i%i|%i%i|%i%i)",
							vertexIsSet[3*((z*bl+y)*bl+x)+0],faceIsSet[3*((z*bl+y)*bl+x)+0],
							vertexIsSet[3*((z*bl+y)*bl+x)+1],faceIsSet[3*((z*bl+y)*bl+x)+1],
							vertexIsSet[3*((z*bl+y)*bl+x)+2],faceIsSet[3*((z*bl+y)*bl+x)+2]);
				}

				for(unsigned int x=0;x<bl;x++){
					fprintf(stderr,"(%.3f %.3f)",d[(z*bl+y)*bl+x],w[(z*bl+y)*bl+x]);
				}
			}
		}
		char c;
		std::cin >> c;
	}

//	mesh.vertices.push_back(Vertex3f(offset.x+(ox     )*scale,offset.y+(oy     )*scale,offset.z+(oz     )*scale));
//	mesh.vertices.push_back(Vertex3f(offset.x+(ox+size)*scale,offset.y+(oy     )*scale,offset.z+(oz     )*scale));
//	mesh.vertices.push_back(Vertex3f(offset.x+(ox     )*scale,offset.y+(oy+size)*scale,offset.z+(oz     )*scale));
//	mesh.vertices.push_back(Vertex3f(offset.x+(ox+size)*scale,offset.y+(oy+size)*scale,offset.z+(oz     )*scale));
//	mesh.vertices.push_back(Vertex3f(offset.x+(ox     )*scale,offset.y+(oy     )*scale,offset.z+(oz+size)*scale));
//	mesh.vertices.push_back(Vertex3f(offset.x+(ox+size)*scale,offset.y+(oy     )*scale,offset.z+(oz+size)*scale));
//	mesh.vertices.push_back(Vertex3f(offset.x+(ox     )*scale,offset.y+(oy+size)*scale,offset.z+(oz+size)*scale));
//	mesh.vertices.push_back(Vertex3f(offset.x+(ox+size)*scale,offset.y+(oy+size)*scale,offset.z+(oz+size)*scale));
//
//	mesh.colors.push_back(Color3b(0,255,0));
//	mesh.colors.push_back(Color3b(0,255,0));
//	mesh.colors.push_back(Color3b(0,255,0));
//	mesh.colors.push_back(Color3b(0,255,0));
//	mesh.colors.push_back(Color3b(0,255,0));
//	mesh.colors.push_back(Color3b(0,255,0));
//	mesh.colors.push_back(Color3b(0,255,0));
//	mesh.colors.push_back(Color3b(0,255,0));
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//
//
//	for(volumetype cubeleaf = lastleaf; cubeleaf<BRANCHINIT;cubeleaf=leafParent[cubeleaf]){
//		sidetype3 p0 = leafPos[cubeleaf];
//		sidetype sl = leafScale[cubeleaf]*bl;
//		mesh.vertices.push_back(Vertex3f(offset.x+(p0.x   )*scale,offset.y+(p0.y   )*scale,offset.z+(p0.z   )*scale));
//		mesh.vertices.push_back(Vertex3f(offset.x+(p0.x+sl)*scale,offset.y+(p0.y   )*scale,offset.z+(p0.z   )*scale));
//		mesh.vertices.push_back(Vertex3f(offset.x+(p0.x   )*scale,offset.y+(p0.y+sl)*scale,offset.z+(p0.z   )*scale));
//		mesh.vertices.push_back(Vertex3f(offset.x+(p0.x+sl)*scale,offset.y+(p0.y+sl)*scale,offset.z+(p0.z   )*scale));
//		mesh.vertices.push_back(Vertex3f(offset.x+(p0.x   )*scale,offset.y+(p0.y   )*scale,offset.z+(p0.z+sl)*scale));
//		mesh.vertices.push_back(Vertex3f(offset.x+(p0.x+sl)*scale,offset.y+(p0.y   )*scale,offset.z+(p0.z+sl)*scale));
//		mesh.vertices.push_back(Vertex3f(offset.x+(p0.x   )*scale,offset.y+(p0.y+sl)*scale,offset.z+(p0.z+sl)*scale));
//		mesh.vertices.push_back(Vertex3f(offset.x+(p0.x+sl)*scale,offset.y+(p0.y+sl)*scale,offset.z+(p0.z+sl)*scale));
//
//		mesh.colors.push_back(Color3b(255,0,0));
//		mesh.colors.push_back(Color3b(255,0,0));
//		mesh.colors.push_back(Color3b(255,0,0));
//		mesh.colors.push_back(Color3b(255,0,0));
//		mesh.colors.push_back(Color3b(255,0,0));
//		mesh.colors.push_back(Color3b(255,0,0));
//		mesh.colors.push_back(Color3b(255,0,0));
//		mesh.colors.push_back(Color3b(255,0,0));
//
//
//		mesh.faces.push_back(mesh.vertices.size()-8);
//		mesh.faces.push_back(mesh.vertices.size()-7);
//		mesh.faces.push_back(mesh.vertices.size()-5);
//
//		mesh.faces.push_back(mesh.vertices.size()-8);
//		mesh.faces.push_back(mesh.vertices.size()-5);
//		mesh.faces.push_back(mesh.vertices.size()-6);
//
//		mesh.faces.push_back(mesh.vertices.size()-4);
//		mesh.faces.push_back(mesh.vertices.size()-3);
//		mesh.faces.push_back(mesh.vertices.size()-1);
//
//		mesh.faces.push_back(mesh.vertices.size()-4);
//		mesh.faces.push_back(mesh.vertices.size()-1);
//		mesh.faces.push_back(mesh.vertices.size()-2);
//
//
//		mesh.faces.push_back(mesh.vertices.size()-8);
//		mesh.faces.push_back(mesh.vertices.size()-7);
//		mesh.faces.push_back(mesh.vertices.size()-3);
//
//		mesh.faces.push_back(mesh.vertices.size()-8);
//		mesh.faces.push_back(mesh.vertices.size()-3);
//		mesh.faces.push_back(mesh.vertices.size()-4);
//
//		mesh.faces.push_back(mesh.vertices.size()-6);
//		mesh.faces.push_back(mesh.vertices.size()-5);
//		mesh.faces.push_back(mesh.vertices.size()-1);
//
//		mesh.faces.push_back(mesh.vertices.size()-6);
//		mesh.faces.push_back(mesh.vertices.size()-1);
//		mesh.faces.push_back(mesh.vertices.size()-2);
//
//
//		mesh.faces.push_back(mesh.vertices.size()-8);
//		mesh.faces.push_back(mesh.vertices.size()-4);
//		mesh.faces.push_back(mesh.vertices.size()-2);
//
//		mesh.faces.push_back(mesh.vertices.size()-8);
//		mesh.faces.push_back(mesh.vertices.size()-2);
//		mesh.faces.push_back(mesh.vertices.size()-6);
//
//		mesh.faces.push_back(mesh.vertices.size()-7);
//		mesh.faces.push_back(mesh.vertices.size()-3);
//		mesh.faces.push_back(mesh.vertices.size()-1);
//
//		mesh.faces.push_back(mesh.vertices.size()-7);
//		mesh.faces.push_back(mesh.vertices.size()-1);
//		mesh.faces.push_back(mesh.vertices.size()-5);
//	}






	delete [] d; delete [] w;
	delete [] c;
	delete [] indices;

	delete [] vertexIsSet;
	delete [] faceIsSet;
	delete [] tables;
}





void addBrickWallX_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshInterleaved &mesh = *pmesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[4]; float df[4]; colortype3 cf[4];

	volumetype lastleaves[2] = {lastleaf0,lastleaf1};

	sidetype sizeMin = std::min(leafScale[lastleaf0],leafScale[lastleaf1]);
	sidetype sizeStretch = leafScale[lastleaf0];

	if(sizeStretch>size){
		fprintf(stderr,"\nWARNING Wall X: The Leaf is too large: %i > %i",sizeStretch,size);
		return;
	}

	sidetype x[2] = {(sidetype)(ox+size-sizeStretch) , (sidetype)(ox+size)};
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;

			for(unsigned int bx=0;bx<2;bx++){
				volumetype idx = (bz*bl+by)*bl+bx;
				if(idx>=512){
					fprintf(stderr,"\nERROR: Wrong Index: [%i %i %i][%i %i %i][%i %i %i] (%i %i)",bx,by,bz,x[0],y,z,ox,oy,oz,sizeMin,size);
					char c; std::cin >> c;
				}
				w[idx] = 0;
				for(volumetype leaf=lastleaves[bx];leaf<BRANCHINIT
#ifdef ADD_WEIGHTS_TRANSITION_140424
				&& w[idx]<=MIN_WEIGHT_FOR_SURFACE;
#else
				&& w[idx]<=0.0f;
#endif
				leaf=leafParent[leaf]
	//				leaf=BRANCHINIT
				 ){
					volumetype start = leaf*brickSize;
					sidetype3 lo = leafPos[leaf];
					sidetype ls = leafScale[leaf];

					sidetype lx = (x[bx]-lo.x)/ls;

					sidetype lyl = (y-lo.y)/ls;
					sidetype lyr = (y-lo.y)%ls;
					float ry = (float)lyr/(float)ls;
					sidetype lyh = lyl+(ry>0.0 && lyl<bl-1);

					sidetype lzl = (z-lo.z)/ls;
					sidetype lzr = (z-lo.z)%ls;
					float rz = (float)lzr/(float)ls;
					sidetype lzh = lzl+(rz>0.0 && lzl<bl-1);

					volumetype idxLeaf[4] = {
							(volumetype)((lzl*bl+lyl)*bl+lx),
							(volumetype)((lzl*bl+lyh)*bl+lx),
							(volumetype)((lzh*bl+lyl)*bl+lx),
							(volumetype)((lzh*bl+lyh)*bl+lx)
					};

					for(volumetype i=0;i<4;i++) {
						wf[i] = weights[start+idxLeaf[i]];
						df[i] = distance[start+idxLeaf[i]];
						if(color) cf[i] = color[start+idxLeaf[i]];
					}

					float ryInv = 1.0f-ry;
					float rzInv = 1.0f-rz;
#ifndef WEIGHT_MINIMUM
#ifdef ADD_WEIGHTS_TRANSITION_140424
					w[idx] += rzInv*ryInv*(float)wf[0]+
									  rzInv*ry   *(float)wf[1]+
									  rz   *ryInv*(float)wf[2]+
									  rz   *ry   *(float)wf[3];
#else
					w[idx] = rzInv*ryInv*(float)wf[0]+
									 rzInv*ry   *(float)wf[1]+
									 rz   *ryInv*(float)wf[2]+
									 rz   *ry   *(float)wf[3];
#endif
#else
					w[idx] = std::min(std::min(wf[0],wf[1]),std::min(wf[2],wf[3]));
#endif

					d[idx] = rzInv*ryInv*df[0]+
									 rzInv*ry   *df[1]+
									 rz   *ryInv*df[2]+
									 rz   *ry   *df[3];

					if(color)
#ifndef BRICKVISUALIZATION
#ifndef COLORINVERSION
					c[idx] = VertexColor(
									 rzInv*ryInv*(float)cf[0].x+
									 rzInv*ry   *(float)cf[1].x+
									 rz   *ryInv*(float)cf[2].x+
									 rz   *ry   *(float)cf[3].x,
									 rzInv*ryInv*(float)cf[0].y+
									 rzInv*ry   *(float)cf[1].y+
									 rz   *ryInv*(float)cf[2].y+
									 rz   *ry   *(float)cf[3].y,
									 rzInv*ryInv*(float)cf[0].z+
									 rzInv*ry   *(float)cf[1].z+
									 rz   *ryInv*(float)cf[2].z+
									 rz   *ry   *(float)cf[3].z);
#else
					c[idx] = VertexColor(
									 rzInv*ryInv*(float)cf[0].z+
									 rzInv*ry   *(float)cf[1].z+
									 rz   *ryInv*(float)cf[2].z+
									 rz   *ry   *(float)cf[3].z,
									 rzInv*ryInv*(float)cf[0].y+
									 rzInv*ry   *(float)cf[1].y+
									 rz   *ryInv*(float)cf[2].y+
									 rz   *ry   *(float)cf[3].y,
									 rzInv*ryInv*(float)cf[0].x+
									 rzInv*ry   *(float)cf[1].x+
									 rz   *ryInv*(float)cf[2].x+
									 rz   *ry   *(float)cf[3].x);
#endif
#else
						c[idx] = VertexColor(0,0,65280);
#endif
				}
			}
		}
	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			volumetype idx = (bz*bl+by)*bl;

			if(weightInfluence(minWeight,
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])){
				int *table = mc.offsetTable[mc.getCubeIndex(
						d[idx],d[idx+1],
						d[idx+bl+1],d[idx+bl],
						d[idx+bs],d[idx+bs+1],
						d[idx+bs+bl+1],d[idx+bs+bl],
						w[idx],w[idx+1],
						w[idx+bl+1],w[idx+bl],
						w[idx+bs],w[idx+bs+1],
						w[idx+bs+bl+1],w[idx+bs+bl])];

				for (unsigned int i=0;table[i]!=-1;i+=3) {
					faceIsSet[3*idx+table[i  ]] = true;
					faceIsSet[3*idx+table[i+1]] = true;
					faceIsSet[3*idx+table[i+2]] = true;
				}
				tables[idx] = table;
			}
		}
	}

	size_t runningIndex = mesh.vertices.size();
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			for(sidetype bx=0;bx<2;bx++){
				volumetype idx = (bz*bl+by)*bl+bx;

				if(faceIsSet[3*idx+0] && w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+(x[bx]            )*scale,offset.y+y*scale,offset.z+z*scale),
							Vertex3f(offset.x+(x[bx]+sizeStretch)*scale,offset.y+y*scale,offset.z+z*scale),
							d[idx],d[idx+1]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
					indices[3*idx+0] = runningIndex++;
					vertexIsSet[3*idx+0] = true;
				}
				if(
						y<oy+size-sizeMin &&
						faceIsSet[3*idx+1]
						&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x[bx]*scale,offset.y+(y        )*scale,offset.z+z*scale),
							Vertex3f(offset.x+x[bx]*scale,offset.y+(y+sizeMin)*scale,offset.z+z*scale),
							d[idx],d[idx+bl]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
					indices[3*idx+1] = runningIndex++;
					vertexIsSet[3*idx+1] = true;
				}
				if(
						z<oz+size-sizeMin &&
					 faceIsSet[3*idx+2]
						&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x[bx]*scale,offset.y+y*scale,offset.z+(z        )*scale),
							Vertex3f(offset.x+x[bx]*scale,offset.y+y*scale,offset.z+(z+sizeMin)*scale),
							d[idx],d[idx+bs]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
					indices[3*idx+2] = runningIndex++;
					vertexIsSet[3*idx+2] = true;
				}
			}
		}
	}


	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			volumetype idx = (bz*bl+by)*bl;

			int *table = tables[idx];
			if(table){
				for (unsigned int i=0;table[i]!=-1;i+=3) {
					mesh.faces.push_back(indices[3*idx+table[i  ]]);
					mesh.faces.push_back(indices[3*idx+table[i+1]]);
					mesh.faces.push_back(indices[3*idx+table[i+2]]);
				}
			}
		}
	}

	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i] in Wall X",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i in Wall X",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);


	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}


void addBrickWallY_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshInterleaved &mesh = *pmesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[4]; float df[4]; colortype3 cf[4];

	volumetype lastleaves[2] = {lastleaf0,lastleaf1};

	sidetype sizeMin = std::min(leafScale[lastleaf0],leafScale[lastleaf1]);
	sidetype sizeStretch = leafScale[lastleaf0];

	if(sizeStretch>size){
		fprintf(stderr,"\nWARNING Wall Y: The Leaf is too large: %i > %i",sizeStretch,size);
		return;
	}

	sidetype y[2] = {(sidetype)(oy+size-sizeStretch) , (sidetype)(oy+size)};

//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy+size-sizeStretch)*scale,
//			offset.z+(oz                 )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin    )*scale,
//			offset.y+(oy+size-sizeStretch)*scale,
//			offset.z+(oz                 )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy+size-sizeStretch)*scale,
//			offset.z+(oz+size-sizeMin    )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin    )*scale,
//			offset.y+(oy+size-sizeStretch)*scale,
//			offset.z+(oz+size-sizeMin    )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy+size            )*scale,
//			offset.z+(oz                 )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin    )*scale,
//			offset.y+(oy+size            )*scale,
//			offset.z+(oz                 )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy+size            )*scale,
//			offset.z+(oz+size-sizeMin    )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin    )*scale,
//			offset.y+(oy+size            )*scale,
//			offset.z+(oz+size-sizeMin    )*scale));
//
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-3);

	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			for(unsigned int by=0;by<2;by++){
				volumetype idx = (bz*bl+by)*bl+bx;
				w[idx] = 0;
				for(volumetype leaf=lastleaves[by];leaf<BRANCHINIT
#ifdef ADD_WEIGHTS_TRANSITION_140424
				&& w[idx]<=MIN_WEIGHT_FOR_SURFACE;
#else
				&& w[idx]<=0.0f;
#endif
				leaf=leafParent[leaf]
	//				leaf=BRANCHINIT
				 ){
					volumetype start = leaf*brickSize;
					sidetype3 lo = leafPos[leaf];
					sidetype ls = leafScale[leaf];

					sidetype ly = (y[by]-lo.y)/ls;

					sidetype lxl = (x-lo.x)/ls;
					sidetype lxr = (x-lo.x)%ls;
					float rx = (float)lxr/(float)ls;
					sidetype lxh = lxl+(rx>0.0 && lxl<bl-1);

					sidetype lzl = (z-lo.z)/ls;
					sidetype lzr = (z-lo.z)%ls;
					float rz = (float)lzr/(float)ls;
					sidetype lzh = lzl+(rz>0.0 && lzl<bl-1);

					volumetype idxLeaf[4] = {
							(volumetype)((lzl*bl+ly)*bl+lxl),
							(volumetype)((lzl*bl+ly)*bl+lxh),
							(volumetype)((lzh*bl+ly)*bl+lxl),
							(volumetype)((lzh*bl+ly)*bl+lxh)
					};

					for(volumetype i=0;i<4;i++) {
						wf[i] = weights[start+idxLeaf[i]];
						df[i] = distance[start+idxLeaf[i]];
						if(color) cf[i] = color[start+idxLeaf[i]];
					}

					float rxInv = 1.0f-rx;
					float rzInv = 1.0f-rz;

#ifndef WEIGHT_MINIMUM
#ifdef ADD_WEIGHTS_TRANSITION_140424
					w[idx] += rzInv*rxInv*(float)wf[0]+
									  rzInv*rx   *(float)wf[1]+
									  rz   *rxInv*(float)wf[2]+
									  rz   *rx   *(float)wf[3];
#else
					w[idx] = rzInv*rxInv*(float)wf[0]+
									 rzInv*rx   *(float)wf[1]+
									 rz   *rxInv*(float)wf[2]+
									 rz   *rx   *(float)wf[3];
#endif
#else
					w[idx] = std::min(std::min(wf[0],wf[1]),std::min(wf[2],wf[3]));
#endif

					d[idx] = rzInv*rxInv*df[0]+
									 rzInv*rx   *df[1]+
									 rz   *rxInv*df[2]+
									 rz   *rx   *df[3];

					if(color)
#ifndef BRICKVISUALIZATION
#ifndef COLORINVERSION
					c[idx] = VertexColor(
									 rzInv*rxInv*(float)cf[0].x+
									 rzInv*rx   *(float)cf[1].x+
									 rz   *rxInv*(float)cf[2].x+
									 rz   *rx   *(float)cf[3].x,
									 rzInv*rxInv*(float)cf[0].y+
									 rzInv*rx   *(float)cf[1].y+
									 rz   *rxInv*(float)cf[2].y+
									 rz   *rx   *(float)cf[3].y,
									 rzInv*rxInv*(float)cf[0].z+
									 rzInv*rx   *(float)cf[1].z+
									 rz   *rxInv*(float)cf[2].z+
									 rz   *rx   *(float)cf[3].z);
#else
					c[idx] = VertexColor(
									 rzInv*rxInv*(float)cf[0].z+
									 rzInv*rx   *(float)cf[1].z+
									 rz   *rxInv*(float)cf[2].z+
									 rz   *rx   *(float)cf[3].z,
									 rzInv*rxInv*(float)cf[0].y+
									 rzInv*rx   *(float)cf[1].y+
									 rz   *rxInv*(float)cf[2].y+
									 rz   *rx   *(float)cf[3].y,
									 rzInv*rxInv*(float)cf[0].x+
									 rzInv*rx   *(float)cf[1].x+
									 rz   *rxInv*(float)cf[2].x+
									 rz   *rx   *(float)cf[3].x);
#endif

#else
					c[idx] = VertexColor(0,0,65280);
#endif
				}
			}
		}
	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			volumetype idx = (bz*bl)*bl+bx;
			if(weightInfluence(minWeight,
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])){
				int *table = mc.offsetTable[mc.getCubeIndex(
						d[idx],d[idx+1],
						d[idx+bl+1],d[idx+bl],
						d[idx+bs],d[idx+bs+1],
						d[idx+bs+bl+1],d[idx+bs+bl],
						w[idx],w[idx+1],
						w[idx+bl+1],w[idx+bl],
						w[idx+bs],w[idx+bs+1],
						w[idx+bs+bl+1],w[idx+bs+bl])];

				for (unsigned int i=0;table[i]!=-1;i+=3) {
					faceIsSet[3*idx+table[i  ]] = true;
					faceIsSet[3*idx+table[i+1]] = true;
					faceIsSet[3*idx+table[i+2]] = true;
				}
				tables[idx] = table;
			}
		}
	}

	size_t runningIndex = mesh.vertices.size();
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;
			for(sidetype by=0;by<2;by++){
				volumetype idx = (bz*bl+by)*bl+bx;

				if(
						x<ox+size-sizeMin &&
						faceIsSet[3*idx+0]
				 	&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+(x        )*scale,offset.y+y[by]*scale,offset.z+z*scale),
							Vertex3f(offset.x+(x+sizeMin)*scale,offset.y+y[by]*scale,offset.z+z*scale),
							d[idx],d[idx+1]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
					indices[3*idx+0] = runningIndex++;
					vertexIsSet[3*idx+0] = true;
				}
				if(
						faceIsSet[3*idx+1]
						&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+(y[by]            )*scale,offset.z+z*scale),
							Vertex3f(offset.x+x*scale,offset.y+(y[by]+sizeStretch)*scale,offset.z+z*scale),
							d[idx],d[idx+bl]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
					indices[3*idx+1] = runningIndex++;
					vertexIsSet[3*idx+1] = true;
				}
				if(
						z<oz+size-sizeMin &&
					 faceIsSet[3*idx+2]
						&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+y[by]*scale,offset.z+(z        )*scale),
							Vertex3f(offset.x+x*scale,offset.y+y[by]*scale,offset.z+(z+sizeMin)*scale),
							d[idx],d[idx+bs]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
					indices[3*idx+2] = runningIndex++;
					vertexIsSet[3*idx+2] = true;
				}
			}
		}
	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;
			volumetype idx = (bz*bl)*bl+bx;

			int *table = tables[idx];
			if(table){
				for (unsigned int i=0;table[i]!=-1;i+=3) {
					mesh.faces.push_back(indices[3*idx+table[i  ]]);
					mesh.faces.push_back(indices[3*idx+table[i+1]]);
					mesh.faces.push_back(indices[3*idx+table[i+2]]);
				}
			}
		}
	}

	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);


	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}

void addBrickWallZ_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshInterleaved &mesh = *pmesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[4]; float df[4]; colortype3 cf[4];

	volumetype lastleaves[2] = {lastleaf0,lastleaf1};

	sidetype sizeMin = std::min(leafScale[lastleaf0],leafScale[lastleaf1]);
	sidetype sizeStretch = leafScale[lastleaf0];

	if(sizeStretch>size){
		fprintf(stderr,"\nWARNING Wall Z: The Leaf is too large: %i > %i",sizeStretch,size);
		return;
	}

	sidetype z[2] = {(sidetype)(oz+size-sizeStretch) , (sidetype)(oz+size)};

//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy                 )*scale,
//			offset.z+(oz+size-sizeStretch)*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin    )*scale,
//			offset.y+(oy                 )*scale,
//			offset.z+(oz+size-sizeStretch)*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy+size-sizeMin    )*scale,
//			offset.z+(oz+size-sizeStretch)*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin    )*scale,
//			offset.y+(oy+size-sizeMin    )*scale,
//			offset.z+(oz+size-sizeStretch)*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy                 )*scale,
//			offset.z+(oz+size            )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin    )*scale,
//			offset.y+(oy                 )*scale,
//			offset.z+(oz+size            )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy+size-sizeMin    )*scale,
//			offset.z+(oz+size            )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin    )*scale,
//			offset.y+(oy+size-sizeMin    )*scale,
//			offset.z+(oz+size            )*scale));
//
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-3);

	for(sidetype y=oy;y<oy+size;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			for(unsigned int bz=0;bz<2;bz++){
				volumetype idx = (bz*bl+by)*bl+bx;
				w[idx] = 0;
				for(volumetype leaf=lastleaves[bz];leaf<BRANCHINIT
#ifdef ADD_WEIGHTS_TRANSITION_140424
				&& w[idx]<=MIN_WEIGHT_FOR_SURFACE;
#else
				&& w[idx]<=0.0f;
#endif
				leaf=leafParent[leaf]
	//				leaf=BRANCHINIT
				 ){
					volumetype start = leaf*brickSize;
					sidetype3 lo = leafPos[leaf];
					sidetype ls = leafScale[leaf];

					sidetype lz = (z[bz]-lo.z)/ls;

					sidetype lxl = (x-lo.x)/ls;
					sidetype lxr = (x-lo.x)%ls;
					float rx = (float)lxr/(float)ls;
					sidetype lxh = lxl+(rx>0.0 && lxl<bl-1);

					sidetype lyl = (y-lo.y)/ls;
					sidetype lyr = (y-lo.y)%ls;
					float ry = (float)lyr/(float)ls;
					sidetype lyh = lyl+(ry>0.0 && lyl<bl-1);

					volumetype idxLeaf[4] = {
							(volumetype)((lz*bl+lyl)*bl+lxl),
							(volumetype)((lz*bl+lyl)*bl+lxh),
							(volumetype)((lz*bl+lyh)*bl+lxl),
							(volumetype)((lz*bl+lyh)*bl+lxh)
					};

					for(volumetype i=0;i<4;i++) {
						wf[i] = weights[start+idxLeaf[i]];
						df[i] = distance[start+idxLeaf[i]];
						if(color) cf[i] = color[start+idxLeaf[i]];
					}

					float rxInv = 1.0f-rx;
					float ryInv = 1.0f-ry;

#ifndef WEIGHT_MINIMUM
#ifdef ADD_WEIGHTS_TRANSITION_140424
					w[idx] += ryInv*rxInv*(float)wf[0]+
									  ryInv*rx   *(float)wf[1]+
									  ry   *rxInv*(float)wf[2]+
									  ry   *rx   *(float)wf[3];
#else
					w[idx] = ryInv*rxInv*(float)wf[0]+
									 ryInv*rx   *(float)wf[1]+
									 ry   *rxInv*(float)wf[2]+
									 ry   *rx   *(float)wf[3];
#endif
#else
					w[idx] = std::min(std::min(wf[0],wf[1]),std::min(wf[2],wf[3]));
#endif

					d[idx] = ryInv*rxInv*df[0]+
									 ryInv*rx   *df[1]+
									 ry   *rxInv*df[2]+
									 ry   *rx   *df[3];

					if(color)
#ifndef BRICKVISUALIZATION
#ifndef COLORINVERSION
					c[idx] = VertexColor(
									 ryInv*rxInv*(float)cf[0].x+
									 ryInv*rx   *(float)cf[1].x+
									 ry   *rxInv*(float)cf[2].x+
									 ry   *rx   *(float)cf[3].x,
									 ryInv*rxInv*(float)cf[0].y+
									 ryInv*rx   *(float)cf[1].y+
									 ry   *rxInv*(float)cf[2].y+
									 ry   *rx   *(float)cf[3].y,
									 ryInv*rxInv*(float)cf[0].z+
									 ryInv*rx   *(float)cf[1].z+
									 ry   *rxInv*(float)cf[2].z+
									 ry   *rx   *(float)cf[3].z);
#else
					c[idx] = VertexColor(
									 ryInv*rxInv*(float)cf[0].z+
									 ryInv*rx   *(float)cf[1].z+
									 ry   *rxInv*(float)cf[2].z+
									 ry   *rx   *(float)cf[3].z,
									 ryInv*rxInv*(float)cf[0].y+
									 ryInv*rx   *(float)cf[1].y+
									 ry   *rxInv*(float)cf[2].y+
									 ry   *rx   *(float)cf[3].y,
									 ryInv*rxInv*(float)cf[0].x+
									 ryInv*rx   *(float)cf[1].x+
									 ry   *rxInv*(float)cf[2].x+
									 ry   *rx   *(float)cf[3].x);
#endif
#else
						c[idx] = VertexColor(0,0,65280);
#endif
				}
			}
		}
	}

	for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			volumetype idx = by*bl+bx;
			if(weightInfluence(minWeight,
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])){
				int *table = mc.offsetTable[mc.getCubeIndex(
						d[idx],d[idx+1],
						d[idx+bl+1],d[idx+bl],
						d[idx+bs],d[idx+bs+1],
						d[idx+bs+bl+1],d[idx+bs+bl],
						w[idx],w[idx+1],
						w[idx+bl+1],w[idx+bl],
						w[idx+bs],w[idx+bs+1],
						w[idx+bs+bl+1],w[idx+bs+bl])];

				for (unsigned int i=0;table[i]!=-1;i+=3) {
					faceIsSet[3*idx+table[i  ]] = true;
					faceIsSet[3*idx+table[i+1]] = true;
					faceIsSet[3*idx+table[i+2]] = true;
				}
				tables[idx] = table;
			}
		}
	}

	size_t runningIndex = mesh.vertices.size();
	for(sidetype y=oy;y<oy+size;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;
			for(sidetype bz=0;bz<2;bz++){
				volumetype idx = (bz*bl+by)*bl+bx;

				if(
						x<ox+size-sizeMin &&
						faceIsSet[3*idx+0]
				 	&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+(x        )*scale,offset.y+y*scale,offset.z+z[bz]*scale),
							Vertex3f(offset.x+(x+sizeMin)*scale,offset.y+y*scale,offset.z+z[bz]*scale),
							d[idx],d[idx+1]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
					indices[3*idx+0] = runningIndex++;
					vertexIsSet[3*idx+0] = true;
				}
				if(
						y<oy+size-sizeMin &&
						faceIsSet[3*idx+1]
						&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+(y        )*scale,offset.z+z[bz]*scale),
							Vertex3f(offset.x+x*scale,offset.y+(y+sizeMin)*scale,offset.z+z[bz]*scale),
							d[idx],d[idx+bl]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
					indices[3*idx+1] = runningIndex++;
					vertexIsSet[3*idx+1] = true;
				}
				if(
					 faceIsSet[3*idx+2]
						&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+y*scale,offset.z+(z[bz]            )*scale),
							Vertex3f(offset.x+x*scale,offset.y+y*scale,offset.z+(z[bz]+sizeStretch)*scale),
							d[idx],d[idx+bs]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
					mesh.vertices.push_back(ver);
					mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
					indices[3*idx+2] = runningIndex++;
					vertexIsSet[3*idx+2] = true;
				}
			}
		}
	}

	for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			volumetype idx = by*bl+bx;
			int *table = tables[idx];
			if(table){
				for (unsigned int i=0;table[i]!=-1;i+=3) {
					mesh.faces.push_back(indices[3*idx+table[i  ]]);
					mesh.faces.push_back(indices[3*idx+table[i+1]]);
					mesh.faces.push_back(indices[3*idx+table[i+2]]);
				}
			}
		}
	}

	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);

	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}


void addBrickEdgeX_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1, volumetype lastleaf2, volumetype lastleaf3,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshInterleaved &mesh = *pmesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[2]; float df[2]; colortype3 cf[2];


	volumetype lastleaves[4] = {lastleaf0,lastleaf1,lastleaf2,lastleaf3};

	sidetype sizeStretch = std::max(std::max(leafScale[lastleaf0],leafScale[lastleaf1]),
                                  leafScale[lastleaf2]);
	sidetype sizeMin =
	std::min(std::min(leafScale[lastleaf0],leafScale[lastleaf1]),
			     std::min(leafScale[lastleaf2],leafScale[lastleaf3]));

	if(sizeStretch>size){
		fprintf(stderr,"\nWARNING Edge X: The Leaf is too large: %i > %i",sizeStretch,size);
		return;
	}

	sidetype y[4] = {(sidetype)(oy+size-sizeStretch) , (sidetype)(oy+size),
			(sidetype)(oy+size-sizeStretch) , (sidetype)(oy+size)};
	sidetype z[4] = {(sidetype)(oz+size-sizeStretch) , (sidetype)(oz+size-sizeStretch),
			(sidetype)(oz+size) , (sidetype)(oz+size)};


//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy+size-sizeStretch)*scale,
//			offset.z+(oz+size-sizeStretch)*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin    )*scale,
//			offset.y+(oy+size-sizeStretch)*scale,
//			offset.z+(oz+size-sizeStretch)*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy+size            )*scale,
//			offset.z+(oz+size-sizeStretch)*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin)*scale,
//			offset.y+(oy+size            )*scale,
//			offset.z+(oz+size-sizeStretch)*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy+size-sizeStretch)*scale,
//			offset.z+(oz+size            )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin    )*scale,
//			offset.y+(oy+size-sizeStretch)*scale,
//			offset.z+(oz+size            )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox                 )*scale,
//			offset.y+(oy+size            )*scale,
//			offset.z+(oz+size            )*scale));
//	mesh.vertices.push_back(Vertex3f(
//			offset.x+(ox+size-sizeMin    )*scale,
//			offset.y+(oy+size            )*scale,
//			offset.z+(oz+size            )*scale));
//
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//	mesh.colors.push_back(Color3b(255,255,0));
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//
//	mesh.faces.push_back(mesh.vertices.size()-8);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//	mesh.faces.push_back(mesh.vertices.size()-6);
//	mesh.faces.push_back(mesh.vertices.size()-2);
//	mesh.faces.push_back(mesh.vertices.size()-4);
//
//	mesh.faces.push_back(mesh.vertices.size()-7);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-3);
//	mesh.faces.push_back(mesh.vertices.size()-5);
//	mesh.faces.push_back(mesh.vertices.size()-1);
//	mesh.faces.push_back(mesh.vertices.size()-3);

	for(sidetype x=ox;x<ox+size;x+=sizeMin){
		sidetype bx = (x-ox)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = ((i/2)*bl+(i%2))*bl+bx;
			w[idx] = 0;
			for(volumetype leaf=lastleaves[i];leaf<BRANCHINIT
#ifdef ADD_WEIGHTS_TRANSITION_140424
				&& w[idx]<=MIN_WEIGHT_FOR_SURFACE;
#else
				&& w[idx]<=0.0f;
#endif
			leaf=leafParent[leaf]
//				leaf=BRANCHINIT
			 ){
				volumetype start = leaf*brickSize;
				sidetype3 lo = leafPos[leaf];
				sidetype ls = leafScale[leaf];

				sidetype lxl = (x-lo.x)/ls;
				sidetype lxr = (x-lo.x)%ls;
				float rx = (float)lxr/(float)ls;
				sidetype lxh = lxl+(rx>0.0 && lxl<bl-1);

				if(lxl>=bl)
					fprintf(stderr,"\nERROR: Position in Leaf = %i %i vs %i",
							lxl,x,lo.x);

				sidetype ly = (y[i]-lo.y)/ls;
				sidetype lz = (z[i]-lo.z)/ls;

				volumetype idxLeaf[2] = {
						(volumetype)((lz*bl+ly)*bl+lxl),
						(volumetype)((lz*bl+ly)*bl+lxh)
				};

				for(volumetype i=0;i<2;i++) {
					wf[i] = weights[start+idxLeaf[i]];
					df[i] = distance[start+idxLeaf[i]];
					if(color) cf[i] = color[start+idxLeaf[i]];
				}
#ifndef WEIGHT_MINIMUM
#ifdef ADD_WEIGHTS_TRANSITION_140424
				w[idx] += std::max(wf[0] , wf[1]);
#else
				w[idx] = std::max(wf[0] , wf[1]);
#endif
#else
				w[idx] = std::min(wf[0] , wf[1]);
#endif
				d[idx] = (1.0f-rx)*df[0] + rx*df[1];
				if(color)
#ifndef BRICKVISUALIZATION
#ifndef COLORINVERSION
				c[idx] = VertexColor(
						(1.0f-rx)*cf[0].x + rx*cf[1].x,
						(1.0f-rx)*cf[0].y + rx*cf[1].y,
						(1.0f-rx)*cf[0].z + rx*cf[1].z);
#else
				c[idx] = VertexColor(
						(1.0f-rx)*cf[0].z + rx*cf[1].z,
						(1.0f-rx)*cf[0].y + rx*cf[1].y,
						(1.0f-rx)*cf[0].x + rx*cf[1].x);
#endif
#else
					c[idx] = VertexColor(65280,0,0);
#endif

			}
		}
	}

	for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
		sidetype bx = (x-ox)/sizeMin;
		volumetype idx = bx;
		if(weightInfluence(minWeight,
				w[idx],w[idx+1],
				w[idx+bl+1],w[idx+bl],
				w[idx+bs],w[idx+bs+1],
				w[idx+bs+bl+1],w[idx+bs+bl])){
			int *table = mc.offsetTable[mc.getCubeIndex(
					d[idx],d[idx+1],
					d[idx+bl+1],d[idx+bl],
					d[idx+bs],d[idx+bs+1],
					d[idx+bs+bl+1],d[idx+bs+bl],
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])];

			for (unsigned int i=0;table[i]!=-1;i+=3) {
				faceIsSet[3*idx+table[i  ]] = true;
				faceIsSet[3*idx+table[i+1]] = true;
				faceIsSet[3*idx+table[i+2]] = true;
			}
			tables[idx] = table;
		}
	}


	size_t runningIndex = mesh.vertices.size();
	for(sidetype x=ox;x<ox+size;x+=sizeMin){
		sidetype bx = (x-ox)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = ((i/2)*bl+(i%2))*bl+bx;
			if(
					x<ox+size-sizeMin &&
					faceIsSet[3*idx+0]
				&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+(x        )*scale,offset.y+y[i]*scale,offset.z+z[i]*scale),
						Vertex3f(offset.x+(x+sizeMin)*scale,offset.y+y[i]*scale,offset.z+z[i]*scale),
						d[idx],d[idx+1]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
				mesh.vertices.push_back(ver);
				mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
				indices[3*idx+0] = runningIndex++;
				vertexIsSet[3*idx+0] = true;
			}
			if(
					faceIsSet[3*idx+1]
					&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x*scale,offset.y+(y[i]            )*scale,offset.z+z[i]*scale),
						Vertex3f(offset.x+x*scale,offset.y+(y[i]+sizeStretch)*scale,offset.z+z[i]*scale),
						d[idx],d[idx+bl]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
				mesh.vertices.push_back(ver);
				mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
				indices[3*idx+1] = runningIndex++;
				vertexIsSet[3*idx+1] = true;
			}
			if(
				 faceIsSet[3*idx+2]
					&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x*scale,offset.y+y[i]*scale,offset.z+(z[i]            )*scale),
						Vertex3f(offset.x+x*scale,offset.y+y[i]*scale,offset.z+(z[i]+sizeStretch)*scale),
						d[idx],d[idx+bs]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
				mesh.vertices.push_back(ver);
				mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
				indices[3*idx+2] = runningIndex++;
				vertexIsSet[3*idx+2] = true;
			}
		}
	}

	for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
		sidetype bx = (x-ox)/sizeMin;
		volumetype idx = bx;
		int *table = tables[idx];
		if(table){
			for (unsigned int i=0;table[i]!=-1;i+=3) {
				mesh.faces.push_back(indices[3*idx+table[i  ]]);
				mesh.faces.push_back(indices[3*idx+table[i+1]]);
				mesh.faces.push_back(indices[3*idx+table[i+2]]);
			}
		}
	}

	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);


	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}


void addBrickEdgeY_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1, volumetype lastleaf2, volumetype lastleaf3,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshInterleaved &mesh = *pmesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[2]; float df[2]; colortype3 cf[2];

	volumetype lastleaves[4] = {lastleaf0,lastleaf1,lastleaf2,lastleaf3};

	sidetype sizeStretch = std::max(std::max(leafScale[lastleaf0],leafScale[lastleaf1]),
                                  leafScale[lastleaf2]);
	sidetype sizeMin =
	std::min(std::min(leafScale[lastleaf0],leafScale[lastleaf1]),
			     std::min(leafScale[lastleaf2],leafScale[lastleaf3]));

	if(sizeStretch>size){
		fprintf(stderr,"\nWARNING Edge Y: The Leaf is too large: %i > %i",sizeStretch,size);
		return;
	}

	sidetype x[4] = {(sidetype)(ox+size-sizeStretch) , (sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch) , (sidetype)(ox+size)};
	sidetype z[4] = {(sidetype)(oz+size-sizeStretch) , (sidetype)(oz+size-sizeStretch),
			(sidetype)(oz+size) , (sidetype)(oz+size)};


	for(sidetype y=oy;y<oy+size;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = ((i/2)*bl+by)*bl+(i%2);
			w[idx] = 0;
			for(volumetype leaf=lastleaves[i];leaf<BRANCHINIT
#ifdef ADD_WEIGHTS_TRANSITION_140424
				&& w[idx]<=MIN_WEIGHT_FOR_SURFACE;
#else
				&& w[idx]<=0.0f;
#endif
			leaf=leafParent[leaf]
//				leaf=BRANCHINIT
			 ){
				volumetype start = leaf*brickSize;
				sidetype3 lo = leafPos[leaf];
				sidetype ls = leafScale[leaf];

				sidetype lyl = (y-lo.y)/ls;
				sidetype lyr = (y-lo.y)%ls;
				float ry = (float)lyr/(float)ls;
				sidetype lyh = lyl+(ry>0.0 && lyl<bl-1);

				sidetype lx = (x[i]-lo.x)/ls;
				sidetype lz = (z[i]-lo.z)/ls;

				volumetype idxLeaf[2] = {
						(volumetype)((lz*bl+lyl)*bl+lx),
						(volumetype)((lz*bl+lyh)*bl+lx)
				};

				for(volumetype i=0;i<2;i++) {
					wf[i] = weights[start+idxLeaf[i]];
					df[i] = distance[start+idxLeaf[i]];
					if(color) cf[i] = color[start+idxLeaf[i]];
				}

//				w[idx] = (1.0f-ry)*(float)wf[0] + ry*(float)wf[1];
#ifndef WEIGHT_MINIMUM
#ifdef ADD_WEIGHTS_TRANSITION_140424
				w[idx] += std::max(wf[0] , wf[1]);
#else
				w[idx] = std::max(wf[0] , wf[1]);
#endif
#else
				w[idx] = std::min(wf[0] , wf[1]);
#endif
				d[idx] = (1.0f-ry)*df[0] + ry*df[1];
				if(color)
#ifndef BRICKVISUALIZATION
#ifndef COLORINVERSION
				c[idx] = VertexColor(
						(1.0f-ry)*cf[0].x + ry*cf[1].x,
						(1.0f-ry)*cf[0].y + ry*cf[1].y,
						(1.0f-ry)*cf[0].z + ry*cf[1].z);
#else
				c[idx] = VertexColor(
						(1.0f-ry)*cf[0].z + ry*cf[1].z,
						(1.0f-ry)*cf[0].y + ry*cf[1].y,
						(1.0f-ry)*cf[0].x + ry*cf[1].x);
#endif
#else
					c[idx] = VertexColor(65280,0,0);
#endif

			}
		}
	}

	for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		volumetype idx = by*bl;
		if(weightInfluence(minWeight,
				w[idx],w[idx+1],
				w[idx+bl+1],w[idx+bl],
				w[idx+bs],w[idx+bs+1],
				w[idx+bs+bl+1],w[idx+bs+bl])){
			int *table = mc.offsetTable[mc.getCubeIndex(
					d[idx],d[idx+1],
					d[idx+bl+1],d[idx+bl],
					d[idx+bs],d[idx+bs+1],
					d[idx+bs+bl+1],d[idx+bs+bl],
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])];

			for (unsigned int i=0;table[i]!=-1;i+=3) {
				faceIsSet[3*idx+table[i  ]] = true;
				faceIsSet[3*idx+table[i+1]] = true;
				faceIsSet[3*idx+table[i+2]] = true;
			}
			tables[idx] = table;
		}
	}


	size_t runningIndex = mesh.vertices.size();
	for(sidetype y=oy;y<oy+size;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = ((i/2)*bl+by)*bl+(i%2);
			if(
					faceIsSet[3*idx+0]
				&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+(x[i]            )*scale,offset.y+y*scale,offset.z+z[i]*scale),
						Vertex3f(offset.x+(x[i]+sizeStretch)*scale,offset.y+y*scale,offset.z+z[i]*scale),
						d[idx],d[idx+1]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
				mesh.vertices.push_back(ver);
				mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
				indices[3*idx+0] = runningIndex++;
				vertexIsSet[3*idx+0] = true;
			}
			if(
					y<oy+size-sizeMin &&
					faceIsSet[3*idx+1]
					&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x[i]*scale,offset.y+(y        )*scale,offset.z+z[i]*scale),
						Vertex3f(offset.x+x[i]*scale,offset.y+(y+sizeMin)*scale,offset.z+z[i]*scale),
						d[idx],d[idx+bl]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
				mesh.vertices.push_back(ver);
				mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
				indices[3*idx+1] = runningIndex++;
				vertexIsSet[3*idx+1] = true;
			}
			if(
				 faceIsSet[3*idx+2]
					&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x[i]*scale,offset.y+y*scale,offset.z+(z[i]            )*scale),
						Vertex3f(offset.x+x[i]*scale,offset.y+y*scale,offset.z+(z[i]+sizeStretch)*scale),
						d[idx],d[idx+bs]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
				mesh.vertices.push_back(ver);
				mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
				indices[3*idx+2] = runningIndex++;
				vertexIsSet[3*idx+2] = true;
			}
		}
	}



	for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		volumetype idx = by*bl;
		int *table = tables[idx];
		if(table){
			for (unsigned int i=0;table[i]!=-1;i+=3) {
				mesh.faces.push_back(indices[3*idx+table[i  ]]);
				mesh.faces.push_back(indices[3*idx+table[i+1]]);
				mesh.faces.push_back(indices[3*idx+table[i+2]]);
			}
		}
	}


	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);


	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}


void addBrickEdgeZ_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1, volumetype lastleaf2, volumetype lastleaf3,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshInterleaved &mesh = *pmesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[2]; float df[2]; colortype3 cf[2];

	volumetype lastleaves[4] = {lastleaf0,lastleaf1,lastleaf2,lastleaf3};

	sidetype sizeStretch = std::max(std::max(leafScale[lastleaf0],leafScale[lastleaf1]),
                                  leafScale[lastleaf2]);
	sidetype sizeMin =
	std::min(std::min(leafScale[lastleaf0],leafScale[lastleaf1]),
			     std::min(leafScale[lastleaf2],leafScale[lastleaf3]));

	if(sizeStretch>size){
		fprintf(stderr,"\nWARNING Edge Z: The Leaf is too large: %i > %i",sizeStretch,size);
		return;
	}

	sidetype x[4] = {(sidetype)(ox+size-sizeStretch) , (sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch) , (sidetype)(ox+size)};
	sidetype y[4] = {(sidetype)(oy+size-sizeStretch) , (sidetype)(oy+size-sizeStretch),
			(sidetype)(oy+size) , (sidetype)(oy+size)};


	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = (bz*bl+(i/2))*bl+(i%2);
			w[idx] = 0;
			for(volumetype leaf=lastleaves[i];leaf<BRANCHINIT
#ifdef ADD_WEIGHTS_TRANSITION_140424
				&& w[idx]<=MIN_WEIGHT_FOR_SURFACE;
#else
				&& w[idx]<=0.0f;
#endif
			leaf=leafParent[leaf]
//				leaf=BRANCHINIT
			 ){
				volumetype start = leaf*brickSize;
				sidetype3 lo = leafPos[leaf];
				sidetype ls = leafScale[leaf];

				sidetype lzl = (z-lo.z)/ls;
				sidetype lzr = (z-lo.z)%ls;
				float rz = (float)lzr/(float)ls;
				sidetype lzh = lzl+(rz>0.0 && lzl<bl-1);

				sidetype lx = (x[i]-lo.x)/ls;
				sidetype ly = (y[i]-lo.y)/ls;

				volumetype idxLeaf[2] = {
						(volumetype)((lzl*bl+ly)*bl+lx),
						(volumetype)((lzh*bl+ly)*bl+lx)
				};

				for(volumetype i=0;i<2;i++) {
					wf[i] = weights[start+idxLeaf[i]];
					df[i] = distance[start+idxLeaf[i]];
					if(color) cf[i] = color[start+idxLeaf[i]];
				}

//				w[idx] = (1.0f-rz)*(float)wf[0] + rz*(float)wf[1];
#ifndef WEIGHT_MINIMUM
#ifdef ADD_WEIGHTS_TRANSITION_140424
				w[idx] += std::max(wf[0] , wf[1]);
#else
				w[idx] = std::max(wf[0] , wf[1]);
#endif
#else
				w[idx] = std::min(wf[0] , wf[1]);
#endif
				d[idx] = (1.0f-rz)*df[0] + rz*df[1];
				if(color)
#ifndef BRICKVISUALIZATION
#ifndef COLORINVERSION
				c[idx] = VertexColor(
						(1.0f-rz)*cf[0].x + rz*cf[1].x,
						(1.0f-rz)*cf[0].y + rz*cf[1].y,
						(1.0f-rz)*cf[0].z + rz*cf[1].z);
#else
				c[idx] = VertexColor(
						(1.0f-rz)*cf[0].z + rz*cf[1].z,
						(1.0f-rz)*cf[0].y + rz*cf[1].y,
						(1.0f-rz)*cf[0].x + rz*cf[1].x);
#endif
#else
					c[idx] = VertexColor(65280,0,0);
#endif
			}
		}
	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		volumetype idx = bz*bs;
		if(weightInfluence(minWeight,
				w[idx],w[idx+1],
				w[idx+bl+1],w[idx+bl],
				w[idx+bs],w[idx+bs+1],
				w[idx+bs+bl+1],w[idx+bs+bl])){
			int *table = mc.offsetTable[mc.getCubeIndex(
					d[idx],d[idx+1],
					d[idx+bl+1],d[idx+bl],
					d[idx+bs],d[idx+bs+1],
					d[idx+bs+bl+1],d[idx+bs+bl],
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])];

			for (unsigned int i=0;table[i]!=-1;i+=3) {
				faceIsSet[3*idx+table[i  ]] = true;
				faceIsSet[3*idx+table[i+1]] = true;
				faceIsSet[3*idx+table[i+2]] = true;
			}
			tables[idx] = table;
		}
	}


	size_t runningIndex = mesh.vertices.size();
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = (bz*bl+(i/2))*bl+(i%2);
			if(
					faceIsSet[3*idx+0]
				&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+(x[i]            )*scale,offset.y+y[i]*scale,offset.z+z*scale),
						Vertex3f(offset.x+(x[i]+sizeStretch)*scale,offset.y+y[i]*scale,offset.z+z*scale),
						d[idx],d[idx+1]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
				mesh.vertices.push_back(ver);
				mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
				indices[3*idx+0] = runningIndex++;
				vertexIsSet[3*idx+0] = true;
			}
			if(
					faceIsSet[3*idx+1]
					&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x[i]*scale,offset.y+(y[i]            )*scale,offset.z+z*scale),
						Vertex3f(offset.x+x[i]*scale,offset.y+(y[i]+sizeStretch)*scale,offset.z+z*scale),
						d[idx],d[idx+bl]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
				mesh.vertices.push_back(ver);
				mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
				indices[3*idx+1] = runningIndex++;
				vertexIsSet[3*idx+1] = true;
			}
			if(
				 z<oz+size-sizeMin &&
				 faceIsSet[3*idx+2]
					&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x[i]*scale,offset.y+y[i]*scale,offset.z+(z        )*scale),
						Vertex3f(offset.x+x[i]*scale,offset.y+y[i]*scale,offset.z+(z+sizeMin)*scale),
						d[idx],d[idx+bs]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
				mesh.vertices.push_back(ver);
				mesh.colors.push_back(Color3b(col.x/COLOR_MULTIPLICATOR,col.y/COLOR_MULTIPLICATOR,col.z/COLOR_MULTIPLICATOR));
				indices[3*idx+2] = runningIndex++;
				vertexIsSet[3*idx+2] = true;
			}
		}
	}



	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		volumetype idx = bz*bs;
		int *table = tables[idx];
		if(table){
			for (unsigned int i=0;table[i]!=-1;i+=3) {
				mesh.faces.push_back(indices[3*idx+table[i  ]]);
				mesh.faces.push_back(indices[3*idx+table[i+1]]);
				mesh.faces.push_back(indices[3*idx+table[i+2]]);
			}
		}
	}


	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);
	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}



//TODO: Das ist noch nicht wirklich indiziert
void addBrickMiddle_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1, volumetype lastleaf2, volumetype lastleaf3,
		volumetype lastleaf4, volumetype lastleaf5, volumetype lastleaf6, volumetype lastleaf7,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshInterleaved &mesh = *pmesh;

	float d[8];
	weighttype w[8] = {0,0,0,0,0,0,0,0};
	VertexColor c[8];

	Vertex3f vertlist[12];
	VertexColor collist[12];


	volumetype lastleaves[8] = {lastleaf0,lastleaf1,lastleaf2,lastleaf3,
			lastleaf4,lastleaf5,lastleaf6,lastleaf7};

	sidetype sizeStretch = leafScale[lastleaf0];
	for(unsigned int i=1;i<7;i++){
		if(sizeStretch < leafScale[lastleaves[i]])
			 sizeStretch = leafScale[lastleaves[i]];
	}
//	sidetype sizeMin = std::min(sizeStretch,leafScale[lastleaf7]);

	if(sizeStretch>size){
		fprintf(stderr,"\nWARNING Wall Z: The Leaf is too large: %i > %i",sizeStretch,size);
		return;
	}

	sidetype x[8] = {
			(sidetype)(ox+size-sizeStretch), (sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch), (sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch), (sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch), (sidetype)(ox+size)
	};

	sidetype y[8] = {
			(sidetype)(oy+size-sizeStretch), (sidetype)(oy+size-sizeStretch),
			(sidetype)(oy+size), (sidetype)(oy+size),
			(sidetype)(oy+size-sizeStretch), (sidetype)(oy+size-sizeStretch),
			(sidetype)(oy+size), (sidetype)(oy+size)
	};

	sidetype z[8] = {
			(sidetype)(oz+size-sizeStretch), (sidetype)(oz+size-sizeStretch),
			(sidetype)(oz+size-sizeStretch), (sidetype)(oz+size-sizeStretch),
			(sidetype)(oz+size), (sidetype)(oz+size),
			(sidetype)(oz+size), (sidetype)(oz+size)
	};

//	unsigned int s = mesh.x.size();
//	mesh.x.push_back(offset.x+x[0]*scale);
//	mesh.y.push_back(offset.y+y[0]*scale);
//	mesh.z.push_back(offset.z+z[0]*scale);
//	mesh.x.push_back(offset.x+x[1]*scale);
//	mesh.y.push_back(offset.y+y[1]*scale);
//	mesh.z.push_back(offset.z+z[1]*scale);
//	mesh.x.push_back(offset.x+x[2]*scale);
//	mesh.y.push_back(offset.y+y[2]*scale);
//	mesh.z.push_back(offset.z+z[2]*scale);
//	mesh.x.push_back(offset.x+x[3]*scale);
//	mesh.y.push_back(offset.y+y[3]*scale);
//	mesh.z.push_back(offset.z+z[3]*scale);
//	mesh.x.push_back(offset.x+x[4]*scale);
//	mesh.y.push_back(offset.y+y[4]*scale);
//	mesh.z.push_back(offset.z+z[4]*scale);
//	mesh.x.push_back(offset.x+x[5]*scale);
//	mesh.y.push_back(offset.y+y[5]*scale);
//	mesh.z.push_back(offset.z+z[5]*scale);
//	mesh.x.push_back(offset.x+x[6]*scale);
//	mesh.y.push_back(offset.y+y[6]*scale);
//	mesh.z.push_back(offset.z+z[6]*scale);
//	mesh.x.push_back(offset.x+x[7]*scale);
//	mesh.y.push_back(offset.y+y[7]*scale);
//	mesh.z.push_back(offset.z+z[7]*scale);
//
//	mesh.r.push_back(0); mesh.g.push_back(255); mesh.b.push_back(0);
//	mesh.r.push_back(0); mesh.g.push_back(255); mesh.b.push_back(0);
//	mesh.r.push_back(0); mesh.g.push_back(255); mesh.b.push_back(0);
//	mesh.r.push_back(0); mesh.g.push_back(255); mesh.b.push_back(0);
//	mesh.r.push_back(0); mesh.g.push_back(255); mesh.b.push_back(0);
//	mesh.r.push_back(0); mesh.g.push_back(255); mesh.b.push_back(0);
//	mesh.r.push_back(0); mesh.g.push_back(255); mesh.b.push_back(0);
//	mesh.r.push_back(0); mesh.g.push_back(255); mesh.b.push_back(0);
//
//	mesh.f.push_back(s+0); mesh.f.push_back(s+1); mesh.f.push_back(s+3);
//	mesh.f.push_back(s+0); mesh.f.push_back(s+3); mesh.f.push_back(s+2);
//	mesh.f.push_back(s+0); mesh.f.push_back(s+2); mesh.f.push_back(s+6);
//	mesh.f.push_back(s+0); mesh.f.push_back(s+6); mesh.f.push_back(s+4);
////	mesh.f.push_back(s+0); mesh.f.push_back(s+4); mesh.f.push_back(s+5);
////	mesh.f.push_back(s+0); mesh.f.push_back(s+5); mesh.f.push_back(s+1);
//
//	mesh.f.push_back(s+4); mesh.f.push_back(s+5); mesh.f.push_back(s+7);
//	mesh.f.push_back(s+4); mesh.f.push_back(s+7); mesh.f.push_back(s+6);
//	mesh.f.push_back(s+1); mesh.f.push_back(s+3); mesh.f.push_back(s+7);
//	mesh.f.push_back(s+1); mesh.f.push_back(s+7); mesh.f.push_back(s+5);
////	mesh.f.push_back(s+2); mesh.f.push_back(s+6); mesh.f.push_back(s+7);
////	mesh.f.push_back(s+2); mesh.f.push_back(s+7); mesh.f.push_back(s+3);
////	return;


	for(unsigned int i=0;i<8;i++){
		for(volumetype leaf=lastleaves[i];leaf<BRANCHINIT
#ifdef ADD_WEIGHTS_TRANSITION_140424
				&& w[i]<=MIN_WEIGHT_FOR_SURFACE;
#else
				&& w[i]<=0.0f;
#endif
		leaf=leafParent[leaf]
//				leaf=BRANCHINIT
		 ){
			volumetype start = leaf*brickSize;
			sidetype3 lo = leafPos[leaf];
			sidetype ls = leafScale[leaf];

			sidetype lx = (x[i]-lo.x)/ls;
			sidetype ly = (y[i]-lo.y)/ls;
			sidetype lz = (z[i]-lo.z)/ls;

#ifdef ADD_WEIGHTS_TRANSITION_140424
			w[i] += weights[start+(lz*bl+ly)*bl+lx];
#else
			w[i] = weights[start+(lz*bl+ly)*bl+lx];
#endif
			d[i] = distance[start+(lz*bl+ly)*bl+lx];
			if(color) c[i] = VertexColor(
					color[start+(lz*bl+ly)*bl+lx].x,
					color[start+(lz*bl+ly)*bl+lx].y,
					color[start+(lz*bl+ly)*bl+lx].z);
		}
	}
	if(weightInfluence(minWeight,
			w[0],w[1],w[2],w[3],w[4],w[5],w[6],w[7])){
		int *table = mc.getColoredTrianglesForCubePrecomputed(vertlist,collist,0,0,0,
				offset.x+(ox+size-sizeStretch)*scale,
				offset.y+(oy+size-sizeStretch)*scale,
				offset.z+(oz+size-sizeStretch)*scale,
				scale*sizeStretch,scale*sizeStretch,scale*sizeStretch,
				d[0],d[1],d[3],d[2],d[4],d[5],d[7],d[6],
				w[0],w[1],w[3],w[2],w[4],w[5],w[7],w[6],
				c[0],c[1],c[3],c[2],c[4],c[5],c[7],c[6],
				color);
		for (unsigned int i=0;table[i]!=-1;i+=3) {
			Vertex3f v0 = vertlist[table[i  ]];
			Vertex3f v1 = vertlist[table[i+1]];
			Vertex3f v2 = vertlist[table[i+2]];
			if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
				degenerate_faces++;
			}
			else{
				mesh.vertices.push_back(v0);
				mesh.faces.push_back(mesh.vertices.size()-1);
				mesh.vertices.push_back(v1);
				mesh.faces.push_back(mesh.vertices.size()-1);
				mesh.vertices.push_back(v2);
				mesh.faces.push_back(mesh.vertices.size()-1);

				if(color){
#ifndef BRICKVISUALIZATION
#ifndef COLORINVERSION
					VertexColor c0 = collist[table[i  ]];
					VertexColor c1 = collist[table[i+1]];
					VertexColor c2 = collist[table[i+2]];
#else
					VertexColor c0 = collist[table[i+1]];
					VertexColor c1 = collist[table[i+1]];
					VertexColor c2 = collist[table[i  ]];
#endif
#else
					VertexColor c0 = VertexColor(0,65280,0);
					VertexColor c1 = VertexColor(0,65280,0);
					VertexColor c2 = VertexColor(0,65280,0);
#endif
					mesh.colors.push_back(Color3b(c0.x/COLOR_MULTIPLICATOR,c0.y/COLOR_MULTIPLICATOR,c0.z/COLOR_MULTIPLICATOR));
					mesh.colors.push_back(Color3b(c1.x/COLOR_MULTIPLICATOR,c1.y/COLOR_MULTIPLICATOR,c1.z/COLOR_MULTIPLICATOR));
					mesh.colors.push_back(Color3b(c2.x/COLOR_MULTIPLICATOR,c2.y/COLOR_MULTIPLICATOR,c2.z/COLOR_MULTIPLICATOR));
				}
			}
		}
	}
}








