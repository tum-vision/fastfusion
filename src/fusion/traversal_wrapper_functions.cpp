/*
 * traversal_wrapper_functions.cpp
 *
 *  Created on: May 10, 2013
 *      Author: steinbrf
 */

#include "traversal_functions.cpp"
#include <math.h>

inline void transformLoopSimPrecalculated
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
)
{
	for(int y=0;y<_imageHeight;y++){

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
				if(std::isfinite(pz[k])){

					sidetype3 pMin, pMax;
					int temp = vx[k]-_bandwidth;
					if(temp<0){
						pMin.x = 0;
						if(_boxMin.x > temp) _boxMin.x = temp;
					} else pMin.x = temp;
					temp = vy[k]-_bandwidth;
					if(temp<0){
						pMin.y = 0;
						if(_boxMin.y > temp) _boxMin.y = temp;
					} else pMin.y = temp;
					temp = vz[k]-_bandwidth;
					if(temp<0){
						pMin.z = 0;
						if(_boxMin.z > temp) _boxMin.z = temp;
					} else pMin.z = temp;
					temp = vx[k]+_bandwidth;
					if(temp>=_n){
						pMax.x = _n-1;
						if(_boxMax.x < temp) _boxMax.x = temp;
					} else pMax.x = temp;
					temp = vy[k]+_bandwidth;
					if(temp>=_n){
						pMax.y = _n-1;
						if(_boxMax.y < temp) _boxMax.y = temp;
					} else pMax.y = temp;
					temp = vz[k]+_bandwidth;
					if(temp>=_n){
						pMax.z = _n-1;
						if(_boxMax.z < temp) _boxMax.z = temp;
					} else pMax.z = temp;

					if(vx[k]>=0 && (sidetype)vx[k]<_n && vy[k]>=0 && (sidetype)vy[k]<_n && vz[k]>=0 && (sidetype)vz[k]<_n){
//						queryPointDepthSingle_func(vx[k],vy[k],vz[k],leafScaleFunc(pz[k],_brickLength),
//								_n,_brickLength,_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueued,
//								_leafNumber,_leafPos,_leafScale,_queueIndexOfLeaf);
						queryBoxDepthSingle_func(pMin,pMax,leafScaleFunc(pz[k],_brickLength),_n,_brickLength,_tree,
								_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueued,
								_leafNumber,_leafPos,_leafScale,_queueIndexOfLeaf,_child,_branchNumber);
					}
				}
			}
		}
	}
}

inline void transformLoopSimPrecalculatedNeg
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
						queryPointDepthSingle_func(vx[k],vy[k],vz[k],leafScaleFunc(pz[k],_brickLength),
								_n,_brickLength,_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueued,
								_leafNumber,_leafPos,_leafScale,_queueIndexOfLeaf);
					}
				}
				else{
					vxt[xk] = vyt[xk] = vzt[xk] = vxl = vyl = vzl = -1;
				}
			}
		}
	}
}

inline void transformLoopSimPrecalculatedNeg_vis
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
		,FusionMipMapCPU::MeshCellArray &_meshCells
		,FusionMipMapCPU::BranchIndicesArray &_meshCellIndicesBranch
		,FusionMipMapCPU::LeafIndicesArray &_meshCellIndicesLeaf
		,ParentArray &_leafParent
		,MCNSplit &_boundary
		,bool performIncrementalMeshing
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
						queryPointDepthSingle_func_vis(vx[k],vy[k],vz[k],leafScaleFunc(pz[k],_brickLength),
								_n,_brickLength,_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueued,
								_leafNumber,_leafPos,_leafScale,_queueIndexOfLeaf
								,_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf,_leafParent
								,_boundary
								,performIncrementalMeshing
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

inline void transformLoopSimPrecalculatedNeg_subtree
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
		,ParentArray &_leafParent
		,volumetype *anchorTreeBuds
		,volumetype *anchorTreeBudsParentLeaf
		,volumetype *anchorLeafBuds
		,volumetype &_numberOfQueuedTreeBuds
		,volumetype &_numberOfQueuedLeafBuds
		,volumetype _treeSizeSinceMeshing
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
						queryPointDepthSingle_func_subtree(vx[k],vy[k],vz[k],leafScaleFunc(pz[k],_brickLength),
								_n,_brickLength,_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueued,
								_leafNumber,_leafPos,_leafScale,_queueIndexOfLeaf,_leafParent,
								anchorTreeBuds,anchorTreeBudsParentLeaf,anchorLeafBuds,
								_numberOfQueuedTreeBuds,_numberOfQueuedLeafBuds,_treeSizeSinceMeshing);
					}
				}
				else{
					vxt[xk] = vyt[xk] = vzt[xk] = vxl = vyl = vzl = -1;
				}
			}
		}
	}
}



inline void transformLoopSimPrecalculatedNeg_vis
(
		const float *qxp1, const float *qxp2, const float *qxp3,
		const float *qyp1, const float *qyp2, const float *qyp3,
		const float &t1, const float &t2, const float &t3,
		const sidetype &_n, const sidetype &_bandwidth,
		const sidetype &_brickLength,
		const int &_imageWidth, const int &_imageHeight,
		int3 &_boxMin, int3 &_boxMax,
		const ushort *data, float scaling, float maxcamdistance,
		volumetype *_tree,
		volumetype &_nBranchesUsed,
		const volumetype &_nLeavesTotal, volumetype &_nLeavesUsed, volumetype &_nLeavesQueued,
		volumetype *_leafNumber, sidetype3 *_leafPos, sidetype *_leafScale,
		volumetype *_queueIndexOfLeaf, uchar *_child, volumetype *_branchNumber
		,FusionMipMapCPU::MeshCellArray &_meshCells
		,FusionMipMapCPU::BranchIndicesArray &_meshCellIndicesBranch
		,FusionMipMapCPU::LeafIndicesArray &_meshCellIndicesLeaf
		,ParentArray &_leafParent
		,MCNSplit &_boundary
		,bool performIncrementalMeshing
)
{

	int vxt[_imageWidth];
	int vyt[_imageWidth];
	int vzt[_imageWidth];
	for(int x=0;x<_imageWidth;x++) vxt[x] = vyt[x] = vzt[x] = -1;

	for(int y=0;y<_imageHeight;y++){

		int vxl = -1; int vyl = -1; int vzl = -1;
		for(int x=0;x<_imageWidth;x+=4){

			const ushort *pz = data+(y*_imageWidth+x);

			int vx[4] = {
					(int)((qxp1[x  ] + qyp1[y])*(float)(pz[0])*scaling + t1),
					(int)((qxp1[x+1] + qyp1[y])*(float)(pz[1])*scaling + t1),
					(int)((qxp1[x+2] + qyp1[y])*(float)(pz[2])*scaling + t1),
					(int)((qxp1[x+3] + qyp1[y])*(float)(pz[3])*scaling + t1)
			};
			int vy[4] = {
					(int)((qxp2[x  ] + qyp2[y])*(float)(pz[0])*scaling + t2),
					(int)((qxp2[x+1] + qyp2[y])*(float)(pz[1])*scaling + t2),
					(int)((qxp2[x+2] + qyp2[y])*(float)(pz[2])*scaling + t2),
					(int)((qxp2[x+3] + qyp2[y])*(float)(pz[3])*scaling + t2)
			};
			int vz[4] = {
					(int)((qxp3[x  ] + qyp3[y])*(float)(pz[0])*scaling + t3),
					(int)((qxp3[x+1] + qyp3[y])*(float)(pz[1])*scaling + t3),
					(int)((qxp3[x+2] + qyp3[y])*(float)(pz[2])*scaling + t3),
					(int)((qxp3[x+3] + qyp3[y])*(float)(pz[3])*scaling + t3)
			};


			for(unsigned int k=0;k<4;k++){
				int xk = x+k;
				if(((float)(pz[k])*scaling>0.0f && (float)(pz[k])*scaling<maxcamdistance)
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
						queryPointDepthSingle_func_vis(vx[k],vy[k],vz[k],leafScaleFunc((float)(pz[k])*scaling,_brickLength),
								_n,_brickLength,_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueued,
								_leafNumber,_leafPos,_leafScale,_queueIndexOfLeaf
								,_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf,_leafParent
								,_boundary
								,performIncrementalMeshing
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

inline void transformLoopSimPrecalculatedNeg_subtree
(
		const float *qxp1, const float *qxp2, const float *qxp3,
		const float *qyp1, const float *qyp2, const float *qyp3,
		const float &t1, const float &t2, const float &t3,
		const sidetype &_n, const sidetype &_bandwidth,
		const sidetype &_brickLength,
		const int &_imageWidth, const int &_imageHeight,
		int3 &_boxMin, int3 &_boxMax,
		const ushort *data, float scaling, float maxcamdistance,
		volumetype *_tree,
		volumetype &_nBranchesUsed,
		const volumetype &_nLeavesTotal, volumetype &_nLeavesUsed, volumetype &_nLeavesQueued,
		volumetype *_leafNumber, sidetype3 *_leafPos, sidetype *_leafScale,
		volumetype *_queueIndexOfLeaf, uchar *_child, volumetype *_branchNumber
		,ParentArray &_leafParent
		,volumetype *anchorTreeBuds
		,volumetype *anchorTreeBudsParentLeaf
		,volumetype *anchorLeafBuds
		,volumetype &_numberOfQueuedTreeBuds
		,volumetype &_numberOfQueuedLeafBuds
		,volumetype _treeSizeSinceMeshing
)
{

	int vxt[_imageWidth];
	int vyt[_imageWidth];
	int vzt[_imageWidth];
	for(int x=0;x<_imageWidth;x++) vxt[x] = vyt[x] = vzt[x] = -1;

	for(int y=0;y<_imageHeight;y++){

		int vxl = -1; int vyl = -1; int vzl = -1;
		for(int x=0;x<_imageWidth;x+=4){

			const ushort *pz = data+(y*_imageWidth+x);

			int vx[4] = {
					(int)((qxp1[x  ] + qyp1[y])*(float)(pz[0])*scaling + t1),
					(int)((qxp1[x+1] + qyp1[y])*(float)(pz[1])*scaling + t1),
					(int)((qxp1[x+2] + qyp1[y])*(float)(pz[2])*scaling + t1),
					(int)((qxp1[x+3] + qyp1[y])*(float)(pz[3])*scaling + t1)
			};
			int vy[4] = {
					(int)((qxp2[x  ] + qyp2[y])*(float)(pz[0])*scaling + t2),
					(int)((qxp2[x+1] + qyp2[y])*(float)(pz[1])*scaling + t2),
					(int)((qxp2[x+2] + qyp2[y])*(float)(pz[2])*scaling + t2),
					(int)((qxp2[x+3] + qyp2[y])*(float)(pz[3])*scaling + t2)
			};
			int vz[4] = {
					(int)((qxp3[x  ] + qyp3[y])*(float)(pz[0])*scaling + t3),
					(int)((qxp3[x+1] + qyp3[y])*(float)(pz[1])*scaling + t3),
					(int)((qxp3[x+2] + qyp3[y])*(float)(pz[2])*scaling + t3),
					(int)((qxp3[x+3] + qyp3[y])*(float)(pz[3])*scaling + t3)
			};


			for(unsigned int k=0;k<4;k++){
				int xk = x+k;
				if(((float)(pz[k])*scaling>0.0f && (float)(pz[k])*scaling<maxcamdistance)
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
						queryPointDepthSingle_func_subtree(vx[k],vy[k],vz[k],leafScaleFunc((float)(pz[k])*scaling,_brickLength),
								_n,_brickLength,_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueued,
								_leafNumber,_leafPos,_leafScale,_queueIndexOfLeaf,_leafParent,
								anchorTreeBuds,anchorTreeBudsParentLeaf,anchorLeafBuds,
								_numberOfQueuedTreeBuds,_numberOfQueuedLeafBuds,
								_treeSizeSinceMeshing);
					}
				}
				else{
					vxt[xk] = vyt[xk] = vzt[xk] = vxl = vyl = vzl = -1;
				}
			}
		}
	}
}
