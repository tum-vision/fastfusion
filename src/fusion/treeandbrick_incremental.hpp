/*
 * treeandbrick.hpp
 *
 *  Created on: Mar 1, 2013
 *      Author: steinbrf
 */

#ifndef TREEANDBRICK_INCREMENTAL_HPP_
#define TREEANDBRICK_INCREMENTAL_HPP_


class BranchNeighborhood;
class MeshCell;
class MCNSplit;

#include "mesh.hpp"
#include "mesh_interleaved.hpp"
#include <fusion/definitions.h>
#include "treeandbrick.hpp"
#include <vector>
#include <list>
//#include <forward_list>


#include <auxiliary/multivector.h>

//#define BRICKVISUALIZATION


//class MeshCell
//{
//public:
//	int type;
//	Mesh mesh;
//	sidetype ox; sidetype oy; sidetype oz; sidetype size;
//	leafstack leaves0; leafstack leaves1; leafstack leaves2; leafstack leaves3;
//	leafstack leaves4; leafstack leaves5; leafstack leaves6; leafstack leaves7;
//	MeshCell(int type_p, sidetype ox_p, sidetype oy_p, sidetype oz_p, sidetype size_p,
//			leafstack leaves0_p,               leafstack leaves1_p = leafstack(),
//			leafstack leaves2_p = leafstack(), leafstack leaves3_p = leafstack(),
//			leafstack leaves4_p = leafstack(), leafstack leaves5_p = leafstack(),
//			leafstack leaves6_p = leafstack(), leafstack leaves7_p = leafstack());
//	void updateMesh(treeinfo info);
//};

class OwnParentArray_{
public:
	OwnParentArray_();
	~OwnParentArray_();
	volumetype &operator[](size_t pos);
	const volumetype &operator[](size_t pos) const;
	size_t size() const;
	size_t capacity() const;
	void resize(size_t count, volumetype value = BRANCHINIT);
	void reserve(size_t count);
	void clear();
	void push_back(const volumetype &value);
	volumetype &back();
	OwnParentArray_ &operator=(const OwnParentArray_ &value);
protected:
	volumetype *_array;
	size_t _size;
	size_t _capacity;
	volumetype _dummy;
};


typedef OwnParentArray_ ParentArray;
//	typedef std::vector<volumetype> ParentArray;

class MeshCell
{
public:
	int type;
//	MeshSeparate *mesh;
	MeshInterleaved *meshinterleaved;
	sidetype ox; sidetype oy; sidetype oz; sidetype size;
//	volumetype lastleaf0; volumetype lastleaf1; volumetype lastleaf2; volumetype lastleaf3;
//	volumetype lastleaf4; volumetype lastleaf5; volumetype lastleaf6; volumetype lastleaf7;
//	volumetype lastBranch0; volumetype lastBranch1; volumetype lastBranch2; volumetype lastBranch3;
//	volumetype lastBranch4; volumetype lastBranch5; volumetype lastBranch6; volumetype lastBranch7;
	volumetype lastLeaf[8]; volumetype lastBranch[8];
	MeshCell();
	MeshCell(int type_p, sidetype ox_p, sidetype oy_p, sidetype oz_p, sidetype size_p,
			volumetype lastleaf0_p,              volumetype lastleaf1_p = BRANCHINIT,
			volumetype lastleaf2_p = BRANCHINIT, volumetype lastleaf3_p = BRANCHINIT,
			volumetype lastleaf4_p = BRANCHINIT, volumetype lastleaf5_p = BRANCHINIT,
			volumetype lastleaf6_p = BRANCHINIT, volumetype lastleaf7_p = BRANCHINIT);
	void updateMesh(treeinfo info, const ParentArray &leafParent,
			const MarchingCubesIndexed &mc, bool verbose = false);
	bool operator==(const MeshCell &cell) const;
	bool operator!=(const MeshCell &cell) const;
};




#define VECTYPE_WALLWALL 0
#define VECTYPE_WALLEDGE 1
#define VECTYPE_WALLCORN 2
#define VECTYPE_EDGEEDGE 3
#define VECTYPE_EDGECORN 4

#define VECTYPE_WALLWALL_0 (0*16+0)
#define VECTYPE_WALLWALL_1 (0*16+1)
#define VECTYPE_WALLWALL_2 (0*16+2)
#define VECTYPE_WALLWALL_3 (0*16+3)
#define VECTYPE_WALLWALL_4 (0*16+4)
#define VECTYPE_WALLWALL_5 (0*16+5)

#define VECTYPE_WALLEDGE_0 (1*16+0)
#define VECTYPE_WALLEDGE_1 (1*16+1)
#define VECTYPE_WALLEDGE_2 (1*16+2)
#define VECTYPE_WALLEDGE_3 (1*16+3)
#define VECTYPE_WALLEDGE_4 (1*16+4)
#define VECTYPE_WALLEDGE_5 (1*16+5)

#define VECTYPE_WALLCORN_0 (2*16+0)
#define VECTYPE_WALLCORN_1 (2*16+1)
#define VECTYPE_WALLCORN_2 (2*16+2)
#define VECTYPE_WALLCORN_3 (2*16+3)
#define VECTYPE_WALLCORN_4 (2*16+4)
#define VECTYPE_WALLCORN_5 (2*16+5)

#define VECTYPE_EDGEEDGE_00 (3*16+0)
#define VECTYPE_EDGEEDGE_01 (3*16+1)
#define VECTYPE_EDGEEDGE_02 (3*16+2)
#define VECTYPE_EDGEEDGE_03 (3*16+3)
#define VECTYPE_EDGEEDGE_04 (3*16+4)
#define VECTYPE_EDGEEDGE_05 (3*16+5)
#define VECTYPE_EDGEEDGE_06 (3*16+6)
#define VECTYPE_EDGEEDGE_07 (3*16+7)
#define VECTYPE_EDGEEDGE_08 (3*16+8)
#define VECTYPE_EDGEEDGE_09 (3*16+9)
#define VECTYPE_EDGEEDGE_10 (3*16+10)
#define VECTYPE_EDGEEDGE_11 (3*16+11)

#define VECTYPE_EDGECORN_00 (4*16+0)
#define VECTYPE_EDGECORN_01 (4*16+1)
#define VECTYPE_EDGECORN_02 (4*16+2)
#define VECTYPE_EDGECORN_03 (4*16+3)
#define VECTYPE_EDGECORN_04 (4*16+4)
#define VECTYPE_EDGECORN_05 (4*16+5)
#define VECTYPE_EDGECORN_06 (4*16+6)
#define VECTYPE_EDGECORN_07 (4*16+7)
#define VECTYPE_EDGECORN_08 (4*16+8)
#define VECTYPE_EDGECORN_09 (4*16+9)
#define VECTYPE_EDGECORN_10 (4*16+10)
#define VECTYPE_EDGECORN_11 (4*16+11)


class MeshCellNeighborhood{
public:
	size_t indexInterior;
	size_t indexCorner[8];
};


class MCNCompact
: public MeshCellNeighborhood
{
public:
	typedef uchar vecnum;
	typedef struct vecpair_{uchar vectype; uchar vecindex;} vecpair;
	typedef stb::doublevector_soa<size_t,vecnum> cellvector;
	typedef std::pair<vecnum,size_t> CellReferenceNum;
	typedef std::pair<vecpair,size_t> CellReferencePair;

	static inline vecnum  encnum(uchar vectype, uchar vecindex){return (vectype<<4)|vecindex;}
	static inline vecnum  encnum(vecpair pair){return (pair.vectype<<4)|pair.vecindex;}
	static inline vecpair decnum(vecnum num){vecpair res;res.vectype=num>>4;res.vecindex=num&15;return res;}


	MCNCompact();
	MCNCompact(const MCNSplit &split);

	inline void add(uchar vectype, uchar vecindex,size_t cellindex){
		cells.push_back(cellindex,encnum(vectype,vecindex));
	}
	inline size_t border_size() const {return cells.size();}
	inline CellReferenceNum get(size_t i) const {return CellReferenceNum(cells[i].second,cells[i].first);}
	inline void clear(){cells.clear();}

	inline void addWall(const MCNCompact &other, uchar wallIndex){
  	for(size_t i=0;i<other.cells.size();i++){
  		MCNCompact::vecpair identifier = MCNCompact::decnum(other.cells[i].second);
  		if(identifier.vecindex == wallIndex &&
  				(identifier.vectype==VECTYPE_WALLWALL ||
  			   identifier.vectype==VECTYPE_WALLEDGE ||
  				 identifier.vectype==VECTYPE_WALLCORN)){
  			add(identifier.vectype,identifier.vecindex,other.cells[i].first);
  		}
  	}
	}

	inline void overwriteWall(const MCNCompact &other, uchar sourceIndex){
		cellvector temp;
  	for(size_t i=0;i<cells.size();i++){
  		MCNCompact::vecpair identifier = MCNCompact::decnum(cells[i].second);
  		if(!(identifier.vecindex == sourceIndex &&
  				(identifier.vectype==VECTYPE_WALLWALL ||
  			   identifier.vectype==VECTYPE_WALLEDGE ||
  				 identifier.vectype==VECTYPE_WALLCORN))){
  			temp.push_back(cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
  		}
  	}
  	cells = temp;
  	for(size_t i=0;i<other.cells.size();i++){
  		MCNCompact::vecpair identifier = MCNCompact::decnum(other.cells[i].second);
  		if(identifier.vecindex == sourceIndex &&
  				(identifier.vectype==VECTYPE_WALLWALL ||
  			   identifier.vectype==VECTYPE_WALLEDGE ||
  				 identifier.vectype==VECTYPE_WALLCORN)){
  			add(identifier.vectype,identifier.vecindex,other.cells[i].first);
  		}
  	}
	}

	inline void overwriteEdge(const MCNCompact &other, uchar sourceIndex){
		cellvector temp;
  	for(size_t i=0;i<cells.size();i++){
  		MCNCompact::vecpair identifier = MCNCompact::decnum(cells[i].second);
  		if(!(identifier.vecindex == sourceIndex &&
  				(identifier.vectype==VECTYPE_EDGEEDGE ||
  	  		 identifier.vectype==VECTYPE_EDGECORN))){
  			temp.push_back(cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
  		}
  	}
  	cells = temp;
  	for(size_t i=0;i<other.cells.size();i++){
  		MCNCompact::vecpair identifier = MCNCompact::decnum(other.cells[i].second);
  		if(identifier.vecindex == sourceIndex &&
  				(identifier.vectype==VECTYPE_EDGEEDGE ||
  	  		 identifier.vectype==VECTYPE_EDGECORN)){
  			add(identifier.vectype,identifier.vecindex,other.cells[i].first);
  		}
  	}
	}

	inline void overwriteTwoEdges(const MCNCompact &other, uchar edgeIndex1, uchar edgeIndex2){
		cellvector temp;
  	for(size_t i=0;i<cells.size();i++){
  		MCNCompact::vecpair identifier = MCNCompact::decnum(cells[i].second);
  		if(!((identifier.vecindex == edgeIndex1 || identifier.vecindex == edgeIndex2) &&
  				(identifier.vectype==VECTYPE_EDGEEDGE ||
  	  		 identifier.vectype==VECTYPE_EDGECORN))){
  			temp.push_back(cells[i].first,MCNCompact::encnum(identifier.vectype,identifier.vecindex));
  		}
  	}
  	cells = temp;
  	for(size_t i=0;i<other.cells.size();i++){
  		MCNCompact::vecpair identifier = MCNCompact::decnum(other.cells[i].second);
  		if((identifier.vecindex == edgeIndex1 || identifier.vecindex == edgeIndex2) &&
  				(identifier.vectype==VECTYPE_EDGEEDGE ||
  	  		 identifier.vectype==VECTYPE_EDGECORN)){
  			add(identifier.vectype,identifier.vecindex,other.cells[i].first);
  		}
  	}
	}
	inline void addThreeEdges(const MCNCompact &other,
			uchar edgeIndex1, uchar edgeIndex2, uchar edgeIndex3){
  	for(size_t i=0;i<other.cells.size();i++){
  		MCNCompact::vecpair identifier = MCNCompact::decnum(other.cells[i].second);
  		if((identifier.vecindex == edgeIndex1 ||
  				identifier.vecindex == edgeIndex2 ||
  				identifier.vecindex == edgeIndex3) &&
  				(identifier.vectype==VECTYPE_EDGEEDGE ||
  	  		 identifier.vectype==VECTYPE_EDGECORN)){
  			add(identifier.vectype,identifier.vecindex,other.cells[i].first);
  		}
  	}
	}

	inline void sort(){
		cells.sort(1);
	}

//	size_t indexInterior;
//	size_t indexCorner[8];
private:
	cellvector cells;
private:
};


class MCNSplit
: public MeshCellNeighborhood
{
public:

//#define CAPACITY_MEMORY_ASSESSMENT
	typedef std::vector<size_t> CellList;

//	typedef std::list<size_t> CellList;
	MCNSplit();
	MCNSplit(const MCNCompact &compact);

	inline bool operator==(const MCNSplit &other) const {
		bool result = true;

		MCNSplit other_sorted = other;
		MCNSplit this_sorted = *this;

		for(uchar vecindex=0;vecindex<6;vecindex++){

			if(other_sorted.indexWallWall[vecindex].size() !=
				 this_sorted. indexWallWall[vecindex].size()){
				fprintf(stderr,"\nUnequal Size of indexWallWall[%i]",vecindex);
			}
			std::sort(other_sorted.indexWallWall[vecindex].begin(),
					      other_sorted.indexWallWall[vecindex].end());
			std::sort(this_sorted.indexWallWall[vecindex].begin(),
					      this_sorted.indexWallWall[vecindex].end());
			int unequal_indices = 0;
			for(size_t i=0;i<std::min(other_sorted.indexWallWall[vecindex].size(),this_sorted.indexWallWall[vecindex].size());i++){
				unequal_indices += !(other_sorted.indexWallWall[vecindex][i] == this_sorted.indexWallWall[vecindex][i]);
			}
			result &= unequal_indices == 0;
			if(unequal_indices) fprintf(stderr,"\n%i Indices differ in indexWallWall[%i]",unequal_indices,vecindex);

			if(other_sorted.indexWallEdge[vecindex].size() !=
				 this_sorted. indexWallEdge[vecindex].size()){
				fprintf(stderr,"\nUnequal Size of indexWallEdge[%i]",vecindex);
			}
			std::sort(other_sorted.indexWallEdge[vecindex].begin(),
					      other_sorted.indexWallEdge[vecindex].end());
			std::sort(this_sorted.indexWallEdge[vecindex].begin(),
					      this_sorted.indexWallEdge[vecindex].end());
			unequal_indices = 0;
			for(size_t i=0;i<std::min(other_sorted.indexWallEdge[vecindex].size(),this_sorted.indexWallEdge[vecindex].size());i++){
				unequal_indices += !(other_sorted.indexWallEdge[vecindex][i] == this_sorted.indexWallEdge[vecindex][i]);
			}
			result &= unequal_indices == 0;
			if(unequal_indices) fprintf(stderr,"\n%i Indices differ in indexWallEdge[%i]",unequal_indices,vecindex);

			if(other_sorted.indexWallCorner[vecindex].size() !=
				 this_sorted. indexWallCorner[vecindex].size()){
				fprintf(stderr,"\nUnequal Size of indexWallCorner[%i]",vecindex);
			}
			std::sort(other_sorted.indexWallCorner[vecindex].begin(),
					      other_sorted.indexWallCorner[vecindex].end());
			std::sort(this_sorted.indexWallCorner[vecindex].begin(),
					      this_sorted.indexWallCorner[vecindex].end());
			unequal_indices = 0;
			for(size_t i=0;i<std::min(other_sorted.indexWallCorner[vecindex].size(),this_sorted.indexWallCorner[vecindex].size());i++){
				unequal_indices += !(other_sorted.indexWallCorner[vecindex][i] == this_sorted.indexWallCorner[vecindex][i]);
			}
			result &= unequal_indices == 0;
			if(unequal_indices) fprintf(stderr,"\n%i Indices differ in indexWallCorner[%i]",unequal_indices,vecindex);

		}

		for(uchar vecindex=0;vecindex<12;vecindex++){

			if(other_sorted.indexEdgeEdge[vecindex].size() !=
				 this_sorted. indexEdgeEdge[vecindex].size()){
				fprintf(stderr,"\nUnequal Size of indexEdgeEdge[%i]",vecindex);
			}
			std::sort(other_sorted.indexEdgeEdge[vecindex].begin(),
					      other_sorted.indexEdgeEdge[vecindex].end());
			std::sort(this_sorted.indexEdgeEdge[vecindex].begin(),
					      this_sorted.indexEdgeEdge[vecindex].end());
			int unequal_indices = 0;
			for(size_t i=0;i<std::min(other_sorted.indexEdgeEdge[vecindex].size(),this_sorted.indexEdgeEdge[vecindex].size());i++){
				unequal_indices += !(other_sorted.indexEdgeEdge[vecindex][i] == this_sorted.indexEdgeEdge[vecindex][i]);
			}
			result &= unequal_indices == 0;
			if(unequal_indices) fprintf(stderr,"\n%i Indices differ in indexEdgeEdge[%i]",unequal_indices,vecindex);

			if(other_sorted.indexEdgeCorner[vecindex].size() !=
				 this_sorted. indexEdgeCorner[vecindex].size()){
				fprintf(stderr,"\nUnequal Size of indexEdgeCorner[%i]",vecindex);
			}
			std::sort(other_sorted.indexEdgeCorner[vecindex].begin(),
					      other_sorted.indexEdgeCorner[vecindex].end());
			std::sort(this_sorted.indexEdgeCorner[vecindex].begin(),
					      this_sorted.indexEdgeCorner[vecindex].end());
			unequal_indices = 0;
			for(size_t i=0;i<std::min(other_sorted.indexEdgeCorner[vecindex].size(),this_sorted.indexEdgeCorner[vecindex].size());i++){
				unequal_indices += !(other_sorted.indexEdgeCorner[vecindex][i] == this_sorted.indexEdgeCorner[vecindex][i]);
			}
			result &= unequal_indices == 0;
			if(unequal_indices) fprintf(stderr,"\n%i Indices differ in indexEdgeCorner[%i]",unequal_indices,vecindex);

		}
		{
			int unequal_indices = 0;
			for(size_t i=0;i<8;i++){
				unequal_indices += !(other_sorted.indexCorner[i] == this_sorted.indexCorner[i]);
			}
			result &= unequal_indices == 0;
			if(unequal_indices) fprintf(stderr,"\n%i Indices differ in indexCorner",unequal_indices);
		}

		return result;
	}

	inline void print() const {
		MCNSplit this_sorted = *this;

		for(uchar vecindex=0;vecindex<6;vecindex++){
			std::sort(this_sorted.indexWallWall[vecindex].begin(),
					      this_sorted.indexWallWall[vecindex].end());
			fprintf(stderr,"\nindexWallWall[%i]:",vecindex);
			for(size_t i=0;i<this_sorted.indexWallWall[vecindex].size();i++){
				fprintf(stderr," %li",this_sorted.indexWallWall[vecindex][i]);
			}
			std::sort(this_sorted.indexWallEdge[vecindex].begin(),
					      this_sorted.indexWallEdge[vecindex].end());
			fprintf(stderr,"\nindexWallEdge[%i]:",vecindex);
			for(size_t i=0;i<this_sorted.indexWallEdge[vecindex].size();i++){
				fprintf(stderr," %li",this_sorted.indexWallEdge[vecindex][i]);
			}
			std::sort(this_sorted.indexWallCorner[vecindex].begin(),
					      this_sorted.indexWallCorner[vecindex].end());
			fprintf(stderr,"\nindexWallCorner[%i]:",vecindex);
			for(size_t i=0;i<this_sorted.indexWallCorner[vecindex].size();i++){
				fprintf(stderr," %li",this_sorted.indexWallCorner[vecindex][i]);
			}
		}
		for(uchar vecindex=0;vecindex<12;vecindex++){

			std::sort(this_sorted.indexEdgeEdge[vecindex].begin(),
					      this_sorted.indexEdgeEdge[vecindex].end());
			fprintf(stderr,"\nindexEdgeEdge[%i]:",vecindex);
			for(size_t i=0;i<this_sorted.indexEdgeEdge[vecindex].size();i++){
				fprintf(stderr," %li",this_sorted.indexEdgeEdge[vecindex][i]);
			}

			std::sort(this_sorted.indexEdgeCorner[vecindex].begin(),
					      this_sorted.indexEdgeCorner[vecindex].end());
			fprintf(stderr,"\nindexWallEdge[%i]:",vecindex);
			for(size_t i=0;i<this_sorted.indexEdgeCorner[vecindex].size();i++){
				fprintf(stderr," %li",this_sorted.indexEdgeCorner[vecindex][i]);
			}
		}
		fprintf(stderr,"\nindecCorner:");
		for(size_t i=0;i<8;i++){
			fprintf(stderr," %li",this_sorted.indexCorner[i]);
		}
	}

	inline void sort(){
		for(uchar vecindex=0;vecindex<6;vecindex++){
			std::sort(indexWallWall[vecindex].begin(),
					      indexWallWall[vecindex].end());
			std::sort(indexWallEdge[vecindex].begin(),
					      indexWallEdge[vecindex].end());
			std::sort(indexWallCorner[vecindex].begin(),
					      indexWallCorner[vecindex].end());
		}
		for(uchar vecindex=0;vecindex<12;vecindex++){

			std::sort(indexEdgeEdge[vecindex].begin(),
					      indexEdgeEdge[vecindex].end());
			std::sort(indexEdgeCorner[vecindex].begin(),
					      indexEdgeCorner[vecindex].end());
		}
	}

	CellList indexWallWall[6];
	CellList indexWallEdge[6];
	CellList indexWallCorner[6];
	CellList indexEdgeEdge[12];
	CellList indexEdgeCorner[12];



	uchar vecnum; size_t innerIndex; size_t runIndex;



	inline void add(uchar vectype, uchar vecindex,size_t cellindex){
		switch(vectype){
		case VECTYPE_WALLWALL:
		indexWallWall[vecindex].push_back(cellindex);
		break;
		case VECTYPE_WALLEDGE:
		indexWallEdge[vecindex].push_back(cellindex);
		break;
		case VECTYPE_WALLCORN:
		indexWallCorner[vecindex].push_back(cellindex);
		break;
		case VECTYPE_EDGEEDGE:
		indexEdgeEdge[vecindex].push_back(cellindex);
		break;
		case VECTYPE_EDGECORN:
		indexEdgeCorner[vecindex].push_back(cellindex);
		break;
		}
	}
	inline size_t border_size(){
		size_t result = 0;
		for(uchar index=0;index<6;index++){
			result += indexWallWall[index].size() +
					      indexWallEdge[index].size() +
					      indexWallCorner[index].size() +
					      indexEdgeEdge[index].size() +
					      indexEdgeCorner[index].size();
		}
		for(uchar index=6;index<12;index++){
      result += indexEdgeEdge[index].size() +
                indexEdgeCorner[index].size();
		}
		return result;
	}

	//TODO: Extremely ugly!
	inline MCNCompact::CellReferenceNum get(size_t i){
		if(!i) {vecnum=0;innerIndex=0;runIndex=0;}
		if(i==runIndex+1){
			innerIndex++;
			if(innerIndex >= indexWallWall[vecnum].size()){
				innerIndex = 0;
				vecnum++;
			}
			uchar vecnumreturn = vecnum <18 ?
					(vecnum/6)*16+(vecnum%6) :
					48+((vecnum-18)/12)*16+((vecnum-18)%12);
			return MCNCompact::CellReferenceNum(vecnumreturn,indexWallWall[vecnum][innerIndex]);
		}
	}
	inline void clear(){
		for(uchar index=0;index<6;index++){
			indexWallWall[index].clear();
			indexWallEdge[index].clear();
			indexWallCorner[index].clear();
			indexEdgeEdge[index].clear();
			indexEdgeCorner[index].clear();
		}
		for(uchar index=6;index<12;index++){
      indexEdgeEdge[index].clear();
      indexEdgeCorner[index].clear();
		}
	}

	inline void addWall(const MCNSplit &other, uchar wallIndex){
			indexWallWall[wallIndex].insert(
					indexWallWall[wallIndex].end(),
					other.indexWallWall[wallIndex].begin(),
					other.indexWallWall[wallIndex].end());
			indexWallEdge[wallIndex].insert(
					indexWallEdge[wallIndex].end(),
					other.indexWallEdge[wallIndex].begin(),
					other.indexWallEdge[wallIndex].end());
			indexWallCorner[wallIndex].insert(
					indexWallCorner[wallIndex].end(),
					other.indexWallCorner[wallIndex].begin(),
					other.indexWallCorner[wallIndex].end());
	}

	inline void overwriteWall(const MCNSplit &other, uchar sourceIndex){
		indexWallWall[sourceIndex]   = other.indexWallWall[sourceIndex];
		indexWallEdge[sourceIndex]   = other.indexWallEdge[sourceIndex];
		indexWallCorner[sourceIndex] = other.indexWallCorner[sourceIndex];
	}

	inline void overwriteEdge(const MCNSplit &other, uchar sourceIndex){
		indexEdgeEdge[sourceIndex]   = other.indexEdgeEdge[sourceIndex];
		indexEdgeCorner[sourceIndex] = other.indexEdgeCorner[sourceIndex];
	}

	inline void overwriteTwoEdges(const MCNSplit &other, uchar edgeIndex1, uchar edgeIndex2){
		indexEdgeEdge[edgeIndex1]   = other.indexEdgeEdge[edgeIndex1];
		indexEdgeCorner[edgeIndex1] = other.indexEdgeCorner[edgeIndex1];
		indexEdgeEdge[edgeIndex2]   = other.indexEdgeEdge[edgeIndex2];
		indexEdgeCorner[edgeIndex2] = other.indexEdgeCorner[edgeIndex2];
	}
	inline void addThreeEdges(const MCNSplit &other,
			uchar edgeIndex1, uchar edgeIndex2, uchar edgeIndex3){
		indexEdgeEdge[edgeIndex1]   = other.indexEdgeEdge[edgeIndex1];
		indexEdgeCorner[edgeIndex1] = other.indexEdgeCorner[edgeIndex1];
		indexEdgeEdge[edgeIndex2]   = other.indexEdgeEdge[edgeIndex2];
		indexEdgeCorner[edgeIndex2] = other.indexEdgeCorner[edgeIndex2];
		indexEdgeEdge[edgeIndex3]   = other.indexEdgeEdge[edgeIndex3];
		indexEdgeCorner[edgeIndex3] = other.indexEdgeCorner[edgeIndex3];
	}

//	size_t indexInterior;
//	size_t indexCorner[8];
	size_t size();
//	size_t emptysize();
//	MeshCellNeighborhood &operator=(const MeshCellNeighborhood &value);
};





class BranchNeighborhood
{
public:
	BranchNeighborhood();
	volumetype indexWall[6];
	volumetype indexEdge[12];
	volumetype indexCorner[8];
};

//void addInterior_incremental
//(
//		leafstack leaves,
//		treeinfo info,
//		volumetype index, sidetype size,
//		sidetype ox, sidetype oy, sidetype oz,
//		volumetype lastLeaf,
//		const std::vector<volumetype> &leafParent
//);


void addBrickWallX_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshSeparate *pmesh = NULL
);

void addBrickWallY_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshSeparate *pmesh = NULL
);

void addBrickWallZ_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshSeparate *pmesh = NULL
);

void addBrickEdgeX_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		volumetype lastleaf2, volumetype lastleaf3,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshSeparate *pmesh = NULL
);

void addBrickEdgeY_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		volumetype lastleaf2, volumetype lastleaf3,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshSeparate *pmesh = NULL
);
void addBrickEdgeZ_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		volumetype lastleaf2, volumetype lastleaf3,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshSeparate *pmesh = NULL
);

void addBrickMiddle_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		volumetype lastleaf2, volumetype lastleaf3,
		volumetype lastleaf4, volumetype lastleaf5,
		volumetype lastleaf6, volumetype lastleaf7,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshSeparate *pmesh = NULL
);

void addBrickInterior_indexed_incremental_optimized
(
		volumetype lastleaf, const ParentArray &leafParent,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		treeinfo info,
		const MarchingCubesIndexed &mc,
		MeshSeparate *pmesh = NULL,
		bool verbose = false
);
















#endif /* TREEANDBRICK_INCREMENTAL_HPP_ */
