/*
 * treeandbrick_incremental.cpp
 *
 *  Created on: Mar 1, 2013
 *      Author: steinbrf
 */

#include "treeandbrick.hpp"
#include "treeandbrick_incremental.hpp"
#include "mesh_interleaved_meshcell.hpp"

#include "pmmintrin.h"



//########################## OwnParentArray_ ############################

OwnParentArray_::OwnParentArray_()
:_array(NULL), _size(0),_capacity(0)
{}

OwnParentArray_::~OwnParentArray_(){
	fprintf(stderr,"\nDestroying Leaf Parent Array");
 clear();
	fprintf(stderr,"\nLeaf Parent Array destroyed");
}

volumetype &OwnParentArray_::operator[](size_t pos){
	if(pos < _size){
		return _array[pos];
	}
	else{
		fprintf(stderr,"\nERROR: Wrong Index in MeshCellNeighborhood Array! %li >= %li",pos,_size);
		return _dummy;
	}
}

const volumetype &OwnParentArray_::operator[](size_t pos) const {
	if(pos < _size){
		return _array[pos];
	}
	else{
		fprintf(stderr,"\nERROR: Wrong Index in MeshCellNeighborhood Array! %li >= %li",pos,_size);
		return _dummy;
	}
}

size_t OwnParentArray_::size() const {
	return _size;
}

size_t OwnParentArray_::capacity() const {
	return _capacity;
}

void OwnParentArray_::resize(size_t count, volumetype value){
	reserve(count);
	for(size_t i=_size;i<count;i++){
		_array[i] = value;
	}
	_size = count;
}

void OwnParentArray_::reserve(size_t count){
	if(count > _capacity){
		//TODO: Possible std::vector-like power of 2
		size_t capacity = count;

		volumetype *array = NULL;

		//The C++-Version
		array = new volumetype[capacity];
		if(_capacity){
			fprintf(stderr,"\nWARNING: Actually resizing MeshCellNeighborhood Array => INEFFICIENT!");
			if(_size){
				for(size_t i=0;i<_size;i++) array[i] = _array[i];
			}
			delete [] _array;
		}

		_array = array;
		_capacity = capacity;
	}
}

void OwnParentArray_::clear(){
	if(_array) delete [] _array;
	_array = NULL;
	_size = _capacity = 0;
}

void OwnParentArray_::push_back(const volumetype &value){
	if(_size==_capacity){
		resize(_size+1,value);
	}
	else{
		_array[_size] = value;
		_size++;
	}
}

volumetype &OwnParentArray_::back(){
	if(_size){
		return _array[_size-1];
	}
	else{
		fprintf(stderr,"\nERROR: LeafIndices Array back() with no elements!");
		return _dummy;
	}
}

OwnParentArray_ &OwnParentArray_::operator=
		(const OwnParentArray_ &value){
	if(_capacity<value._size){
		delete [] _array;
		_capacity = value._capacity;
		_array = new volumetype[_capacity];
	}
	_size = value._size;
	for(size_t i=0;i<_size;i++){
		_array[i] = value._array[i];
	}
	return *this;
}



//########################## MeshCell               ############################

MeshCell::MeshCell()
: type(8),
//  mesh(NULL),
  meshinterleaved(NULL ),
  ox(0), oy(0), oz(0), size(0)
{
	lastLeaf[0] = lastLeaf[1] = lastLeaf[2] = lastLeaf[3] =
	lastLeaf[4] = lastLeaf[5] = lastLeaf[6] = lastLeaf[7] = BRANCHINIT;
	lastBranch[0] = lastBranch[1] = lastBranch[2] = lastBranch[3] =
	lastBranch[4] = lastBranch[5] = lastBranch[6] = lastBranch[7] = BRANCHINIT;

}

MeshCell::MeshCell(int type_p, sidetype ox_p, sidetype oy_p, sidetype oz_p, sidetype size_p,
		volumetype lastleaf0_p, volumetype lastleaf1_p,
		volumetype lastleaf2_p, volumetype lastleaf3_p,
		volumetype lastleaf4_p, volumetype lastleaf5_p,
		volumetype lastleaf6_p, volumetype lastleaf7_p)
: type(type_p),
//  mesh(new MeshSeparate(3)),
//  mesh(NULL),
  meshinterleaved(new MeshInterleaved(3)),
  ox(ox_p), oy(oy_p), oz(oz_p), size(size_p)
// ,lastleaf0(lastleaf0_p),lastleaf1(lastleaf1_p),lastleaf2(lastleaf2_p),lastleaf3(lastleaf3_p),
//  lastleaf4(lastleaf4_p),lastleaf5(lastleaf5_p),lastleaf6(lastleaf6_p),lastleaf7(lastleaf7_p),
//  lastBranch0(BRANCHINIT),lastBranch1(BRANCHINIT),lastBranch2(BRANCHINIT),lastBranch3(BRANCHINIT),
//  lastBranch4(BRANCHINIT),lastBranch5(BRANCHINIT),lastBranch6(BRANCHINIT),lastBranch7(BRANCHINIT)
{
//	lastleaf0 = lastleaf0_p; lastleaf1 = lastleaf1_p;
//	lastleaf2 = lastleaf2_p; lastleaf3 = lastleaf3_p;
//	lastleaf4 = lastleaf4_p; lastleaf5 = lastleaf5_p;
//	lastleaf6 = lastleaf6_p; lastleaf7 = lastleaf7_p;
//	lastBranch0 = lastBranch1 = lastBranch2 = lastBranch3 =
//	lastBranch4 = lastBranch5 = lastBranch6 = lastBranch7 = BRANCHINIT;
//
	lastLeaf[0] = lastleaf0_p; lastLeaf[1] = lastleaf1_p;
	lastLeaf[2] = lastleaf2_p; lastLeaf[3] = lastleaf3_p;
	lastLeaf[4] = lastleaf4_p; lastLeaf[5] = lastleaf5_p;
	lastLeaf[6] = lastleaf6_p; lastLeaf[7] = lastleaf7_p;
	lastBranch[0] = lastBranch[1] = lastBranch[2] = lastBranch[3] =
	lastBranch[4] = lastBranch[5] = lastBranch[6] = lastBranch[7] = BRANCHINIT;

}



MCNSplit::MCNSplit()
//:
//	indices(new CellList[42]),
//	indexWallWall(indices[0]),
//  indexWallEdge(indices[6]),
//  indexWallCorner(indices[12]),
//  indexEdgeEdge(indices[18]),
//  indexEdgeCorner(indices[30])
{

//	indices = new CellList[42];
	indexInterior =

//			indexWall[0] = indexWall[1] = indexWall[2] = indexWall[3] = indexWall[4] = indexWall[5] =
//
//			indexEdge[0] = indexEdge[1] = indexEdge[2]  = indexEdge[3]  =
//			indexEdge[4] = indexEdge[5] = indexEdge[6]  = indexEdge[7]  =
//			indexEdge[8] = indexEdge[9] = indexEdge[10] = indexEdge[11] =


//
			indexCorner[0] = indexCorner[1] = indexCorner[2] = indexCorner[3] =
	    indexCorner[4] = indexCorner[5] = indexCorner[6] = indexCorner[7] =

	    MAXCELLINDEX;

//	for(size_t i=0;i<6;i++){
//		indexWallWall[i].reserve(8);
//		indexWallEdge[i].reserve(8);
//		indexWallCorner[i].reserve(4);
//		indexEdgeEdge[i].reserve(4);
//		indexEdgeCorner[i].reserve(4);
//	}
//	for(size_t i=6;i<12;i++){
//		indexEdgeEdge[i].reserve(4);
//		indexEdgeCorner[i].reserve(4);
//	}
}

MCNSplit::MCNSplit(const MCNCompact &compact)
{
	indexInterior  = compact.indexInterior;
	indexCorner[0] = compact.indexCorner[0];
	indexCorner[1] = compact.indexCorner[1];
	indexCorner[2] = compact.indexCorner[2];
	indexCorner[3] = compact.indexCorner[3];
  indexCorner[4] = compact.indexCorner[4];
  indexCorner[5] = compact.indexCorner[5];
  indexCorner[6] = compact.indexCorner[6];
  indexCorner[7] = compact.indexCorner[7];

	for(size_t i=0;i<compact.border_size();i++){
		MCNCompact::CellReferenceNum num = compact.get(i);
		MCNCompact::vecpair pair = MCNCompact::decnum(num.first);
		switch(pair.vectype){
		case VECTYPE_WALLWALL:
		indexWallWall[pair.vecindex].push_back(num.second);
		break;
		case VECTYPE_WALLEDGE:
		indexWallEdge[pair.vecindex].push_back(num.second);
		break;
		case VECTYPE_WALLCORN:
		indexWallCorner[pair.vecindex].push_back(num.second);
		break;
		case VECTYPE_EDGEEDGE:
		indexEdgeEdge[pair.vecindex].push_back(num.second);
		break;
		case VECTYPE_EDGECORN:
		indexEdgeCorner[pair.vecindex].push_back(num.second);
		break;
		}
	}
}


//size_t MeshCellNeighborhood::emptysize(){
//	return sizeof(CellList) * 42 + (1+8)*sizeof(size_t);
//}

#define CAPACITY_MEMORY_ASSESSMENT

size_t MCNSplit::size()
{
	size_t capacities = 0;
	for(size_t i=0;i<6;i++){

#ifdef CAPACITY_MEMORY_ASSESSMENT
		capacities += indexWallWall[i].capacity() +
									indexWallEdge[i].capacity() +
									indexWallCorner[i].capacity() +
									indexEdgeEdge[i].capacity() +
									indexEdgeCorner[i].capacity();
#else
		capacities += indexWallWall[i].size() +
									indexWallEdge[i].size() +
									indexWallCorner[i].size() +
									indexEdgeEdge[i].size() +
									indexEdgeCorner[i].size();
#endif
	}
	for(size_t i=6;i<12;i++){

#ifdef CAPACITY_MEMORY_ASSESSMENT
		capacities += indexEdgeEdge[i].capacity() +
									indexEdgeCorner[i].capacity();
#else
		capacities += indexEdgeEdge[i].size() +
									indexEdgeCorner[i].size();
#endif
	}
	return sizeof(size_t) * capacities + sizeof(MCNSplit);
}


//MeshCellNeighborhood &MeshCellNeighborhood::operator=(const MeshCellNeighborhood &value){
//	fprintf(stderr,"\nMeshCellNeighborhood Assignment operator start");
//	indexInterior = value.indexInterior;
//	fprintf(stderr,"\nTEst1");
//	for(int i=0;i<6;i++){
//		indexWallWall[i]   = value.indexWallWall[i];
//		indexWallEdge[i]   = value.indexWallEdge[i];
//		indexWallCorner[i] = value.indexWallCorner[i];
//		indexEdgeEdge[i]   = value.indexEdgeEdge[i];
//		indexEdgeCorner[i] = value.indexEdgeCorner[i];
//	}
//	fprintf(stderr,"\nTEst2");
//	for(int i=6;i<12;i++){
//		indexEdgeEdge[i]   = value.indexEdgeEdge[i];
//		indexEdgeCorner[i] = value.indexEdgeCorner[i];
//	}
//	fprintf(stderr,"\nTEst3");
//	for(int i=0;i<8;i++){
//		indexCorner[i] = value.indexCorner[i];
//	}
//	fprintf(stderr,"\nMeshCellNeighborhood Assignment operator done");
//	return *this;
//}



MCNCompact::MCNCompact(){
	indexInterior =
	indexCorner[0] = indexCorner[1] = indexCorner[2] = indexCorner[3] =
  indexCorner[4] = indexCorner[5] = indexCorner[6] = indexCorner[7] =
  MAXCELLINDEX;
}

MCNCompact::MCNCompact(const MCNSplit &split)
{
	indexInterior  = split.indexInterior;
	indexCorner[0] = split.indexCorner[0];
	indexCorner[1] = split.indexCorner[1];
	indexCorner[2] = split.indexCorner[2];
	indexCorner[3] = split.indexCorner[3];
  indexCorner[4] = split.indexCorner[4];
  indexCorner[5] = split.indexCorner[5];
  indexCorner[6] = split.indexCorner[6];
  indexCorner[7] = split.indexCorner[7];

	for(uchar vecindex=0;vecindex<6;vecindex++){
		for(size_t i=0;i<split.indexWallWall[vecindex].size();i++){
			add(VECTYPE_WALLWALL,vecindex,split.indexWallWall[vecindex][i]);
		}
		for(size_t i=0;i<split.indexWallEdge[vecindex].size();i++){
			add(VECTYPE_WALLEDGE,vecindex,split.indexWallEdge[vecindex][i]);
		}
	}

	for(uchar vecindex=0;vecindex<12;vecindex++){
		for(size_t i=0;i<split.indexEdgeEdge[vecindex].size();i++){
			add(VECTYPE_EDGEEDGE,vecindex,split.indexEdgeEdge[vecindex][i]);
		}
		for(size_t i=0;i<split.indexEdgeCorner[vecindex].size();i++){
			add(VECTYPE_EDGECORN,vecindex,split.indexEdgeCorner[vecindex][i]);
		}
	}
}






BranchNeighborhood::BranchNeighborhood(){

	indexWall[0] = indexWall[1] = indexWall[2] = indexWall[3] = indexWall[4] = indexWall[5] =

	indexEdge[0] = indexEdge[1] = indexEdge[2]  = indexEdge[3]  =
	indexEdge[4] = indexEdge[5] = indexEdge[6]  = indexEdge[7]  =
	indexEdge[8] = indexEdge[9] = indexEdge[10] = indexEdge[11] =

	indexCorner[0] = indexCorner[1] = indexCorner[2] = indexCorner[3] =
	indexCorner[4] = indexCorner[5] = indexCorner[6] = indexCorner[7] =

	BRANCHINIT;
}







void addBrickInterior_indexed_incremental_optimized
(
		volumetype lastleaf, const ParentArray &leafParent,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		treeinfo info,
		const MarchingCubesIndexed &mc,
		MeshSeparate *pmesh,
		bool verbose
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
//	float minWeight = info.minWeight;
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = pmesh ? *pmesh : *info.mesh;

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

	sidetype sizeMin = leafScale[lastleaf];

	if(sizeMin>size){
		fprintf(stderr,"\nWARNING: The Leaf is too large: %i > %i",sizeMin,size);
		return;
	}

	if(size/sizeMin>bl){
		fprintf(stderr,"\nERROR: The leaf is too small! (%i %i %i | %i): %i",ox,oy,oz,size,sizeMin);
		return;
	}

	for(volumetype idx=0;idx<brickSize;idx++) w[idx] = 0;


	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			for(sidetype x=ox;x<ox+size;x+=sizeMin){
				sidetype bx = (x-ox)/sizeMin;
				volumetype idx = (bz*bl+by)*bl+bx;
				w[idx] = 0;
				for(volumetype leaf=lastleaf;leaf<BRANCHINIT
				&& w[idx]<=0.0f;
				leaf=leafParent[leaf]
//				leaf=BRANCHINIT
				                ){

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
					w[idx] = temp1[0]+temp1[1];

					wsum = _mm_add_ps(_mm_mul_ps(fSSE0,_mm_load_ps(df)),_mm_mul_ps(fSSE1,_mm_load_ps(df+4)));
					_mm_store_ps(temp1,_mm_hadd_ps(wsum,wsum));
					d[idx] = temp1[0]+temp1[1];

					if(color){
						ALIGNED float temp2[4]; ALIGNED float temp3[4];
						wsum = _mm_add_ps(_mm_mul_ps(fSSE0,_mm_load_ps(cf)),_mm_mul_ps(fSSE1,_mm_load_ps(cf+4)));
						_mm_store_ps(temp1,_mm_hadd_ps(wsum,wsum));
						wsum = _mm_add_ps(_mm_mul_ps(fSSE0,_mm_load_ps(cf+8)),_mm_mul_ps(fSSE1,_mm_load_ps(cf+12)));
						_mm_store_ps(temp2,_mm_hadd_ps(wsum,wsum));
						wsum = _mm_add_ps(_mm_mul_ps(fSSE0,_mm_load_ps(cf+16)),_mm_mul_ps(fSSE1,_mm_load_ps(cf+20)));
						_mm_store_ps(temp3,_mm_hadd_ps(wsum,wsum));
#ifndef BRICKVISUALIZATION
						c[idx] = VertexColor(temp1[0]+temp1[1],temp2[0]+temp2[1],temp3[0]+temp3[1]);
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
			}
		}
	}

	size_t runningIndex = mesh.x.size();
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			for(sidetype x=ox;x<ox+size;x+=sizeMin){
				sidetype bx = (x-ox)/sizeMin;
				volumetype idx = (bz*bl+by)*bl+bx;

				if(x<ox+size-sizeMin
						&& w[idx] && w[idx+1] && d[idx]*d[idx+1]<0.0f
				     ){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+(x      )*scale,offset.y+y*scale,offset.z+z*scale),
							Vertex3f(offset.x+(x+sizeMin)*scale,offset.y+y*scale,offset.z+z*scale),
							d[idx],d[idx+1]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
//					mesh.r.push_back(0);
//					mesh.g.push_back(255);
//					mesh.b.push_back(0);
					indices[3*idx+0] = runningIndex++;
				}
				if(y<oy+size-sizeMin
						&& w[idx] && w[idx+bl] && d[idx]*d[idx+bl]<0.0f
				     ){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+(y        )*scale,offset.z+z*scale),
							Vertex3f(offset.x+x*scale,offset.y+(y+sizeMin)*scale,offset.z+z*scale),
							d[idx],d[idx+bl]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
//					mesh.r.push_back(0);
//					mesh.g.push_back(255);
//					mesh.b.push_back(0);
					indices[3*idx+1] = runningIndex++;
				}
				if(z<oz+size-sizeMin
						&& w[idx] && w[idx+bs] && d[idx]*d[idx+bs]<0.0f
						){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+y*scale,offset.z+(z        )*scale),
							Vertex3f(offset.x+x*scale,offset.y+y*scale,offset.z+(z+sizeMin)*scale),
							d[idx],d[idx+bs]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
//					mesh.r.push_back(0);
//					mesh.g.push_back(255);
//					mesh.b.push_back(0);
					indices[3*idx+2] = runningIndex++;
				}
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
					mesh.f.push_back(indices[3*idx+table[i  ]]);
					mesh.f.push_back(indices[3*idx+table[i+1]]);
					mesh.f.push_back(indices[3*idx+table[i+2]]);
				}
			}
		}
	}


	delete [] d; delete [] w;
	delete [] c;
	delete [] indices;
}


void addBrickInterior_incremental_indexed_full8
(
		volumetype lastleaf, const ParentArray &leafParent,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		treeinfo info,
		const MarchingCubesIndexed &mc,
		bool verbose = false
)
{
//	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
//	float minWeight = info.minWeight;
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

//	sidetype bs = bl*bl;

	float *d = NULL;
//	float *w = NULL;
	weighttype *w = NULL;
	VertexColor *c = NULL;
//	float *dInc = NULL;
//	float *wInc = NULL;
//	VertexColor *cInc = NULL;
	int allocationERROR = 0;
	allocationERROR |= posix_memalign((void**)&d,16,576*sizeof(float));
	allocationERROR |= posix_memalign((void**)&w,16,576*sizeof(float));
	allocationERROR |= posix_memalign((void**)&c,16,576*sizeof(VertexColor));
//	allocationERROR |= posix_memalign((void**)&dInc,16,576*sizeof(float));
//	allocationERROR |= posix_memalign((void**)&wInc,16,576*sizeof(float));
//	allocationERROR |= posix_memalign((void**)&cInc,16,576*sizeof(VertexColor));





	unsigned short *indices = new unsigned short[brickSize*3];

	sidetype sizeMin = leafScale[lastleaf];

	memcpy(d,distance+lastleaf*brickSize,512*sizeof(float));
	memcpy(w,weights+lastleaf*brickSize,512*sizeof(weighttype));
	const colortype3 *colorstart = color+lastleaf*brickSize;
	bool zeroweights = false;
	for(volumetype idx=0;idx<512;idx++) {
		zeroweights |= !w[idx];
		c[idx] = VertexColor(colorstart[idx].x,colorstart[idx].y,colorstart[idx].z);
	}

	for(volumetype leaf=leafParent[lastleaf];zeroweights && leaf<BRANCHINIT;leaf=leafParent[leaf]){
		zeroweights = false;
		sidetype ls = leafScale[leaf];
		sidetype relativeSize = ls/sizeMin;
		sidetype relativeRange = 8/relativeSize;
		sidetype3 lo = leafPos[leaf];
		sidetype startx = (ox-lo.x)/ls;
		sidetype starty = (oy-lo.y)/ls;
		sidetype startz = (oz-lo.z)/ls;

		const float *di = distance+leaf*brickSize;
		const weighttype *wi = weights+leaf*brickSize;
		const colortype3 *ci = color+leaf*brickSize;

		volumetype idx=0;
		volumetype runningIndex = (startz*8+starty)*8+startx;
		for(sidetype z=0;z<8;z++,runningIndex += (z%relativeSize==0)*64-relativeRange*8){
			for(sidetype y=0;y<8;y++, runningIndex += (y%relativeSize==0)*8-relativeRange){
				for(sidetype x=0;x<8;x++,idx++,runningIndex += (x%relativeSize==0)){
					if(!w[idx]){
						w[idx] = wi[runningIndex];
						d[idx] = di[runningIndex];
						c[idx] = VertexColor(ci[runningIndex].x,ci[runningIndex].y,ci[runningIndex].z);
//						c[idx] = VertexColor(65280,0,0);
					}
					zeroweights |= !w[idx];
				}
			}
		}
	}

	size_t runningIndex = mesh.x.size();
	volumetype idx=0;


	float oxc = offset.x+ox*scale;
	float oyc = offset.y+oy*scale;
	float ozc = offset.z+oz*scale;
	float sc = sizeMin*scale;

//	ALIGNED uchar rx[4]; ALIGNED uchar gx[4]; ALIGNED uchar bx[4];
//	ALIGNED uchar ry[4]; ALIGNED uchar gy[4]; ALIGNED uchar by[4];
//	ALIGNED uchar rz[4]; ALIGNED uchar gz[4]; ALIGNED uchar bz[4];
//	const __m128 sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31

	for(sidetype z=0;z<8;z++){
		float cz = ozc+z*sc;
		for(sidetype y=0;y<8;y++){
			float cy = oyc+y*sc;
			for(sidetype x=0;x<8;x++,idx++){

				float cx = oxc+x*sc;

				if(x<7 && w[idx] && w[idx+1] && (d[idx]<0)!=(d[idx+1]<0)){
					float mux = fabs(d[idx]) < DISTZEROEPSILON ? 0 : (fabs(d[idx+1]) < DISTZEROEPSILON ? 1 :
					 (fabs(d[idx]-d[idx+1]) < DISTZEROEPSILON ? 0 : d[idx]/(d[idx]-d[idx+1])));
					mesh.x.push_back(cx + mux*sc);
					mesh.y.push_back(cy);
					mesh.z.push_back(cz);
					mesh.r.push_back(((float)c[idx].x + mux*((float)c[idx+1].x-(float)c[idx].x))/COLOR_MULTIPLICATOR);
					mesh.g.push_back(((float)c[idx].y + mux*((float)c[idx+1].y-(float)c[idx].y))/COLOR_MULTIPLICATOR);
					mesh.b.push_back(((float)c[idx].z + mux*((float)c[idx+1].z-(float)c[idx].z))/COLOR_MULTIPLICATOR);
					indices[3*idx+0] = runningIndex++;
				}

				if(y<7 && w[idx] && w[idx+8] && (d[idx]<0)!=(d[idx+8]<0)){
					float muy = fabs(d[idx]) < DISTZEROEPSILON ? 0 : (fabs(d[idx+8]) < DISTZEROEPSILON ? 1 :
					 (fabs(d[idx]-d[idx+8]) < DISTZEROEPSILON ? 0 : d[idx]/(d[idx]-d[idx+8])));
					mesh.x.push_back(cx);
					mesh.y.push_back(cy + muy*sc);
					mesh.z.push_back(cz);
					mesh.r.push_back(((float)c[idx].x + muy*((float)c[idx+8].x-(float)c[idx].x))/COLOR_MULTIPLICATOR);
					mesh.g.push_back(((float)c[idx].y + muy*((float)c[idx+8].y-(float)c[idx].y))/COLOR_MULTIPLICATOR);
					mesh.b.push_back(((float)c[idx].z + muy*((float)c[idx+8].z-(float)c[idx].z))/COLOR_MULTIPLICATOR);
					indices[3*idx+1] = runningIndex++;
				}

				if(z<7 && w[idx] && w[idx+64] && (d[idx]<0)!=(d[idx+64]<0)){
					float muz = fabs(d[idx]) < DISTZEROEPSILON ? 0 : (fabs(d[idx+64]) < DISTZEROEPSILON ? 1 :
					 (fabs(d[idx]-d[idx+64]) < DISTZEROEPSILON ? 0 : d[idx]/(d[idx]-d[idx+64])));
					mesh.x.push_back(cx);
					mesh.y.push_back(cy);
					mesh.z.push_back(cz + muz*sc);
					mesh.r.push_back(((float)c[idx].x + muz*((float)c[idx+64].x-(float)c[idx].x))/COLOR_MULTIPLICATOR);
					mesh.g.push_back(((float)c[idx].y + muz*((float)c[idx+64].y-(float)c[idx].y))/COLOR_MULTIPLICATOR);
					mesh.b.push_back(((float)c[idx].z + muz*((float)c[idx+64].z-(float)c[idx].z))/COLOR_MULTIPLICATOR);
					indices[3*idx+2] = runningIndex++;
				}
			}
		}
	}


	idx=0;
	for(sidetype z=0;z<7;z++){
		for(sidetype y=0;y<7;y++){
			for(sidetype x=0;x<7;x++,idx++){
				int *table = mc.offsetTable[mc.getCubeIndex(
						d[idx],d[idx+1],
						d[idx+9],d[idx+8],
						d[idx+64],d[idx+65],
						d[idx+73],d[idx+72],
						w[idx],w[idx+1],
						w[idx+9],w[idx+8],
						w[idx+64],w[idx+65],
						w[idx+73],w[idx+72])];

				for (unsigned int i=0;table[i]!=-1;i+=3) {
					mesh.f.push_back(indices[3*idx+table[i  ]]);
					mesh.f.push_back(indices[3*idx+table[i+1]]);
					mesh.f.push_back(indices[3*idx+table[i+2]]);
				}
			}
			idx++;
		}
		idx+=8;
	}





	free(d); free(w); free(c);
	delete [] indices;
}

void addBrickInterior_incremental_indexed_full8_SSE
(
		volumetype lastleaf, const ParentArray &leafParent,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		treeinfo info,
		const MarchingCubesIndexed &mc,
		bool verbose = false
)
{
//	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
//	float minWeight = info.minWeight;
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

//	sidetype bs = bl*bl;

	float *d = NULL;
//	float *w = NULL;
	weighttype *w = NULL;
	VertexColor *c = NULL;
//	float *dInc = NULL;
//	float *wInc = NULL;
//	VertexColor *cInc = NULL;
	int allocationERROR = 0;
	allocationERROR |= posix_memalign((void**)&d,16,576*sizeof(float));
	allocationERROR |= posix_memalign((void**)&w,16,576*sizeof(float));
	allocationERROR |= posix_memalign((void**)&c,16,576*sizeof(VertexColor));
//	allocationERROR |= posix_memalign((void**)&dInc,16,576*sizeof(float));
//	allocationERROR |= posix_memalign((void**)&wInc,16,576*sizeof(float));
//	allocationERROR |= posix_memalign((void**)&cInc,16,576*sizeof(VertexColor));





	unsigned short *indices = new unsigned short[brickSize*3];

	sidetype sizeMin = leafScale[lastleaf];

	memcpy(d,distance+lastleaf*brickSize,512*sizeof(float));
	memcpy(w,weights+lastleaf*brickSize,512*sizeof(weighttype));
//	const weighttype *weightstart = weights+lastleaf*brickSize;
	const colortype3 *colorstart = color+lastleaf*brickSize;
	bool zeroweights = false;
	for(volumetype idx=0;idx<512;idx++) {
		zeroweights |= !w[idx];
		c[idx] = VertexColor(colorstart[idx].x,colorstart[idx].y,colorstart[idx].z);
	}

	for(volumetype leaf=leafParent[lastleaf];zeroweights && leaf<BRANCHINIT;leaf=leafParent[leaf]){
		zeroweights = false;
		sidetype ls = leafScale[leaf];
		sidetype relativeSize = ls/sizeMin;
		sidetype relativeRange = 8/relativeSize;
		sidetype3 lo = leafPos[leaf];
		sidetype startx = (ox-lo.x)/ls;
		sidetype starty = (oy-lo.y)/ls;
		sidetype startz = (oz-lo.z)/ls;

		const float *di = distance+leaf*brickSize;
		const weighttype *wi = weights+leaf*brickSize;
		const colortype3 *ci = color+leaf*brickSize;

		volumetype idx=0;
		volumetype runningIndex = (startz*8+starty)*8+startx;
		for(sidetype z=0;z<8;z++,runningIndex += (z%relativeSize==0)*64-relativeRange*8){
			for(sidetype y=0;y<8;y++, runningIndex += (y%relativeSize==0)*8-relativeRange){
				for(sidetype x=0;x<8;x++,idx++,runningIndex += (x%relativeSize==0)){
					if(!w[idx]){
						w[idx] = wi[runningIndex];
						d[idx] = di[runningIndex];
						c[idx] = VertexColor(ci[runningIndex].x,ci[runningIndex].y,ci[runningIndex].z);
//						c[idx] = VertexColor(65280,0,0);
					}
					zeroweights |= !w[idx];
				}
			}
		}
	}

	size_t runningIndex = mesh.x.size();
	volumetype idx=0;


	float oxc = offset.x+ox*scale;
	float oyc = offset.y+oy*scale;
	float ozc = offset.z+oz*scale;
	float sc = sizeMin*scale;

	ALIGNED uchar rx[4]; ALIGNED uchar gx[4]; ALIGNED uchar bx[4];
	ALIGNED uchar ry[4]; ALIGNED uchar gy[4]; ALIGNED uchar by[4];
	ALIGNED uchar rz[4]; ALIGNED uchar gz[4]; ALIGNED uchar bz[4];


	const __m128 sign_mask = _mm_set1_ps(-0.f); // -0.f = 1 << 31

//	volumetype idx3=0;
	for(sidetype z=0;z<8;z++){
		float cz = ozc+z*sc;
		for(sidetype y=0;y<8;y++){
			float cy = oyc+y*sc;
			for(sidetype x=0;x<8;x+=4,idx+=4){

				float cx[4] = {
						oxc+x*sc,
						oxc+(x+1)*sc,
						oxc+(x+2)*sc,
						oxc+(x+3)*sc
				};


				ALIGNED float midx[4] = {-1000000.0f,-1000000.0f,-1000000.0f,-1000000.0f};
				ALIGNED float midy[4] = {-1000000.0f,-1000000.0f,-1000000.0f,-1000000.0f};
				ALIGNED float midz[4] = {-1000000.0f,-1000000.0f,-1000000.0f,-1000000.0f};

				ALIGNED float d4c[4] = {d[idx],d[idx+1],d[idx+2],d[idx+3]};
				ALIGNED float d4x[4] = {d[idx+1],d[idx+2],d[idx+3],d[idx+4]};

//				float mux[4] = {
//						fabs(d4c[0]) < DISTZEROEPSILON ? 0 : (fabs(d4x[0]) < DISTZEROEPSILON ? 1 :
//					 (fabs(d4c[0]-d4x[0]) < DISTZEROEPSILON ? 0 : d4c[0]/(d4c[0]-d4x[0]))),
//						fabs(d4c[1]) < DISTZEROEPSILON ? 0 : (fabs(d4x[1]) < DISTZEROEPSILON ? 1 :
//					 (fabs(d4c[1]-d4x[1]) < DISTZEROEPSILON ? 0 : d4c[1]/(d4c[1]-d4x[1]))),
//						fabs(d4c[2]) < DISTZEROEPSILON ? 0 : (fabs(d4x[2]) < DISTZEROEPSILON ? 1 :
//					 (fabs(d4c[2]-d4x[2]) < DISTZEROEPSILON ? 0 : d4c[2]/(d4c[2]-d4x[2]))),
//						fabs(d4c[3]) < DISTZEROEPSILON ? 0 : (fabs(d4x[3]) < DISTZEROEPSILON ? 1 :
//					 (fabs(d4c[3]-d4x[3]) < DISTZEROEPSILON ? 0 : d4c[3]/(d4c[3]-d4x[3])))
//				};

				ALIGNED float mux[4];
				{
					__m128 center = _mm_load_ps(d4c);
					__m128 neighbor = _mm_load_ps(d4x);
					__m128 diff = _mm_sub_ps(center,neighbor);
					__m128 comp1 = _mm_cmplt_ps(_mm_andnot_ps(sign_mask,center),_mm_set1_ps(DISTZEROEPSILON));
					__m128 comp2 = _mm_cmplt_ps(_mm_andnot_ps(sign_mask,neighbor),_mm_set1_ps(DISTZEROEPSILON));
					__m128 comp3 = _mm_cmplt_ps(_mm_andnot_ps(sign_mask,diff),_mm_set1_ps(DISTZEROEPSILON));
					__m128 result = _mm_andnot_ps(comp1,_mm_add_ps(_mm_and_ps(comp2,_mm_set1_ps(1.0f)),
							_mm_andnot_ps(comp2,_mm_andnot_ps(comp3,_mm_mul_ps(center,_mm_rcp_ps(diff))))));
					_mm_store_ps(mux,result);
				}



				for(sidetype i=0;i<4;i++){
					if(x+i<7 && w[idx+i] && w[idx+i+1]
//					  && d4c[i]*d4x[i]<0.0f
					  && (d4c[i]<0)!=(d4x[i]<0)
					  ){
						midx[i] = cx[i] + mux[i]*sc;
						rx[i] = ((float)c[idx+i].x + mux[i]*((float)c[idx+i+1].x-(float)c[idx+i].x))/COLOR_MULTIPLICATOR;
						gx[i] = ((float)c[idx+i].y + mux[i]*((float)c[idx+i+1].y-(float)c[idx+i].y))/COLOR_MULTIPLICATOR;
						bx[i] = ((float)c[idx+i].z + mux[i]*((float)c[idx+i+1].z-(float)c[idx+i].z))/COLOR_MULTIPLICATOR;
					}
				}

				if(y<7){
					ALIGNED float d4y[4] = {d[idx+8],d[idx+9],d[idx+10],d[idx+11]};

//					float muy[4] = {
//							fabs(d4c[0]) < DISTZEROEPSILON ? 0 : (fabs(d4y[0]) < DISTZEROEPSILON ? 1 :
//						 (fabs(d4c[0]-d4y[0]) < DISTZEROEPSILON ? 0 : d4c[0]/(d4c[0]-d4y[0]))),
//							fabs(d4c[1]) < DISTZEROEPSILON ? 0 : (fabs(d4y[1]) < DISTZEROEPSILON ? 1 :
//						 (fabs(d4c[1]-d4y[1]) < DISTZEROEPSILON ? 0 : d4c[1]/(d4c[1]-d4y[1]))),
//							fabs(d4c[2]) < DISTZEROEPSILON ? 0 : (fabs(d4y[2]) < DISTZEROEPSILON ? 1 :
//						 (fabs(d4c[2]-d4y[2]) < DISTZEROEPSILON ? 0 : d4c[2]/(d4c[2]-d4y[2]))),
//							fabs(d4c[3]) < DISTZEROEPSILON ? 0 : (fabs(d4y[3]) < DISTZEROEPSILON ? 1 :
//						 (fabs(d4c[3]-d4y[3]) < DISTZEROEPSILON ? 0 : d4c[3]/(d4c[3]-d4y[3])))
//					};

					ALIGNED float muy[4];
					{
						__m128 center = _mm_load_ps(d4c);
						__m128 neighbor = _mm_load_ps(d4y);
						__m128 diff = _mm_sub_ps(center,neighbor);
						__m128 comp1 = _mm_cmplt_ps(_mm_andnot_ps(sign_mask,center),_mm_set1_ps(DISTZEROEPSILON));
						__m128 comp2 = _mm_cmplt_ps(_mm_andnot_ps(sign_mask,neighbor),_mm_set1_ps(DISTZEROEPSILON));
						__m128 comp3 = _mm_cmplt_ps(_mm_andnot_ps(sign_mask,diff),_mm_set1_ps(DISTZEROEPSILON));
						__m128 result = _mm_andnot_ps(comp1,_mm_add_ps(_mm_and_ps(comp2,_mm_set1_ps(1.0f)),
								_mm_andnot_ps(comp2,_mm_andnot_ps(comp3,_mm_mul_ps(center,_mm_rcp_ps(diff))))));
						_mm_store_ps(muy,result);
					}

					for(sidetype i=0;i<4;i++){
						if(w[idx+i] && w[idx+i+8]
//						  && d4c[i]*d4y[i]<0.0f
						  && (d4c[i]<0)!=(d4y[i]<0)
						  ){
							midy[i] = cy + muy[i]*sc;
							ry[i] = ((float)c[idx+i].x + muy[i]*((float)c[idx+i+8].x-(float)c[idx+i].x))/COLOR_MULTIPLICATOR;
							gy[i] = ((float)c[idx+i].y + muy[i]*((float)c[idx+i+8].y-(float)c[idx+i].y))/COLOR_MULTIPLICATOR;
							by[i] = ((float)c[idx+i].z + muy[i]*((float)c[idx+i+8].z-(float)c[idx+i].z))/COLOR_MULTIPLICATOR;
						}
					}
				}

				if(z<7){
					ALIGNED float d4z[4] = {d[idx+64],d[idx+65],d[idx+66],d[idx+67]};

//					float muz[4] = {
//							fabs(d4c[0]) < DISTZEROEPSILON ? 0 : (fabs(d4z[0]) < DISTZEROEPSILON ? 1 :
//						 (fabs(d4c[0]-d4z[0]) < DISTZEROEPSILON ? 0 : d4c[0]/(d4c[0]-d4z[0]))),
//							fabs(d4c[1]) < DISTZEROEPSILON ? 0 : (fabs(d4z[1]) < DISTZEROEPSILON ? 1 :
//						 (fabs(d4c[1]-d4z[1]) < DISTZEROEPSILON ? 0 : d4c[1]/(d4c[1]-d4z[1]))),
//							fabs(d4c[2]) < DISTZEROEPSILON ? 0 : (fabs(d4z[2]) < DISTZEROEPSILON ? 1 :
//						 (fabs(d4c[2]-d4z[2]) < DISTZEROEPSILON ? 0 : d4c[2]/(d4c[2]-d4z[2]))),
//							fabs(d4c[3]) < DISTZEROEPSILON ? 0 : (fabs(d4z[3]) < DISTZEROEPSILON ? 1 :
//						 (fabs(d4c[3]-d4z[3]) < DISTZEROEPSILON ? 0 : d4c[3]/(d4c[3]-d4z[3])))
//					};

					ALIGNED float muz[4];
					{
						__m128 center = _mm_load_ps(d4c);
						__m128 neighbor = _mm_load_ps(d4z);
						__m128 diff = _mm_sub_ps(center,neighbor);
						__m128 comp1 = _mm_cmplt_ps(_mm_andnot_ps(sign_mask,center),_mm_set1_ps(DISTZEROEPSILON));
						__m128 comp2 = _mm_cmplt_ps(_mm_andnot_ps(sign_mask,neighbor),_mm_set1_ps(DISTZEROEPSILON));
						__m128 comp3 = _mm_cmplt_ps(_mm_andnot_ps(sign_mask,diff),_mm_set1_ps(DISTZEROEPSILON));
						__m128 result = _mm_andnot_ps(comp1,_mm_add_ps(_mm_and_ps(comp2,_mm_set1_ps(1.0f)),
								_mm_andnot_ps(comp2,_mm_andnot_ps(comp3,_mm_mul_ps(center,_mm_rcp_ps(diff))))));
						_mm_store_ps(muz,result);
					}

					for(sidetype i=0;i<4;i++){
						if(w[idx+i] && w[idx+i+64]
//						  && d4c[i]*d4z[i]<0.0f
              && (d4c[i]<0)!=(d4z[i]<0)
						  ){
							midz[i] = cz + muz[i]*sc;
							rz[i] = ((float)c[idx+i].x + muz[i]*((float)c[idx+i+64].x-(float)c[idx+i].x))/COLOR_MULTIPLICATOR;
							gz[i] = ((float)c[idx+i].y + muz[i]*((float)c[idx+i+64].y-(float)c[idx+i].y))/COLOR_MULTIPLICATOR;
							bz[i] = ((float)c[idx+i].z + muz[i]*((float)c[idx+i+64].z-(float)c[idx+i].z))/COLOR_MULTIPLICATOR;
						}
					}
				}
				for(sidetype i=0;i<4;i++){
					if(midx[i]> -1000000.0f){
						mesh.x.push_back(midx[i]);
						mesh.y.push_back(cy);
						mesh.z.push_back(cz);
						mesh.r.push_back(rx[i]);
						mesh.g.push_back(gx[i]);
						mesh.b.push_back(bx[i]);
						indices[3*(idx+i)+0] = runningIndex++;
					}
					if(midy[i]> -1000000.0f){
						mesh.x.push_back(cx[i]);
						mesh.y.push_back(midy[i]);
						mesh.z.push_back(cz);
						mesh.r.push_back(ry[i]);
						mesh.g.push_back(gy[i]);
						mesh.b.push_back(by[i]);
						indices[3*(idx+i)+1] = runningIndex++;
					}
					if(midz[i]> -1000000.0f){
						mesh.x.push_back(cx[i]);
						mesh.y.push_back(cy);
						mesh.z.push_back(midz[i]);
						mesh.r.push_back(rz[i]);
						mesh.g.push_back(gz[i]);
						mesh.b.push_back(bz[i]);
						indices[3*(idx+i)+2] = runningIndex++;
					}
				}
			}
		}
	}


	idx=0;
	for(sidetype z=0;z<7;z++){
		for(sidetype y=0;y<7;y++){
			for(sidetype x=0;x<7;x++,idx++){
				int *table = mc.offsetTable[mc.getCubeIndex(
						d[idx],d[idx+1],
						d[idx+9],d[idx+8],
						d[idx+64],d[idx+65],
						d[idx+73],d[idx+72],
						w[idx],w[idx+1],
						w[idx+9],w[idx+8],
						w[idx+64],w[idx+65],
						w[idx+73],w[idx+72])];

				for (unsigned int i=0;table[i]!=-1;i+=3) {
					mesh.f.push_back(indices[3*idx+table[i  ]]);
					mesh.f.push_back(indices[3*idx+table[i+1]]);
					mesh.f.push_back(indices[3*idx+table[i+2]]);
				}
			}
			idx++;
		}
		idx+=8;
	}





	free(d); free(w); free(c);
//	delete dInc; delete wInc; delete cInc;
	delete [] indices;
}


void addBrickEdgeX_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1, volumetype lastleaf2, volumetype lastleaf3,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshSeparate *pmesh
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
	MeshSeparate &mesh = pmesh ? *pmesh :*info.mesh;

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

	for(sidetype x=ox;x<ox+size;x+=sizeMin){
		sidetype bx = (x-ox)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = ((i/2)*bl+(i%2))*bl+bx;
			w[idx] = 0;
			for(volumetype leaf=lastleaves[i];leaf<BRANCHINIT
			&& w[idx]<=0.0f;
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

				w[idx] = std::max(wf[0] , wf[1]);
				d[idx] = (1.0f-rx)*df[0] + rx*df[1];
				if(color)
#ifndef BRICKVISUALIZATION
				c[idx] = VertexColor(
						(1.0f-rx)*cf[0].x + rx*cf[1].x,
						(1.0f-rx)*cf[0].y + rx*cf[1].y,
						(1.0f-rx)*cf[0].z + rx*cf[1].z);
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


	size_t runningIndex = mesh.x.size();
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
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
				mesh.f.push_back(indices[3*idx+table[i  ]]);
				mesh.f.push_back(indices[3*idx+table[i+1]]);
				mesh.f.push_back(indices[3*idx+table[i+2]]);
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
		MeshSeparate *pmesh
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
	MeshSeparate &mesh = pmesh ? *pmesh : *info.mesh;

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
			&& w[idx]<=0.0f;
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

				w[idx] = (1.0f-ry)*(float)wf[0] + ry*(float)wf[1];
				d[idx] = (1.0f-ry)*df[0] + ry*df[1];
				if(color)
#ifndef BRICKVISUALIZATION
				c[idx] = VertexColor(
						(1.0f-ry)*cf[0].x + ry*cf[1].x,
						(1.0f-ry)*cf[0].y + ry*cf[1].y,
						(1.0f-ry)*cf[0].z + ry*cf[1].z);
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


	size_t runningIndex = mesh.x.size();
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
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
				mesh.f.push_back(indices[3*idx+table[i  ]]);
				mesh.f.push_back(indices[3*idx+table[i+1]]);
				mesh.f.push_back(indices[3*idx+table[i+2]]);
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
		MeshSeparate *pmesh
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
	MeshSeparate &mesh = pmesh ? *pmesh : *info.mesh;

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
			&& w[idx]<=0.0f;
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

				w[idx] = (1.0f-rz)*(float)wf[0] + rz*(float)wf[1];
				d[idx] = (1.0f-rz)*df[0] + rz*df[1];
				if(color)
#ifndef BRICKVISUALIZATION
				c[idx] = VertexColor(
						(1.0f-rz)*cf[0].x + rz*cf[1].x,
						(1.0f-rz)*cf[0].y + rz*cf[1].y,
						(1.0f-rz)*cf[0].z + rz*cf[1].z);
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


	size_t runningIndex = mesh.x.size();
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
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
				mesh.f.push_back(indices[3*idx+table[i  ]]);
				mesh.f.push_back(indices[3*idx+table[i+1]]);
				mesh.f.push_back(indices[3*idx+table[i+2]]);
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


void addBrickWallX_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshSeparate *pmesh
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
	MeshSeparate &mesh = pmesh ? *pmesh : *info.mesh;

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
				&& w[idx]<=0.0f;
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

					w[idx] = rzInv*ryInv*(float)wf[0]+
									 rzInv*ry   *(float)wf[1]+
									 rz   *ryInv*(float)wf[2]+
									 rz   *ry   *(float)wf[3];

					d[idx] = rzInv*ryInv*df[0]+
									 rzInv*ry   *df[1]+
									 rz   *ryInv*df[2]+
									 rz   *ry   *df[3];

					if(color)
#ifndef BRICKVISUALIZATION
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

	size_t runningIndex = mesh.x.size();
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			for(sidetype bx=0;bx<2;bx++){
				volumetype idx = (bz*bl+by)*bl+bx;

				if(faceIsSet[3*idx+0]
				 	&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+(x[bx]            )*scale,offset.y+y*scale,offset.z+z*scale),
							Vertex3f(offset.x+(x[bx]+sizeStretch)*scale,offset.y+y*scale,offset.z+z*scale),
							d[idx],d[idx+1]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
					mesh.f.push_back(indices[3*idx+table[i  ]]);
					mesh.f.push_back(indices[3*idx+table[i+1]]);
					mesh.f.push_back(indices[3*idx+table[i+2]]);
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
		MeshSeparate *pmesh
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
	MeshSeparate &mesh = pmesh ? *pmesh : *info.mesh;

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
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			for(unsigned int by=0;by<2;by++){
				volumetype idx = (bz*bl+by)*bl+bx;
				w[idx] = 0;
				for(volumetype leaf=lastleaves[by];leaf<BRANCHINIT
				&& w[idx]<=0.0f;
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

					w[idx] = rzInv*rxInv*(float)wf[0]+
									 rzInv*rx   *(float)wf[1]+
									 rz   *rxInv*(float)wf[2]+
									 rz   *rx   *(float)wf[3];

					d[idx] = rzInv*rxInv*df[0]+
									 rzInv*rx   *df[1]+
									 rz   *rxInv*df[2]+
									 rz   *rx   *df[3];

					if(color)
#ifndef BRICKVISUALIZATION
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

	size_t runningIndex = mesh.x.size();
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
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
					mesh.f.push_back(indices[3*idx+table[i  ]]);
					mesh.f.push_back(indices[3*idx+table[i+1]]);
					mesh.f.push_back(indices[3*idx+table[i+2]]);
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
		MeshSeparate *pmesh
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
	MeshSeparate &mesh = pmesh ? *pmesh : *info.mesh;

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
	for(sidetype y=oy;y<oy+size;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			for(unsigned int bz=0;bz<2;bz++){
				volumetype idx = (bz*bl+by)*bl+bx;
				w[idx] = 0;
				for(volumetype leaf=lastleaves[bz];leaf<BRANCHINIT
				&& w[idx]<=0.0f;
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

					w[idx] = ryInv*rxInv*(float)wf[0]+
									 ryInv*rx   *(float)wf[1]+
									 ry   *rxInv*(float)wf[2]+
									 ry   *rx   *(float)wf[3];

					d[idx] = ryInv*rxInv*df[0]+
									 ryInv*rx   *df[1]+
									 ry   *rxInv*df[2]+
									 ry   *rx   *df[3];

					if(color)
#ifndef BRICKVISUALIZATION
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

	size_t runningIndex = mesh.x.size();
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
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
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
					mesh.f.push_back(indices[3*idx+table[i  ]]);
					mesh.f.push_back(indices[3*idx+table[i+1]]);
					mesh.f.push_back(indices[3*idx+table[i+2]]);
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
		MeshSeparate *pmesh
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
	MeshSeparate &mesh = pmesh ? *pmesh : *info.mesh;

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
		&& w[i]<=0.0f;
		leaf=leafParent[leaf]
//				leaf=BRANCHINIT
		 ){
			volumetype start = leaf*brickSize;
			sidetype3 lo = leafPos[leaf];
			sidetype ls = leafScale[leaf];

			sidetype lx = (x[i]-lo.x)/ls;
			sidetype ly = (y[i]-lo.y)/ls;
			sidetype lz = (z[i]-lo.z)/ls;

			w[i] = weights[start+(lz*bl+ly)*bl+lx];
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
				mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
				mesh.f.push_back(mesh.x.size()-1);
				mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
				mesh.f.push_back(mesh.x.size()-1);
				mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
				mesh.f.push_back(mesh.x.size()-1);

				if(color){
#ifndef BRICKVISUALIZATION
					VertexColor c0 = collist[table[i  ]];
					VertexColor c1 = collist[table[i+1]];
					VertexColor c2 = collist[table[i+2]];
#else
					VertexColor c0 = VertexColor(0,65280,0);
					VertexColor c1 = VertexColor(0,65280,0);
					VertexColor c2 = VertexColor(0,65280,0);
#endif
					mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
					mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
					mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
				}
			}
		}
	}
}




void MeshCell::updateMesh
(
		treeinfo info, const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		bool verbose
)
{
//	if(!mesh) mesh = new MeshSeparate(3);
	if(!meshinterleaved) meshinterleaved = new MeshInterleaved(3);
//	*mesh = MeshSeparate(3);
	*meshinterleaved = MeshInterleaved(3);

//	if(type!=0) return;

	switch(type){
	case 0:
//		fprintf(stderr,"0");
	if(lastLeaf[0]<BRANCHINIT){
		if(info.brickLength==8 && size==info.leafScale[lastLeaf[0]]*info.brickLength){
//		addBrickInterior_incremental_indexed_full8_SSE
//		(lastleaf0,leafParent,ox,oy,oz,size,info,mc,verbose);

//		addBrickInterior_incremental_indexed_full8
//		(lastleaf0,leafParent,ox,oy,oz,size,info,mc,verbose);

//			addBrickInterior_indexed_incremental_optimized
//				(lastLeaf[0],leafParent,ox,oy,oz,size,info,mc,mesh,verbose);


//			fprintf(stderr,"\nCalling full Cell Mesh Update [%i %i %i]%i",ox,oy,oz,size);
//			fprintf(stderr,"\nLast Leaf is %i: [%i %i %i]%i",
//					lastLeaf[0],info.leafPos[lastLeaf[0]].x,info.leafPos[lastLeaf[0]].y,
//					info.leafPos[lastLeaf[0]].z,info.leafScale[lastLeaf[0]]*info.brickLength);

			addBrickInterior_indexed_incremental_optimized
				(lastLeaf[0],leafParent,ox,oy,oz,size,info,mc,meshinterleaved);

		}
		else {
//			addBrickInterior_indexed_incremental_optimized
//			(lastleaf0,leafParent,ox,oy,oz,size,info,mc,mesh,verbose);

//			fprintf(stderr,"\nCalling partial Cell Mesh Update [%i %i %i]%i",ox,oy,oz,size);
//			fprintf(stderr,"\nLast Leaf is %i: [%i %i %i]%i",
//					lastLeaf[0],info.leafPos[lastLeaf[0]].x,info.leafPos[lastLeaf[0]].y,
//					info.leafPos[lastLeaf[0]].z,info.leafScale[lastLeaf[0]]*info.brickLength);

			addBrickInterior_indexed_incremental_optimized
				(lastLeaf[0],leafParent,ox,oy,oz,size,info,mc,meshinterleaved);
		}



//		if(oz+size>1192 && meshinterleaved->vertices.size()){
//			fprintf(stderr,"\nCell for Leaf %li is has offset %i and has %li vertices",lastLeaf[0],oz,meshinterleaved->vertices.size());
//		}
//		float maxZ = -1.0f;
//		for(size_t i=0;i<meshinterleaved->vertices.size();i++){
//			if(maxZ<meshinterleaved->vertices[i].z) maxZ = meshinterleaved->vertices[i].z;
//		}
//		if(maxZ>= 1192.0f*info.scale+info.offset.z){
//			fprintf(stderr,"\nCell for Leaf %li is has offset %i and has %li vertices and has maximum Z Vertex %f",
//					lastLeaf[0],oz,meshinterleaved->vertices.size(),maxZ);
//			for(size_t i=0;i<meshinterleaved->vertices.size();i++){
//				if(meshinterleaved->vertices[i].z > 1192.0f*info.scale+info.offset.z){
//					meshinterleaved->colors[i].r = 255;
//					meshinterleaved->colors[i].g = 0;
//					meshinterleaved->colors[i].b = 0;
//				}
//			}
//		}


	}

	break;
	case 1:
	if(lastLeaf[0]<BRANCHINIT && lastLeaf[1]<BRANCHINIT){
//		addBrickWallX_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastlLeaf[1],leafParent,mc,mesh);
		addBrickWallX_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],leafParent,mc,meshinterleaved);
	}
	break;
	case 2:
	if(lastLeaf[0]<BRANCHINIT && lastLeaf[1]<BRANCHINIT){
//		addBrickWallY_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0,lastLeaf[1,leafParent,mc,mesh);
		addBrickWallY_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],leafParent,mc,meshinterleaved);
	}
	break;
	case 3:
	if(lastLeaf[0]<BRANCHINIT && lastLeaf[1]<BRANCHINIT){
//		addBrickWallZ_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],leafParent,mc,mesh);
		addBrickWallZ_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],leafParent,mc,meshinterleaved);
	}
	break;
	case 4:
	if(lastLeaf[0]<BRANCHINIT && lastLeaf[1]<BRANCHINIT && lastLeaf[2]<BRANCHINIT && lastLeaf[3]<BRANCHINIT){
//		addBrickEdgeX_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],lastLeaf[2],lastLeaf[3],leafParent,mc,mesh);
		addBrickEdgeX_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],lastLeaf[2],lastLeaf[3],leafParent,mc,meshinterleaved);
	}
	break;
	case 5:
	if(lastLeaf[0]<BRANCHINIT && lastLeaf[1]<BRANCHINIT && lastLeaf[2]<BRANCHINIT && lastLeaf[3]<BRANCHINIT){
//		addBrickEdgeY_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],lastLeaf[2],lastLeaf[3],leafParent,mc,mesh);
		addBrickEdgeY_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],lastLeaf[2],lastLeaf[3],leafParent,mc,meshinterleaved);
	}
	break;
	case 6:
	if(lastLeaf[0]<BRANCHINIT && lastLeaf[1]<BRANCHINIT && lastLeaf[2]<BRANCHINIT && lastLeaf[3]<BRANCHINIT){
//		addBrickEdgeZ_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],lastLeaf[2],lastLeaf[3],leafParent,mc,mesh);
		addBrickEdgeZ_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],lastLeaf[2],lastLeaf[3],leafParent,mc,meshinterleaved);
	}
	break;
	case 7:
	if(lastLeaf[0]<BRANCHINIT && lastLeaf[1]<BRANCHINIT && lastLeaf[2]<BRANCHINIT && lastLeaf[3]<BRANCHINIT &&
		 lastLeaf[4]<BRANCHINIT && lastLeaf[5]<BRANCHINIT && lastLeaf[6]<BRANCHINIT && lastLeaf[7]<BRANCHINIT){
//		addBrickMiddle_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],lastLeaf[2],lastLeaf[3],lastLeaf[4],lastLeaf[5],lastLeaf[6],lastLeaf[7],leafParent,mc,mesh);
		addBrickMiddle_indexed_incremental(info,ox,oy,oz,size,lastLeaf[0],lastLeaf[1],lastLeaf[2],lastLeaf[3],lastLeaf[4],lastLeaf[5],lastLeaf[6],lastLeaf[7],leafParent,mc,meshinterleaved);
	}
	break;
	}
}

bool MeshCell::operator==(const MeshCell &cell) const{
	return type==cell.type &&
			ox==cell.ox && oy==cell.oy && oz==cell.oz &&
			lastLeaf[0]==cell.lastLeaf[0] &&
			lastLeaf[1]==cell.lastLeaf[1] &&
			lastLeaf[2]==cell.lastLeaf[2] &&
			lastLeaf[3]==cell.lastLeaf[3] &&
			lastLeaf[4]==cell.lastLeaf[4] &&
			lastLeaf[5]==cell.lastLeaf[5] &&
			lastLeaf[6]==cell.lastLeaf[6] &&
			lastLeaf[7]==cell.lastLeaf[7] &&
			lastBranch[0]==cell.lastBranch[0] &&
			lastBranch[1]==cell.lastBranch[1] &&
			lastBranch[2]==cell.lastBranch[2] &&
			lastBranch[3]==cell.lastBranch[3] &&
			lastBranch[4]==cell.lastBranch[4] &&
			lastBranch[5]==cell.lastBranch[5] &&
			lastBranch[6]==cell.lastBranch[6] &&
			lastBranch[7]==cell.lastBranch[7];
}

bool MeshCell::operator!=(const MeshCell &cell) const{
 return !operator==(cell);
}









