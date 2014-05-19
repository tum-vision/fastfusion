
#include "geometryfusion_mipmap_cpu.hpp"


#ifdef MESHCELLSATGROW
	fprintf(stderr,"\nCorrecting Offsets for shifted Lower Boundary...");
	for(size_t i=0;i<_meshCells.size();i++){
		_meshCells[i].ox += oxchild; _meshCells[i].oy += oychild; _meshCells[i].oz += ozchild;
	}
	if(oxchild){
		fprintf(stderr,"x...");
//			fprintf(stderr,"W");
		for(size_t i=0;i<_boundary.indexWallWall[0].size();i++){
			_meshCells[_boundary.indexWallWall[0][i]].ox -= _meshCells[_boundary.indexWallWall[0][i]].size;}
		for(size_t i=0;i<_boundary.indexWallEdge[0].size();i++){
			_meshCells[_boundary.indexWallEdge[0][i]].ox -= _meshCells[_boundary.indexWallEdge[0][i]].size;}
		for(size_t i=0;i<_boundary.indexWallCorner[0].size();i++){
			_meshCells[_boundary.indexWallCorner[0][i]].ox -= _meshCells[_boundary.indexWallCorner[0][i]].size;}

//			fprintf(stderr,"E");
//			fprintf(stderr," 4");
		for(size_t i=0;i<_boundary.indexEdgeEdge[4].size();i++){
			_meshCells[_boundary.indexEdgeEdge[4][i]].ox -= _meshCells[_boundary.indexEdgeEdge[4][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[4].size();i++){
			_meshCells[_boundary.indexEdgeCorner[4][i]].ox -= _meshCells[_boundary.indexEdgeCorner[4][i]].size;}

//			fprintf(stderr," 6");
		for(size_t i=0;i<_boundary.indexEdgeEdge[6].size();i++){
			_meshCells[_boundary.indexEdgeEdge[6][i]].ox -= _meshCells[_boundary.indexEdgeEdge[6][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[6].size();i++){
			_meshCells[_boundary.indexEdgeCorner[6][i]].ox -= _meshCells[_boundary.indexEdgeCorner[6][i]].size;}

//			fprintf(stderr," 8");
		for(size_t i=0;i<_boundary.indexEdgeEdge[8].size();i++){
			_meshCells[_boundary.indexEdgeEdge[8][i]].ox -= _meshCells[_boundary.indexEdgeEdge[8][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[8].size();i++){
			_meshCells[_boundary.indexEdgeCorner[8][i]].ox -= _meshCells[_boundary.indexEdgeCorner[8][i]].size;}

//			fprintf(stderr," 10");
		for(size_t i=0;i<_boundary.indexEdgeEdge[10].size();i++){
			_meshCells[_boundary.indexEdgeEdge[10][i]].ox -= _meshCells[_boundary.indexEdgeEdge[10][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[10].size();i++){
			_meshCells[_boundary.indexEdgeCorner[10][i]].ox -= _meshCells[_boundary.indexEdgeCorner[10][i]].size;}

//			fprintf(stderr,"C");
		if(_boundary.indexCorner[0]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[0]].ox -= _meshCells[_boundary.indexCorner[0]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[0]");
		if(_boundary.indexCorner[2]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[2]].ox -= _meshCells[_boundary.indexCorner[2]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[2]");
		if(_boundary.indexCorner[4]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[4]].ox -= _meshCells[_boundary.indexCorner[4]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[4]");
		if(_boundary.indexCorner[6]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[6]].ox -= _meshCells[_boundary.indexCorner[6]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[6]");
	}
	if(oychild){
		fprintf(stderr,"y...");
		for(size_t i=0;i<_boundary.indexWallWall[2].size();i++){
			_meshCells[_boundary.indexWallWall[2][i]].oy -= _meshCells[_boundary.indexWallWall[2][i]].size;}
		for(size_t i=0;i<_boundary.indexWallEdge[2].size();i++){
			_meshCells[_boundary.indexWallEdge[2][i]].oy -= _meshCells[_boundary.indexWallEdge[2][i]].size;}
		for(size_t i=0;i<_boundary.indexWallCorner[2].size();i++){
			_meshCells[_boundary.indexWallCorner[2][i]].oy -= _meshCells[_boundary.indexWallCorner[2][i]].size;}

		for(size_t i=0;i<_boundary.indexEdgeEdge[0].size();i++){
			_meshCells[_boundary.indexEdgeEdge[0][i]].oy -= _meshCells[_boundary.indexEdgeEdge[0][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[0].size();i++){
			_meshCells[_boundary.indexEdgeCorner[0][i]].oy -= _meshCells[_boundary.indexEdgeCorner[0][i]].size;}

		for(size_t i=0;i<_boundary.indexEdgeEdge[2].size();i++){
			_meshCells[_boundary.indexEdgeEdge[2][i]].oy -= _meshCells[_boundary.indexEdgeEdge[2][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[2].size();i++){
			_meshCells[_boundary.indexEdgeCorner[2][i]].oy -= _meshCells[_boundary.indexEdgeCorner[2][i]].size;}

		for(size_t i=0;i<_boundary.indexEdgeEdge[8].size();i++){
			_meshCells[_boundary.indexEdgeEdge[8][i]].oy -= _meshCells[_boundary.indexEdgeEdge[8][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[8].size();i++){
			_meshCells[_boundary.indexEdgeCorner[8][i]].oy -= _meshCells[_boundary.indexEdgeCorner[8][i]].size;}

		for(size_t i=0;i<_boundary.indexEdgeEdge[9].size();i++){
			_meshCells[_boundary.indexEdgeEdge[9][i]].oy -= _meshCells[_boundary.indexEdgeEdge[9][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[9].size();i++){
			_meshCells[_boundary.indexEdgeCorner[9][i]].oy -= _meshCells[_boundary.indexEdgeCorner[9][i]].size;}

		if(_boundary.indexCorner[0]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[0]].oy -= _meshCells[_boundary.indexCorner[0]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[0]");
		if(_boundary.indexCorner[2]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[1]].oy -= _meshCells[_boundary.indexCorner[1]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[1]");
		if(_boundary.indexCorner[4]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[4]].oy -= _meshCells[_boundary.indexCorner[4]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[4]");
		if(_boundary.indexCorner[6]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[5]].oy -= _meshCells[_boundary.indexCorner[5]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[5]");
	}
	if(ozchild){
		fprintf(stderr,"z...");
		for(size_t i=0;i<_boundary.indexWallWall[4].size();i++){
			_meshCells[_boundary.indexWallWall[4][i]].oz -= _meshCells[_boundary.indexWallWall[4][i]].size;}
		for(size_t i=0;i<_boundary.indexWallEdge[4].size();i++){
			_meshCells[_boundary.indexWallEdge[4][i]].oz -= _meshCells[_boundary.indexWallEdge[4][i]].size;}
		for(size_t i=0;i<_boundary.indexWallCorner[4].size();i++){
			_meshCells[_boundary.indexWallCorner[4][i]].oz -= _meshCells[_boundary.indexWallCorner[4][i]].size;}

		for(size_t i=0;i<_boundary.indexEdgeEdge[0].size();i++){
			_meshCells[_boundary.indexEdgeEdge[0][i]].oz -= _meshCells[_boundary.indexEdgeEdge[0][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[0].size();i++){
			_meshCells[_boundary.indexEdgeCorner[0][i]].oz -= _meshCells[_boundary.indexEdgeCorner[0][i]].size;}

		for(size_t i=0;i<_boundary.indexEdgeEdge[1].size();i++){
			_meshCells[_boundary.indexEdgeEdge[1][i]].oz -= _meshCells[_boundary.indexEdgeEdge[1][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[1].size();i++){
			_meshCells[_boundary.indexEdgeCorner[1][i]].oz -= _meshCells[_boundary.indexEdgeCorner[1][i]].size;}

		for(size_t i=0;i<_boundary.indexEdgeEdge[4].size();i++){
			_meshCells[_boundary.indexEdgeEdge[4][i]].oz -= _meshCells[_boundary.indexEdgeEdge[4][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[4].size();i++){
			_meshCells[_boundary.indexEdgeCorner[4][i]].oz -= _meshCells[_boundary.indexEdgeCorner[4][i]].size;}

		for(size_t i=0;i<_boundary.indexEdgeEdge[5].size();i++){
			_meshCells[_boundary.indexEdgeEdge[5][i]].oz -= _meshCells[_boundary.indexEdgeEdge[5][i]].size;}
		for(size_t i=0;i<_boundary.indexEdgeCorner[5].size();i++){
			_meshCells[_boundary.indexEdgeCorner[5][i]].oz -= _meshCells[_boundary.indexEdgeCorner[5][i]].size;}

		if(_boundary.indexCorner[0]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[0]].oz -= _meshCells[_boundary.indexCorner[0]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[0]");
		if(_boundary.indexCorner[2]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[1]].oz -= _meshCells[_boundary.indexCorner[1]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[1]");
		if(_boundary.indexCorner[4]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[2]].oz -= _meshCells[_boundary.indexCorner[2]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[2]");
		if(_boundary.indexCorner[6]<MAXCELLINDEX) _meshCells[_boundary.indexCorner[3]].oz -= _meshCells[_boundary.indexCorner[3]].size;
		else fprintf(stderr,"\nERROR: Missing Boundary Corner[3]");
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

  MCNSplit newboundary;


  fprintf(stderr,"\nAdding Cells");
  fprintf(stderr,"\n Walls");

  //Inner Walls
	if((co&6)!=0){
  	_meshCellIndicesBranch[index+0].indexWallWall[1].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+1].indexWallWall[0].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(1,0,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //7
		_meshCells.back().lastBranch[0] = index+0;
		_meshCells.back().lastBranch[1] = index+1;

		//Boundary
		_meshCells.push_back(MeshCell(1,0 ,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //8
		_meshCells.push_back(MeshCell(1,_n,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //9
  	_meshCellIndicesBranch[index+0].indexWallWall[0].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+1].indexWallWall[1].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[0].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[1].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+0;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+1;
	}
	if((co&6)!=2){
  	_meshCellIndicesBranch[index+2].indexWallWall[1].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+3].indexWallWall[0].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(1,0,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //10
		_meshCells.back().lastBranch[0] = index+2;
		_meshCells.back().lastBranch[1] = index+3;

		//Boundary
		_meshCells.push_back(MeshCell(1,0 ,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //11
		_meshCells.push_back(MeshCell(1,_n,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //12
  	_meshCellIndicesBranch[index+2].indexWallWall[0].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+3].indexWallWall[1].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[0].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[1].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+2;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+3;
	}
	if((co&6)!=4){
  	_meshCellIndicesBranch[index+4].indexWallWall[1].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+5].indexWallWall[0].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(1,0,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //13
		_meshCells.back().lastBranch[0] = index+4;
		_meshCells.back().lastBranch[1] = index+5;

		//Boundary
		_meshCells.push_back(MeshCell(1,0 ,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //14
		_meshCells.push_back(MeshCell(1,_n,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //15
  	_meshCellIndicesBranch[index+4].indexWallWall[0].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+5].indexWallWall[1].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[0].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[1].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+4;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+5;
	}
	if((co&6)!=6){
  	_meshCellIndicesBranch[index+6].indexWallWall[1].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+7].indexWallWall[0].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(1,0,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.back().lastBranch[0] = index+6;
		_meshCells.back().lastBranch[1] = index+7;

		//Boundary
		_meshCells.push_back(MeshCell(1,0 ,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.push_back(MeshCell(1,_n,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //x
  	_meshCellIndicesBranch[index+6].indexWallWall[0].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+7].indexWallWall[1].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[0].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[1].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+6;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+7;
	}

	if((co&5)!=0){
  	_meshCellIndicesBranch[index+0].indexWallWall[3].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+2].indexWallWall[2].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(2,0 ,0,0 ,_n,BRANCHINIT,BRANCHINIT)); //16
		_meshCells.back().lastBranch[0] = index+0;
		_meshCells.back().lastBranch[1] = index+2;

		//Boundary
		_meshCells.push_back(MeshCell(2,0 ,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //17
		_meshCells.push_back(MeshCell(2,0 ,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //18
  	_meshCellIndicesBranch[index+0].indexWallWall[2].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+2].indexWallWall[3].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[2].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[3].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+0;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+2;
	}
	if((co&5)!=1){
  	_meshCellIndicesBranch[index+1].indexWallWall[3].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+3].indexWallWall[2].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(2,_n,0,0 ,_n,BRANCHINIT,BRANCHINIT)); //19
		_meshCells.back().lastBranch[0] = index+1;
		_meshCells.back().lastBranch[1] = index+3;

		//Boundary
		_meshCells.push_back(MeshCell(2,_n,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //20
		_meshCells.push_back(MeshCell(2,_n,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //21
  	_meshCellIndicesBranch[index+1].indexWallWall[2].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+3].indexWallWall[3].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[2].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[3].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+1;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+3;
	}
	if((co&5)!=4){
  	_meshCellIndicesBranch[index+4].indexWallWall[3].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+6].indexWallWall[2].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(2,0 ,0,_n,_n,BRANCHINIT,BRANCHINIT)); //22
		_meshCells.back().lastBranch[0] = index+4;
		_meshCells.back().lastBranch[1] = index+6;

		//Boundary
		_meshCells.push_back(MeshCell(2,0 ,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //23
		_meshCells.push_back(MeshCell(2,0 ,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //24
  	_meshCellIndicesBranch[index+4].indexWallWall[2].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+6].indexWallWall[3].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[2].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[3].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+4;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+6;
	}
	if((co&5)!=5){
  	_meshCellIndicesBranch[index+5].indexWallWall[3].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+7].indexWallWall[2].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(2,_n,0,_n,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.back().lastBranch[0] = index+5;
		_meshCells.back().lastBranch[1] = index+7;

		//Boundary
		_meshCells.push_back(MeshCell(2,_n,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.push_back(MeshCell(2,_n,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //x
  	_meshCellIndicesBranch[index+5].indexWallWall[2].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+7].indexWallWall[3].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[2].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[3].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+5;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+7;
	}

	if((co&3)!=0){
  	_meshCellIndicesBranch[index+0].indexWallWall[5].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+4].indexWallWall[4].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(3,0 ,0 ,0,_n,BRANCHINIT,BRANCHINIT)); //25
		_meshCells.back().lastBranch[0] = index+0;
		_meshCells.back().lastBranch[1] = index+4;

		//Boundary
		_meshCells.push_back(MeshCell(3,0 ,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //26
		_meshCells.push_back(MeshCell(3,0 ,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //27
  	_meshCellIndicesBranch[index+0].indexWallWall[4].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+4].indexWallWall[5].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[4].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[5].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+0;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+4;
	}
	if((co&3)!=1){
  	_meshCellIndicesBranch[index+1].indexWallWall[5].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+5].indexWallWall[4].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(3,_n,0 ,0,_n,BRANCHINIT,BRANCHINIT)); //28
		_meshCells.back().lastBranch[0] = index+1;
		_meshCells.back().lastBranch[1] = index+5;

		//Boundary
		_meshCells.push_back(MeshCell(3,_n,0 ,0 ,_n,BRANCHINIT,BRANCHINIT)); //29
		_meshCells.push_back(MeshCell(3,_n,0 ,_n,_n,BRANCHINIT,BRANCHINIT)); //30
  	_meshCellIndicesBranch[index+1].indexWallWall[4].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+5].indexWallWall[5].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[4].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[5].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+1;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+5;
	}
	if((co&3)!=2){
  	_meshCellIndicesBranch[index+2].indexWallWall[5].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+6].indexWallWall[4].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(3,0 ,_n,0,_n,BRANCHINIT,BRANCHINIT)); //31
		_meshCells.back().lastBranch[0] = index+2;
		_meshCells.back().lastBranch[1] = index+6;

		//Boundary
		_meshCells.push_back(MeshCell(3,0 ,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //32
		_meshCells.push_back(MeshCell(3,0 ,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //33
  	_meshCellIndicesBranch[index+2].indexWallWall[4].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+6].indexWallWall[5].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[4].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[5].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+2;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+6;
	}
	if((co&3)!=3){
  	_meshCellIndicesBranch[index+3].indexWallWall[5].push_back(_meshCells.size());
  	_meshCellIndicesBranch[index+7].indexWallWall[4].push_back(_meshCells.size());
		_meshCells.push_back(MeshCell(3,_n,_n,0,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.back().lastBranch[0] = index+3;
		_meshCells.back().lastBranch[1] = index+7;

		//Boundary
		_meshCells.push_back(MeshCell(3,_n,_n,0 ,_n,BRANCHINIT,BRANCHINIT)); //x
		_meshCells.push_back(MeshCell(3,_n,_n,_n,_n,BRANCHINIT,BRANCHINIT)); //x
  	_meshCellIndicesBranch[index+3].indexWallWall[4].push_back(_meshCells.size()-2);
  	_meshCellIndicesBranch[index+7].indexWallWall[5].push_back(_meshCells.size()-1);
  	newboundary.indexWallWall[4].push_back(_meshCells.size()-2);
  	newboundary.indexWallWall[5].push_back(_meshCells.size()-1);
  	_meshCells[_meshCells.size()-2].lastBranch[1] = index+3;
  	_meshCells[_meshCells.size()-1].lastBranch[0] = index+7;
	}

	_meshCells.push_back(MeshCell(1,_n*((co&1)==0),_n*((co&2)!=0),_n*((co&4)!=0),_n,BRANCHINIT,BRANCHINIT)); //34
	_meshCellIndicesBranch[index+(co&6)+((co&1)==0)].indexWallWall[0+((co&1)==0)].push_back(_meshCells.size()-1);
	newboundary.indexWallWall[0+((co&1)==0)].push_back(_meshCells.size()-1);
	if((co&1)==0) _meshCells[_meshCells.size()-1].lastBranch[0] = index+(co&6)+1;
	else          _meshCells[_meshCells.size()-1].lastBranch[1] = index+(co&6)+0;

	_meshCells.push_back(MeshCell(2,_n*((co&1)!=0),_n*((co&2)==0),_n*((co&4)!=0),_n,BRANCHINIT,BRANCHINIT)); //35
	_meshCellIndicesBranch[index+(co&5)+2*((co&2)==0)].indexWallWall[2+((co&2)==0)].push_back(_meshCells.size()-1);
	newboundary.indexWallWall[2+((co&2)==0)].push_back(_meshCells.size()-1);
	if((co&2)==0) _meshCells[_meshCells.size()-1].lastBranch[0] = index+(co&5)+2;
	else          _meshCells[_meshCells.size()-1].lastBranch[1] = index+(co&5)+0;

	_meshCells.push_back(MeshCell(3,_n*((co&1)!=0),_n*((co&2)!=0),_n*((co&4)==0),_n,BRANCHINIT,BRANCHINIT)); //36
	_meshCellIndicesBranch[index+(co&3)+4*((co&4)==0)].indexWallWall[4+((co&4)==0)].push_back(_meshCells.size()-1);
	newboundary.indexWallWall[4+((co&4)==0)].push_back(_meshCells.size()-1);
	if((co&4)==0) _meshCells[_meshCells.size()-1].lastBranch[0] = index+(co&3)+4;
	else          _meshCells[_meshCells.size()-1].lastBranch[1] = index+(co&3)+0;


  fprintf(stderr,"\n Edges");
	//Edges
	//X Inner
	bool shift = ((co&1)==0);
	_meshCellIndicesBranch[index+0+shift].indexEdgeEdge[3].push_back(_meshCells.size());
	_meshCellIndicesBranch[index+2+shift].indexEdgeEdge[2].push_back(_meshCells.size());
	_meshCellIndicesBranch[index+4+shift].indexEdgeEdge[1].push_back(_meshCells.size());
	_meshCellIndicesBranch[index+6+shift].indexEdgeEdge[0].push_back(_meshCells.size());
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

	_meshCellIndicesBranch[index+0+shift].indexEdgeEdge[0].push_back(_meshCells.size()-8);
	_meshCellIndicesBranch[index+0+shift].indexEdgeEdge[1].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+0+shift].indexEdgeEdge[2].push_back(_meshCells.size()-5);

	_meshCellIndicesBranch[index+2+shift].indexEdgeEdge[0].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+2+shift].indexEdgeEdge[1].push_back(_meshCells.size()-6);
	_meshCellIndicesBranch[index+2+shift].indexEdgeEdge[3].push_back(_meshCells.size()-4);

	_meshCellIndicesBranch[index+4+shift].indexEdgeEdge[0].push_back(_meshCells.size()-5);
	_meshCellIndicesBranch[index+4+shift].indexEdgeEdge[2].push_back(_meshCells.size()-3);
	_meshCellIndicesBranch[index+4+shift].indexEdgeEdge[3].push_back(_meshCells.size()-2);

	_meshCellIndicesBranch[index+6+shift].indexEdgeEdge[1].push_back(_meshCells.size()-4);
	_meshCellIndicesBranch[index+6+shift].indexEdgeEdge[2].push_back(_meshCells.size()-2);
	_meshCellIndicesBranch[index+6+shift].indexEdgeEdge[3].push_back(_meshCells.size()-1);

	newboundary.indexEdgeEdge[0].push_back(_meshCells.size()-8);
	newboundary.indexEdgeEdge[1].push_back(_meshCells.size()-6);
	newboundary.indexEdgeEdge[2].push_back(_meshCells.size()-3);
	newboundary.indexEdgeEdge[3].push_back(_meshCells.size()-1);

	newboundary.indexWallEdge[4].push_back(_meshCells.size()-7);
	newboundary.indexWallEdge[2].push_back(_meshCells.size()-5);
	newboundary.indexWallEdge[3].push_back(_meshCells.size()-4);
	newboundary.indexWallEdge[5].push_back(_meshCells.size()-2);

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

	_meshCellIndicesBranch[index+(co&1)+2*((co&2)==0)  ].indexEdgeEdge[0+((co&2)==0)].push_back(_meshCells.size()-10);
	_meshCellIndicesBranch[index+(co&1)+2*((co&2)==0)  ].indexEdgeEdge[2+((co&2)==0)].push_back(_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&1)+2*((co&2)==0)+4].indexEdgeEdge[0+((co&2)==0)].push_back(_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&1)+2*((co&2)==0)+4].indexEdgeEdge[2+((co&2)==0)].push_back(_meshCells.size()-8);
	_meshCellIndicesBranch[index+(co&1)+4*((co&4)==0)+0].indexEdgeEdge[2*((co&4)==0)+1].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&1)+4*((co&4)==0)+2].indexEdgeEdge[2*((co&4)==0)+0].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&1)+2*((co&2)!=0)+4*((co&4)==0)].indexEdgeEdge[((co&2)!=0)+2*((co&4)==0)].push_back(_meshCells.size()-6);

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

	newboundary.indexEdgeEdge[0+((co&2)==0)].push_back(_meshCells.size()-10);
	newboundary.indexEdgeEdge[2+((co&2)==0)].push_back(_meshCells.size()-8);
	newboundary.indexEdgeEdge[((co&2)!=0)+2*((co&4)==0)].push_back(_meshCells.size()-6);
	newboundary.indexWallEdge[2+((co&2)==0)].push_back(_meshCells.size()-9);
	newboundary.indexWallEdge[4+((co&4)==0)].push_back(_meshCells.size()-7);

	newboundary.indexEdgeCorner[0+((co&2)==0)].push_back(_meshCells.size()-5);
	newboundary.indexEdgeCorner[2+((co&2)==0)].push_back(_meshCells.size()-3);
	newboundary.indexEdgeCorner[((co&2)!=0)+2*((co&4)==0)].push_back(_meshCells.size()-1);
	newboundary.indexWallCorner[2+((co&2)==0)].push_back(_meshCells.size()-4);
	newboundary.indexWallCorner[4+((co&4)==0)].push_back(_meshCells.size()-2);

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
	_meshCellIndicesBranch[index+0+2*shift].indexEdgeEdge[7].push_back(_meshCells.size());
	_meshCellIndicesBranch[index+1+2*shift].indexEdgeEdge[6].push_back(_meshCells.size());
	_meshCellIndicesBranch[index+4+2*shift].indexEdgeEdge[5].push_back(_meshCells.size());
	_meshCellIndicesBranch[index+5+2*shift].indexEdgeEdge[4].push_back(_meshCells.size());
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

	_meshCellIndicesBranch[index+0+2*shift].indexEdgeEdge[4].push_back(_meshCells.size()-8);
	_meshCellIndicesBranch[index+0+2*shift].indexEdgeEdge[5].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+0+2*shift].indexEdgeEdge[6].push_back(_meshCells.size()-5);

	_meshCellIndicesBranch[index+1+2*shift].indexEdgeEdge[4].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+1+2*shift].indexEdgeEdge[5].push_back(_meshCells.size()-6);
	_meshCellIndicesBranch[index+1+2*shift].indexEdgeEdge[7].push_back(_meshCells.size()-4);

	_meshCellIndicesBranch[index+4+2*shift].indexEdgeEdge[4].push_back(_meshCells.size()-5);
	_meshCellIndicesBranch[index+4+2*shift].indexEdgeEdge[6].push_back(_meshCells.size()-3);
	_meshCellIndicesBranch[index+4+2*shift].indexEdgeEdge[7].push_back(_meshCells.size()-2);

	_meshCellIndicesBranch[index+5+2*shift].indexEdgeEdge[5].push_back(_meshCells.size()-4);
	_meshCellIndicesBranch[index+5+2*shift].indexEdgeEdge[6].push_back(_meshCells.size()-2);
	_meshCellIndicesBranch[index+5+2*shift].indexEdgeEdge[7].push_back(_meshCells.size()-1);

	newboundary.indexEdgeEdge[4].push_back(_meshCells.size()-8);
	newboundary.indexEdgeEdge[5].push_back(_meshCells.size()-6);
	newboundary.indexEdgeEdge[6].push_back(_meshCells.size()-3);
	newboundary.indexEdgeEdge[7].push_back(_meshCells.size()-1);

	newboundary.indexWallEdge[4].push_back(_meshCells.size()-7);
	newboundary.indexWallEdge[0].push_back(_meshCells.size()-5);
	newboundary.indexWallEdge[1].push_back(_meshCells.size()-4);
	newboundary.indexWallEdge[5].push_back(_meshCells.size()-2);

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

	_meshCellIndicesBranch[index+(co&2)+  ((co&1)==0)  ].indexEdgeEdge[4+((co&1)==0)].push_back(_meshCells.size()-10);
	_meshCellIndicesBranch[index+(co&2)+  ((co&1)==0)  ].indexEdgeEdge[6+((co&1)==0)].push_back(_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&2)+  ((co&1)==0)+4].indexEdgeEdge[4+((co&1)==0)].push_back(_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&2)+  ((co&1)==0)+4].indexEdgeEdge[6+((co&1)==0)].push_back(_meshCells.size()-8);
	_meshCellIndicesBranch[index+(co&2)+4*((co&4)==0)+0].indexEdgeEdge[4+2*((co&4)==0)+1].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&2)+4*((co&4)==0)+1].indexEdgeEdge[4+2*((co&4)==0)+0].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&2)+  ((co&1)!=0)+4*((co&4)==0)].indexEdgeEdge[4+((co&1)!=0)+2*((co&4)==0)].push_back(_meshCells.size()-6);

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



	newboundary.indexEdgeEdge[4+((co&1)==0)].push_back(_meshCells.size()-10);
	newboundary.indexEdgeEdge[6+((co&1)==0)].push_back(_meshCells.size()-8);
	newboundary.indexEdgeEdge[4+((co&1)!=0)+2*((co&4)==0)].push_back(_meshCells.size()-6);
	newboundary.indexWallEdge[0+((co&1)==0)].push_back(_meshCells.size()-9);
	newboundary.indexWallEdge[4+((co&4)==0)].push_back(_meshCells.size()-7);

	newboundary.indexEdgeCorner[4+((co&1)==0)].push_back(_meshCells.size()-5);
	newboundary.indexEdgeCorner[6+((co&1)==0)].push_back(_meshCells.size()-3);
	newboundary.indexEdgeCorner[4+((co&1)!=0)+2*((co&4)==0)].push_back(_meshCells.size()-1);
	newboundary.indexWallCorner[0+((co&1)==0)].push_back(_meshCells.size()-4);
	newboundary.indexWallCorner[4+((co&4)==0)].push_back(_meshCells.size()-2);

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
	_meshCellIndicesBranch[index+0+4*shift].indexEdgeEdge[11].push_back(_meshCells.size());
	_meshCellIndicesBranch[index+1+4*shift].indexEdgeEdge[10].push_back(_meshCells.size());
	_meshCellIndicesBranch[index+2+4*shift].indexEdgeEdge[ 9].push_back(_meshCells.size());
	_meshCellIndicesBranch[index+3+4*shift].indexEdgeEdge[ 8].push_back(_meshCells.size());
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

	_meshCellIndicesBranch[index+0+4*shift].indexEdgeEdge[ 8].push_back(_meshCells.size()-8);
	_meshCellIndicesBranch[index+0+4*shift].indexEdgeEdge[ 9].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+0+4*shift].indexEdgeEdge[10].push_back(_meshCells.size()-5);

	_meshCellIndicesBranch[index+1+4*shift].indexEdgeEdge[ 8].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+1+4*shift].indexEdgeEdge[ 9].push_back(_meshCells.size()-6);
	_meshCellIndicesBranch[index+1+4*shift].indexEdgeEdge[11].push_back(_meshCells.size()-4);

	_meshCellIndicesBranch[index+2+4*shift].indexEdgeEdge[ 8].push_back(_meshCells.size()-5);
	_meshCellIndicesBranch[index+2+4*shift].indexEdgeEdge[10].push_back(_meshCells.size()-3);
	_meshCellIndicesBranch[index+2+4*shift].indexEdgeEdge[11].push_back(_meshCells.size()-2);

	_meshCellIndicesBranch[index+3+4*shift].indexEdgeEdge[ 9].push_back(_meshCells.size()-4);
	_meshCellIndicesBranch[index+3+4*shift].indexEdgeEdge[10].push_back(_meshCells.size()-2);
	_meshCellIndicesBranch[index+3+4*shift].indexEdgeEdge[11].push_back(_meshCells.size()-1);

	newboundary.indexEdgeEdge[ 8].push_back(_meshCells.size()-8);
	newboundary.indexEdgeEdge[ 9].push_back(_meshCells.size()-6);
	newboundary.indexEdgeEdge[10].push_back(_meshCells.size()-3);
	newboundary.indexEdgeEdge[11].push_back(_meshCells.size()-1);

	newboundary.indexWallEdge[2].push_back(_meshCells.size()-7);
	newboundary.indexWallEdge[0].push_back(_meshCells.size()-5);
	newboundary.indexWallEdge[1].push_back(_meshCells.size()-4);
	newboundary.indexWallEdge[3].push_back(_meshCells.size()-2);

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

	_meshCellIndicesBranch[index+(co&4)+  ((co&1)==0)  ].indexEdgeEdge[ 8+((co&1)==0)].push_back(_meshCells.size()-10);
	_meshCellIndicesBranch[index+(co&4)+  ((co&1)==0)  ].indexEdgeEdge[10+((co&1)==0)].push_back(_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&4)+  ((co&1)==0)+2].indexEdgeEdge[ 8+((co&1)==0)].push_back(_meshCells.size()-9);
	_meshCellIndicesBranch[index+(co&4)+  ((co&1)==0)+2].indexEdgeEdge[10+((co&1)==0)].push_back(_meshCells.size()-8);
	_meshCellIndicesBranch[index+(co&4)+2*((co&2)==0)+0].indexEdgeEdge[ 8+2*((co&2)==0)+1].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&4)+2*((co&2)==0)+1].indexEdgeEdge[ 8+2*((co&2)==0)+0].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[index+(co&4)+  ((co&1)!=0)+2*((co&2)==0)].indexEdgeEdge[8+((co&1)!=0)+2*((co&2)==0)].push_back(_meshCells.size()-6);

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

	newboundary.indexEdgeEdge[ 8+((co&1)==0)].push_back(_meshCells.size()-10);
	newboundary.indexEdgeEdge[10+((co&1)==0)].push_back(_meshCells.size()-8);
	newboundary.indexEdgeEdge[ 8+((co&1)!=0)+2*((co&2)==0)].push_back(_meshCells.size()-6);
	newboundary.indexWallEdge[0+((co&1)==0)].push_back(_meshCells.size()-9);
	newboundary.indexWallEdge[2+((co&2)==0)].push_back(_meshCells.size()-7);

	newboundary.indexEdgeCorner[ 8+((co&1)==0)].push_back(_meshCells.size()-5);
	newboundary.indexEdgeCorner[10+((co&1)==0)].push_back(_meshCells.size()-3);
	newboundary.indexEdgeCorner[ 8+((co&1)!=0)+2*((co&2)==0)].push_back(_meshCells.size()-1);
	newboundary.indexWallCorner[0+((co&1)==0)].push_back(_meshCells.size()-4);
	newboundary.indexWallCorner[2+((co&2)==0)].push_back(_meshCells.size()-2);

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


//		//The Cells concerning the former root, now child
	sidetype half = _n/2;


  fprintf(stderr,"\nAdapting Inner Boundary");

	//Inner Cells with the old boundary
	//Wall X
	{
//		  fprintf(stderr,"\nWall X");
		std::vector<size_t> &wallvectorSource = _boundary.indexWallWall[0+((co&1)==0)];
		std::vector<size_t> &wallvectorTarget = _meshCellIndicesBranch[index+(co&6)+((co&1)==0)].indexWallWall[0+((co&1)!=0)];
		std::vector<size_t> &edgevectorSource = _boundary.indexWallEdge[0+((co&1)==0)];
		std::vector<size_t> &edgevectorTarget = _meshCellIndicesBranch[index+(co&6)+((co&1)==0)].indexWallEdge[0+((co&1)!=0)];
		std::vector<size_t> &cornvectorSource = _boundary.indexWallCorner[0+((co&1)==0)];
		std::vector<size_t> &cornvectorTarget = _meshCellIndicesBranch[index+(co&6)+((co&1)==0)].indexWallCorner[0+((co&1)!=0)];

		for(size_t i=0;i<wallvectorSource.size();i++){
			wallvectorTarget.push_back(wallvectorSource[i]);
			if((co&1)!=0){
				_meshCells[wallvectorSource[i]].lastBranch[0] = index+(co&6)+0;
			}
			else{
				_meshCells[wallvectorSource[i]].lastBranch[1] = index+(co&6)+1;
			}
		}
		for(size_t i=0;i<edgevectorSource.size();i++){
			edgevectorTarget.push_back(edgevectorSource[i]);
			if((co&1)!=0){
				_meshCells[edgevectorSource[i]].lastBranch[0] =
				_meshCells[edgevectorSource[i]].lastBranch[2] = index+(co&6)+0;
			}
			else{
				_meshCells[edgevectorSource[i]].lastBranch[1] =
				_meshCells[edgevectorSource[i]].lastBranch[3] = index+(co&6)+1;
			}
		}
		for(size_t i=0;i<cornvectorSource.size();i++){
			cornvectorTarget.push_back(cornvectorSource[i]);
			if((co&1)!=0){
				_meshCells[cornvectorSource[i]].lastBranch[0] =
				_meshCells[cornvectorSource[i]].lastBranch[2] =
				_meshCells[cornvectorSource[i]].lastBranch[4] =
				_meshCells[cornvectorSource[i]].lastBranch[6] = index+(co&6)+0;
			}
			else{
				_meshCells[cornvectorSource[i]].lastBranch[1] =
				_meshCells[cornvectorSource[i]].lastBranch[3] =
				_meshCells[cornvectorSource[i]].lastBranch[5] =
				_meshCells[cornvectorSource[i]].lastBranch[7] = index+(co&6)+1;
			}
		}
		_boundary.indexWallWall[0+((co&1)==0)]   = newboundary.indexWallWall  [0+((co&1)==0)];
		_boundary.indexWallEdge[0+((co&1)==0)]   = newboundary.indexWallEdge  [0+((co&1)==0)];
		_boundary.indexWallCorner[0+((co&1)==0)] = newboundary.indexWallCorner[0+((co&1)==0)];
	}

	//Wall Y
	{
//		  fprintf(stderr,"\nWall Y");
		std::vector<size_t> &wallvectorSource = _boundary.indexWallWall[2+((co&2)==0)];
		std::vector<size_t> &wallvectorTarget = _meshCellIndicesBranch[index+(co&5)+2*((co&2)==0)].indexWallWall[2+((co&2)!=0)];
		std::vector<size_t> &edgevectorSource = _boundary.indexWallEdge[2+((co&2)==0)];
		std::vector<size_t> &edgevectorTarget = _meshCellIndicesBranch[index+(co&5)+2*((co&2)==0)].indexWallEdge[2+((co&2)!=0)];
		std::vector<size_t> &cornvectorSource = _boundary.indexWallCorner[2+((co&2)==0)];
		std::vector<size_t> &cornvectorTarget = _meshCellIndicesBranch[index+(co&5)+2*((co&2)==0)].indexWallCorner[2+((co&2)!=0)];

		for(size_t i=0;i<wallvectorSource.size();i++){
			wallvectorTarget.push_back(wallvectorSource[i]);
			if((co&2)!=0){
				_meshCells[wallvectorSource[i]].lastBranch[0] = index+(co&5)+0;
			}
			else{
				_meshCells[wallvectorSource[i]].lastBranch[1] = index+(co&5)+2;
			}
		}
		for(size_t i=0;i<edgevectorSource.size();i++){
			edgevectorTarget.push_back(edgevectorSource[i]);
			if((co&2)!=0) {
				if(_meshCells[edgevectorSource[i]].type==4){
					_meshCells[edgevectorSource[i]].lastBranch[0] =
					_meshCells[edgevectorSource[i]].lastBranch[2] = index+(co&5)+0;
				}
				else{
					_meshCells[edgevectorSource[i]].lastBranch[0] =
					_meshCells[edgevectorSource[i]].lastBranch[1] = index+(co&5)+0;
				}
			}
			else{
				if(_meshCells[edgevectorSource[i]].type==4){
					_meshCells[edgevectorSource[i]].lastBranch[1] =
					_meshCells[edgevectorSource[i]].lastBranch[3] = index+(co&5)+2;
				}
				else{
					_meshCells[edgevectorSource[i]].lastBranch[2] =
					_meshCells[edgevectorSource[i]].lastBranch[3] = index+(co&5)+2;
				}
			}
		}
		for(size_t i=0;i<cornvectorSource.size();i++){
			cornvectorTarget.push_back(cornvectorSource[i]);
			if((co&2)!=0) {
				_meshCells[cornvectorSource[i]].lastBranch[0] =
				_meshCells[cornvectorSource[i]].lastBranch[2] =
				_meshCells[cornvectorSource[i]].lastBranch[4] =
				_meshCells[cornvectorSource[i]].lastBranch[6] = index+(co&5)+0;
			}
			else{
				_meshCells[cornvectorSource[i]].lastBranch[1] =
				_meshCells[cornvectorSource[i]].lastBranch[3] =
				_meshCells[cornvectorSource[i]].lastBranch[5] =
				_meshCells[cornvectorSource[i]].lastBranch[7] = index+(co&5)+2;
			}
		}
		_boundary.indexWallWall[2+((co&2)==0)]   = newboundary.indexWallWall  [2+((co&2)==0)];
		_boundary.indexWallEdge[2+((co&2)==0)]   = newboundary.indexWallEdge  [2+((co&2)==0)];
		_boundary.indexWallCorner[2+((co&2)==0)] = newboundary.indexWallCorner[2+((co&2)==0)];
	}

	//Wall Z
	{
//		  fprintf(stderr,"\nWall Z");
		std::vector<size_t> &wallvectorSource = _boundary.indexWallWall[4+((co&4)==0)];
		std::vector<size_t> &wallvectorTarget = _meshCellIndicesBranch[index+(co&3)+4*((co&4)==0)].indexWallWall[4+((co&4)!=0)];
		std::vector<size_t> &edgevectorSource = _boundary.indexWallEdge[4+((co&4)==0)];
		std::vector<size_t> &edgevectorTarget = _meshCellIndicesBranch[index+(co&3)+4*((co&4)==0)].indexWallEdge[4+((co&4)!=0)];
		std::vector<size_t> &cornvectorSource = _boundary.indexWallCorner[4+((co&4)==0)];
		std::vector<size_t> &cornvectorTarget = _meshCellIndicesBranch[index+(co&3)+4*((co&4)==0)].indexWallCorner[4+((co&4)!=0)];

		for(size_t i=0;i<wallvectorSource.size();i++){
			wallvectorTarget.push_back(wallvectorSource[i]);
			if((co&4)!=0){
				_meshCells[wallvectorSource[i]].lastBranch[0] = index+(co&3)+0;
			}
			else{
				_meshCells[wallvectorSource[i]].lastBranch[1] = index+(co&3)+4;
			}
		}
		for(size_t i=0;i<edgevectorSource.size();i++){
			edgevectorTarget.push_back(edgevectorSource[i]);
			if((co&4)!=0){
				_meshCells[edgevectorSource[i]].lastBranch[0] =
				_meshCells[edgevectorSource[i]].lastBranch[1] = index+(co&3)+0;
			}
			else{
				_meshCells[edgevectorSource[i]].lastBranch[2] =
				_meshCells[edgevectorSource[i]].lastBranch[3] = index+(co&3)+4;
			}
		}
		for(size_t i=0;i<cornvectorSource.size();i++){
			cornvectorTarget.push_back(cornvectorSource[i]);
			if((co&4)!=0){
				_meshCells[cornvectorSource[i]].lastBranch[0] =
				_meshCells[cornvectorSource[i]].lastBranch[1] =
				_meshCells[cornvectorSource[i]].lastBranch[2] =
				_meshCells[cornvectorSource[i]].lastBranch[3] = index+(co&3)+0;
			}
			else{
				_meshCells[cornvectorSource[i]].lastBranch[4] =
				_meshCells[cornvectorSource[i]].lastBranch[5] =
				_meshCells[cornvectorSource[i]].lastBranch[6] =
				_meshCells[cornvectorSource[i]].lastBranch[7] = index+(co&3)+4;
			}
		}
		_boundary.indexWallWall  [4+((co&4)==0)] = newboundary.indexWallWall  [4+((co&4)==0)];
		_boundary.indexWallEdge  [4+((co&4)==0)] = newboundary.indexWallEdge  [4+((co&4)==0)];
		_boundary.indexWallCorner[4+((co&4)==0)] = newboundary.indexWallCorner[4+((co&4)==0)];
	}

	//Edge X
	{
//		  fprintf(stderr,"\nEdge X");
		std::vector<size_t> &edgevectorSource = _boundary.indexEdgeEdge  [0+((co&2)==0)+2*((co&4)==0)];
		std::vector<size_t> &cornvectorSource = _boundary.indexEdgeCorner[0+((co&2)==0)+2*((co&4)==0)];
		std::vector<size_t> &edgevectorTarget0 = _meshCellIndicesBranch[index+((co&1)!=0)+2*((co&2)==0)+4*((co&4)!=0)].indexEdgeEdge  [0+((co&2)!=0)+2*((co&4)==0)];
		std::vector<size_t> &edgevectorTarget1 = _meshCellIndicesBranch[index+((co&1)!=0)+2*((co&2)!=0)+4*((co&4)==0)].indexEdgeEdge  [0+((co&2)==0)+2*((co&4)!=0)];
		std::vector<size_t> &edgevectorTarget2 = _meshCellIndicesBranch[index+((co&1)!=0)+2*((co&2)==0)+4*((co&4)==0)].indexEdgeEdge  [0+((co&2)!=0)+2*((co&4)!=0)];
		std::vector<size_t> &cornvectorTarget0 = _meshCellIndicesBranch[index+((co&1)!=0)+2*((co&2)==0)+4*((co&4)!=0)].indexEdgeCorner[0+((co&2)!=0)+2*((co&4)==0)];
		std::vector<size_t> &cornvectorTarget1 = _meshCellIndicesBranch[index+((co&1)!=0)+2*((co&2)!=0)+4*((co&4)==0)].indexEdgeCorner[0+((co&2)==0)+2*((co&4)!=0)];
		std::vector<size_t> &cornvectorTarget2 = _meshCellIndicesBranch[index+((co&1)!=0)+2*((co&2)==0)+4*((co&4)==0)].indexEdgeCorner[0+((co&2)!=0)+2*((co&4)!=0)];

		for(size_t i=0;i<edgevectorSource.size();i++){
			edgevectorTarget0.push_back(edgevectorSource[i]);
			edgevectorTarget1.push_back(edgevectorSource[i]);
			edgevectorTarget2.push_back(edgevectorSource[i]);
			MeshCell &cell = _meshCells[edgevectorSource[i]];
			if((co&6)!=0){
				cell.lastBranch[0] = index+(co&1)+0;
			}
			if((co&6)!=2){
				cell.lastBranch[1] = index+(co&1)+2;
			}
			if((co&6)!=4){
				cell.lastBranch[2] = index+(co&1)+4;
			}
			if((co&6)!=6){
				cell.lastBranch[3] = index+(co&1)+6;
			}
		}
		for(size_t i=0;i<cornvectorSource.size();i++){
			cornvectorTarget0.push_back(cornvectorSource[i]);
			cornvectorTarget1.push_back(cornvectorSource[i]);
			cornvectorTarget2.push_back(cornvectorSource[i]);
			MeshCell &cell = _meshCells[cornvectorSource[i]];
			if((co&6)!=0){
				cell.lastBranch[0] = cell.lastBranch[1] = index+(co&1)+0;
			}
			if((co&6)!=2){
				cell.lastBranch[2] = cell.lastBranch[3] = index+(co&1)+2;
			}
			if((co&6)!=4){
				cell.lastBranch[4] = cell.lastBranch[5] = index+(co&1)+4;
			}
			if((co&6)!=6){
				cell.lastBranch[6] = cell.lastBranch[7] = index+(co&1)+6;
			}
		}
		_boundary.indexEdgeEdge  [0+((co&2)==0)+2*((co&4)==0)] = newboundary.indexEdgeEdge  [0+((co&2)==0)+2*((co&4)==0)];
		_boundary.indexEdgeCorner[0+((co&2)==0)+2*((co&4)==0)] = newboundary.indexEdgeCorner[0+((co&2)==0)+2*((co&4)==0)];
	}

	//Edge Y
	{
//		  fprintf(stderr,"\nEdge Y");
		std::vector<size_t> &edgevectorSource = _boundary.indexEdgeEdge  [4+((co&1)==0)+2*((co&4)==0)];
		std::vector<size_t> &cornvectorSource = _boundary.indexEdgeCorner[4+((co&1)==0)+2*((co&4)==0)];
		std::vector<size_t> &edgevectorTarget0 = _meshCellIndicesBranch[index+2*((co&2)!=0)+((co&1)==0)+4*((co&4)!=0)].indexEdgeEdge  [4+((co&1)!=0)+2*((co&4)==0)];
		std::vector<size_t> &edgevectorTarget1 = _meshCellIndicesBranch[index+2*((co&2)!=0)+((co&1)!=0)+4*((co&4)==0)].indexEdgeEdge  [4+((co&1)==0)+2*((co&4)!=0)];
		std::vector<size_t> &edgevectorTarget2 = _meshCellIndicesBranch[index+2*((co&2)!=0)+((co&1)==0)+4*((co&4)==0)].indexEdgeEdge  [4+((co&1)!=0)+2*((co&4)!=0)];
		std::vector<size_t> &cornvectorTarget0 = _meshCellIndicesBranch[index+2*((co&2)!=0)+((co&1)==0)+4*((co&4)!=0)].indexEdgeCorner[4+((co&1)!=0)+2*((co&4)==0)];
		std::vector<size_t> &cornvectorTarget1 = _meshCellIndicesBranch[index+2*((co&2)!=0)+((co&1)!=0)+4*((co&4)==0)].indexEdgeCorner[4+((co&1)==0)+2*((co&4)!=0)];
		std::vector<size_t> &cornvectorTarget2 = _meshCellIndicesBranch[index+2*((co&2)!=0)+((co&1)==0)+4*((co&4)==0)].indexEdgeCorner[4+((co&1)!=0)+2*((co&4)!=0)];

		for(size_t i=0;i<edgevectorSource.size();i++){
			edgevectorTarget0.push_back(edgevectorSource[i]);
			edgevectorTarget1.push_back(edgevectorSource[i]);
			edgevectorTarget2.push_back(edgevectorSource[i]);
			MeshCell &cell = _meshCells[edgevectorSource[i]];
			if((co&5)!=0){
				cell.lastBranch[0] = index+(co&2)+0;
			}
			if((co&5)!=1){
				cell.lastBranch[1] = index+(co&2)+1;
			}
			if((co&5)!=4){
				cell.lastBranch[2] = index+(co&2)+4;
			}
			if((co&5)!=5){
				cell.lastBranch[3] = index+(co&2)+5;
			}
		}
		for(size_t i=0;i<cornvectorSource.size();i++){
			cornvectorTarget0.push_back(cornvectorSource[i]);
			cornvectorTarget1.push_back(cornvectorSource[i]);
			cornvectorTarget2.push_back(cornvectorSource[i]);
			MeshCell &cell = _meshCells[cornvectorSource[i]];
			if((co&5)!=0){
				cell.lastBranch[0] = cell.lastBranch[2] = index+(co&2)+0;
			}
			if((co&5)!=1){
				cell.lastBranch[1] = cell.lastBranch[3] = index+(co&2)+1;
			}
			if((co&5)!=4){
				cell.lastBranch[4] = cell.lastBranch[6] = index+(co&2)+4;
			}
			if((co&5)!=5){
				cell.lastBranch[5] = cell.lastBranch[7] = index+(co&2)+5;
			}
		}
		_boundary.indexEdgeEdge  [4+((co&1)==0)+2*((co&4)==0)] = newboundary.indexEdgeEdge  [4+((co&1)==0)+2*((co&4)==0)];
		_boundary.indexEdgeCorner[4+((co&1)==0)+2*((co&4)==0)] = newboundary.indexEdgeCorner[4+((co&1)==0)+2*((co&4)==0)];
	}

	//Edge Z
	{
//		  fprintf(stderr,"\nEdge Z");
		std::vector<size_t> &edgevectorSource = _boundary.indexEdgeEdge  [8+((co&1)==0)+2*((co&2)==0)];
		std::vector<size_t> &cornvectorSource = _boundary.indexEdgeCorner[8+((co&1)==0)+2*((co&2)==0)];
		std::vector<size_t> &edgevectorTarget0 = _meshCellIndicesBranch[index+4*((co&4)!=0)+((co&1)==0)+2*((co&2)!=0)].indexEdgeEdge  [8+((co&1)!=0)+2*((co&2)==0)];
		std::vector<size_t> &edgevectorTarget1 = _meshCellIndicesBranch[index+4*((co&4)!=0)+((co&1)!=0)+2*((co&2)==0)].indexEdgeEdge  [8+((co&1)==0)+2*((co&2)!=0)];
		std::vector<size_t> &edgevectorTarget2 = _meshCellIndicesBranch[index+4*((co&4)!=0)+((co&1)==0)+2*((co&2)==0)].indexEdgeEdge  [8+((co&1)!=0)+2*((co&2)!=0)];
		std::vector<size_t> &cornvectorTarget0 = _meshCellIndicesBranch[index+4*((co&4)!=0)+((co&1)==0)+2*((co&2)!=0)].indexEdgeCorner[8+((co&1)!=0)+2*((co&2)==0)];
		std::vector<size_t> &cornvectorTarget1 = _meshCellIndicesBranch[index+4*((co&4)!=0)+((co&1)!=0)+2*((co&2)==0)].indexEdgeCorner[8+((co&1)==0)+2*((co&2)!=0)];
		std::vector<size_t> &cornvectorTarget2 = _meshCellIndicesBranch[index+4*((co&4)!=0)+((co&1)==0)+2*((co&2)==0)].indexEdgeCorner[8+((co&1)!=0)+2*((co&2)!=0)];

//			fprintf(stderr,"\nEdges");
		for(size_t i=0;i<edgevectorSource.size();i++){
//				fprintf(stderr," %li",i);
			edgevectorTarget0.push_back(edgevectorSource[i]);
			edgevectorTarget1.push_back(edgevectorSource[i]);
			edgevectorTarget2.push_back(edgevectorSource[i]);
			MeshCell &cell = _meshCells[edgevectorSource[i]];
			if((co&3)!=0){
				cell.lastBranch[0] = index+(co&4)+0;
			}
			if((co&3)!=1){
				cell.lastBranch[1] = index+(co&4)+1;
			}
			if((co&3)!=2){
				cell.lastBranch[2] = index+(co&4)+2;
			}
			if((co&3)!=3){
				cell.lastBranch[3] = index+(co&4)+3;
			}
		}
//			fprintf(stderr,"\nCorners");
		for(size_t i=0;i<cornvectorSource.size();i++){
//				fprintf(stderr," %li",i);
			cornvectorTarget0.push_back(cornvectorSource[i]);
			cornvectorTarget1.push_back(cornvectorSource[i]);
			cornvectorTarget2.push_back(cornvectorSource[i]);
			MeshCell &cell = _meshCells[cornvectorSource[i]];
			if((co&3)!=0){
				cell.lastBranch[0] = cell.lastBranch[4] = index+(co&4)+0;
			}
			if((co&3)!=1){
				cell.lastBranch[1] = cell.lastBranch[5] = index+(co&4)+1;
			}
			if((co&3)!=2){
				cell.lastBranch[2] = cell.lastBranch[6] = index+(co&4)+2;
			}
			if((co&3)!=3){
				cell.lastBranch[3] = cell.lastBranch[7] = index+(co&4)+3;
			}
		}
//			fprintf(stderr,"\nOverwriting");
		_boundary.indexEdgeEdge  [8+((co&1)==0)+2*((co&2)==0)] = newboundary.indexEdgeEdge  [8+((co&1)==0)+2*((co&2)==0)];
		_boundary.indexEdgeCorner[8+((co&1)==0)+2*((co&2)==0)] = newboundary.indexEdgeCorner[8+((co&1)==0)+2*((co&2)==0)];
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

//	  fprintf(stderr,"\nAdapting Outer Boundary");
	//The remaining boundary gets bigger and the edges need to be included
	//in the new volume
	//Wall X
	{
		int wallIndex  = 0+((co&1)!=0);
		int edgeIndexY = 4+((co&1)!=0)+2*((co&4)==0);
		int edgeIndexZ = 8+((co&1)!=0)+2*((co&2)==0);
		int cornerIndex = ((co&1)!=0)+2*((co&2)==0)+4*((co&4)==0);

		for(size_t i=0;i<newboundary.indexWallWall[wallIndex].size();i++){
			_boundary.indexWallWall[wallIndex].push_back(newboundary.indexWallWall[wallIndex][i]);
		}
		for(size_t i=0;i<newboundary.indexWallEdge[wallIndex].size();i++){
			_boundary.indexWallEdge[wallIndex].push_back(newboundary.indexWallEdge[wallIndex][i]);
		}
		for(size_t i=0;i<newboundary.indexWallCorner[wallIndex].size();i++){
			_boundary.indexWallCorner[wallIndex].push_back(newboundary.indexWallCorner[wallIndex][i]);
		}

		for(size_t i=0;i<_boundary.indexEdgeEdge[edgeIndexY].size();i++){
			_boundary.indexWallEdge[wallIndex].push_back(_boundary.indexEdgeEdge[edgeIndexY][i]);
			_meshCellIndicesBranch[(co&3)+4*((co&4)==0)].indexEdgeEdge[4+((co&1)!=0)+2*((co&4)!=0)].
				push_back(_boundary.indexEdgeEdge[edgeIndexY][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallEdge[wallIndex].back()];
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
		for(size_t i=0;i<_boundary.indexEdgeCorner[edgeIndexY].size();i++){
			_boundary.indexWallCorner[wallIndex].push_back(_boundary.indexEdgeCorner[edgeIndexY][i]);
			_meshCellIndicesBranch[(co&3)+4*((co&4)==0)].indexEdgeCorner[4+((co&1)!=0)+2*((co&4)!=0)].
				push_back(_boundary.indexEdgeCorner[edgeIndexY][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallCorner[wallIndex].back()];
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
		for(size_t i=0;i<_boundary.indexEdgeEdge[edgeIndexZ].size();i++){
			_boundary.indexWallEdge[wallIndex].push_back(_boundary.indexEdgeEdge[edgeIndexZ][i]);
			_meshCellIndicesBranch[(co&5)+2*((co&2)==0)].indexEdgeEdge[8+((co&1)!=0)+2*((co&2)!=0)].
				push_back(_boundary.indexEdgeEdge[edgeIndexZ][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallEdge[wallIndex].back()];
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
		for(size_t i=0;i<_boundary.indexEdgeCorner[edgeIndexZ].size();i++){
			_boundary.indexWallCorner[wallIndex].push_back(_boundary.indexEdgeCorner[edgeIndexZ][i]);
			_meshCellIndicesBranch[(co&5)+2*((co&2)==0)].indexEdgeCorner[8+((co&1)!=0)+2*((co&2)!=0)].
				push_back(_boundary.indexEdgeCorner[edgeIndexZ][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallCorner[wallIndex].back()];
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
		{
			_boundary.indexWallCorner[wallIndex].push_back(_boundary.indexCorner[cornerIndex]);
			MeshCell & cell = _meshCells[_boundary.indexCorner[cornerIndex]];
			if((co&6)!=0){
				_meshCellIndicesBranch[index+0+((co&1)==0)].indexCorner[6+(co&1)] = _boundary.indexCorner[cornerIndex];
				if(co&1){
					cell.lastBranch[1] = index+0+0;
				}
				else{
					cell.lastBranch[0] = index+0+1;
				}
			}
			if((co&6)!=2){
				_meshCellIndicesBranch[index+2+((co&1)==0)].indexCorner[4+(co&1)] = _boundary.indexCorner[cornerIndex];
				if(co&1){
					cell.lastBranch[3] = index+2+0;
				}
				else{
					cell.lastBranch[2] = index+2+1;
				}
			}
			if((co&6)!=4){
				_meshCellIndicesBranch[index+4+((co&1)==0)].indexCorner[2+(co&1)] = _boundary.indexCorner[cornerIndex];
				if(co&1){
					cell.lastBranch[5] = index+4+0;
				}
				else{
					cell.lastBranch[4] = index+4+1;
				}
			}
			if((co&6)!=6){
				_meshCellIndicesBranch[index+6+((co&1)==0)].indexCorner[0+(co&1)] = _boundary.indexCorner[cornerIndex];
				if(co&1){
					cell.lastBranch[7] = index+6+0;
				}
				else{
					cell.lastBranch[6] = index+6+1;
				}
			}
		}

		_boundary.indexEdgeEdge[edgeIndexY] = newboundary.indexEdgeEdge[edgeIndexY];
		_boundary.indexEdgeCorner[edgeIndexY] = newboundary.indexEdgeCorner[edgeIndexY];
		_boundary.indexEdgeEdge[edgeIndexZ] = newboundary.indexEdgeEdge[edgeIndexZ];
		_boundary.indexEdgeCorner[edgeIndexZ] = newboundary.indexEdgeCorner[edgeIndexZ];
		_boundary.indexCorner  [cornerIndex] =
		newboundary.indexCorner[cornerIndex];
	}
	//Wall Y
	{
		int wallIndex  = 2+((co&2)!=0);
		int edgeIndexX = 0+((co&2)!=0)+2*((co&4)==0);
		int edgeIndexZ = 8+((co&1)==0)+2*((co&2)!=0);
		int cornerIndex = ((co&1)==0)+2*((co&2)!=0)+4*((co&4)==0);

		for(size_t i=0;i<newboundary.indexWallWall[wallIndex].size();i++){
			_boundary.indexWallWall[wallIndex].push_back(newboundary.indexWallWall[wallIndex][i]);
		}
		for(size_t i=0;i<newboundary.indexWallEdge[wallIndex].size();i++){
			_boundary.indexWallEdge[wallIndex].push_back(newboundary.indexWallEdge[wallIndex][i]);
		}
		for(size_t i=0;i<newboundary.indexWallCorner[wallIndex].size();i++){
			_boundary.indexWallCorner[wallIndex].push_back(newboundary.indexWallCorner[wallIndex][i]);
		}

		for(size_t i=0;i<_boundary.indexEdgeEdge[edgeIndexX].size();i++){
			_boundary.indexWallEdge[wallIndex].push_back(_boundary.indexEdgeEdge[edgeIndexX][i]);
			_meshCellIndicesBranch[(co&3)+4*((co&4)==0)].indexEdgeEdge[0+((co&2)!=0)+2*((co&4)!=0)].
				push_back(_boundary.indexEdgeEdge[edgeIndexX][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallEdge[wallIndex].back()];
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
		for(size_t i=0;i<_boundary.indexEdgeCorner[edgeIndexX].size();i++){
			_boundary.indexWallCorner[wallIndex].push_back(_boundary.indexEdgeCorner[edgeIndexX][i]);
			_meshCellIndicesBranch[(co&3)+4*((co&4)==0)].indexEdgeCorner[0+((co&2)!=0)+2*((co&4)!=0)].
				push_back(_boundary.indexEdgeCorner[edgeIndexX][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallCorner[wallIndex].back()];
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
		for(size_t i=0;i<_boundary.indexEdgeEdge[edgeIndexZ].size();i++){
			_boundary.indexWallEdge[wallIndex].push_back(_boundary.indexEdgeEdge[edgeIndexZ][i]);
			_meshCellIndicesBranch[(co&6)+((co&1)==0)].indexEdgeEdge[8+((co&1)!=0)+2*((co&2)!=0)].
				push_back(_boundary.indexEdgeEdge[edgeIndexZ][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallEdge[wallIndex].back()];
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
		for(size_t i=0;i<_boundary.indexEdgeCorner[edgeIndexZ].size();i++){
			_boundary.indexWallCorner[wallIndex].push_back(_boundary.indexEdgeCorner[edgeIndexZ][i]);
			_meshCellIndicesBranch[(co&6)+((co&1)==0)].indexEdgeCorner[8+((co&1)!=0)+2*((co&2)!=0)].
				push_back(_boundary.indexEdgeCorner[edgeIndexZ][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallCorner[wallIndex].back()];
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
		{
			_boundary.indexWallCorner[wallIndex].push_back(_boundary.indexCorner[cornerIndex]);
			MeshCell & cell = _meshCells[_boundary.indexCorner[cornerIndex]];
			if((co&5)!=0){
				_meshCellIndicesBranch[index+0+2*((co&2)==0)].indexCorner[5+(co&2)] = _boundary.indexCorner[cornerIndex];
				if(co&2){
					cell.lastBranch[2] = index+0+0;
				}
				else{
					cell.lastBranch[0] = index+0+2;
				}
			}
			if((co&5)!=1){
				_meshCellIndicesBranch[index+1+2*((co&2)==0)].indexCorner[4+(co&2)] = _boundary.indexCorner[cornerIndex];
				if(co&2){
					cell.lastBranch[3] = index+1+0;
				}
				else{
					cell.lastBranch[1] = index+1+2;
				}
			}
			if((co&5)!=4){
				_meshCellIndicesBranch[index+4+2*((co&2)==0)].indexCorner[1+(co&2)] = _boundary.indexCorner[cornerIndex];
				if(co&2){
					cell.lastBranch[6] = index+4+0;
				}
				else{
					cell.lastBranch[4] = index+4+2;
				}
			}
			if((co&5)!=5){
				_meshCellIndicesBranch[index+5+2*((co&2)==0)].indexCorner[0+(co&2)] = _boundary.indexCorner[cornerIndex];
				if(co&2){
					cell.lastBranch[7] = index+5+0;
				}
				else{
					cell.lastBranch[5] = index+5+2;
				}
			}

		}

		_boundary.indexEdgeEdge[edgeIndexX] = newboundary.indexEdgeEdge[edgeIndexX];
		_boundary.indexEdgeCorner[edgeIndexX] = newboundary.indexEdgeCorner[edgeIndexX];
		_boundary.indexEdgeEdge[edgeIndexZ] = newboundary.indexEdgeEdge[edgeIndexZ];
		_boundary.indexEdgeCorner[edgeIndexZ] = newboundary.indexEdgeCorner[edgeIndexZ];
		_boundary.indexCorner  [cornerIndex] =
		newboundary.indexCorner[cornerIndex];
	}
	//Wall Z
	{
		int wallIndex  = 4+((co&4)!=0);
		int edgeIndexX = 0+((co&2)==0)+2*((co&4)!=0);
		int edgeIndexY = 4+((co&1)==0)+2*((co&4)!=0);
		int cornerIndex = ((co&1)==0)+2*((co&2)==0)+4*((co&4)!=0);

		for(size_t i=0;i<newboundary.indexWallWall[wallIndex].size();i++){
			_boundary.indexWallWall[wallIndex].push_back(newboundary.indexWallWall[wallIndex][i]);
		}
		for(size_t i=0;i<newboundary.indexWallEdge[wallIndex].size();i++){
			_boundary.indexWallEdge[wallIndex].push_back(newboundary.indexWallEdge[wallIndex][i]);
		}
		for(size_t i=0;i<newboundary.indexWallCorner[wallIndex].size();i++){
			_boundary.indexWallCorner[wallIndex].push_back(newboundary.indexWallCorner[wallIndex][i]);
		}

		for(size_t i=0;i<_boundary.indexEdgeEdge[edgeIndexX].size();i++){
			_boundary.indexWallEdge[wallIndex].push_back(_boundary.indexEdgeEdge[edgeIndexX][i]);
			_meshCellIndicesBranch[(co&5)+2*((co&2)==0)].indexEdgeEdge[0+((co&2)!=0)+2*((co&4)!=0)].
				push_back(_boundary.indexEdgeEdge[edgeIndexX][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallEdge[wallIndex].back()];
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
		for(size_t i=0;i<_boundary.indexEdgeCorner[edgeIndexX].size();i++){
			_boundary.indexWallCorner[wallIndex].push_back(_boundary.indexEdgeCorner[edgeIndexX][i]);
			_meshCellIndicesBranch[(co&5)+2*((co&2)==0)].indexEdgeCorner[0+((co&2)!=0)+2*((co&4)!=0)].
				push_back(_boundary.indexEdgeCorner[edgeIndexX][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallCorner[wallIndex].back()];
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
		for(size_t i=0;i<_boundary.indexEdgeEdge[edgeIndexY].size();i++){
			_boundary.indexWallEdge[wallIndex].push_back(_boundary.indexEdgeEdge[edgeIndexY][i]);
			_meshCellIndicesBranch[(co&6)+((co&1)==0)].indexEdgeEdge[4+((co&1)!=0)+2*((co&4)!=0)].
				push_back(_boundary.indexEdgeEdge[edgeIndexY][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallEdge[wallIndex].back()];
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
		for(size_t i=0;i<_boundary.indexEdgeCorner[edgeIndexY].size();i++){
			_boundary.indexWallCorner[wallIndex].push_back(_boundary.indexEdgeCorner[edgeIndexY][i]);
			_meshCellIndicesBranch[(co&6)+((co&1)==0)].indexEdgeCorner[4+((co&1)!=0)+2*((co&4)!=0)].
				push_back(_boundary.indexEdgeCorner[edgeIndexY][i]);
			MeshCell & cell = _meshCells[_boundary.indexWallCorner[wallIndex].back()];
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
		{
			_boundary.indexWallCorner[wallIndex].push_back(_boundary.indexCorner[cornerIndex]);
			MeshCell & cell = _meshCells[_boundary.indexCorner[cornerIndex]];
			if((co&3)!=0){
				_meshCellIndicesBranch[0+4*((co&4)==0)].indexCorner[3+(co&4)] = _boundary.indexCorner[cornerIndex];
				if(co&4){
					cell.lastBranch[4] = index+0+0;
				}
				else{
					cell.lastBranch[0] = index+0+4;
				}
			}
			if((co&3)!=1){
				_meshCellIndicesBranch[1+4*((co&4)==0)].indexCorner[2+(co&4)] = _boundary.indexCorner[cornerIndex];
				if(co&4){
					cell.lastBranch[5] = index+1+0;
				}
				else{
					cell.lastBranch[1] = index+1+4;
				}
			}
			if((co&3)!=2){
				_meshCellIndicesBranch[2+4*((co&4)==0)].indexCorner[1+(co&4)] = _boundary.indexCorner[cornerIndex];
				if(co&4){
					cell.lastBranch[6] = index+2+0;
				}
				else{
					cell.lastBranch[2] = index+2+4;
				}
			}
			if((co&3)!=3){
				_meshCellIndicesBranch[3+4*((co&4)==0)].indexCorner[0+(co&4)] = _boundary.indexCorner[cornerIndex];
				if(co&4){
					cell.lastBranch[7] = index+3+0;
				}
				else{
					cell.lastBranch[3] = index+3+4;
				}
			}

		}

		_boundary.indexEdgeEdge[edgeIndexX] = newboundary.indexEdgeEdge[edgeIndexX];
		_boundary.indexEdgeCorner[edgeIndexX] = newboundary.indexEdgeCorner[edgeIndexX];
		_boundary.indexEdgeEdge[edgeIndexY] = newboundary.indexEdgeEdge[edgeIndexY];
		_boundary.indexEdgeCorner[edgeIndexY] = newboundary.indexEdgeCorner[edgeIndexY];
		_boundary.indexCorner  [cornerIndex] =
		newboundary.indexCorner[cornerIndex];
	}

	//Edge X
	{
		int edgeIndex = 0+((co&2)!=0)+2*((co&4)!=0);
		for(size_t i=0;i<newboundary.indexEdgeEdge[edgeIndex].size();i++){
			_boundary.indexEdgeEdge[edgeIndex].push_back(newboundary.indexEdgeEdge[edgeIndex][i]);
		}
		for(size_t i=0;i<newboundary.indexEdgeCorner[edgeIndex].size();i++){
			_boundary.indexEdgeCorner[edgeIndex].push_back(newboundary.indexEdgeCorner[edgeIndex][i]);
		}
	}

	//Edge Y
	{
		int edgeIndex = 4+((co&1)!=0)+2*((co&4)!=0);
		for(size_t i=0;i<newboundary.indexEdgeEdge[edgeIndex].size();i++){
			_boundary.indexEdgeEdge[edgeIndex].push_back(newboundary.indexEdgeEdge[edgeIndex][i]);
		}
		for(size_t i=0;i<newboundary.indexEdgeCorner[edgeIndex].size();i++){
			_boundary.indexEdgeCorner[edgeIndex].push_back(newboundary.indexEdgeCorner[edgeIndex][i]);
		}
	}

	//Edge Z
	{
		int edgeIndex = 8+((co&1)!=0)+2*((co&2)!=0);
		for(size_t i=0;i<newboundary.indexEdgeEdge[edgeIndex].size();i++){
			_boundary.indexEdgeEdge[edgeIndex].push_back(newboundary.indexEdgeEdge[edgeIndex][i]);
		}
		for(size_t i=0;i<newboundary.indexEdgeCorner[edgeIndex].size();i++){
			_boundary.indexEdgeCorner[edgeIndex].push_back(newboundary.indexEdgeCorner[edgeIndex][i]);
		}
	}

#endif



#ifdef BRANCHSUBDIVISION
	size_t oldSize = _meshCellIndicesBranch.size();
//			_meshCellIndicesBranch.resize(oldSize+16);
//			for(size_t i=oldSize;i<oldSize+16;i++){
	_meshCellIndicesBranch.resize(oldSize+childBranchSize);
	for(size_t i=oldSize;i<oldSize+childBranchSize;i++){
		_meshCellIndicesBranch[i].indexInterior = MAXCELLINDEX;
	}

//			fprintf(stderr,"\nAdding Branch");
	size_t i = _meshCellIndicesBranch[branch+childoffset].indexInterior;
	if(i>=MAXCELLINDEX){
		 fprintf(stderr,"\nERROR: Branch has no Mesh Cell");
	}
	_meshCellIndicesBranch[oldSize+0].indexInterior = i;
	_meshCells[i].lastLeaf[0] = pl;
	_meshCells[i].ox = oxnew;
	_meshCells[i].oy = oynew;
	_meshCells[i].oz = oznew;
	_meshCells[i].size = quarter;


	_meshCells.push_back(MeshCell(0,oxnew+quarter,oynew,        oznew,        quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew,        oynew+quarter,oznew,        quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew+quarter,oynew+quarter,oznew,        quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew,        oynew,        oznew+quarter,quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew+quarter,oynew,        oznew+quarter,quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew,        oynew+quarter,oznew+quarter,quarter,pl));
	_meshCells.push_back(MeshCell(0,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,pl));

	_meshCellIndicesBranch[oldSize+1].indexInterior = _meshCells.size()-7;
	_meshCellIndicesBranch[oldSize+2].indexInterior = _meshCells.size()-6;
	_meshCellIndicesBranch[oldSize+3].indexInterior = _meshCells.size()-5;
	_meshCellIndicesBranch[oldSize+4].indexInterior = _meshCells.size()-4;
	_meshCellIndicesBranch[oldSize+5].indexInterior = _meshCells.size()-3;
	_meshCellIndicesBranch[oldSize+6].indexInterior = _meshCells.size()-2;
	_meshCellIndicesBranch[oldSize+7].indexInterior = _meshCells.size()-1;

	if(pl<BRANCHINIT){
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-5);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-6);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-7);
	}

	_meshCellIndicesBranch[branch+childoffset].indexInterior = MAXCELLINDEX;

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

	_meshCells[_meshCells.size()-12].lastBranch[0] = oldSize+0;
	_meshCells[_meshCells.size()-12].lastBranch[1] = oldSize+1;
	_meshCells[_meshCells.size()-11].lastBranch[0] = oldSize+2;
	_meshCells[_meshCells.size()-11].lastBranch[1] = oldSize+3;
	_meshCells[_meshCells.size()-10].lastBranch[0] = oldSize+4;
	_meshCells[_meshCells.size()-10].lastBranch[1] = oldSize+5;
	_meshCells[_meshCells.size()-9].lastBranch[0] = oldSize+6;
	_meshCells[_meshCells.size()-9].lastBranch[1] = oldSize+7;

	_meshCells[_meshCells.size()-8].lastBranch[0] = oldSize+0;
	_meshCells[_meshCells.size()-8].lastBranch[1] = oldSize+2;
	_meshCells[_meshCells.size()-7].lastBranch[0] = oldSize+1;
	_meshCells[_meshCells.size()-7].lastBranch[1] = oldSize+3;
	_meshCells[_meshCells.size()-6].lastBranch[0] = oldSize+4;
	_meshCells[_meshCells.size()-6].lastBranch[1] = oldSize+6;
	_meshCells[_meshCells.size()-5].lastBranch[0] = oldSize+5;
	_meshCells[_meshCells.size()-5].lastBranch[1] = oldSize+7;

	_meshCells[_meshCells.size()-4].lastBranch[0] = oldSize+0;
	_meshCells[_meshCells.size()-4].lastBranch[1] = oldSize+4;
	_meshCells[_meshCells.size()-3].lastBranch[0] = oldSize+1;
	_meshCells[_meshCells.size()-3].lastBranch[1] = oldSize+5;
	_meshCells[_meshCells.size()-2].lastBranch[0] = oldSize+2;
	_meshCells[_meshCells.size()-2].lastBranch[1] = oldSize+6;
	_meshCells[_meshCells.size()-1].lastBranch[0] = oldSize+3;
	_meshCells[_meshCells.size()-1].lastBranch[1] = oldSize+7;

	_meshCellIndicesBranch[oldSize+0].indexWallWall[1].push_back(_meshCells.size()-12);
	_meshCellIndicesBranch[oldSize+1].indexWallWall[0].push_back(_meshCells.size()-12);
	_meshCellIndicesBranch[oldSize+2].indexWallWall[1].push_back(_meshCells.size()-11);
	_meshCellIndicesBranch[oldSize+3].indexWallWall[0].push_back(_meshCells.size()-11);
	_meshCellIndicesBranch[oldSize+4].indexWallWall[1].push_back(_meshCells.size()-10);
	_meshCellIndicesBranch[oldSize+5].indexWallWall[0].push_back(_meshCells.size()-10);
	_meshCellIndicesBranch[oldSize+6].indexWallWall[1].push_back(_meshCells.size()-9);
	_meshCellIndicesBranch[oldSize+7].indexWallWall[0].push_back(_meshCells.size()-9);

	_meshCellIndicesBranch[oldSize+0].indexWallWall[3].push_back(_meshCells.size()-8);
	_meshCellIndicesBranch[oldSize+2].indexWallWall[2].push_back(_meshCells.size()-8);
	_meshCellIndicesBranch[oldSize+1].indexWallWall[3].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[oldSize+3].indexWallWall[2].push_back(_meshCells.size()-7);
	_meshCellIndicesBranch[oldSize+4].indexWallWall[3].push_back(_meshCells.size()-6);
	_meshCellIndicesBranch[oldSize+6].indexWallWall[2].push_back(_meshCells.size()-6);
	_meshCellIndicesBranch[oldSize+5].indexWallWall[3].push_back(_meshCells.size()-5);
	_meshCellIndicesBranch[oldSize+7].indexWallWall[2].push_back(_meshCells.size()-5);

	_meshCellIndicesBranch[oldSize+0].indexWallWall[5].push_back(_meshCells.size()-4);
	_meshCellIndicesBranch[oldSize+4].indexWallWall[4].push_back(_meshCells.size()-4);
	_meshCellIndicesBranch[oldSize+1].indexWallWall[5].push_back(_meshCells.size()-3);
	_meshCellIndicesBranch[oldSize+5].indexWallWall[4].push_back(_meshCells.size()-3);
	_meshCellIndicesBranch[oldSize+2].indexWallWall[5].push_back(_meshCells.size()-2);
	_meshCellIndicesBranch[oldSize+6].indexWallWall[4].push_back(_meshCells.size()-2);
	_meshCellIndicesBranch[oldSize+3].indexWallWall[5].push_back(_meshCells.size()-1);
	_meshCellIndicesBranch[oldSize+7].indexWallWall[4].push_back(_meshCells.size()-1);

	if(pl<BRANCHINIT){
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


	//Edges
	_meshCells.push_back(MeshCell(4,oxnew        ,oynew        ,oznew        ,quarter,pl,pl,pl,pl));
	_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew        ,oznew        ,quarter,pl,pl,pl,pl));

	_meshCells.push_back(MeshCell(5,oxnew        ,oynew        ,oznew        ,quarter,pl,pl,pl,pl));
	_meshCells.push_back(MeshCell(5,oxnew        ,oynew+quarter,oznew        ,quarter,pl,pl,pl,pl));

	_meshCells.push_back(MeshCell(6,oxnew        ,oynew        ,oznew        ,quarter,pl,pl,pl,pl));
	_meshCells.push_back(MeshCell(6,oxnew        ,oynew        ,oznew+quarter,quarter,pl,pl,pl,pl));

	_meshCells[_meshCells.size()-6].lastBranch[0] = oldSize+0;
	_meshCells[_meshCells.size()-6].lastBranch[1] = oldSize+2;
	_meshCells[_meshCells.size()-6].lastBranch[2] = oldSize+4;
	_meshCells[_meshCells.size()-6].lastBranch[3] = oldSize+6;

	_meshCells[_meshCells.size()-5].lastBranch[0] = oldSize+1;
	_meshCells[_meshCells.size()-5].lastBranch[1] = oldSize+3;
	_meshCells[_meshCells.size()-5].lastBranch[2] = oldSize+5;
	_meshCells[_meshCells.size()-5].lastBranch[3] = oldSize+7;

	_meshCells[_meshCells.size()-4].lastBranch[0] = oldSize+0;
	_meshCells[_meshCells.size()-4].lastBranch[1] = oldSize+1;
	_meshCells[_meshCells.size()-4].lastBranch[2] = oldSize+4;
	_meshCells[_meshCells.size()-4].lastBranch[3] = oldSize+5;

	_meshCells[_meshCells.size()-3].lastBranch[0] = oldSize+2;
	_meshCells[_meshCells.size()-3].lastBranch[1] = oldSize+3;
	_meshCells[_meshCells.size()-3].lastBranch[2] = oldSize+6;
	_meshCells[_meshCells.size()-3].lastBranch[3] = oldSize+7;

	_meshCells[_meshCells.size()-2].lastBranch[0] = oldSize+0;
	_meshCells[_meshCells.size()-2].lastBranch[1] = oldSize+1;
	_meshCells[_meshCells.size()-2].lastBranch[2] = oldSize+2;
	_meshCells[_meshCells.size()-2].lastBranch[3] = oldSize+3;

	_meshCells[_meshCells.size()-1].lastBranch[0] = oldSize+4;
	_meshCells[_meshCells.size()-1].lastBranch[1] = oldSize+5;
	_meshCells[_meshCells.size()-1].lastBranch[2] = oldSize+6;
	_meshCells[_meshCells.size()-1].lastBranch[3] = oldSize+7;

	_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[3].push_back(_meshCells.size()-6);
	_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[2].push_back(_meshCells.size()-6);
	_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[1].push_back(_meshCells.size()-6);
	_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[0].push_back(_meshCells.size()-6);

	_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[3].push_back(_meshCells.size()-5);
	_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[2].push_back(_meshCells.size()-5);
	_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[1].push_back(_meshCells.size()-5);
	_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[0].push_back(_meshCells.size()-5);

	_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[7].push_back(_meshCells.size()-4);
	_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[6].push_back(_meshCells.size()-4);
	_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[5].push_back(_meshCells.size()-4);
	_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[4].push_back(_meshCells.size()-4);

	_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[7].push_back(_meshCells.size()-3);
	_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[6].push_back(_meshCells.size()-3);
	_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[5].push_back(_meshCells.size()-3);
	_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[4].push_back(_meshCells.size()-3);

	_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[11].push_back(_meshCells.size()-2);
	_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[10].push_back(_meshCells.size()-2);
	_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[ 9].push_back(_meshCells.size()-2);
	_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[ 8].push_back(_meshCells.size()-2);

	_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[11].push_back(_meshCells.size()-1);
	_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[10].push_back(_meshCells.size()-1);
	_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[ 9].push_back(_meshCells.size()-1);
	_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[ 8].push_back(_meshCells.size()-1);

	if(pl<BRANCHINIT){
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-6);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-5);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
	}

	//Corner(s)
	_meshCells.push_back(MeshCell(7,oxnew,oynew,oznew,quarter,pl,pl,pl,pl,pl,pl,pl,pl));
	_meshCells.back().lastBranch[0] = oldSize+0;
	_meshCells.back().lastBranch[1] = oldSize+1;
	_meshCells.back().lastBranch[2] = oldSize+2;
	_meshCells.back().lastBranch[3] = oldSize+3;
	_meshCells.back().lastBranch[4] = oldSize+4;
	_meshCells.back().lastBranch[5] = oldSize+5;
	_meshCells.back().lastBranch[6] = oldSize+6;
	_meshCells.back().lastBranch[7] = oldSize+7;
	_meshCellIndicesBranch[oldSize+0].indexCorner[7] = _meshCells.size()-1;
	_meshCellIndicesBranch[oldSize+1].indexCorner[6] = _meshCells.size()-1;
	_meshCellIndicesBranch[oldSize+2].indexCorner[5] = _meshCells.size()-1;
	_meshCellIndicesBranch[oldSize+3].indexCorner[4] = _meshCells.size()-1;
	_meshCellIndicesBranch[oldSize+4].indexCorner[3] = _meshCells.size()-1;
	_meshCellIndicesBranch[oldSize+5].indexCorner[2] = _meshCells.size()-1;
	_meshCellIndicesBranch[oldSize+6].indexCorner[1] = _meshCells.size()-1;
	_meshCellIndicesBranch[oldSize+7].indexCorner[0] = _meshCells.size()-1;
	if(pl<BRANCHINIT){
		_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
	}
//			fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Interior",_meshCells.size()-1,quarter);


	for(int dim=0;dim<6;dim++){
		bool border = (dim==0 && ox==0) || (dim==1 && ox+half==_n) ||
				          (dim==2 && oy==0) || (dim==3 && oy+half==_n) ||
				          (dim==4 && oz==0) || (dim==5 && oz+half==_n);
		if((_meshCellIndicesBranch[branch+childoffset].indexWallWall[dim].size()<1 && !border) ||
			 (_meshCellIndicesBranch[branch+childoffset].indexWallWall[dim].size()==1 &&
				_meshCellIndicesBranch[branch+childoffset].indexWallEdge[dim].size()) ||
				 (_meshCellIndicesBranch[branch+childoffset].indexWallWall[dim].size()>1 &&
				 !_meshCellIndicesBranch[branch+childoffset].indexWallEdge[dim].size())){
//					fprintf(stderr,"\nERROR in Branch %i Wall Side %i: [%li %li]",branch+childoffset,dim,
//							_meshCellIndicesBranch[branch+childoffset].indexWallWall[dim].size(),
//							_meshCellIndicesBranch[branch+childoffset].indexWallEdge[dim].size());
		}
	}


//			fprintf(stderr,"W");
//			fprintf(stderr,"0");
	{
		std::vector<size_t> &wallvector = _meshCellIndicesBranch[branch+childoffset].indexWallWall[0];
		if(wallvector.size()>1){
			std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexWallEdge[0];
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexWallCorner[0];
			if(!edgevector.size() || !cornervector.size()){
				fprintf(stderr,"\nERROR: Empty Edge or Corner Cells for Wall 0 [%li %li] at [%i %i %i]",edgevector.size(),cornervector.size(),oxnew,oynew,oznew);
			}
			for(unsigned int i=0;i<wallvector.size();i++){
				MeshCell &cell = _meshCells[wallvector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallWall Cell [%i %i %i] X[0] is too large: %i > %i at [%i %i %i]",
						cell.ox,cell.oy,cell.oz,cell.size,quarter,oxnew,oynew,oznew);}
				if(cell.ox>=oxnew) fprintf(stderr,"\nERROR: Cell X offset too large: %i >= %i",cell.ox,oxnew);
				volumetype cellchildindex = (cell.oy>=oynew+quarter)*2 + (cell.oz>=oznew+quarter)*4;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexWallWall[0].push_back(wallvector[i]);
				cell.lastBranch[1] = oldSize+cellchildindex;
			}
//					fprintf(stderr,"\nDEBUG: Passing Edges at Wall[0] %i from [%i %i %i]%i:",branch+childoffset,oxnew,oynew,oznew,half);
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallEdge Cell is too large");}
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
//						fprintf(stderr,"\n%li: %i [%i %i %i]%i",edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,cell.size);
				if(cell.type == 5){
					if(cell.oz+cell.size==oznew+quarter){
//								fprintf(stderr,"-> Y Edge");
						_meshCellIndicesBranch[oldSize+shifty+0].indexEdgeEdge[ 6].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shifty+4].indexEdgeEdge[ 4].push_back(edgevector[i]);
						cell.lastBranch[1] = oldSize+shifty+0;
						cell.lastBranch[3] = oldSize+shifty+4;
					}
					else{
//								fprintf(stderr,"-> Wall");
						_meshCellIndicesBranch[oldSize+shifty+shiftz].indexWallEdge[0].push_back(edgevector[i]);
						cell.lastBranch[1] = cell.lastBranch[3] = oldSize+shifty+shiftz;
					}
				}
				else if(cell.type == 6){
					if(cell.oy+cell.size==oynew+quarter){
//								fprintf(stderr,"-> Z Edge");
						_meshCellIndicesBranch[oldSize+shiftz+0].indexEdgeEdge[10].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shiftz+2].indexEdgeEdge[ 8].push_back(edgevector[i]);
						cell.lastBranch[1] = oldSize+shiftz+0;
						cell.lastBranch[3] = oldSize+shiftz+2;
					}
					else{
//								fprintf(stderr,"-> Wall");
						_meshCellIndicesBranch[oldSize+shifty+shiftz].indexWallEdge[0].push_back(edgevector[i]);
						cell.lastBranch[1] = cell.lastBranch[3] = oldSize+shifty+shiftz;
					}
				}
				else{fprintf(stderr,"\nERROR: Wall of Type %i has Edges of Type %i",0,cell.type);}
			}
			edgevector.clear();
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallCorner Cell is too large");}
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				if(cell.oy+cell.size==oynew+quarter && cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[oldSize+0].indexCorner[6] = cornervector[i];
					_meshCellIndicesBranch[oldSize+2].indexCorner[4] = cornervector[i];
					_meshCellIndicesBranch[oldSize+4].indexCorner[2] = cornervector[i];
					_meshCellIndicesBranch[oldSize+6].indexCorner[0] = cornervector[i];
					cell.lastBranch[1] = oldSize+0; cell.lastBranch[3] = oldSize+2;
					cell.lastBranch[5] = oldSize+4; cell.lastBranch[7] = oldSize+6;
				}
				else{
					if(cell.oy+cell.size==oynew+quarter){
						_meshCellIndicesBranch[oldSize+shiftz+0].indexEdgeCorner[10].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shiftz+2].indexEdgeCorner[ 8].push_back(cornervector[i]);
						cell.lastBranch[1] = cell.lastBranch[5] = oldSize+shiftz+0;
						cell.lastBranch[3] = cell.lastBranch[7] = oldSize+shiftz+2;
					}
					else if(cell.oz+cell.size==oznew+quarter){
						_meshCellIndicesBranch[oldSize+shifty+0].indexEdgeCorner[ 6].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shifty+4].indexEdgeCorner[ 4].push_back(cornervector[i]);
						cell.lastBranch[1] = cell.lastBranch[3] = oldSize+shifty+0;
						cell.lastBranch[5] = cell.lastBranch[7] = oldSize+shifty+4;
					}
					else{
						_meshCellIndicesBranch[oldSize+shifty+shiftz].indexWallCorner[0].push_back(cornervector[i]);
						cell.lastBranch[1] = cell.lastBranch[3] =
						cell.lastBranch[5] = cell.lastBranch[7] = oldSize+shifty+shiftz;
					}
				}
			}
			cornervector.clear();
		}
		else if(wallvector.size()==1){
			_meshCellIndicesBranch[oldSize+0].indexWallWall[0].push_back(wallvector[0]);
			MeshCell &cell = _meshCells[wallvector[0]];
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew; cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[1] = oldSize+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[0];

			_meshCells.push_back(MeshCell(1,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew        ,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(1,oxnew-quarter*(oxnew!=0),oynew        ,oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(1,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew+quarter,quarter,ll0,ll1));

			_meshCells[_meshCells.size()-3].lastBranch[1] = oldSize+2;
			_meshCells[_meshCells.size()-2].lastBranch[1] = oldSize+4;
			_meshCells[_meshCells.size()-1].lastBranch[1] = oldSize+6;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;

			_meshCellIndicesBranch[oldSize+2].indexWallWall[0].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+4].indexWallWall[0].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+6].indexWallWall[0].push_back(_meshCells.size()-1);
			if(oxnew!=0) _meshCellIndicesBranch[nb].indexWallWall[1].push_back(_meshCells.size()-3);
			if(oxnew!=0) _meshCellIndicesBranch[nb].indexWallWall[1].push_back(_meshCells.size()-2);
			if(oxnew!=0) _meshCellIndicesBranch[nb].indexWallWall[1].push_back(_meshCells.size()-1);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(5,oxnew-quarter*(oxnew!=0),oynew        ,oznew,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(5,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew-quarter*(oxnew!=0),oynew,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew-quarter*(oxnew!=0),oynew,oznew+quarter,quarter,ll0,ll1,ll0,ll1));

//					fprintf(stderr,"\nCreated Cells at W[0]%i %li %li %li %li [%li %li %li %li | %i]",
//							quarter,_meshCells.size()-4,_meshCells.size()-3,_meshCells.size()-2,_meshCells.size()-1,
//							oldSize+0,oldSize+2,oldSize+4,oldSize+6,nb);

			_meshCells[_meshCells.size()-4].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[1] = oldSize+0;
			_meshCells[_meshCells.size()-4].lastBranch[3] = oldSize+4;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[1] = oldSize+2;
			_meshCells[_meshCells.size()-3].lastBranch[3] = oldSize+6;

			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = oldSize+0;
			_meshCells[_meshCells.size()-2].lastBranch[3] = oldSize+2;

			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = oldSize+4;
			_meshCells[_meshCells.size()-1].lastBranch[3] = oldSize+6;

			_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[ 6].push_back(_meshCells.size()-4);
			_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[ 4].push_back(_meshCells.size()-4);
			if(oxnew!=0) _meshCellIndicesBranch[nb].indexWallEdge[1].push_back(_meshCells.size()-4);

			_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[ 6].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[ 4].push_back(_meshCells.size()-3);
			if(oxnew!=0) _meshCellIndicesBranch[nb].indexWallEdge[1].push_back(_meshCells.size()-3);

			_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[10].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[ 8].push_back(_meshCells.size()-2);
			if(oxnew!=0) _meshCellIndicesBranch[nb].indexWallEdge[1].push_back(_meshCells.size()-2);

			_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[10].push_back(_meshCells.size()-1);
			_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[ 8].push_back(_meshCells.size()-1);
			if(oxnew!=0) _meshCellIndicesBranch[nb].indexWallEdge[1].push_back(_meshCells.size()-1);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew-quarter*(oxnew!=0),oynew,oznew,quarter,ll0,ll1,ll0,ll1,ll0,ll1,ll0,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[2] = corner.lastBranch[4] = corner.lastBranch[6] = nb;
			corner.lastBranch[1] = oldSize+0; corner.lastBranch[3] = oldSize+2;
			corner.lastBranch[5] = oldSize+4; corner.lastBranch[7] = oldSize+6;

			_meshCellIndicesBranch[oldSize+0].indexCorner[6] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+2].indexCorner[4] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+4].indexCorner[2] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+6].indexCorner[0] = _meshCells.size()-1;
			if(oxnew!=0) _meshCellIndicesBranch[nb].indexWallCorner[1].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oxnew==0){
				_boundary.indexWallWall[0].push_back(_meshCells.size()-8);
				_boundary.indexWallWall[0].push_back(_meshCells.size()-7);
				_boundary.indexWallWall[0].push_back(_meshCells.size()-6);
				_boundary.indexWallEdge[0].push_back(_meshCells.size()-5);
				_boundary.indexWallEdge[0].push_back(_meshCells.size()-4);
				_boundary.indexWallEdge[0].push_back(_meshCells.size()-3);
				_boundary.indexWallEdge[0].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[0].push_back(_meshCells.size()-1);
			}
//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Wall[0]",_meshCells.size()-1,quarter);
		}
		wallvector.clear();
	}

//			fprintf(stderr,"1");
	{
		std::vector<size_t> &wallvector = _meshCellIndicesBranch[branch+childoffset].indexWallWall[1];
		if(wallvector.size()>1){
			std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexWallEdge[1];
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexWallCorner[1];
			if(!edgevector.size() || !cornervector.size()){
				fprintf(stderr,"\nERROR: Empty Edge or Corner Cells for Wall 1 [%li %li] at [%i %i %i]",edgevector.size(),cornervector.size(),oxnew,oynew,oznew);
			}
			for(unsigned int i=0;i<wallvector.size();i++){
				MeshCell &cell = _meshCells[wallvector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallWall Cell [%i %i %i] X[1] is too large: %i > %i at [%i %i %i]",
						cell.ox,cell.oy,cell.oz,cell.size,quarter,oxnew,oynew,oznew);}
				if(cell.ox+cell.size!=oxnew+half) fprintf(stderr,"\nERROR: Cell X does not fit: %i+%i vs %i+%i",cell.ox,cell.size,oxnew,half);
				volumetype cellchildindex = 1 + (cell.oy>=oynew+quarter)*2 + (cell.oz>=oznew+quarter)*4;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexWallWall[1].push_back(wallvector[i]);
				cell.lastBranch[0] = oldSize+cellchildindex;
			}
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallEdge Cell is too large");}
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				if(cell.type == 5){
					if(cell.oz+cell.size==oznew+quarter){
						_meshCellIndicesBranch[oldSize+shifty+1].indexEdgeEdge[ 7].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shifty+5].indexEdgeEdge[ 5].push_back(edgevector[i]);
						cell.lastBranch[0] = oldSize+shifty+1;
						cell.lastBranch[2] = oldSize+shifty+5;
					}
					else{
						_meshCellIndicesBranch[oldSize+shifty+shiftz+1].indexWallEdge[1].push_back(edgevector[i]);
						cell.lastBranch[0] = cell.lastBranch[2] = oldSize+shifty+shiftz+1;
					}
				}
				else if(cell.type == 6){
					if(cell.oy+cell.size==oynew+quarter){
						_meshCellIndicesBranch[oldSize+shiftz+1].indexEdgeEdge[11].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shiftz+3].indexEdgeEdge[ 9].push_back(edgevector[i]);
						cell.lastBranch[0] = oldSize+shiftz+1;
						cell.lastBranch[2] = oldSize+shiftz+3;
					}
					else{
						_meshCellIndicesBranch[oldSize+shifty+shiftz+1].indexWallEdge[1].push_back(edgevector[i]);
						cell.lastBranch[0] = cell.lastBranch[2] = oldSize+shifty+shiftz+1;
					}
				}
				else{fprintf(stderr,"\nERROR: Wall of Type %i has Edges of Type %i",1,cell.type);}
			}
			edgevector.clear();
			for(unsigned int i=0;i<cornervector.size();i++){
//						fprintf(stderr,"\nPassing Corner Cell %li at Wall[1], Leaf %i",cornervector[i],pl);
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallCorner Cell is too large");}
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				if(cell.oy+cell.size==oynew+quarter && cell.oz+cell.size==oznew+quarter){
//							fprintf(stderr," - > Middle");
					_meshCellIndicesBranch[oldSize+1].indexCorner[7] = cornervector[i];
					_meshCellIndicesBranch[oldSize+3].indexCorner[5] = cornervector[i];
					_meshCellIndicesBranch[oldSize+5].indexCorner[3] = cornervector[i];
					_meshCellIndicesBranch[oldSize+7].indexCorner[1] = cornervector[i];
					cell.lastBranch[0] = oldSize+1; cell.lastBranch[2] = oldSize+3;
					cell.lastBranch[4] = oldSize+5; cell.lastBranch[6] = oldSize+7;
				}
				else{
					if(cell.oy+cell.size==oynew+quarter){
						_meshCellIndicesBranch[oldSize+shiftz+1].indexEdgeCorner[11].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shiftz+3].indexEdgeCorner[ 9].push_back(cornervector[i]);
						cell.lastBranch[0] = cell.lastBranch[4] = oldSize+shiftz+1;
						cell.lastBranch[2] = cell.lastBranch[6] = oldSize+shiftz+3;
					}
					else if(cell.oz+cell.size==oznew+quarter){
						_meshCellIndicesBranch[oldSize+shifty+1].indexEdgeCorner[ 7].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shifty+5].indexEdgeCorner[ 5].push_back(cornervector[i]);
						cell.lastBranch[0] = cell.lastBranch[2] = oldSize+shifty+1;
						cell.lastBranch[4] = cell.lastBranch[6] = oldSize+shifty+5;
					}
					else{
						_meshCellIndicesBranch[oldSize+shifty+shiftz+1].indexWallCorner[1].push_back(cornervector[i]);
						cell.lastBranch[0] = cell.lastBranch[2] =
						cell.lastBranch[4] = cell.lastBranch[6] = oldSize+shifty+shiftz+1;
					}
				}
			}
			cornervector.clear();
		}
		else if(wallvector.size()==1){
			_meshCellIndicesBranch[oldSize+1].indexWallWall[1].push_back(wallvector[0]);
			MeshCell &cell = _meshCells[wallvector[0]];
			cell.ox = oxnew+quarter; cell.oy = oynew; cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[0] = oldSize+1;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[1];

			_meshCells.push_back(MeshCell(1,oxnew+quarter,oynew+quarter,oznew        ,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(1,oxnew+quarter,oynew        ,oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(1,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1));

			_meshCells[_meshCells.size()-3].lastBranch[0] = oldSize+3;
			_meshCells[_meshCells.size()-2].lastBranch[0] = oldSize+5;
			_meshCells[_meshCells.size()-1].lastBranch[0] = oldSize+7;

			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;

			_meshCellIndicesBranch[oldSize+3].indexWallWall[1].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+5].indexWallWall[1].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+7].indexWallWall[1].push_back(_meshCells.size()-1);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].indexWallWall[0].push_back(_meshCells.size()-3);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].indexWallWall[0].push_back(_meshCells.size()-2);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].indexWallWall[0].push_back(_meshCells.size()-1);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}


			_meshCells.push_back(MeshCell(5,oxnew+quarter,oynew        ,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(5,oxnew+quarter,oynew+quarter,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew+quarter,oynew        ,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew+quarter,oynew        ,oznew+quarter,quarter,ll0,ll1,ll0,ll1));

//					fprintf(stderr,"\nCreated Cells at W[1]%i %li %li %li %li [%li %li %li %li | %i]",
//							quarter,_meshCells.size()-4,_meshCells.size()-3,_meshCells.size()-2,_meshCells.size()-1,
//							oldSize+1,oldSize+3,oldSize+5,oldSize+7,nb);

			_meshCells[_meshCells.size()-4].lastBranch[0] = oldSize+1;
			_meshCells[_meshCells.size()-4].lastBranch[2] = oldSize+5;
			_meshCells[_meshCells.size()-4].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-3].lastBranch[0] = oldSize+3;
			_meshCells[_meshCells.size()-3].lastBranch[2] = oldSize+7;
			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-2].lastBranch[0] = oldSize+1;
			_meshCells[_meshCells.size()-2].lastBranch[2] = oldSize+3;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-1].lastBranch[0] = oldSize+5;
			_meshCells[_meshCells.size()-1].lastBranch[2] = oldSize+7;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[3] = nb;

			_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[ 7].push_back(_meshCells.size()-4);
			_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[ 5].push_back(_meshCells.size()-4);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[0].push_back(_meshCells.size()-4);

			_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[ 7].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[ 5].push_back(_meshCells.size()-3);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[0].push_back(_meshCells.size()-3);

			_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[11].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[ 9].push_back(_meshCells.size()-2);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[0].push_back(_meshCells.size()-2);

			_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[11].push_back(_meshCells.size()-1);
			_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[ 9].push_back(_meshCells.size()-1);
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[0].push_back(_meshCells.size()-1);

//					fprintf(stderr,"\nCreated Cells %li %li %li %li at Wall[1] Size %i",
//							_meshCells.size()-4,_meshCells.size()-3,_meshCells.size()-2,_meshCells.size()-1,_meshCells.back().size);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew+quarter,oynew,oznew,quarter,ll0,ll1,ll0,ll1,ll0,ll1,ll0,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[1] = corner.lastBranch[3] = corner.lastBranch[5] = corner.lastBranch[7] = nb;
			corner.lastBranch[0] = oldSize+0; corner.lastBranch[2] = oldSize+2;
			corner.lastBranch[4] = oldSize+4; corner.lastBranch[6] = oldSize+6;

			_meshCellIndicesBranch[oldSize+1].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+3].indexCorner[5] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+5].indexCorner[3] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+7].indexCorner[1] = _meshCells.size()-1;
			if(oxnew+half!=_n) _meshCellIndicesBranch[nb].indexWallCorner[0].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oxnew+half==_n){
				_boundary.indexWallWall[1].push_back(_meshCells.size()-8);
				_boundary.indexWallWall[1].push_back(_meshCells.size()-7);
				_boundary.indexWallWall[1].push_back(_meshCells.size()-6);
				_boundary.indexWallEdge[1].push_back(_meshCells.size()-5);
				_boundary.indexWallEdge[1].push_back(_meshCells.size()-4);
				_boundary.indexWallEdge[1].push_back(_meshCells.size()-3);
				_boundary.indexWallEdge[1].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[1].push_back(_meshCells.size()-1);
			}
//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Wall[1]",_meshCells.size()-1,quarter);
		}
		wallvector.clear();
	}

//			fprintf(stderr,"2");
	{
		std::vector<size_t> &wallvector = _meshCellIndicesBranch[branch+childoffset].indexWallWall[2];
		if(wallvector.size()>1){
			std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexWallEdge[2];
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexWallCorner[2];
			if(!edgevector.size() || !cornervector.size()){
				fprintf(stderr,"\nERROR: Empty Edge or Corner Cells for Wall 2 [%li %li] at [%i %i %i]",edgevector.size(),cornervector.size(),oxnew,oynew,oznew);
			}
			for(unsigned int i=0;i<wallvector.size();i++){
				MeshCell &cell = _meshCells[wallvector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallWall Cell [%i %i %i] Y[0] is too large: %i > %i at [%i %i %i]",
						cell.ox,cell.oy,cell.oz,cell.size,quarter,oxnew,oynew,oznew);}
				if(cell.oy>=oynew) fprintf(stderr,"\nERROR: Cell Y offset too large: %i >= %i",cell.oy,oynew);
				volumetype cellchildindex = (cell.ox>=oxnew+quarter)*1 + (cell.oz>=oznew+quarter)*4;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexWallWall[2].push_back(wallvector[i]);
				cell.lastBranch[1] = oldSize+cellchildindex;
			}
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallEdge Cell is too large");}
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				if(cell.type == 4){
					if(cell.oz+cell.size==oznew+quarter){
						_meshCellIndicesBranch[oldSize+shiftx+0].indexEdgeEdge[ 2].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shiftx+4].indexEdgeEdge[ 0].push_back(edgevector[i]);
						cell.lastBranch[1] = oldSize+shiftx+0;
						cell.lastBranch[3] = oldSize+shiftx+4;
					}
					else{
						_meshCellIndicesBranch[oldSize+shiftx+shiftz].indexWallEdge[2].push_back(edgevector[i]);
						cell.lastBranch[1] = cell.lastBranch[3] = oldSize+shiftx+shiftz;
					}
				}
				else if(cell.type == 6){
					if(cell.ox+cell.size==oxnew+quarter){
						_meshCellIndicesBranch[oldSize+shiftz+0].indexEdgeEdge[ 9].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shiftz+1].indexEdgeEdge[ 8].push_back(edgevector[i]);
						cell.lastBranch[2] = oldSize+shiftz+0;
						cell.lastBranch[3] = oldSize+shiftz+1;
					}
					else{
						_meshCellIndicesBranch[oldSize+shiftx+shiftz].indexWallEdge[2].push_back(edgevector[i]);
						cell.lastBranch[2] = cell.lastBranch[3] = oldSize+shiftx+shiftz;
					}
				}
				else{fprintf(stderr,"\nERROR: Wall of Type %i has Edges of Type %i",0,cell.type);}
			}
			edgevector.clear();
			for(unsigned int i=0;i<cornervector.size();i++){
//						fprintf(stderr,"\nPassing Corner Cell %li at Wall[2], Leaf %i",cornervector[i],pl);
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallCorner Cell is too large");}
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				if(cell.ox+cell.size==oxnew+quarter && cell.oz+cell.size==oznew+quarter){
//							fprintf(stderr," - > Middle");
					_meshCellIndicesBranch[oldSize+0].indexCorner[5] = cornervector[i];
					_meshCellIndicesBranch[oldSize+1].indexCorner[4] = cornervector[i];
					_meshCellIndicesBranch[oldSize+4].indexCorner[1] = cornervector[i];
					_meshCellIndicesBranch[oldSize+5].indexCorner[0] = cornervector[i];
					cell.lastBranch[2] = oldSize+0; cell.lastBranch[3] = oldSize+1;
					cell.lastBranch[6] = oldSize+4; cell.lastBranch[7] = oldSize+5;
				}
				else{
					if(cell.ox+cell.size==oxnew+quarter){
//								fprintf(stderr," - > Edge X");
						_meshCellIndicesBranch[oldSize+shiftz+0].indexEdgeCorner[ 9].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shiftz+1].indexEdgeCorner[ 8].push_back(cornervector[i]);
						cell.lastBranch[2] = cell.lastBranch[6] = oldSize+shiftz+0;
						cell.lastBranch[3] = cell.lastBranch[7] = oldSize+shiftz+1;
					}
					else if(cell.oz+cell.size==oznew+quarter){
//								fprintf(stderr," - > Edge Z");
						_meshCellIndicesBranch[oldSize+shiftx+0].indexEdgeCorner[ 2].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shiftx+4].indexEdgeCorner[ 0].push_back(cornervector[i]);
						cell.lastBranch[2] = cell.lastBranch[3] = oldSize+shiftx+0;
						cell.lastBranch[6] = cell.lastBranch[7] = oldSize+shiftx+4;
					}
					else{
//								fprintf(stderr," - > Wall");
						_meshCellIndicesBranch[oldSize+shiftx+shiftz].indexWallCorner[2].push_back(cornervector[i]);
						cell.lastBranch[2] = cell.lastBranch[3] =
						cell.lastBranch[6] = cell.lastBranch[7] = oldSize+shiftx+shiftz;
					}
				}
			}
			cornervector.clear();
		}
		else if(wallvector.size()==1){
			_meshCellIndicesBranch[oldSize+0].indexWallWall[2].push_back(wallvector[0]);
			MeshCell &cell = _meshCells[wallvector[0]];
			cell.ox = oxnew; cell.oy = oynew-quarter*(oynew!=0); cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[1] = oldSize+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[0];

			_meshCells.push_back(MeshCell(2,oxnew+quarter,oynew-quarter*(oynew!=0),oznew        ,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(2,oxnew        ,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(2,oxnew+quarter,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll1));

			_meshCells[_meshCells.size()-3].lastBranch[1] = oldSize+1;
			_meshCells[_meshCells.size()-2].lastBranch[1] = oldSize+4;
			_meshCells[_meshCells.size()-1].lastBranch[1] = oldSize+5;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;

			_meshCellIndicesBranch[oldSize+1].indexWallWall[2].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+4].indexWallWall[2].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+5].indexWallWall[2].push_back(_meshCells.size()-1);
			if(oynew!=0) _meshCellIndicesBranch[nb].indexWallWall[3].push_back(_meshCells.size()-3);
			if(oynew!=0) _meshCellIndicesBranch[nb].indexWallWall[3].push_back(_meshCells.size()-2);
			if(oynew!=0) _meshCellIndicesBranch[nb].indexWallWall[3].push_back(_meshCells.size()-1);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}


			_meshCells.push_back(MeshCell(4,oxnew        ,oynew-quarter*(oynew!=0),oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew-quarter*(oynew!=0),oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew        ,oynew-quarter*(oynew!=0),oznew        ,quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(6,oxnew        ,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll0,ll1,ll1));

//					fprintf(stderr,"\nCreated Cells at W[2]%i %li %li %li %li [%li %li %li %li | %i]",
//							quarter,_meshCells.size()-4,_meshCells.size()-3,_meshCells.size()-2,_meshCells.size()-1,
//							oldSize+0,oldSize+1,oldSize+4,oldSize+5,nb);

			_meshCells[_meshCells.size()-4].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[1] = oldSize+0;
			_meshCells[_meshCells.size()-4].lastBranch[3] = oldSize+4;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[1] = oldSize+1;
			_meshCells[_meshCells.size()-3].lastBranch[3] = oldSize+5;

			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[2] = oldSize+0;
			_meshCells[_meshCells.size()-2].lastBranch[3] = oldSize+1;

			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[2] = oldSize+4;
			_meshCells[_meshCells.size()-1].lastBranch[3] = oldSize+5;

			_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[ 2].push_back(_meshCells.size()-4);
			_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[ 0].push_back(_meshCells.size()-4);
			if(oynew!=0) _meshCellIndicesBranch[nb].indexWallEdge[3].push_back(_meshCells.size()-4);

			_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[ 2].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[ 0].push_back(_meshCells.size()-3);
			if(oynew!=0) _meshCellIndicesBranch[nb].indexWallEdge[3].push_back(_meshCells.size()-3);

			_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[ 9].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[ 8].push_back(_meshCells.size()-2);
			if(oynew!=0) _meshCellIndicesBranch[nb].indexWallEdge[3].push_back(_meshCells.size()-2);

			_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[ 9].push_back(_meshCells.size()-1);
			_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[ 8].push_back(_meshCells.size()-1);
			if(oynew!=0) _meshCellIndicesBranch[nb].indexWallEdge[3].push_back(_meshCells.size()-1);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew,oynew-quarter*(oynew!=0),oznew,quarter,ll0,ll0,ll1,ll1,ll0,ll0,ll1,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[1] = corner.lastBranch[4] = corner.lastBranch[5] = nb;
			corner.lastBranch[2] = oldSize+0; corner.lastBranch[3] = oldSize+1;
			corner.lastBranch[6] = oldSize+4; corner.lastBranch[7] = oldSize+5;

			_meshCellIndicesBranch[oldSize+0].indexCorner[5] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+1].indexCorner[4] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+4].indexCorner[1] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+5].indexCorner[0] = _meshCells.size()-1;
			if(oynew!=0) _meshCellIndicesBranch[nb].indexWallCorner[3].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oynew==0){
				_boundary.indexWallWall[2].push_back(_meshCells.size()-8);
				_boundary.indexWallWall[2].push_back(_meshCells.size()-7);
				_boundary.indexWallWall[2].push_back(_meshCells.size()-6);
				_boundary.indexWallEdge[2].push_back(_meshCells.size()-5);
				_boundary.indexWallEdge[2].push_back(_meshCells.size()-4);
				_boundary.indexWallEdge[2].push_back(_meshCells.size()-3);
				_boundary.indexWallEdge[2].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[2].push_back(_meshCells.size()-1);
			}
//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Wall[2]",_meshCells.size()-1,quarter);
		}
		wallvector.clear();
	}

//			fprintf(stderr,"3");
	{
		std::vector<size_t> &wallvector = _meshCellIndicesBranch[branch+childoffset].indexWallWall[3];
		if(wallvector.size()>1){
//					fprintf(stderr,"\nWall 3 Cells at [%i %i %i]%i:",oxnew,oynew,oznew,half);
//					for(unsigned int i=0;i<wallvector.size();i++){
//						MeshCell &cell = _meshCells[wallvector[i]];
//						fprintf(stderr,"\n%i [%i %i %i]%i : %li",cell.type,cell.ox,cell.oy,cell.oz,cell.size,wallvector[i]);
//					}
			std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexWallEdge[3];
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexWallCorner[3];
			if(!edgevector.size() || !cornervector.size()){
				fprintf(stderr,"\nERROR: Empty Edge or Corner Cells for Wall 3 [%li %li] at [%i %i %i]",edgevector.size(),cornervector.size(),oxnew,oynew,oznew);
			}
			for(unsigned int i=0;i<wallvector.size();i++){
				MeshCell &cell = _meshCells[wallvector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallWall Cell [%i %i %i] Y[1] is too large: %i > %i at [%i %i %i]",
						cell.ox,cell.oy,cell.oz,cell.size,quarter,oxnew,oynew,oznew);}
				if(cell.oy+cell.size!=oynew+half) fprintf(stderr,"\nERROR: Cell Y does not fit: %i+%i vs %i+%i",cell.oy,cell.size,oynew,half);
				volumetype cellchildindex = (cell.ox>=oxnew+quarter)*1 + 2 + (cell.oz>=oznew+quarter)*4;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexWallWall[3].push_back(wallvector[i]);
				cell.lastBranch[0] = oldSize+cellchildindex;
			}
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallEdge Cell is too large");}
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				if(cell.type == 4){
					if(cell.oz+cell.size==oznew+quarter){
						_meshCellIndicesBranch[oldSize+shiftx+2].indexEdgeEdge[ 3].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shiftx+6].indexEdgeEdge[ 1].push_back(edgevector[i]);
						cell.lastBranch[0] = oldSize+shiftx+2;
						cell.lastBranch[2] = oldSize+shiftx+6;
					}
					else{
						_meshCellIndicesBranch[oldSize+shiftx+shiftz+2].indexWallEdge[3].push_back(edgevector[i]);
						cell.lastBranch[0] = cell.lastBranch[2] = oldSize+shiftx+shiftz+2;
					}
				}
				else if(cell.type == 6){
					if(cell.ox+cell.size==oxnew+quarter){
						_meshCellIndicesBranch[oldSize+shiftz+2].indexEdgeEdge[11].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shiftz+3].indexEdgeEdge[10].push_back(edgevector[i]);
						cell.lastBranch[0] = oldSize+shiftz+2;
						cell.lastBranch[1] = oldSize+shiftz+3;
					}
					else{
						_meshCellIndicesBranch[oldSize+shiftx+shiftz+2].indexWallEdge[3].push_back(edgevector[i]);
						cell.lastBranch[0] = cell.lastBranch[1] = oldSize+shiftx+shiftz+2;
					}
				}
				else{fprintf(stderr,"\nERROR: Wall of Type %i has Edges of Type %i",0,cell.type);}
			}
			edgevector.clear();
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallCorner Cell is too large");}
//						fprintf(stderr,"\nPassing WallCorner Cell %li Size %i at Wall[3] Size 2*%i",cornervector[i],cell.size,quarter);
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				volumetype shiftz = (cell.oz>=oznew+quarter)*4;
				if(cell.ox+cell.size==oxnew+quarter && cell.oz+cell.size==oznew+quarter){
//							fprintf(stderr,"->Middle");
					_meshCellIndicesBranch[oldSize+2].indexCorner[7] = cornervector[i];
					_meshCellIndicesBranch[oldSize+3].indexCorner[6] = cornervector[i];
					_meshCellIndicesBranch[oldSize+6].indexCorner[3] = cornervector[i];
					_meshCellIndicesBranch[oldSize+7].indexCorner[2] = cornervector[i];
					cell.lastBranch[0] = oldSize+2; cell.lastBranch[1] = oldSize+3;
					cell.lastBranch[4] = oldSize+6; cell.lastBranch[5] = oldSize+7;
				}
				else{
					if(cell.ox+cell.size==oxnew+quarter){
//								fprintf(stderr,"->Edge Z");
						_meshCellIndicesBranch[oldSize+shiftz+2].indexEdgeCorner[11].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shiftz+3].indexEdgeCorner[10].push_back(cornervector[i]);
						cell.lastBranch[0] = cell.lastBranch[4] = oldSize+shiftz+2;
						cell.lastBranch[1] = cell.lastBranch[3] = oldSize+shiftz+3;
					}
					else if(cell.oz+cell.size==oznew+quarter){
//								fprintf(stderr,"->Edge X");
						_meshCellIndicesBranch[oldSize+shiftx+2].indexEdgeCorner[ 3].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shiftx+6].indexEdgeCorner[ 1].push_back(cornervector[i]);
						cell.lastBranch[0] = cell.lastBranch[1] = oldSize+shiftx+2;
						cell.lastBranch[4] = cell.lastBranch[5] = oldSize+shiftx+6;
					}
					else{
//								fprintf(stderr,"-> Wall[3]");
						_meshCellIndicesBranch[oldSize+shiftx+shiftz+2].indexWallCorner[3].push_back(cornervector[i]);
						cell.lastBranch[0] = cell.lastBranch[1] =
						cell.lastBranch[4] = cell.lastBranch[5] = oldSize+shiftx+shiftz+2;
					}
				}
			}
			cornervector.clear();
		}
		else if(wallvector.size()==1){
			_meshCellIndicesBranch[oldSize+2].indexWallWall[3].push_back(wallvector[0]);
			MeshCell &cell = _meshCells[wallvector[0]];
			cell.ox = oxnew; cell.oy = oynew+quarter; cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[0] = oldSize+2;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[1];

			_meshCells.push_back(MeshCell(2,oxnew+quarter,oynew+quarter,oznew        ,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(2,oxnew        ,oynew+quarter,oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(2,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1));

			_meshCells[_meshCells.size()-3].lastBranch[0] = oldSize+3;
			_meshCells[_meshCells.size()-2].lastBranch[0] = oldSize+6;
			_meshCells[_meshCells.size()-1].lastBranch[0] = oldSize+7;

			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;

			_meshCellIndicesBranch[oldSize+3].indexWallWall[3].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+6].indexWallWall[3].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+7].indexWallWall[3].push_back(_meshCells.size()-1);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].indexWallWall[2].push_back(_meshCells.size()-3);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].indexWallWall[2].push_back(_meshCells.size()-2);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].indexWallWall[2].push_back(_meshCells.size()-1);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}


			_meshCells.push_back(MeshCell(4,oxnew        ,oynew+quarter,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew+quarter,oznew        ,quarter,ll0,ll1,ll0,ll1));
			_meshCells.push_back(MeshCell(6,oxnew        ,oynew+quarter,oznew        ,quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(6,oxnew        ,oynew+quarter,oznew+quarter,quarter,ll0,ll0,ll1,ll1));

//					fprintf(stderr,"\nCreated Cells at W[3]%i %li %li %li %li [%li %li %li %li | %i]",
//							quarter,_meshCells.size()-4,_meshCells.size()-3,_meshCells.size()-2,_meshCells.size()-1,
//							oldSize+2,oldSize+3,oldSize+6,oldSize+7,nb);

			_meshCells[_meshCells.size()-4].lastBranch[0] = oldSize+2;
			_meshCells[_meshCells.size()-4].lastBranch[2] = oldSize+6;
			_meshCells[_meshCells.size()-4].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-3].lastBranch[0] = oldSize+3;
			_meshCells[_meshCells.size()-3].lastBranch[2] = oldSize+7;
			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-2].lastBranch[0] = oldSize+2;
			_meshCells[_meshCells.size()-2].lastBranch[1] = oldSize+3;
			_meshCells[_meshCells.size()-2].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-1].lastBranch[0] = oldSize+6;
			_meshCells[_meshCells.size()-1].lastBranch[1] = oldSize+7;
			_meshCells[_meshCells.size()-1].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[3] = nb;

			_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[ 3].push_back(_meshCells.size()-4);
			_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[ 1].push_back(_meshCells.size()-4);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[2].push_back(_meshCells.size()-4);

			_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[ 3].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[ 1].push_back(_meshCells.size()-3);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[2].push_back(_meshCells.size()-3);

			_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[11].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[10].push_back(_meshCells.size()-2);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[2].push_back(_meshCells.size()-2);

			_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[11].push_back(_meshCells.size()-1);
			_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[10].push_back(_meshCells.size()-1);
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[2].push_back(_meshCells.size()-1);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew,oynew+quarter,oznew,quarter,ll0,ll0,ll1,ll1,ll0,ll0,ll1,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[2] = corner.lastBranch[3] = corner.lastBranch[6] = corner.lastBranch[7] = nb;
			corner.lastBranch[0] = oldSize+2; corner.lastBranch[1] = oldSize+3;
			corner.lastBranch[4] = oldSize+6; corner.lastBranch[5] = oldSize+7;

			_meshCellIndicesBranch[oldSize+2].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+3].indexCorner[6] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+6].indexCorner[3] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+7].indexCorner[2] = _meshCells.size()-1;
			if(oynew+half!=_n) _meshCellIndicesBranch[nb].indexWallCorner[2].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oynew+half==_n){
				_boundary.indexWallWall[3].push_back(_meshCells.size()-8);
				_boundary.indexWallWall[3].push_back(_meshCells.size()-7);
				_boundary.indexWallWall[3].push_back(_meshCells.size()-6);
				_boundary.indexWallEdge[3].push_back(_meshCells.size()-5);
				_boundary.indexWallEdge[3].push_back(_meshCells.size()-4);
				_boundary.indexWallEdge[3].push_back(_meshCells.size()-3);
				_boundary.indexWallEdge[3].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[3].push_back(_meshCells.size()-1);
			}
//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Wall[3]",_meshCells.size()-1,quarter);
		}
		wallvector.clear();
	}

//			fprintf(stderr,"4");
	{
		std::vector<size_t> &wallvector = _meshCellIndicesBranch[branch+childoffset].indexWallWall[4];
		if(wallvector.size()>1){
			std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexWallEdge[4];
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexWallCorner[4];

			if(!edgevector.size() || !cornervector.size()){
				fprintf(stderr,"\nERROR: Empty Edge or Corner Cells for Wall 4 [%li %li]",edgevector.size(),cornervector.size());
			}
			for(unsigned int i=0;i<wallvector.size();i++){
				MeshCell &cell = _meshCells[wallvector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallWall Cell [%i %i %i] Z[0] is too large: %i > %i at [%i %i %i]",
						cell.ox,cell.oy,cell.oz,cell.size,quarter,oxnew,oynew,oznew);}
				if(cell.oz>=oznew) fprintf(stderr,"\nERROR: Cell Z offset too large: %i >= %i",cell.oz,oznew);
				volumetype cellchildindex = (cell.ox>=oxnew+quarter)*1 + (cell.oy>=oynew+quarter)*2;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexWallWall[4].push_back(wallvector[i]);
				cell.lastBranch[1] = oldSize+cellchildindex;
			}
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallEdge Cell is too large");}
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				if(cell.type == 4){
					if(cell.oy+cell.size==oynew+quarter){
						_meshCellIndicesBranch[oldSize+shiftx+0].indexEdgeEdge[ 1].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shiftx+2].indexEdgeEdge[ 0].push_back(edgevector[i]);
						cell.lastBranch[2] = oldSize+shiftx+0;
						cell.lastBranch[3] = oldSize+shiftx+2;
					}
					else{
						_meshCellIndicesBranch[oldSize+shiftx+shifty].indexWallEdge[4].push_back(edgevector[i]);
						cell.lastBranch[2] = cell.lastBranch[3] = oldSize+shiftx+shifty;
					}
				}
				else if(cell.type == 5){
					if(cell.ox+cell.size==oxnew+quarter){
						_meshCellIndicesBranch[oldSize+shifty+0].indexEdgeEdge[ 5].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shifty+1].indexEdgeEdge[ 4].push_back(edgevector[i]);
						cell.lastBranch[2] = oldSize+shifty+0;
						cell.lastBranch[3] = oldSize+shifty+1;
					}
					else{
						_meshCellIndicesBranch[oldSize+shiftx+shifty].indexWallEdge[4].push_back(edgevector[i]);
						cell.lastBranch[2] = cell.lastBranch[3] = oldSize+shiftx+shifty;
					}
				}
				else{fprintf(stderr,"\nERROR: Wall of Type %i has Edges of Type %i",0,cell.type);}
			}
			edgevector.clear();
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallCorner Cell is too large");}
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				if(cell.ox+cell.size==oxnew+quarter && cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[oldSize+0].indexCorner[3] = cornervector[i];
					_meshCellIndicesBranch[oldSize+1].indexCorner[2] = cornervector[i];
					_meshCellIndicesBranch[oldSize+2].indexCorner[1] = cornervector[i];
					_meshCellIndicesBranch[oldSize+3].indexCorner[0] = cornervector[i];
					cell.lastBranch[4] = oldSize+0; cell.lastBranch[5] = oldSize+1;
					cell.lastBranch[6] = oldSize+2; cell.lastBranch[7] = oldSize+3;
				}
				else{
					if(cell.ox+cell.size==oxnew+quarter){
						_meshCellIndicesBranch[oldSize+shifty+0].indexEdgeCorner[ 5].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shifty+1].indexEdgeCorner[ 4].push_back(cornervector[i]);
						cell.lastBranch[4] = cell.lastBranch[6] = oldSize+shifty+0;
						cell.lastBranch[5] = cell.lastBranch[7] = oldSize+shifty+1;
					}
					else if(cell.oy+cell.size==oynew+quarter){
						_meshCellIndicesBranch[oldSize+shiftx+0].indexEdgeCorner[ 1].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shiftx+2].indexEdgeCorner[ 0].push_back(cornervector[i]);
						cell.lastBranch[4] = cell.lastBranch[5] = oldSize+shiftx+0;
						cell.lastBranch[6] = cell.lastBranch[7] = oldSize+shiftx+2;
					}
					else{
						_meshCellIndicesBranch[oldSize+shiftx+shifty].indexWallCorner[4].push_back(cornervector[i]);
						cell.lastBranch[4] = cell.lastBranch[5] =
						cell.lastBranch[6] = cell.lastBranch[7] = oldSize+shiftx+shifty;
					}
				}
			}
			cornervector.clear();
		}
		else if(wallvector.size()==1){
			_meshCellIndicesBranch[oldSize+0].indexWallWall[4].push_back(wallvector[0]);
			MeshCell &cell = _meshCells[wallvector[0]];
//					if(cell.oz+cell.size==640){
//						fprintf(stderr,"\nSplitting Cell %li [%i %i %i]%i at Wall[4]:",
//								wallvector[0],cell.ox,cell.oy,cell.oz,cell.size);
//					}
			cell.ox = oxnew; cell.oy = oynew; cell.oz = oznew-quarter*(oznew!=0);
			cell.size = quarter; cell.lastBranch[1] = oldSize+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[0];

			_meshCells.push_back(MeshCell(3,oxnew+quarter,oynew        ,oznew-quarter*(oznew!=0),quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(3,oxnew        ,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(3,oxnew+quarter,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll1));

//					if(cell.oz+cell.size==640){
//						fprintf(stderr," %li %li %li",_meshCells.size()-3,_meshCells.size()-2,_meshCells.size()-1);
//					}

			_meshCells[_meshCells.size()-3].lastBranch[1] = oldSize+1;
			_meshCells[_meshCells.size()-2].lastBranch[1] = oldSize+2;
			_meshCells[_meshCells.size()-1].lastBranch[1] = oldSize+3;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;

			_meshCellIndicesBranch[oldSize+1].indexWallWall[4].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+2].indexWallWall[4].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+3].indexWallWall[4].push_back(_meshCells.size()-1);
			if(oznew!=0) _meshCellIndicesBranch[nb].indexWallWall[5].push_back(_meshCells.size()-3);
			if(oznew!=0) _meshCellIndicesBranch[nb].indexWallWall[5].push_back(_meshCells.size()-2);
			if(oznew!=0) _meshCellIndicesBranch[nb].indexWallWall[5].push_back(_meshCells.size()-1);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}


			_meshCells.push_back(MeshCell(4,oxnew        ,oynew        ,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew        ,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(5,oxnew        ,oynew        ,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(5,oxnew        ,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1));

//					fprintf(stderr,"\nCreated Cells at W[4]%i %li %li %li %li [%li %li %li %li | %i]",
//							quarter,_meshCells.size()-4,_meshCells.size()-3,_meshCells.size()-2,_meshCells.size()-1,
//							oldSize+0,oldSize+1,oldSize+2,oldSize+3,nb);

			_meshCells[_meshCells.size()-4].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[2] = oldSize+0;
			_meshCells[_meshCells.size()-4].lastBranch[3] = oldSize+2;

			_meshCells[_meshCells.size()-3].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[2] = oldSize+1;
			_meshCells[_meshCells.size()-3].lastBranch[3] = oldSize+3;

			_meshCells[_meshCells.size()-2].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[2] = oldSize+0;
			_meshCells[_meshCells.size()-2].lastBranch[3] = oldSize+1;

			_meshCells[_meshCells.size()-1].lastBranch[0] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[2] = oldSize+2;
			_meshCells[_meshCells.size()-1].lastBranch[3] = oldSize+3;

			_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[ 1].push_back(_meshCells.size()-4);
			_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[ 0].push_back(_meshCells.size()-4);
			if(oznew!=0) _meshCellIndicesBranch[nb].indexWallEdge[5].push_back(_meshCells.size()-4);

			_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[ 1].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[ 0].push_back(_meshCells.size()-3);
			if(oznew!=0) _meshCellIndicesBranch[nb].indexWallEdge[5].push_back(_meshCells.size()-3);

			_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[ 5].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[ 4].push_back(_meshCells.size()-2);
			if(oznew!=0) _meshCellIndicesBranch[nb].indexWallEdge[5].push_back(_meshCells.size()-2);

			_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[ 5].push_back(_meshCells.size()-1);
			_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[ 4].push_back(_meshCells.size()-1);
			if(oznew!=0) _meshCellIndicesBranch[nb].indexWallEdge[5].push_back(_meshCells.size()-1);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew,oynew,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll0,ll0,ll1,ll1,ll1,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[1] = corner.lastBranch[2] = corner.lastBranch[3] = nb;
			corner.lastBranch[4] = oldSize+0; corner.lastBranch[5] = oldSize+1;
			corner.lastBranch[6] = oldSize+2; corner.lastBranch[7] = oldSize+3;

			_meshCellIndicesBranch[oldSize+0].indexCorner[3] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+1].indexCorner[2] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+2].indexCorner[1] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+3].indexCorner[0] = _meshCells.size()-1;
			if(oznew!=0) _meshCellIndicesBranch[nb].indexWallCorner[5].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oznew==0){
				_boundary.indexWallWall[4].push_back(_meshCells.size()-8);
				_boundary.indexWallWall[4].push_back(_meshCells.size()-7);
				_boundary.indexWallWall[4].push_back(_meshCells.size()-6);
				_boundary.indexWallEdge[4].push_back(_meshCells.size()-5);
				_boundary.indexWallEdge[4].push_back(_meshCells.size()-4);
				_boundary.indexWallEdge[4].push_back(_meshCells.size()-3);
				_boundary.indexWallEdge[4].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[4].push_back(_meshCells.size()-1);
			}
//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Wall[4]",_meshCells.size()-1,quarter);
		}
		wallvector.clear();
	}

//			fprintf(stderr,"5");
	{
		std::vector<size_t> &wallvector = _meshCellIndicesBranch[branch+childoffset].indexWallWall[5];
		if(wallvector.size()>1){
//					fprintf(stderr,"\nWall 5 Cells at [%i %i %i]%i:",oxnew,oynew,oznew,half);
//					for(unsigned int i=0;i<wallvector.size();i++){
//						MeshCell &cell = _meshCells[wallvector[i]];
//						fprintf(stderr,"\n%i [%i %i %i]%i : %li",cell.type,cell.ox,cell.oy,cell.oz,cell.size,wallvector[i]);
//					}
			std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexWallEdge[5];
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexWallCorner[5];
			if(!edgevector.size() || !cornervector.size()){
				fprintf(stderr,"\nERROR: Empty Edge or Corner Cells for Wall 5 [%li %li]",edgevector.size(),cornervector.size());
			}
			for(unsigned int i=0;i<wallvector.size();i++){
				MeshCell &cell = _meshCells[wallvector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallWall Cell [%i %i %i] Z[1] is too large: %i > %i at [%i %i %i]",
						cell.ox,cell.oy,cell.oz,cell.size,quarter,oxnew,oynew,oznew);}
				if(cell.oz+cell.size!=oznew+half)
					fprintf(stderr,"\nERROR: Cell Z Nr %li at %i (%i %i %i) does not fit: %i+%i vs %i+%i",
							wallvector[i],branch+childoffset,oxnew,oynew,oznew,
							cell.oz,cell.size,oznew,half);
				volumetype cellchildindex = (cell.ox>=oxnew+quarter)*1 + (cell.oy>=oynew+quarter)*2 + 4;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexWallWall[5].push_back(wallvector[i]);
				cell.lastBranch[0] = oldSize+cellchildindex;
			}
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallEdge Cell is too large");}
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				if(cell.type == 4){
					if(cell.oy+cell.size==oynew+quarter){
						_meshCellIndicesBranch[oldSize+shiftx+4].indexEdgeEdge[ 3].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shiftx+6].indexEdgeEdge[ 2].push_back(edgevector[i]);
						cell.lastBranch[0] = oldSize+shiftx+4;
						cell.lastBranch[1] = oldSize+shiftx+6;
					}
					else{
						_meshCellIndicesBranch[oldSize+shiftx+shifty+4].indexWallEdge[5].push_back(edgevector[i]);
						cell.lastBranch[0] = cell.lastBranch[1] = oldSize+shiftx+shifty+4;
					}
				}
				else if(cell.type == 5){
					if(cell.ox+cell.size==oxnew+quarter){
						_meshCellIndicesBranch[oldSize+shifty+4].indexEdgeEdge[ 7].push_back(edgevector[i]);
						_meshCellIndicesBranch[oldSize+shifty+5].indexEdgeEdge[ 6].push_back(edgevector[i]);
						cell.lastBranch[0] = oldSize+shifty+4;
						cell.lastBranch[1] = oldSize+shifty+5;
					}
					else{
						_meshCellIndicesBranch[oldSize+shiftx+shifty+4].indexWallEdge[5].push_back(edgevector[i]);
						cell.lastBranch[0] = cell.lastBranch[1] = oldSize+shiftx+shifty+4;
					}
				}
				else{fprintf(stderr,"\nERROR: Wall of Type %i has Edges of Type %i",0,cell.type);}
			}
			edgevector.clear();
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR:WallCorner Cell is too large");}
				volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
				volumetype shifty = (cell.oy>=oynew+quarter)*2;
				if(cell.ox+cell.size==oxnew+quarter && cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[oldSize+4].indexCorner[7] = cornervector[i];
					_meshCellIndicesBranch[oldSize+5].indexCorner[6] = cornervector[i];
					_meshCellIndicesBranch[oldSize+6].indexCorner[5] = cornervector[i];
					_meshCellIndicesBranch[oldSize+7].indexCorner[4] = cornervector[i];
					cell.lastBranch[0] = oldSize+4; cell.lastBranch[1] = oldSize+5;
					cell.lastBranch[2] = oldSize+6; cell.lastBranch[3] = oldSize+7;
				}
				else{
					if(cell.ox+cell.size==oxnew+quarter){
						_meshCellIndicesBranch[oldSize+shifty+4].indexEdgeCorner[ 7].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shifty+5].indexEdgeCorner[ 6].push_back(cornervector[i]);
						cell.lastBranch[0] = cell.lastBranch[2] = oldSize+shifty+4;
						cell.lastBranch[1] = cell.lastBranch[3] = oldSize+shifty+5;
					}
					else if(cell.oy+cell.size==oynew+quarter){
						_meshCellIndicesBranch[oldSize+shiftx+4].indexEdgeCorner[ 3].push_back(cornervector[i]);
						_meshCellIndicesBranch[oldSize+shiftx+6].indexEdgeCorner[ 2].push_back(cornervector[i]);
						cell.lastBranch[0] = cell.lastBranch[1] = oldSize+shiftx+4;
						cell.lastBranch[2] = cell.lastBranch[3] = oldSize+shiftx+6;
					}
					else{
						_meshCellIndicesBranch[oldSize+shiftx+shifty+4].indexWallCorner[5].push_back(cornervector[i]);
						cell.lastBranch[4] = cell.lastBranch[5] =
						cell.lastBranch[6] = cell.lastBranch[7] = oldSize+shiftx+shifty+4;
					}
				}
			}
			cornervector.clear();
		}
		else if(wallvector.size()==1){
			_meshCellIndicesBranch[oldSize+4].indexWallWall[5].push_back(wallvector[0]);
			MeshCell &cell = _meshCells[wallvector[0]];
//					if(cell.oz+cell.size==640){
//						fprintf(stderr,"\nSplitting Cell %li [%i %i %i]%i at Wall[5]:",
//								wallvector[0],cell.ox,cell.oy,cell.oz,cell.size);
//					}
			cell.ox = oxnew; cell.oy = oynew; cell.oz = oznew+quarter;
			cell.size = quarter; cell.lastBranch[0] = oldSize+4;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype nb = cell.lastBranch[1];

			_meshCells.push_back(MeshCell(3,oxnew+quarter,oynew        ,oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(3,oxnew        ,oynew+quarter,oznew+quarter,quarter,ll0,ll1));
			_meshCells.push_back(MeshCell(3,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1));

//					if(cell.oz+cell.size==640){
//						fprintf(stderr," %li %li %li",_meshCells.size()-3,_meshCells.size()-2,_meshCells.size()-1);
//					}

			_meshCells[_meshCells.size()-3].lastBranch[0] = oldSize+5;
			_meshCells[_meshCells.size()-2].lastBranch[0] = oldSize+6;
			_meshCells[_meshCells.size()-1].lastBranch[0] = oldSize+7;

			_meshCells[_meshCells.size()-3].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[1] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[1] = nb;

			_meshCellIndicesBranch[oldSize+5].indexWallWall[5].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+6].indexWallWall[5].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+7].indexWallWall[5].push_back(_meshCells.size()-1);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].indexWallWall[4].push_back(_meshCells.size()-3);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].indexWallWall[4].push_back(_meshCells.size()-2);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].indexWallWall[4].push_back(_meshCells.size()-1);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}


			_meshCells.push_back(MeshCell(4,oxnew        ,oynew        ,oznew+quarter,quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew        ,oznew+quarter,quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(5,oxnew        ,oynew        ,oznew+quarter,quarter,ll0,ll0,ll1,ll1));
			_meshCells.push_back(MeshCell(5,oxnew        ,oynew+quarter,oznew+quarter,quarter,ll0,ll0,ll1,ll1));

//					fprintf(stderr,"\nCreated Cells at W[5]%i %li %li %li %li [%li %li %li %li | %i]",
//							quarter,_meshCells.size()-4,_meshCells.size()-3,_meshCells.size()-2,_meshCells.size()-1,
//							oldSize+4,oldSize+5,oldSize+6,oldSize+7,nb);

			_meshCells[_meshCells.size()-4].lastBranch[0] = oldSize+4;
			_meshCells[_meshCells.size()-4].lastBranch[1] = oldSize+6;
			_meshCells[_meshCells.size()-4].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-4].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-3].lastBranch[0] = oldSize+5;
			_meshCells[_meshCells.size()-3].lastBranch[1] = oldSize+7;
			_meshCells[_meshCells.size()-3].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-3].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-2].lastBranch[0] = oldSize+4;
			_meshCells[_meshCells.size()-2].lastBranch[1] = oldSize+5;
			_meshCells[_meshCells.size()-2].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-2].lastBranch[3] = nb;

			_meshCells[_meshCells.size()-1].lastBranch[0] = oldSize+6;
			_meshCells[_meshCells.size()-1].lastBranch[1] = oldSize+7;;
			_meshCells[_meshCells.size()-1].lastBranch[2] = nb;
			_meshCells[_meshCells.size()-1].lastBranch[3] = nb;

			_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[ 3].push_back(_meshCells.size()-4);
			_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[ 2].push_back(_meshCells.size()-4);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[4].push_back(_meshCells.size()-4);

			_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[ 3].push_back(_meshCells.size()-3);
			_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[ 2].push_back(_meshCells.size()-3);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[4].push_back(_meshCells.size()-3);

			_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[ 7].push_back(_meshCells.size()-2);
			_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[ 6].push_back(_meshCells.size()-2);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[4].push_back(_meshCells.size()-2);

			_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[ 7].push_back(_meshCells.size()-1);
			_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[ 6].push_back(_meshCells.size()-1);
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].indexWallEdge[4].push_back(_meshCells.size()-1);

			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-4);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-3);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-2);
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew,oynew,oznew+quarter,quarter,ll0,ll0,ll0,ll0,ll1,ll1,ll1,ll1));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[4] = corner.lastBranch[5] = corner.lastBranch[6] = corner.lastBranch[7] = nb;
			corner.lastBranch[0] = oldSize+4; corner.lastBranch[1] = oldSize+5;
			corner.lastBranch[2] = oldSize+6; corner.lastBranch[3] = oldSize+7;

			_meshCellIndicesBranch[oldSize+4].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+5].indexCorner[6] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+6].indexCorner[5] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+7].indexCorner[4] = _meshCells.size()-1;
			if(oznew+half!=_n) _meshCellIndicesBranch[nb].indexWallCorner[4].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oznew+half==_n){
				_boundary.indexWallWall[5].push_back(_meshCells.size()-8);
				_boundary.indexWallWall[5].push_back(_meshCells.size()-7);
				_boundary.indexWallWall[5].push_back(_meshCells.size()-6);
				_boundary.indexWallEdge[5].push_back(_meshCells.size()-5);
				_boundary.indexWallEdge[5].push_back(_meshCells.size()-4);
				_boundary.indexWallEdge[5].push_back(_meshCells.size()-3);
				_boundary.indexWallEdge[5].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[5].push_back(_meshCells.size()-1);
			}
//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Wall[5]",_meshCells.size()-1,quarter);
		}
		wallvector.clear();
	}
//			fprintf(stderr,"!");

//			fprintf(stderr,"E");
//			fprintf(stderr," 0");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[0];
		if(edgevector.size()>1){
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] X[0] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if((oynew && cell.oy>=oynew) || (oznew && cell.oz>=oznew))
					fprintf(stderr,"\nERROR: Edge Cell %li at X[0] offsets Y or Z too large: %i >= %i || %i >= %i",
							edgevector[i],cell.oy,oynew,cell.oz,oznew);
				volumetype cellchildindex = (cell.ox>=oxnew+quarter);
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[0].push_back(edgevector[i]);
				cell.lastBranch[3] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[0];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge X[0]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at X[0] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.ox+cell.size==oxnew+quarter){
					_meshCellIndicesBranch[oldSize+0].indexCorner[1] = cornervector[i];
					_meshCellIndicesBranch[oldSize+1].indexCorner[0] = cornervector[i];
					cell.lastBranch[6] = oldSize+0; cell.lastBranch[7] = oldSize+1;
				}
				else{
					volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
					_meshCellIndicesBranch[oldSize+shiftx].indexEdgeCorner[0].push_back(cornervector[i]);
					cell.lastBranch[6] = cell.lastBranch[7] = oldSize+shiftx;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
			_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[0].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at X[0] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew; cell.oy = oynew-quarter*(oynew!=0); cell.oz = oznew-quarter*(oznew!=0);
			cell.size = quarter; cell.lastBranch[3] = oldSize+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];

			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew-quarter*(oynew!=0),oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off X[0] Cell %li to %li",edgevector[0],_meshCells.size()-1);

			_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[0].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = oldSize+1;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].indexWallEdge[5].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].indexEdgeEdge[2].push_back(_meshCells.size()-1);
			}
			if(oynew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].indexWallEdge[3].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].indexEdgeEdge[1].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].indexEdgeEdge[3].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew,oynew-quarter*(oynew!=0),oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1,ll2,ll2,ll3,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[1] = nb0;
			corner.lastBranch[2] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = corner.lastBranch[5] = nb2;
			corner.lastBranch[6] = oldSize+0;
			corner.lastBranch[7] = oldSize+1;
			_meshCellIndicesBranch[oldSize+0].indexCorner[1] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+1].indexCorner[0] = _meshCells.size()-1;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].indexWallCorner[5].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].indexEdgeCorner[2].push_back(_meshCells.size()-1);
			}
			if(oynew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].indexWallCorner[3].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].indexEdgeCorner[1].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].indexEdgeCorner[3].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oynew==0){
				if(oznew==0){
					_boundary.indexEdgeEdge[0].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[0].push_back(_meshCells.size()-1);
				}
				else{
					_boundary.indexWallEdge[2].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[2].push_back(_meshCells.size()-1);
				}
			}
			else if(oznew==0){
				_boundary.indexWallEdge[4].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[4].push_back(_meshCells.size()-1);
			}
//					if(oynew==0 && oznew==0){
//						_boundary.indexEdgeEdge[0].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[0].push_back(_meshCells.size()-1);
//					}

//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[0]",_meshCells.size()-1,quarter);
		}
		edgevector.clear();
	}

//			fprintf(stderr," 1");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[1];
		if(edgevector.size()>1){
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] X[1] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if(cell.oy+cell.size>oynew+half || (oznew && cell.oz>=oznew))
					fprintf(stderr,"\nERROR: Edge Cell %li at X[1] offsets Y or Z do not fit: %i+%i > %i+%i || %i >= %i",
							edgevector[i],cell.oy,cell.size,oynew,half,cell.oz,oznew);
				volumetype cellchildindex = (cell.ox>=oxnew+quarter) + 2;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[1].push_back(edgevector[i]);
				cell.lastBranch[2] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[1];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge X[1]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at X[1] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.ox+cell.size==oxnew+quarter){
					_meshCellIndicesBranch[oldSize+2].indexCorner[3] = cornervector[i];
					_meshCellIndicesBranch[oldSize+3].indexCorner[2] = cornervector[i];
					cell.lastBranch[4] = oldSize+2; cell.lastBranch[5] = oldSize+3;
				}
				else{
					volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
					_meshCellIndicesBranch[oldSize+shiftx+2].indexEdgeCorner[1].push_back(cornervector[i]);
					cell.lastBranch[4] = cell.lastBranch[5] = oldSize+shiftx+2;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
			_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[1].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at X[1] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew; cell.oy = oynew+quarter; cell.oz = oznew-quarter*(oznew!=0);
			cell.size = quarter; cell.lastBranch[2] = oldSize+2;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off X[1] Cell %li to %li",edgevector[0],_meshCells.size()-1);

			_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[1].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = oldSize+3;
			edge.lastBranch[3] = nb3;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].indexWallEdge[5].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeEdge[3].push_back(_meshCells.size()-1);
			}
			if(oynew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].indexWallEdge[2].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeEdge[0].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].indexEdgeEdge[2].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll0,ll1,ll1,ll2,ll2,ll3,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[1] = nb0;
			corner.lastBranch[2] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = oldSize+2;
			corner.lastBranch[5] = oldSize+3;
			corner.lastBranch[6] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[oldSize+2].indexCorner[3] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+3].indexCorner[2] = _meshCells.size()-1;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].indexWallCorner[5].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeCorner[3].push_back(_meshCells.size()-1);
			}
			if(oynew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].indexWallCorner[2].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeCorner[0].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].indexEdgeCorner[2].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oynew+half==_n){
				if(oznew==0){
					_boundary.indexEdgeEdge[1].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[1].push_back(_meshCells.size()-1);
				}
				else{
					_boundary.indexWallEdge[3].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[3].push_back(_meshCells.size()-1);
				}
			}
			else if(oznew==0){
				_boundary.indexWallEdge[4].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[4].push_back(_meshCells.size()-1);
			}
//					if(oynew+half==_n && oznew==0){
//						_boundary.indexEdgeEdge[1].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[1].push_back(_meshCells.size()-1);
//					}

//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[1]",_meshCells.size()-1,quarter);
		}
		edgevector.clear();
	}

//			fprintf(stderr," 2");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[2];
		if(edgevector.size()>1){
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] X[2] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if((oynew && cell.oy>=oynew) || cell.oz+cell.size>oznew+half)
					fprintf(stderr,"\nERROR: Edge Cell %li at X[2] offsets Y or Z do not fit: %i >= %i || %i+%i > %i+%i",
							edgevector[i],cell.oy,oynew,cell.oz,cell.size,oznew,half);
				volumetype cellchildindex = (cell.ox>=oxnew+quarter) + 4;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[2].push_back(edgevector[i]);
				cell.lastBranch[1] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[2];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge X[2]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at X[2] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.ox+cell.size==oxnew+quarter){
					_meshCellIndicesBranch[oldSize+4].indexCorner[5] = cornervector[i];
					_meshCellIndicesBranch[oldSize+5].indexCorner[4] = cornervector[i];
					cell.lastBranch[2] = oldSize+4; cell.lastBranch[3] = oldSize+5;
				}
				else{
					volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
					_meshCellIndicesBranch[oldSize+shiftx+4].indexEdgeCorner[2].push_back(cornervector[i]);
					cell.lastBranch[2] = cell.lastBranch[3] = oldSize+shiftx+4;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
			_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[2].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at X[2] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew; cell.oy = oynew-quarter*(oynew!=0); cell.oz = oznew+quarter;
			cell.size = quarter; cell.lastBranch[1] = oldSize+4;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off X[2] Cell %li to %li [%i|%i %i %i]",
//							edgevector[0],_meshCells.size()-1,branch+childoffset,nb0,nb2,nb3);

			_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[2].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = oldSize+5;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oynew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].indexWallEdge[3].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeEdge[3].push_back(_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].indexWallEdge[4].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeEdge[0].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].indexEdgeEdge[1].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll0,ll1,ll1,ll2,ll2,ll3,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[1] = nb0;
			corner.lastBranch[2] = oldSize+4;
			corner.lastBranch[3] = oldSize+5;
			corner.lastBranch[4] = corner.lastBranch[5] = nb2;
			corner.lastBranch[6] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[oldSize+4].indexCorner[5] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+5].indexCorner[4] = _meshCells.size()-1;
			if(oynew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].indexWallCorner[3].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeCorner[3].push_back(_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].indexWallCorner[4].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeCorner[0].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].indexEdgeCorner[1].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oynew==0){
				if(oznew+half==_n){
					_boundary.indexEdgeEdge[2].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[2].push_back(_meshCells.size()-1);
				}
				else{
					_boundary.indexWallEdge[2].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[2].push_back(_meshCells.size()-1);
				}
			}
			else if(oznew+half==_n){
				_boundary.indexWallEdge[5].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[5].push_back(_meshCells.size()-1);
			}
//					if(oynew==0 && oznew+half==_n){
//						_boundary.indexEdgeEdge[2].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[2].push_back(_meshCells.size()-1);
//					}

//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[2]",_meshCells.size()-1,quarter);
		}
		edgevector.clear();
	}

//			fprintf(stderr," 3");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[3];
		if(edgevector.size()>1){
//					fprintf(stderr,"\n%li edges:",edgevector.size());
//					for(unsigned int i=0;i<edgevector.size();i++){
//						MeshCell &cell = _meshCells[edgevector[i]];
//						fprintf(stderr,"\n%i [%i %i %i]%i %li",cell.type,cell.ox,cell.oy,cell.oz,cell.size,edgevector[i]);
//					}
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] X[3] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if(cell.oy+cell.size>oynew+half || cell.oz+cell.size>oznew+half)
					fprintf(stderr,"\nERROR: Edge Cell %li at X[3] offsets Y or Z do not fit: %i+%i > %i+%i || %i+%i > %i+%i",
							edgevector[i],cell.oy,cell.size,oynew,half,cell.oz,cell.size,oznew,half);
				volumetype cellchildindex = (cell.ox>=oxnew+quarter) + 6;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[3].push_back(edgevector[i]);
				cell.lastBranch[0] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[3];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge X[3]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at X[3] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.ox+cell.size==oxnew+quarter){
					_meshCellIndicesBranch[oldSize+6].indexCorner[7] = cornervector[i];
					_meshCellIndicesBranch[oldSize+7].indexCorner[6] = cornervector[i];
					cell.lastBranch[0] = oldSize+6; cell.lastBranch[1] = oldSize+7;
				}
				else{
					volumetype shiftx = (cell.ox>=oxnew+quarter)*1;
					_meshCellIndicesBranch[oldSize+shiftx+6].indexEdgeCorner[3].push_back(cornervector[i]);
					cell.lastBranch[0] = cell.lastBranch[1] = oldSize+shiftx+6;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
			_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[3].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at X[3] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew; cell.oy = oynew+quarter; cell.oz = oznew+quarter;
			cell.size = quarter; cell.lastBranch[0] = oldSize+6;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(4,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off X[3] Cell %li to %li",edgevector[0],_meshCells.size()-1);

			_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[3].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = oldSize+7;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oynew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb1].indexWallEdge[2].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].indexEdgeEdge[2].push_back(_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb2].indexWallEdge[4].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].indexEdgeEdge[1].push_back(_meshCells.size()-1);
			}
			if(nb1!=nb3 && nb2!=nb3) _meshCellIndicesBranch[nb3].indexEdgeEdge[0].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew,oynew+quarter,oznew+quarter,quarter,ll0,ll0,ll1,ll1,ll2,ll2,ll3,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = oldSize+6;
			corner.lastBranch[1] = oldSize+7;
			corner.lastBranch[2] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = corner.lastBranch[5] = nb2;
			corner.lastBranch[6] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[oldSize+6].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+7].indexCorner[6] = _meshCells.size()-1;
			if(oynew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb1].indexWallCorner[2].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].indexEdgeCorner[2].push_back(_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb2].indexWallCorner[4].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].indexEdgeCorner[1].push_back(_meshCells.size()-1);
			}
			if(nb1!=nb3 && nb2!=nb3) _meshCellIndicesBranch[nb3].indexEdgeCorner[0].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oynew+half==_n){
				if(oznew+half==_n){
					_boundary.indexEdgeEdge[3].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[3].push_back(_meshCells.size()-1);
				}
				else{
					_boundary.indexWallEdge[3].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[3].push_back(_meshCells.size()-1);
				}
			}
			else if(oznew+half==_n){
				_boundary.indexWallEdge[5].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[5].push_back(_meshCells.size()-1);
			}
//					if(oynew+half==_n && oznew+half==_n){
//						_boundary.indexEdgeEdge[3].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[3].push_back(_meshCells.size()-1);
//					}

//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[3][%i %i %i][%i %i %i %i]",
//							_meshCells.size()-1,quarter,nb1,nb2,nb3,ll0,ll1,ll2,ll3);
		}
		edgevector.clear();
	}

//			fprintf(stderr," 4");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[4];
		if(edgevector.size()>1){
//					fprintf(stderr,"\n%li edges:",edgevector.size());
//					for(unsigned int i=0;i<edgevector.size();i++){
//						MeshCell &cell = _meshCells[edgevector[i]];
//						fprintf(stderr,"\n%i [%i %i %i]%i",cell.type,cell.ox,cell.oy,cell.oz,cell.size);
//					}
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] Y[0] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if((oxnew && cell.ox>=oxnew) || (oznew && cell.oz>=oznew))
					fprintf(stderr,"\nERROR: Edge Cell %li at Y[0] offsets X or Z too large: %i >= %i || %i >= %i",
							edgevector[i],cell.ox,oxnew,cell.oz,oznew);
				volumetype cellchildindex = (cell.oy>=oynew+quarter)*2;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[4].push_back(edgevector[i]);
				cell.lastBranch[3] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[4];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge Y[0]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at Y[0] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[oldSize+0].indexCorner[2] = cornervector[i];
					_meshCellIndicesBranch[oldSize+2].indexCorner[0] = cornervector[i];
					cell.lastBranch[5] = oldSize+0; cell.lastBranch[7] = oldSize+2;
				}
				else{
					volumetype shifty = (cell.oy>=oynew+quarter)*2;
					_meshCellIndicesBranch[oldSize+shifty].indexEdgeCorner[4].push_back(cornervector[i]);
					cell.lastBranch[5] = cell.lastBranch[7] = oldSize+shifty;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
			_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[4].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at Y[0] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew; cell.oz = oznew-quarter*(oznew!=0);
			cell.size = quarter; cell.lastBranch[3] = oldSize+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];

			_meshCells.push_back(MeshCell(5,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Y[0] Cell %li to %li",edgevector[0],_meshCells.size()-1);

			_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[4].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = oldSize+2;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].indexWallEdge[5].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].indexEdgeEdge[6].push_back(_meshCells.size()-1);
			}
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].indexWallEdge[1].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].indexEdgeEdge[5].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].indexEdgeEdge[7].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew-quarter*(oxnew!=0),oynew,oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll0,ll1,ll2,ll3,ll2,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[2] = nb0;
			corner.lastBranch[1] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = corner.lastBranch[6] = nb2;
			corner.lastBranch[5] = oldSize+0;
			corner.lastBranch[7] = oldSize+2;
			_meshCellIndicesBranch[oldSize+0].indexCorner[2] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+2].indexCorner[0] = _meshCells.size()-1;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].indexWallCorner[5].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].indexEdgeCorner[6].push_back(_meshCells.size()-1);
			}
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].indexWallCorner[1].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].indexEdgeCorner[5].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].indexEdgeCorner[7].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oxnew==0){
				if(oznew==0){
					_boundary.indexEdgeEdge[4].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[4].push_back(_meshCells.size()-1);
				}
				else{
					_boundary.indexWallEdge[0].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[0].push_back(_meshCells.size()-1);
				}
			}
			else if(oznew==0){
				_boundary.indexWallEdge[4].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[4].push_back(_meshCells.size()-1);
			}
//					if(oxnew==0 && oznew==0){
//						_boundary.indexEdgeEdge[4].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[4].push_back(_meshCells.size()-1);
//					}

//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[4]",_meshCells.size()-1,quarter);
		}
		edgevector.clear();
	}


//			fprintf(stderr," 5");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[5];
		if(edgevector.size()>1){
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] Y[1] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if(cell.ox+cell.size>oxnew+half || (oznew && cell.oz>=oznew))
					fprintf(stderr,"\nERROR: Edge Cell %li at Y[1] offsets X or Z do not fit: %i+%i > %i+%i || %i >= %i",
							edgevector[i],cell.ox,cell.size,oxnew,half,cell.oz,oznew);
				volumetype cellchildindex = (cell.oy>=oynew+quarter)*2 + 1;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[5].push_back(edgevector[i]);
				cell.lastBranch[2] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[5];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge Y[1]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at Y[1] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[oldSize+1].indexCorner[3] = cornervector[i];
					_meshCellIndicesBranch[oldSize+3].indexCorner[1] = cornervector[i];
					cell.lastBranch[4] = oldSize+1; cell.lastBranch[6] = oldSize+3;
				}
				else{
					volumetype shifty = (cell.oy>=oynew+quarter)*2;
					_meshCellIndicesBranch[oldSize+shifty+1].indexEdgeCorner[5].push_back(cornervector[i]);
					cell.lastBranch[4] = cell.lastBranch[6] = oldSize+shifty+1;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
			_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[5].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at Y[1] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew+quarter; cell.oy = oynew; cell.oz = oznew-quarter*(oznew!=0);
			cell.size = quarter; cell.lastBranch[2] = oldSize+1;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb3 = cell.lastBranch[3];
//					fprintf(stderr,"->[%i %i %i %i]",nb0,nb1,cell.lastBranch[2],nb3);

			_meshCells.push_back(MeshCell(5,oxnew+quarter,oynew+quarter,oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Y[1] Cell %li to %li",edgevector[0],_meshCells.size()-1);

			_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[5].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = oldSize+3;
			edge.lastBranch[3] = nb3;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].indexWallEdge[5].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeEdge[7].push_back(_meshCells.size()-1);
			}
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].indexWallEdge[0].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeEdge[4].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].indexEdgeEdge[6].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

//					fprintf(stderr,"c");
			_meshCells.push_back(MeshCell(7,oxnew+quarter,oynew,oznew-quarter*(oznew!=0),quarter,ll0,ll1,ll0,ll1,ll2,ll3,ll2,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[2] = nb0;
			corner.lastBranch[1] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = oldSize+1;
			corner.lastBranch[6] = oldSize+3;
			corner.lastBranch[5] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[oldSize+1].indexCorner[3] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+3].indexCorner[1] = _meshCells.size()-1;
			if(oznew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].indexWallCorner[5].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeCorner[7].push_back(_meshCells.size()-1);
			}
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].indexWallCorner[0].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeCorner[4].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].indexEdgeCorner[6].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oxnew+half==_n){
				if(oznew==0){
					_boundary.indexEdgeEdge[5].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[5].push_back(_meshCells.size()-1);
				}
				else{
					_boundary.indexWallEdge[1].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[1].push_back(_meshCells.size()-1);
				}
			}
			else if(oznew==0){
				_boundary.indexWallEdge[4].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[4].push_back(_meshCells.size()-1);
			}
//					if(oxnew+half==_n && oznew==0){
//						_boundary.indexEdgeEdge[5].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[5].push_back(_meshCells.size()-1);
//					}

//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[5]",_meshCells.size()-1,quarter);
		}
		edgevector.clear();
	}

//			fprintf(stderr," 6");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[6];
		if(edgevector.size()>1){
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] Y[2] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if((oxnew && cell.ox>=oxnew) || cell.oz+cell.size>oznew+half)
					fprintf(stderr,"\nERROR: Edge Cell %li at Y[2] offsets X or Z do not fit: %i >= %i || %i+%i > %i+%i",
							edgevector[i],cell.ox,oxnew,cell.oz,cell.size,oznew,half);
				volumetype cellchildindex = (cell.oy>=oynew+quarter)*2 + 4;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[6].push_back(edgevector[i]);
				cell.lastBranch[1] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[6];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge Y[2]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at Y[2] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[oldSize+4].indexCorner[6] = cornervector[i];
					_meshCellIndicesBranch[oldSize+6].indexCorner[4] = cornervector[i];
					cell.lastBranch[1] = oldSize+4; cell.lastBranch[3] = oldSize+6;
				}
				else{
					volumetype shifty = (cell.oy>=oynew+quarter)*2;
					_meshCellIndicesBranch[oldSize+shifty+4].indexEdgeCorner[6].push_back(cornervector[i]);
					cell.lastBranch[1] = cell.lastBranch[3] = oldSize+shifty+4;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
			_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[6].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at Y[2] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew; cell.oz = oznew+quarter;
			cell.size = quarter; cell.lastBranch[1] = oldSize+4;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(5,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew+quarter,quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Y[2] Cell %li to %li",edgevector[0],_meshCells.size()-1);

			_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[6].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = oldSize+6;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].indexWallEdge[1].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeEdge[7].push_back(_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].indexWallEdge[4].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeEdge[4].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].indexEdgeEdge[5].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew-quarter*(oxnew!=0),oynew,oznew+quarter,quarter,ll0,ll1,ll0,ll1,ll2,ll3,ll2,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[2] = nb0;
			corner.lastBranch[1] = oldSize+4;
			corner.lastBranch[3] = oldSize+6;
			corner.lastBranch[4] = corner.lastBranch[6] = nb2;
			corner.lastBranch[5] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[oldSize+4].indexCorner[6] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+6].indexCorner[4] = _meshCells.size()-1;
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].indexWallCorner[1].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeCorner[7].push_back(_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].indexWallCorner[4].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeCorner[4].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].indexEdgeCorner[5].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oxnew==0){
				if(oznew+half==_n){
					_boundary.indexEdgeEdge[6].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[6].push_back(_meshCells.size()-1);
				}
				else{
					_boundary.indexWallEdge[0].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[0].push_back(_meshCells.size()-1);
				}
			}
			else if(oznew+half==_n){
				_boundary.indexWallEdge[5].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[5].push_back(_meshCells.size()-1);
			}
//					if(oxnew==0 && oznew+half==_n){
//						_boundary.indexEdgeEdge[6].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[6].push_back(_meshCells.size()-1);
//					}

//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[6]",_meshCells.size()-1,quarter);
		}
		edgevector.clear();
	}

//			fprintf(stderr," 7");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[7];
		if(edgevector.size()>1){
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] Y[3] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if(cell.ox+cell.size>oxnew+half || cell.oz+cell.size>oznew+half)
					fprintf(stderr,"\nERROR: Edge Cell %li at Y[3] offsets X or Z do not fit: %i+%i > %i+%i || %i+%i > %i+%i",
							edgevector[i],cell.ox,cell.size,oxnew,half,cell.oz,cell.size,oznew,half);
				volumetype cellchildindex = (cell.oy>=oynew+quarter)*2 + 5;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[7].push_back(edgevector[i]);
				cell.lastBranch[0] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[7];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge Y[3]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at Y[3] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.oy+cell.size==oynew+quarter){
					_meshCellIndicesBranch[oldSize+5].indexCorner[7] = cornervector[i];
					_meshCellIndicesBranch[oldSize+7].indexCorner[5] = cornervector[i];
					cell.lastBranch[0] = oldSize+5; cell.lastBranch[2] = oldSize+7;
				}
				else{
					volumetype shifty = (cell.oy>=oynew+quarter)*2;
					_meshCellIndicesBranch[oldSize+shifty+5].indexEdgeCorner[7].push_back(cornervector[i]);
					cell.lastBranch[0] = cell.lastBranch[2] = oldSize+shifty+5;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
			_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[7].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at Y[3] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew+quarter; cell.oy = oynew; cell.oz = oznew+quarter;
			cell.size = quarter; cell.lastBranch[0] = oldSize+5;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(5,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Y[3] Cell %li to %li",edgevector[0],_meshCells.size()-1);

			_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[7].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = oldSize+7;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb1].indexWallEdge[0].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].indexEdgeEdge[6].push_back(_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].indexWallEdge[4].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].indexEdgeEdge[5].push_back(_meshCells.size()-1);
			}
			if(nb1!=nb3 && nb2!=nb3) _meshCellIndicesBranch[nb3].indexEdgeEdge[4].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew+quarter,oynew,oznew+quarter,quarter,ll0,ll1,ll0,ll1,ll2,ll3,ll2,ll3));
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = oldSize+5;
			corner.lastBranch[2] = oldSize+7;
			corner.lastBranch[1] = corner.lastBranch[3] = nb1;
			corner.lastBranch[4] = corner.lastBranch[6] = nb2;
			corner.lastBranch[5] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[oldSize+5].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+7].indexCorner[5] = _meshCells.size()-1;
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb1].indexWallCorner[0].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].indexEdgeCorner[6].push_back(_meshCells.size()-1);
			}
			if(oznew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].indexWallCorner[4].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].indexEdgeCorner[5].push_back(_meshCells.size()-1);
			}
			if(nb1!=nb3 && nb2!=nb3) _meshCellIndicesBranch[nb3].indexEdgeCorner[4].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oxnew+half==_n){
				if(oznew+half==_n){
					_boundary.indexEdgeEdge[7].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[7].push_back(_meshCells.size()-1);
				}
				else{
					_boundary.indexWallEdge[1].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[1].push_back(_meshCells.size()-1);
				}
			}
			else if(oznew+half==_n){
				_boundary.indexWallEdge[5].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[5].push_back(_meshCells.size()-1);
			}
//					if(oxnew+half==_n && oznew+half==_n){
//						_boundary.indexEdgeEdge[7].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[7].push_back(_meshCells.size()-1);
//					}

//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[7]",_meshCells.size()-1,quarter);
		}
		edgevector.clear();
	}

//			fprintf(stderr," 8");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[8];
		if(edgevector.size()>1){
//					fprintf(stderr,"\n%li edges at [%i %i %i]%i:",edgevector.size(),oxnew,oynew,oznew,half);
//					for(unsigned int i=0;i<edgevector.size();i++){
//						MeshCell &cell = _meshCells[edgevector[i]];
//						fprintf(stderr,"\n%i [%i %i %i]%i : %li",cell.type,cell.ox,cell.oy,cell.oz,cell.size,edgevector[i]);
//					}
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] Z[0] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if((oxnew && cell.ox>=oxnew) || (oynew && cell.oy>=oynew))
					fprintf(stderr,"\nERROR: Edge Cell %li at Z[0] offsets X or Y too large: %i >= %i || %i >= %i",
							edgevector[i],cell.ox,oxnew,cell.oy,oynew);
				volumetype cellchildindex = (cell.oz>=oznew+quarter)*4;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[8].push_back(edgevector[i]);
				cell.lastBranch[3] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[8];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge Z[0]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at Z[0] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[oldSize+0].indexCorner[4] = cornervector[i];
					_meshCellIndicesBranch[oldSize+4].indexCorner[0] = cornervector[i];
					cell.lastBranch[3] = oldSize+0; cell.lastBranch[7] = oldSize+4;
				}
				else{
					volumetype shiftz = (cell.oz>=oznew+quarter)*4;
					_meshCellIndicesBranch[oldSize+shiftz].indexEdgeCorner[8].push_back(cornervector[i]);
					cell.lastBranch[3] = cell.lastBranch[7] = oldSize+shiftz;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
			_meshCellIndicesBranch[oldSize+0].indexEdgeEdge[8].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at Z[0] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew-quarter*(oynew!=0); cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[3] = oldSize+0;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];

			_meshCells.push_back(MeshCell(6,oxnew-quarter*(oxnew!=0),oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Z[0] EdgeEdge Cell %li to %li to Size %i",edgevector[0],_meshCells.size()-1,_meshCells.back().size);
//					fprintf(stderr," Neighbors: %i %i %i",nb0,nb1,nb2);
//					fprintf(stderr," Position [%i %i %i]%i",oxnew,oynew,oznew,half);

			_meshCellIndicesBranch[oldSize+4].indexEdgeEdge[8].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = oldSize+4;
			if(oynew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].indexWallEdge[3].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].indexEdgeEdge[10].push_back(_meshCells.size()-1);
			}
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].indexWallEdge[1].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].indexEdgeEdge[9].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].indexEdgeEdge[11].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew-quarter*(oxnew!=0),oynew-quarter*(oynew!=0),oznew,quarter,ll0,ll1,ll2,ll3,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Z[0] EdgeCorner Cell %li to %li to Size %i",edgevector[0],_meshCells.size()-1,_meshCells.back().size);

			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[4] = nb0;
			corner.lastBranch[1] = corner.lastBranch[5] = nb1;
			corner.lastBranch[2] = corner.lastBranch[6] = nb2;
			corner.lastBranch[3] = oldSize+0;
			corner.lastBranch[7] = oldSize+4;
			_meshCellIndicesBranch[oldSize+0].indexCorner[4] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+4].indexCorner[0] = _meshCells.size()-1;
			if(oynew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb1].indexWallCorner[3].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb1].indexEdgeCorner[10].push_back(_meshCells.size()-1);
			}
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb2].indexWallCorner[1].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb2].indexEdgeCorner[9].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb0!=nb2) _meshCellIndicesBranch[nb0].indexEdgeCorner[11].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oxnew==0){
				if(oynew==0){
					_boundary.indexEdgeEdge[8].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[8].push_back(_meshCells.size()-1);
				}
				else{
					_boundary.indexWallEdge[0].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[0].push_back(_meshCells.size()-1);
				}
			}
			else if(oynew==0){
				_boundary.indexWallEdge[2].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[2].push_back(_meshCells.size()-1);
			}
//					if(oxnew==0 && oynew==0){
//						_boundary.indexEdgeEdge[8].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[8].push_back(_meshCells.size()-1);
//					}

//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[8]",_meshCells.size()-1,quarter);
		}
		edgevector.clear();
	}

//			fprintf(stderr," 9");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[9];
		if(edgevector.size()>1){
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] Z[1] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if(cell.ox+cell.size>oxnew+half || (oynew && cell.oy>=oynew))
					fprintf(stderr,"\nERROR: Edge Cell %li at Z[1] offsets X or Y do not fit: %i+%i > %i+%i || %i >= %i",
							edgevector[i],cell.ox,cell.size,oxnew,half,cell.oy,oynew);
				volumetype cellchildindex = (cell.oz>=oznew+quarter)*4 + 1;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[9].push_back(edgevector[i]);
				cell.lastBranch[2] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[9];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge Z[1]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at Z[1] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[oldSize+1].indexCorner[5] = cornervector[i];
					_meshCellIndicesBranch[oldSize+5].indexCorner[1] = cornervector[i];
					cell.lastBranch[2] = oldSize+1; cell.lastBranch[6] = oldSize+5;
				}
				else{
					volumetype shiftz = (cell.oz>=oznew+quarter)*4;
					_meshCellIndicesBranch[oldSize+shiftz+1].indexEdgeCorner[9].push_back(cornervector[i]);
					cell.lastBranch[2] = cell.lastBranch[6] = oldSize+shiftz+1;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
//					fprintf(stderr,"T");
			_meshCellIndicesBranch[oldSize+1].indexEdgeEdge[9].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at Z[1] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew+quarter; cell.oy = oynew-quarter*(oynew!=0); cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[2] = oldSize+1;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(6,oxnew+quarter,oynew-quarter*(oynew!=0),oznew+quarter,quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Z[1] EdgeEdge Cell %li to %li to Size %i",edgevector[0],_meshCells.size()-1,_meshCells.back().size);

			_meshCellIndicesBranch[oldSize+5].indexEdgeEdge[9].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = oldSize+5;
			edge.lastBranch[3] = nb3;
			if(oynew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].indexWallEdge[3].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeEdge[11].push_back(_meshCells.size()-1);
			}
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].indexWallEdge[0].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeEdge[8].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].indexEdgeEdge[10].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

//					fprintf(stderr,"c");
			_meshCells.push_back(MeshCell(7,oxnew+quarter,oynew-quarter*(oynew!=0),oznew,quarter,ll0,ll1,ll2,ll3,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Z[1] EdgeCorner Cell %li to %li to Size %i",edgevector[0],_meshCells.size()-1,_meshCells.back().size);

			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[4] = nb0;
			corner.lastBranch[1] = corner.lastBranch[5] = nb1;
			corner.lastBranch[2] = oldSize+1;
			corner.lastBranch[6] = oldSize+5;
			corner.lastBranch[3] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[oldSize+1].indexCorner[5] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+5].indexCorner[1] = _meshCells.size()-1;
			if(oynew!=0){
				if(nb0==nb1) _meshCellIndicesBranch[nb0].indexWallCorner[3].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeCorner[11].push_back(_meshCells.size()-1);
			}
			if(oxnew+half!=_n){
				if(nb1==nb3) _meshCellIndicesBranch[nb3].indexWallCorner[0].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeCorner[8].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb1 && nb1!=nb3) _meshCellIndicesBranch[nb1].indexEdgeCorner[10].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

//					fprintf(stderr,"b");
			if(oxnew+half==_n){
//						fprintf(stderr,"\nCells are part of the Boundary");
				if(oynew==0){
//							fprintf(stderr," Edge Z[1]");
					_boundary.indexEdgeEdge[9].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[9].push_back(_meshCells.size()-1);
				}
				else{
//							fprintf(stderr," Wall X[1]");
					_boundary.indexWallEdge[1].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[1].push_back(_meshCells.size()-1);
				}
			}
			else if(oynew==0){
//						fprintf(stderr," Wall Y[0]");
				_boundary.indexWallEdge[2].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[2].push_back(_meshCells.size()-1);
			}
//					if(oxnew+half==_n && oynew==0){
//						_boundary.indexEdgeEdge[9].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[9].push_back(_meshCells.size()-1);
//					}

//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[9]",_meshCells.size()-1,quarter);
		}
		edgevector.clear();
	}

//			fprintf(stderr," 10");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[10];
		if(edgevector.size()>1){
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] Z[2] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if((oxnew && cell.ox>=oxnew) || cell.oy+cell.size>oynew+half)
					fprintf(stderr,"\nERROR: Edge Cell %li at Z[2] offsets X or Y do not fit: %i >= %i || %i+%i > %i+%i",
							edgevector[i],cell.ox,oxnew,cell.oy,cell.size,oynew,half);
				volumetype cellchildindex = (cell.oz>=oznew+quarter)*4 + 2;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[10].push_back(edgevector[i]);
				cell.lastBranch[1] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[10];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge Z[2]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at Z[2] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[oldSize+2].indexCorner[6] = cornervector[i];
					_meshCellIndicesBranch[oldSize+6].indexCorner[2] = cornervector[i];
					cell.lastBranch[1] = oldSize+2; cell.lastBranch[5] = oldSize+6;
				}
				else{
					volumetype shiftz = (cell.oz>=oznew+quarter)*4;
					_meshCellIndicesBranch[oldSize+shiftz+2].indexEdgeCorner[10].push_back(cornervector[i]);
					cell.lastBranch[1] = cell.lastBranch[5] = oldSize+shiftz+2;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
			_meshCellIndicesBranch[oldSize+2].indexEdgeEdge[10].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at Z[2] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew+quarter; cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[1] = oldSize+2;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb0 = cell.lastBranch[0];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(6,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew+quarter,quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Z[2] EdgeEdge Cell %li to %li to Size %i",edgevector[0],_meshCells.size()-1,_meshCells.back().size);

			_meshCellIndicesBranch[oldSize+6].indexEdgeEdge[10].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = nb0;
			edge.lastBranch[1] = oldSize+6;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].indexWallEdge[1].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeEdge[11].push_back(_meshCells.size()-1);
			}
			if(oynew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].indexWallEdge[2].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeEdge[8].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].indexEdgeEdge[9].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew-quarter*(oxnew!=0),oynew+quarter,oznew,quarter,ll0,ll1,ll2,ll3,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Z[2] EdgeCorner Cell %li to %li to Size %i",edgevector[0],_meshCells.size()-1,_meshCells.back().size);
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = corner.lastBranch[4] = nb0;
			corner.lastBranch[1] = oldSize+2;
			corner.lastBranch[5] = oldSize+6;
			corner.lastBranch[2] = corner.lastBranch[6] = nb2;
			corner.lastBranch[3] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[oldSize+2].indexCorner[6] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+6].indexCorner[2] = _meshCells.size()-1;
			if(oxnew!=0){
				if(nb0==nb2) _meshCellIndicesBranch[nb0].indexWallCorner[1].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb0].indexEdgeCorner[11].push_back(_meshCells.size()-1);
			}
			if(oynew+half!=_n){
				if(nb2==nb3) _meshCellIndicesBranch[nb3].indexWallCorner[2].push_back(_meshCells.size()-1);
				else         _meshCellIndicesBranch[nb3].indexEdgeCorner[8].push_back(_meshCells.size()-1);
			}
			if(nb0!=nb2 && nb2!=nb3) _meshCellIndicesBranch[nb2].indexEdgeCorner[9].push_back(_meshCells.size()-1);
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oxnew==0){
				if(oynew+half==_n){
					_boundary.indexEdgeEdge[10].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[10].push_back(_meshCells.size()-1);
				}
				else{
					_boundary.indexWallEdge[0].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[0].push_back(_meshCells.size()-1);
				}
			}
			else if(oynew+half==_n){
				_boundary.indexWallEdge[3].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[3].push_back(_meshCells.size()-1);
			}
//					if(oxnew==0 && oynew+half==_n){
//						_boundary.indexEdgeEdge[10].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[10].push_back(_meshCells.size()-1);
//					}
//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[10]",_meshCells.size()-1,quarter);
		}
		edgevector.clear();
	}

//			fprintf(stderr," 11");
	{
		std::vector<size_t> &edgevector = _meshCellIndicesBranch[branch+childoffset].indexEdgeEdge[11];
		if(edgevector.size()>1){
//					fprintf(stderr,"\n%li edges at [%i %i %i]%i:",edgevector.size(),oxnew,oynew,oznew,half);
//					for(unsigned int i=0;i<edgevector.size();i++){
//						MeshCell &cell = _meshCells[edgevector[i]];
//						fprintf(stderr,"\n%i [%i %i %i]%i : %li",cell.type,cell.ox,cell.oy,cell.oz,cell.size,edgevector[i]);
//					}
			for(unsigned int i=0;i<edgevector.size();i++){
				MeshCell &cell = _meshCells[edgevector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeEdge Cell %li %i[%i %i %i] Z[3] at [%i %i %i] too large: %i -> %i",
						edgevector[i],cell.type,cell.ox,cell.oy,cell.oz,oxnew,oynew,oznew,cell.size,quarter);}
				if(cell.ox+cell.size>oxnew+half || cell.oy+cell.size>oynew+half)
					fprintf(stderr,"\nERROR: Edge Cell %li at Z[3] offsets X or Y do not fit: %i+%i > %i+%i || %i+%i > %i+%i",
							edgevector[i],cell.ox,cell.size,oxnew,half,cell.oy,cell.size,oynew,half);
				volumetype cellchildindex = (cell.oz>=oznew+quarter)*4 + 3;
				_meshCellIndicesBranch[oldSize+cellchildindex].indexEdgeEdge[11].push_back(edgevector[i]);
				cell.lastBranch[0] = oldSize+cellchildindex;
			}
			std::vector<size_t> &cornervector = _meshCellIndicesBranch[branch+childoffset].indexEdgeCorner[11];
			if(cornervector.size()==0) fprintf(stderr,"\nERROR:Recursive Edges but no Corners at Edge Z[3]");
			for(unsigned int i=0;i<cornervector.size();i++){
				MeshCell &cell = _meshCells[cornervector[i]];
				if(cell.size>quarter){fprintf(stderr,"\nERROR: EdgeCorner Cell %li at Z[3] is too large: %i > %i",cornervector[i],cell.size,quarter);}
				if(cell.oz+cell.size==oznew+quarter){
					_meshCellIndicesBranch[oldSize+3].indexCorner[7] = cornervector[i];
					_meshCellIndicesBranch[oldSize+7].indexCorner[3] = cornervector[i];
					cell.lastBranch[0] = oldSize+3; cell.lastBranch[4] = oldSize+7;
				}
				else{
					volumetype shiftz = (cell.oz>=oznew+quarter)*4;
					_meshCellIndicesBranch[oldSize+shiftz+3].indexEdgeCorner[11].push_back(cornervector[i]);
					cell.lastBranch[0] = cell.lastBranch[4] = oldSize+shiftz+3;
				}
			}
			cornervector.clear();
		}
		else if(edgevector.size()==1){
			_meshCellIndicesBranch[oldSize+3].indexEdgeEdge[11].push_back(edgevector[0]);
			MeshCell &cell = _meshCells[edgevector[0]];
			if(cell.size<=quarter){
				fprintf(stderr,"\nERROR: EdgeEdge Cell %li at Z[3] %i too small: %i<=%i",
						edgevector[0],branch+childoffset,cell.size,quarter);
			}
			cell.ox = oxnew+quarter; cell.oy = oynew+quarter; cell.oz = oznew;
			cell.size = quarter; cell.lastBranch[0] = oldSize+3;
			volumetype ll0 = cell.lastLeaf[0]; volumetype ll1 = cell.lastLeaf[1];
			volumetype ll2 = cell.lastLeaf[2]; volumetype ll3 = cell.lastLeaf[3];
			volumetype nb1 = cell.lastBranch[1];
			volumetype nb2 = cell.lastBranch[2];
			volumetype nb3 = cell.lastBranch[3];

			_meshCells.push_back(MeshCell(6,oxnew+quarter,oynew+quarter,oznew+quarter,quarter,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Z[3] EdgeEdge Cell %li to %li to Size %i",edgevector[0],_meshCells.size()-1,_meshCells.back().size);

			_meshCellIndicesBranch[oldSize+7].indexEdgeEdge[11].push_back(_meshCells.size()-1);
			MeshCell &edge = _meshCells.back();
			edge.lastBranch[0] = oldSize+7;
			edge.lastBranch[1] = nb1;
			edge.lastBranch[2] = nb2;
			edge.lastBranch[3] = nb3;
			if(oxnew+half!=_n){
				if(nb1==nb3) { _meshCellIndicesBranch[nb1].indexWallEdge[0].push_back(_meshCells.size()-1);}
				else         {_meshCellIndicesBranch[nb1].indexEdgeEdge[10].push_back(_meshCells.size()-1);}
			}
			if(oynew+half!=_n){
				if(nb2==nb3) {_meshCellIndicesBranch[nb2].indexWallEdge[2].push_back(_meshCells.size()-1);}
				else         {_meshCellIndicesBranch[nb2].indexEdgeEdge[9].push_back(_meshCells.size()-1);}
			}
			if(nb1!=nb3 && nb2!=nb3) {_meshCellIndicesBranch[nb3].indexEdgeEdge[8].push_back(_meshCells.size()-1);}
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			_meshCells.push_back(MeshCell(7,oxnew+quarter,oynew+quarter,oznew,quarter,ll0,ll1,ll2,ll3,ll0,ll1,ll2,ll3));
//					fprintf(stderr,"\nSplit off Z[3] EdgeCorner Cell %li to %li to Size %i",edgevector[0],_meshCells.size()-1,_meshCells.back().size);
			MeshCell &corner = _meshCells.back();
			corner.lastBranch[0] = oldSize+3;
			corner.lastBranch[4] = oldSize+7;
			corner.lastBranch[1] = corner.lastBranch[5] = nb1;
			corner.lastBranch[2] = corner.lastBranch[6] = nb2;
			corner.lastBranch[3] = corner.lastBranch[7] = nb3;
			_meshCellIndicesBranch[oldSize+3].indexCorner[7] = _meshCells.size()-1;
			_meshCellIndicesBranch[oldSize+7].indexCorner[3] = _meshCells.size()-1;
			if(oxnew+half!=_n){
				if(nb1==nb3) {_meshCellIndicesBranch[nb1].indexWallCorner[0].push_back(_meshCells.size()-1);}
				else         {_meshCellIndicesBranch[nb1].indexEdgeCorner[10].push_back(_meshCells.size()-1);}
			}
			if(oynew+half!=_n){
				if(nb2==nb3) {_meshCellIndicesBranch[nb2].indexWallCorner[2].push_back(_meshCells.size()-1);}
				else         {_meshCellIndicesBranch[nb2].indexEdgeCorner[9].push_back(_meshCells.size()-1);}
			}
			if(nb1!=nb3 && nb2!=nb3) {_meshCellIndicesBranch[nb3].indexEdgeCorner[8].push_back(_meshCells.size()-1);}
			if(pl<BRANCHINIT){
				_meshCellIndicesLeaf[pl].push_back(_meshCells.size()-1);
			}

			if(oxnew+half==_n){
				if(oynew+half==_n){
					_boundary.indexEdgeEdge[11].push_back(_meshCells.size()-2);
					_boundary.indexEdgeCorner[11].push_back(_meshCells.size()-1);
				}
				else{
					_boundary.indexWallEdge[1].push_back(_meshCells.size()-2);
					_boundary.indexWallCorner[1].push_back(_meshCells.size()-1);
				}
			}
			else if(oynew+half==_n){
				_boundary.indexWallEdge[3].push_back(_meshCells.size()-2);
				_boundary.indexWallCorner[3].push_back(_meshCells.size()-1);
			}
//					if(oxnew+half==_n && oynew+half==_n){
//						_boundary.indexEdgeEdge[11].push_back(_meshCells.size()-2);
//						_boundary.indexEdgeCorner[11].push_back(_meshCells.size()-1);
//					}

//					fprintf(stderr,"\nCreated Corner Cell %li with Size %i at Edge[11]",_meshCells.size()-1,quarter);
		}
		edgevector.clear();
	}
//			fprintf(stderr,"!");


//			fprintf(stderr,"C");
	//Corner
	_meshCellIndicesBranch[oldSize+0].indexCorner[0] = _meshCellIndicesBranch[branch+childoffset].indexCorner[0];
	_meshCellIndicesBranch[oldSize+1].indexCorner[1] = _meshCellIndicesBranch[branch+childoffset].indexCorner[1];
	_meshCellIndicesBranch[oldSize+2].indexCorner[2] = _meshCellIndicesBranch[branch+childoffset].indexCorner[2];
	_meshCellIndicesBranch[oldSize+3].indexCorner[3] = _meshCellIndicesBranch[branch+childoffset].indexCorner[3];
	_meshCellIndicesBranch[oldSize+4].indexCorner[4] = _meshCellIndicesBranch[branch+childoffset].indexCorner[4];
	_meshCellIndicesBranch[oldSize+5].indexCorner[5] = _meshCellIndicesBranch[branch+childoffset].indexCorner[5];
	_meshCellIndicesBranch[oldSize+6].indexCorner[6] = _meshCellIndicesBranch[branch+childoffset].indexCorner[6];
	_meshCellIndicesBranch[oldSize+7].indexCorner[7] = _meshCellIndicesBranch[branch+childoffset].indexCorner[7];


	_meshCellIndicesBranch[branch+childoffset].indexCorner[0] = MAXCELLINDEX;
	_meshCellIndicesBranch[branch+childoffset].indexCorner[1] = MAXCELLINDEX;
	_meshCellIndicesBranch[branch+childoffset].indexCorner[2] = MAXCELLINDEX;
	_meshCellIndicesBranch[branch+childoffset].indexCorner[3] = MAXCELLINDEX;
	_meshCellIndicesBranch[branch+childoffset].indexCorner[4] = MAXCELLINDEX;
	_meshCellIndicesBranch[branch+childoffset].indexCorner[5] = MAXCELLINDEX;
	_meshCellIndicesBranch[branch+childoffset].indexCorner[6] = MAXCELLINDEX;
	_meshCellIndicesBranch[branch+childoffset].indexCorner[7] = MAXCELLINDEX;

	if(_meshCellIndicesBranch[oldSize+0].indexCorner[0]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[oldSize+0].indexCorner[0]];
//				fprintf(stderr,"\nPassing Corner Cell[0] %li",_meshCellIndicesBranch[oldSize+0].indexCorner[0]);
		cell.lastBranch[7] = oldSize+0;
		if(cell.size>quarter){
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew-quarter*(oynew!=0);
			cell.oz = oznew-quarter*(oznew!=0); cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[oldSize+1].indexCorner[1]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[oldSize+1].indexCorner[1]];
//				fprintf(stderr,"\nPassing Corner Cell[1] %li",_meshCellIndicesBranch[oldSize+1].indexCorner[1]);
		cell.lastBranch[6] = oldSize+1;
		if(cell.size>quarter){
			cell.ox = oxnew+quarter; cell.oy = oynew-quarter*(oynew!=0);
			cell.oz = oznew-quarter*(oznew!=0); cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[oldSize+2].indexCorner[2]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[oldSize+2].indexCorner[2]];
//				fprintf(stderr,"\nPassing Corner Cell[2] %li",_meshCellIndicesBranch[oldSize+2].indexCorner[2]);
		cell.lastBranch[5] = oldSize+2;
		if(cell.size>quarter){
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew+quarter;
			cell.oz = oznew-quarter*(oznew!=0); cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[oldSize+3].indexCorner[3]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[oldSize+3].indexCorner[3]];
//				fprintf(stderr,"\nPassing Corner Cell[3] %li",_meshCellIndicesBranch[oldSize+3].indexCorner[3]);
		cell.lastBranch[4] = oldSize+3;
		if(cell.size>quarter){
			cell.ox = oxnew+quarter; cell.oy = oynew+quarter;
			cell.oz = oznew-quarter*(oznew!=0); cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[oldSize+4].indexCorner[4]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[oldSize+4].indexCorner[4]];
//				fprintf(stderr,"\nPassing Corner Cell[4] %li",_meshCellIndicesBranch[oldSize+4].indexCorner[4]);
		cell.lastBranch[3] = oldSize+4;
		if(cell.size>quarter){
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew-quarter*(oynew!=0);
			cell.oz = oznew+quarter; cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[oldSize+5].indexCorner[5]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[oldSize+5].indexCorner[5]];
//				fprintf(stderr,"\nPassing Corner Cell[5] %li",_meshCellIndicesBranch[oldSize+5].indexCorner[5]);
		cell.lastBranch[2] = oldSize+5;
		if(cell.size>quarter){
			cell.ox = oxnew+quarter; cell.oy = oynew-quarter*(oynew!=0);
			cell.oz = oznew+quarter; cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[oldSize+6].indexCorner[6]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[oldSize+6].indexCorner[6]];
//				fprintf(stderr,"\nPassing Corner Cell[6] %li",_meshCellIndicesBranch[oldSize+6].indexCorner[6]);
		cell.lastBranch[1] = oldSize+6;
		if(cell.size>quarter){
			cell.ox = oxnew-quarter*(oxnew!=0); cell.oy = oynew+quarter;
			cell.oz = oznew+quarter; cell.size = quarter;
		}
	}
	if(_meshCellIndicesBranch[oldSize+7].indexCorner[7]<MAXCELLINDEX){
		MeshCell &cell = _meshCells[_meshCellIndicesBranch[oldSize+7].indexCorner[7]];
//				fprintf(stderr,"\nPassing Corner Cell[7] %li",_meshCellIndicesBranch[oldSize+7].indexCorner[7]);
		cell.lastBranch[0] = oldSize+7;
		if(cell.size>quarter){
			cell.ox = oxnew+quarter; cell.oy = oynew+quarter;
			cell.oz = oznew+quarter; cell.size = quarter;
		}
	}
//			fprintf(stderr,"!");
#endif




#ifdef LEAFASSOCIATION


#endif







