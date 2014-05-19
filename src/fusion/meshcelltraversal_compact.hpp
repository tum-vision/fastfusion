#ifndef MESHCELLTRAVERSAL_COMPACT_HPP
#define MESHCELLTRAVERSAL_COMPACT_HPP



inline void initializeMeshCellStructures_compact(
		sidetype _n,
		FusionMipMapCPU::BranchIndicesCompactArray &_meshCellIndicesBranch,
		FusionMipMapCPU::MeshCellArray &_meshCells,
		MCNCompact &_boundary
){

	sidetype half = _n/2;
  for(volumetype childoffset=0;childoffset<8;childoffset++){
  	sidetype oxnew = ((childoffset&1)!=0)*half;
  	sidetype oynew = ((childoffset&2)!=0)*half;
  	sidetype oznew = ((childoffset&4)!=0)*half;
  	_meshCellIndicesBranch[childoffset].indexInterior = _meshCells.size();
  	_meshCells.push_back(MeshCell(0,oxnew,oynew,oznew,half,BRANCHINIT,BRANCHINIT)); //0-7
  }

  //Walls
  //Inner
	_meshCells.push_back(MeshCell(1,0,0     ,0     ,half,BRANCHINIT,BRANCHINIT)); //8
	_meshCells.push_back(MeshCell(1,0,0+half,0     ,half,BRANCHINIT,BRANCHINIT)); //9
	_meshCells.push_back(MeshCell(1,0,0     ,0+half,half,BRANCHINIT,BRANCHINIT)); //10
	_meshCells.push_back(MeshCell(1,0,0+half,0+half,half,BRANCHINIT,BRANCHINIT)); //11

	_meshCells.push_back(MeshCell(2,0     ,0,0     ,half,BRANCHINIT,BRANCHINIT)); //12
	_meshCells.push_back(MeshCell(2,0+half,0,0     ,half,BRANCHINIT,BRANCHINIT)); //13
	_meshCells.push_back(MeshCell(2,0     ,0,0+half,half,BRANCHINIT,BRANCHINIT)); //14
	_meshCells.push_back(MeshCell(2,0+half,0,0+half,half,BRANCHINIT,BRANCHINIT)); //15

	_meshCells.push_back(MeshCell(3,0     ,0     ,0,half,BRANCHINIT,BRANCHINIT)); //16
	_meshCells.push_back(MeshCell(3,0+half,0     ,0,half,BRANCHINIT,BRANCHINIT)); //17
	_meshCells.push_back(MeshCell(3,0     ,0+half,0,half,BRANCHINIT,BRANCHINIT)); //18
	_meshCells.push_back(MeshCell(3,0+half,0+half,0,half,BRANCHINIT,BRANCHINIT)); //19

	_meshCellIndicesBranch[0].add(VECTYPE_WALLWALL,1,_meshCells.size()-12);
	_meshCellIndicesBranch[1].add(VECTYPE_WALLWALL,0,_meshCells.size()-12);
	_meshCellIndicesBranch[2].add(VECTYPE_WALLWALL,1,_meshCells.size()-11);
	_meshCellIndicesBranch[3].add(VECTYPE_WALLWALL,0,_meshCells.size()-11);
	_meshCellIndicesBranch[4].add(VECTYPE_WALLWALL,1,_meshCells.size()-10);
	_meshCellIndicesBranch[5].add(VECTYPE_WALLWALL,0,_meshCells.size()-10);
	_meshCellIndicesBranch[6].add(VECTYPE_WALLWALL,1,_meshCells.size()-9);
	_meshCellIndicesBranch[7].add(VECTYPE_WALLWALL,0,_meshCells.size()-9);

	_meshCellIndicesBranch[0].add(VECTYPE_WALLWALL,3,_meshCells.size()-8);
	_meshCellIndicesBranch[2].add(VECTYPE_WALLWALL,2,_meshCells.size()-8);
	_meshCellIndicesBranch[1].add(VECTYPE_WALLWALL,3,_meshCells.size()-7);
	_meshCellIndicesBranch[3].add(VECTYPE_WALLWALL,2,_meshCells.size()-7);
	_meshCellIndicesBranch[4].add(VECTYPE_WALLWALL,3,_meshCells.size()-6);
	_meshCellIndicesBranch[6].add(VECTYPE_WALLWALL,2,_meshCells.size()-6);
	_meshCellIndicesBranch[5].add(VECTYPE_WALLWALL,3,_meshCells.size()-5);
	_meshCellIndicesBranch[7].add(VECTYPE_WALLWALL,2,_meshCells.size()-5);

	_meshCellIndicesBranch[0].add(VECTYPE_WALLWALL,5,_meshCells.size()-4);
	_meshCellIndicesBranch[4].add(VECTYPE_WALLWALL,4,_meshCells.size()-4);
	_meshCellIndicesBranch[1].add(VECTYPE_WALLWALL,5,_meshCells.size()-3);
	_meshCellIndicesBranch[5].add(VECTYPE_WALLWALL,4,_meshCells.size()-3);
	_meshCellIndicesBranch[2].add(VECTYPE_WALLWALL,5,_meshCells.size()-2);
	_meshCellIndicesBranch[6].add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
	_meshCellIndicesBranch[3].add(VECTYPE_WALLWALL,5,_meshCells.size()-1);
	_meshCellIndicesBranch[7].add(VECTYPE_WALLWALL,4,_meshCells.size()-1);

	_meshCells[_meshCells.size()-12].lastBranch[0] = 0;
	_meshCells[_meshCells.size()-12].lastBranch[1] = 1;
	_meshCells[_meshCells.size()-11].lastBranch[0] = 2;
	_meshCells[_meshCells.size()-11].lastBranch[1] = 3;
	_meshCells[_meshCells.size()-10].lastBranch[0] = 4;
	_meshCells[_meshCells.size()-10].lastBranch[1] = 5;
	_meshCells[_meshCells.size()-9].lastBranch[0] = 6;
	_meshCells[_meshCells.size()-9].lastBranch[1] = 7;

	_meshCells[_meshCells.size()-8].lastBranch[0] = 0;
	_meshCells[_meshCells.size()-8].lastBranch[1] = 2;
	_meshCells[_meshCells.size()-7].lastBranch[0] = 1;
	_meshCells[_meshCells.size()-7].lastBranch[1] = 3;
	_meshCells[_meshCells.size()-6].lastBranch[0] = 4;
	_meshCells[_meshCells.size()-6].lastBranch[1] = 6;
	_meshCells[_meshCells.size()-5].lastBranch[0] = 5;
	_meshCells[_meshCells.size()-5].lastBranch[1] = 7;

	_meshCells[_meshCells.size()-4].lastBranch[0] = 0;
	_meshCells[_meshCells.size()-4].lastBranch[1] = 4;
	_meshCells[_meshCells.size()-3].lastBranch[0] = 1;
	_meshCells[_meshCells.size()-3].lastBranch[1] = 5;
	_meshCells[_meshCells.size()-2].lastBranch[0] = 2;
	_meshCells[_meshCells.size()-2].lastBranch[1] = 6;
	_meshCells[_meshCells.size()-1].lastBranch[0] = 3;
	_meshCells[_meshCells.size()-1].lastBranch[1] = 7;

	//Boundary
	_meshCells.push_back(MeshCell(1,0,0     ,0     ,half,BRANCHINIT,BRANCHINIT)); //20
	_meshCells.push_back(MeshCell(1,0,0+half,0     ,half,BRANCHINIT,BRANCHINIT)); //21
	_meshCells.push_back(MeshCell(1,0,0     ,0+half,half,BRANCHINIT,BRANCHINIT)); //22
	_meshCells.push_back(MeshCell(1,0,0+half,0+half,half,BRANCHINIT,BRANCHINIT)); //23

	_meshCells.push_back(MeshCell(2,0     ,0,0     ,half,BRANCHINIT,BRANCHINIT)); //24
	_meshCells.push_back(MeshCell(2,0+half,0,0     ,half,BRANCHINIT,BRANCHINIT)); //25
	_meshCells.push_back(MeshCell(2,0     ,0,0+half,half,BRANCHINIT,BRANCHINIT)); //26
	_meshCells.push_back(MeshCell(2,0+half,0,0+half,half,BRANCHINIT,BRANCHINIT)); //27

	_meshCells.push_back(MeshCell(3,0     ,0     ,0,half,BRANCHINIT,BRANCHINIT)); //28
	_meshCells.push_back(MeshCell(3,0+half,0     ,0,half,BRANCHINIT,BRANCHINIT)); //29
	_meshCells.push_back(MeshCell(3,0     ,0+half,0,half,BRANCHINIT,BRANCHINIT)); //30
	_meshCells.push_back(MeshCell(3,0+half,0+half,0,half,BRANCHINIT,BRANCHINIT)); //31

	_meshCells.push_back(MeshCell(1,half,0     ,0     ,half,BRANCHINIT,BRANCHINIT)); //32
	_meshCells.push_back(MeshCell(1,half,0+half,0     ,half,BRANCHINIT,BRANCHINIT)); //33
	_meshCells.push_back(MeshCell(1,half,0     ,0+half,half,BRANCHINIT,BRANCHINIT)); //34
	_meshCells.push_back(MeshCell(1,half,0+half,0+half,half,BRANCHINIT,BRANCHINIT)); //35

	_meshCells.push_back(MeshCell(2,0     ,half,0     ,half,BRANCHINIT,BRANCHINIT)); //36
	_meshCells.push_back(MeshCell(2,0+half,half,0     ,half,BRANCHINIT,BRANCHINIT)); //37
	_meshCells.push_back(MeshCell(2,0     ,half,0+half,half,BRANCHINIT,BRANCHINIT)); //38
	_meshCells.push_back(MeshCell(2,0+half,half,0+half,half,BRANCHINIT,BRANCHINIT)); //39

	_meshCells.push_back(MeshCell(3,0     ,0     ,half,half,BRANCHINIT,BRANCHINIT)); //40
	_meshCells.push_back(MeshCell(3,0+half,0     ,half,half,BRANCHINIT,BRANCHINIT)); //41
	_meshCells.push_back(MeshCell(3,0     ,0+half,half,half,BRANCHINIT,BRANCHINIT)); //42
	_meshCells.push_back(MeshCell(3,0+half,0+half,half,half,BRANCHINIT,BRANCHINIT)); //43

	_meshCellIndicesBranch[0].add(VECTYPE_WALLWALL,0,_meshCells.size()-24);
	_meshCellIndicesBranch[2].add(VECTYPE_WALLWALL,0,_meshCells.size()-23);
	_meshCellIndicesBranch[4].add(VECTYPE_WALLWALL,0,_meshCells.size()-22);
	_meshCellIndicesBranch[6].add(VECTYPE_WALLWALL,0,_meshCells.size()-21);

	_meshCellIndicesBranch[0].add(VECTYPE_WALLWALL,2,_meshCells.size()-20);
	_meshCellIndicesBranch[1].add(VECTYPE_WALLWALL,2,_meshCells.size()-19);
	_meshCellIndicesBranch[4].add(VECTYPE_WALLWALL,2,_meshCells.size()-18);
	_meshCellIndicesBranch[5].add(VECTYPE_WALLWALL,2,_meshCells.size()-17);

	_meshCellIndicesBranch[0].add(VECTYPE_WALLWALL,4,_meshCells.size()-16);
	_meshCellIndicesBranch[1].add(VECTYPE_WALLWALL,4,_meshCells.size()-15);
	_meshCellIndicesBranch[2].add(VECTYPE_WALLWALL,4,_meshCells.size()-14);
	_meshCellIndicesBranch[3].add(VECTYPE_WALLWALL,4,_meshCells.size()-13);

	_meshCellIndicesBranch[1].add(VECTYPE_WALLWALL,1,_meshCells.size()-12);
	_meshCellIndicesBranch[3].add(VECTYPE_WALLWALL,1,_meshCells.size()-11);
	_meshCellIndicesBranch[5].add(VECTYPE_WALLWALL,1,_meshCells.size()-10);
	_meshCellIndicesBranch[7].add(VECTYPE_WALLWALL,1,_meshCells.size()-9);

	_meshCellIndicesBranch[2].add(VECTYPE_WALLWALL,3,_meshCells.size()-8);
	_meshCellIndicesBranch[3].add(VECTYPE_WALLWALL,3,_meshCells.size()-7);
	_meshCellIndicesBranch[6].add(VECTYPE_WALLWALL,3,_meshCells.size()-6);
	_meshCellIndicesBranch[7].add(VECTYPE_WALLWALL,3,_meshCells.size()-5);

	_meshCellIndicesBranch[4].add(VECTYPE_WALLWALL,5,_meshCells.size()-4);
	_meshCellIndicesBranch[5].add(VECTYPE_WALLWALL,5,_meshCells.size()-3);
	_meshCellIndicesBranch[6].add(VECTYPE_WALLWALL,5,_meshCells.size()-2);
	_meshCellIndicesBranch[7].add(VECTYPE_WALLWALL,5,_meshCells.size()-1);

	_boundary.add(VECTYPE_WALLWALL,0,_meshCells.size()-24);
	_boundary.add(VECTYPE_WALLWALL,0,_meshCells.size()-23);
	_boundary.add(VECTYPE_WALLWALL,0,_meshCells.size()-22);
	_boundary.add(VECTYPE_WALLWALL,0,_meshCells.size()-21);

	_boundary.add(VECTYPE_WALLWALL,2,_meshCells.size()-20);
	_boundary.add(VECTYPE_WALLWALL,2,_meshCells.size()-19);
	_boundary.add(VECTYPE_WALLWALL,2,_meshCells.size()-18);
	_boundary.add(VECTYPE_WALLWALL,2,_meshCells.size()-17);

	_boundary.add(VECTYPE_WALLWALL,4,_meshCells.size()-16);
	_boundary.add(VECTYPE_WALLWALL,4,_meshCells.size()-15);
	_boundary.add(VECTYPE_WALLWALL,4,_meshCells.size()-14);
	_boundary.add(VECTYPE_WALLWALL,4,_meshCells.size()-13);

	_boundary.add(VECTYPE_WALLWALL,1,_meshCells.size()-12);
	_boundary.add(VECTYPE_WALLWALL,1,_meshCells.size()-11);
	_boundary.add(VECTYPE_WALLWALL,1,_meshCells.size()-10);
	_boundary.add(VECTYPE_WALLWALL,1,_meshCells.size()-9);

	_boundary.add(VECTYPE_WALLWALL,3,_meshCells.size()-8);
	_boundary.add(VECTYPE_WALLWALL,3,_meshCells.size()-7);
	_boundary.add(VECTYPE_WALLWALL,3,_meshCells.size()-6);
	_boundary.add(VECTYPE_WALLWALL,3,_meshCells.size()-5);

	_boundary.add(VECTYPE_WALLWALL,5,_meshCells.size()-4);
	_boundary.add(VECTYPE_WALLWALL,5,_meshCells.size()-3);
	_boundary.add(VECTYPE_WALLWALL,5,_meshCells.size()-2);
	_boundary.add(VECTYPE_WALLWALL,5,_meshCells.size()-1);

	_meshCells[_meshCells.size()-24].lastBranch[1] = 0;
	_meshCells[_meshCells.size()-23].lastBranch[1] = 2;
	_meshCells[_meshCells.size()-22].lastBranch[1] = 4;
	_meshCells[_meshCells.size()-21].lastBranch[1] = 6;

	_meshCells[_meshCells.size()-20].lastBranch[1] = 0;
	_meshCells[_meshCells.size()-19].lastBranch[1] = 1;
	_meshCells[_meshCells.size()-18].lastBranch[1] = 4;
	_meshCells[_meshCells.size()-17].lastBranch[1] = 5;

	_meshCells[_meshCells.size()-16].lastBranch[1] = 0;
	_meshCells[_meshCells.size()-15].lastBranch[1] = 1;
	_meshCells[_meshCells.size()-14].lastBranch[1] = 2;
	_meshCells[_meshCells.size()-13].lastBranch[1] = 3;

	_meshCells[_meshCells.size()-12].lastBranch[0] = 1;
	_meshCells[_meshCells.size()-11].lastBranch[0] = 3;
	_meshCells[_meshCells.size()-10].lastBranch[0] = 5;
	_meshCells[_meshCells.size()-9].lastBranch[0] = 7;

	_meshCells[_meshCells.size()-8].lastBranch[0] = 2;
	_meshCells[_meshCells.size()-7].lastBranch[0] = 3;
	_meshCells[_meshCells.size()-6].lastBranch[0] = 6;
	_meshCells[_meshCells.size()-5].lastBranch[0] = 7;

	_meshCells[_meshCells.size()-4].lastBranch[0] = 4;
	_meshCells[_meshCells.size()-3].lastBranch[0] = 5;
	_meshCells[_meshCells.size()-2].lastBranch[0] = 6;
	_meshCells[_meshCells.size()-1].lastBranch[0] = 7;



	//Edges
	//Inner Double Edges and Middle Corner
	_meshCells.push_back(MeshCell(4,0     ,0     ,0     ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //44
	_meshCells.push_back(MeshCell(4,0+half,0     ,0     ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //45
	_meshCells.push_back(MeshCell(5,0     ,0     ,0     ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //46
	_meshCells.push_back(MeshCell(5,0     ,0+half,0     ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //47
	_meshCells.push_back(MeshCell(6,0     ,0     ,0     ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //48
	_meshCells.push_back(MeshCell(6,0     ,0     ,0+half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //49

	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-6);
	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-6);
	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-6);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-6);

	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-5);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-5);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-5);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-5);

	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-4);
	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-4);
	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-4);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-4);

	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-3);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-3);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-3);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-3);

	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-2);
	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-2);
	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-2);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-2);

	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-1);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-1);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-1);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-1);

	_meshCells[_meshCells.size()-6].lastBranch[0] = 0;
	_meshCells[_meshCells.size()-6].lastBranch[1] = 2;
	_meshCells[_meshCells.size()-6].lastBranch[2] = 4;
	_meshCells[_meshCells.size()-6].lastBranch[3] = 6;

	_meshCells[_meshCells.size()-5].lastBranch[0] = 1;
	_meshCells[_meshCells.size()-5].lastBranch[1] = 3;
	_meshCells[_meshCells.size()-5].lastBranch[2] = 5;
	_meshCells[_meshCells.size()-5].lastBranch[3] = 7;

	_meshCells[_meshCells.size()-4].lastBranch[0] = 0;
	_meshCells[_meshCells.size()-4].lastBranch[1] = 1;
	_meshCells[_meshCells.size()-4].lastBranch[2] = 4;
	_meshCells[_meshCells.size()-4].lastBranch[3] = 5;

	_meshCells[_meshCells.size()-3].lastBranch[0] = 2;
	_meshCells[_meshCells.size()-3].lastBranch[1] = 3;
	_meshCells[_meshCells.size()-3].lastBranch[2] = 6;
	_meshCells[_meshCells.size()-3].lastBranch[3] = 7;

	_meshCells[_meshCells.size()-2].lastBranch[0] = 0;
	_meshCells[_meshCells.size()-2].lastBranch[1] = 1;
	_meshCells[_meshCells.size()-2].lastBranch[2] = 2;
	_meshCells[_meshCells.size()-2].lastBranch[3] = 3;

	_meshCells[_meshCells.size()-1].lastBranch[0] = 4;
	_meshCells[_meshCells.size()-1].lastBranch[1] = 5;
	_meshCells[_meshCells.size()-1].lastBranch[2] = 6;
	_meshCells[_meshCells.size()-1].lastBranch[3] = 7;

	//Middle / Corner
	_meshCells.push_back(MeshCell(7,0,0,0,half,BRANCHINIT)); //50
	_meshCells.back().lastBranch[0] = 0;
	_meshCells.back().lastBranch[1] = 1;
	_meshCells.back().lastBranch[2] = 2;
	_meshCells.back().lastBranch[3] = 3;
	_meshCells.back().lastBranch[4] = 4;
	_meshCells.back().lastBranch[5] = 5;
	_meshCells.back().lastBranch[6] = 6;
	_meshCells.back().lastBranch[7] = 7;
	_meshCellIndicesBranch[0].indexCorner[7] =
	_meshCellIndicesBranch[1].indexCorner[6] =
	_meshCellIndicesBranch[2].indexCorner[5] =
	_meshCellIndicesBranch[3].indexCorner[4] =
	_meshCellIndicesBranch[4].indexCorner[3] =
	_meshCellIndicesBranch[5].indexCorner[2] =
	_meshCellIndicesBranch[6].indexCorner[1] =
	_meshCellIndicesBranch[7].indexCorner[0] = _meshCells.size()-1;

	//Boundary Double Edges and EdgeCorners
	_meshCells.push_back(MeshCell(4,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //51
	_meshCells.push_back(MeshCell(4,half,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //52
	_meshCells.push_back(MeshCell(4,0   ,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //53
	_meshCells.push_back(MeshCell(4,half,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //54
	_meshCells.push_back(MeshCell(4,0   ,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //55
	_meshCells.push_back(MeshCell(4,half,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //56
	_meshCells.push_back(MeshCell(4,0   ,half,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //57
	_meshCells.push_back(MeshCell(4,half,half,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //58

	_meshCells.push_back(MeshCell(5,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //59
	_meshCells.push_back(MeshCell(5,0   ,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //60
	_meshCells.push_back(MeshCell(5,half,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //61
	_meshCells.push_back(MeshCell(5,half,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //62
	_meshCells.push_back(MeshCell(5,0   ,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //63
	_meshCells.push_back(MeshCell(5,0   ,half,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //64
	_meshCells.push_back(MeshCell(5,half,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //65
	_meshCells.push_back(MeshCell(5,half,half,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //66

	_meshCells.push_back(MeshCell(6,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //67
	_meshCells.push_back(MeshCell(6,0   ,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //68
	_meshCells.push_back(MeshCell(6,half,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //69
	_meshCells.push_back(MeshCell(6,half,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //70
	_meshCells.push_back(MeshCell(6,0   ,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //71
	_meshCells.push_back(MeshCell(6,0   ,half,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //72
	_meshCells.push_back(MeshCell(6,half,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //73
	_meshCells.push_back(MeshCell(6,half,half,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //74

	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-24);
	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-23);
	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-22);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-21);
	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-20);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-19);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-18);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-17);

	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-16);
	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-15);
	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-14);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-13);
	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-12);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-11);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-10);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-9);

	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-8);
	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-7);
	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-6);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-5);
	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-4);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-3);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-2);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-1);

	_boundary.add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-24);
	_boundary.add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-23);
	_boundary.add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-22);
	_boundary.add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-21);
	_boundary.add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-20);
	_boundary.add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-19);
	_boundary.add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-18);
	_boundary.add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-17);

	_boundary.add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-16);
	_boundary.add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-15);
	_boundary.add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-14);
	_boundary.add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-13);
	_boundary.add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-12);
	_boundary.add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-11);
	_boundary.add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-10);
	_boundary.add(VECTYPE_EDGEEDGE, 7,_meshCells.size()- 9);

	_boundary.add(VECTYPE_EDGEEDGE, 8,_meshCells.size()- 8);
	_boundary.add(VECTYPE_EDGEEDGE, 8,_meshCells.size()- 7);
	_boundary.add(VECTYPE_EDGEEDGE, 9,_meshCells.size()- 6);
	_boundary.add(VECTYPE_EDGEEDGE, 9,_meshCells.size()- 5);
	_boundary.add(VECTYPE_EDGEEDGE,10,_meshCells.size()- 4);
	_boundary.add(VECTYPE_EDGEEDGE,10,_meshCells.size()- 3);
	_boundary.add(VECTYPE_EDGEEDGE,11,_meshCells.size()- 2);
	_boundary.add(VECTYPE_EDGEEDGE,11,_meshCells.size()- 1);

	_meshCells[_meshCells.size()-24].lastBranch[3] = 0;
	_meshCells[_meshCells.size()-23].lastBranch[3] = 1;
	_meshCells[_meshCells.size()-22].lastBranch[2] = 2;
	_meshCells[_meshCells.size()-21].lastBranch[2] = 3;
	_meshCells[_meshCells.size()-20].lastBranch[1] = 4;
	_meshCells[_meshCells.size()-19].lastBranch[1] = 5;
	_meshCells[_meshCells.size()-18].lastBranch[0] = 6;
	_meshCells[_meshCells.size()-17].lastBranch[0] = 7;

	_meshCells[_meshCells.size()-16].lastBranch[3] = 0;
	_meshCells[_meshCells.size()-15].lastBranch[3] = 2;
	_meshCells[_meshCells.size()-14].lastBranch[2] = 1;
	_meshCells[_meshCells.size()-13].lastBranch[2] = 3;
	_meshCells[_meshCells.size()-12].lastBranch[1] = 4;
	_meshCells[_meshCells.size()-11].lastBranch[1] = 6;
	_meshCells[_meshCells.size()-10].lastBranch[0] = 5;
	_meshCells[_meshCells.size()- 9].lastBranch[0] = 7;

	_meshCells[_meshCells.size()- 8].lastBranch[3] = 0;
	_meshCells[_meshCells.size()- 7].lastBranch[3] = 4;
	_meshCells[_meshCells.size()- 6].lastBranch[2] = 1;
	_meshCells[_meshCells.size()- 5].lastBranch[2] = 5;
	_meshCells[_meshCells.size()- 4].lastBranch[1] = 2;
	_meshCells[_meshCells.size()- 3].lastBranch[1] = 6;
	_meshCells[_meshCells.size()- 2].lastBranch[0] = 3;
	_meshCells[_meshCells.size()- 1].lastBranch[0] = 7;

	_meshCells.push_back(MeshCell(7,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //75
	_meshCells.push_back(MeshCell(7,0   ,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //76
	_meshCells.push_back(MeshCell(7,0   ,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //77
	_meshCells.push_back(MeshCell(7,0   ,half,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //78

	_meshCells.push_back(MeshCell(7,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //79
	_meshCells.push_back(MeshCell(7,half,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //80
	_meshCells.push_back(MeshCell(7,0   ,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //81
	_meshCells.push_back(MeshCell(7,half,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //82

	_meshCells.push_back(MeshCell(7,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //83
	_meshCells.push_back(MeshCell(7,half,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //84
	_meshCells.push_back(MeshCell(7,0   ,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //85
	_meshCells.push_back(MeshCell(7,half,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //86

	_meshCellIndicesBranch[0].indexCorner[1] = _meshCells.size()-12;
	_meshCellIndicesBranch[1].indexCorner[0] = _meshCells.size()-12;
	_meshCellIndicesBranch[2].indexCorner[3] = _meshCells.size()-11;
	_meshCellIndicesBranch[3].indexCorner[2] = _meshCells.size()-11;
	_meshCellIndicesBranch[4].indexCorner[5] = _meshCells.size()-10;
	_meshCellIndicesBranch[5].indexCorner[4] = _meshCells.size()-10;
	_meshCellIndicesBranch[6].indexCorner[7] = _meshCells.size()- 9;
	_meshCellIndicesBranch[7].indexCorner[6] = _meshCells.size()- 9;

	_meshCellIndicesBranch[0].indexCorner[2] = _meshCells.size()- 8;
	_meshCellIndicesBranch[2].indexCorner[0] = _meshCells.size()- 8;
	_meshCellIndicesBranch[1].indexCorner[3] = _meshCells.size()- 7;
	_meshCellIndicesBranch[3].indexCorner[1] = _meshCells.size()- 7;
	_meshCellIndicesBranch[4].indexCorner[6] = _meshCells.size()- 6;
	_meshCellIndicesBranch[6].indexCorner[4] = _meshCells.size()- 6;
	_meshCellIndicesBranch[5].indexCorner[7] = _meshCells.size()- 5;
	_meshCellIndicesBranch[7].indexCorner[5] = _meshCells.size()- 5;

	_meshCellIndicesBranch[0].indexCorner[4] = _meshCells.size()- 4;
	_meshCellIndicesBranch[4].indexCorner[0] = _meshCells.size()- 4;
	_meshCellIndicesBranch[1].indexCorner[5] = _meshCells.size()- 3;
	_meshCellIndicesBranch[5].indexCorner[1] = _meshCells.size()- 3;
	_meshCellIndicesBranch[2].indexCorner[6] = _meshCells.size()- 2;
	_meshCellIndicesBranch[6].indexCorner[2] = _meshCells.size()- 2;
	_meshCellIndicesBranch[3].indexCorner[7] = _meshCells.size()- 1;
	_meshCellIndicesBranch[7].indexCorner[3] = _meshCells.size()- 1;

	_boundary.add(VECTYPE_EDGECORN, 0,_meshCells.size()-12);
	_boundary.add(VECTYPE_EDGECORN, 1,_meshCells.size()-11);
	_boundary.add(VECTYPE_EDGECORN, 2,_meshCells.size()-10);
	_boundary.add(VECTYPE_EDGECORN, 3,_meshCells.size()- 9);

	_boundary.add(VECTYPE_EDGECORN, 4,_meshCells.size()- 8);
	_boundary.add(VECTYPE_EDGECORN, 5,_meshCells.size()- 7);
	_boundary.add(VECTYPE_EDGECORN, 6,_meshCells.size()- 6);
	_boundary.add(VECTYPE_EDGECORN, 7,_meshCells.size()- 5);

	_boundary.add(VECTYPE_EDGECORN, 8,_meshCells.size()- 4);
	_boundary.add(VECTYPE_EDGECORN, 9,_meshCells.size()- 3);
	_boundary.add(VECTYPE_EDGECORN,10,_meshCells.size()- 2);
	_boundary.add(VECTYPE_EDGECORN,11,_meshCells.size()- 1);

	_meshCells[_meshCells.size()-12].lastBranch[6] = 0;
	_meshCells[_meshCells.size()-12].lastBranch[7] = 1;
	_meshCells[_meshCells.size()-11].lastBranch[4] = 2;
	_meshCells[_meshCells.size()-11].lastBranch[5] = 3;
	_meshCells[_meshCells.size()-10].lastBranch[2] = 4;
	_meshCells[_meshCells.size()-10].lastBranch[3] = 5;
	_meshCells[_meshCells.size()- 9].lastBranch[0] = 6;
	_meshCells[_meshCells.size()- 9].lastBranch[1] = 7;

	_meshCells[_meshCells.size()- 8].lastBranch[5] = 0;
	_meshCells[_meshCells.size()- 8].lastBranch[7] = 2;
	_meshCells[_meshCells.size()- 7].lastBranch[4] = 1;
	_meshCells[_meshCells.size()- 7].lastBranch[6] = 3;
	_meshCells[_meshCells.size()- 6].lastBranch[1] = 4;
	_meshCells[_meshCells.size()- 6].lastBranch[3] = 6;
	_meshCells[_meshCells.size()- 5].lastBranch[0] = 5;
	_meshCells[_meshCells.size()- 5].lastBranch[2] = 7;

	_meshCells[_meshCells.size()- 4].lastBranch[3] = 0;
	_meshCells[_meshCells.size()- 4].lastBranch[7] = 4;
	_meshCells[_meshCells.size()- 3].lastBranch[2] = 1;
	_meshCells[_meshCells.size()- 3].lastBranch[6] = 5;
	_meshCells[_meshCells.size()- 2].lastBranch[1] = 2;
	_meshCells[_meshCells.size()- 2].lastBranch[5] = 6;
	_meshCells[_meshCells.size()- 1].lastBranch[0] = 3;
	_meshCells[_meshCells.size()- 1].lastBranch[4] = 7;


	//Boundary Wall Edges and Corners
	_meshCells.push_back(MeshCell(4,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //87
	_meshCells.push_back(MeshCell(4,half,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //88
	_meshCells.push_back(MeshCell(4,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //89
	_meshCells.push_back(MeshCell(4,half,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //90
	_meshCells.push_back(MeshCell(4,0   ,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //91
	_meshCells.push_back(MeshCell(4,half,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //92
	_meshCells.push_back(MeshCell(4,0   ,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //93
	_meshCells.push_back(MeshCell(4,half,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //94

	_meshCells.push_back(MeshCell(5,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //95
	_meshCells.push_back(MeshCell(5,0   ,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //96
	_meshCells.push_back(MeshCell(5,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //97
	_meshCells.push_back(MeshCell(5,0   ,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //98
	_meshCells.push_back(MeshCell(5,half,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //99
	_meshCells.push_back(MeshCell(5,half,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //100
	_meshCells.push_back(MeshCell(5,0   ,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //101
	_meshCells.push_back(MeshCell(5,0   ,half,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //102

	_meshCells.push_back(MeshCell(6,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //103
	_meshCells.push_back(MeshCell(6,0   ,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //104
	_meshCells.push_back(MeshCell(6,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //105
	_meshCells.push_back(MeshCell(6,0   ,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //106
	_meshCells.push_back(MeshCell(6,half,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //107
	_meshCells.push_back(MeshCell(6,half,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //108
	_meshCells.push_back(MeshCell(6,0   ,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //109
	_meshCells.push_back(MeshCell(6,0   ,half,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //110


	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-24);
	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-24);
	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-23);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-23);

	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-22);
	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-22);
	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-21);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-21);

	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-20);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-20);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-19);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-19);

	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-18);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-18);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-17);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-17);

	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-16);
	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-16);
	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-15);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-15);

	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-14);
	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-14);
	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-13);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-13);

	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-12);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-12);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-11);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-11);

	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-10);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-10);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()- 9);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()- 9);

	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()- 8);
	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()- 8);
	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()- 7);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()- 7);

	_meshCellIndicesBranch[0].add(VECTYPE_EDGEEDGE,10,_meshCells.size()- 6);
	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()- 6);
	_meshCellIndicesBranch[4].add(VECTYPE_EDGEEDGE,10,_meshCells.size()- 5);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()- 5);

	_meshCellIndicesBranch[1].add(VECTYPE_EDGEEDGE,11,_meshCells.size()- 4);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()- 4);
	_meshCellIndicesBranch[5].add(VECTYPE_EDGEEDGE,11,_meshCells.size()- 3);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()- 3);

	_meshCellIndicesBranch[2].add(VECTYPE_EDGEEDGE,11,_meshCells.size()- 2);
	_meshCellIndicesBranch[3].add(VECTYPE_EDGEEDGE,10,_meshCells.size()- 2);
	_meshCellIndicesBranch[6].add(VECTYPE_EDGEEDGE,11,_meshCells.size()- 1);
	_meshCellIndicesBranch[7].add(VECTYPE_EDGEEDGE,10,_meshCells.size()- 1);


	_boundary.add(VECTYPE_WALLEDGE, 4,_meshCells.size()-24);
	_boundary.add(VECTYPE_WALLEDGE, 4,_meshCells.size()-23);
	_boundary.add(VECTYPE_WALLEDGE, 2,_meshCells.size()-22);
	_boundary.add(VECTYPE_WALLEDGE, 2,_meshCells.size()-21);
	_boundary.add(VECTYPE_WALLEDGE, 3,_meshCells.size()-20);
	_boundary.add(VECTYPE_WALLEDGE, 3,_meshCells.size()-19);
	_boundary.add(VECTYPE_WALLEDGE, 5,_meshCells.size()-18);
	_boundary.add(VECTYPE_WALLEDGE, 5,_meshCells.size()-17);

	_boundary.add(VECTYPE_WALLEDGE, 4,_meshCells.size()-16);
	_boundary.add(VECTYPE_WALLEDGE, 4,_meshCells.size()-15);
	_boundary.add(VECTYPE_WALLEDGE, 0,_meshCells.size()-14);
	_boundary.add(VECTYPE_WALLEDGE, 0,_meshCells.size()-13);
	_boundary.add(VECTYPE_WALLEDGE, 1,_meshCells.size()-12);
	_boundary.add(VECTYPE_WALLEDGE, 1,_meshCells.size()-11);
	_boundary.add(VECTYPE_WALLEDGE, 5,_meshCells.size()-10);
	_boundary.add(VECTYPE_WALLEDGE, 5,_meshCells.size()- 9);

	_boundary.add(VECTYPE_WALLEDGE, 2,_meshCells.size()- 8);
	_boundary.add(VECTYPE_WALLEDGE, 2,_meshCells.size()- 7);
	_boundary.add(VECTYPE_WALLEDGE, 0,_meshCells.size()- 6);
	_boundary.add(VECTYPE_WALLEDGE, 0,_meshCells.size()- 5);
	_boundary.add(VECTYPE_WALLEDGE, 1,_meshCells.size()- 4);
	_boundary.add(VECTYPE_WALLEDGE, 1,_meshCells.size()- 3);
	_boundary.add(VECTYPE_WALLEDGE, 3,_meshCells.size()- 2);
	_boundary.add(VECTYPE_WALLEDGE, 3,_meshCells.size()- 1);


	_meshCells[_meshCells.size()-24].lastBranch[2] = 0;
	_meshCells[_meshCells.size()-24].lastBranch[3] = 2;
	_meshCells[_meshCells.size()-23].lastBranch[2] = 1;
	_meshCells[_meshCells.size()-23].lastBranch[3] = 3;

	_meshCells[_meshCells.size()-22].lastBranch[1] = 0;
	_meshCells[_meshCells.size()-22].lastBranch[3] = 4;
	_meshCells[_meshCells.size()-21].lastBranch[1] = 1;
	_meshCells[_meshCells.size()-21].lastBranch[3] = 5;

	_meshCells[_meshCells.size()-20].lastBranch[0] = 2;
	_meshCells[_meshCells.size()-20].lastBranch[2] = 6;
	_meshCells[_meshCells.size()-19].lastBranch[0] = 3;
	_meshCells[_meshCells.size()-19].lastBranch[2] = 7;

	_meshCells[_meshCells.size()-18].lastBranch[0] = 4;
	_meshCells[_meshCells.size()-18].lastBranch[1] = 6;
	_meshCells[_meshCells.size()-17].lastBranch[0] = 5;
	_meshCells[_meshCells.size()-17].lastBranch[1] = 7;

	_meshCells[_meshCells.size()-16].lastBranch[2] = 0;
	_meshCells[_meshCells.size()-16].lastBranch[3] = 1;
	_meshCells[_meshCells.size()-15].lastBranch[2] = 2;
	_meshCells[_meshCells.size()-15].lastBranch[3] = 3;

	_meshCells[_meshCells.size()-14].lastBranch[1] = 0;
	_meshCells[_meshCells.size()-14].lastBranch[3] = 4;
	_meshCells[_meshCells.size()-13].lastBranch[1] = 2;
	_meshCells[_meshCells.size()-13].lastBranch[3] = 6;

	_meshCells[_meshCells.size()-12].lastBranch[0] = 1;
	_meshCells[_meshCells.size()-12].lastBranch[2] = 5;
	_meshCells[_meshCells.size()-11].lastBranch[0] = 3;
	_meshCells[_meshCells.size()-11].lastBranch[2] = 7;

	_meshCells[_meshCells.size()-10].lastBranch[0] = 4;
	_meshCells[_meshCells.size()-10].lastBranch[1] = 5;
	_meshCells[_meshCells.size()- 9].lastBranch[0] = 6;
	_meshCells[_meshCells.size()- 9].lastBranch[1] = 7;

	_meshCells[_meshCells.size()- 8].lastBranch[2] = 0;
	_meshCells[_meshCells.size()- 8].lastBranch[3] = 1;
	_meshCells[_meshCells.size()- 7].lastBranch[2] = 4;
	_meshCells[_meshCells.size()- 7].lastBranch[3] = 5;

	_meshCells[_meshCells.size()- 6].lastBranch[1] = 0;
	_meshCells[_meshCells.size()- 6].lastBranch[3] = 2;
	_meshCells[_meshCells.size()- 5].lastBranch[1] = 4;
	_meshCells[_meshCells.size()- 5].lastBranch[3] = 6;

	_meshCells[_meshCells.size()- 4].lastBranch[0] = 1;
	_meshCells[_meshCells.size()- 4].lastBranch[2] = 3;
	_meshCells[_meshCells.size()- 3].lastBranch[0] = 5;
	_meshCells[_meshCells.size()- 3].lastBranch[2] = 7;

	_meshCells[_meshCells.size()- 2].lastBranch[0] = 2;
	_meshCells[_meshCells.size()- 2].lastBranch[1] = 3;
	_meshCells[_meshCells.size()- 1].lastBranch[0] = 6;
	_meshCells[_meshCells.size()- 1].lastBranch[1] = 7;

	//Boundary WallCorners
	_meshCells.push_back(MeshCell(7,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //111
	_meshCells.push_back(MeshCell(7,half,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //112
	_meshCells.push_back(MeshCell(7,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //113
	_meshCells.push_back(MeshCell(7,0   ,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //114
	_meshCells.push_back(MeshCell(7,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //115
	_meshCells.push_back(MeshCell(7,0   ,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //116

	_meshCellIndicesBranch[0].indexCorner[6] =
	_meshCellIndicesBranch[2].indexCorner[4] =
	_meshCellIndicesBranch[4].indexCorner[2] =
	_meshCellIndicesBranch[6].indexCorner[0] = _meshCells.size()-6;

	_meshCellIndicesBranch[1].indexCorner[7] =
	_meshCellIndicesBranch[3].indexCorner[5] =
	_meshCellIndicesBranch[5].indexCorner[3] =
	_meshCellIndicesBranch[7].indexCorner[1] = _meshCells.size()-5;

	_meshCellIndicesBranch[0].indexCorner[5] =
	_meshCellIndicesBranch[1].indexCorner[4] =
	_meshCellIndicesBranch[4].indexCorner[1] =
	_meshCellIndicesBranch[5].indexCorner[0] = _meshCells.size()-4;

	_meshCellIndicesBranch[2].indexCorner[7] =
	_meshCellIndicesBranch[3].indexCorner[6] =
	_meshCellIndicesBranch[6].indexCorner[3] =
	_meshCellIndicesBranch[7].indexCorner[2] = _meshCells.size()-3;

	_meshCellIndicesBranch[0].indexCorner[3] =
	_meshCellIndicesBranch[1].indexCorner[2] =
	_meshCellIndicesBranch[2].indexCorner[1] =
	_meshCellIndicesBranch[3].indexCorner[0] = _meshCells.size()-2;

	_meshCellIndicesBranch[4].indexCorner[7] =
	_meshCellIndicesBranch[5].indexCorner[6] =
	_meshCellIndicesBranch[6].indexCorner[5] =
	_meshCellIndicesBranch[7].indexCorner[4] = _meshCells.size()-1;

	_boundary.add(VECTYPE_WALLCORN,0,_meshCells.size()-6);
	_boundary.add(VECTYPE_WALLCORN,1,_meshCells.size()-5);
	_boundary.add(VECTYPE_WALLCORN,2,_meshCells.size()-4);
	_boundary.add(VECTYPE_WALLCORN,3,_meshCells.size()-3);
	_boundary.add(VECTYPE_WALLCORN,4,_meshCells.size()-2);
	_boundary.add(VECTYPE_WALLCORN,5,_meshCells.size()-1);

	_meshCells[_meshCells.size()-6].lastBranch[1] = 0;
	_meshCells[_meshCells.size()-6].lastBranch[3] = 2;
	_meshCells[_meshCells.size()-6].lastBranch[5] = 4;
	_meshCells[_meshCells.size()-6].lastBranch[7] = 6;

	_meshCells[_meshCells.size()-5].lastBranch[0] = 0;
	_meshCells[_meshCells.size()-5].lastBranch[2] = 2;
	_meshCells[_meshCells.size()-5].lastBranch[4] = 4;
	_meshCells[_meshCells.size()-5].lastBranch[6] = 6;

	_meshCells[_meshCells.size()-4].lastBranch[2] = 0;
	_meshCells[_meshCells.size()-4].lastBranch[3] = 1;
	_meshCells[_meshCells.size()-4].lastBranch[6] = 4;
	_meshCells[_meshCells.size()-4].lastBranch[7] = 5;

	_meshCells[_meshCells.size()-3].lastBranch[0] = 2;
	_meshCells[_meshCells.size()-3].lastBranch[1] = 3;
	_meshCells[_meshCells.size()-3].lastBranch[4] = 6;
	_meshCells[_meshCells.size()-3].lastBranch[5] = 7;

	_meshCells[_meshCells.size()-2].lastBranch[4] = 0;
	_meshCells[_meshCells.size()-2].lastBranch[5] = 1;
	_meshCells[_meshCells.size()-2].lastBranch[6] = 2;
	_meshCells[_meshCells.size()-2].lastBranch[7] = 3;

	_meshCells[_meshCells.size()-1].lastBranch[0] = 4;
	_meshCells[_meshCells.size()-1].lastBranch[1] = 5;
	_meshCells[_meshCells.size()-1].lastBranch[2] = 6;
	_meshCells[_meshCells.size()-1].lastBranch[3] = 7;

	//Boundary Corners
	_meshCells.push_back(MeshCell(7,0   ,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //117
	_meshCells.push_back(MeshCell(7,half,0   ,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //118
	_meshCells.push_back(MeshCell(7,0   ,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //119
	_meshCells.push_back(MeshCell(7,half,half,0   ,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //120
	_meshCells.push_back(MeshCell(7,0   ,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //121
	_meshCells.push_back(MeshCell(7,half,0   ,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //122
	_meshCells.push_back(MeshCell(7,0   ,half,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //123
	_meshCells.push_back(MeshCell(7,half,half,half,half,BRANCHINIT,BRANCHINIT,BRANCHINIT,BRANCHINIT)); //124

	_meshCellIndicesBranch[0].indexCorner[0] = _meshCells.size()-8;
	_meshCellIndicesBranch[1].indexCorner[1] = _meshCells.size()-7;
	_meshCellIndicesBranch[2].indexCorner[2] = _meshCells.size()-6;
	_meshCellIndicesBranch[3].indexCorner[3] = _meshCells.size()-5;
	_meshCellIndicesBranch[4].indexCorner[4] = _meshCells.size()-4;
	_meshCellIndicesBranch[5].indexCorner[5] = _meshCells.size()-3;
	_meshCellIndicesBranch[6].indexCorner[6] = _meshCells.size()-2;
	_meshCellIndicesBranch[7].indexCorner[7] = _meshCells.size()-1;

	_boundary.indexCorner[0] = _meshCells.size()-8;
	_boundary.indexCorner[1] = _meshCells.size()-7;
	_boundary.indexCorner[2] = _meshCells.size()-6;
	_boundary.indexCorner[3] = _meshCells.size()-5;
	_boundary.indexCorner[4] = _meshCells.size()-4;
	_boundary.indexCorner[5] = _meshCells.size()-3;
	_boundary.indexCorner[6] = _meshCells.size()-2;
	_boundary.indexCorner[7] = _meshCells.size()-1;

	_meshCells[_meshCells.size()-8].lastBranch[7] = 0;
	_meshCells[_meshCells.size()-7].lastBranch[6] = 1;
	_meshCells[_meshCells.size()-6].lastBranch[5] = 2;
	_meshCells[_meshCells.size()-5].lastBranch[4] = 3;
	_meshCells[_meshCells.size()-4].lastBranch[3] = 4;
	_meshCells[_meshCells.size()-3].lastBranch[2] = 5;
	_meshCells[_meshCells.size()-2].lastBranch[1] = 6;
	_meshCells[_meshCells.size()-1].lastBranch[0] = 7;

	fprintf(stderr,"\nCreated %li Mesh Cells",_meshCells.size());
}




inline void growMeshCellStructures_compact
(
		FusionMipMapCPU::MeshCellArray &_meshCells,
		FusionMipMapCPU::BranchIndicesCompactArray &_meshCellIndicesBranch,
		MCNCompact &_boundary,
		const volumetype &index,
		const sidetype &level,
		const volumetype &co,
		const sidetype &oxchild,
		const sidetype &oychild,
		const sidetype &ozchild,
		const sidetype &_n
)
{

	fprintf(stderr,"\nCorrecting Offsets for shifted Lower Boundary...");
	for(size_t i=0;i<_meshCells.size();i++){
		_meshCells[i].ox += oxchild; _meshCells[i].oy += oychild; _meshCells[i].oz += ozchild;
	}

//	for(size_t i=0;i<_boundary.cells.size();i++){
//		MeshCell &cell = _meshCells[_boundary.cells[i].first];
//		MCNCompact::vecnum identifier = _boundary.cells[i].second;
	for(size_t i=0;i<_boundary.border_size();i++){
		MCNCompact::CellReferenceNum ref = _boundary.get(i);
		MeshCell &cell = _meshCells[ref.second];
		MCNCompact::vecnum identifier = ref.first;

		if(oxchild){
			if (identifier==VECTYPE_WALLWALL_0 ||
					identifier==VECTYPE_WALLEDGE_0 ||
					identifier==VECTYPE_WALLCORN_0 ||
					identifier==VECTYPE_EDGEEDGE_04 || identifier==VECTYPE_EDGECORN_04 ||
					identifier==VECTYPE_EDGEEDGE_06 || identifier==VECTYPE_EDGECORN_06 ||
					identifier==VECTYPE_EDGEEDGE_08 || identifier==VECTYPE_EDGECORN_08 ||
					identifier==VECTYPE_EDGEEDGE_10 || identifier==VECTYPE_EDGECORN_10){
				cell.ox -= cell.size;
			}
		}

		if(oychild){
			if (identifier==VECTYPE_WALLWALL_2 ||
					identifier==VECTYPE_WALLEDGE_2 ||
					identifier==VECTYPE_WALLCORN_2 ||
					identifier==VECTYPE_EDGEEDGE_00 || identifier==VECTYPE_EDGECORN_00 ||
					identifier==VECTYPE_EDGEEDGE_02 || identifier==VECTYPE_EDGECORN_02 ||
					identifier==VECTYPE_EDGEEDGE_08 || identifier==VECTYPE_EDGECORN_08 ||
					identifier==VECTYPE_EDGEEDGE_09 || identifier==VECTYPE_EDGECORN_09){
				cell.oy -= cell.size;
			}
		}

		if(ozchild){
			if (identifier==VECTYPE_WALLWALL_4 ||
					identifier==VECTYPE_WALLEDGE_4 ||
					identifier==VECTYPE_WALLCORN_4 ||
					identifier==VECTYPE_EDGEEDGE_00 || identifier==VECTYPE_EDGECORN_00 ||
					identifier==VECTYPE_EDGEEDGE_01 || identifier==VECTYPE_EDGECORN_01 ||
					identifier==VECTYPE_EDGEEDGE_04 || identifier==VECTYPE_EDGECORN_04 ||
					identifier==VECTYPE_EDGEEDGE_05 || identifier==VECTYPE_EDGECORN_05){
				cell.oz -= cell.size;
			}
		}
	}

	if(oxchild){
		if(_boundary.indexCorner[0]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[0]].ox -= _meshCells[_boundary.indexCorner[0]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[0]");
		if(_boundary.indexCorner[2]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[2]].ox -= _meshCells[_boundary.indexCorner[2]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[2]");
		if(_boundary.indexCorner[4]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[4]].ox -= _meshCells[_boundary.indexCorner[4]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[4]");
		if(_boundary.indexCorner[6]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[6]].ox -= _meshCells[_boundary.indexCorner[6]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[6]");
	}
	if(oychild){
		if(_boundary.indexCorner[0]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[0]].oy -= _meshCells[_boundary.indexCorner[0]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[0]");
		if(_boundary.indexCorner[2]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[1]].oy -= _meshCells[_boundary.indexCorner[1]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[1]");
		if(_boundary.indexCorner[4]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[4]].oy -= _meshCells[_boundary.indexCorner[4]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[4]");
		if(_boundary.indexCorner[6]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[5]].oy -= _meshCells[_boundary.indexCorner[5]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[5]");
	}
	if(ozchild){
		if(_boundary.indexCorner[0]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[0]].oz -= _meshCells[_boundary.indexCorner[0]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[0]");
		if(_boundary.indexCorner[2]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[1]].oz -= _meshCells[_boundary.indexCorner[1]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[1]");
		if(_boundary.indexCorner[4]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[2]].oz -= _meshCells[_boundary.indexCorner[2]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[2]");
		if(_boundary.indexCorner[6]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[3]].oz -= _meshCells[_boundary.indexCorner[3]].size;
		else fprintf(stderr,"\nERROR MCNCompact: Missing Boundary Corner[3]");
	}


  for(volumetype co2=0;co2<8;co2++){
  	if(co2!=co){
	  	sidetype oxnew = ((co2&1)!=0)*_n;
	  	sidetype oynew = ((co2&2)!=0)*_n;
	  	sidetype oznew = ((co2&4)!=0)*_n;
	  	_meshCellIndicesBranch[index+co2].indexInterior = _meshCells.size();
	  	_meshCells.push_back(MeshCell(0,oxnew,oynew,oznew,_n,BRANCHINIT,BRANCHINIT)); //0-6
  	}
  }

  //The Inner Cells not concerning the former root, now child

  MCNCompact newboundary;


  fprintf(stderr,"\nAdding Cells");
  fprintf(stderr,"\n Walls");

  //Inner Walls
	if((co&6)!=0){
  	_meshCellIndicesBranch[index+0].add(VECTYPE_WALLWALL,1,_meshCells.size());
  	_meshCellIndicesBranch[index+1].add(VECTYPE_WALLWALL,0,_meshCells.size());
		_meshCells.push_back(MeshCell(1,0,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //7
		_meshCells.back().lastBranch[0] = index+0;
		_meshCells.back().lastBranch[1] = index+1;

		//Boundary
		_meshCells.push_back(MeshCell(1,0 ,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //8
		_meshCells.push_back(MeshCell(1,_n,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //9
  	_meshCellIndicesBranch[index+0].add(VECTYPE_WALLWALL,0,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+1].add(VECTYPE_WALLWALL,1,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,0,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,1,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+0;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+1;
	}
	if((co&6)!=2){
  	_meshCellIndicesBranch[index+2].add(VECTYPE_WALLWALL,1,_meshCells.size());
  	_meshCellIndicesBranch[index+3].add(VECTYPE_WALLWALL,0,_meshCells.size());
		_meshCells.push_back(MeshCell(1,0,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //10
		_meshCells.back().lastBranch[0] = index+2;
		_meshCells.back().lastBranch[1] = index+3;

		//Boundary
		_meshCells.push_back(MeshCell(1,0 ,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //11
		_meshCells.push_back(MeshCell(1,_n,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //12
  	_meshCellIndicesBranch[index+2].add(VECTYPE_WALLWALL,0,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+3].add(VECTYPE_WALLWALL,1,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,0,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,1,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+2;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+3;
	}
	if((co&6)!=4){
  	_meshCellIndicesBranch[index+4].add(VECTYPE_WALLWALL,1,_meshCells.size());
  	_meshCellIndicesBranch[index+5].add(VECTYPE_WALLWALL,0,_meshCells.size());
		_meshCells.push_back(MeshCell(1,0,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //13
		_meshCells.back().lastBranch[0] = index+4;
		_meshCells.back().lastBranch[1] = index+5;

		//Boundary
		_meshCells.push_back(MeshCell(1,0 ,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //14
		_meshCells.push_back(MeshCell(1,_n,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //15
  	_meshCellIndicesBranch[index+4].add(VECTYPE_WALLWALL,0,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+5].add(VECTYPE_WALLWALL,1,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,0,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,1,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+4;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+5;
	}
	if((co&6)!=6){
  	_meshCellIndicesBranch[index+6].add(VECTYPE_WALLWALL,1,_meshCells.size());
  	_meshCellIndicesBranch[index+7].add(VECTYPE_WALLWALL,0,_meshCells.size());
		_meshCells.push_back(MeshCell(1,0,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.back().lastBranch[0] = index+6;
		_meshCells.back().lastBranch[1] = index+7;

		//Boundary
		_meshCells.push_back(MeshCell(1,0 ,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.push_back(MeshCell(1,_n,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //x
  	_meshCellIndicesBranch[index+6].add(VECTYPE_WALLWALL,0,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+7].add(VECTYPE_WALLWALL,1,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,0,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,1,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+6;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+7;
	}

	if((co&5)!=0){
  	_meshCellIndicesBranch[index+0].add(VECTYPE_WALLWALL,3,_meshCells.size());
  	_meshCellIndicesBranch[index+2].add(VECTYPE_WALLWALL,2,_meshCells.size());
		_meshCells.push_back(MeshCell(2,0 ,0,0 ,_n,BRANCHINIT,BRANCHINIT)); //16
		_meshCells.back().lastBranch[0] = index+0;
		_meshCells.back().lastBranch[1] = index+2;

		//Boundary
		_meshCells.push_back(MeshCell(2,0 ,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //17
		_meshCells.push_back(MeshCell(2,0 ,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //18
  	_meshCellIndicesBranch[index+0].add(VECTYPE_WALLWALL,2,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+2].add(VECTYPE_WALLWALL,3,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,2,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,3,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+0;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+2;
	}
	if((co&5)!=1){
  	_meshCellIndicesBranch[index+1].add(VECTYPE_WALLWALL,3,_meshCells.size());
  	_meshCellIndicesBranch[index+3].add(VECTYPE_WALLWALL,2,_meshCells.size());
		_meshCells.push_back(MeshCell(2,_n,0,0 ,_n,BRANCHINIT,BRANCHINIT)); //19
		_meshCells.back().lastBranch[0] = index+1;
		_meshCells.back().lastBranch[1] = index+3;

		//Boundary
		_meshCells.push_back(MeshCell(2,_n,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //20
		_meshCells.push_back(MeshCell(2,_n,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //21
  	_meshCellIndicesBranch[index+1].add(VECTYPE_WALLWALL,2,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+3].add(VECTYPE_WALLWALL,3,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,2,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,3,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+1;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+3;
	}
	if((co&5)!=4){
  	_meshCellIndicesBranch[index+4].add(VECTYPE_WALLWALL,3,_meshCells.size());
  	_meshCellIndicesBranch[index+6].add(VECTYPE_WALLWALL,2,_meshCells.size());
		_meshCells.push_back(MeshCell(2,0 ,0,_n,_n,BRANCHINIT,BRANCHINIT)); //22
		_meshCells.back().lastBranch[0] = index+4;
		_meshCells.back().lastBranch[1] = index+6;

		//Boundary
		_meshCells.push_back(MeshCell(2,0 ,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //23
		_meshCells.push_back(MeshCell(2,0 ,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //24
  	_meshCellIndicesBranch[index+4].add(VECTYPE_WALLWALL,2,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+6].add(VECTYPE_WALLWALL,3,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,2,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,3,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+4;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+6;
	}
	if((co&5)!=5){
  	_meshCellIndicesBranch[index+5].add(VECTYPE_WALLWALL,3,_meshCells.size());
  	_meshCellIndicesBranch[index+7].add(VECTYPE_WALLWALL,2,_meshCells.size());
		_meshCells.push_back(MeshCell(2,_n,0,_n,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.back().lastBranch[0] = index+5;
		_meshCells.back().lastBranch[1] = index+7;

		//Boundary
		_meshCells.push_back(MeshCell(2,_n,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.push_back(MeshCell(2,_n,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //x
  	_meshCellIndicesBranch[index+5].add(VECTYPE_WALLWALL,2,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+7].add(VECTYPE_WALLWALL,3,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,2,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,3,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+5;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+7;
  	fprintf(stderr,"!");
	}

	if((co&3)!=0){
  	_meshCellIndicesBranch[index+0].add(VECTYPE_WALLWALL,5,_meshCells.size());
  	_meshCellIndicesBranch[index+4].add(VECTYPE_WALLWALL,4,_meshCells.size());
		_meshCells.push_back(MeshCell(3,0 ,0 ,0,_n,BRANCHINIT,BRANCHINIT)); //25
		_meshCells.back().lastBranch[0] = index+0;
		_meshCells.back().lastBranch[1] = index+4;

		//Boundary
		_meshCells.push_back(MeshCell(3,0 ,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //26
		_meshCells.push_back(MeshCell(3,0 ,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //27
  	_meshCellIndicesBranch[index+0].add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+4].add(VECTYPE_WALLWALL,5,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,5,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+0;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+4;
	}
	if((co&3)!=1){
  	_meshCellIndicesBranch[index+1].add(VECTYPE_WALLWALL,5,_meshCells.size());
  	_meshCellIndicesBranch[index+5].add(VECTYPE_WALLWALL,4,_meshCells.size());
		_meshCells.push_back(MeshCell(3,_n,0 ,0,_n,BRANCHINIT,BRANCHINIT)); //28
		_meshCells.back().lastBranch[0] = index+1;
		_meshCells.back().lastBranch[1] = index+5;

		//Boundary
		_meshCells.push_back(MeshCell(3,_n,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //29
		_meshCells.push_back(MeshCell(3,_n,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //30
  	_meshCellIndicesBranch[index+1].add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+5].add(VECTYPE_WALLWALL,5,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,5,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+1;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+5;
	}
	if((co&3)!=2){
  	_meshCellIndicesBranch[index+2].add(VECTYPE_WALLWALL,5,_meshCells.size());
  	_meshCellIndicesBranch[index+6].add(VECTYPE_WALLWALL,4,_meshCells.size());
		_meshCells.push_back(MeshCell(3,0 ,_n,0,_n,BRANCHINIT,BRANCHINIT)); //31
		_meshCells.back().lastBranch[0] = index+2;
		_meshCells.back().lastBranch[1] = index+6;

		//Boundary
		_meshCells.push_back(MeshCell(3,0 ,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //32
		_meshCells.push_back(MeshCell(3,0 ,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //33
  	_meshCellIndicesBranch[index+2].add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+6].add(VECTYPE_WALLWALL,5,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,5,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+2;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+6;
	}
	if((co&3)!=3){
  	_meshCellIndicesBranch[index+3].add(VECTYPE_WALLWALL,5,_meshCells.size());
  	_meshCellIndicesBranch[index+7].add(VECTYPE_WALLWALL,4,_meshCells.size());
		_meshCells.push_back(MeshCell(3,_n,_n,0,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.back().lastBranch[0] = index+3;
		_meshCells.back().lastBranch[1] = index+7;

		//Boundary
		_meshCells.push_back(MeshCell(3,_n,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.push_back(MeshCell(3,_n,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //x
  	_meshCellIndicesBranch[index+3].add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
  	_meshCellIndicesBranch[index+7].add(VECTYPE_WALLWALL,5,_meshCells.size()-1);
  	newboundary.add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
  	newboundary.add(VECTYPE_WALLWALL,5,_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+3;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+7;
	}


	_meshCells.push_back(MeshCell(1,_n*((co&1)==0),_n*((co&2)!=0),_n*((co&4)!=0),_n,BRANCHINIT,BRANCHINIT)); //34
	_meshCellIndicesBranch[index+(co&6)+((co&1)==0)].add(VECTYPE_WALLWALL,0+((co&1)==0),_meshCells.size()-1);
	newboundary.add(VECTYPE_WALLWALL,0+((co&1)==0),_meshCells.size()-1);
	if((co&1)==0) _meshCells[_meshCells.size()-1].lastBranch[0] = index+(co&6)+1;
	else          _meshCells[_meshCells.size()-1].lastBranch[1] = index+(co&6)+0;

	_meshCells.push_back(MeshCell(2,_n*((co&1)!=0),_n*((co&2)==0),_n*((co&4)!=0),_n,BRANCHINIT,BRANCHINIT)); //35
	_meshCellIndicesBranch[index+(co&5)+2*((co&2)==0)].add(VECTYPE_WALLWALL,2+((co&2)==0),_meshCells.size()-1);
	newboundary.add(VECTYPE_WALLWALL,2+((co&2)==0),_meshCells.size()-1);
	if((co&2)==0) _meshCells[_meshCells.size()-1].lastBranch[0] = index+(co&5)+2;
	else          _meshCells[_meshCells.size()-1].lastBranch[1] = index+(co&5)+0;

	_meshCells.push_back(MeshCell(3,_n*((co&1)!=0),_n*((co&2)!=0),_n*((co&4)==0),_n,BRANCHINIT,BRANCHINIT)); //36
	_meshCellIndicesBranch[index+(co&3)+4*((co&4)==0)].add(VECTYPE_WALLWALL,4+((co&4)==0),_meshCells.size()-1);
	newboundary.add(VECTYPE_WALLWALL,4+((co&4)==0),_meshCells.size()-1);
	if((co&4)==0) _meshCells[_meshCells.size()-1].lastBranch[0] = index+(co&3)+4;
	else          _meshCells[_meshCells.size()-1].lastBranch[1] = index+(co&3)+0;


  fprintf(stderr,"\n Edges");
	//Edges
	//X Inner
	bool shift = ((co&1)==0);
	_meshCellIndicesBranch[index+0+shift].add(VECTYPE_EDGEEDGE,3,_meshCells.size());
	_meshCellIndicesBranch[index+2+shift].add(VECTYPE_EDGEEDGE,2,_meshCells.size());
	_meshCellIndicesBranch[index+4+shift].add(VECTYPE_EDGEEDGE,1,_meshCells.size());
	_meshCellIndicesBranch[index+6+shift].add(VECTYPE_EDGEEDGE,0,_meshCells.size());
	_meshCells.push_back(MeshCell(4,_n*shift,0,0,_n,BRANCHINIT,BRANCHINIT)); //37
	_meshCells.back().lastBranch[0] = index+0+shift;
	_meshCells.back().lastBranch[1] = index+2+shift;
	_meshCells.back().lastBranch[2] = index+4+shift;
	_meshCells.back().lastBranch[3] = index+6+shift;

	//X Boundary
	_meshCells.push_back(MeshCell(4,_n*shift,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //38
	_meshCells.push_back(MeshCell(4,_n*shift,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //39
	_meshCells.push_back(MeshCell(4,_n*shift,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //40
	_meshCells.push_back(MeshCell(4,_n*shift,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //41
	_meshCells.push_back(MeshCell(4,_n*shift,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //42
	_meshCells.push_back(MeshCell(4,_n*shift,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //43
	_meshCells.push_back(MeshCell(4,_n*shift,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //44
	_meshCells.push_back(MeshCell(4,_n*shift,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //45

	_meshCellIndicesBranch[index+0+shift].add(VECTYPE_EDGEEDGE,0,_meshCells.size()-8);
	_meshCellIndicesBranch[index+0+shift].add(VECTYPE_EDGEEDGE,1,_meshCells.size()-7);
	_meshCellIndicesBranch[index+0+shift].add(VECTYPE_EDGEEDGE,2,_meshCells.size()-5);

	_meshCellIndicesBranch[index+2+shift].add(VECTYPE_EDGEEDGE,0,_meshCells.size()-7);
	_meshCellIndicesBranch[index+2+shift].add(VECTYPE_EDGEEDGE,1,_meshCells.size()-6);
	_meshCellIndicesBranch[index+2+shift].add(VECTYPE_EDGEEDGE,3,_meshCells.size()-4);

	_meshCellIndicesBranch[index+4+shift].add(VECTYPE_EDGEEDGE,0,_meshCells.size()-5);
	_meshCellIndicesBranch[index+4+shift].add(VECTYPE_EDGEEDGE,2,_meshCells.size()-3);
	_meshCellIndicesBranch[index+4+shift].add(VECTYPE_EDGEEDGE,3,_meshCells.size()-2);

	_meshCellIndicesBranch[index+6+shift].add(VECTYPE_EDGEEDGE,1,_meshCells.size()-4);
	_meshCellIndicesBranch[index+6+shift].add(VECTYPE_EDGEEDGE,2,_meshCells.size()-2);
	_meshCellIndicesBranch[index+6+shift].add(VECTYPE_EDGEEDGE,3,_meshCells.size()-1);

	newboundary.add(VECTYPE_EDGEEDGE,0,_meshCells.size()-8);
	newboundary.add(VECTYPE_EDGEEDGE,1,_meshCells.size()-6);
	newboundary.add(VECTYPE_EDGEEDGE,2,_meshCells.size()-3);
	newboundary.add(VECTYPE_EDGEEDGE,3,_meshCells.size()-1);

	newboundary.add(VECTYPE_WALLEDGE,4,_meshCells.size()-7);
	newboundary.add(VECTYPE_WALLEDGE,2,_meshCells.size()-5);
	newboundary.add(VECTYPE_WALLEDGE,3,_meshCells.size()-4);
	newboundary.add(VECTYPE_WALLEDGE,5,_meshCells.size()-2);

	_meshCells[_meshCells.size()-8].lastBranch[3] = index+0+shift;
	_meshCells[_meshCells.size()-7].lastBranch[2] = index+0+shift;
	_meshCells[_meshCells.size()-7].lastBranch[3] = index+2+shift;
	_meshCells[_meshCells.size()-6].lastBranch[2] = index+2+shift;
	_meshCells[_meshCells.size()-5].lastBranch[1] = index+0+shift;
	_meshCells[_meshCells.size()-5].lastBranch[3] = index+4+shift;
	_meshCells[_meshCells.size()-4].lastBranch[0] = index+2+shift;
	_meshCells[_meshCells.size()-4].lastBranch[2] = index+6+shift;
	_meshCells[_meshCells.size()-3].lastBranch[1] = index+4+shift;
	_meshCells[_meshCells.size()-2].lastBranch[0] = index+4+shift;
	_meshCells[_meshCells.size()-2].lastBranch[1] = index+6+shift;
	_meshCells[_meshCells.size()-1].lastBranch[0] = index+6+shift;

	// Z
	// 4---3---2            2---+---+
	// |   |   |            |   | x |
	// +---+---1            1---+---+
	// | x |   |            |   |   |
	// +---+---0 Y          0---3---4
	_meshCells.push_back(MeshCell(4,_n*((co&1)!=0),_n*((co&2)==0),0             ,_n,BRANCHINIT,BRANCHINIT)); //46
	_meshCells.push_back(MeshCell(4,_n*((co&1)!=0),_n*((co&2)==0),0             ,_n,BRANCHINIT,BRANCHINIT)); //47
	_meshCells.push_back(MeshCell(4,_n*((co&1)!=0),_n*((co&2)==0),_n            ,_n,BRANCHINIT,BRANCHINIT)); //48
	_meshCells.push_back(MeshCell(4,_n*((co&1)!=0),0             ,_n*((co&4)==0),_n,BRANCHINIT,BRANCHINIT)); //49
	_meshCells.push_back(MeshCell(4,_n*((co&1)!=0),_n*((co&2)!=0),_n*((co&4)==0),_n,BRANCHINIT,BRANCHINIT)); //50

	_meshCells.push_back(MeshCell(7,0,_n*((co&2)==0),0             ,_n,BRANCHINIT,BRANCHINIT)); //46
	_meshCells.push_back(MeshCell(7,0,_n*((co&2)==0),0             ,_n,BRANCHINIT,BRANCHINIT)); //47
	_meshCells.push_back(MeshCell(7,0,_n*((co&2)==0),_n            ,_n,BRANCHINIT,BRANCHINIT)); //48
	_meshCells.push_back(MeshCell(7,0,0             ,_n*((co&4)==0),_n,BRANCHINIT,BRANCHINIT)); //49
	_meshCells.push_back(MeshCell(7,0,_n*((co&2)!=0),_n*((co&4)==0),_n,BRANCHINIT,BRANCHINIT)); //50

	_meshCellIndicesBranch[index+(co&1)+2*((co&2)==0)  ].add(VECTYPE_EDGEEDGE,0+((co&2)==0),_meshCells.size()-10);
	_meshCellIndicesBranch[index+(co&1)+2*((co&2)==0)  ].add(VECTYPE_EDGEEDGE,2+((co&2)==0),_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&1)+2*((co&2)==0)+4].add(VECTYPE_EDGEEDGE,0+((co&2)==0),_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&1)+2*((co&2)==0)+4].add(VECTYPE_EDGEEDGE,2+((co&2)==0),_meshCells.size()-8);
	_meshCellIndicesBranch[index+(co&1)+4*((co&4)==0)+0].add(VECTYPE_EDGEEDGE,2*((co&4)==0)+1,_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&1)+4*((co&4)==0)+2].add(VECTYPE_EDGEEDGE,2*((co&4)==0)+0,_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&1)+2*((co&2)!=0)+4*((co&4)==0)].add(VECTYPE_EDGEEDGE,((co&2)!=0)+2*((co&4)==0),_meshCells.size()-6);

	_meshCellIndicesBranch[index+0+2*((co&2)==0)  ].indexCorner[1+2*((co&2)==0)] = _meshCells.size()-5;
	_meshCellIndicesBranch[index+1+2*((co&2)==0)  ].indexCorner[0+2*((co&2)==0)] = _meshCells.size()-5;
	_meshCellIndicesBranch[index+0+2*((co&2)==0)  ].indexCorner[5+2*((co&2)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+1+2*((co&2)==0)  ].indexCorner[4+2*((co&2)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+0+2*((co&2)==0)+4].indexCorner[1+((co&2)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+1+2*((co&2)==0)+4].indexCorner[0+((co&2)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+0+2*((co&2)==0)+4].indexCorner[5+((co&2)==0)] = _meshCells.size()-3;
	_meshCellIndicesBranch[index+1+2*((co&2)==0)+4].indexCorner[4+((co&2)==0)] = _meshCells.size()-3;
	_meshCellIndicesBranch[index+0+4*((co&4)==0)+0].indexCorner[3+4*((co&4)==0)] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+1+4*((co&4)==0)+0].indexCorner[2+4*((co&4)==0)] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+0+4*((co&4)==0)+2].indexCorner[1+4*((co&4)==0)] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+1+4*((co&4)==0)+2].indexCorner[0+4*((co&4)==0)] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+0+2*((co&2)!=0)+4*((co&4)==0)].indexCorner[1+2*((co&2)!=0)+4*((co&4)==0)] = _meshCells.size()-1;
	_meshCellIndicesBranch[index+1+2*((co&2)!=0)+4*((co&4)==0)].indexCorner[0+2*((co&2)!=0)+4*((co&4)==0)] = _meshCells.size()-1;

	newboundary.add(VECTYPE_EDGEEDGE,0+((co&2)==0),_meshCells.size()-10);
	newboundary.add(VECTYPE_EDGEEDGE,2+((co&2)==0),_meshCells.size()-8);
	newboundary.add(VECTYPE_EDGEEDGE,((co&2)!=0)+2*((co&4)==0),_meshCells.size()-6);
	newboundary.add(VECTYPE_WALLEDGE,2+((co&2)==0),_meshCells.size()-9);
	newboundary.add(VECTYPE_WALLEDGE,4+((co&4)==0),_meshCells.size()-7);

	newboundary.add(VECTYPE_EDGECORN,0+((co&2)==0),_meshCells.size()-5);
	newboundary.add(VECTYPE_EDGECORN,2+((co&2)==0),_meshCells.size()-3);
	newboundary.add(VECTYPE_EDGECORN,((co&2)!=0)+2*((co&4)==0),_meshCells.size()-1);
	newboundary.add(VECTYPE_WALLCORN,2+((co&2)==0),_meshCells.size()-4);
	newboundary.add(VECTYPE_WALLCORN,4+((co&4)==0),_meshCells.size()-2);

	if((co&2)!=0){
		_meshCells[_meshCells.size()-10].lastBranch[3] = index+(co&1)+0;
		_meshCells[_meshCells.size()- 5].lastBranch[6] = index+0+0;
		_meshCells[_meshCells.size()- 5].lastBranch[7] = index+1+0;
	}
	else{
		_meshCells[_meshCells.size()-10].lastBranch[2] = index+(co&1)+2;
		_meshCells[_meshCells.size()- 5].lastBranch[4] = index+0+2;
		_meshCells[_meshCells.size()- 5].lastBranch[5] = index+1+2;
	}
	if((co&2)!=0){
		_meshCells[_meshCells.size()-9].lastBranch[1] = index+(co&1)+0;
		_meshCells[_meshCells.size()-9].lastBranch[3] = index+(co&1)+4;
		_meshCells[_meshCells.size()-4].lastBranch[2] = index+0+0;
		_meshCells[_meshCells.size()-4].lastBranch[3] = index+1+0;
		_meshCells[_meshCells.size()-4].lastBranch[6] = index+0+4;
		_meshCells[_meshCells.size()-4].lastBranch[7] = index+1+4;
	}
	else{
		_meshCells[_meshCells.size()-9].lastBranch[0] = index+(co&1)+2;
		_meshCells[_meshCells.size()-9].lastBranch[2] = index+(co&1)+6;
		_meshCells[_meshCells.size()-4].lastBranch[0] = index+0+2;
		_meshCells[_meshCells.size()-4].lastBranch[1] = index+1+2;
		_meshCells[_meshCells.size()-4].lastBranch[4] = index+0+6;
		_meshCells[_meshCells.size()-4].lastBranch[5] = index+1+6;
	}
	if((co&2)!=0){
		_meshCells[_meshCells.size()-8].lastBranch[1] = index+(co&1)+4;
		_meshCells[_meshCells.size()-3].lastBranch[2] = index+0+4;
		_meshCells[_meshCells.size()-3].lastBranch[3] = index+1+4;
	}
	else{
		_meshCells[_meshCells.size()-8].lastBranch[0] = index+(co&1)+6;
		_meshCells[_meshCells.size()-3].lastBranch[0] = index+0+6;
		_meshCells[_meshCells.size()-3].lastBranch[1] = index+1+6;
	}
	if((co&4)!=0){
		_meshCells[_meshCells.size()-7].lastBranch[2] = index+(co&1)+0;
		_meshCells[_meshCells.size()-7].lastBranch[3] = index+(co&1)+2;
		_meshCells[_meshCells.size()-2].lastBranch[4] = index+0+0;
		_meshCells[_meshCells.size()-2].lastBranch[5] = index+1+0;
		_meshCells[_meshCells.size()-2].lastBranch[6] = index+0+2;
		_meshCells[_meshCells.size()-2].lastBranch[7] = index+1+2;
	}
	else{
		_meshCells[_meshCells.size()-7].lastBranch[0] = index+(co&1)+4;
		_meshCells[_meshCells.size()-7].lastBranch[1] = index+(co&1)+6;
		_meshCells[_meshCells.size()-2].lastBranch[0] = index+0+4;
		_meshCells[_meshCells.size()-2].lastBranch[1] = index+1+4;
		_meshCells[_meshCells.size()-2].lastBranch[2] = index+0+6;
		_meshCells[_meshCells.size()-2].lastBranch[3] = index+1+6;
	}
	switch(co&6){
	case 0:
	_meshCells[_meshCells.size()-6].lastBranch[1] = index+(co&1)+4;
	_meshCells[_meshCells.size()-1].lastBranch[2] = index+0+4;
	_meshCells[_meshCells.size()-1].lastBranch[3] = index+1+4;
	break;
	case 2:
	_meshCells[_meshCells.size()-6].lastBranch[0] = index+(co&1)+6;
	_meshCells[_meshCells.size()-1].lastBranch[0] = index+0+6;
	_meshCells[_meshCells.size()-1].lastBranch[1] = index+1+6;
	break;
	case 4:
	_meshCells[_meshCells.size()-6].lastBranch[3] = index+(co&1)+0;
	_meshCells[_meshCells.size()-1].lastBranch[6] = index+0+0;
	_meshCells[_meshCells.size()-1].lastBranch[7] = index+1+0;
	break;
	case 6:
	_meshCells[_meshCells.size()-6].lastBranch[2] = index+(co&1)+2;
	_meshCells[_meshCells.size()-1].lastBranch[4] = index+0+2;
	_meshCells[_meshCells.size()-1].lastBranch[5] = index+1+2;
	break;
	}

	//Y Inner
	shift = ((co&2)==0);
	_meshCellIndicesBranch[index+0+2*shift].add(VECTYPE_EDGEEDGE,7,_meshCells.size());
	_meshCellIndicesBranch[index+1+2*shift].add(VECTYPE_EDGEEDGE,6,_meshCells.size());
	_meshCellIndicesBranch[index+4+2*shift].add(VECTYPE_EDGEEDGE,5,_meshCells.size());
	_meshCellIndicesBranch[index+5+2*shift].add(VECTYPE_EDGEEDGE,4,_meshCells.size());
	_meshCells.push_back(MeshCell(5,0,_n*shift,0,_n,BRANCHINIT,BRANCHINIT)); //51
//		fprintf(stderr,"\nCreated inner Y Edge %li",_meshCells.size()-1);
	_meshCells.back().lastBranch[0] = index+0+2*shift;
	_meshCells.back().lastBranch[1] = index+1+2*shift;
	_meshCells.back().lastBranch[2] = index+4+2*shift;
	_meshCells.back().lastBranch[3] = index+5+2*shift;

	//Y Boundary
	_meshCells.push_back(MeshCell(5,0 ,_n*shift,0 ,_n,BRANCHINIT,BRANCHINIT)); //52
	_meshCells.push_back(MeshCell(5,0 ,_n*shift,0 ,_n,BRANCHINIT,BRANCHINIT)); //53
	_meshCells.push_back(MeshCell(5,_n,_n*shift,0 ,_n,BRANCHINIT,BRANCHINIT)); //54
	_meshCells.push_back(MeshCell(5,0 ,_n*shift,0 ,_n,BRANCHINIT,BRANCHINIT)); //55
	_meshCells.push_back(MeshCell(5,_n,_n*shift,0 ,_n,BRANCHINIT,BRANCHINIT)); //56
	_meshCells.push_back(MeshCell(5,0 ,_n*shift,_n,_n,BRANCHINIT,BRANCHINIT)); //57
	_meshCells.push_back(MeshCell(5,0 ,_n*shift,_n,_n,BRANCHINIT,BRANCHINIT)); //58
	_meshCells.push_back(MeshCell(5,_n,_n*shift,_n,_n,BRANCHINIT,BRANCHINIT)); //59
//		fprintf(stderr,"\nCreated outer Y Boundary Edges"
//				"\n%li %li %li %li %li %li %li %li",
//				_meshCells.size()-8,_meshCells.size()-7,_meshCells.size()-6,_meshCells.size()-5,
//				_meshCells.size()-4,_meshCells.size()-3,_meshCells.size()-2,_meshCells.size()-1);

	_meshCellIndicesBranch[index+0+2*shift].add(VECTYPE_EDGEEDGE,4,_meshCells.size()-8);
	_meshCellIndicesBranch[index+0+2*shift].add(VECTYPE_EDGEEDGE,5,_meshCells.size()-7);
	_meshCellIndicesBranch[index+0+2*shift].add(VECTYPE_EDGEEDGE,6,_meshCells.size()-5);

	_meshCellIndicesBranch[index+1+2*shift].add(VECTYPE_EDGEEDGE,4,_meshCells.size()-7);
	_meshCellIndicesBranch[index+1+2*shift].add(VECTYPE_EDGEEDGE,5,_meshCells.size()-6);
	_meshCellIndicesBranch[index+1+2*shift].add(VECTYPE_EDGEEDGE,7,_meshCells.size()-4);

	_meshCellIndicesBranch[index+4+2*shift].add(VECTYPE_EDGEEDGE,4,_meshCells.size()-5);
	_meshCellIndicesBranch[index+4+2*shift].add(VECTYPE_EDGEEDGE,6,_meshCells.size()-3);
	_meshCellIndicesBranch[index+4+2*shift].add(VECTYPE_EDGEEDGE,7,_meshCells.size()-2);

	_meshCellIndicesBranch[index+5+2*shift].add(VECTYPE_EDGEEDGE,5,_meshCells.size()-4);
	_meshCellIndicesBranch[index+5+2*shift].add(VECTYPE_EDGEEDGE,6,_meshCells.size()-2);
	_meshCellIndicesBranch[index+5+2*shift].add(VECTYPE_EDGEEDGE,7,_meshCells.size()-1);

	newboundary.add(VECTYPE_EDGEEDGE,4,_meshCells.size()-8);
	newboundary.add(VECTYPE_EDGEEDGE,5,_meshCells.size()-6);
	newboundary.add(VECTYPE_EDGEEDGE,6,_meshCells.size()-3);
	newboundary.add(VECTYPE_EDGEEDGE,7,_meshCells.size()-1);

	newboundary.add(VECTYPE_WALLEDGE,4,_meshCells.size()-7);
	newboundary.add(VECTYPE_WALLEDGE,0,_meshCells.size()-5);
	newboundary.add(VECTYPE_WALLEDGE,1,_meshCells.size()-4);
	newboundary.add(VECTYPE_WALLEDGE,5,_meshCells.size()-2);

	_meshCells[_meshCells.size()-8].lastBranch[3] = index+0+2*shift;
	_meshCells[_meshCells.size()-7].lastBranch[2] = index+0+2*shift;
	_meshCells[_meshCells.size()-7].lastBranch[3] = index+1+2*shift;
	_meshCells[_meshCells.size()-6].lastBranch[2] = index+1+2*shift;
	_meshCells[_meshCells.size()-5].lastBranch[1] = index+0+2*shift;
	_meshCells[_meshCells.size()-5].lastBranch[3] = index+4+2*shift;
	_meshCells[_meshCells.size()-4].lastBranch[0] = index+1+2*shift;
	_meshCells[_meshCells.size()-4].lastBranch[2] = index+5+2*shift;
	_meshCells[_meshCells.size()-3].lastBranch[1] = index+4+2*shift;
	_meshCells[_meshCells.size()-2].lastBranch[0] = index+4+2*shift;
	_meshCells[_meshCells.size()-2].lastBranch[1] = index+5+2*shift;
	_meshCells[_meshCells.size()-1].lastBranch[0] = index+5+2*shift;

	  	// Z
	// 4---3---2            2---+---+
	// |   |   |            |   | x |
	// +---+---1            1---+---+
	// | x |   |            |   |   |
	// +---+---0 X          0---3---4
	_meshCells.push_back(MeshCell(5,_n*((co&1)==0),_n*((co&2)!=0),0             ,_n,BRANCHINIT,BRANCHINIT)); //60
	_meshCells.push_back(MeshCell(5,_n*((co&1)==0),_n*((co&2)!=0),0             ,_n,BRANCHINIT,BRANCHINIT)); //61
	_meshCells.push_back(MeshCell(5,_n*((co&1)==0),_n*((co&2)!=0),_n            ,_n,BRANCHINIT,BRANCHINIT)); //62
	_meshCells.push_back(MeshCell(5,0             ,_n*((co&2)!=0),_n*((co&4)==0),_n,BRANCHINIT,BRANCHINIT)); //63
	_meshCells.push_back(MeshCell(5,_n*((co&1)!=0),_n*((co&2)!=0),_n*((co&4)==0),_n,BRANCHINIT,BRANCHINIT)); //64

	_meshCells.push_back(MeshCell(7,_n*((co&1)==0),0,0             ,_n,BRANCHINIT,BRANCHINIT)); //60
	_meshCells.push_back(MeshCell(7,_n*((co&1)==0),0,0             ,_n,BRANCHINIT,BRANCHINIT)); //61
	_meshCells.push_back(MeshCell(7,_n*((co&1)==0),0,_n            ,_n,BRANCHINIT,BRANCHINIT)); //62
	_meshCells.push_back(MeshCell(7,0             ,0,_n*((co&4)==0),_n,BRANCHINIT,BRANCHINIT)); //63
	_meshCells.push_back(MeshCell(7,_n*((co&1)!=0),0,_n*((co&4)==0),_n,BRANCHINIT,BRANCHINIT)); //64

	_meshCellIndicesBranch[index+(co&2)+  ((co&1)==0)  ].add(VECTYPE_EDGEEDGE,4+((co&1)==0),_meshCells.size()-10);
	_meshCellIndicesBranch[index+(co&2)+  ((co&1)==0)  ].add(VECTYPE_EDGEEDGE,6+((co&1)==0),_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&2)+  ((co&1)==0)+4].add(VECTYPE_EDGEEDGE,4+((co&1)==0),_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&2)+  ((co&1)==0)+4].add(VECTYPE_EDGEEDGE,6+((co&1)==0),_meshCells.size()-8);
	_meshCellIndicesBranch[index+(co&2)+4*((co&4)==0)+0].add(VECTYPE_EDGEEDGE,4+2*((co&4)==0)+1,_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&2)+4*((co&4)==0)+1].add(VECTYPE_EDGEEDGE,4+2*((co&4)==0)+0,_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&2)+  ((co&1)!=0)+4*((co&4)==0)].add(VECTYPE_EDGEEDGE,4+((co&1)!=0)+2*((co&4)==0),_meshCells.size()-6);

	_meshCellIndicesBranch[index+0+  ((co&1)==0)  ].indexCorner[2+((co&1)==0)] = _meshCells.size()-5;
	_meshCellIndicesBranch[index+2+  ((co&1)==0)  ].indexCorner[0+((co&1)==0)] = _meshCells.size()-5;
	_meshCellIndicesBranch[index+0+  ((co&1)==0)  ].indexCorner[6+((co&1)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+2+  ((co&1)==0)  ].indexCorner[4+((co&1)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+0+  ((co&1)==0)+4].indexCorner[2+((co&1)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+2+  ((co&1)==0)+4].indexCorner[0+((co&1)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+0+  ((co&1)==0)+4].indexCorner[6+((co&1)==0)] = _meshCells.size()-3;
	_meshCellIndicesBranch[index+2+  ((co&1)==0)+4].indexCorner[4+((co&1)==0)] = _meshCells.size()-3;
	_meshCellIndicesBranch[index+0+4*((co&4)==0)+0].indexCorner[2+4*((co&4)==0)+1] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+2+4*((co&4)==0)+0].indexCorner[0+4*((co&4)==0)+1] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+0+4*((co&4)==0)+1].indexCorner[2+4*((co&4)==0)+0] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+2+4*((co&4)==0)+1].indexCorner[0+4*((co&4)==0)+0] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+0+  ((co&1)!=0)+4*((co&4)==0)].indexCorner[2+((co&1)!=0)+4*((co&4)==0)] = _meshCells.size()-1;
	_meshCellIndicesBranch[index+2+  ((co&1)!=0)+4*((co&4)==0)].indexCorner[0+((co&1)!=0)+4*((co&4)==0)] = _meshCells.size()-1;



	newboundary.add(VECTYPE_EDGEEDGE,4+((co&1)==0),_meshCells.size()-10);
	newboundary.add(VECTYPE_EDGEEDGE,6+((co&1)==0),_meshCells.size()-8);
	newboundary.add(VECTYPE_EDGEEDGE,4+((co&1)!=0)+2*((co&4)==0),_meshCells.size()-6);
	newboundary.add(VECTYPE_WALLEDGE,0+((co&1)==0),_meshCells.size()-9);
	newboundary.add(VECTYPE_WALLEDGE,4+((co&4)==0),_meshCells.size()-7);

	newboundary.add(VECTYPE_EDGECORN,4+((co&1)==0),_meshCells.size()-5);
	newboundary.add(VECTYPE_EDGECORN,6+((co&1)==0),_meshCells.size()-3);
	newboundary.add(VECTYPE_EDGECORN,4+((co&1)!=0)+2*((co&4)==0),_meshCells.size()-1);
	newboundary.add(VECTYPE_WALLCORN,0+((co&1)==0),_meshCells.size()-4);
	newboundary.add(VECTYPE_WALLCORN,4+((co&4)==0),_meshCells.size()-2);

	if((co&1)!=0){
		_meshCells[_meshCells.size()-10].lastBranch[3] = index+(co&2)+0;
		_meshCells[_meshCells.size()-5].lastBranch[5] = index+0+0;
		_meshCells[_meshCells.size()-5].lastBranch[7] = index+2+0;
	}
	else{
		_meshCells[_meshCells.size()-10].lastBranch[2] = index+(co&2)+1;
		_meshCells[_meshCells.size()-5].lastBranch[4] = index+0+1;
		_meshCells[_meshCells.size()-5].lastBranch[6] = index+2+1;
	}
	if((co&1)!=0){
		_meshCells[_meshCells.size()-9].lastBranch[1] = index+(co&2)+0;
		_meshCells[_meshCells.size()-9].lastBranch[3] = index+(co&2)+4;
		_meshCells[_meshCells.size()-4].lastBranch[1] = index+0+0;
		_meshCells[_meshCells.size()-4].lastBranch[3] = index+2+0;
		_meshCells[_meshCells.size()-4].lastBranch[5] = index+0+4;
		_meshCells[_meshCells.size()-4].lastBranch[7] = index+2+4;
	}
	else{
		_meshCells[_meshCells.size()-9].lastBranch[0] = index+(co&2)+1;
		_meshCells[_meshCells.size()-9].lastBranch[2] = index+(co&2)+5;
		_meshCells[_meshCells.size()-4].lastBranch[0] = index+0+1;
		_meshCells[_meshCells.size()-4].lastBranch[2] = index+2+1;
		_meshCells[_meshCells.size()-4].lastBranch[4] = index+0+5;
		_meshCells[_meshCells.size()-4].lastBranch[6] = index+2+5;
	}
	if((co&1)!=0){
		_meshCells[_meshCells.size()-8].lastBranch[1] = index+(co&2)+4;
		_meshCells[_meshCells.size()-3].lastBranch[1] = index+0+4;
		_meshCells[_meshCells.size()-3].lastBranch[3] = index+2+4;
	}
	else{
		_meshCells[_meshCells.size()-8].lastBranch[0] = index+(co&2)+5;
		_meshCells[_meshCells.size()-3].lastBranch[0] = index+0+5;
		_meshCells[_meshCells.size()-3].lastBranch[2] = index+2+5;
	}
	if((co&4)!=0){
		_meshCells[_meshCells.size()-7].lastBranch[2] = index+(co&2)+0;
		_meshCells[_meshCells.size()-7].lastBranch[3] = index+(co&2)+1;
		_meshCells[_meshCells.size()-2].lastBranch[4] = index+0+0;
		_meshCells[_meshCells.size()-2].lastBranch[6] = index+2+0;
		_meshCells[_meshCells.size()-2].lastBranch[5] = index+0+1;
		_meshCells[_meshCells.size()-2].lastBranch[7] = index+2+1;
	}
	else{
		_meshCells[_meshCells.size()-7].lastBranch[0] = index+(co&2)+4;
		_meshCells[_meshCells.size()-7].lastBranch[1] = index+(co&2)+5;
		_meshCells[_meshCells.size()-2].lastBranch[0] = index+0+4;
		_meshCells[_meshCells.size()-2].lastBranch[2] = index+2+4;
		_meshCells[_meshCells.size()-2].lastBranch[1] = index+0+5;
		_meshCells[_meshCells.size()-2].lastBranch[3] = index+2+5;
	}
	switch(co&5){
	case 0:
	_meshCells[_meshCells.size()-6].lastBranch[1] = index+(co&2)+4;
	_meshCells[_meshCells.size()-1].lastBranch[1] = index+0+4;
	_meshCells[_meshCells.size()-1].lastBranch[3] = index+2+4;
	break;
	case 1:
	_meshCells[_meshCells.size()-6].lastBranch[0] = index+(co&2)+5;
	_meshCells[_meshCells.size()-1].lastBranch[0] = index+0+5;
	_meshCells[_meshCells.size()-1].lastBranch[2] = index+2+5;
	break;
	case 4:
	_meshCells[_meshCells.size()-6].lastBranch[3] = index+(co&2)+0;
	_meshCells[_meshCells.size()-1].lastBranch[5] = index+0+0;
	_meshCells[_meshCells.size()-1].lastBranch[7] = index+2+0;
	break;
	case 5:
	_meshCells[_meshCells.size()-6].lastBranch[2] = index+(co&2)+1;
	_meshCells[_meshCells.size()-1].lastBranch[4] = index+0+1;
	_meshCells[_meshCells.size()-1].lastBranch[6] = index+2+1;
	break;
	}

	//Z Inner
	shift = ((co&4)==0);
	_meshCellIndicesBranch[index+0+4*shift].add(VECTYPE_EDGEEDGE,11,_meshCells.size());
	_meshCellIndicesBranch[index+1+4*shift].add(VECTYPE_EDGEEDGE,10,_meshCells.size());
	_meshCellIndicesBranch[index+2+4*shift].add(VECTYPE_EDGEEDGE, 9,_meshCells.size());
	_meshCellIndicesBranch[index+3+4*shift].add(VECTYPE_EDGEEDGE, 8,_meshCells.size());
	_meshCells.push_back(MeshCell(6,0,0,_n*shift,_n,BRANCHINIT,BRANCHINIT)); //65
	_meshCells.back().lastBranch[0] = index+0+4*shift;
	_meshCells.back().lastBranch[1] = index+1+4*shift;
	_meshCells.back().lastBranch[2] = index+2+4*shift;
	_meshCells.back().lastBranch[3] = index+3+4*shift;

	//Z Boundary
	_meshCells.push_back(MeshCell(6,0 ,0 ,_n*shift,_n,BRANCHINIT,BRANCHINIT)); //66
	_meshCells.push_back(MeshCell(6,0 ,0 ,_n*shift,_n,BRANCHINIT,BRANCHINIT)); //67
	_meshCells.push_back(MeshCell(6,_n,0 ,_n*shift,_n,BRANCHINIT,BRANCHINIT)); //68
	_meshCells.push_back(MeshCell(6,0 ,0 ,_n*shift,_n,BRANCHINIT,BRANCHINIT)); //69
	_meshCells.push_back(MeshCell(6,_n,0 ,_n*shift,_n,BRANCHINIT,BRANCHINIT)); //70
	_meshCells.push_back(MeshCell(6,0 ,_n,_n*shift,_n,BRANCHINIT,BRANCHINIT)); //71
	_meshCells.push_back(MeshCell(6,0 ,_n,_n*shift,_n,BRANCHINIT,BRANCHINIT)); //72
	_meshCells.push_back(MeshCell(6,_n,_n,_n*shift,_n,BRANCHINIT,BRANCHINIT)); //73

	_meshCellIndicesBranch[index+0+4*shift].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-8);
	_meshCellIndicesBranch[index+0+4*shift].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-7);
	_meshCellIndicesBranch[index+0+4*shift].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-5);

	_meshCellIndicesBranch[index+1+4*shift].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-7);
	_meshCellIndicesBranch[index+1+4*shift].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-6);
	_meshCellIndicesBranch[index+1+4*shift].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-4);

	_meshCellIndicesBranch[index+2+4*shift].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-5);
	_meshCellIndicesBranch[index+2+4*shift].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-3);
	_meshCellIndicesBranch[index+2+4*shift].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-2);

	_meshCellIndicesBranch[index+3+4*shift].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-4);
	_meshCellIndicesBranch[index+3+4*shift].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-2);
	_meshCellIndicesBranch[index+3+4*shift].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-1);

	newboundary.add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-8);
	newboundary.add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-6);
	newboundary.add(VECTYPE_EDGEEDGE,10,_meshCells.size()-3);
	newboundary.add(VECTYPE_EDGEEDGE,11,_meshCells.size()-1);

	newboundary.add(VECTYPE_WALLEDGE,2,_meshCells.size()-7);
	newboundary.add(VECTYPE_WALLEDGE,0,_meshCells.size()-5);
	newboundary.add(VECTYPE_WALLEDGE,1,_meshCells.size()-4);
	newboundary.add(VECTYPE_WALLEDGE,3,_meshCells.size()-2);

	_meshCells[_meshCells.size()-8].lastBranch[3] = index+0+4*shift;
	_meshCells[_meshCells.size()-7].lastBranch[2] = index+0+4*shift;
	_meshCells[_meshCells.size()-7].lastBranch[3] = index+1+4*shift;
	_meshCells[_meshCells.size()-6].lastBranch[2] = index+1+4*shift;
	_meshCells[_meshCells.size()-5].lastBranch[1] = index+0+4*shift;
	_meshCells[_meshCells.size()-5].lastBranch[3] = index+2+4*shift;
	_meshCells[_meshCells.size()-4].lastBranch[0] = index+1+4*shift;
	_meshCells[_meshCells.size()-4].lastBranch[2] = index+3+4*shift;
	_meshCells[_meshCells.size()-3].lastBranch[1] = index+2+4*shift;
	_meshCells[_meshCells.size()-2].lastBranch[0] = index+2+4*shift;
	_meshCells[_meshCells.size()-2].lastBranch[1] = index+3+4*shift;
	_meshCells[_meshCells.size()-1].lastBranch[0] = index+3+4*shift;

	// Y
	// 4---3---2            2---+---+
	// |   |   |            |   | x |
	// +---+---1            1---+---+
	// | x |   |            |   |   |
	// +---+---0 X          0---3---4
	_meshCells.push_back(MeshCell(6,_n*((co&1)==0),0             ,_n*((co&4)!=0),_n,BRANCHINIT,BRANCHINIT)); //74
	_meshCells.push_back(MeshCell(6,_n*((co&1)==0),0             ,_n*((co&4)!=0),_n,BRANCHINIT,BRANCHINIT)); //75
	_meshCells.push_back(MeshCell(6,_n*((co&1)==0),_n            ,_n*((co&4)!=0),_n,BRANCHINIT,BRANCHINIT)); //76
	_meshCells.push_back(MeshCell(6,0             ,_n*((co&2)==0),_n*((co&4)!=0),_n,BRANCHINIT,BRANCHINIT)); //77
	_meshCells.push_back(MeshCell(6,_n*((co&1)!=0),_n*((co&2)==0),_n*((co&4)!=0),_n,BRANCHINIT,BRANCHINIT)); //78

	_meshCells.push_back(MeshCell(7,_n*((co&1)==0),0             ,0,_n,BRANCHINIT,BRANCHINIT)); //74
	_meshCells.push_back(MeshCell(7,_n*((co&1)==0),0             ,0,_n,BRANCHINIT,BRANCHINIT)); //75
	_meshCells.push_back(MeshCell(7,_n*((co&1)==0),_n            ,0,_n,BRANCHINIT,BRANCHINIT)); //76
	_meshCells.push_back(MeshCell(7,0             ,_n*((co&2)==0),0,_n,BRANCHINIT,BRANCHINIT)); //77
	_meshCells.push_back(MeshCell(7,_n*((co&1)!=0),_n*((co&2)==0),0,_n,BRANCHINIT,BRANCHINIT)); //78

	_meshCellIndicesBranch[index+(co&4)+  ((co&1)==0)  ].add(VECTYPE_EDGEEDGE, 8+((co&1)==0),_meshCells.size()-10);
	_meshCellIndicesBranch[index+(co&4)+  ((co&1)==0)  ].add(VECTYPE_EDGEEDGE,10+((co&1)==0),_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&4)+  ((co&1)==0)+2].add(VECTYPE_EDGEEDGE, 8+((co&1)==0),_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&4)+  ((co&1)==0)+2].add(VECTYPE_EDGEEDGE,10+((co&1)==0),_meshCells.size()-8);
	_meshCellIndicesBranch[index+(co&4)+2*((co&2)==0)+0].add(VECTYPE_EDGEEDGE, 8+2*((co&2)==0)+1,_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&4)+2*((co&2)==0)+1].add(VECTYPE_EDGEEDGE, 8+2*((co&2)==0)+0,_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&4)+  ((co&1)!=0)+2*((co&2)==0)].add(VECTYPE_EDGEEDGE,8+((co&1)!=0)+2*((co&2)==0),_meshCells.size()-6);

	_meshCellIndicesBranch[index+0+  ((co&1)==0)  ].indexCorner[4+((co&1)==0)] = _meshCells.size()-5;
	_meshCellIndicesBranch[index+4+  ((co&1)==0)  ].indexCorner[0+((co&1)==0)] = _meshCells.size()-5;
	_meshCellIndicesBranch[index+0+  ((co&1)==0)  ].indexCorner[6+((co&1)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+4+  ((co&1)==0)  ].indexCorner[2+((co&1)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+0+  ((co&1)==0)+2].indexCorner[4+((co&1)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+4+  ((co&1)==0)+2].indexCorner[0+((co&1)==0)] = _meshCells.size()-4;
	_meshCellIndicesBranch[index+0+  ((co&1)==0)+2].indexCorner[6+((co&1)==0)] = _meshCells.size()-3;
	_meshCellIndicesBranch[index+4+  ((co&1)==0)+2].indexCorner[2+((co&1)==0)] = _meshCells.size()-3;
	_meshCellIndicesBranch[index+0+2*((co&2)==0)+0].indexCorner[4+2*((co&2)==0)+1] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+4+2*((co&2)==0)+0].indexCorner[0+2*((co&2)==0)+1] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+0+2*((co&2)==0)+1].indexCorner[4+2*((co&2)==0)+0] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+4+2*((co&2)==0)+1].indexCorner[0+2*((co&2)==0)+0] = _meshCells.size()-2;
	_meshCellIndicesBranch[index+0+  ((co&1)!=0)+2*((co&2)==0)].indexCorner[4+((co&1)!=0)+2*((co&2)==0)] = _meshCells.size()-1;
	_meshCellIndicesBranch[index+4+  ((co&1)!=0)+2*((co&2)==0)].indexCorner[0+((co&1)!=0)+2*((co&2)==0)] = _meshCells.size()-1;

	newboundary.add(VECTYPE_EDGEEDGE, 8+((co&1)==0),_meshCells.size()-10);
	newboundary.add(VECTYPE_EDGEEDGE,10+((co&1)==0),_meshCells.size()-8);
	newboundary.add(VECTYPE_EDGEEDGE, 8+((co&1)!=0)+2*((co&2)==0),_meshCells.size()-6);
	newboundary.add(VECTYPE_WALLEDGE,0+((co&1)==0),_meshCells.size()-9);
	newboundary.add(VECTYPE_WALLEDGE,2+((co&2)==0),_meshCells.size()-7);

	newboundary.add(VECTYPE_EDGECORN, 8+((co&1)==0),_meshCells.size()-5);
	newboundary.add(VECTYPE_EDGECORN,10+((co&1)==0),_meshCells.size()-3);
	newboundary.add(VECTYPE_EDGECORN, 8+((co&1)!=0)+2*((co&2)==0),_meshCells.size()-1);
	newboundary.add(VECTYPE_WALLCORN,0+((co&1)==0),_meshCells.size()-4);
	newboundary.add(VECTYPE_WALLCORN,2+((co&2)==0),_meshCells.size()-2);

	if((co&1)!=0){
		_meshCells[_meshCells.size()-10].lastBranch[3] = index+(co&4)+0;
		_meshCells[_meshCells.size()-5].lastBranch[3] = index+0+0;
		_meshCells[_meshCells.size()-5].lastBranch[7] = index+4+0;
	}
	else{
		_meshCells[_meshCells.size()-10].lastBranch[2] = index+(co&4)+1;
		_meshCells[_meshCells.size()-5].lastBranch[2] = index+0+1;
		_meshCells[_meshCells.size()-5].lastBranch[6] = index+4+1;
	}
	if((co&1)!=0){
		_meshCells[_meshCells.size()-9].lastBranch[1] = index+(co&4)+0;
		_meshCells[_meshCells.size()-9].lastBranch[3] = index+(co&4)+2;
		_meshCells[_meshCells.size()-4].lastBranch[1] = index+0+0;
		_meshCells[_meshCells.size()-4].lastBranch[5] = index+4+0;
		_meshCells[_meshCells.size()-4].lastBranch[3] = index+0+2;
		_meshCells[_meshCells.size()-4].lastBranch[7] = index+4+2;
	}
	else{
		_meshCells[_meshCells.size()-9].lastBranch[0] = index+(co&4)+1;
		_meshCells[_meshCells.size()-9].lastBranch[2] = index+(co&4)+3;
		_meshCells[_meshCells.size()-4].lastBranch[0] = index+0+1;
		_meshCells[_meshCells.size()-4].lastBranch[4] = index+4+1;
		_meshCells[_meshCells.size()-4].lastBranch[2] = index+0+3;
		_meshCells[_meshCells.size()-4].lastBranch[6] = index+4+3;
	}
	if((co&1)!=0){
		_meshCells[_meshCells.size()-8].lastBranch[1] = index+(co&4)+2;
		_meshCells[_meshCells.size()-3].lastBranch[1] = index+0+2;
		_meshCells[_meshCells.size()-3].lastBranch[5] = index+4+2;
	}
	else{
		_meshCells[_meshCells.size()-8].lastBranch[0] = index+(co&4)+3;
		_meshCells[_meshCells.size()-3].lastBranch[0] = index+0+3;
		_meshCells[_meshCells.size()-3].lastBranch[4] = index+4+3;
	}
	if((co&2)!=0){
		_meshCells[_meshCells.size()-7].lastBranch[2] = index+(co&4)+0;
		_meshCells[_meshCells.size()-7].lastBranch[3] = index+(co&4)+1;
		_meshCells[_meshCells.size()-2].lastBranch[2] = index+0+0;
		_meshCells[_meshCells.size()-2].lastBranch[6] = index+4+0;
		_meshCells[_meshCells.size()-2].lastBranch[3] = index+0+1;
		_meshCells[_meshCells.size()-2].lastBranch[7] = index+4+1;
	}
	else{
		_meshCells[_meshCells.size()-7].lastBranch[0] = index+(co&4)+2;
		_meshCells[_meshCells.size()-7].lastBranch[1] = index+(co&4)+3;
		_meshCells[_meshCells.size()-2].lastBranch[0] = index+0+2;
		_meshCells[_meshCells.size()-2].lastBranch[4] = index+4+2;
		_meshCells[_meshCells.size()-2].lastBranch[1] = index+0+3;
		_meshCells[_meshCells.size()-2].lastBranch[5] = index+4+3;
	}
	switch(co&3){
	case 0:
	_meshCells[_meshCells.size()-6].lastBranch[1] = index+(co&4)+2;
	_meshCells[_meshCells.size()-1].lastBranch[1] = index+0+2;
	_meshCells[_meshCells.size()-1].lastBranch[5] = index+4+2;
	break;
	case 1:
	_meshCells[_meshCells.size()-6].lastBranch[0] = index+(co&4)+3;
	_meshCells[_meshCells.size()-1].lastBranch[0] = index+0+3;
	_meshCells[_meshCells.size()-1].lastBranch[4] = index+4+3;
	break;
	case 2:
	_meshCells[_meshCells.size()-6].lastBranch[3] = index+(co&4)+0;
	_meshCells[_meshCells.size()-1].lastBranch[3] = index+0+0;
	_meshCells[_meshCells.size()-1].lastBranch[7] = index+4+0;
	break;
	case 3:
	_meshCells[_meshCells.size()-6].lastBranch[2] = index+(co&4)+1;
	_meshCells[_meshCells.size()-1].lastBranch[2] = index+0+1;
	_meshCells[_meshCells.size()-1].lastBranch[6] = index+4+1;
	break;
	}

	//The 7 new Corners
	if(co!=0){
		_meshCells.push_back(MeshCell(7,0 ,0 ,0 ,_n,BRANCHINIT,BRANCHINIT));
		_meshCellIndicesBranch[index+0].indexCorner[0] = _meshCells.size()-1;
		newboundary.indexCorner[0] = _meshCells.size()-1;
		_meshCells.back().lastBranch[7] = index+0;
	}
	if(co!=1){
		_meshCells.push_back(MeshCell(7,_n,0 ,0 ,_n,BRANCHINIT,BRANCHINIT));
		_meshCellIndicesBranch[index+1].indexCorner[1] = _meshCells.size()-1;
		newboundary.indexCorner[1] = _meshCells.size()-1;
		_meshCells.back().lastBranch[6] = index+1;
	}
	if(co!=2){
		_meshCells.push_back(MeshCell(7,0 ,_n,0 ,_n,BRANCHINIT,BRANCHINIT));
		_meshCellIndicesBranch[index+2].indexCorner[2] = _meshCells.size()-1;
		newboundary.indexCorner[2] = _meshCells.size()-1;
		_meshCells.back().lastBranch[5] = index+2;
	}
	if(co!=3){
		_meshCells.push_back(MeshCell(7,_n,_n,0 ,_n,BRANCHINIT,BRANCHINIT));
		_meshCellIndicesBranch[index+3].indexCorner[3] = _meshCells.size()-1;
		newboundary.indexCorner[3] = _meshCells.size()-1;
		_meshCells.back().lastBranch[4] = index+3;
	}
	if(co!=4){
		_meshCells.push_back(MeshCell(7,0 ,0 ,_n,_n,BRANCHINIT,BRANCHINIT));
		_meshCellIndicesBranch[index+4].indexCorner[4] = _meshCells.size()-1;
		newboundary.indexCorner[4] = _meshCells.size()-1;
		_meshCells.back().lastBranch[3] = index+4;
	}
	if(co!=5){
		_meshCells.push_back(MeshCell(7,_n,0 ,_n,_n,BRANCHINIT,BRANCHINIT));
		_meshCellIndicesBranch[index+5].indexCorner[5] = _meshCells.size()-1;
		newboundary.indexCorner[5] = _meshCells.size()-1;
		_meshCells.back().lastBranch[2] = index+5;
	}
	if(co!=6){
		_meshCells.push_back(MeshCell(7,0 ,_n,_n,_n,BRANCHINIT,BRANCHINIT));
		_meshCellIndicesBranch[index+6].indexCorner[6] = _meshCells.size()-1;
		newboundary.indexCorner[6] = _meshCells.size()-1;
		_meshCells.back().lastBranch[1] = index+6;
	}
	if(co!=7){
		_meshCells.push_back(MeshCell(7,_n,_n,_n,_n,BRANCHINIT,BRANCHINIT));
		_meshCellIndicesBranch[index+7].indexCorner[7] = _meshCells.size()-1;
		newboundary.indexCorner[7] = _meshCells.size()-1;
		_meshCells.back().lastBranch[0] = index+7;
	}


  fprintf(stderr,"\nAdapting Inner Boundary");

	//Inner Cells with the old boundary
	//Wall X
	{
//		  fprintf(stderr,"\nWall X");
  	size_t sourceIndex = 0+((co&1)==0);
  	size_t targetIndex = 0+((co&1)!=0);
  	size_t branchIndex = index+(co&6)+((co&1)==0);

  	for(size_t i=0;i<_boundary.border_size();i++){
  		MCNCompact::CellReferenceNum ref = _boundary.get(i);
  		size_t cellIndex = ref.second;
  		MeshCell &cell = _meshCells[cellIndex];
  		MCNCompact::vecpair identifier = MCNCompact::decnum(ref.first);
  		if(identifier.vecindex == sourceIndex){
  			if(identifier.vectype == VECTYPE_WALLWALL){
  				_meshCellIndicesBranch[branchIndex].add(identifier.vectype,targetIndex,cellIndex);
  				if((co&1)!=0){
  					cell.lastBranch[0] = index+(co&6)+0;
  				}
  				else{
  					cell.lastBranch[1] = index+(co&6)+1;
  				}
  			}
  			if(identifier.vectype == VECTYPE_WALLEDGE){
  				_meshCellIndicesBranch[branchIndex].add(identifier.vectype,targetIndex,cellIndex);
  				if((co&1)!=0){
  					cell.lastBranch[0] =
  					cell.lastBranch[2] = index+(co&6)+0;
  				}
  				else{
  					cell.lastBranch[1] =
  					cell.lastBranch[3] = index+(co&6)+1;
  				}
  			}
  			if(identifier.vectype == VECTYPE_WALLCORN){
  				_meshCellIndicesBranch[branchIndex].add(identifier.vectype,targetIndex,cellIndex);
  				if((co&1)!=0){
  					cell.lastBranch[0] =
  					cell.lastBranch[2] =
  					cell.lastBranch[4] =
  					cell.lastBranch[6] = index+(co&6)+0;
  				}
  				else{
  					cell.lastBranch[1] =
  					cell.lastBranch[3] =
  					cell.lastBranch[5] =
  					cell.lastBranch[7] = index+(co&6)+1;
  				}
  			}
  		}
  	}
//  	//Taking out the old indices
//  	MCNCompact::cellvector temp;
//  	for(size_t i=0;i<_boundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(_boundary.cells[i].second);
//  		if(!(identifier.vecindex == sourceIndex &&
//  				(identifier.vectype==VECTYPE_WALLWALL ||
//  			   identifier.vectype==VECTYPE_WALLEDGE ||
//  				 identifier.vectype==VECTYPE_WALLCORN))){
//  			temp.push_back(_boundary.cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
//  		}
//  	}
//  	_boundary.cells = temp;
//  	//Putting in the new indices
//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if(identifier.vecindex == sourceIndex &&
//  				(identifier.vectype==VECTYPE_WALLWALL ||
//  			   identifier.vectype==VECTYPE_WALLEDGE ||
//  				 identifier.vectype==VECTYPE_WALLCORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
  	_boundary.overwriteWall(newboundary,sourceIndex);
	}

	//Wall Y
	{
//		  fprintf(stderr,"\nWall Y");
  	size_t sourceIndex = 2+((co&2)==0);
  	size_t targetIndex = 2+((co&2)!=0);
  	size_t branchIndex = index+(co&5)+2*((co&2)==0);

  	for(size_t i=0;i<_boundary.border_size();i++){
  		MCNCompact::CellReferenceNum ref = _boundary.get(i);
  		size_t cellIndex = ref.second;
  		MeshCell &cell = _meshCells[cellIndex];
  		MCNCompact::vecpair identifier = MCNCompact::decnum(ref.first);
  		if(identifier.vecindex == sourceIndex){
  			if(identifier.vectype == VECTYPE_WALLWALL){
  				_meshCellIndicesBranch[branchIndex].add(identifier.vectype,targetIndex,cellIndex);
  				if((co&2)!=0){
  					cell.lastBranch[0] = index+(co&5)+0;
  				}
  				else{
  					cell.lastBranch[1] = index+(co&5)+2;
  				}
  			}
  			if(identifier.vectype == VECTYPE_WALLEDGE){
  				_meshCellIndicesBranch[branchIndex].add(identifier.vectype,targetIndex,cellIndex);
  				if((co&2)!=0) {
  					if(cell.type==4){
  						cell.lastBranch[0] =
  						cell.lastBranch[2] = index+(co&5)+0;
  					}
  					else{
  						cell.lastBranch[0] =
  						cell.lastBranch[1] = index+(co&5)+0;
  					}
  				}
  				else{
  					if(cell.type==4){
  						cell.lastBranch[1] =
  						cell.lastBranch[3] = index+(co&5)+2;
  					}
  					else{
  						cell.lastBranch[2] =
  						cell.lastBranch[3] = index+(co&5)+2;
  					}
  				}
  			}
  			if(identifier.vectype == VECTYPE_WALLCORN){
  				_meshCellIndicesBranch[branchIndex].add(identifier.vectype,targetIndex,cellIndex);
  				if((co&2)!=0) {
  					cell.lastBranch[0] =
  					cell.lastBranch[2] =
  					cell.lastBranch[4] =
  					cell.lastBranch[6] = index+(co&5)+0;
  				}
  				else{
  					cell.lastBranch[1] =
  					cell.lastBranch[3] =
  					cell.lastBranch[5] =
  					cell.lastBranch[7] = index+(co&5)+2;
  				}
  			}
  		}
  	}
//  	//Taking out the old indices
//  	MCNCompact::cellvector temp;
//  	for(size_t i=0;i<_boundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(_boundary.cells[i].second);
//  		if(!(identifier.vecindex == sourceIndex &&
//  				(identifier.vectype==VECTYPE_WALLWALL ||
//  			   identifier.vectype==VECTYPE_WALLEDGE ||
//  				 identifier.vectype==VECTYPE_WALLCORN))){
//  			temp.push_back(_boundary.cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
//  		}
//  	}
//  	_boundary.cells = temp;
//  	//Putting in the new indices
//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if(identifier.vecindex == sourceIndex &&
//  				(identifier.vectype==VECTYPE_WALLWALL ||
//  			   identifier.vectype==VECTYPE_WALLEDGE ||
//  				 identifier.vectype==VECTYPE_WALLCORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
  	_boundary.overwriteWall(newboundary,sourceIndex);
	}

	//Wall Z
	{
//		  fprintf(stderr,"\nWall Z");
  	size_t sourceIndex = 4+((co&4)==0);
  	size_t targetIndex = 4+((co&4)!=0);
  	size_t branchIndex = index+(co&3)+4*((co&4)==0);

  	for(size_t i=0;i<_boundary.border_size();i++){
  		MCNCompact::CellReferenceNum ref = _boundary.get(i);
  		size_t cellIndex = ref.second;
  		MeshCell &cell = _meshCells[cellIndex];
  		MCNCompact::vecpair identifier = MCNCompact::decnum(ref.first);
  		if(identifier.vecindex == sourceIndex){
  			if(identifier.vectype == VECTYPE_WALLWALL){
  				_meshCellIndicesBranch[branchIndex].add(identifier.vectype,targetIndex,cellIndex);
  				if((co&4)!=0){
  					cell.lastBranch[0] = index+(co&3)+0;
  				}
  				else{
  					cell.lastBranch[1] = index+(co&3)+4;
  				}
  			}
  			if(identifier.vectype == VECTYPE_WALLEDGE){
  				_meshCellIndicesBranch[branchIndex].add(identifier.vectype,targetIndex,cellIndex);
  				if((co&4)!=0){
  					cell.lastBranch[0] =
  					cell.lastBranch[1] = index+(co&3)+0;
  				}
  				else{
  					cell.lastBranch[2] =
  					cell.lastBranch[3] = index+(co&3)+4;
  				}
  			}
  			if(identifier.vectype == VECTYPE_WALLCORN){
  				_meshCellIndicesBranch[branchIndex].add(identifier.vectype,targetIndex,cellIndex);
  				if((co&4)!=0){
  					cell.lastBranch[0] =
  					cell.lastBranch[1] =
  					cell.lastBranch[2] =
  					cell.lastBranch[3] = index+(co&3)+0;
  				}
  				else{
  					cell.lastBranch[4] =
  					cell.lastBranch[5] =
  					cell.lastBranch[6] =
  					cell.lastBranch[7] = index+(co&3)+4;
  				}
  			}
  		}
  	}
//  	//Taking out the old indices
//  	MCNCompact::cellvector temp;
//  	for(size_t i=0;i<_boundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(_boundary.cells[i].second);
//  		if(!(identifier.vecindex == sourceIndex &&
//  				(identifier.vectype==VECTYPE_WALLWALL ||
//  			   identifier.vectype==VECTYPE_WALLEDGE ||
//  				 identifier.vectype==VECTYPE_WALLCORN))){
//  			temp.push_back(_boundary.cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
//  		}
//  	}
//  	_boundary.cells = temp;
//  	//Putting in the new indices
//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if(identifier.vecindex == sourceIndex &&
//  				(identifier.vectype==VECTYPE_WALLWALL ||
//  			   identifier.vectype==VECTYPE_WALLEDGE ||
//  				 identifier.vectype==VECTYPE_WALLCORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
  	_boundary.overwriteWall(newboundary,sourceIndex);
	}

	//Edge X
	{
//		  fprintf(stderr,"\nEdge X");
  	size_t sourceIndex  = 0+((co&2)==0)+2*((co&4)==0);
  	size_t targetIndex0 = 0+((co&2)!=0)+2*((co&4)==0);
  	size_t targetIndex1 = 0+((co&2)==0)+2*((co&4)!=0);
  	size_t targetIndex2 = 0+((co&2)!=0)+2*((co&4)!=0);
  	size_t branchIndex0 = index+((co&1)!=0)+2*((co&2)==0)+4*((co&4)!=0);
  	size_t branchIndex1 = index+((co&1)!=0)+2*((co&2)!=0)+4*((co&4)==0);
  	size_t branchIndex2 = index+((co&1)!=0)+2*((co&2)==0)+4*((co&4)==0);

  	for(size_t i=0;i<_boundary.border_size();i++){
  		MCNCompact::CellReferenceNum ref = _boundary.get(i);
  		size_t cellIndex = ref.second;
  		MeshCell &cell = _meshCells[cellIndex];
  		MCNCompact::vecpair identifier = MCNCompact::decnum(ref.first);
  		if(identifier.vecindex == sourceIndex){
  			if(identifier.vectype == VECTYPE_EDGEEDGE){
  				_meshCellIndicesBranch[branchIndex0].add(identifier.vectype,targetIndex0,cellIndex);
  				_meshCellIndicesBranch[branchIndex1].add(identifier.vectype,targetIndex1,cellIndex);
  				_meshCellIndicesBranch[branchIndex2].add(identifier.vectype,targetIndex2,cellIndex);
  				if((co&6)!=0) cell.lastBranch[0] = index+(co&1)+0;
  				if((co&6)!=2) cell.lastBranch[1] = index+(co&1)+2;
  				if((co&6)!=4) cell.lastBranch[2] = index+(co&1)+4;
  				if((co&6)!=6) cell.lastBranch[3] = index+(co&1)+6;
  			}
  			if(identifier.vectype == VECTYPE_EDGECORN){
  				_meshCellIndicesBranch[branchIndex0].add(identifier.vectype,targetIndex0,cellIndex);
  				_meshCellIndicesBranch[branchIndex1].add(identifier.vectype,targetIndex1,cellIndex);
  				_meshCellIndicesBranch[branchIndex2].add(identifier.vectype,targetIndex2,cellIndex);
  				if((co&6)!=0) cell.lastBranch[0] = cell.lastBranch[1] = index+(co&1)+0;
  				if((co&6)!=2) cell.lastBranch[2] = cell.lastBranch[3] = index+(co&1)+2;
  				if((co&6)!=4) cell.lastBranch[4] = cell.lastBranch[5] = index+(co&1)+4;
  				if((co&6)!=6) cell.lastBranch[6] = cell.lastBranch[7] = index+(co&1)+6;
  			}
  		}
  	}
//  	//Taking out the old indices
//  	MCNCompact::cellvector temp;
//  	for(size_t i=0;i<_boundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(_boundary.cells[i].second);
//  		if(!(identifier.vecindex == sourceIndex &&
//  		  	(identifier.vectype==VECTYPE_EDGEEDGE ||
//  		  	 identifier.vectype==VECTYPE_EDGECORN))){
//  			temp.push_back(_boundary.cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
//  		}
//  	}
//  	_boundary.cells = temp;
//  	//Putting in the new indices
//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if(identifier.vecindex == sourceIndex &&
//  				(identifier.vectype==VECTYPE_EDGEEDGE ||
//  			   identifier.vectype==VECTYPE_EDGECORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
  	_boundary.overwriteEdge(newboundary,sourceIndex);
	}

	//Edge Y
	{
//		  fprintf(stderr,"\nEdge Y");
  	size_t sourceIndex  = 4+((co&1)==0)+2*((co&4)==0);
  	size_t targetIndex0 = 4+((co&1)!=0)+2*((co&4)==0);
  	size_t targetIndex1 = 4+((co&1)==0)+2*((co&4)!=0);
  	size_t targetIndex2 = 4+((co&1)!=0)+2*((co&4)!=0);
  	size_t branchIndex0 = index+2*((co&2)!=0)+((co&1)==0)+4*((co&4)!=0);
  	size_t branchIndex1 = index+2*((co&2)!=0)+((co&1)!=0)+4*((co&4)==0);
  	size_t branchIndex2 = index+2*((co&2)!=0)+((co&1)==0)+4*((co&4)==0);

  	for(size_t i=0;i<_boundary.border_size();i++){
  		MCNCompact::CellReferenceNum ref = _boundary.get(i);
  		size_t cellIndex = ref.second;
  		MeshCell &cell = _meshCells[cellIndex];
  		MCNCompact::vecpair identifier = MCNCompact::decnum(ref.first);
  		if(identifier.vecindex == sourceIndex){
  			if(identifier.vectype == VECTYPE_EDGEEDGE){
  				_meshCellIndicesBranch[branchIndex0].add(identifier.vectype,targetIndex0,cellIndex);
  				_meshCellIndicesBranch[branchIndex1].add(identifier.vectype,targetIndex1,cellIndex);
  				_meshCellIndicesBranch[branchIndex2].add(identifier.vectype,targetIndex2,cellIndex);
  				if((co&5)!=0) cell.lastBranch[0] = index+(co&2)+0;
  				if((co&5)!=1) cell.lastBranch[1] = index+(co&2)+1;
  				if((co&5)!=4) cell.lastBranch[2] = index+(co&2)+4;
  				if((co&5)!=5) cell.lastBranch[3] = index+(co&2)+5;
  			}
  			if(identifier.vectype == VECTYPE_EDGECORN){
  				_meshCellIndicesBranch[branchIndex0].add(identifier.vectype,targetIndex0,cellIndex);
  				_meshCellIndicesBranch[branchIndex1].add(identifier.vectype,targetIndex1,cellIndex);
  				_meshCellIndicesBranch[branchIndex2].add(identifier.vectype,targetIndex2,cellIndex);
  				if((co&5)!=0) cell.lastBranch[0] = cell.lastBranch[2] = index+(co&2)+0;
  				if((co&5)!=1) cell.lastBranch[1] = cell.lastBranch[3] = index+(co&2)+1;
  				if((co&5)!=4) cell.lastBranch[4] = cell.lastBranch[6] = index+(co&2)+4;
  				if((co&5)!=5) cell.lastBranch[5] = cell.lastBranch[7] = index+(co&2)+5;
  			}
  		}
  	}
//  	//Taking out the old indices
//  	MCNCompact::cellvector temp;
//  	for(size_t i=0;i<_boundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(_boundary.cells[i].second);
//  		if(!(identifier.vecindex == sourceIndex &&
//  		  	(identifier.vectype==VECTYPE_EDGEEDGE ||
//  		  	 identifier.vectype==VECTYPE_EDGECORN))){
//  			temp.push_back(_boundary.cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
//  		}
//  	}
//  	_boundary.cells = temp;
//  	//Putting in the new indices
//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if(identifier.vecindex == sourceIndex &&
//  				(identifier.vectype==VECTYPE_EDGEEDGE ||
//  			   identifier.vectype==VECTYPE_EDGECORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
  	_boundary.overwriteEdge(newboundary,sourceIndex);
	}

	//Edge Z
	{
//		  fprintf(stderr,"\nEdge Z");
  	size_t sourceIndex  = 8+((co&1)==0)+2*((co&2)==0);
  	size_t targetIndex0 = 8+((co&1)!=0)+2*((co&2)==0);
  	size_t targetIndex1 = 8+((co&1)==0)+2*((co&2)!=0);
  	size_t targetIndex2 = 8+((co&1)!=0)+2*((co&2)!=0);
  	size_t branchIndex0 = index+4*((co&4)!=0)+((co&1)==0)+2*((co&2)!=0);
  	size_t branchIndex1 = index+4*((co&4)!=0)+((co&1)!=0)+2*((co&2)==0);
  	size_t branchIndex2 = index+4*((co&4)!=0)+((co&1)==0)+2*((co&2)==0);

  	for(size_t i=0;i<_boundary.border_size();i++){
  		MCNCompact::CellReferenceNum ref = _boundary.get(i);
  		size_t cellIndex = ref.second;
  		MeshCell &cell = _meshCells[cellIndex];
  		MCNCompact::vecpair identifier = MCNCompact::decnum(ref.first);
  		if(identifier.vecindex == sourceIndex){
  			if(identifier.vectype == VECTYPE_EDGEEDGE){
  				_meshCellIndicesBranch[branchIndex0].add(identifier.vectype,targetIndex0,cellIndex);
  				_meshCellIndicesBranch[branchIndex1].add(identifier.vectype,targetIndex1,cellIndex);
  				_meshCellIndicesBranch[branchIndex2].add(identifier.vectype,targetIndex2,cellIndex);
  				if((co&3)!=0) cell.lastBranch[0] = index+(co&4)+0;
  				if((co&3)!=1) cell.lastBranch[1] = index+(co&4)+1;
  				if((co&3)!=2) cell.lastBranch[2] = index+(co&4)+2;
  				if((co&3)!=3) cell.lastBranch[3] = index+(co&4)+3;
  			}
  			if(identifier.vectype == VECTYPE_EDGECORN){
  				_meshCellIndicesBranch[branchIndex0].add(identifier.vectype,targetIndex0,cellIndex);
  				_meshCellIndicesBranch[branchIndex1].add(identifier.vectype,targetIndex1,cellIndex);
  				_meshCellIndicesBranch[branchIndex2].add(identifier.vectype,targetIndex2,cellIndex);
  				if((co&3)!=0) cell.lastBranch[0] = cell.lastBranch[4] = index+(co&4)+0;
  				if((co&3)!=1) cell.lastBranch[1] = cell.lastBranch[5] = index+(co&4)+1;
  				if((co&3)!=2) cell.lastBranch[2] = cell.lastBranch[6] = index+(co&4)+2;
  				if((co&3)!=3) cell.lastBranch[3] = cell.lastBranch[7] = index+(co&4)+3;
  			}
  		}
  	}
//  	//Taking out the old indices
//  	MCNCompact::cellvector temp;
//  	for(size_t i=0;i<_boundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(_boundary.cells[i].second);
//  		if(!(identifier.vecindex == sourceIndex &&
//  		  	(identifier.vectype==VECTYPE_EDGEEDGE ||
//  		  	 identifier.vectype==VECTYPE_EDGECORN))){
//  			temp.push_back(_boundary.cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
//  		}
//  	}
//  	_boundary.cells = temp;
//  	//Putting in the new indices
//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if(identifier.vecindex == sourceIndex &&
//  				(identifier.vectype==VECTYPE_EDGEEDGE ||
//  			   identifier.vectype==VECTYPE_EDGECORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
  	_boundary.overwriteEdge(newboundary,sourceIndex);
	}

	//Middle
	{
	  fprintf(stderr,"\nMiddle");
		MeshCell &cell = _meshCells[_boundary.indexCorner[7-co]];
		if(co!=0){
			_meshCellIndicesBranch[index+0].indexCorner[7] = _boundary.indexCorner[7-co];
			cell.lastBranch[0] = index+0;
		}
		if(co!=1){
			_meshCellIndicesBranch[index+1].indexCorner[6] = _boundary.indexCorner[7-co];
			cell.lastBranch[1] = index+1;
		}
		if(co!=2){
			_meshCellIndicesBranch[index+2].indexCorner[5] = _boundary.indexCorner[7-co];
			cell.lastBranch[2] = index+2;
		}
		if(co!=3){
			_meshCellIndicesBranch[index+3].indexCorner[4] = _boundary.indexCorner[7-co];
			cell.lastBranch[3] = index+3;
		}
		if(co!=4){
			_meshCellIndicesBranch[index+4].indexCorner[3] = _boundary.indexCorner[7-co];
			cell.lastBranch[4] = index+4;
		}
		if(co!=5){
			_meshCellIndicesBranch[index+5].indexCorner[2] = _boundary.indexCorner[7-co];
			cell.lastBranch[5] = index+5;
		}
		if(co!=6){
			_meshCellIndicesBranch[index+6].indexCorner[1] = _boundary.indexCorner[7-co];
			cell.lastBranch[6] = index+6;
		}
		if(co!=7){
			_meshCellIndicesBranch[index+7].indexCorner[0] = _boundary.indexCorner[7-co];
			cell.lastBranch[7] = index+7;
		}
		_boundary.indexCorner[7-co] = newboundary.indexCorner[7-co];
	}

	  fprintf(stderr,"\nAdapting Outer Boundary");
	//The remaining boundary gets bigger and the edges need to be included
	//in the new volume
	//Wall X
	{
		int wallIndex  = 0+((co&1)!=0);
		int edgeIndexY = 4+((co&1)!=0)+2*((co&4)==0);
		int edgeIndexZ = 8+((co&1)!=0)+2*((co&2)==0);
		int cornerIndex = ((co&1)!=0)+2*((co&2)==0)+4*((co&4)==0);

//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if(identifier.vecindex == wallIndex &&
//  				(identifier.vectype==VECTYPE_WALLWALL ||
//  			   identifier.vectype==VECTYPE_WALLEDGE ||
//  				 identifier.vectype==VECTYPE_WALLCORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
		_boundary.addWall(newboundary,wallIndex);
  	for(size_t i=0;i<_boundary.border_size();i++){
  		MCNCompact::CellReferenceNum ref = _boundary.get(i);
  		size_t cellIndex = ref.second;
  		MCNCompact::vecpair identifier = MCNCompact::decnum(ref.first);
  		if(identifier.vecindex == edgeIndexY){
  			if(identifier.vectype==VECTYPE_EDGEEDGE){
    			_boundary.add(VECTYPE_WALLEDGE,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&3)+4*((co&4)==0)].add(VECTYPE_EDGEEDGE,4+((co&1)!=0)+2*((co&4)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[3] = index+4; break;
    			case 1: cell.lastBranch[2] = index+5; break;
    			case 2: cell.lastBranch[3] = index+6; break;
    			case 3: cell.lastBranch[2] = index+7; break;
    			case 4: cell.lastBranch[1] = index+0; break;
    			case 5: cell.lastBranch[0] = index+1; break;
    			case 6: cell.lastBranch[1] = index+2; break;
    			case 7: cell.lastBranch[0] = index+3; break;
    			}
  			}
  			if(identifier.vectype==VECTYPE_EDGECORN){
    			_boundary.add(VECTYPE_WALLCORN,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&3)+4*((co&4)==0)].add(VECTYPE_EDGECORN,4+((co&1)!=0)+2*((co&4)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[5] = cell.lastBranch[7] = index+4; break;
    			case 1: cell.lastBranch[4] = cell.lastBranch[6] = index+5; break;
    			case 2: cell.lastBranch[5] = cell.lastBranch[7] = index+6; break;
    			case 3: cell.lastBranch[4] = cell.lastBranch[6] = index+7; break;
    			case 4: cell.lastBranch[1] = cell.lastBranch[3] = index+0; break;
    			case 5: cell.lastBranch[0] = cell.lastBranch[2] = index+1; break;
    			case 6: cell.lastBranch[1] = cell.lastBranch[3] = index+2; break;
    			case 7: cell.lastBranch[0] = cell.lastBranch[2] = index+3; break;
    			}
  			}
  		}
  		if(identifier.vecindex == edgeIndexZ){
  			if(identifier.vectype==VECTYPE_EDGEEDGE){
    			_boundary.add(VECTYPE_WALLEDGE,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&5)+2*((co&2)==0)].add(VECTYPE_EDGEEDGE,8+((co&1)!=0)+2*((co&2)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[3] = index+2; break;
    			case 1: cell.lastBranch[2] = index+3; break;
    			case 2: cell.lastBranch[1] = index+0; break;
    			case 3: cell.lastBranch[0] = index+1; break;
    			case 4: cell.lastBranch[3] = index+6; break;
    			case 5: cell.lastBranch[2] = index+7; break;
    			case 6: cell.lastBranch[1] = index+4; break;
    			case 7: cell.lastBranch[0] = index+5; break;
    			}
  			}
  			if(identifier.vectype==VECTYPE_EDGECORN){
    			_boundary.add(VECTYPE_WALLCORN,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&5)+2*((co&2)==0)].add(VECTYPE_EDGECORN,8+((co&1)!=0)+2*((co&2)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[3] = cell.lastBranch[7] = index+2; break;
    			case 1: cell.lastBranch[2] = cell.lastBranch[6] = index+3; break;
    			case 2: cell.lastBranch[1] = cell.lastBranch[5] = index+0; break;
    			case 3: cell.lastBranch[0] = cell.lastBranch[4] = index+1; break;
    			case 4: cell.lastBranch[3] = cell.lastBranch[7] = index+6; break;
    			case 5: cell.lastBranch[2] = cell.lastBranch[6] = index+7; break;
    			case 6: cell.lastBranch[1] = cell.lastBranch[5] = index+4; break;
    			case 7: cell.lastBranch[0] = cell.lastBranch[4] = index+5; break;
    			}
  			}
  		}
  	}

		{
			_boundary.add(VECTYPE_WALLCORN,wallIndex,_boundary.indexCorner[cornerIndex]);
			MeshCell & cell = _meshCells[_boundary.indexCorner[cornerIndex]];
			if((co&6)!=0){
				_meshCellIndicesBranch[index+0+((co&1)==0)].indexCorner[6+(co&1)] = _boundary.indexCorner[cornerIndex];
				if(co&1) cell.lastBranch[1] = index+0+0;
				else     cell.lastBranch[0] = index+0+1;
			}
			if((co&6)!=2){
				_meshCellIndicesBranch[index+2+((co&1)==0)].indexCorner[4+(co&1)] = _boundary.indexCorner[cornerIndex];
				if(co&1) cell.lastBranch[3] = index+2+0;
				else     cell.lastBranch[2] = index+2+1;
			}
			if((co&6)!=4){
				_meshCellIndicesBranch[index+4+((co&1)==0)].indexCorner[2+(co&1)] = _boundary.indexCorner[cornerIndex];
				if(co&1) cell.lastBranch[5] = index+4+0;
				else     cell.lastBranch[4] = index+4+1;
			}
			if((co&6)!=6){
				_meshCellIndicesBranch[index+6+((co&1)==0)].indexCorner[0+(co&1)] = _boundary.indexCorner[cornerIndex];
				if(co&1) cell.lastBranch[7] = index+6+0;
				else     cell.lastBranch[6] = index+6+1;
			}
		}

//  	//Taking out the old indices
//  	MCNCompact::cellvector temp;
//  	for(size_t i=0;i<_boundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(_boundary.cells[i].second);
//  		if(!((identifier.vecindex == edgeIndexY || identifier.vecindex == edgeIndexZ) &&
//  		  	(identifier.vectype==VECTYPE_EDGEEDGE ||
//  		  	 identifier.vectype==VECTYPE_EDGECORN))){
//  			temp.push_back(_boundary.cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
//  		}
//  	}
//  	_boundary.cells = temp;
//  	//Putting in the new indices
//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if((identifier.vecindex == edgeIndexY || identifier.vecindex == edgeIndexZ) &&
//  				(identifier.vectype==VECTYPE_EDGEEDGE ||
//  			   identifier.vectype==VECTYPE_EDGECORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
  	_boundary.overwriteTwoEdges(newboundary,edgeIndexY,edgeIndexZ);
//		_boundary.indexEdgeEdge[edgeIndexY]   = newboundary.indexEdgeEdge[edgeIndexY];
//		_boundary.indexEdgeCorner[edgeIndexY] = newboundary.indexEdgeCorner[edgeIndexY];
//		_boundary.indexEdgeEdge[edgeIndexZ]   = newboundary.indexEdgeEdge[edgeIndexZ];
//		_boundary.indexEdgeCorner[edgeIndexZ] = newboundary.indexEdgeCorner[edgeIndexZ];
		_boundary.indexCorner  [cornerIndex]  = newboundary.indexCorner[cornerIndex];
	}
	//Wall Y
	{
		int wallIndex  = 2+((co&2)!=0);
		int edgeIndexX = 0+((co&2)!=0)+2*((co&4)==0);
		int edgeIndexZ = 8+((co&1)==0)+2*((co&2)!=0);
		int cornerIndex = ((co&1)==0)+2*((co&2)!=0)+4*((co&4)==0);

//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if(identifier.vecindex == wallIndex &&
//  				(identifier.vectype==VECTYPE_WALLWALL ||
//  			   identifier.vectype==VECTYPE_WALLEDGE ||
//  				 identifier.vectype==VECTYPE_WALLCORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
		_boundary.addWall(newboundary,wallIndex);
  	for(size_t i=0;i<_boundary.border_size();i++){
  		MCNCompact::CellReferenceNum ref = _boundary.get(i);
  		size_t cellIndex = ref.second;
  		MCNCompact::vecpair identifier = MCNCompact::decnum(ref.first);
  		if(identifier.vecindex == edgeIndexX){
  			if(identifier.vectype==VECTYPE_EDGEEDGE){
    			_boundary.add(VECTYPE_WALLEDGE,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&3)+4*((co&4)==0)].add(VECTYPE_EDGEEDGE,0+((co&2)!=0)+2*((co&4)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[3] = index+4; break;
    			case 1: cell.lastBranch[3] = index+5; break;
    			case 2: cell.lastBranch[2] = index+6; break;
    			case 3: cell.lastBranch[2] = index+7; break;
    			case 4: cell.lastBranch[1] = index+0; break;
    			case 5: cell.lastBranch[1] = index+1; break;
    			case 6: cell.lastBranch[0] = index+2; break;
    			case 7: cell.lastBranch[0] = index+3; break;
    			}
  			}
  			if(identifier.vectype==VECTYPE_EDGECORN){
    			_boundary.add(VECTYPE_WALLCORN,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&3)+4*((co&4)==0)].add(VECTYPE_EDGECORN,0+((co&2)!=0)+2*((co&4)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[6] = cell.lastBranch[7] = index+4; break;
    			case 1: cell.lastBranch[6] = cell.lastBranch[7] = index+5; break;
    			case 2: cell.lastBranch[4] = cell.lastBranch[5] = index+6; break;
    			case 3: cell.lastBranch[4] = cell.lastBranch[5] = index+7; break;
    			case 4: cell.lastBranch[2] = cell.lastBranch[3] = index+0; break;
    			case 5: cell.lastBranch[2] = cell.lastBranch[3] = index+1; break;
    			case 6: cell.lastBranch[0] = cell.lastBranch[1] = index+2; break;
    			case 7: cell.lastBranch[0] = cell.lastBranch[1] = index+3; break;
    			}
  			}
  		}
  		if(identifier.vecindex == edgeIndexZ){
  			if(identifier.vectype==VECTYPE_EDGEEDGE){
    			_boundary.add(VECTYPE_WALLEDGE,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&6)+((co&1)==0)].add(VECTYPE_EDGEEDGE,8+((co&1)!=0)+2*((co&2)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[3] = index+1; break;
    			case 1: cell.lastBranch[2] = index+0; break;
    			case 2: cell.lastBranch[1] = index+3; break;
    			case 3: cell.lastBranch[0] = index+2; break;
    			case 4: cell.lastBranch[3] = index+5; break;
    			case 5: cell.lastBranch[2] = index+4; break;
    			case 6: cell.lastBranch[1] = index+7; break;
    			case 7: cell.lastBranch[0] = index+6; break;
    			}
  			}
  			if(identifier.vectype==VECTYPE_EDGECORN){
    			_boundary.add(VECTYPE_WALLCORN,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&6)+((co&1)==0)].add(VECTYPE_EDGECORN,8+((co&1)!=0)+2*((co&2)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[3] = cell.lastBranch[7] = index+1; break;
    			case 1: cell.lastBranch[2] = cell.lastBranch[6] = index+0; break;
    			case 2: cell.lastBranch[1] = cell.lastBranch[5] = index+3; break;
    			case 3: cell.lastBranch[0] = cell.lastBranch[4] = index+2; break;
    			case 4: cell.lastBranch[3] = cell.lastBranch[7] = index+5; break;
    			case 5: cell.lastBranch[2] = cell.lastBranch[6] = index+4; break;
    			case 6: cell.lastBranch[1] = cell.lastBranch[5] = index+7; break;
    			case 7: cell.lastBranch[0] = cell.lastBranch[4] = index+6; break;
    			}
  			}
  		}
  	}

		{
			_boundary.add(VECTYPE_WALLCORN,wallIndex,_boundary.indexCorner[cornerIndex]);
			MeshCell & cell = _meshCells[_boundary.indexCorner[cornerIndex]];
			if((co&5)!=0){
				_meshCellIndicesBranch[index+0+2*((co&2)==0)].indexCorner[5+(co&2)] = _boundary.indexCorner[cornerIndex];
				if(co&2) cell.lastBranch[2] = index+0+0;
				else     cell.lastBranch[0] = index+0+2;
			}
			if((co&5)!=1){
				_meshCellIndicesBranch[index+1+2*((co&2)==0)].indexCorner[4+(co&2)] = _boundary.indexCorner[cornerIndex];
				if(co&2) cell.lastBranch[3] = index+1+0;
				else     cell.lastBranch[1] = index+1+2;
			}
			if((co&5)!=4){
				_meshCellIndicesBranch[index+4+2*((co&2)==0)].indexCorner[1+(co&2)] = _boundary.indexCorner[cornerIndex];
				if(co&2) cell.lastBranch[6] = index+4+0;
				else     cell.lastBranch[4] = index+4+2;
			}
			if((co&5)!=5){
				_meshCellIndicesBranch[index+5+2*((co&2)==0)].indexCorner[0+(co&2)] = _boundary.indexCorner[cornerIndex];
				if(co&2) cell.lastBranch[7] = index+5+0;
				else     cell.lastBranch[5] = index+5+2;
			}
		}

//  	//Taking out the old indices
//  	MCNCompact::cellvector temp;
//  	for(size_t i=0;i<_boundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(_boundary.cells[i].second);
//  		if(!((identifier.vecindex == edgeIndexX || identifier.vecindex == edgeIndexZ) &&
//  		  	(identifier.vectype==VECTYPE_EDGEEDGE ||
//  		  	 identifier.vectype==VECTYPE_EDGECORN))){
//  			temp.push_back(_boundary.cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
//  		}
//  	}
//  	_boundary.cells = temp;
//  	//Putting in the new indices
//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if((identifier.vecindex == edgeIndexX || identifier.vecindex == edgeIndexZ) &&
//  				(identifier.vectype==VECTYPE_EDGEEDGE ||
//  			   identifier.vectype==VECTYPE_EDGECORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
  	_boundary.overwriteTwoEdges(newboundary,edgeIndexX,edgeIndexZ);
//		_boundary.indexEdgeEdge[edgeIndexX] = newboundary.indexEdgeEdge[edgeIndexX];
//		_boundary.indexEdgeCorner[edgeIndexX] = newboundary.indexEdgeCorner[edgeIndexX];
//		_boundary.indexEdgeEdge[edgeIndexZ] = newboundary.indexEdgeEdge[edgeIndexZ];
//		_boundary.indexEdgeCorner[edgeIndexZ] = newboundary.indexEdgeCorner[edgeIndexZ];
		_boundary.indexCorner  [cornerIndex] =
		newboundary.indexCorner[cornerIndex];
	}
	//Wall Z
	{
		int wallIndex  = 4+((co&4)!=0);
		int edgeIndexX = 0+((co&2)==0)+2*((co&4)!=0);
		int edgeIndexY = 4+((co&1)==0)+2*((co&4)!=0);
		int cornerIndex = ((co&1)==0)+2*((co&2)==0)+4*((co&4)!=0);

//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if(identifier.vecindex == wallIndex &&
//  				(identifier.vectype==VECTYPE_WALLWALL ||
//  			   identifier.vectype==VECTYPE_WALLEDGE ||
//  				 identifier.vectype==VECTYPE_WALLCORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
		_boundary.addWall(newboundary,wallIndex);
  	for(size_t i=0;i<_boundary.border_size();i++){
  		MCNCompact::CellReferenceNum ref = _boundary.get(i);
  		size_t cellIndex = ref.second;
  		MCNCompact::vecpair identifier = MCNCompact::decnum(ref.first);
  		if(identifier.vecindex == edgeIndexX){
  			if(identifier.vectype==VECTYPE_EDGEEDGE){
    			_boundary.add(VECTYPE_WALLEDGE,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&5)+2*((co&2)==0)].add(VECTYPE_EDGEEDGE,0+((co&2)!=0)+2*((co&4)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[3] = index+2; break;
    			case 1: cell.lastBranch[3] = index+3; break;
    			case 2: cell.lastBranch[2] = index+0; break;
    			case 3: cell.lastBranch[2] = index+1; break;
    			case 4: cell.lastBranch[1] = index+6; break;
    			case 5: cell.lastBranch[1] = index+7; break;
    			case 6: cell.lastBranch[0] = index+4; break;
    			case 7: cell.lastBranch[0] = index+5; break;
    			}
  			}
  			if(identifier.vectype==VECTYPE_EDGECORN){
    			_boundary.add(VECTYPE_WALLCORN,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&5)+2*((co&2)==0)].add(VECTYPE_EDGECORN,0+((co&2)!=0)+2*((co&4)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[6] = cell.lastBranch[7] = index+2; break;
    			case 1: cell.lastBranch[6] = cell.lastBranch[7] = index+3; break;
    			case 2: cell.lastBranch[4] = cell.lastBranch[5] = index+0; break;
    			case 3: cell.lastBranch[4] = cell.lastBranch[5] = index+1; break;
    			case 4: cell.lastBranch[2] = cell.lastBranch[3] = index+6; break;
    			case 5: cell.lastBranch[2] = cell.lastBranch[3] = index+7; break;
    			case 6: cell.lastBranch[0] = cell.lastBranch[1] = index+4; break;
    			case 7: cell.lastBranch[0] = cell.lastBranch[1] = index+5; break;
    			}
  			}
  		}
  		if(identifier.vecindex == edgeIndexY){
  			if(identifier.vectype==VECTYPE_EDGEEDGE){
    			_boundary.add(VECTYPE_WALLEDGE,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&6)+((co&1)==0)].add(VECTYPE_EDGEEDGE,4+((co&1)!=0)+2*((co&4)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[3] = index+1; break;
    			case 1: cell.lastBranch[2] = index+0; break;
    			case 2: cell.lastBranch[3] = index+3; break;
    			case 3: cell.lastBranch[2] = index+2; break;
    			case 4: cell.lastBranch[1] = index+5; break;
    			case 5: cell.lastBranch[0] = index+4; break;
    			case 6: cell.lastBranch[1] = index+7; break;
    			case 7: cell.lastBranch[0] = index+6; break;
    			}
  			}
  			if(identifier.vectype==VECTYPE_EDGECORN){
    			_boundary.add(VECTYPE_WALLCORN,wallIndex,cellIndex);
    			_meshCellIndicesBranch[(co&6)+((co&1)==0)].add(VECTYPE_EDGECORN,4+((co&1)!=0)+2*((co&4)!=0),cellIndex);
    			MeshCell &cell = _meshCells[cellIndex];
    			switch(co){
    			case 0: cell.lastBranch[5] = cell.lastBranch[7] = index+1; break;
    			case 1: cell.lastBranch[4] = cell.lastBranch[6] = index+0; break;
    			case 2: cell.lastBranch[5] = cell.lastBranch[7] = index+3; break;
    			case 3: cell.lastBranch[4] = cell.lastBranch[6] = index+2; break;
    			case 4: cell.lastBranch[1] = cell.lastBranch[3] = index+5; break;
    			case 5: cell.lastBranch[0] = cell.lastBranch[2] = index+4; break;
    			case 6: cell.lastBranch[1] = cell.lastBranch[3] = index+7; break;
    			case 7: cell.lastBranch[0] = cell.lastBranch[2] = index+6; break;
    			}
  			}
  		}
  	}


		{
			_boundary.add(VECTYPE_WALLCORN,wallIndex,_boundary.indexCorner[cornerIndex]);
			MeshCell & cell = _meshCells[_boundary.indexCorner[cornerIndex]];
			if((co&3)!=0){
				_meshCellIndicesBranch[0+4*((co&4)==0)].indexCorner[3+(co&4)] = _boundary.indexCorner[cornerIndex];
				if(co&4) cell.lastBranch[4] = index+0+0;
				else     cell.lastBranch[0] = index+0+4;
			}
			if((co&3)!=1){
				_meshCellIndicesBranch[1+4*((co&4)==0)].indexCorner[2+(co&4)] = _boundary.indexCorner[cornerIndex];
				if(co&4) cell.lastBranch[5] = index+1+0;
				else     cell.lastBranch[1] = index+1+4;
			}
			if((co&3)!=2){
				_meshCellIndicesBranch[2+4*((co&4)==0)].indexCorner[1+(co&4)] = _boundary.indexCorner[cornerIndex];
				if(co&4) cell.lastBranch[6] = index+2+0;
				else     cell.lastBranch[2] = index+2+4;
			}
			if((co&3)!=3){
				_meshCellIndicesBranch[3+4*((co&4)==0)].indexCorner[0+(co&4)] = _boundary.indexCorner[cornerIndex];
				if(co&4) cell.lastBranch[7] = index+3+0;
				else     cell.lastBranch[3] = index+3+4;
			}

		}


//  	//Taking out the old indices
//  	MCNCompact::cellvector temp;
//  	for(size_t i=0;i<_boundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(_boundary.cells[i].second);
//  		if(!((identifier.vecindex == edgeIndexX || identifier.vecindex == edgeIndexY) &&
//  		  	(identifier.vectype==VECTYPE_EDGEEDGE ||
//  		  	 identifier.vectype==VECTYPE_EDGECORN))){
//  			temp.push_back(_boundary.cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
//  		}
//  	}
//  	_boundary.cells = temp;
//  	//Putting in the new indices
//  	for(size_t i=0;i<newboundary.cells.size();i++){
//  		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//  		if((identifier.vecindex == edgeIndexX || identifier.vecindex == edgeIndexY) &&
//  				(identifier.vectype==VECTYPE_EDGEEDGE ||
//  			   identifier.vectype==VECTYPE_EDGECORN)){
//  			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//  		}
//  	}
  	_boundary.overwriteTwoEdges(newboundary,edgeIndexX,edgeIndexY);

//		_boundary.indexEdgeEdge[edgeIndexX] = newboundary.indexEdgeEdge[edgeIndexX];
//		_boundary.indexEdgeCorner[edgeIndexX] = newboundary.indexEdgeCorner[edgeIndexX];
//		_boundary.indexEdgeEdge[edgeIndexY] = newboundary.indexEdgeEdge[edgeIndexY];
//		_boundary.indexEdgeCorner[edgeIndexY] = newboundary.indexEdgeCorner[edgeIndexY];
		_boundary.indexCorner  [cornerIndex] =
		newboundary.indexCorner[cornerIndex];
	}
	fprintf(stderr,"\nEdges");
	int edgeIndexX = 0+((co&2)!=0)+2*((co&4)!=0);
	int edgeIndexY = 4+((co&1)!=0)+2*((co&4)!=0);
	int edgeIndexZ = 8+((co&1)!=0)+2*((co&2)!=0);
//	for(size_t i=0;i<newboundary.cells.size();i++){
//		MCNCompact::vecpair identifier = MCNCompact::decnum(newboundary.cells[i].second);
//		if((identifier.vecindex == edgeIndexX ||
//				identifier.vecindex == edgeIndexY ||
//				identifier.vecindex == edgeIndexZ) &&
//				(identifier.vectype==VECTYPE_EDGEEDGE ||
//			   identifier.vectype==VECTYPE_EDGECORN)){
//			_boundary.add(identifier.vectype,identifier.vecindex,newboundary.cells[i].first);
//		}
//	}
	_boundary.addThreeEdges(newboundary,edgeIndexX,edgeIndexY,edgeIndexZ);

//	Edge X
//	{
//		int edgeIndex = 0+((co&2)!=0)+2*((co&4)!=0);
//  	for(MeshCellNeighborhood::CellList::iterator i=newboundary.indexEdgeEdge[edgeIndex].begin();
//  			i!=newboundary.indexEdgeEdge[edgeIndex].end();i++){
//  		_boundary.indexEdgeEdge[edgeIndex].push_back(*i);
//  	}
//  	for(MeshCellNeighborhood::CellList::iterator i=newboundary.indexEdgeCorner[edgeIndex].begin();
//  			i!=newboundary.indexEdgeCorner[edgeIndex].end();i++){
//  		_boundary.indexEdgeCorner[edgeIndex].push_back(*i);
//  	}
//	}
//	//Edge Y
//	{
//		int edgeIndex = 4+((co&1)!=0)+2*((co&4)!=0);
//  	for(MeshCellNeighborhood::CellList::iterator i=newboundary.indexEdgeEdge[edgeIndex].begin();
//  			i!=newboundary.indexEdgeEdge[edgeIndex].end();i++){
//  		_boundary.indexEdgeEdge[edgeIndex].push_back(*i);
//  	}
//  	for(MeshCellNeighborhood::CellList::iterator i=newboundary.indexEdgeCorner[edgeIndex].begin();
//  			i!=newboundary.indexEdgeCorner[edgeIndex].end();i++){
//  		_boundary.indexEdgeCorner[edgeIndex].push_back(*i);
//  	}
//	}
//
//	//Edge Z
//	{
//		int edgeIndex = 8+((co&1)!=0)+2*((co&2)!=0);
//  	for(MeshCellNeighborhood::CellList::iterator i=newboundary.indexEdgeEdge[edgeIndex].begin();
//  			i!=newboundary.indexEdgeEdge[edgeIndex].end();i++){
//  		_boundary.indexEdgeEdge[edgeIndex].push_back(*i);
//  	}
//  	for(MeshCellNeighborhood::CellList::iterator i=newboundary.indexEdgeCorner[edgeIndex].begin();
//  			i!=newboundary.indexEdgeCorner[edgeIndex].end();i++){
//  		_boundary.indexEdgeCorner[edgeIndex].push_back(*i);
//  	}
//	}

}



















inline void createMeshCellStructuresForBranch_list_vector_polymorph_compact
(
		FusionMipMapCPU::MeshCellArray &_meshCells,
		FusionMipMapCPU::BranchIndicesCompactArray &_meshCellIndicesBranch,
		FusionMipMapCPU::LeafIndicesArray &_meshCellIndicesLeaf,
		MCNCompact &_boundary,
		const volumetype &child,
		const size_t newBranchIndex,
		const volumetype &pl,
		const uchar &childBranchSize,
		const sidetype &_n
){
//	fprintf(stderr,"\nCreating compact MeshCell Structures for Subbranch %li",newBranchIndex);

//			_meshCellIndicesBranch.resize(oldSize+16);
//			for(size_t i=oldSize;i<oldSize+16;i++){
//	_meshCellIndicesBranch.resize(newBranchIndex+childBranchSize);
	for(size_t i=newBranchIndex;i<newBranchIndex+childBranchSize;i++){
		_meshCellIndicesBranch[i].indexInterior = MAXCELLINDEX;
	}

//			fprintf(stderr,"\nAdding Branch");
	size_t i = _meshCellIndicesBranch[child].indexInterior;
	if(i>=MAXCELLINDEX){
		 fprintf(stderr,"\nERROR MCNCompact: Branch[%i] has no Mesh Cell",child);
//		 fprintf(stderr,"\nBranch to be subdivided has Position/Size [%i %i %i]%i",ox,oy,oz,half);
	}
	_meshCellIndicesBranch[newBranchIndex+0].indexInterior = i;
	_meshCells[i].lastLeaf[0] = pl;
	const sidetype oxnew = _meshCells[i].ox;
	const sidetype oynew = _meshCells[i].oy;
	const sidetype oznew = _meshCells[i].oz;
	const sidetype half = _meshCells[i].size;
	const sidetype quarter = half/2;
	_meshCells[i].size = quarter;

	_meshCells.push_back(MeshCell(0,oxnew+quarter,oynew,        oznew,        quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew,        oynew+quarter,oznew,        quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew+quarter,oynew+quarter,oznew,        quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew,        oynew,        oznew+quarter,quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew+quarter,oynew,        oznew+quarter,quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew,        oynew+quarter,oznew+quarter,quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,pl));

	_meshCellIndicesBranch[newBranchIndex+1].indexInterior = _meshCells.size()-7;
	_meshCellIndicesBranch[newBranchIndex+2].indexInterior = _meshCells.size()-6;
	_meshCellIndicesBranch[newBranchIndex+3].indexInterior = _meshCells.size()-5;
	_meshCellIndicesBranch[newBranchIndex+4].indexInterior = _meshCells.size()-4;
	_meshCellIndicesBranch[newBranchIndex+5].indexInterior = _meshCells.size()-3;
	_meshCellIndicesBranch[newBranchIndex+6].indexInterior = _meshCells.size()-2;
	_meshCellIndicesBranch[newBranchIndex+7].indexInterior = _meshCells.size()-1;

	_meshCellIndicesBranch[child].indexInterior = MAXCELLINDEX;

//			fprintf(stderr,"\nInterior Updated");

	_meshCells.push_back(MeshCell(1,oxnew,oynew        ,oznew        ,quarter,pl,pl));
	_meshCells.push_back(MeshCell(1,oxnew,oynew+quarter,oznew        ,quarter,pl,pl));
	_meshCells.push_back(MeshCell(1,oxnew,oynew        ,oznew+quarter,quarter,pl,pl));
	_meshCells.push_back(MeshCell(1,oxnew,oynew+quarter,oznew+quarter,quarter,pl,pl));

	_meshCells.push_back(MeshCell(2,oxnew        ,oynew,oznew        ,quarter,pl,pl));
	_meshCells.push_back(MeshCell(2,oxnew+quarter,oynew,oznew        ,quarter,pl,pl));
	_meshCells.push_back(MeshCell(2,oxnew        ,oynew,oznew+quarter,quarter,pl,pl));
	_meshCells.push_back(MeshCell(2,oxnew+quarter,oynew,oznew+quarter,quarter,pl,pl));

	_meshCells.push_back(MeshCell(3,oxnew        ,oynew        ,oznew,quarter,pl,pl));
	_meshCells.push_back(MeshCell(3,oxnew+quarter,oynew        ,oznew,quarter,pl,pl));
	_meshCells.push_back(MeshCell(3,oxnew        ,oynew+quarter,oznew,quarter,pl,pl));
	_meshCells.push_back(MeshCell(3,oxnew+quarter,oynew+quarter,oznew,quarter,pl,pl));

//			if(oznew+quarter==640){
//				fprintf(stderr,"\nCreated 4 Z-Walls at [%i %i %i]%i : %li %li %li %li",
//						oxnew,oynew,oznew,quarter,
//						_meshCells.size()-4,_meshCells.size()-3,_meshCells.size()-2,_meshCells.size()-1);
//			}

	_meshCells[_meshCells.size()-12].lastBranch[0] = newBranchIndex+0;
	_meshCells[_meshCells.size()-12].lastBranch[1] = newBranchIndex+1;
	_meshCells[_meshCells.size()-11].lastBranch[0] = newBranchIndex+2;
	_meshCells[_meshCells.size()-11].lastBranch[1] = newBranchIndex+3;
	_meshCells[_meshCells.size()-10].lastBranch[0] = newBranchIndex+4;
	_meshCells[_meshCells.size()-10].lastBranch[1] = newBranchIndex+5;
	_meshCells[_meshCells.size()-9].lastBranch[0] = newBranchIndex+6;
	_meshCells[_meshCells.size()-9].lastBranch[1] = newBranchIndex+7;

	_meshCells[_meshCells.size()-8].lastBranch[0] = newBranchIndex+0;
	_meshCells[_meshCells.size()-8].lastBranch[1] = newBranchIndex+2;
	_meshCells[_meshCells.size()-7].lastBranch[0] = newBranchIndex+1;
	_meshCells[_meshCells.size()-7].lastBranch[1] = newBranchIndex+3;
	_meshCells[_meshCells.size()-6].lastBranch[0] = newBranchIndex+4;
	_meshCells[_meshCells.size()-6].lastBranch[1] = newBranchIndex+6;
	_meshCells[_meshCells.size()-5].lastBranch[0] = newBranchIndex+5;
	_meshCells[_meshCells.size()-5].lastBranch[1] = newBranchIndex+7;

	_meshCells[_meshCells.size()-4].lastBranch[0] = newBranchIndex+0;
	_meshCells[_meshCells.size()-4].lastBranch[1] = newBranchIndex+4;
	_meshCells[_meshCells.size()-3].lastBranch[0] = newBranchIndex+1;
	_meshCells[_meshCells.size()-3].lastBranch[1] = newBranchIndex+5;
	_meshCells[_meshCells.size()-2].lastBranch[0] = newBranchIndex+2;
	_meshCells[_meshCells.size()-2].lastBranch[1] = newBranchIndex+6;
	_meshCells[_meshCells.size()-1].lastBranch[0] = newBranchIndex+3;
	_meshCells[_meshCells.size()-1].lastBranch[1] = newBranchIndex+7;

	_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_WALLWALL,1,_meshCells.size()-12);
	_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_WALLWALL,0,_meshCells.size()-12);
	_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_WALLWALL,1,_meshCells.size()-11);
	_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_WALLWALL,0,_meshCells.size()-11);
	_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_WALLWALL,1,_meshCells.size()-10);
	_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_WALLWALL,0,_meshCells.size()-10);
	_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_WALLWALL,1,_meshCells.size()-9);
	_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_WALLWALL,0,_meshCells.size()-9);

	_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_WALLWALL,3,_meshCells.size()-8);
	_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_WALLWALL,2,_meshCells.size()-8);
	_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_WALLWALL,3,_meshCells.size()-7);
	_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_WALLWALL,2,_meshCells.size()-7);
	_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_WALLWALL,3,_meshCells.size()-6);
	_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_WALLWALL,2,_meshCells.size()-6);
	_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_WALLWALL,3,_meshCells.size()-5);
	_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_WALLWALL,2,_meshCells.size()-5);

	_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_WALLWALL,5,_meshCells.size()-4);
	_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_WALLWALL,4,_meshCells.size()-4);
	_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_WALLWALL,5,_meshCells.size()-3);
	_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_WALLWALL,4,_meshCells.size()-3);
	_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_WALLWALL,5,_meshCells.size()-2);
	_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
	_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_WALLWALL,5,_meshCells.size()-1);
	_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_WALLWALL,4,_meshCells.size()-1);

	//Edges
	_meshCells.push_back(MeshCell(4,oxnew        ,oynew        ,oznew        ,quarter,pl,pl,pl,pl));
	_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew        ,oznew        ,quarter,pl,pl,pl,pl));

	_meshCells.push_back(MeshCell(5,oxnew        ,oynew        ,oznew        ,quarter,pl,pl,pl,pl));
	_meshCells.push_back(MeshCell(5,oxnew        ,oynew+quarter,oznew        ,quarter,pl,pl,pl,pl));

	_meshCells.push_back(MeshCell(6,oxnew        ,oynew        ,oznew        ,quarter,pl,pl,pl,pl));
	_meshCells.push_back(MeshCell(6,oxnew        ,oynew        ,oznew+quarter,quarter,pl,pl,pl,pl));

	_meshCells[_meshCells.size()-6].lastBranch[0] = newBranchIndex+0;
	_meshCells[_meshCells.size()-6].lastBranch[1] = newBranchIndex+2;
	_meshCells[_meshCells.size()-6].lastBranch[2] = newBranchIndex+4;
	_meshCells[_meshCells.size()-6].lastBranch[3] = newBranchIndex+6;

	_meshCells[_meshCells.size()-5].lastBranch[0] = newBranchIndex+1;
	_meshCells[_meshCells.size()-5].lastBranch[1] = newBranchIndex+3;
	_meshCells[_meshCells.size()-5].lastBranch[2] = newBranchIndex+5;
	_meshCells[_meshCells.size()-5].lastBranch[3] = newBranchIndex+7;

	_meshCells[_meshCells.size()-4].lastBranch[0] = newBranchIndex+0;
	_meshCells[_meshCells.size()-4].lastBranch[1] = newBranchIndex+1;
	_meshCells[_meshCells.size()-4].lastBranch[2] = newBranchIndex+4;
	_meshCells[_meshCells.size()-4].lastBranch[3] = newBranchIndex+5;

	_meshCells[_meshCells.size()-3].lastBranch[0] = newBranchIndex+2;
	_meshCells[_meshCells.size()-3].lastBranch[1] = newBranchIndex+3;
	_meshCells[_meshCells.size()-3].lastBranch[2] = newBranchIndex+6;
	_meshCells[_meshCells.size()-3].lastBranch[3] = newBranchIndex+7;

	_meshCells[_meshCells.size()-2].lastBranch[0] = newBranchIndex+0;
	_meshCells[_meshCells.size()-2].lastBranch[1] = newBranchIndex+1;
	_meshCells[_meshCells.size()-2].lastBranch[2] = newBranchIndex+2;
	_meshCells[_meshCells.size()-2].lastBranch[3] = newBranchIndex+3;

	_meshCells[_meshCells.size()-1].lastBranch[0] = newBranchIndex+4;
	_meshCells[_meshCells.size()-1].lastBranch[1] = newBranchIndex+5;
	_meshCells[_meshCells.size()-1].lastBranch[2] = newBranchIndex+6;
	_meshCells[_meshCells.size()-1].lastBranch[3] = newBranchIndex+7;

	_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE,3,_meshCells.size()-6);
	_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE,2,_meshCells.size()-6);
	_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE,1,_meshCells.size()-6);
	_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE,0,_meshCells.size()-6);

	_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE,3,_meshCells.size()-5);
	_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE,2,_meshCells.size()-5);
	_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE,1,_meshCells.size()-5);
	_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE,0,_meshCells.size()-5);

	_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE,7,_meshCells.size()-4);
	_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE,6,_meshCells.size()-4);
	_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE,5,_meshCells.size()-4);
	_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE,4,_meshCells.size()-4);

	_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE,7,_meshCells.size()-3);
	_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE,6,_meshCells.size()-3);
	_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE,5,_meshCells.size()-3);
	_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE,4,_meshCells.size()-3);

	_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-2);
	_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-2);
	_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-2);
	_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-2);

	_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-1);
	_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-1);
	_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-1);
	_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-1);

	//Corner(s)
	_meshCells.push_back(MeshCell(7,oxnew,oynew,oznew,quarter,pl,pl,pl,pl,pl,pl,pl,pl));
	_meshCells.back().lastBranch[0] = newBranchIndex+0;
	_meshCells.back().lastBranch[1] = newBranchIndex+1;
	_meshCells.back().lastBranch[2] = newBranchIndex+2;
	_meshCells.back().lastBranch[3] = newBranchIndex+3;
	_meshCells.back().lastBranch[4] = newBranchIndex+4;
	_meshCells.back().lastBranch[5] = newBranchIndex+5;
	_meshCells.back().lastBranch[6] = newBranchIndex+6;
	_meshCells.back().lastBranch[7] = newBranchIndex+7;
	_meshCellIndicesBranch[newBranchIndex+0].indexCorner[7] = _meshCells.size()-1;
	_meshCellIndicesBranch[newBranchIndex+1].indexCorner[6] = _meshCells.size()-1;
	_meshCellIndicesBranch[newBranchIndex+2].indexCorner[5] = _meshCells.size()-1;
	_meshCellIndicesBranch[newBranchIndex+3].indexCorner[4] = _meshCells.size()-1;
	_meshCellIndicesBranch[newBranchIndex+4].indexCorner[3] = _meshCells.size()-1;
	_meshCellIndicesBranch[newBranchIndex+5].indexCorner[2] = _meshCells.size()-1;
	_meshCellIndicesBranch[newBranchIndex+6].indexCorner[1] = _meshCells.size()-1;
	_meshCellIndicesBranch[newBranchIndex+7].indexCorner[0] = _meshCells.size()-1;

#ifndef DEBUG_NO_LEAFCELLS
	//Leaf
	if(pl<BRANCHINIT){
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-26);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-25);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-24);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-23);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-22);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-21);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-20);

		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-19);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-18);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-17);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-16);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-15);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-14);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-13);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-12);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-11);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-10);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-9);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-8);

		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-7);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-6);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-5);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);

		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
	}
#endif

	//TODO: Wieder rausnehmen
	_meshCellIndicesBranch[child].sort();

	for(size_t i=0;i<_meshCellIndicesBranch[child].border_size();i++){
		MCNCompact::CellReferenceNum ref = _meshCellIndicesBranch[child].get(i);
		size_t index = ref.second;
		MeshCell &cell = _meshCells[index];
		switch(ref.first){
		case VECTYPE_WALLWALL_0:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_WALLWALL,0,index);
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew; cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[1] = newBranchIndex+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[0];

			_meshCells.push_back(MeshCell(1,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew        ,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(1,oxnew-quarter*(oxnew!=0),oynew        ,oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(1,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew+quarter,quarter,ll0,ll1));

			_meshCells[_meshCells.size()-3].lastBranch[1] = newBranchIndex+2;
			_meshCells[_meshCells.size()-2].lastBranch[1] = newBranchIndex+4;
			_meshCells[_meshCells.size()-1].lastBranch[1] = newBranchIndex+6;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;

			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_WALLWALL,0,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_WALLWALL,0,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_WALLWALL,0,_meshCells.size()-1);
			if(oxnew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,1,_meshCells.size()-3);
			if(oxnew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,1,_meshCells.size()-2);
			if(oxnew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,1,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(5,oxnew-quarter*(oxnew!=0),oynew        ,oznew,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(5,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew-quarter*(oxnew!=0),oynew,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew-quarter*(oxnew!=0),oynew,oznew+quarter,quarter,ll0,ll1,ll0,ll1));

			_meshCells[_meshCells.size()-4].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[1] = newBranchIndex+0;
			_meshCells[_meshCells.size()-4].lastBranch[3] = newBranchIndex+4;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[1] = newBranchIndex+2;
			_meshCells[_meshCells.size()-3].lastBranch[3] = newBranchIndex+6;

			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = newBranchIndex+0;
			_meshCells[_meshCells.size()-2].lastBranch[3] = newBranchIndex+2;

			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = newBranchIndex+4;
			_meshCells[_meshCells.size()-1].lastBranch[3] = newBranchIndex+6;

			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-4);
			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-4);
			if(oxnew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,1,_meshCells.size()-4);

			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-3);
			if(oxnew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,1,_meshCells.size()-3);

			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-2);
			if(oxnew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,1,_meshCells.size()-2);

			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-1);
			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-1);
			if(oxnew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,1,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew-quarter*(oxnew!=0),oynew,oznew,quarter,ll0,ll1,ll0,ll1,ll0,ll1,ll0,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[2] = corner.lastBranch[4] = corner.lastBranch[6] = nb;
			corner.lastBranch[1] = newBranchIndex+0; corner.lastBranch[3] = newBranchIndex+2;
			corner.lastBranch[5] = newBranchIndex+4; corner.lastBranch[7] = newBranchIndex+6;

			_meshCellIndicesBranch[newBranchIndex+0].indexCorner[6] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+2].indexCorner[4] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+4].indexCorner[2] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+6].indexCorner[0] = _meshCells.size()-1;
			if(oxnew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLCORN,1,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				//3 Walls
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-8);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-7);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-6);
				//4 Edges
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-5);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				//1 Middle
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oxnew==0){
				_boundary.add(VECTYPE_WALLWALL,0,_meshCells.size()-8);
				_boundary.add(VECTYPE_WALLWALL,0,_meshCells.size()-7);
				_boundary.add(VECTYPE_WALLWALL,0,_meshCells.size()-6);
				_boundary.add(VECTYPE_WALLEDGE,0,_meshCells.size()-5);
				_boundary.add(VECTYPE_WALLEDGE,0,_meshCells.size()-4);
				_boundary.add(VECTYPE_WALLEDGE,0,_meshCells.size()-3);
				_boundary.add(VECTYPE_WALLEDGE,0,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,0,_meshCells.size()-1);
			}
		} else {
			if(cell.ox>=oxnew) {
				fprintf(stderr,"\nERROR MCNCompact: Cell X offset too large: %i >= %i",cell.ox,oxnew);
			}
			volumetype cellchildindex = (cell.oy>=oynew+quarter)*2 + (cell.oz>=oznew+quarter)*4;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_WALLWALL,0,index);
			cell.lastBranch[1] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_WALLWALL_1:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_WALLWALL,1,index);
			cell.ox = oxnew+quarter; cell.oy = oynew; cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[0] = newBranchIndex+1;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[1];

			_meshCells.push_back(MeshCell(1,oxnew+quarter,oynew+quarter,oznew        ,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(1,oxnew+quarter,oynew        ,oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(1,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1));

			_meshCells[_meshCells.size()-3].lastBranch[0] = newBranchIndex+3;
			_meshCells[_meshCells.size()-2].lastBranch[0] = newBranchIndex+5;
			_meshCells[_meshCells.size()-1].lastBranch[0] = newBranchIndex+7;

			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;

			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_WALLWALL,1,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_WALLWALL,1,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_WALLWALL,1,_meshCells.size()-1);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,0,_meshCells.size()-3);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,0,_meshCells.size()-2);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,0,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(5,oxnew+quarter,oynew        ,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(5,oxnew+quarter,oynew+quarter,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew+quarter,oynew        ,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew+quarter,oynew        ,oznew+quarter,quarter,ll0,ll1,ll0,ll1));

			_meshCells[_meshCells.size()-4].lastBranch[0] = newBranchIndex+1;
			_meshCells[_meshCells.size()-4].lastBranch[2] = newBranchIndex+5;
			_meshCells[_meshCells.size()-4].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-3].lastBranch[0] = newBranchIndex+3;
			_meshCells[_meshCells.size()-3].lastBranch[2] = newBranchIndex+7;
			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-2].lastBranch[0] = newBranchIndex+1;
			_meshCells[_meshCells.size()-2].lastBranch[2] = newBranchIndex+3;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-1].lastBranch[0] = newBranchIndex+5;
			_meshCells[_meshCells.size()-1].lastBranch[2] = newBranchIndex+7;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[3] = nb;

			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-4);
			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-4);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,0,_meshCells.size()-4);

			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-3);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,0,_meshCells.size()-3);

			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-2);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,0,_meshCells.size()-2);

			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-1);
			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-1);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,0,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew+quarter,oynew,oznew,quarter,ll0,ll1,ll0,ll1,ll0,ll1,ll0,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[1] = corner.lastBranch[3] = corner.lastBranch[5] = corner.lastBranch[7] = nb;
			corner.lastBranch[0] = newBranchIndex+0; corner.lastBranch[2] = newBranchIndex+2;
			corner.lastBranch[4] = newBranchIndex+4; corner.lastBranch[6] = newBranchIndex+6;

			_meshCellIndicesBranch[newBranchIndex+1].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+3].indexCorner[5] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+5].indexCorner[3] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+7].indexCorner[1] = _meshCells.size()-1;
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLCORN,0,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				//3 Walls
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-8);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-7);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-6);
				//4 Edges
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-5);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				//1 Middle
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oxnew+half==_n){
				_boundary.add(VECTYPE_WALLWALL,1,_meshCells.size()-8);
				_boundary.add(VECTYPE_WALLWALL,1,_meshCells.size()-7);
				_boundary.add(VECTYPE_WALLWALL,1,_meshCells.size()-6);
				_boundary.add(VECTYPE_WALLEDGE,1,_meshCells.size()-5);
				_boundary.add(VECTYPE_WALLEDGE,1,_meshCells.size()-4);
				_boundary.add(VECTYPE_WALLEDGE,1,_meshCells.size()-3);
				_boundary.add(VECTYPE_WALLEDGE,1,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,1,_meshCells.size()-1);
			}
		} else {
			if(cell.ox+cell.size!=oxnew+half) {
				fprintf(stderr,"\nERROR MCNCompact: Cell X WallWall_1 at %li does not fit: %i+%i vs %i+%i",
						index,cell.ox,cell.size,oxnew,half);
			}
			volumetype cellchildindex = 1 + (cell.oy>=oynew+quarter)*2 + (cell.oz>=oznew+quarter)*4;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_WALLWALL,1,index);
			cell.lastBranch[0] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_WALLWALL_2:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_WALLWALL,2,index);
			cell.ox = oxnew; cell.oy = oynew-quarter*(oynew!=0); cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[1] = newBranchIndex+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[0];

			_meshCells.push_back(MeshCell(2,oxnew+quarter,oynew-quarter*(oynew!=0),oznew        ,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(2,oxnew        ,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(2,oxnew+quarter,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll1));

			_meshCells[_meshCells.size()-3].lastBranch[1] = newBranchIndex+1;
			_meshCells[_meshCells.size()-2].lastBranch[1] = newBranchIndex+4;
			_meshCells[_meshCells.size()-1].lastBranch[1] = newBranchIndex+5;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;

			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_WALLWALL,2,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_WALLWALL,2,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_WALLWALL,2,_meshCells.size()-1);
			if(oynew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,3,_meshCells.size()-3);
			if(oynew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,3,_meshCells.size()-2);
			if(oynew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,3,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(4,oxnew        ,oynew-quarter*(oynew!=0),oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew-quarter*(oynew!=0),oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew        ,oynew-quarter*(oynew!=0),oznew        ,quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(6,oxnew        ,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll0,ll1,ll1));


			fprintf(stderr,"\nindexWallWall[2]: Created Compact Edge Cells 4 and 6 from %li to %li",_meshCells.size()-4,_meshCells.size()-1);

			_meshCells[_meshCells.size()-4].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[1] = newBranchIndex+0;
			_meshCells[_meshCells.size()-4].lastBranch[3] = newBranchIndex+4;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[1] = newBranchIndex+1;
			_meshCells[_meshCells.size()-3].lastBranch[3] = newBranchIndex+5;

			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[2] = newBranchIndex+0;
			_meshCells[_meshCells.size()-2].lastBranch[3] = newBranchIndex+1;

			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[2] = newBranchIndex+4;
			_meshCells[_meshCells.size()-1].lastBranch[3] = newBranchIndex+5;

			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-4);
			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-4);
			if(oynew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,3,_meshCells.size()-4);

			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-3);
			if(oynew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,3,_meshCells.size()-3);

			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-2);
			if(oynew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,3,_meshCells.size()-2);

			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE, 9,_meshCells.size()-1);
			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE, 8,_meshCells.size()-1);
			if(oynew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,3,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew,oynew-quarter*(oynew!=0),oznew,quarter,ll0,ll0,ll1,ll1,ll0,ll0,ll1,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[1] = corner.lastBranch[4] = corner.lastBranch[5] = nb;
			corner.lastBranch[2] = newBranchIndex+0; corner.lastBranch[3] = newBranchIndex+1;
			corner.lastBranch[6] = newBranchIndex+4; corner.lastBranch[7] = newBranchIndex+5;

			_meshCellIndicesBranch[newBranchIndex+0].indexCorner[5] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+1].indexCorner[4] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+4].indexCorner[1] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+5].indexCorner[0] = _meshCells.size()-1;
			if(oynew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLCORN,3,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				//3 Walls
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-8);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-7);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-6);
				//4 Edges
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-5);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				//1 Middle
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oynew==0){
				_boundary.add(VECTYPE_WALLWALL,2,_meshCells.size()-8);
				_boundary.add(VECTYPE_WALLWALL,2,_meshCells.size()-7);
				_boundary.add(VECTYPE_WALLWALL,2,_meshCells.size()-6);
				_boundary.add(VECTYPE_WALLEDGE,2,_meshCells.size()-5);
				_boundary.add(VECTYPE_WALLEDGE,2,_meshCells.size()-4);
				_boundary.add(VECTYPE_WALLEDGE,2,_meshCells.size()-3);
				_boundary.add(VECTYPE_WALLEDGE,2,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,2,_meshCells.size()-1);
			}
		} else {
			if(cell.oy>=oynew) fprintf(stderr,"\nERROR MCNCompact: Cell Y offset too large: %i >= %i",cell.oy,oynew);
			volumetype cellchildindex = (cell.ox>=oxnew+quarter)*1 + (cell.oz>=oznew+quarter)*4;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_WALLWALL,2,index);
			cell.lastBranch[1] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_WALLWALL_3:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_WALLWALL,3,index);
			cell.ox = oxnew; cell.oy = oynew+quarter; cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[0] = newBranchIndex+2;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[1];

			_meshCells.push_back(MeshCell(2,oxnew+quarter,oynew+quarter,oznew        ,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(2,oxnew        ,oynew+quarter,oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(2,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1));

			_meshCells[_meshCells.size()-3].lastBranch[0] = newBranchIndex+3;
			_meshCells[_meshCells.size()-2].lastBranch[0] = newBranchIndex+6;
			_meshCells[_meshCells.size()-1].lastBranch[0] = newBranchIndex+7;

			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;

			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_WALLWALL,3,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_WALLWALL,3,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_WALLWALL,3,_meshCells.size()-1);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,2,_meshCells.size()-3);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,2,_meshCells.size()-2);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,2,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(4,oxnew        ,oynew+quarter,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew+quarter,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew        ,oynew+quarter,oznew        ,quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(6,oxnew        ,oynew+quarter,oznew+quarter,quarter,ll0,ll0,ll1,ll1));

			_meshCells[_meshCells.size()-4].lastBranch[0] = newBranchIndex+2;
			_meshCells[_meshCells.size()-4].lastBranch[2] = newBranchIndex+6;
			_meshCells[_meshCells.size()-4].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-3].lastBranch[0] = newBranchIndex+3;
			_meshCells[_meshCells.size()-3].lastBranch[2] = newBranchIndex+7;
			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-2].lastBranch[0] = newBranchIndex+2;
			_meshCells[_meshCells.size()-2].lastBranch[1] = newBranchIndex+3;
			_meshCells[_meshCells.size()-2].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-1].lastBranch[0] = newBranchIndex+6;
			_meshCells[_meshCells.size()-1].lastBranch[1] = newBranchIndex+7;
			_meshCells[_meshCells.size()-1].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[3] = nb;

			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-4);
			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-4);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,2,_meshCells.size()-4);

			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-3);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,2,_meshCells.size()-3);

			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-2);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,2,_meshCells.size()-2);

			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-1);
			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-1);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,2,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew,oynew+quarter,oznew,quarter,ll0,ll0,ll1,ll1,ll0,ll0,ll1,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[2] = corner.lastBranch[3] = corner.lastBranch[6] = corner.lastBranch[7] = nb;
			corner.lastBranch[0] = newBranchIndex+2; corner.lastBranch[1] = newBranchIndex+3;
			corner.lastBranch[4] = newBranchIndex+6; corner.lastBranch[5] = newBranchIndex+7;

			_meshCellIndicesBranch[newBranchIndex+2].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+3].indexCorner[6] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+6].indexCorner[3] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+7].indexCorner[2] = _meshCells.size()-1;
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLCORN,2,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				//3 Walls
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-8);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-7);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-6);
				//4 Edges
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-5);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				//1 Middle
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oynew+half==_n){
				_boundary.add(VECTYPE_WALLWALL,3,_meshCells.size()-8);
				_boundary.add(VECTYPE_WALLWALL,3,_meshCells.size()-7);
				_boundary.add(VECTYPE_WALLWALL,3,_meshCells.size()-6);
				_boundary.add(VECTYPE_WALLEDGE,3,_meshCells.size()-5);
				_boundary.add(VECTYPE_WALLEDGE,3,_meshCells.size()-4);
				_boundary.add(VECTYPE_WALLEDGE,3,_meshCells.size()-3);
				_boundary.add(VECTYPE_WALLEDGE,3,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,3,_meshCells.size()-1);
			}
		} else {
			if(cell.oy+cell.size!=oynew+half)
				fprintf(stderr,"\nERROR MCNCompact: Cell Y %li of %li does not fit: %i+%i vs %i+%i",
						index,_meshCells.size(),cell.oy,cell.size,oynew,half);
			volumetype cellchildindex = (cell.ox>=oxnew+quarter)*1 + 2 + (cell.oz>=oznew+quarter)*4;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_WALLWALL,3,index);
			cell.lastBranch[0] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_WALLWALL_4:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_WALLWALL,4,index);
			cell.ox = oxnew; cell.oy = oynew; cell.oz = oznew-quarter*(oznew!=0);
			cell.size = quarter; cell.lastBranch[1] = newBranchIndex+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[0];

			_meshCells.push_back(MeshCell(3,oxnew+quarter,oynew        ,oznew-quarter*(oznew!=0),quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(3,oxnew        ,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(3,oxnew+quarter,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll1));

			_meshCells[_meshCells.size()-3].lastBranch[1] = newBranchIndex+1;
			_meshCells[_meshCells.size()-2].lastBranch[1] = newBranchIndex+2;
			_meshCells[_meshCells.size()-1].lastBranch[1] = newBranchIndex+3;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;

			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_WALLWALL,4,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_WALLWALL,4,_meshCells.size()-1);
			if(oznew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,5,_meshCells.size()-3);
			if(oznew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,5,_meshCells.size()-2);
			if(oznew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,5,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(4,oxnew        ,oynew        ,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew        ,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(5,oxnew        ,oynew        ,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(5,oxnew        ,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1));

			_meshCells[_meshCells.size()-4].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[2] = newBranchIndex+0;
			_meshCells[_meshCells.size()-4].lastBranch[3] = newBranchIndex+2;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[2] = newBranchIndex+1;
			_meshCells[_meshCells.size()-3].lastBranch[3] = newBranchIndex+3;

			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[2] = newBranchIndex+0;
			_meshCells[_meshCells.size()-2].lastBranch[3] = newBranchIndex+1;

			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[2] = newBranchIndex+2;
			_meshCells[_meshCells.size()-1].lastBranch[3] = newBranchIndex+3;

			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-4);
			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-4);
			if(oznew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,5,_meshCells.size()-4);

			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE, 1,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE, 0,_meshCells.size()-3);
			if(oznew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,5,_meshCells.size()-3);

			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-2);
			if(oznew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,5,_meshCells.size()-2);

			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE, 5,_meshCells.size()-1);
			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE, 4,_meshCells.size()-1);
			if(oznew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,5,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew,oynew,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll0,ll0,ll1,ll1,ll1,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[1] = corner.lastBranch[2] = corner.lastBranch[3] = nb;
			corner.lastBranch[4] = newBranchIndex+0; corner.lastBranch[5] = newBranchIndex+1;
			corner.lastBranch[6] = newBranchIndex+2; corner.lastBranch[7] = newBranchIndex+3;

			_meshCellIndicesBranch[newBranchIndex+0].indexCorner[3] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+1].indexCorner[2] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+2].indexCorner[1] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+3].indexCorner[0] = _meshCells.size()-1;
			if(oznew!=0) _meshCellIndicesBranch[nb].add(VECTYPE_WALLCORN,5,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				//3 Walls
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-8);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-7);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-6);
				//4 Edges
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-5);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				//1 Middle
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oznew==0){
				_boundary.add(VECTYPE_WALLWALL,4,_meshCells.size()-8);
				_boundary.add(VECTYPE_WALLWALL,4,_meshCells.size()-7);
				_boundary.add(VECTYPE_WALLWALL,4,_meshCells.size()-6);
				_boundary.add(VECTYPE_WALLEDGE,4,_meshCells.size()-5);
				_boundary.add(VECTYPE_WALLEDGE,4,_meshCells.size()-4);
				_boundary.add(VECTYPE_WALLEDGE,4,_meshCells.size()-3);
				_boundary.add(VECTYPE_WALLEDGE,4,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,4,_meshCells.size()-1);
			}
		} else {
			if(cell.oz>=oznew) fprintf(stderr,"\nERROR MCNCompact: Cell Z offset too large: %i >= %i",cell.oz,oznew);
			volumetype cellchildindex = (cell.ox>=oxnew+quarter)*1 + (cell.oy>=oynew+quarter)*2;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_WALLWALL,4,index);
			cell.lastBranch[1] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_WALLWALL_5:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_WALLWALL,5,index);
			cell.ox = oxnew; cell.oy = oynew; cell.oz = oznew+quarter;
			cell.size = quarter; cell.lastBranch[0] = newBranchIndex+4;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[1];

			_meshCells.push_back(MeshCell(3,oxnew+quarter,oynew        ,oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(3,oxnew        ,oynew+quarter,oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(3,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1));

			_meshCells[_meshCells.size()-3].lastBranch[0] = newBranchIndex+5;
			_meshCells[_meshCells.size()-2].lastBranch[0] = newBranchIndex+6;
			_meshCells[_meshCells.size()-1].lastBranch[0] = newBranchIndex+7;

			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;

			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_WALLWALL,5,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_WALLWALL,5,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_WALLWALL,5,_meshCells.size()-1);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,4,_meshCells.size()-3);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,4,_meshCells.size()-2);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLWALL,4,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(4,oxnew        ,oynew        ,oznew+quarter,quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew        ,oznew+quarter,quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(5,oxnew        ,oynew        ,oznew+quarter,quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(5,oxnew        ,oynew+quarter,oznew+quarter,quarter,ll0,ll0,ll1,ll1));

			_meshCells[_meshCells.size()-4].lastBranch[0] = newBranchIndex+4;
			_meshCells[_meshCells.size()-4].lastBranch[1] = newBranchIndex+6;
			_meshCells[_meshCells.size()-4].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-3].lastBranch[0] = newBranchIndex+5;
			_meshCells[_meshCells.size()-3].lastBranch[1] = newBranchIndex+7;
			_meshCells[_meshCells.size()-3].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-2].lastBranch[0] = newBranchIndex+4;
			_meshCells[_meshCells.size()-2].lastBranch[1] = newBranchIndex+5;
			_meshCells[_meshCells.size()-2].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-1].lastBranch[0] = newBranchIndex+6;
			_meshCells[_meshCells.size()-1].lastBranch[1] = newBranchIndex+7;;
			_meshCells[_meshCells.size()-1].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[3] = nb;

			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-4);
			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-4);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,4,_meshCells.size()-4);

			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE, 3,_meshCells.size()-3);
			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE, 2,_meshCells.size()-3);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,4,_meshCells.size()-3);

			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-2);
			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-2);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,4,_meshCells.size()-2);

			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE, 7,_meshCells.size()-1);
			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE, 6,_meshCells.size()-1);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLEDGE,4,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew,oynew,oznew+quarter,quarter,ll0,ll0,ll0,ll0,ll1,ll1,ll1,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[4] = corner.lastBranch[5] = corner.lastBranch[6] = corner.lastBranch[7] = nb;
			corner.lastBranch[0] = newBranchIndex+4; corner.lastBranch[1] = newBranchIndex+5;
			corner.lastBranch[2] = newBranchIndex+6; corner.lastBranch[3] = newBranchIndex+7;

			_meshCellIndicesBranch[newBranchIndex+4].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+5].indexCorner[6] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+6].indexCorner[5] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+7].indexCorner[4] = _meshCells.size()-1;
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].add(VECTYPE_WALLCORN,4,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				//3 Walls
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-8);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-7);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-6);
				//4 Edges
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-5);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				//1 Middle
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oznew+half==_n){
				_boundary.add(VECTYPE_WALLWALL,5,_meshCells.size()-8);
				_boundary.add(VECTYPE_WALLWALL,5,_meshCells.size()-7);
				_boundary.add(VECTYPE_WALLWALL,5,_meshCells.size()-6);
				_boundary.add(VECTYPE_WALLEDGE,5,_meshCells.size()-5);
				_boundary.add(VECTYPE_WALLEDGE,5,_meshCells.size()-4);
				_boundary.add(VECTYPE_WALLEDGE,5,_meshCells.size()-3);
				_boundary.add(VECTYPE_WALLEDGE,5,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,5,_meshCells.size()-1);
			}
		} else {
			if(cell.oz+cell.size!=oznew+half)
				fprintf(stderr,"\nERROR MCNCompact: Cell Z Nr %li at %i (%i %i %i) does not fit: %i+%i vs %i+%i",
						index,child,oxnew,oynew,oznew,
						cell.oz,cell.size,oznew,half);
			volumetype cellchildindex = (cell.ox>=oxnew+quarter)*1 + (cell.oy>=oynew+quarter)*2 + 4;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_WALLWALL,5,index);
			cell.lastBranch[0] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_WALLEDGE_0:
		if (cell.size>quarter) {

		} else {
			volumetype shifty = (cell.oy>=oynew+quarter)*2;
			volumetype shiftz = (cell.oz>=oznew+quarter)*4;
			if(cell.type == 5){
				if(cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shifty+0].add(VECTYPE_EDGEEDGE, 6,index);
					_meshCellIndicesBranch[newBranchIndex+shifty+4].add(VECTYPE_EDGEEDGE, 4,index);
					cell.lastBranch[1] = newBranchIndex+shifty+0;
					cell.lastBranch[3] = newBranchIndex+shifty+4;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shifty+shiftz].add(VECTYPE_WALLEDGE,0,index);
					cell.lastBranch[1] = cell.lastBranch[3] = newBranchIndex+shifty+shiftz;
				}
			}
			else if(cell.type == 6){
				if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftz+0].add(VECTYPE_EDGEEDGE,10,index);
					_meshCellIndicesBranch[newBranchIndex+shiftz+2].add(VECTYPE_EDGEEDGE, 8,index);
					cell.lastBranch[1] = newBranchIndex+shiftz+0;
					cell.lastBranch[3] = newBranchIndex+shiftz+2;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shifty+shiftz].add(VECTYPE_WALLEDGE,0,index);
					cell.lastBranch[1] = cell.lastBranch[3] = newBranchIndex+shifty+shiftz;
				}
			}
			else{fprintf(stderr,"\nERROR MCNCompact: Wall of Type %i has Edges of Type %i",0,cell.type);}
		}
		break;
		case VECTYPE_WALLEDGE_1:
		if (cell.size>quarter) {

		} else {
			volumetype shifty = (cell.oy>=oynew+quarter)*2;
			volumetype shiftz = (cell.oz>=oznew+quarter)*4;
			if(cell.type == 5){
				if(cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shifty+1].add(VECTYPE_EDGEEDGE, 7,index);
					_meshCellIndicesBranch[newBranchIndex+shifty+5].add(VECTYPE_EDGEEDGE, 5,index);
					cell.lastBranch[0] = newBranchIndex+shifty+1;
					cell.lastBranch[2] = newBranchIndex+shifty+5;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shifty+shiftz+1].add(VECTYPE_WALLEDGE,1,index);
					cell.lastBranch[0] = cell.lastBranch[2] = newBranchIndex+shifty+shiftz+1;
				}
			}
			else if(cell.type == 6){
				if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftz+1].add(VECTYPE_EDGEEDGE,11,index);
					_meshCellIndicesBranch[newBranchIndex+shiftz+3].add(VECTYPE_EDGEEDGE, 9,index);
					cell.lastBranch[0] = newBranchIndex+shiftz+1;
					cell.lastBranch[2] = newBranchIndex+shiftz+3;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shifty+shiftz+1].add(VECTYPE_WALLEDGE,1,index);
					cell.lastBranch[0] = cell.lastBranch[2] = newBranchIndex+shifty+shiftz+1;
				}
			}
			else{fprintf(stderr,"\nERROR MCNCompact: Wall of Type %i has Edges of Type %i",1,cell.type);}
		}
		break;
		case VECTYPE_WALLEDGE_2:
		if (cell.size>quarter) {

		} else {
			volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
			volumetype shiftz = (cell.oz>=oznew+quarter)*4;
			if(cell.type == 4){
				if(cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftx+0].add(VECTYPE_EDGEEDGE, 2,index);
					_meshCellIndicesBranch[newBranchIndex+shiftx+4].add(VECTYPE_EDGEEDGE, 0,index);
					cell.lastBranch[1] = newBranchIndex+shiftx+0;
					cell.lastBranch[3] = newBranchIndex+shiftx+4;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shiftx+shiftz].add(VECTYPE_WALLEDGE,2,index);
					cell.lastBranch[1] = cell.lastBranch[3] = newBranchIndex+shiftx+shiftz;
				}
			}
			else if(cell.type == 6){
				if(cell.ox+cell.size==oxnew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftz+0].add(VECTYPE_EDGEEDGE, 9,index);
					_meshCellIndicesBranch[newBranchIndex+shiftz+1].add(VECTYPE_EDGEEDGE, 8,index);
					cell.lastBranch[2] = newBranchIndex+shiftz+0;
					cell.lastBranch[3] = newBranchIndex+shiftz+1;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shiftx+shiftz].add(VECTYPE_WALLEDGE,2,index);
					cell.lastBranch[2] = cell.lastBranch[3] = newBranchIndex+shiftx+shiftz;
				}
			}
			else{fprintf(stderr,"\nERROR MCNCompact: Wall of Type %i has Edges of Type %i",0,cell.type);}
		}
		break;
		case VECTYPE_WALLEDGE_3:
		if (cell.size>quarter) {

		} else {
			volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
			volumetype shiftz = (cell.oz>=oznew+quarter)*4;
			if(cell.type == 4){
				if(cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftx+2].add(VECTYPE_EDGEEDGE, 3,index);
					_meshCellIndicesBranch[newBranchIndex+shiftx+6].add(VECTYPE_EDGEEDGE, 1,index);
					cell.lastBranch[0] = newBranchIndex+shiftx+2;
					cell.lastBranch[2] = newBranchIndex+shiftx+6;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shiftx+shiftz+2].add(VECTYPE_WALLEDGE,3,index);
					cell.lastBranch[0] = cell.lastBranch[2] = newBranchIndex+shiftx+shiftz+2;
				}
			}
			else if(cell.type == 6){
				if(cell.ox+cell.size==oxnew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftz+2].add(VECTYPE_EDGEEDGE,11,index);
					_meshCellIndicesBranch[newBranchIndex+shiftz+3].add(VECTYPE_EDGEEDGE,10,index);
					cell.lastBranch[0] = newBranchIndex+shiftz+2;
					cell.lastBranch[1] = newBranchIndex+shiftz+3;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shiftx+shiftz+2].add(VECTYPE_WALLEDGE,3,index);
					cell.lastBranch[0] = cell.lastBranch[1] = newBranchIndex+shiftx+shiftz+2;
				}
			}
			else{fprintf(stderr,"\nERROR MCNCompact: Wall of Type %i has Edges of Type %i",0,cell.type);}
		}
		break;
		case VECTYPE_WALLEDGE_4:
		if (cell.size>quarter) {

		} else {
			volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
			volumetype shifty = (cell.oy>=oynew+quarter)*2;
			if(cell.type == 4){
				if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftx+0].add(VECTYPE_EDGEEDGE, 1,index);
					_meshCellIndicesBranch[newBranchIndex+shiftx+2].add(VECTYPE_EDGEEDGE, 0,index);
					cell.lastBranch[2] = newBranchIndex+shiftx+0;
					cell.lastBranch[3] = newBranchIndex+shiftx+2;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shiftx+shifty].add(VECTYPE_WALLEDGE,4,index);
					cell.lastBranch[2] = cell.lastBranch[3] = newBranchIndex+shiftx+shifty;
				}
			}
			else if(cell.type == 5){
				if(cell.ox+cell.size==oxnew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shifty+0].add(VECTYPE_EDGEEDGE, 5,index);
					_meshCellIndicesBranch[newBranchIndex+shifty+1].add(VECTYPE_EDGEEDGE, 4,index);
					cell.lastBranch[2] = newBranchIndex+shifty+0;
					cell.lastBranch[3] = newBranchIndex+shifty+1;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shiftx+shifty].add(VECTYPE_WALLEDGE,4,index);
					cell.lastBranch[2] = cell.lastBranch[3] = newBranchIndex+shiftx+shifty;
				}
			}
			else{fprintf(stderr,"\nERROR MCNCompact: Wall of Type %i has Edges of Type %i",0,cell.type);}
		}
		break;
		case VECTYPE_WALLEDGE_5:
		if (cell.size>quarter) {

		} else {
			volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
			volumetype shifty = (cell.oy>=oynew+quarter)*2;
			if(cell.type == 4){
				if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftx+4].add(VECTYPE_EDGEEDGE, 3,index);
					_meshCellIndicesBranch[newBranchIndex+shiftx+6].add(VECTYPE_EDGEEDGE, 2,index);
					cell.lastBranch[0] = newBranchIndex+shiftx+4;
					cell.lastBranch[1] = newBranchIndex+shiftx+6;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shiftx+shifty+4].add(VECTYPE_WALLEDGE,5,index);
					cell.lastBranch[0] = cell.lastBranch[1] = newBranchIndex+shiftx+shifty+4;
				}
			}
			else if(cell.type == 5){
				if(cell.ox+cell.size==oxnew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shifty+4].add(VECTYPE_EDGEEDGE, 7,index);
					_meshCellIndicesBranch[newBranchIndex+shifty+5].add(VECTYPE_EDGEEDGE, 6,index);
					cell.lastBranch[0] = newBranchIndex+shifty+4;
					cell.lastBranch[1] = newBranchIndex+shifty+5;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shiftx+shifty+4].add(VECTYPE_WALLEDGE,5,index);
					cell.lastBranch[0] = cell.lastBranch[1] = newBranchIndex+shiftx+shifty+4;
				}
			}
			else{fprintf(stderr,"\nERROR MCNCompact: Wall of Type %i has Edges of Type %i",0,cell.type);}
		}
		break;
		case VECTYPE_WALLCORN_0:
		if (cell.size>quarter) {

		} else {
			volumetype shifty = (cell.oy>=oynew+quarter)*2;
			volumetype shiftz = (cell.oz>=oznew+quarter)*4;
			if(cell.oy+cell.size==oynew+quarter && cell.oz+cell.size==oznew+quarter){
				_meshCellIndicesBranch[newBranchIndex+0].indexCorner[6] = index;
				_meshCellIndicesBranch[newBranchIndex+2].indexCorner[4] = index;
				_meshCellIndicesBranch[newBranchIndex+4].indexCorner[2] = index;
				_meshCellIndicesBranch[newBranchIndex+6].indexCorner[0] = index;
				cell.lastBranch[1] = newBranchIndex+0; cell.lastBranch[3] = newBranchIndex+2;
				cell.lastBranch[5] = newBranchIndex+4; cell.lastBranch[7] = newBranchIndex+6;
			}
			else{
				if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftz+0].add(VECTYPE_EDGECORN,10,index);
					_meshCellIndicesBranch[newBranchIndex+shiftz+2].add(VECTYPE_EDGECORN, 8,index);
					cell.lastBranch[1] = cell.lastBranch[5] = newBranchIndex+shiftz+0;
					cell.lastBranch[3] = cell.lastBranch[7] = newBranchIndex+shiftz+2;
				}
				else if(cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shifty+0].add(VECTYPE_EDGECORN, 6,index);
					_meshCellIndicesBranch[newBranchIndex+shifty+4].add(VECTYPE_EDGECORN, 4,index);
					cell.lastBranch[1] = cell.lastBranch[3] = newBranchIndex+shifty+0;
					cell.lastBranch[5] = cell.lastBranch[7] = newBranchIndex+shifty+4;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shifty+shiftz].add(VECTYPE_WALLCORN,0,index);
					cell.lastBranch[1] = cell.lastBranch[3] =
					cell.lastBranch[5] = cell.lastBranch[7] = newBranchIndex+shifty+shiftz;
				}
			}
		}
		break;
		case VECTYPE_WALLCORN_1:
		if (cell.size>quarter) {

		} else {
			volumetype shifty = (cell.oy>=oynew+quarter)*2;
			volumetype shiftz = (cell.oz>=oznew+quarter)*4;
			if(cell.oy+cell.size==oynew+quarter && cell.oz+cell.size==oznew+quarter){
				_meshCellIndicesBranch[newBranchIndex+1].indexCorner[7] = index;
				_meshCellIndicesBranch[newBranchIndex+3].indexCorner[5] = index;
				_meshCellIndicesBranch[newBranchIndex+5].indexCorner[3] = index;
				_meshCellIndicesBranch[newBranchIndex+7].indexCorner[1] = index;
				cell.lastBranch[0] = newBranchIndex+1; cell.lastBranch[2] = newBranchIndex+3;
				cell.lastBranch[4] = newBranchIndex+5; cell.lastBranch[6] = newBranchIndex+7;
			}
			else{
				if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftz+1].add(VECTYPE_EDGECORN,11,index);
					_meshCellIndicesBranch[newBranchIndex+shiftz+3].add(VECTYPE_EDGECORN, 9,index);
					cell.lastBranch[0] = cell.lastBranch[4] = newBranchIndex+shiftz+1;
					cell.lastBranch[2] = cell.lastBranch[6] = newBranchIndex+shiftz+3;
				}
				else if(cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shifty+1].add(VECTYPE_EDGECORN, 7,index);
					_meshCellIndicesBranch[newBranchIndex+shifty+5].add(VECTYPE_EDGECORN, 5,index);
					cell.lastBranch[0] = cell.lastBranch[2] = newBranchIndex+shifty+1;
					cell.lastBranch[4] = cell.lastBranch[6] = newBranchIndex+shifty+5;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shifty+shiftz+1].add(VECTYPE_WALLCORN,1,index);
					cell.lastBranch[0] = cell.lastBranch[2] =
					cell.lastBranch[4] = cell.lastBranch[6] = newBranchIndex+shifty+shiftz+1;
				}
			}
		}
		break;
		case VECTYPE_WALLCORN_2:
		if (cell.size>quarter) {

		} else {
			volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
			volumetype shiftz = (cell.oz>=oznew+quarter)*4;
			if(cell.ox+cell.size==oxnew+quarter && cell.oz+cell.size==oznew+quarter){
				_meshCellIndicesBranch[newBranchIndex+0].indexCorner[5] = index;
				_meshCellIndicesBranch[newBranchIndex+1].indexCorner[4] = index;
				_meshCellIndicesBranch[newBranchIndex+4].indexCorner[1] = index;
				_meshCellIndicesBranch[newBranchIndex+5].indexCorner[0] = index;
				cell.lastBranch[2] = newBranchIndex+0; cell.lastBranch[3] = newBranchIndex+1;
				cell.lastBranch[6] = newBranchIndex+4; cell.lastBranch[7] = newBranchIndex+5;
			}
			else{
				if(cell.ox+cell.size==oxnew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftz+0].add(VECTYPE_EDGECORN, 9,index);
					_meshCellIndicesBranch[newBranchIndex+shiftz+1].add(VECTYPE_EDGECORN, 8,index);
					cell.lastBranch[2] = cell.lastBranch[6] = newBranchIndex+shiftz+0;
					cell.lastBranch[3] = cell.lastBranch[7] = newBranchIndex+shiftz+1;
				}
				else if(cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftx+0].add(VECTYPE_EDGECORN, 2,index);
					_meshCellIndicesBranch[newBranchIndex+shiftx+4].add(VECTYPE_EDGECORN, 0,index);
					cell.lastBranch[2] = cell.lastBranch[3] = newBranchIndex+shiftx+0;
					cell.lastBranch[6] = cell.lastBranch[7] = newBranchIndex+shiftx+4;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shiftx+shiftz].add(VECTYPE_WALLCORN,2,index);
					cell.lastBranch[2] = cell.lastBranch[3] =
					cell.lastBranch[6] = cell.lastBranch[7] = newBranchIndex+shiftx+shiftz;
				}
			}
		}
		break;
		case VECTYPE_WALLCORN_3:
		if (cell.size>quarter) {

		} else {
			volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				if(cell.ox+cell.size==oxnew+quarter && cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[newBranchIndex+2].indexCorner[7] = index;
					_meshCellIndicesBranch[newBranchIndex+3].indexCorner[6] = index;
					_meshCellIndicesBranch[newBranchIndex+6].indexCorner[3] = index;
					_meshCellIndicesBranch[newBranchIndex+7].indexCorner[2] = index;
					cell.lastBranch[0] = newBranchIndex+2; cell.lastBranch[1] = newBranchIndex+3;
					cell.lastBranch[4] = newBranchIndex+6; cell.lastBranch[5] = newBranchIndex+7;
				}
				else{
					if(cell.ox+cell.size==oxnew+quarter){
						_meshCellIndicesBranch[newBranchIndex+shiftz+2].add(VECTYPE_EDGECORN,11,index);
						_meshCellIndicesBranch[newBranchIndex+shiftz+3].add(VECTYPE_EDGECORN,10,index);
						cell.lastBranch[0] = cell.lastBranch[4] = newBranchIndex+shiftz+2;
						cell.lastBranch[1] = cell.lastBranch[3] = newBranchIndex+shiftz+3;
					}
					else if(cell.oz+cell.size==oznew+quarter){
						_meshCellIndicesBranch[newBranchIndex+shiftx+2].add(VECTYPE_EDGECORN, 3,index);
						_meshCellIndicesBranch[newBranchIndex+shiftx+6].add(VECTYPE_EDGECORN, 1,index);
						cell.lastBranch[0] = cell.lastBranch[1] = newBranchIndex+shiftx+2;
						cell.lastBranch[4] = cell.lastBranch[5] = newBranchIndex+shiftx+6;
					}
					else{
						_meshCellIndicesBranch[newBranchIndex+shiftx+shiftz+2].add(VECTYPE_WALLCORN,3,index);
						cell.lastBranch[0] = cell.lastBranch[1] =
						cell.lastBranch[4] = cell.lastBranch[5] = newBranchIndex+shiftx+shiftz+2;
					}
				}
		}
		break;
		case VECTYPE_WALLCORN_4:
		if (cell.size>quarter) {

		} else {
			volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
			volumetype shifty = (cell.oy>=oynew+quarter)*2;
			if(cell.ox+cell.size==oxnew+quarter && cell.oy+cell.size==oynew+quarter){
				_meshCellIndicesBranch[newBranchIndex+0].indexCorner[3] = index;
				_meshCellIndicesBranch[newBranchIndex+1].indexCorner[2] = index;
				_meshCellIndicesBranch[newBranchIndex+2].indexCorner[1] = index;
				_meshCellIndicesBranch[newBranchIndex+3].indexCorner[0] = index;
				cell.lastBranch[4] = newBranchIndex+0; cell.lastBranch[5] = newBranchIndex+1;
				cell.lastBranch[6] = newBranchIndex+2; cell.lastBranch[7] = newBranchIndex+3;
			}
			else{
				if(cell.ox+cell.size==oxnew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shifty+0].add(VECTYPE_EDGECORN, 5,index);
					_meshCellIndicesBranch[newBranchIndex+shifty+1].add(VECTYPE_EDGECORN, 4,index);
					cell.lastBranch[4] = cell.lastBranch[6] = newBranchIndex+shifty+0;
					cell.lastBranch[5] = cell.lastBranch[7] = newBranchIndex+shifty+1;
				}
				else if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftx+0].add(VECTYPE_EDGECORN, 1,index);
					_meshCellIndicesBranch[newBranchIndex+shiftx+2].add(VECTYPE_EDGECORN, 0,index);
					cell.lastBranch[4] = cell.lastBranch[5] = newBranchIndex+shiftx+0;
					cell.lastBranch[6] = cell.lastBranch[7] = newBranchIndex+shiftx+2;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shiftx+shifty].add(VECTYPE_WALLCORN,4,index);
					cell.lastBranch[4] = cell.lastBranch[5] =
					cell.lastBranch[6] = cell.lastBranch[7] = newBranchIndex+shiftx+shifty;
				}
			}
		}
		break;
		case VECTYPE_WALLCORN_5:
		if (cell.size>quarter) {

		} else {
			volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
			volumetype shifty = (cell.oy>=oynew+quarter)*2;
			if(cell.ox+cell.size==oxnew+quarter && cell.oy+cell.size==oynew+quarter){
				_meshCellIndicesBranch[newBranchIndex+4].indexCorner[7] = index;
				_meshCellIndicesBranch[newBranchIndex+5].indexCorner[6] = index;
				_meshCellIndicesBranch[newBranchIndex+6].indexCorner[5] = index;
				_meshCellIndicesBranch[newBranchIndex+7].indexCorner[4] = index;
				cell.lastBranch[0] = newBranchIndex+4; cell.lastBranch[1] = newBranchIndex+5;
				cell.lastBranch[2] = newBranchIndex+6; cell.lastBranch[3] = newBranchIndex+7;
			}
			else{
				if(cell.ox+cell.size==oxnew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shifty+4].add(VECTYPE_EDGECORN, 7,index);
					_meshCellIndicesBranch[newBranchIndex+shifty+5].add(VECTYPE_EDGECORN, 6,index);
					cell.lastBranch[0] = cell.lastBranch[2] = newBranchIndex+shifty+4;
					cell.lastBranch[1] = cell.lastBranch[3] = newBranchIndex+shifty+5;
				}
				else if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[newBranchIndex+shiftx+4].add(VECTYPE_EDGECORN, 3,index);
					_meshCellIndicesBranch[newBranchIndex+shiftx+6].add(VECTYPE_EDGECORN, 2,index);
					cell.lastBranch[0] = cell.lastBranch[1] = newBranchIndex+shiftx+4;
					cell.lastBranch[2] = cell.lastBranch[3] = newBranchIndex+shiftx+6;
				}
				else{
					_meshCellIndicesBranch[newBranchIndex+shiftx+shifty+4].add(VECTYPE_WALLCORN,5,index);
					cell.lastBranch[4] = cell.lastBranch[5] =
					cell.lastBranch[6] = cell.lastBranch[7] = newBranchIndex+shiftx+shifty+4;
				}
			}
		}
		break;
		case VECTYPE_EDGEEDGE_00:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE,0,index);
			cell.ox = oxnew; cell.oy = oynew-quarter*(oynew!=0); cell.oz = oznew-quarter*(oznew!=0);
			cell.size = quarter; cell.lastBranch[3] = newBranchIndex+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];

			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew-quarter*(oynew!=0),oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE,0,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = newBranchIndex+1;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].add(VECTYPE_WALLEDGE,5,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].add(VECTYPE_EDGEEDGE,2,_meshCells.size()-1);
			}
			if(oynew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].add(VECTYPE_WALLEDGE,3,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].add(VECTYPE_EDGEEDGE,1,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_EDGEEDGE,3,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew,oynew-quarter*(oynew!=0),oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1,ll2,ll2,ll3,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[1] = nb0;
			corner.lastBranch[2] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = corner.lastBranch[5] = nb2;
			corner.lastBranch[6] = newBranchIndex+0;
			corner.lastBranch[7] = newBranchIndex+1;
			_meshCellIndicesBranch[newBranchIndex+0].indexCorner[1] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+1].indexCorner[0] = _meshCells.size()-1;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].add(VECTYPE_WALLCORN,5,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].add(VECTYPE_EDGECORN, 2,_meshCells.size()-1);
			}
			if(oynew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].add(VECTYPE_WALLCORN,3,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].add(VECTYPE_EDGECORN, 1,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_EDGECORN, 3,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oynew==0){
				if(oznew==0){
					_boundary.add(VECTYPE_EDGEEDGE,0,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN,0,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,2,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,2,_meshCells.size()-1);
				}
			}
			else if(oznew==0){
				_boundary.add(VECTYPE_WALLEDGE,4,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,4,_meshCells.size()-1);
			}
		} else {
			if((oynew && cell.oy>=oynew) || (oznew && cell.oz>=oznew))
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at X[0] offsets Y or Z too large: %i >= %i || %i >= %i",
						index,cell.oy,oynew,cell.oz,oznew);
			volumetype cellchildindex = (cell.ox>=oxnew+quarter);
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,0,index);
			cell.lastBranch[3] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGEEDGE_01:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE,1,index);
			cell.ox = oxnew; cell.oy = oynew+quarter; cell.oz = oznew-quarter*(oznew!=0);
			cell.size = quarter; cell.lastBranch[2] = newBranchIndex+2;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE,1,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = newBranchIndex+3;
			edge.lastBranch[3] = nb3;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLEDGE,5,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGEEDGE,3,_meshCells.size()-1);
			}
			if(oynew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLEDGE,2,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGEEDGE,0,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].add(VECTYPE_EDGEEDGE,2,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1,ll2,ll2,ll3,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[1] = nb0;
			corner.lastBranch[2] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = newBranchIndex+2;
			corner.lastBranch[5] = newBranchIndex+3;
			corner.lastBranch[6] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[newBranchIndex+2].indexCorner[3] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+3].indexCorner[2] = _meshCells.size()-1;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLCORN,5,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGECORN, 3,_meshCells.size()-1);
			}
			if(oynew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLCORN,2,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGECORN,0,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].add(VECTYPE_EDGECORN, 2,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oynew+half==_n){
				if(oznew==0){
					_boundary.add(VECTYPE_EDGEEDGE,1,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN, 1,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,3,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,3,_meshCells.size()-1);
				}
			}
			else if(oznew==0){
				_boundary.add(VECTYPE_WALLEDGE,4,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,4,_meshCells.size()-1);
			}
		} else {
			if(cell.oy+cell.size>oynew+half || (oznew && cell.oz>=oznew))
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at X[1] offsets Y or Z do not fit: %i+%i > %i+%i || %i >= %i",
						index,cell.oy,cell.size,oynew,half,cell.oz,oznew);
			volumetype cellchildindex = (cell.ox>=oxnew+quarter) + 2;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,1,index);
			cell.lastBranch[2] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGEEDGE_02:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE,2,index);
			cell.ox = oxnew; cell.oy = oynew-quarter*(oynew!=0); cell.oz = oznew+quarter;
			cell.size = quarter; cell.lastBranch[1] = newBranchIndex+4;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE,2,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = newBranchIndex+5;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oynew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLEDGE,3,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGEEDGE,3,_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLEDGE,4,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGEEDGE,0,_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].add(VECTYPE_EDGEEDGE,1,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll0,ll1,ll1,ll2,ll2,ll3,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[1] = nb0;
			corner.lastBranch[2] = newBranchIndex+4;
			corner.lastBranch[3] = newBranchIndex+5;
			corner.lastBranch[4] = corner.lastBranch[5] = nb2;
			corner.lastBranch[6] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[newBranchIndex+4].indexCorner[5] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+5].indexCorner[4] = _meshCells.size()-1;
			if(oynew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLCORN,3,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGECORN, 3,_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLCORN,4,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGECORN,0,_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].add(VECTYPE_EDGECORN, 1,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oynew==0){
				if(oznew+half==_n){
					_boundary.add(VECTYPE_EDGEEDGE,2,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN, 2,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,2,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,2,_meshCells.size()-1);
				}
			}
			else if(oznew+half==_n){
				_boundary.add(VECTYPE_WALLEDGE,5,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,5,_meshCells.size()-1);
			}
		} else {
			if((oynew && cell.oy>=oynew) || cell.oz+cell.size>oznew+half)
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at X[2] offsets Y or Z do not fit: %i >= %i || %i+%i > %i+%i",
						index,cell.oy,oynew,cell.oz,cell.size,oznew,half);
			volumetype cellchildindex = (cell.ox>=oxnew+quarter) + 4;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,2,index);
			cell.lastBranch[1] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGEEDGE_03:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE,3,index);
			cell.ox = oxnew; cell.oy = oynew+quarter; cell.oz = oznew+quarter;
			cell.size = quarter; cell.lastBranch[0] = newBranchIndex+6;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE,3,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = newBranchIndex+7;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oynew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb1].add(VECTYPE_WALLEDGE,2,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].add(VECTYPE_EDGEEDGE,2,_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb2].add(VECTYPE_WALLEDGE,4,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].add(VECTYPE_EDGEEDGE,1,_meshCells.size()-1);
			}
			if(nb1!=nb3 && nb2!=nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_EDGEEDGE,0,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew,oynew+quarter,oznew+quarter,quarter,ll0,ll0,ll1,ll1,ll2,ll2,ll3,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = newBranchIndex+6;
			corner.lastBranch[1] = newBranchIndex+7;
			corner.lastBranch[2] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = corner.lastBranch[5] = nb2;
			corner.lastBranch[6] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[newBranchIndex+6].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+7].indexCorner[6] = _meshCells.size()-1;
			if(oynew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb1].add(VECTYPE_WALLCORN,2,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].add(VECTYPE_EDGECORN, 2,_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb2].add(VECTYPE_WALLCORN,4,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].add(VECTYPE_EDGECORN, 1,_meshCells.size()-1);
			}
			if(nb1!=nb3 && nb2!=nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_EDGECORN,0,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oynew+half==_n){
				if(oznew+half==_n){
					_boundary.add(VECTYPE_EDGEEDGE,3,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN, 3,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,3,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,3,_meshCells.size()-1);
				}
			}
			else if(oznew+half==_n){
				_boundary.add(VECTYPE_WALLEDGE,5,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,5,_meshCells.size()-1);
			}
		} else {
			if(cell.oy+cell.size>oynew+half || cell.oz+cell.size>oznew+half)
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at X[3] offsets Y or Z do not fit: %i+%i > %i+%i || %i+%i > %i+%i",
						index,cell.oy,cell.size,oynew,half,cell.oz,cell.size,oznew,half);
			volumetype cellchildindex = (cell.ox>=oxnew+quarter) + 6;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,3,index);
			cell.lastBranch[0] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGEEDGE_04:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE,4,index);
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew; cell.oz = oznew-quarter*(oznew!=0);
			cell.size = quarter; cell.lastBranch[3] = newBranchIndex+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];

			_meshCells.push_back(MeshCell(5,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE,4,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = newBranchIndex+2;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].add(VECTYPE_WALLEDGE,5,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].add(VECTYPE_EDGEEDGE,6,_meshCells.size()-1);
			}
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].add(VECTYPE_WALLEDGE,1,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].add(VECTYPE_EDGEEDGE,5,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_EDGEEDGE,7,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew-quarter*(oxnew!=0),oynew,oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll0,ll1,ll2,ll3,ll2,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[2] = nb0;
			corner.lastBranch[1] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = corner.lastBranch[6] = nb2;
			corner.lastBranch[5] = newBranchIndex+0;
			corner.lastBranch[7] = newBranchIndex+2;
			_meshCellIndicesBranch[newBranchIndex+0].indexCorner[2] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+2].indexCorner[0] = _meshCells.size()-1;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].add(VECTYPE_WALLCORN,5,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].add(VECTYPE_EDGECORN, 6,_meshCells.size()-1);
			}
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].add(VECTYPE_WALLCORN,1,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].add(VECTYPE_EDGECORN, 5,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_EDGECORN, 7,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oxnew==0){
				if(oznew==0){
					_boundary.add(VECTYPE_EDGEEDGE,4,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN, 4,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,0,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,0,_meshCells.size()-1);
				}
			}
			else if(oznew==0){
				_boundary.add(VECTYPE_WALLEDGE,4,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,4,_meshCells.size()-1);
			}
		} else {
			if((oxnew && cell.ox>=oxnew) || (oznew && cell.oz>=oznew))
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at Y[0] offsets X or Z too large: %i >= %i || %i >= %i",
						index,cell.ox,oxnew,cell.oz,oznew);
			volumetype cellchildindex = (cell.oy>=oynew+quarter)*2;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,4,index);
			cell.lastBranch[3] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGEEDGE_05:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE,5,index);
			cell.ox = oxnew+quarter; cell.oy = oynew; cell.oz = oznew-quarter*(oznew!=0);
			cell.size = quarter; cell.lastBranch[2] = newBranchIndex+1;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(5,oxnew+quarter,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE,5,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = newBranchIndex+3;
			edge.lastBranch[3] = nb3;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLEDGE,5,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGEEDGE,7,_meshCells.size()-1);
			}
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLEDGE,0,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGEEDGE,4,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].add(VECTYPE_EDGEEDGE,6,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew+quarter,oynew,oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll0,ll1,ll2,ll3,ll2,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[2] = nb0;
			corner.lastBranch[1] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = newBranchIndex+1;
			corner.lastBranch[6] = newBranchIndex+3;
			corner.lastBranch[5] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[newBranchIndex+1].indexCorner[3] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+3].indexCorner[1] = _meshCells.size()-1;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLCORN,5,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGECORN, 7,_meshCells.size()-1);
			}
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLCORN,0,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGECORN, 4,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].add(VECTYPE_EDGECORN, 6,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oxnew+half==_n){
				if(oznew==0){
					_boundary.add(VECTYPE_EDGEEDGE,5,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN, 5,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,1,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,1,_meshCells.size()-1);
				}
			}
			else if(oznew==0){
				_boundary.add(VECTYPE_WALLEDGE,4,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,4,_meshCells.size()-1);
			}
		} else {
			if(cell.ox+cell.size>oxnew+half || (oznew && cell.oz>=oznew))
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at Y[1] offsets X or Z do not fit: %i+%i > %i+%i || %i >= %i",
						index,cell.ox,cell.size,oxnew,half,cell.oz,oznew);
			volumetype cellchildindex = (cell.oy>=oynew+quarter)*2 + 1;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,5,index);
			cell.lastBranch[2] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGEEDGE_06:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE,6,index);
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew; cell.oz = oznew+quarter;
			cell.size = quarter; cell.lastBranch[1] = newBranchIndex+4;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(5,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew+quarter,quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE,6,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = newBranchIndex+6;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLEDGE,1,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGEEDGE,7,_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLEDGE,4,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGEEDGE,4,_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].add(VECTYPE_EDGEEDGE,5,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew-quarter*(oxnew!=0),oynew,oznew+quarter,quarter,ll0,ll1,ll0,ll1,ll2,ll3,ll2,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[2] = nb0;
			corner.lastBranch[1] = newBranchIndex+4;
			corner.lastBranch[3] = newBranchIndex+6;
			corner.lastBranch[4] = corner.lastBranch[6] = nb2;
			corner.lastBranch[5] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[newBranchIndex+4].indexCorner[6] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+6].indexCorner[4] = _meshCells.size()-1;
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLCORN,1,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGECORN, 7,_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLCORN,4,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGECORN, 4,_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].add(VECTYPE_EDGECORN, 5,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oxnew==0){
				if(oznew+half==_n){
					_boundary.add(VECTYPE_EDGEEDGE,6,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN, 6,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,0,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,0,_meshCells.size()-1);
				}
			}
			else if(oznew+half==_n){
				_boundary.add(VECTYPE_WALLEDGE,5,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,5,_meshCells.size()-1);
			}
		} else {
			if((oxnew && cell.ox>=oxnew) || cell.oz+cell.size>oznew+half)
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at Y[2] offsets X or Z do not fit: %i >= %i || %i+%i > %i+%i",
						index,cell.ox,oxnew,cell.oz,cell.size,oznew,half);
			volumetype cellchildindex = (cell.oy>=oynew+quarter)*2 + 4;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,6,index);
			cell.lastBranch[1] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGEEDGE_07:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE,7,index);
			cell.ox = oxnew+quarter; cell.oy = oynew; cell.oz = oznew+quarter;
			cell.size = quarter; cell.lastBranch[0] = newBranchIndex+5;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(5,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE,7,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = newBranchIndex+7;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb1].add(VECTYPE_WALLEDGE,0,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].add(VECTYPE_EDGEEDGE,6,_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLEDGE,4,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].add(VECTYPE_EDGEEDGE,5,_meshCells.size()-1);
			}
			if(nb1!=nb3 && nb2!=nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_EDGEEDGE,4,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew+quarter,oynew,oznew+quarter,quarter,ll0,ll1,ll0,ll1,ll2,ll3,ll2,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = newBranchIndex+5;
			corner.lastBranch[2] = newBranchIndex+7;
			corner.lastBranch[1] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = corner.lastBranch[6] = nb2;
			corner.lastBranch[5] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[newBranchIndex+5].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+7].indexCorner[5] = _meshCells.size()-1;
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb1].add(VECTYPE_WALLCORN,0,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].add(VECTYPE_EDGECORN, 6,_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLCORN,4,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].add(VECTYPE_EDGECORN, 5,_meshCells.size()-1);
			}
			if(nb1!=nb3 && nb2!=nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_EDGECORN, 4,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oxnew+half==_n){
				if(oznew+half==_n){
					_boundary.add(VECTYPE_EDGEEDGE,7,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN, 7,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,1,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,1,_meshCells.size()-1);
				}
			}
			else if(oznew+half==_n){
				_boundary.add(VECTYPE_WALLEDGE,5,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,5,_meshCells.size()-1);
			}
		} else {
			if(cell.ox+cell.size>oxnew+half || cell.oz+cell.size>oznew+half)
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at Y[3] offsets X or Z do not fit: %i+%i > %i+%i || %i+%i > %i+%i",
						index,cell.ox,cell.size,oxnew,half,cell.oz,cell.size,oznew,half);
			volumetype cellchildindex = (cell.oy>=oynew+quarter)*2 + 5;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,7,index);
			cell.lastBranch[0] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGEEDGE_08:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+0].add(VECTYPE_EDGEEDGE,8,index);
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew-quarter*(oynew!=0); cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[3] = newBranchIndex+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];

			_meshCells.push_back(MeshCell(6,oxnew-quarter*(oxnew!=0),oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+4].add(VECTYPE_EDGEEDGE,8,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = newBranchIndex+4;
			if(oynew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].add(VECTYPE_WALLEDGE,3,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-1);
			}
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].add(VECTYPE_WALLEDGE,1,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].add(VECTYPE_EDGEEDGE,9,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew-quarter*(oxnew!=0),oynew-quarter*(oynew!=0),oznew,quarter,ll0,ll1,ll2,ll3,ll0,ll1,ll2,ll3));

			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[4] = nb0;
			corner.lastBranch[1] = corner.lastBranch[5] = nb1;
			corner.lastBranch[2] = corner.lastBranch[6] = nb2;
			corner.lastBranch[3] = newBranchIndex+0;
			corner.lastBranch[7] = newBranchIndex+4;
			_meshCellIndicesBranch[newBranchIndex+0].indexCorner[4] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+4].indexCorner[0] = _meshCells.size()-1;
			if(oynew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].add(VECTYPE_WALLCORN,3,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].add(VECTYPE_EDGECORN,10,_meshCells.size()-1);
			}
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].add(VECTYPE_WALLCORN,1,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].add(VECTYPE_EDGECORN, 9,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_EDGECORN,11,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oxnew==0){
				if(oynew==0){
					_boundary.add(VECTYPE_EDGEEDGE,8,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN, 8,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,0,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,0,_meshCells.size()-1);
				}
			}
			else if(oynew==0){
				_boundary.add(VECTYPE_WALLEDGE,2,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,2,_meshCells.size()-1);
			}
		} else {
			if((oxnew && cell.ox>=oxnew) || (oynew && cell.oy>=oynew))
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at Z[0] offsets X or Y too large: %i >= %i || %i >= %i",
						index,cell.ox,oxnew,cell.oy,oynew);
			volumetype cellchildindex = (cell.oz>=oznew+quarter)*4;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,8,index);
			cell.lastBranch[3] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGEEDGE_09:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+1].add(VECTYPE_EDGEEDGE,9,index);
			cell.ox = oxnew+quarter; cell.oy = oynew-quarter*(oynew!=0); cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[2] = newBranchIndex+1;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(6,oxnew+quarter,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+5].add(VECTYPE_EDGEEDGE,9,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = newBranchIndex+5;
			edge.lastBranch[3] = nb3;
			if(oynew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLEDGE,3,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-1);
			}
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLEDGE,0,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGEEDGE,8,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew+quarter,oynew-quarter*(oynew!=0),oznew,quarter,ll0,ll1,ll2,ll3,ll0,ll1,ll2,ll3));

			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[4] = nb0;
			corner.lastBranch[1] = corner.lastBranch[5] = nb1;
			corner.lastBranch[2] = newBranchIndex+1;
			corner.lastBranch[6] = newBranchIndex+5;
			corner.lastBranch[3] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[newBranchIndex+1].indexCorner[5] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+5].indexCorner[1] = _meshCells.size()-1;
			if(oynew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLCORN,3,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGECORN,11,_meshCells.size()-1);
			}
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLCORN,0,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGECORN, 8,_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].add(VECTYPE_EDGECORN,10,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oxnew+half==_n){
				if(oynew==0){
					_boundary.add(VECTYPE_EDGEEDGE,9,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN, 9,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,1,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,1,_meshCells.size()-1);
				}
			}
			else if(oynew==0){
				_boundary.add(VECTYPE_WALLEDGE,2,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,2,_meshCells.size()-1);
			}
		} else {
			if(cell.ox+cell.size>oxnew+half || (oynew && cell.oy>=oynew))
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at Z[1] offsets X or Y do not fit: %i+%i > %i+%i || %i >= %i",
						index,cell.ox,cell.size,oxnew,half,cell.oy,oynew);
			volumetype cellchildindex = (cell.oz>=oznew+quarter)*4 + 1;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,9,index);
			cell.lastBranch[2] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGEEDGE_10:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+2].add(VECTYPE_EDGEEDGE,10,index);
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew+quarter; cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[1] = newBranchIndex+2;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(6,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew+quarter,quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+6].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = newBranchIndex+6;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLEDGE,1,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-1);
			}
			if(oynew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLEDGE,2,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGEEDGE,8,_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].add(VECTYPE_EDGEEDGE,9,_meshCells.size()-1);

			_meshCells.push_back(MeshCell(7,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew,quarter,ll0,ll1,ll2,ll3,ll0,ll1,ll2,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[4] = nb0;
			corner.lastBranch[1] = newBranchIndex+2;
			corner.lastBranch[5] = newBranchIndex+6;
			corner.lastBranch[2] = corner.lastBranch[6] = nb2;
			corner.lastBranch[3] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[newBranchIndex+2].indexCorner[6] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+6].indexCorner[2] = _meshCells.size()-1;
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].add(VECTYPE_WALLCORN,1,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].add(VECTYPE_EDGECORN,11,_meshCells.size()-1);
			}
			if(oynew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].add(VECTYPE_WALLCORN,2,_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].add(VECTYPE_EDGECORN, 8,_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].add(VECTYPE_EDGECORN, 9,_meshCells.size()-1);

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oxnew==0){
				if(oynew+half==_n){
					_boundary.add(VECTYPE_EDGEEDGE,10,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN,10,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,0,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,0,_meshCells.size()-1);
				}
			}
			else if(oynew+half==_n){
				_boundary.add(VECTYPE_WALLEDGE,3,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,3,_meshCells.size()-1);
			}
		} else {
			if((oxnew && cell.ox>=oxnew) || cell.oy+cell.size>oynew+half)
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at Z[2] offsets X or Y do not fit: %i >= %i || %i+%i > %i+%i",
						index,cell.ox,oxnew,cell.oy,cell.size,oynew,half);
			volumetype cellchildindex = (cell.oz>=oznew+quarter)*4 + 2;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,10,index);
			cell.lastBranch[1] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGEEDGE_11:
		if (cell.size>quarter) {
			_meshCellIndicesBranch[newBranchIndex+3].add(VECTYPE_EDGEEDGE,11,index);
			cell.ox = oxnew+quarter; cell.oy = oynew+quarter; cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[0] = newBranchIndex+3;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(6,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1,ll2,ll3));

			_meshCellIndicesBranch[newBranchIndex+7].add(VECTYPE_EDGEEDGE,11,_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = newBranchIndex+7;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oxnew+half!=_n){
				if(nb1==nb3) { _meshCellIndicesBranch[nb1].add(VECTYPE_WALLEDGE,0,_meshCells.size()-1);}
				else         {_meshCellIndicesBranch[nb1].add(VECTYPE_EDGEEDGE,10,_meshCells.size()-1);}
			}
			if(oynew+half!=_n){
				if(nb2==nb3) {_meshCellIndicesBranch[nb2].add(VECTYPE_WALLEDGE,2,_meshCells.size()-1);}
				else         {_meshCellIndicesBranch[nb2].add(VECTYPE_EDGEEDGE,9,_meshCells.size()-1);}
			}
			if(nb1!=nb3 && nb2!=nb3) {_meshCellIndicesBranch[nb3].add(VECTYPE_EDGEEDGE,8,_meshCells.size()-1);}

			_meshCells.push_back(MeshCell(7,oxnew+quarter,oynew+quarter,oznew,quarter,ll0,ll1,ll2,ll3,ll0,ll1,ll2,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = newBranchIndex+3;
			corner.lastBranch[4] = newBranchIndex+7;
			corner.lastBranch[1] = corner.lastBranch[5] = nb1;
			corner.lastBranch[2] = corner.lastBranch[6] = nb2;
			corner.lastBranch[3] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[newBranchIndex+3].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[newBranchIndex+7].indexCorner[3] = _meshCells.size()-1;
			if(oxnew+half!=_n){
				if(nb1==nb3) {_meshCellIndicesBranch[nb1].add(VECTYPE_WALLCORN,0,_meshCells.size()-1);}
				else         {_meshCellIndicesBranch[nb1].add(VECTYPE_EDGECORN,10,_meshCells.size()-1);}
			}
			if(oynew+half!=_n){
				if(nb2==nb3) {_meshCellIndicesBranch[nb2].add(VECTYPE_WALLCORN,2,_meshCells.size()-1);}
				else         {_meshCellIndicesBranch[nb2].add(VECTYPE_EDGECORN, 9,_meshCells.size()-1);}
			}
			if(nb1!=nb3 && nb2!=nb3) {_meshCellIndicesBranch[nb3].add(VECTYPE_EDGECORN, 8,_meshCells.size()-1);}

#ifndef DEBUG_NO_LEAFCELLS
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}
#endif

			if(oxnew+half==_n){
				if(oynew+half==_n){
					_boundary.add(VECTYPE_EDGEEDGE,11,_meshCells.size()-2);
					_boundary.add(VECTYPE_EDGECORN,11,_meshCells.size()-1);
				}
				else{
					_boundary.add(VECTYPE_WALLEDGE,1,_meshCells.size()-2);
					_boundary.add(VECTYPE_WALLCORN,1,_meshCells.size()-1);
				}
			}
			else if(oynew+half==_n){
				_boundary.add(VECTYPE_WALLEDGE,3,_meshCells.size()-2);
				_boundary.add(VECTYPE_WALLCORN,3,_meshCells.size()-1);
			}
		} else {
			if(cell.ox+cell.size>oxnew+half || cell.oy+cell.size>oynew+half)
				fprintf(stderr,"\nERROR MCNCompact: Edge Cell %li at Z[3] offsets X or Y do not fit: %i+%i > %i+%i || %i+%i > %i+%i",
						index,cell.ox,cell.size,oxnew,half,cell.oy,cell.size,oynew,half);
			volumetype cellchildindex = (cell.oz>=oznew+quarter)*4 + 3;
			_meshCellIndicesBranch[newBranchIndex+cellchildindex].add(VECTYPE_EDGEEDGE,11,index);
			cell.lastBranch[0] = newBranchIndex+cellchildindex;
		}
		break;
		case VECTYPE_EDGECORN_00:
		if (cell.size>quarter) {

		} else {
			if(cell.ox+cell.size==oxnew+quarter){
				_meshCellIndicesBranch[newBranchIndex+0].indexCorner[1] = index;
				_meshCellIndicesBranch[newBranchIndex+1].indexCorner[0] = index;
				cell.lastBranch[6] = newBranchIndex+0; cell.lastBranch[7] = newBranchIndex+1;
			}
			else{
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				_meshCellIndicesBranch[newBranchIndex+shiftx].add(VECTYPE_EDGECORN,0,index);
				cell.lastBranch[6] = cell.lastBranch[7] = newBranchIndex+shiftx;
			}
		}
		break;
		case VECTYPE_EDGECORN_01:
		if (cell.size>quarter) {

		} else {
			if(cell.ox+cell.size==oxnew+quarter){
				_meshCellIndicesBranch[newBranchIndex+2].indexCorner[3] = index;
				_meshCellIndicesBranch[newBranchIndex+3].indexCorner[2] = index;
				cell.lastBranch[4] = newBranchIndex+2; cell.lastBranch[5] = newBranchIndex+3;
			}
			else{
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				_meshCellIndicesBranch[newBranchIndex+shiftx+2].add(VECTYPE_EDGECORN, 1,index);
				cell.lastBranch[4] = cell.lastBranch[5] = newBranchIndex+shiftx+2;
			}
		}
		break;
		case VECTYPE_EDGECORN_02:
		if (cell.size>quarter) {

		} else {
			if(cell.ox+cell.size==oxnew+quarter){
				_meshCellIndicesBranch[newBranchIndex+4].indexCorner[5] = index;
				_meshCellIndicesBranch[newBranchIndex+5].indexCorner[4] = index;
				cell.lastBranch[2] = newBranchIndex+4; cell.lastBranch[3] = newBranchIndex+5;
			}
			else{
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				_meshCellIndicesBranch[newBranchIndex+shiftx+4].add(VECTYPE_EDGECORN, 2,index);
				cell.lastBranch[2] = cell.lastBranch[3] = newBranchIndex+shiftx+4;
			}
		}
		break;
		case VECTYPE_EDGECORN_03:
		if (cell.size>quarter) {

		} else {
			if(cell.ox+cell.size==oxnew+quarter){
				_meshCellIndicesBranch[newBranchIndex+6].indexCorner[7] = index;
				_meshCellIndicesBranch[newBranchIndex+7].indexCorner[6] = index;
				cell.lastBranch[0] = newBranchIndex+6; cell.lastBranch[1] = newBranchIndex+7;
			}
			else{
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				_meshCellIndicesBranch[newBranchIndex+shiftx+6].add(VECTYPE_EDGECORN, 3,index);
				cell.lastBranch[0] = cell.lastBranch[1] = newBranchIndex+shiftx+6;
			}
		}
		break;
		case VECTYPE_EDGECORN_04:
		if (cell.size>quarter) {

		} else {
			if(cell.oy+cell.size==oynew+quarter){
				_meshCellIndicesBranch[newBranchIndex+0].indexCorner[2] = index;
				_meshCellIndicesBranch[newBranchIndex+2].indexCorner[0] = index;
				cell.lastBranch[5] = newBranchIndex+0; cell.lastBranch[7] = newBranchIndex+2;
			}
			else{
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				_meshCellIndicesBranch[newBranchIndex+shifty].add(VECTYPE_EDGECORN, 4,index);
				cell.lastBranch[5] = cell.lastBranch[7] = newBranchIndex+shifty;
			}
		}
		break;
		case VECTYPE_EDGECORN_05:
		if (cell.size>quarter) {

		} else {
			if(cell.oy+cell.size==oynew+quarter){
				_meshCellIndicesBranch[newBranchIndex+1].indexCorner[3] = index;
				_meshCellIndicesBranch[newBranchIndex+3].indexCorner[1] = index;
				cell.lastBranch[4] = newBranchIndex+1; cell.lastBranch[6] = newBranchIndex+3;
			}
			else{
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				_meshCellIndicesBranch[newBranchIndex+shifty+1].add(VECTYPE_EDGECORN, 5,index);
				cell.lastBranch[4] = cell.lastBranch[6] = newBranchIndex+shifty+1;
			}
		}
		break;
		case VECTYPE_EDGECORN_06:
		if (cell.size>quarter) {

		} else {
			if(cell.oy+cell.size==oynew+quarter){
				_meshCellIndicesBranch[newBranchIndex+4].indexCorner[6] = index;
				_meshCellIndicesBranch[newBranchIndex+6].indexCorner[4] = index;
				cell.lastBranch[1] = newBranchIndex+4; cell.lastBranch[3] = newBranchIndex+6;
			}
			else{
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				_meshCellIndicesBranch[newBranchIndex+shifty+4].add(VECTYPE_EDGECORN, 6,index);
				cell.lastBranch[1] = cell.lastBranch[3] = newBranchIndex+shifty+4;
			}
		}
		break;
		case VECTYPE_EDGECORN_07:
		if (cell.size>quarter) {

		} else {
			if(cell.oy+cell.size==oynew+quarter){
				_meshCellIndicesBranch[newBranchIndex+5].indexCorner[7] = index;
				_meshCellIndicesBranch[newBranchIndex+7].indexCorner[5] = index;
				cell.lastBranch[0] = newBranchIndex+5; cell.lastBranch[2] = newBranchIndex+7;
			}
			else{
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				_meshCellIndicesBranch[newBranchIndex+shifty+5].add(VECTYPE_EDGECORN, 7,index);
				cell.lastBranch[0] = cell.lastBranch[2] = newBranchIndex+shifty+5;
			}
		}
		break;
		case VECTYPE_EDGECORN_08:
		if (cell.size>quarter) {

		} else {
			if(cell.oz+cell.size==oznew+quarter){
				_meshCellIndicesBranch[newBranchIndex+0].indexCorner[4] = index;
				_meshCellIndicesBranch[newBranchIndex+4].indexCorner[0] = index;
				cell.lastBranch[3] = newBranchIndex+0; cell.lastBranch[7] = newBranchIndex+4;
			}
			else{
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				_meshCellIndicesBranch[newBranchIndex+shiftz].add(VECTYPE_EDGECORN, 8,index);
				cell.lastBranch[3] = cell.lastBranch[7] = newBranchIndex+shiftz;
			}
		}
		break;
		case VECTYPE_EDGECORN_09:
		if (cell.size>quarter) {

		} else {
			if(cell.oz+cell.size==oznew+quarter){
				_meshCellIndicesBranch[newBranchIndex+1].indexCorner[5] = index;
				_meshCellIndicesBranch[newBranchIndex+5].indexCorner[1] = index;
				cell.lastBranch[2] = newBranchIndex+1; cell.lastBranch[6] = newBranchIndex+5;
			}
			else{
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				_meshCellIndicesBranch[newBranchIndex+shiftz+1].add(VECTYPE_EDGECORN, 9,index);
				cell.lastBranch[2] = cell.lastBranch[6] = newBranchIndex+shiftz+1;
			}
		}
		break;
		case VECTYPE_EDGECORN_10:
		if (cell.size>quarter) {

		} else {
			if(cell.oz+cell.size==oznew+quarter){
				_meshCellIndicesBranch[newBranchIndex+2].indexCorner[6] = index;
				_meshCellIndicesBranch[newBranchIndex+6].indexCorner[2] = index;
				cell.lastBranch[1] = newBranchIndex+2; cell.lastBranch[5] = newBranchIndex+6;
			}
			else{
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				_meshCellIndicesBranch[newBranchIndex+shiftz+2].add(VECTYPE_EDGECORN,10,index);
				cell.lastBranch[1] = cell.lastBranch[5] = newBranchIndex+shiftz+2;
			}
		}
		break;
		case VECTYPE_EDGECORN_11:
		if (cell.size>quarter) {

		} else {
			if(cell.oz+cell.size==oznew+quarter){
				_meshCellIndicesBranch[newBranchIndex+3].indexCorner[7] = index;
				_meshCellIndicesBranch[newBranchIndex+7].indexCorner[3] = index;
				cell.lastBranch[0] = newBranchIndex+3; cell.lastBranch[4] = newBranchIndex+7;
			}
			else{
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				_meshCellIndicesBranch[newBranchIndex+shiftz+3].add(VECTYPE_EDGECORN,11,index);
				cell.lastBranch[0] = cell.lastBranch[4] = newBranchIndex+shiftz+3;
			}
		}
		break;
		}
	}

//	_meshCellIndicesBranch[child].cells.clear();
	_meshCellIndicesBranch[child].clear();



//			fprintf(stderr,"C");
	//Corner
	_meshCellIndicesBranch[newBranchIndex+0].indexCorner[0] = _meshCellIndicesBranch[child].indexCorner[0];
	_meshCellIndicesBranch[newBranchIndex+1].indexCorner[1] = _meshCellIndicesBranch[child].indexCorner[1];
	_meshCellIndicesBranch[newBranchIndex+2].indexCorner[2] = _meshCellIndicesBranch[child].indexCorner[2];
	_meshCellIndicesBranch[newBranchIndex+3].indexCorner[3] = _meshCellIndicesBranch[child].indexCorner[3];
	_meshCellIndicesBranch[newBranchIndex+4].indexCorner[4] = _meshCellIndicesBranch[child].indexCorner[4];
	_meshCellIndicesBranch[newBranchIndex+5].indexCorner[5] = _meshCellIndicesBranch[child].indexCorner[5];
	_meshCellIndicesBranch[newBranchIndex+6].indexCorner[6] = _meshCellIndicesBranch[child].indexCorner[6];
	_meshCellIndicesBranch[newBranchIndex+7].indexCorner[7] = _meshCellIndicesBranch[child].indexCorner[7];


	_meshCellIndicesBranch[child].indexCorner[0] = MAXCELLINDEX;
	_meshCellIndicesBranch[child].indexCorner[1] = MAXCELLINDEX;
	_meshCellIndicesBranch[child].indexCorner[2] = MAXCELLINDEX;
	_meshCellIndicesBranch[child].indexCorner[3] = MAXCELLINDEX;
	_meshCellIndicesBranch[child].indexCorner[4] = MAXCELLINDEX;
	_meshCellIndicesBranch[child].indexCorner[5] = MAXCELLINDEX;
	_meshCellIndicesBranch[child].indexCorner[6] = MAXCELLINDEX;
	_meshCellIndicesBranch[child].indexCorner[7] = MAXCELLINDEX;

	if(_meshCellIndicesBranch[newBranchIndex+0].indexCorner[0]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[newBranchIndex+0].indexCorner[0]];
		cell.lastBranch[7] = newBranchIndex+0;
		if(cell.size>quarter){
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew-quarter*(oynew!=0);
			cell.oz = oznew-quarter*(oznew!=0); cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[newBranchIndex+1].indexCorner[1]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[newBranchIndex+1].indexCorner[1]];
		cell.lastBranch[6] = newBranchIndex+1;
		if(cell.size>quarter){
			cell.ox = oxnew+quarter; cell.oy = oynew-quarter*(oynew!=0);
			cell.oz = oznew-quarter*(oznew!=0); cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[newBranchIndex+2].indexCorner[2]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[newBranchIndex+2].indexCorner[2]];
		cell.lastBranch[5] = newBranchIndex+2;
		if(cell.size>quarter){
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew+quarter;
			cell.oz = oznew-quarter*(oznew!=0); cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[newBranchIndex+3].indexCorner[3]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[newBranchIndex+3].indexCorner[3]];
		cell.lastBranch[4] = newBranchIndex+3;
		if(cell.size>quarter){
			cell.ox = oxnew+quarter; cell.oy = oynew+quarter;
			cell.oz = oznew-quarter*(oznew!=0); cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[newBranchIndex+4].indexCorner[4]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[newBranchIndex+4].indexCorner[4]];
		cell.lastBranch[3] = newBranchIndex+4;
		if(cell.size>quarter){
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew-quarter*(oynew!=0);
			cell.oz = oznew+quarter; cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[newBranchIndex+5].indexCorner[5]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[newBranchIndex+5].indexCorner[5]];
		cell.lastBranch[2] = newBranchIndex+5;
		if(cell.size>quarter){
			cell.ox = oxnew+quarter; cell.oy = oynew-quarter*(oynew!=0);
			cell.oz = oznew+quarter; cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[newBranchIndex+6].indexCorner[6]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[newBranchIndex+6].indexCorner[6]];
		cell.lastBranch[1] = newBranchIndex+6;
		if(cell.size>quarter){
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew+quarter;
			cell.oz = oznew+quarter; cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[newBranchIndex+7].indexCorner[7]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[newBranchIndex+7].indexCorner[7]];
		cell.lastBranch[0] = newBranchIndex+7;
		if(cell.size>quarter){
			cell.ox = oxnew+quarter; cell.oy = oynew+quarter;
			cell.oz = oznew+quarter; cell.size = quarter;
		}
	}

//	for(size_t i=0;i<8;i++){
//		fprintf(stderr,"\nNew compact Branch Indices %li",newBranchIndex+i);
//		MeshCellNeighborhood temp = _meshCellIndicesBranch[newBranchIndex+i];
//		temp.print();
//	}
//	fprintf(stderr,"\nDEBUG: Compact Branch Index %li",16);
//	MeshCellNeighborhood temp = _meshCellIndicesBranch[16];
//	temp.print();



//	fprintf(stderr,"\nCompact MeshCell Structures for Subbranch created.");
}

inline void createMeshCellStructuresForLeaf
(
		volumetype leaf,
		MCNCompact &branchCells,
		FusionMipMapCPU::LeafNeighborhood &meshCellIndicesLeafSingle,
		FusionMipMapCPU::MeshCellArray &_meshCells
//		,const sidetype3 *_leafPos
		,const sidetype *_leafScale
)
{
	size_t &subinteriorindex = branchCells.indexInterior;
	MeshCell &interiorcell = _meshCells[subinteriorindex];
	meshCellIndicesLeafSingle.push_back(subinteriorindex);
	if(interiorcell.lastLeaf[0]>=BRANCHINIT || _leafScale[interiorcell.lastLeaf[0]]>_leafScale[leaf]){
		interiorcell.lastLeaf[0] = leaf;
	}

//	for(size_t i=0;i<branchCells.cells.size();i++){
//		size_t index = branchCells.cells[i].first;
//		MCNCompact::vecpair identifier = MCNCompact::decnum(branchCells.cells[i].second);
	for(size_t i=0;i<branchCells.border_size();i++){
		MCNCompact::CellReferenceNum ref = branchCells.get(i);
		size_t index = ref.second;
		MCNCompact::vecpair identifier = MCNCompact::decnum(ref.first);
		MeshCell &cell = _meshCells[index];
		int j = identifier.vecindex;
		int e = j%4;
		//TODO: switch
		if(identifier.vectype == VECTYPE_WALLWALL){
			volumetype &lastleaf = (j&1) ? cell.lastLeaf[0] : cell.lastLeaf[1];
			if(lastleaf>=BRANCHINIT || _leafScale[lastleaf]>_leafScale[leaf]){
				lastleaf = leaf;
			}
		}
		if(identifier.vectype == VECTYPE_WALLEDGE){
			volumetype *lastleaf0 = NULL; volumetype *lastleaf1 = NULL;
			switch(j){
			case 0 : lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3]; break;
			case 1 : lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2]; break;
			case 2 : if(cell.type==4){lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3];}
				                   else{lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3];}break;
			case 3 : if(cell.type==4){lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2];}
				                   else{lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1];}break;
			case 4 : lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3]; break;
			case 5 : lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1]; break;
			}
			if(*lastleaf0>=BRANCHINIT || _leafScale[*lastleaf0]>_leafScale[leaf]){
				*lastleaf0 = *lastleaf1 = leaf;
			}
		}
		if(identifier.vectype == VECTYPE_WALLCORN){
			volumetype *lastleaf0 = NULL; volumetype *lastleaf1 = NULL;
			volumetype *lastleaf2 = NULL; volumetype *lastleaf3 = NULL;
			switch(j){
				case 0:
				lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3];
				lastleaf2 = &cell.lastLeaf[5]; lastleaf3 = &cell.lastLeaf[7];
				break;
				case 1:
				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2];
				lastleaf2 = &cell.lastLeaf[4]; lastleaf3 = &cell.lastLeaf[6];
				break;
				case 2:
				lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3];
				lastleaf2 = &cell.lastLeaf[6]; lastleaf3 = &cell.lastLeaf[7];
				break;
				case 3:
				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1];
				lastleaf2 = &cell.lastLeaf[4]; lastleaf3 = &cell.lastLeaf[5];
				break;
				case 4:
				lastleaf0 = &cell.lastLeaf[4]; lastleaf1 = &cell.lastLeaf[5];
				lastleaf2 = &cell.lastLeaf[6]; lastleaf3 = &cell.lastLeaf[7];
				break;
				case 5:
				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1];
				lastleaf2 = &cell.lastLeaf[2]; lastleaf3 = &cell.lastLeaf[3];
				break;
			}
			if(*lastleaf0>=BRANCHINIT || _leafScale[*lastleaf0]>_leafScale[leaf]){
				*lastleaf0 = *lastleaf1 = *lastleaf2 = *lastleaf3 = leaf;
			}
		}
		if(identifier.vectype == VECTYPE_EDGEEDGE){
			volumetype &lastleaf = (e&2) ?
			((e&1) ? cell.lastLeaf[0] : cell.lastLeaf[1]) :
			((e&1) ? cell.lastLeaf[2] : cell.lastLeaf[3]);
			if(lastleaf>=BRANCHINIT || _leafScale[lastleaf]>_leafScale[leaf]){
				lastleaf = leaf;
			}
		}
		if(identifier.vectype == VECTYPE_EDGECORN){
			volumetype *lastleaf0 = NULL; volumetype *lastleaf1 = NULL;
			switch(j){
				case 0:
				lastleaf0 = &cell.lastLeaf[6]; lastleaf1 = &cell.lastLeaf[7]; break;
				case 1:
				lastleaf0 = &cell.lastLeaf[4]; lastleaf1 = &cell.lastLeaf[5]; break;
				case 2:
				lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3]; break;
				case 3:
				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1]; break;
				case 4:
				lastleaf0 = &cell.lastLeaf[5]; lastleaf1 = &cell.lastLeaf[7]; break;
				case 5:
				lastleaf0 = &cell.lastLeaf[4]; lastleaf1 = &cell.lastLeaf[6]; break;
				case 6:
				lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3]; break;
				case 7:
				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2]; break;
				case 8:
				lastleaf0 = &cell.lastLeaf[3]; lastleaf1 = &cell.lastLeaf[7]; break;
				case 9:
				lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[6]; break;
				case 10:
				lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[5]; break;
				case 11:
				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[4]; break;
			}
			if(*lastleaf0>=BRANCHINIT || _leafScale[*lastleaf0]>_leafScale[leaf]){
				*lastleaf0 = *lastleaf1 = leaf;
			}
		}
		meshCellIndicesLeafSingle.push_back(index);
	}

//	for(int j=0;j<6;j++){
//		MeshCellNeighborhood::CellList &vectorWallWall = branchCells.indexWallWall[j];
//		MeshCellNeighborhood::CellList &vectorWallEdge = branchCells.indexWallEdge[j];
//		MeshCellNeighborhood::CellList &vectorWallCorner = branchCells.indexWallCorner[j];
//		for(MeshCellNeighborhood::CellList::iterator i=vectorWallWall.begin();i!=vectorWallWall.end();i++){
//			MeshCell &cell = _meshCells[*i];
//			volumetype &lastleaf = (j&1) ? cell.lastLeaf[0] : cell.lastLeaf[1];
//			if(lastleaf>=BRANCHINIT || _leafScale[lastleaf]>_leafScale[leaf]){
//				lastleaf = leaf;
//			}
//			meshCellIndicesLeafSingle.push_back(*i);
//		}
//		for(MeshCellNeighborhood::CellList::iterator i=vectorWallEdge.begin();i!=vectorWallEdge.end();i++){
//			MeshCell &cell = _meshCells[*i];
//			volumetype *lastleaf0 = NULL; volumetype *lastleaf1 = NULL;
//			switch(j){
//			case 0 : lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3]; break;
//			case 1 : lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2]; break;
//			case 2 : if(cell.type==4){lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3];}
//				                   else{lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3];}break;
//			case 3 : if(cell.type==4){lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2];}
//				                   else{lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1];}break;
//			case 4 : lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3]; break;
//			case 5 : lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1]; break;
//			}
//			if(*lastleaf0>=BRANCHINIT || _leafScale[*lastleaf0]>_leafScale[leaf]){
//				*lastleaf0 = *lastleaf1 = leaf;
//			}
//			meshCellIndicesLeafSingle.push_back(*i);
//		}
//		for(MeshCellNeighborhood::CellList::iterator i=vectorWallCorner.begin();i!=vectorWallCorner.end();i++){
//			MeshCell &cell = _meshCells[*i];
//			volumetype *lastleaf0 = NULL; volumetype *lastleaf1 = NULL;
//			volumetype *lastleaf2 = NULL; volumetype *lastleaf3 = NULL;
//			switch(j){
//				case 0:
//				lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3];
//				lastleaf2 = &cell.lastLeaf[5]; lastleaf3 = &cell.lastLeaf[7];
//				break;
//				case 1:
//				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2];
//				lastleaf2 = &cell.lastLeaf[4]; lastleaf3 = &cell.lastLeaf[6];
//				break;
//				case 2:
//				lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3];
//				lastleaf2 = &cell.lastLeaf[6]; lastleaf3 = &cell.lastLeaf[7];
//				break;
//				case 3:
//				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1];
//				lastleaf2 = &cell.lastLeaf[4]; lastleaf3 = &cell.lastLeaf[5];
//				break;
//				case 4:
//				lastleaf0 = &cell.lastLeaf[4]; lastleaf1 = &cell.lastLeaf[5];
//				lastleaf2 = &cell.lastLeaf[6]; lastleaf3 = &cell.lastLeaf[7];
//				break;
//				case 5:
//				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1];
//				lastleaf2 = &cell.lastLeaf[2]; lastleaf3 = &cell.lastLeaf[3];
//				break;
//			}
//			if(*lastleaf0>=BRANCHINIT || _leafScale[*lastleaf0]>_leafScale[leaf]){
//				*lastleaf0 = *lastleaf1 = *lastleaf2 = *lastleaf3 = leaf;
//			}
//			meshCellIndicesLeafSingle.push_back(*i);
//		}
//	}
//
//	for(int j=0;j<12;j++){
//		int e = j%4;
//		MeshCellNeighborhood::CellList &vectorEdgeEdge = branchCells.indexEdgeEdge[j];
//		MeshCellNeighborhood::CellList &vectorEdgeCorner = branchCells.indexEdgeCorner[j];
//		for(MeshCellNeighborhood::CellList::iterator i=vectorEdgeEdge.begin();i!=vectorEdgeEdge.end();i++){
//			MeshCell &cell = _meshCells[*i];
//			volumetype &lastleaf = (e&2) ?
//			((e&1) ? cell.lastLeaf[0] : cell.lastLeaf[1]) :
//			((e&1) ? cell.lastLeaf[2] : cell.lastLeaf[3]);
//			if(lastleaf>=BRANCHINIT || _leafScale[lastleaf]>_leafScale[leaf]){
//				lastleaf = leaf;
//			}
//			meshCellIndicesLeafSingle.push_back(*i);
//		}
//		for(MeshCellNeighborhood::CellList::iterator i=vectorEdgeCorner.begin();i!=vectorEdgeCorner.end();i++){
//			MeshCell &cell = _meshCells[*i];
//			volumetype *lastleaf0 = NULL; volumetype *lastleaf1 = NULL;
//			switch(j){
//				case 0:
//				lastleaf0 = &cell.lastLeaf[6]; lastleaf1 = &cell.lastLeaf[7]; break;
//				case 1:
//				lastleaf0 = &cell.lastLeaf[4]; lastleaf1 = &cell.lastLeaf[5]; break;
//				case 2:
//				lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[3]; break;
//				case 3:
//				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[1]; break;
//				case 4:
//				lastleaf0 = &cell.lastLeaf[5]; lastleaf1 = &cell.lastLeaf[7]; break;
//				case 5:
//				lastleaf0 = &cell.lastLeaf[4]; lastleaf1 = &cell.lastLeaf[6]; break;
//				case 6:
//				lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[3]; break;
//				case 7:
//				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[2]; break;
//				case 8:
//				lastleaf0 = &cell.lastLeaf[3]; lastleaf1 = &cell.lastLeaf[7]; break;
//				case 9:
//				lastleaf0 = &cell.lastLeaf[2]; lastleaf1 = &cell.lastLeaf[6]; break;
//				case 10:
//				lastleaf0 = &cell.lastLeaf[1]; lastleaf1 = &cell.lastLeaf[5]; break;
//				case 11:
//				lastleaf0 = &cell.lastLeaf[0]; lastleaf1 = &cell.lastLeaf[4]; break;
//			}
//			if(*lastleaf0>=BRANCHINIT || _leafScale[*lastleaf0]>_leafScale[leaf]){
//				*lastleaf0 = *lastleaf1 = leaf;
//			}
//			meshCellIndicesLeafSingle.push_back(*i);
//		}
//	}

	for(int j=0;j<8;j++){
		size_t &corner = branchCells.indexCorner[j];
		if(corner<MAXCELLINDEX){
			volumetype *lastleaf = &_meshCells[corner].lastLeaf[7-j];
			if(*lastleaf>=BRANCHINIT || _leafScale[*lastleaf]>_leafScale[leaf]){
				*lastleaf = leaf;
			}
			meshCellIndicesLeafSingle.push_back(corner);
		}
	}
}




#endif
