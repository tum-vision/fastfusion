
#include "treeandbrick.hpp"
#include "treeandbrick_incremental.hpp"
#include "treeandbrick_indexed.hpp"
#include "treeandbrick_incremental_recursive.hpp"



void addMiddle_incremental
(
		sidetype size,
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		volumetype index0, volumetype index1,
		volumetype index2, volumetype index3,
		volumetype index4, volumetype index5,
		volumetype index6, volumetype index7,
		volumetype lastLeaf0, volumetype lastLeaf1,
		volumetype lastLeaf2, volumetype lastLeaf3,
		volumetype lastLeaf4, volumetype lastLeaf5,
		volumetype lastLeaf6, volumetype lastLeaf7,
		leafstack leaves0, leafstack leaves1,
		leafstack leaves2, leafstack leaves3,
		leafstack leaves4, leafstack leaves5,
		leafstack leaves6, leafstack leaves7,
		const MarchingCubesIndexed &mc,
		const ParentArray &leafParent,
		std::vector<MeshCell> *meshCells
)
{
	sidetype half = size/2;
	const volumetype *tree = info.tree;
	volumetype parentBranch[8] = {
			index0, index1, index2, index3, index4, index5, index6, index7
	};
	volumetype lastLeaf[8] = {
			lastLeaf0, lastLeaf1, lastLeaf2, lastLeaf3, lastLeaf4, lastLeaf5, lastLeaf6, lastLeaf7
	};
	leafstack parentLeaves[8] = {
			leaves0,leaves1,leaves2,leaves3,leaves4,leaves5,leaves6,leaves7
	};

	volumetype childBranch[8];
	for(volumetype i=0;i<8;i++){
		childBranch[i] = (parentBranch[i] && half>info.brickLength && tree[parentBranch[i]+7-i+8] < info.maxBranches) ?
				tree[parentBranch[i]+7-i+8] : 0;
	}
	volumetype childLeaf[8];
	leafstack childLeaves[8];
	for(volumetype i=0;i<8;i++){
		childLeaf[i] = (parentBranch[i] && tree[parentBranch[i]+7-i] < info.maxLeaves) ?
				tree[parentBranch[i]+7-i] : lastLeaf[i];
		childLeaves[i] = parentLeaves[i];
		if(childLeaf[i]<info.maxLeaves && (childLeaves[i].size()==0 ||
				childLeaf[i]!=childLeaves[i].back())){
			childLeaves[i].push_back(childLeaf[i]);
		}
	}

	bool recurse = false; bool valid = true;
	for(volumetype i=0;i<8;i++){
		recurse |= size>info.brickLength && (childBranch[i] || childLeaf[i] != lastLeaf[i]);
		valid &= lastLeaf[i]<info.maxLeaves;
	}
	if(recurse){
		addMiddle_incremental(half,info,ox+half,oy+half,oz+half,
				childBranch[0],childBranch[1],childBranch[2],childBranch[3],
				childBranch[4],childBranch[5],childBranch[6],childBranch[7],
				childLeaf[0],childLeaf[1],childLeaf[2],childLeaf[3],
				childLeaf[4],childLeaf[5],childLeaf[6],childLeaf[7],
				childLeaves[0],childLeaves[1],childLeaves[2],childLeaves[3],
				childLeaves[4],childLeaves[5],childLeaves[6],childLeaves[7],
				mc,leafParent,meshCells);
	} else if(valid){
		if(meshCells) meshCells->push_back(MeshCell(7,ox,oy,oz,size,
				childLeaves[0].back(),childLeaves[1].back(),childLeaves[2].back(),childLeaves[3].back(),
				childLeaves[4].back(),childLeaves[5].back(),childLeaves[6].back(),childLeaves[7].back()
				)); else
		addBrickMiddle_indexed_incremental(info,ox,oy,oz,size,
				childLeaves[0].back(),childLeaves[1].back(),childLeaves[2].back(),childLeaves[3].back(),
				childLeaves[4].back(),childLeaves[5].back(),childLeaves[6].back(),childLeaves[7].back(),
				leafParent,mc);
	}
}


void addEdge_incremental
(
		int dimShift,
		sidetype size,
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		volumetype index0, volumetype index1,
		volumetype index2, volumetype index3,
		volumetype lastLeaf0, volumetype lastLeaf1,
		volumetype lastLeaf2, volumetype lastLeaf3,
		leafstack leaves0, leafstack leaves1,
		leafstack leaves2, leafstack leaves3,
		const MarchingCubesIndexed &mc,
		const ParentArray &leafParent,
		std::vector<MeshCell> *meshCells
)
{

	const volumetype *tree = info.tree; sidetype brickLength = info.brickLength;
	volumetype maxBranches = info.maxBranches; volumetype maxLeaves = info.maxLeaves;
	sidetype half = size/2;

	volumetype dimL = dimShift==1 ? 2 : 1;
	volumetype dimH = dimShift==4 ? 2 : 4;
	int dimLog = dimShift/2;
	volumetype childoffset[3][8] =
	{{2+4+0,
	  2+4+1,
	  0+4+0,
	  0+4+1,
	  2+0+0,
	  2+0+1,
	  0+0+0,
	  0+0+1},

	 {1+4+0,
		0+4+0,
	  1+4+2,
	  0+4+2,
	  1+0+0,
	  0+0+0,
	  1+0+2,
	  0+0+2},

	 {1+0+2,
		0+0+2,
		1+0+0,
		0+0+0,
		1+4+2,
		0+4+2,
		1+4+0,
		0+4+0}};

	volumetype parentBranch[3][8] =
	{{index0,index0,index1,index1,index2,index2,index3,index3},
	 {index0,index1,index0,index1,index2,index3,index2,index3},
	 {index0,index1,index2,index3,index0,index1,index2,index3}};

	volumetype parentLeaf[3][8] =
	{{lastLeaf0,lastLeaf0,lastLeaf1,lastLeaf1,lastLeaf2,lastLeaf2,lastLeaf3,lastLeaf3},
	 {lastLeaf0,lastLeaf1,lastLeaf0,lastLeaf1,lastLeaf2,lastLeaf3,lastLeaf2,lastLeaf3},
	 {lastLeaf0,lastLeaf1,lastLeaf2,lastLeaf3,lastLeaf0,lastLeaf1,lastLeaf2,lastLeaf3}};

	leafstack parentLeaves[3][8] =
	{{leaves0,leaves0,leaves1,leaves1,leaves2,leaves2,leaves3,leaves3},
	 {leaves0,leaves1,leaves0,leaves1,leaves2,leaves3,leaves2,leaves3},
	 {leaves0,leaves1,leaves2,leaves3,leaves0,leaves1,leaves2,leaves3}};

	bool recurse = false;
	volumetype childBranch[8];
	volumetype childLeaf[8];
	leafstack childLeaves[8];
	for(volumetype i=0;i<8;i++){
		childBranch[i] = (parentBranch[dimLog][i] && half>brickLength &&
				tree[parentBranch[dimLog][i]+childoffset[dimLog][i]+8]<maxBranches) ?
				tree[parentBranch[dimLog][i]+childoffset[dimLog][i]+8]	: 0;
		childLeaf[i] = (parentBranch[dimLog][i] &&
				tree[parentBranch[dimLog][i]+childoffset[dimLog][i]]<maxLeaves) ?
				tree[parentBranch[dimLog][i]+childoffset[dimLog][i]]	: parentLeaf[dimLog][i];

		childLeaves[i] = parentLeaves[dimLog][i];
		if(childLeaf[i]<maxLeaves && (childLeaves[i].size()==0 ||
				childLeaf[i]!=childLeaves[i].back())){
			childLeaves[i].push_back(childLeaf[i]);
		}
		recurse |= size>brickLength && (childBranch[i] || childLeaf[i] != parentLeaf[dimLog][i]);
	}


	if(recurse){
		addEdge_incremental(dimShift,half,info,
				ox+half*(dimShift!=1),oy+half*(dimShift!=2),oz+half*(dimShift!=4),
				childBranch[0],childBranch[dimL],childBranch[dimH],childBranch[dimL+dimH],
				  childLeaf[0],  childLeaf[dimL],  childLeaf[dimH],  childLeaf[dimL+dimH],
				childLeaves[0],childLeaves[dimL],childLeaves[dimH],childLeaves[dimL+dimH],
				mc,leafParent,meshCells);
		addEdge_incremental(dimShift,half,info,
				ox+half,oy+half,oz+half,
				childBranch[dimShift],childBranch[dimL+dimShift],childBranch[dimH+dimShift],childBranch[dimL+dimH+dimShift],
				  childLeaf[dimShift],  childLeaf[dimL+dimShift],  childLeaf[dimH+dimShift],  childLeaf[dimL+dimH+dimShift],
				childLeaves[dimShift],childLeaves[dimL+dimShift],childLeaves[dimH+dimShift],childLeaves[dimL+dimH+dimShift],
				mc,leafParent,meshCells);

		addMiddle_incremental(half,info,
				ox+half*(dimShift!=1),oy+half*(dimShift!=2),oz+half*(dimShift!=4),
				childBranch[0],childBranch[1],childBranch[2],childBranch[3],
				childBranch[4],childBranch[5],childBranch[6],childBranch[7],
				childLeaf[0],childLeaf[1],childLeaf[2],childLeaf[3],
				childLeaf[4],childLeaf[5],childLeaf[6],childLeaf[7],
				childLeaves[0],childLeaves[1],childLeaves[2],childLeaves[3],
				childLeaves[4],childLeaves[5],childLeaves[6],childLeaves[7],
				mc,leafParent,meshCells);
	}
	else if(lastLeaf0<maxLeaves && lastLeaf1<maxLeaves && lastLeaf2<maxLeaves && lastLeaf3<maxLeaves){
		if(dimShift==1){
//			addBrickEdgeX_indexed(info,ox,oy,oz,size,leaves0,leaves1,leaves2,leaves3,mc);
			if(meshCells) meshCells->push_back(MeshCell(4,ox,oy,oz,size,leaves0.back(),leaves1.back(),leaves2.back(),leaves3.back())); else
			addBrickEdgeX_indexed_incremental(info,ox,oy,oz,size,leaves0.back(),leaves1.back(),leaves2.back(),leaves3.back(),leafParent,mc);
		}
		if(dimShift==2){
//			addBrickEdgeY_indexed(info,ox,oy,oz,size,leaves0,leaves1,leaves2,leaves3,mc);
			if(meshCells) meshCells->push_back(MeshCell(5,ox,oy,oz,size,leaves0.back(),leaves1.back(),leaves2.back(),leaves3.back())); else
			addBrickEdgeY_indexed_incremental(info,ox,oy,oz,size,leaves0.back(),leaves1.back(),leaves2.back(),leaves3.back(),leafParent,mc);
		}
		if(dimShift==4){
//			addBrickEdgeZ_indexed(info,ox,oy,oz,size,leaves0,leaves1,leaves2,leaves3,mc);
			if(meshCells) meshCells->push_back(MeshCell(6,ox,oy,oz,size,leaves0.back(),leaves1.back(),leaves2.back(),leaves3.back())); else
			addBrickEdgeZ_indexed_incremental(info,ox,oy,oz,size,leaves0.back(),leaves1.back(),leaves2.back(),leaves3.back(),leafParent,mc);
		}
	}

}


void addWall_incremental
(
		int dimShift,
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype indexLow,
		volumetype lastLeafLow,
		leafstack leavesLow,
		volumetype indexHigh,
		volumetype lastLeafHigh,
		leafstack leavesHigh,
		const MarchingCubesIndexed &mc,
		const ParentArray &leafParent,
		std::vector<MeshCell> *meshCells
)
{
	const volumetype *tree = info.tree; sidetype brickLength = info.brickLength;
	volumetype maxBranches = info.maxBranches; volumetype maxLeaves = info.maxLeaves;
	sidetype half = size/2;

	volumetype dimL = dimShift==1 ? 2 : 1;
	volumetype dimH = dimShift==4 ? 2 : 4;

	volumetype dimLEdgeL = dimL==1 ? 2 : 1;
	volumetype dimLEdgeH = dimL==4 ? 2 : 4;

	volumetype dimHEdgeL = dimH==1 ? 2 : 1;
	volumetype dimHEdgeH = dimH==4 ? 2 : 4;

	int dimLog = dimShift/2;
	volumetype childoffset[3][8] =
	{{1+0+0,
	  0+0+0,
	  1+2+0,
	  0+2+0,
	  1+0+4,
		0+0+4,
		1+2+4,
		0+2+4,},

	 {0+2+0,
		1+2+0,
	  0+0+0,
	  1+0+0,
	  0+2+4,
		1+2+4,
		0+0+4,
		1+0+4,},

	 {0+0+4,
		1+0+4,
		0+2+4,
		1+2+4,
		0+0+0,
		1+0+0,
		0+2+0,
		1+2+0}};

	volumetype parentBranch[3][8] =
	{{indexLow,indexHigh,indexLow,indexHigh,indexLow,indexHigh,indexLow,indexHigh},
	 {indexLow,indexLow,indexHigh,indexHigh,indexLow,indexLow,indexHigh,indexHigh},
	 {indexLow,indexLow,indexLow,indexLow,indexHigh,indexHigh,indexHigh,indexHigh}};

	volumetype parentLeaf[3][8] =
	{{lastLeafLow,lastLeafHigh,lastLeafLow,lastLeafHigh,lastLeafLow,lastLeafHigh,lastLeafLow,lastLeafHigh},
	 {lastLeafLow,lastLeafLow,lastLeafHigh,lastLeafHigh,lastLeafLow,lastLeafLow,lastLeafHigh,lastLeafHigh},
	 {lastLeafLow,lastLeafLow,lastLeafLow,lastLeafLow,lastLeafHigh,lastLeafHigh,lastLeafHigh,lastLeafHigh}};

	leafstack parentLeaves[3][8] =
	{{leavesLow,leavesHigh,leavesLow,leavesHigh,leavesLow,leavesHigh,leavesLow,leavesHigh},
	 {leavesLow,leavesLow,leavesHigh,leavesHigh,leavesLow,leavesLow,leavesHigh,leavesHigh},
	 {leavesLow,leavesLow,leavesLow,leavesLow,leavesHigh,leavesHigh,leavesHigh,leavesHigh}};

	bool recurse = false;
	volumetype childBranch[8];
	volumetype childLeaf[8];
	leafstack childLeaves[8];
	for(volumetype i=0;i<8;i++){
		childBranch[i] = (parentBranch[dimLog][i] && half>brickLength &&
				tree[parentBranch[dimLog][i]+childoffset[dimLog][i]+8]<maxBranches) ?
				tree[parentBranch[dimLog][i]+childoffset[dimLog][i]+8]	: 0;
		childLeaf[i] = (parentBranch[dimLog][i] &&
				tree[parentBranch[dimLog][i]+childoffset[dimLog][i]]<maxLeaves) ?
				tree[parentBranch[dimLog][i]+childoffset[dimLog][i]]	: parentLeaf[dimLog][i];

		childLeaves[i] = parentLeaves[dimLog][i];
		if(childLeaf[i]<maxLeaves && (childLeaves[i].size()==0 ||
				childLeaf[i]!=childLeaves[i].back())){
			childLeaves[i].push_back(childLeaf[i]);
		}
		recurse |= size>brickLength && (childBranch[i] || childLeaf[i] != parentLeaf[dimLog][i]);
	}

	if(recurse){

		addWall_incremental(dimShift,info,
				ox+half*(dimShift==1),oy+half*(dimShift==2),oz+half*(dimShift==4),half,
				childBranch[0],childLeaf[0],childLeaves[0],childBranch[dimShift],childLeaf[dimShift],childLeaves[dimShift],
				mc,leafParent,meshCells);
		addWall_incremental(dimShift,info,
				ox+half,oy+half*(dimShift==2 || dimShift==1),oz+half*(dimShift==4),half,
				childBranch[dimL],childLeaf[dimL],childLeaves[dimL],childBranch[dimL+dimShift],childLeaf[dimL+dimShift],childLeaves[dimL+dimShift],
				mc,leafParent,meshCells);
		addWall_incremental(dimShift,info,
				ox+half*(dimShift==1),oy+half*(dimShift==2 || dimShift==4),oz+half,half,
				childBranch[dimH],childLeaf[dimH],childLeaves[dimH],childBranch[dimH+dimShift],childLeaf[dimH+dimShift],childLeaves[dimH+dimShift],
				mc,leafParent,meshCells);
		addWall_incremental(dimShift,info,
				ox+half,oy+half,oz+half,half,
				childBranch[dimL+dimH],childLeaf[dimL+dimH],childLeaves[dimL+dimH],childBranch[dimL+dimH+dimShift],childLeaf[dimL+dimH+dimShift],childLeaves[dimL+dimH+dimShift],
				mc,leafParent,meshCells);

		addEdge_incremental(dimL,half,info,
				ox+half*(dimShift==1),
				oy+half*(dimShift==2),
				oz+half*(dimShift==4),
				childBranch[0],childBranch[dimLEdgeL],
				childBranch[dimLEdgeH],childBranch[dimLEdgeL+dimLEdgeH],
				childLeaf[0],childLeaf[dimLEdgeL],
				childLeaf[dimLEdgeH],childLeaf[dimLEdgeL+dimLEdgeH],
				childLeaves[0],childLeaves[dimLEdgeL],
				childLeaves[dimLEdgeH],childLeaves[dimLEdgeL+dimLEdgeH],
				mc,leafParent,meshCells);
		addEdge_incremental(dimL,half,info,
				ox+half*(dimShift==1 || dimL==1),
				oy+half*(dimShift==2 || dimL==2),
				oz+half*(dimShift==4 || dimL==4),
				childBranch[dimL],childBranch[dimL+dimLEdgeL],
				childBranch[dimL+dimLEdgeH],childBranch[dimL+dimLEdgeL+dimLEdgeH],
				childLeaf[dimL],childLeaf[dimL+dimLEdgeL],
				childLeaf[dimL+dimLEdgeH],childLeaf[dimL+dimLEdgeL+dimLEdgeH],
				childLeaves[dimL],childLeaves[dimL+dimLEdgeL],
				childLeaves[dimL+dimLEdgeH],childLeaves[dimL+dimLEdgeL+dimLEdgeH],
				mc,leafParent,meshCells);
		addEdge_incremental(dimH,half,info,
				ox+half*(dimShift==1),
				oy+half*(dimShift==2),
				oz+half*(dimShift==4),
				childBranch[0],childBranch[dimHEdgeL],
				childBranch[dimHEdgeH],childBranch[dimHEdgeL+dimHEdgeH],
				childLeaf[0],childLeaf[dimHEdgeL],
				childLeaf[dimHEdgeH],childLeaf[dimHEdgeL+dimHEdgeH],
				childLeaves[0],childLeaves[dimHEdgeL],
				childLeaves[dimHEdgeH],childLeaves[dimHEdgeL+dimHEdgeH],
				mc,leafParent,meshCells);
		addEdge_incremental(dimH,half,info,
				ox+half*(dimShift==1 || dimH==1),
				oy+half*(dimShift==2 || dimH==2),
				oz+half*(dimShift==4 || dimH==4),
				childBranch[dimH],childBranch[dimH+dimHEdgeL],
				childBranch[dimH+dimHEdgeH],childBranch[dimH+dimHEdgeL+dimHEdgeH],
				childLeaf[dimH],childLeaf[dimH+dimHEdgeL],
				childLeaf[dimH+dimHEdgeH],childLeaf[dimH+dimHEdgeL+dimHEdgeH],
				childLeaves[dimH],childLeaves[dimH+dimHEdgeL],
				childLeaves[dimH+dimHEdgeH],childLeaves[dimH+dimHEdgeL+dimHEdgeH],
				mc,leafParent,meshCells);

		addMiddle_incremental(half,info,
				ox+half*(dimShift==1),oy+half*(dimShift==2),oz+half*(dimShift==4),
				childBranch[0],childBranch[1],childBranch[2],childBranch[3],
				childBranch[4],childBranch[5],childBranch[6],childBranch[7],
				childLeaf[0],childLeaf[1],childLeaf[2],childLeaf[3],
				childLeaf[4],childLeaf[5],childLeaf[6],childLeaf[7],
				childLeaves[0],childLeaves[1],childLeaves[2],childLeaves[3],
				childLeaves[4],childLeaves[5],childLeaves[6],childLeaves[7],
				mc,leafParent,meshCells);

	}
	else if(lastLeafLow<maxLeaves && lastLeafHigh<maxLeaves){
		if(dimShift==1){
//			addBrickWallX_indexed(info,ox,oy,oz,size,leavesLow,leavesHigh,mc);
			if(meshCells) meshCells->push_back(MeshCell(1,ox,oy,oz,size,leavesLow.back(),leavesHigh.back())); else
			addBrickWallX_indexed_incremental(info,ox,oy,oz,size,leavesLow.back(),leavesHigh.back(),leafParent,mc);
		}
		if(dimShift==2){
//			addBrickWallY_indexed(info,ox,oy,oz,size,leavesLow,leavesHigh,mc);
			if(meshCells) meshCells->push_back(MeshCell(2,ox,oy,oz,size,leavesLow.back(),leavesHigh.back())); else
			addBrickWallY_indexed_incremental(info,ox,oy,oz,size,leavesLow.back(),leavesHigh.back(),leafParent,mc);
		}
		if(dimShift==4){
//			addBrickWallZ(info,ox,oy,oz,size,leavesLow,leavesHigh);
//			addBrickWallZ_indexed(info,ox,oy,oz,size,leavesLow,leavesHigh,mc);
			if(meshCells) meshCells->push_back(MeshCell(3,ox,oy,oz,size,leavesLow.back(),leavesHigh.back())); else
			addBrickWallZ_indexed_incremental(info,ox,oy,oz,size,leavesLow.back(),leavesHigh.back(),leafParent,mc);
		}
	}
}




void addInterior_incremental
(
		leafstack leaves,
		treeinfo info,
		volumetype index, sidetype size,
		sidetype ox, sidetype oy, sidetype oz,
		volumetype lastLeaf,
		const MarchingCubesIndexed &mc,
		const ParentArray &leafParent,
		std::vector<MeshCell> *meshCells
)
{

	sidetype half = size/2;

	volumetype childBranch[8];
	volumetype childLeaf[8];
	leafstack childLeaves[8];
	const volumetype *tree = info.tree; sidetype brickLength = info.brickLength;
	volumetype maxBranches = info.maxBranches; volumetype maxLeaves = info.maxLeaves;
	for(volumetype childoffset=0;childoffset<8;childoffset++){
		childBranch[childoffset] = half>brickLength && tree[index+childoffset+8]<maxBranches ? tree[index+childoffset+8] : 0;
		childLeaf[childoffset] = tree[index+childoffset]<maxLeaves ? tree[index+childoffset] : lastLeaf;
		childLeaves[childoffset] = leaves;
		if(childLeaf[childoffset]<maxLeaves && (childLeaves[childoffset].size()==0 ||
				childLeaf[childoffset]!=childLeaves[childoffset].back())){
			childLeaves[childoffset].push_back(childLeaf[childoffset]);
		}
	}


	bool recurse = false;
	for(volumetype childoffset=0;childoffset<8;childoffset++){
		recurse |= tree[index+childoffset]<maxLeaves || childBranch[childoffset];
	}

	if(recurse){

		for(int dimShift = 1; dimShift<8;dimShift*=2){
			volumetype dimL = dimShift==1 ? 2 : 1;
			volumetype dimH = dimShift==4 ? 2 : 4;

			addWall_incremental(dimShift,info,
					ox,oy,oz,half,
					childBranch[0],childLeaf[0],childLeaves[0],childBranch[dimShift],childLeaf[dimShift],childLeaves[dimShift],
					mc,leafParent,meshCells);
			addWall_incremental(dimShift,info,
					ox+half*(dimL==1),oy+half*(dimL==2),oz,half,
					childBranch[dimL],childLeaf[dimL],childLeaves[dimL],childBranch[dimL+dimShift],childLeaf[dimL+dimShift],childLeaves[dimL+dimShift],
					mc,leafParent,meshCells);
			addWall_incremental(dimShift,info,
					ox,oy+half*(dimH==2),oz+half*(dimH==4),half,
					childBranch[dimH],childLeaf[dimH],childLeaves[dimH],childBranch[dimH+dimShift],childLeaf[dimH+dimShift],childLeaves[dimH+dimShift],
					mc,leafParent,meshCells);
			addWall_incremental(dimShift,info,
					ox+half*(dimL==1),oy+half*(dimL==2 || dimH==2),oz+half*(dimH==4),half,
					childBranch[dimL+dimH],childLeaf[dimL+dimH],childLeaves[dimL+dimH],childBranch[dimL+dimH+dimShift],childLeaf[dimL+dimH+dimShift],childLeaves[dimL+dimH+dimShift],
					mc,leafParent,meshCells);

			addEdge_incremental(dimShift,half,info,
					ox,oy,oz,
					childBranch[0],childBranch[dimL],childBranch[dimH],childBranch[dimL+dimH],
					  childLeaf[0],  childLeaf[dimL],  childLeaf[dimH],  childLeaf[dimL+dimH],
					  childLeaves[0],childLeaves[dimL],childLeaves[dimH],childLeaves[dimL+dimH],
						mc,leafParent,meshCells);
			addEdge_incremental(dimShift,half,info,
					ox+half*(dimShift==1),oy+half*(dimShift==2),oz+half*(dimShift==4),
					childBranch[dimShift],childBranch[dimL+dimShift],childBranch[dimH+dimShift],childBranch[dimL+dimH+dimShift],
					  childLeaf[dimShift],  childLeaf[dimL+dimShift],  childLeaf[dimH+dimShift],  childLeaf[dimL+dimH+dimShift],
					  childLeaves[dimShift],childLeaves[dimL+dimShift],childLeaves[dimH+dimShift],childLeaves[dimL+dimH+dimShift],
						mc,leafParent,meshCells);

			addMiddle_incremental(half,info,
					ox,oy,oz,
					childBranch[0],childBranch[1],childBranch[2],childBranch[3],
					childBranch[4],childBranch[5],childBranch[6],childBranch[7],
					childLeaf[0],childLeaf[1],childLeaf[2],childLeaf[3],
					childLeaf[4],childLeaf[5],childLeaf[6],childLeaf[7],
					childLeaves[0],childLeaves[1],childLeaves[2],childLeaves[3],
					childLeaves[4],childLeaves[5],childLeaves[6],childLeaves[7],
					mc,leafParent,meshCells);
		}

		for(volumetype childoffset=0;childoffset<8;childoffset++){
			sidetype oxChild = ox+half*((childoffset&1)!=0);
			sidetype oyChild = oy+half*((childoffset&2)!=0);
			sidetype ozChild = oz+half*((childoffset&4)!=0);
			if(childBranch[childoffset]){
				addInterior_incremental(childLeaves[childoffset],info,
						tree[index+8+childoffset],half,
						oxChild,oyChild,ozChild,
						childLeaf[childoffset],mc,leafParent,meshCells);
			}
			else if(childLeaf[childoffset]<maxLeaves){
//				addBrickInterior_indexed(childLeaves[childoffset],oxChild,oyChild,ozChild,half,info,mc);
				if(meshCells) meshCells->push_back(
						MeshCell(0,oxChild,oyChild,ozChild,half,childLeaves[childoffset].back())); else
				addBrickInterior_indexed_incremental_optimized(
						childLeaves[childoffset].back(),leafParent,
						oxChild,oyChild,ozChild,half,info,mc);
			}
		}
	}
	else{
		fprintf(stderr,"\nERROR: Branch without Children!");
	}
}
