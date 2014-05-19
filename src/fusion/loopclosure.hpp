/*
 * loopclosure.hpp
 *
 *  Created on: Aug 25, 2013
 *      Author: steinbrf
 */

#ifndef LOOPCLOSURE_HPP_
#define LOOPCLOSURE_HPP_

#include <stdio.h>

#include "definitions.h"
#include "treeandbrick_incremental.hpp"
#include "inline_functions.hpp"



//The Weights are not weighttype, but float
typedef struct threadparameterLoop_ {
	const float*depth;
	const uchar* red;
	const uchar* green;
	const uchar* blue;
	int imageWidth; int imageHeight;
	float m11; float m12; float m13; float m14;
	float m21; float m22; float m23; float m24;
	float m31; float m32; float m33; float m34;
	float fx; float fy; float cx; float cy;
	float scale; float distanceThreshold;
	volumetype *_leafNumber; sidetype3 *_leafPos; sidetype *_leafScale;
	float *_distance; float *_weights; colortype3 *_color;
	sidetype brickLength;
	threadparameterLoop_(
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
			float *_distance, float *_weights, colortype3 *_color,
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
} threadparameterLoop;

inline void populate_branch_loop
(
		const volumetype *_tree, volumetype branch,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype half, sidetype _brickLength,
		volumetype leaf,
		const sidetype3 *_leafPos, const sidetype *_leafScale
		,std::vector<MeshCell> &_meshCells
		,std::vector<MCNSplit> &_meshCellIndicesBranch
		,std::vector<size_t> &_meshCellIndicesLeaf
		,volumetype *_leafMapLoop
		,std::vector<size_t> _meshCellsChangedLoop
)
{
	//TODO:
//	for(uchar childoffset=0;childoffset<8;childoffset++){
////		fprintf(stderr,"\nRecursing");
//		if(half>_brickLength && _tree[branch+8+childoffset]<BRANCHINIT){
//			populate_branch_loop(_tree,_tree[branch+8+childoffset],
//					ox+half*((childoffset&1)!=0),
//					oy+half*((childoffset&2)!=0),
//					oz+half*((childoffset&4)!=0),
//					half>>1,_brickLength,leaf,_leafPos,_leafScale,
//					_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf
//					,_leafMapLoop,_meshCellsChangedLoop);
//		}
//		else{
//			volumetype pointertochildindex = branch+childoffset;
//			MeshCellNeighborhood &branchCells = _meshCellIndicesBranch[pointertochildindex];
//
//			size_t &subinteriorindex = branchCells.indexInterior;
//			if(_tree[branch+childoffset]>=BRANCHINIT){
//				if(subinteriorindex >= MAXCELLINDEX){
//					fprintf(stderr,"\nERROR: This should not happen, no MeshCell for Branch!");
//				}
//				else if(_meshCells[subinteriorindex].lastLeaf[0]>=BRANCHINIT ||
//						_leafScale[_meshCells[subinteriorindex].lastLeaf[0]]>
//								_leafScale[leaf]){
//					_meshCells[subinteriorindex].lastLeaf[0] = leaf;
//					if(_meshCells[subinteriorindex].size>
//						_leafScale[_meshCells[subinteriorindex].lastLeaf[0]]*_brickLength){
//						fprintf(stderr,"\nFAULT2[%i](%i %i)",leaf,
//								_meshCells[subinteriorindex].size,
//								_leafScale[_meshCells[subinteriorindex].lastLeaf[0]]);
//					}
//				}
//			}
//			_meshCellIndicesLeaf.push_back(subinteriorindex);
//
//
//			createMeshCellStructuresForLeaf(leaf,branchCells,meshCellIndicesLeafSingle,_meshCells,_leafPos,_leafScale);
//
////#define LEAFASSOCIATION
////#include "meshcelltraversal.cpp"
////#undef LEAFASSOCIATION
//		}
//	}
}


inline void queryPointDepthSingle_func_vis_loop
(
		sidetype x, sidetype y, sidetype z, sidetype brickLengthTarget,
		const sidetype &_n, const sidetype &_brickLength, volumetype *_tree,
		volumetype &_nBranchesUsed,
		const volumetype &_nLeavesTotal, volumetype &_nLeavesUsed, volumetype &_nLeavesQueued,
		volumetype *_leafNumber, sidetype3 *_leafPos, sidetype *_leafScale,
		volumetype *_queueIndexOfLeaf
		,std::vector<MeshCell> &_meshCells
		,std::vector<MCNSplit> &_meshCellIndicesBranch
		,std::vector<std::vector<size_t> >  &_meshCellIndicesLeaf
		,std::vector<volumetype> &_leafParent
		,MCNSplit &_boundary
		,bool performIncrementalMeshing
		,volumetype *_leafMapLoop
		,std::vector<size_t> _meshCellsChangedLoop
)
{
//	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
//	sidetype half = _n>>1;
//	volumetype branch = 0;
//	volumetype pl = BRANCHINIT; //parentleaf
//
//	while((half>>1)>=brickLengthTarget){
//		sidetype childoffset = (x>=ox+half)*1 + (y>=oy+half)*2 + (z>=oz+half)*4;
//
//		volumetype pointertochildindex = branch+childoffset+8;
//		volumetype childindex = _tree[pointertochildindex];
//		volumetype leafindex = _tree[pointertochildindex-8];
//
//		sidetype quarter = half/2;
//		sidetype oxnew = ox + half * ((childoffset & 1) != 0);
//		sidetype oynew = oy + half * ((childoffset & 2) != 0);
//		sidetype oznew = oz + half * ((childoffset & 4) != 0);
//
//		if(leafindex < _nLeavesTotal){
//			pl = leafindex;
//			if(_queueIndexOfLeaf[leafindex] >= _nLeavesQueued){
//				//The Leaf Brick has not been queued yet
//				_queueIndexOfLeaf[leafindex] = _nLeavesQueued;
//				_leafNumber[_nLeavesQueued] = leafindex;
//				_nLeavesQueued++;
//			}
//		}
//
//		if(childindex == BRANCHINIT){
//			//The child is not allocated
//			_tree[pointertochildindex] = childindex = _nBranchesUsed;
//			uchar childBranchSize = 16;
////			uchar childBranchSize = 8 + ((half>>1)>=_brickLength)*8;
////			_nBranchesUsed += 16;
//			_nBranchesUsed += childBranchSize;
//
//			if(performIncrementalMeshing){
//				createMeshCellStructuresForBranch(
//						_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf,_boundary,
//						branch,childoffset,pl,childBranchSize,
//						ox,oy,oz,half,_n,quarter,oxnew,oynew,oznew);
////#define BRANCHSUBDIVISION
////#include "meshcelltraversal.cpp"
////#undef BRANCHSUBDIVISION
//			}
//		}
//
//    ox += half * ((childoffset & 1) != 0);
//    oy += half * ((childoffset & 2) != 0);
//    oz += half * ((childoffset & 4) != 0);
//    branch = childindex;
//    half >>= 1;
//	}
//	sidetype childoffset = (x>=ox+half)*1 + (y>=oy+half)*2 + (z>=oz+half)*4;
//	volumetype pointertochildindex = branch+childoffset;
//	volumetype childindex = _tree[pointertochildindex];
//	if(childindex >= BRANCHINIT){
////		fprintf(stderr,"\nAdding Bottom Leaf");
//		//The Leaf has not been allocated yet
//		if((_tree[pointertochildindex] = childindex = _nLeavesUsed) < _nLeavesTotal){
//			_leafPos[_nLeavesUsed].x = ox+half*((childoffset&1)!=0);
//			_leafPos[_nLeavesUsed].y = oy+half*((childoffset&2)!=0);
//			_leafPos[_nLeavesUsed].z = oz+half*((childoffset&4)!=0);
//			_leafScale[_nLeavesUsed] = half/_brickLength;
//
//			_leafParent.push_back(pl);
//			_meshCellIndicesLeaf.push_back(std::vector<size_t>());
//
//			if(half > _brickLength && _tree[pointertochildindex+8]<BRANCHINIT){
////				fprintf(stderr,"\nPopulating Branch");
//				size_t indexBefore = _meshCells.size();
//				populate_branch_loop(_tree,_tree[pointertochildindex+8],
//						ox+half*((childoffset&1)!=0),
//						oy+half*((childoffset&2)!=0),
//						oz+half*((childoffset&4)!=0),
//						half>>1,_brickLength,
//						_nLeavesUsed,_leafPos, _leafScale,_meshCells,
//						_meshCellIndicesBranch,_meshCellIndicesLeaf.back()
//						,_leafMapLoop,_meshCellsChangedLoop);
//			}
//			else{
//				size_t subinteriorindex = _meshCellIndicesBranch[pointertochildindex].indexInterior;
//				if(subinteriorindex >= MAXCELLINDEX){
//					fprintf(stderr,"\nERROR: This should not happen, no MeshCell for Branch!");
//				}
//
//				_meshCellIndicesLeaf.back().push_back(subinteriorindex);
//				if(_meshCells[subinteriorindex].lastLeaf[0]>=BRANCHINIT || _leafScale[_meshCells[subinteriorindex].lastLeaf[0]]>_leafScale[_nLeavesUsed]){
//					_meshCells[subinteriorindex].lastLeaf[0] = _nLeavesUsed;
//				}
//
//				for(int j=0;j<6;j++){
//					MeshCellNeighborhood::CellList &vectorWallWall = _meshCellIndicesBranch[pointertochildindex].indexWallWall[j];
//					MeshCellNeighborhood::CellList &vectorWallEdge = _meshCellIndicesBranch[pointertochildindex].indexWallEdge[j];
//					MeshCellNeighborhood::CellList &vectorWallCorner = _meshCellIndicesBranch[pointertochildindex].indexWallCorner[j];
//					for(size_t i=0;i<vectorWallWall.size();i++){
//						MeshCell &cell = _meshCells[vectorWallWall[i]];
//						volumetype &lastleaf = (j&1) ? cell.lastLeaf[0] : cell.lastLeaf[1];
//						if(lastleaf>=BRANCHINIT || _leafScale[lastleaf]>_leafScale[_nLeavesUsed]){
//							lastleaf = _nLeavesUsed;
//						}
//						_meshCellIndicesLeaf[_nLeavesUsed].push_back(vectorWallWall[i]);
//					}
//					for(size_t i=0;i<vectorWallEdge.size();i++){
//						MeshCell &cell = _meshCells[vectorWallEdge[i]];
//						volumetype *lastleaf0 = NULL; volumetype *lastleaf1 = NULL;
//						switch(j){
//						case 0 : lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3]; break;
//						case 1 : lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2]; break;
//						case 2 : if(cell.type==4){lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3];}
//							                   else{lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3];}break;
//						case 3 : if(cell.type==4){lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2];}
//							                   else{lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1];}break;
//						case 4 : lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3]; break;
//						case 5 : lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1]; break;
//						}
//						if(*lastleaf0>=BRANCHINIT || _leafScale[*lastleaf0]>_leafScale[_nLeavesUsed]){
//							*lastleaf0 = *lastleaf1 = _nLeavesUsed;
//						}
//						_meshCellIndicesLeaf[_nLeavesUsed].push_back(vectorWallEdge[i]);
//					}
//					for(size_t i=0;i<vectorWallCorner.size();i++){
//						MeshCell &cell = _meshCells[vectorWallCorner[i]];
//						volumetype *lastleaf0 = NULL; volumetype *lastleaf1 = NULL;
//						volumetype *lastleaf2 = NULL; volumetype *lastleaf3 = NULL;
//						switch(j){
//							case 0:
//							lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3];
//							lastleaf2 = &cell.lastLeaf[5]; lastleaf3 = &cell.lastLeaf[7];
//							break;
//							case 1:
//							lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2];
//							lastleaf2 = &cell.lastLeaf[4]; lastleaf3 = &cell.lastLeaf[6];
//							break;
//							case 2:
//							lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3];
//							lastleaf2 = &cell.lastLeaf[6]; lastleaf3 = &cell.lastLeaf[7];
//							break;
//							case 3:
//							lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1];
//							lastleaf2 = &cell.lastLeaf[4]; lastleaf3 = &cell.lastLeaf[5];
//							break;
//							case 4:
//							lastleaf0 = &cell.lastLeaf[4]; lastleaf1 = &cell.lastLeaf[5];
//							lastleaf2 = &cell.lastLeaf[6]; lastleaf3 = &cell.lastLeaf[7];
//							break;
//							case 5:
//							lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1];
//							lastleaf2 = &cell.lastLeaf[2]; lastleaf3 = &cell.lastLeaf[3];
//							break;
//						}
//						if(*lastleaf0>=BRANCHINIT || _leafScale[*lastleaf0]>_leafScale[_nLeavesUsed]){
//							*lastleaf0 = *lastleaf1 = *lastleaf2 = *lastleaf3 = _nLeavesUsed;
//						}
//						_meshCellIndicesLeaf[_nLeavesUsed].push_back(vectorWallCorner[i]);
//					}
//				}
//
//				for(int j=0;j<12;j++){
//					std::vector<size_t> &vectorEdgeEdge = _meshCellIndicesBranch[pointertochildindex].indexEdgeEdge[j];
//					std::vector<size_t> &vectorEdgeCorner = _meshCellIndicesBranch[pointertochildindex].indexEdgeCorner[j];
//					for(size_t i=0;i<vectorEdgeEdge.size();i++){
//						int e = j%4;
//						MeshCell &cell = _meshCells[vectorEdgeEdge[i]];
//						volumetype &lastleaf = (e&2) ?
//						((e&1) ? cell.lastLeaf[0] : cell.lastLeaf[1]) :
//						((e&1) ? cell.lastLeaf[2] : cell.lastLeaf[3]);
//						if(lastleaf>=BRANCHINIT || _leafScale[lastleaf]>_leafScale[_nLeavesUsed]){
//							lastleaf = _nLeavesUsed;
//						}
//						_meshCellIndicesLeaf[_nLeavesUsed].push_back(vectorEdgeEdge[i]);
//					}
//					for(size_t i=0;i<vectorEdgeCorner.size();i++){
//						MeshCell &cell = _meshCells[vectorEdgeCorner[i]];
//						volumetype *lastleaf0 = NULL; volumetype *lastleaf1 = NULL;
//						switch(j){
//							case 0:
//							lastleaf0 = &cell.lastLeaf[6]; lastleaf1 = &cell.lastLeaf[7]; break;
//							case 1:
//							lastleaf0 = &cell.lastLeaf[4]; lastleaf1 = &cell.lastLeaf[5]; break;
//							case 2:
//							lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3]; break;
//							case 3:
//							lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1]; break;
//							case 4:
//							lastleaf0 = &cell.lastLeaf[5]; lastleaf1 = &cell.lastLeaf[7]; break;
//							case 5:
//							lastleaf0 = &cell.lastLeaf[4]; lastleaf1 = &cell.lastLeaf[6]; break;
//							case 6:
//							lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3]; break;
//							case 7:
//							lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2]; break;
//							case 8:
//							lastleaf0 = &cell.lastLeaf[3]; lastleaf1 = &cell.lastLeaf[7]; break;
//							case 9:
//							lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[6]; break;
//							case 10:
//							lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[5]; break;
//							case 11:
//							lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[4]; break;
//						}
//						if(*lastleaf0>=BRANCHINIT || _leafScale[*lastleaf0]>_leafScale[_nLeavesUsed]){
//							*lastleaf0 = *lastleaf1 = _nLeavesUsed;
//						}
//						_meshCellIndicesLeaf[_nLeavesUsed].push_back(vectorEdgeCorner[i]);
//					}
//				}
//
////				if(_nLeavesUsed==347){
////					fprintf(stderr,"\nDimension of %i: [%i %i %i]%i",347,
////							_leafPos[347].x,_leafPos[347].y,_leafPos[347].z,_leafScale[347]);
////					fprintf(stderr,"\nCorners of %i:",347);
////				}
//				for(int j=0;j<8;j++){
////					fprintf(stderr,"\nLeaf %i, Corner %i->%li",_nLeavesUsed,j,_meshCellIndicesBranch[pointertochildindex].indexCorner[j]);
//
//					size_t &corner = _meshCellIndicesBranch[pointertochildindex].indexCorner[j];
//					if(corner<MAXCELLINDEX){
////						fprintf(stderr," Leafing Cell %li",corner);
//						if(_meshCells[corner].type!=7) fprintf(stderr,"\nERROR: Type %i instead of 7 at %i[%i]->%li",
//							_meshCells[corner].type,pointertochildindex,j,corner);
//						volumetype *lastleaf = NULL;
//						switch(j){
//						case 0: lastleaf = &_meshCells[corner].lastLeaf[7]; break;
//						case 1: lastleaf = &_meshCells[corner].lastLeaf[6]; break;
//						case 2: lastleaf = &_meshCells[corner].lastLeaf[5]; break;
//						case 3: lastleaf = &_meshCells[corner].lastLeaf[4]; break;
//						case 4: lastleaf = &_meshCells[corner].lastLeaf[3]; break;
//						case 5: lastleaf = &_meshCells[corner].lastLeaf[2]; break;
//						case 6: lastleaf = &_meshCells[corner].lastLeaf[1]; break;
//						case 7: lastleaf = &_meshCells[corner].lastLeaf[0]; break;
//						}
//						if(*lastleaf>=BRANCHINIT || _leafScale[*lastleaf]>_leafScale[_nLeavesUsed]){
//							*lastleaf = _nLeavesUsed;
//						}
//						_meshCellIndicesLeaf[_nLeavesUsed].push_back(corner);
//					}
//				}
//			}
//			if(_meshCellIndicesLeaf.back().size()==0){
//				fprintf(stderr,"\nERROR: Leaf %li has no associated Mesh Cells!",_meshCellIndicesLeaf.size()-1);
//			}
//			else{
//				bool wrongIndex = false;
//				for(size_t i=0;i<_meshCellIndicesLeaf.back().size();i++){
//					wrongIndex |= _meshCellIndicesLeaf.back()[i] >= MAXCELLINDEX;
//				}
//				if(wrongIndex){
//					fprintf(stderr,"\nERROR: Wrong Traversal Index at Leaf %li:",
//							_meshCellIndicesLeaf.size()-1);
//					for(size_t i=0;i<_meshCellIndicesLeaf.back().size();i++){
//						fprintf(stderr," %li",_meshCellIndicesLeaf.back()[i]);
//					}
//				}
//			}
//		}
//		_nLeavesUsed++;
//	}
//	//Put the Leaf onto the Data Queue, if not already there
//	if(childindex < _nLeavesTotal &&_queueIndexOfLeaf[childindex] >= _nLeavesQueued){
//    //The Leaf Brick has not been queued yet
//		_queueIndexOfLeaf[childindex] = _nLeavesQueued;
//		_leafNumber[_nLeavesQueued] = childindex;
//		_nLeavesQueued++;
//	}
}


inline void transformLoopSimPrecalculatedNeg_vis_loop
(
		const float *qxp1, const float *qxp2, const float *qxp3,
		const float *qyp1, const float *qyp2, const float *qyp3,
		const float &t1, const float &t2, const float &t3,
		const sidetype &_n, const sidetype &_bandwidth,
		const sidetype &_brickLength,
		const int &_imageWidth, const int &_imageHeight,
		int3 &_boxMin, int3 &_boxMax,
		const float *data,
		volumetype *_tree,
		volumetype &_nBranchesUsed,
		const volumetype &_nLeavesTotal, volumetype &_nLeavesUsed, volumetype &_nLeavesQueued,
		volumetype *_leafNumber, sidetype3 *_leafPos, sidetype *_leafScale,
		volumetype *_queueIndexOfLeaf, uchar *_child, volumetype *_branchNumber
		,std::vector<MeshCell> &_meshCells
		,std::vector<MCNSplit> &_meshCellIndicesBranch
		,std::vector<std::vector<size_t> > &_meshCellIndicesLeaf
		,std::vector<volumetype> &_leafParent
		,MCNSplit &_boundary
		,bool performIncrementalMeshing
		,volumetype *_leafMapLoop
		,std::vector<size_t> _meshCellsChangedLoop
)
{

	int vxt[_imageWidth];
	int vyt[_imageWidth];
	int vzt[_imageWidth];
	for(int x=0;x<_imageWidth;x++) vxt[x] = vyt[x] = vzt[x] = -1;

	for(int y=0;y<_imageHeight;y++){

		int vxl = -1; int vyl = -1; int vzl = -1;
		for(int x=0;x<_imageWidth;x+=4){

			const float *pz = data+(y*_imageWidth+x);

			int vx[4] = {
					(int)((qxp1[x  ] + qyp1[y])*pz[0] + t1),
					(int)((qxp1[x+1] + qyp1[y])*pz[1] + t1),
					(int)((qxp1[x+2] + qyp1[y])*pz[2] + t1),
					(int)((qxp1[x+3] + qyp1[y])*pz[3] + t1)
			};
			int vy[4] = {
					(int)((qxp2[x  ] + qyp2[y])*pz[0] + t2),
					(int)((qxp2[x+1] + qyp2[y])*pz[1] + t2),
					(int)((qxp2[x+2] + qyp2[y])*pz[2] + t2),
					(int)((qxp2[x+3] + qyp2[y])*pz[3] + t2)
			};
			int vz[4] = {
					(int)((qxp3[x  ] + qyp3[y])*pz[0] + t3),
					(int)((qxp3[x+1] + qyp3[y])*pz[1] + t3),
					(int)((qxp3[x+2] + qyp3[y])*pz[2] + t3),
					(int)((qxp3[x+3] + qyp3[y])*pz[3] + t3)
			};




			for(unsigned int k=0;k<4;k++){
				int xk = x+k;
				if(std::isfinite(pz[k])
					&& ( (vx[k]&(-8)) != vxt[xk] || (vy[k]&(-8)) != vyt[xk] || (vz[k]&(-8)) != vzt[xk])
					&& ( (vx[k]&(-8)) != vxl || (vy[k]&(-8)) != vyl || (vz[k]&(-8)) != vzl)
				  ){
					vxt[xk] = vxl = vx[k]&(-8); vyt[xk] = vyl = vy[k]&(-8); vzt[xk] = vzl = vz[k]&(-8);

//					sidetype3 pMin, pMax;
					int temp = vx[k]-_bandwidth;
					if(temp<0){
//						pMin.x = 0;
						if(_boxMin.x > temp) _boxMin.x = temp;
					}
//					else pMin.x = temp;
					temp = vy[k]-_bandwidth;
					if(temp<0){
//						pMin.y = 0;
						if(_boxMin.y > temp) _boxMin.y = temp;
					}
//					else pMin.y = temp;
					temp = vz[k]-_bandwidth;
					if(temp<0){
//						pMin.z = 0;
						if(_boxMin.z > temp) _boxMin.z = temp;
					}
//					else pMin.z = temp;
					temp = vx[k]+_bandwidth;
					if(temp>=_n){
//						pMax.x = _n-1;
						if(_boxMax.x < temp) _boxMax.x = temp;
					}
//					else pMax.x = temp;
					temp = vy[k]+_bandwidth;
					if(temp>=_n){
//						pMax.y = _n-1;
						if(_boxMax.y < temp) _boxMax.y = temp;
					}
//					else pMax.y = temp;
					temp = vz[k]+_bandwidth;
					if(temp>=_n){
//						pMax.z = _n-1;
						if(_boxMax.z < temp) _boxMax.z = temp;
					}
//					else pMax.z = temp;

					if(vx[k]>=0 && (sidetype)vx[k]<_n && vy[k]>=0 && (sidetype)vy[k]<_n && vz[k]>=0 && (sidetype)vz[k]<_n){
						queryPointDepthSingle_func_vis_loop(vx[k],vy[k],vz[k],leafScaleFunc(pz[k],_brickLength),
								_n,_brickLength,_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueued,
								_leafNumber,_leafPos,_leafScale,_queueIndexOfLeaf
								,_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf,_leafParent
								,_boundary
								,performIncrementalMeshing
//								,_neighbors
								,_leafMapLoop
								,_meshCellsChangedLoop
								);
					}
				}
				else{
					vxt[xk] = vyt[xk] = vzt[xk] = vxl = vyl = vzl = -1;
				}
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
		volumetype *_nLeavesUsed
);



#endif /* LOOPCLOSURE_HPP_ */
