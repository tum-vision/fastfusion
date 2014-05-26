/*
 * geometryfusion_mipmap_cpu.cpp
 *
 *  Created on: Apr 14, 2013
 *      Author: steinbrf
 */
#include <omp.h>
#include <auxiliary/debug.hpp>
#include <auxiliary/memory.hpp>
#include "treeandbrick.hpp"
#include "treeandbrick_incremental.hpp"
#include "treeandbrick_incremental_recursive.hpp"
#include "mesh.hpp"
#include "geometryfusion_mipmap_cpu.hpp"

#include "inline_functions.hpp"

#include <stdlib.h>
#include <xmmintrin.h> // SSE  intrinsics
#include <emmintrin.h> // SSE2 intrinsics
#include <pmmintrin.h> // SSE3 intrinsics
#include <smmintrin.h>
#include <immintrin.h> // AVX  intrinsics

#include "update_functions.cpp"


#include <set>
#include <map>


#ifdef BRANCHNEIGHBORHOOD_REFERECE
#include "meshcelltraversalSplitReferenced.hpp"
#else
#include "meshcelltraversal.hpp"
#endif

#include "loopclosure.hpp"

#include <auxiliary/memory.hpp>

#include "meshcelltraversal.hpp"
#include "meshcelltraversal_compact.hpp"



//########################## OwnBranchIndicesArray_ ############################

FusionMipMapCPU::OwnBranchIndicesArray_::OwnBranchIndicesArray_()
:_array(NULL), _size(0),_capacity(0)
{}

FusionMipMapCPU::OwnBranchIndicesArray_::~OwnBranchIndicesArray_(){
	fprintf(stderr,"\nDestroying Branch Indices Array");
 clear();
	fprintf(stderr,"\nBranch Indices Array destroyed");
}

#ifdef BRANCHNEIGHBORHOOD_REFERECE
MCNSplit* &FusionMipMapCPU::OwnBranchIndicesArray_::operator[](size_t pos){
#else
MCNSplit &FusionMipMapCPU::OwnBranchIndicesArray_::operator[](size_t pos){
#endif
	if(pos < _size){
		return _array[pos];
	}
	else{
		fprintf(stderr,"\nERROR: Wrong Index in MeshCellNeighborhood Array! %li >= %li",pos,_size);
		return _dummy;
	}
}

size_t FusionMipMapCPU::OwnBranchIndicesArray_::size() const {
	return _size;
}

size_t FusionMipMapCPU::OwnBranchIndicesArray_::capacity() const {
	return _capacity;
}

#ifdef BRANCHNEIGHBORHOOD_REFERECE
void FusionMipMapCPU::OwnBranchIndicesArray_::resize(size_t count, MCNSplit* value){
#else
void FusionMipMapCPU::OwnBranchIndicesArray_::resize(size_t count, MCNSplit value){
#endif
	reserve(count);
	for(size_t i=_size;i<count;i++){
		_array[i] = value;
	}
	_size = count;
}

void FusionMipMapCPU::OwnBranchIndicesArray_::reserve(size_t count){
	if(count > _capacity){
		//TODO: Possible std::vector-like power of 2
		size_t capacity = count;

#ifdef BRANCHNEIGHBORHOOD_REFERECE
		MCNSplit **array = new MCNSplit*[capacity];
#else
		MCNSplit *array = new MCNSplit[capacity];
#endif

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

void FusionMipMapCPU::OwnBranchIndicesArray_::clear(){
	if(_array) delete [] _array; _array = NULL;
	_size = _capacity = 0;
}

#ifdef BRANCHNEIGHBORHOOD_REFERECE
void FusionMipMapCPU::OwnBranchIndicesArray_::push_back(MCNSplit* const &value){
#else
void FusionMipMapCPU::OwnBranchIndicesArray_::push_back(const MCNSplit &value){
#endif
	if(_size==_capacity){
		resize(_size+1,value);
	}
	else{
		_array[_size] = value;
		_size++;
	}
}

FusionMipMapCPU::OwnBranchIndicesCompactArray_::OwnBranchIndicesCompactArray_()
:_array(NULL), _size(0),_capacity(0)
{}

FusionMipMapCPU::OwnBranchIndicesCompactArray_::~OwnBranchIndicesCompactArray_(){
	fprintf(stderr,"\nDestroying Branch Indices Array");
 clear();
	fprintf(stderr,"\nBranch Indices Array destroyed");
}

MCNCompact &FusionMipMapCPU::OwnBranchIndicesCompactArray_::operator[](size_t pos){
	if(pos < _size){
		return _array[pos];
	}
	else{
		fprintf(stderr,"\nERROR: Wrong Index in MeshCellNeighborhood Array! %li >= %li",pos,_size);
		return _dummy;
	}
}

size_t FusionMipMapCPU::OwnBranchIndicesCompactArray_::size() const {
	return _size;
}

size_t FusionMipMapCPU::OwnBranchIndicesCompactArray_::capacity() const {
	return _capacity;
}

void FusionMipMapCPU::OwnBranchIndicesCompactArray_::resize(size_t count, MCNCompact value){
	reserve(count);
	for(size_t i=_size;i<count;i++){
		_array[i] = value;
	}
	_size = count;
}

void FusionMipMapCPU::OwnBranchIndicesCompactArray_::reserve(size_t count){
	if(count > _capacity){
		//TODO: Possible std::vector-like power of 2
		size_t capacity = count;

		MCNCompact *array = NULL;

		//The non-working C-Version
//		array = (MeshCellNeighborhood*)malloc(capacity*sizeof(MeshCellNeighborhood));
//		bool result = posix_memalign((void**)&array,sizeof(MeshCellNeighborhood),capacity*sizeof(MeshCellNeighborhood));
//		if(_capacity){
//			fprintf(stderr,"\nWARNING: Actually resizing MeshCellNeighborhood Array => INEFFICIENT!");
//			if(_size){
//				memcpy(array,_array,_size*sizeof(MeshCellNeighborhood));
//			}
//			free(_array);
//		}

		//The C++-Version
		array = new MCNCompact[capacity];
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

void FusionMipMapCPU::OwnBranchIndicesCompactArray_::clear(){
	if(_array) delete [] _array; _array = NULL;
	_size = _capacity = 0;
}

void FusionMipMapCPU::OwnBranchIndicesCompactArray_::push_back(const MCNCompact &value){
	if(_size==_capacity){
		resize(_size+1,value);
	}
	else{
		_array[_size] = value;
		_size++;
	}
}

//########################## OwnLeafIndicesArray_ ############################

FusionMipMapCPU::OwnLeafIndicesArray_::OwnLeafIndicesArray_()
:_array(NULL), _size(0),_capacity(0)
{}

FusionMipMapCPU::OwnLeafIndicesArray_::~OwnLeafIndicesArray_(){
	fprintf(stderr,"\nDestroying Leaf Indices Array");
 clear();
	fprintf(stderr,"\nLeaf Indices Array destroyed");
}

FusionMipMapCPU::LeafNeighborhood &FusionMipMapCPU::OwnLeafIndicesArray_::operator[](size_t pos){
	if(pos < _size){
		return _array[pos];
	}
	else{
		fprintf(stderr,"\nERROR: Wrong Index in MeshCellNeighborhood Array! %li >= %li",pos,_size);
		return _dummy;
	}
}

size_t FusionMipMapCPU::OwnLeafIndicesArray_::size() const {
	return _size;
}

size_t FusionMipMapCPU::OwnLeafIndicesArray_::capacity() const {
	return _capacity;
}

void FusionMipMapCPU::OwnLeafIndicesArray_::resize(size_t count, FusionMipMapCPU::LeafNeighborhood value){
	reserve(count);
	for(size_t i=_size;i<count;i++){
		_array[i] = value;
	}
	_size = count;
}

void FusionMipMapCPU::OwnLeafIndicesArray_::reserve(size_t count){
	if(count > _capacity){
		//TODO: Possible std::vector-like power of 2
		size_t capacity = count;

		FusionMipMapCPU::LeafNeighborhood *array = NULL;

		//The C++-Version
		array = new FusionMipMapCPU::LeafNeighborhood[capacity];
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

void FusionMipMapCPU::OwnLeafIndicesArray_::clear(){
	if(_array) delete [] _array; _array = NULL;
	_size = _capacity = 0;
}

void FusionMipMapCPU::OwnLeafIndicesArray_::push_back(const FusionMipMapCPU::LeafNeighborhood &value){
	if(_size==_capacity){
		resize(_size+1,value);
	}
	else{
		_array[_size] = value;
		_size++;
	}
}

FusionMipMapCPU::LeafNeighborhood &FusionMipMapCPU::OwnLeafIndicesArray_::back(){
	if(_size){
		return _array[_size-1];
	}
	else{
		fprintf(stderr,"\nERROR: LeafIndices Array back() with no elements!");
		return _dummy;
	}
}



//########################## OwnMeshCellArray_ ############################
FusionMipMapCPU::OwnMeshCellArray_::OwnMeshCellArray_()
:_array(NULL), _size(0),_capacity(0)
{}

FusionMipMapCPU::OwnMeshCellArray_::~OwnMeshCellArray_(){
	fprintf(stderr,"\nDestroying Mesh Cell Array");
 clear();
	fprintf(stderr,"\nMesh Cell Array destroyed");
}

MeshCell &FusionMipMapCPU::OwnMeshCellArray_::operator[](size_t pos){
	if(pos < _size){
		return _array[pos];
	}
	else{
		fprintf(stderr,"\nERROR: Wrong Index in MeshCell Array! %li >= %li",pos,_size);
		return _dummy;
	}
}

//size_t FusionMipMapCPU::OwnMeshCellArray_::size() const {
//	return _size;
//}
//
//size_t FusionMipMapCPU::OwnMeshCellArray_::capacity() const {
//	return _capacity;
//}

void FusionMipMapCPU::OwnMeshCellArray_::resize(size_t count, MeshCell value){
	reserve(count);
	for(size_t i=_size;i<count;i++) _array[i] = value;
	_size = count;
}

void FusionMipMapCPU::OwnMeshCellArray_::reserve(size_t count){
	fprintf(stderr,"\nReserving %li Mesh Cells",count);
	if(count > _capacity){
		//TODO: Possible std::vector-like power of 2
		size_t capacity = count;
		MeshCell *array = NULL;
//		array = (MeshCell*)malloc(capacity*sizeof(MeshCell));
//		bool result = posix_memalign((void**)&array,sizeof(MeshCell),capacity*sizeof(MeshCell));
//		if(_capacity){
//			fprintf(stderr,"\nWARNING: Actually resizing MeshCell Array => INEFFICIENT!");
//			if(_size){
//				memcpy(array,_array,_size*sizeof(MeshCell));
//			}
//			free(_array);
//		}

		array = new MeshCell[capacity];
		if(_capacity){
			fprintf(stderr,"\nWARNING: Actually resizing MeshCell Array => INEFFICIENT!");
			if(_size){
				for(size_t i=0;i<_size;i++) array[i] = _array[i];
			}
			delete [] array;
		}

		_array = array;
		_capacity = capacity;
	}
	fprintf(stderr,"\nMesh Cells reserved");
}

void FusionMipMapCPU::OwnMeshCellArray_::clear(){
	if(_array) delete [] _array;
	_array = NULL;
	_size = _capacity = 0;
}

void FusionMipMapCPU::OwnMeshCellArray_::push_back(const MeshCell &value){
	if(_size==_capacity){
		resize(_size+1,value);
	}
	else{
		_array[_size] = value;
		_size++;
	}
}

MeshCell &FusionMipMapCPU::OwnMeshCellArray_::back(){
	if(_size){
		return _array[_size-1];
	}
	else{
		fprintf(stderr,"\nERROR: MeshCell Array back() with no elements!");
		return _dummy;
	}
}

FusionMipMapCPU::OwnMeshCellArray_ &FusionMipMapCPU::OwnMeshCellArray_::operator=
		(const FusionMipMapCPU::OwnMeshCellArray_ &value){
	if(_capacity<value._size){
		delete [] _array;
		_capacity = value._capacity;
		_array = new MeshCell[_capacity];
	}
	_size = value._size;
	for(size_t i=0;i<_size;i++){
		_array[i] = value._array[i];
	}
	return *this;
}








FusionMipMapCPU::FusionMipMapCPU(
		float offsetX, float offsetY, float offsetZ,
		float scale, float distanceThreshold,
		sidetype n, bool color)
: Fusion_AoS(0.0,0.0,0.0,scale,distanceThreshold,n,color),
  _bandwidth(_distanceThreshold / _scale*BANDWIDTHFACTOR),
  _brickLength(BRICKLENGTH),
  _brickSize(_brickLength*_brickLength*_brickLength),
  _nBranchesTotal(0), _nLeavesTotal(0),
  _nBranchesUsed(0), _nLeavesUsed(0),
  _nLeavesQueuedSurface(0), _nLeavesQueuedFrustum(0),
  _tree(NULL), _queueIndexOfLeaf(NULL),
  _leafNumberSurface(NULL), _leafNumberFrustum(NULL),
  _leafPos(NULL), _leafScale(NULL),
  _distance(NULL), _weights(NULL), _color(NULL),
  _branchNumber(NULL), _child(NULL),
  _nLeavesBeforeLastFrame(0),
  _sharedBoxes(NULL), _threaded(USE_THREADS), _threadValid(false),
  _imageWidth(0),_imageHeight(0),
  _deviceNumber(5),
  _pxp(NULL), _pyp(NULL),
  _avgTimeQueueSurface(0.0),_avgTimeQueueFrustum(0.0),
  _avgTimeBricksSurface(0.0),_avgTimeBricksFrustum(0.0),
  _avgTimeMesh(0.0),_avgTimeSumMesh(0.0),
  _verbose(true),
  _averageLeaves(0),
  _performIncrementalMeshing(true),
  _numberOfQueuedTreeBuds(0),
  _numberOfQueuedLeafBuds(0),
  _treeSizeSinceMeshing(16),
  _treeSizeForMeshing(16),
  _differentImageSize(false),
  _meshCellIsQueuedCurrent(new std::vector<bool>()),
  _meshCellIsQueuedNext(new std::vector<bool>()),
#ifdef SWITCH_MESHCELLS_SPLIT
  _meshCells(_meshCellsSplit),
#else
  _meshCells(_meshCellsCompact),
#endif
  _interactiveMemoryManagement(INTERACTIVE_MEMORY_MANAGEMENT)
,_meshThread(NULL),
//_threadMeshing(THREAD_MESHING)
_threadMeshing(false)
,_degenerate_faces(0),_mc(_brickLength,_brickLength)
,_treeinfo(NULL,_brickLength,_brickSize,
			std::min((double)_framesAdded,MIN_WEIGHT_FOR_SURFACE),
			_offset,_scale,&_degenerate_faces,_nBranchesUsed,_nLeavesUsed,_tree,
			_leafPos,_leafScale,_distance,_weights,_color)
,_meshingDone(0)
,_meshSeparateCurrent(new MeshSeparate(3)), _meshSeparateNext(new MeshSeparate(3))
,_meshCurrent(new MeshInterleaved(3)), _meshNext(new MeshInterleaved(3))
,_updateCurrent(new CellUpdate())
,_updateNext(new CellUpdate())

,_loopClosureLogging(false)
,_loopClosureEnabled(false)
,_loopClosureDone(true)
,_maxPosesPerLoop(MAX_NUMBER_POSES_PER_LOOP)
,_nLeavesLoopTotal(0)
,_nLeavesLoopUsed(0)
,_loopClosureFusion(NULL)
,_imageSaveThread(NULL)
,_loopClosureThread(NULL)
,_loopClosureMode(false)
,_weightsLoop(NULL)
,_leafMapLoop(NULL)
,_nLeavesMapped(0)
,_numCamsPerLeaf(NULL)
,_maxImageBufferSize(MAX_NUMBER_POSES_PER_LOOP)
,_loggingEnabled(false)
,_meshingStartFrame(0) ,_meshTime(0.0)
,_numCheckImages(0)
{

  fprintf(stderr,"\nCPU Tree has as Side Length of %i, a Voxel scale %f ^ 3, and a Bandwidth of %i .",_n,_scale,_bandwidth);

	//Memory Check CPU
	long long cpuMemory = getTotalSystemMemory()/2;
	std::cerr << "\nAvailable Memory on CPU: " << cpuMemory << " Bytes (" << memoryInfo(cpuMemory) << ")";

	//Available Memory
	long long availableMemory = cpuMemory; if(availableMemory > LAPPY_MEMORY) availableMemory = LAPPY_MEMORY;
	std::cerr << "\nThe Minimum of the two is " << availableMemory << " Bytes (" << memoryInfo(availableMemory) << ")";

	std::string treeinfo = getMaximalNumberOfLeavesForDynamicTreeMipMap(availableMemory,
			_brickLength,sizeof(float),sizeof(weighttype),_useColor*sizeof(colortype3),
			&_nLeavesTotal,&_nBranchesTotal);
	std::cerr << treeinfo;

	allocateMemoryCPU();

//	_newSubtreesSinceMeshingToQueue.subtreeIndex =
//			_newSubtreesSinceMeshingToQueue.parentLeaf =
//					_newSubtreesSinceMeshingToQueue.depth = NULL;


	_leafIsQueuedForMeshing.resize(_nLeavesTotal,false);

	_newBudsSinceMeshingToQueue.subtreeBuds = new std::vector<volumetype>();
	_newBudsSinceMeshingToQueue.subtreeBudsParentLeaf = new std::vector<volumetype>();
	_newBudsSinceMeshingToQueue.leafBuds = new std::vector<volumetype>();

	_newBudsSinceMeshingToAccumulate.subtreeBuds = new std::vector<volumetype>();
	_newBudsSinceMeshingToAccumulate.subtreeBudsParentLeaf = new std::vector<volumetype>();
	_newBudsSinceMeshingToAccumulate.leafBuds = new std::vector<volumetype>();

	_newBudsSinceMeshingToClear.subtreeBuds = new std::vector<volumetype>();
	_newBudsSinceMeshingToClear.subtreeBudsParentLeaf = new std::vector<volumetype>();
	_newBudsSinceMeshingToClear.leafBuds = new std::vector<volumetype>();



#ifdef RESERVE_VECTOR_MEMORY
//  ProcessMemoryStats usedMemory = getProcessMemory();
//  fprintf(stderr,"\nUsed Memory: %li Bytes",usedMemory.size*usedMemory.pageSize);
	char input[256];
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to reserve memory for the vectors");
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
	size_t totalMemoryForVectors = 0;

	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to reserve %i * %li = %li Bytes for _meshCellIndicesBranch",
				_nBranchesTotal,sizeof(MCNSplit),(size_t)_nBranchesTotal*sizeof(MCNSplit));
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
#ifdef MESHCELLINDICES_SPLIT
  _meshCellIndicesBranch.reserve(_nBranchesTotal);
  fprintf(stderr,"\nReserved at least %i*%li = %li Bytes for _meshCellIndicesBranch",
  		_nBranchesTotal,sizeof(MCNSplit),(size_t)_nBranchesTotal*sizeof(MCNSplit));
  fprintf(stderr,"\nThe Capacity of _meshCellIndicesBranch is now %li*%li = %li Bytes ",
  		_meshCellIndicesBranch.capacity(),sizeof(MCNSplit),_meshCellIndicesBranch.capacity()*sizeof(MCNSplit));
  totalMemoryForVectors += _meshCellIndicesBranch.capacity()*sizeof(MCNSplit);
#endif
#ifdef MESHCELLINDICES_COMPACT
  _meshCellIndicesBranchCompact.reserve(_nBranchesTotal);
  fprintf(stderr,"\nReserved at least %i*%li = %li Bytes for _meshCellIndicesBranchCompact",
  		_nBranchesTotal,sizeof(MCNCompact),(size_t)_nBranchesTotal*sizeof(MCNCompact));
  fprintf(stderr,"\nThe Capacity of _meshCellIndicesBranchCompact is now %li*%li = %li Bytes ",
  		_meshCellIndicesBranchCompact.capacity(),sizeof(MCNCompact),_meshCellIndicesBranchCompact.capacity()*sizeof(MCNCompact));
  totalMemoryForVectors += _meshCellIndicesBranchCompact.capacity()*sizeof(MCNCompact);
#endif


	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to reserve %i * %li = %li Bytes for _leafParent",
				_nLeavesTotal,sizeof(volumetype),(size_t)_nLeavesTotal*sizeof(volumetype));
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
  _leafParent.reserve(_nLeavesTotal);
  fprintf(stderr,"\nReserved at least %i*%li = %li Bytes for _leafParent",
  		_nLeavesTotal,sizeof(volumetype),(size_t)_nLeavesTotal*sizeof(volumetype));
  fprintf(stderr,"\nThe Capacity of _leafParent is now %li*%li = %li Bytes ",
  		_leafParent.capacity(),sizeof(volumetype),_leafParent.capacity()*sizeof(volumetype));
  totalMemoryForVectors += _leafParent.capacity()*sizeof(volumetype);

  unsigned long numberMeshCells = ((unsigned long)_nBranchesTotal*6)/2+
  		((unsigned long)_nBranchesTotal*12)/4+ ((unsigned long)_nBranchesTotal*8)/8;
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to reserve %li * %li = %li Bytes for _meshCells",
				numberMeshCells,sizeof(MeshCell),numberMeshCells*sizeof(MeshCell));
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
  _meshCells.reserve(numberMeshCells);
  fprintf(stderr,"\nReserved at least %li*%li = %li Bytes for _meshCells",
  		numberMeshCells,sizeof(MeshCell),numberMeshCells*sizeof(MeshCell));
  fprintf(stderr,"\nThe Capacity of _meshCells is now %li*%li = %li Bytes ",
  		_meshCells.capacity(),sizeof(MeshCell),_meshCells.capacity()*sizeof(MeshCell));
  totalMemoryForVectors += _meshCells.capacity()*sizeof(MeshCell);

	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to reserve %i * %li = %li Bytes for _meshCellIndicesLeaf",
				_nLeavesTotal,sizeof(LeafNeighborhood),(size_t)_nLeavesTotal*sizeof(LeafNeighborhood));
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
  _meshCellIndicesLeaf.reserve(_nLeavesTotal);
  fprintf(stderr,"\nReserved at least %i*%li = %li Bytes for _meshCellIndicesLeaf",
  		_nLeavesTotal,sizeof(LeafNeighborhood),(size_t)_nLeavesTotal*sizeof(LeafNeighborhood));
  fprintf(stderr,"\nThe Capacity of _meshCellIndicesLeaf is now %li*%li = %li Bytes ",
  		_meshCellIndicesLeaf.capacity(),sizeof(LeafNeighborhood),_meshCellIndicesLeaf.capacity()*sizeof(LeafNeighborhood));
  totalMemoryForVectors += _meshCellIndicesLeaf.capacity()*sizeof(LeafNeighborhood);
  fprintf(stderr,"\nTotal Memory reserved for Vectors = %li Bytes",totalMemoryForVectors);

//  usedMemory = getProcessMemory();
//  fprintf(stderr,"\nUsed Memory: %li Bytes",usedMemory.size*usedMemory.pageSize);


//	fprintf(stderr,"\nPress Enter to resize the vectors");
//	fprintf(stderr,"%s",fgets(input,256,stdin));
//	//  MeshCell testCell(7,0,0,0,1,BRANCHINIT);
//	//  _meshCells.resize(numberMeshCells,testCell);
//  _meshCellIndicesBranch.resize(_nBranchesTotal);
//  _leafParent.resize(_nLeavesTotal);
//  _meshCellIndicesLeaf.resize(_nLeavesTotal);
////  _meshCellIndicesBranch.resize(1);
////  _leafParent.resize(1);
////  _meshCellIndicesLeaf.resize(1);
//  fprintf(stderr,"\nResized the vectors");

	_boxMin.x = _boxMin.y = _boxMin.z = _boxMax.x = _boxMax.y = _boxMax.z = 0;
  //Add a single root node with no Children to the tree
  _nLeavesUsed = 0; _nLeavesQueuedSurface = 0; _nBranchesUsed = 16;
#ifdef MESHCELLINDICES_SPLIT
#ifdef BRANCHNEIGHBORHOOD_REFERECE
  _meshCellIndicesBranch.resize(16);
  for(size_t i=0;i<16;i++) _meshCellIndicesBranch[i] = new MCNSplit();
#else
  _meshCellIndicesBranch.resize(16);
#endif
#endif
#ifdef MESHCELLINDICES_COMPACT
  _meshCellIndicesBranchCompact.resize(16);
#endif

//  usedMemory = getProcessMemory();
//  fprintf(stderr,"\nUsed Memory: %li Bytes",usedMemory.size*usedMemory.pageSize);
  if(_verbose) fprintf(stderr,"\nFusionMipMapCPU created.\n\n");

#endif
}




FusionMipMapCPU::FusionMipMapCPU(FusionMipMapCPU *fusion, volumetype nLeavesTotalLoop)
: Fusion_AoS(fusion->_offset.x,fusion->_offset.y,fusion->_offset.z,fusion->_scale,fusion->_distanceThreshold,fusion->_n,fusion->_useColor),
_bandwidth(fusion->_bandwidth),
_brickLength(fusion->_brickLength),
_brickSize(fusion->_brickSize),
_nBranchesTotal(fusion->_nBranchesTotal), _nLeavesTotal(nLeavesTotalLoop),
_nBranchesUsed(fusion->_nBranchesUsed), _nLeavesUsed(0),
_nLeavesQueuedSurface(0), _nLeavesQueuedFrustum(0),
_tree(NULL), _queueIndexOfLeaf(NULL),
_leafNumberSurface(NULL), _leafNumberFrustum(NULL),
_leafPos(NULL), _leafScale(NULL),
_distance(NULL), _weights(NULL), _color(NULL),
_branchNumber(NULL), _child(NULL),
_sharedBoxes(NULL), _threaded(USE_THREADS), _threadValid(false),
_imageWidth(0),_imageHeight(0),
_deviceNumber(5),
_pxp(NULL), _pyp(NULL),
_avgTimeQueueSurface(0.0),_avgTimeQueueFrustum(0.0),
_avgTimeBricksSurface(0.0),_avgTimeBricksFrustum(0.0),
_avgTimeMesh(0.0),_avgTimeSumMesh(0.0),
_verbose(true),
_averageLeaves(0),
_performIncrementalMeshing(false),
_numberOfQueuedTreeBuds(0),
_numberOfQueuedLeafBuds(0),
_treeSizeSinceMeshing(16),
_treeSizeForMeshing(16),
_differentImageSize(false),
_meshCellIsQueuedCurrent(NULL),
_meshCellIsQueuedNext(NULL),
#ifdef SWITCH_MESHCELLS_SPLIT
  _meshCells(_meshCellsSplit),
#else
  _meshCells(_meshCellsCompact),
#endif
_interactiveMemoryManagement(false)
,_meshThread(NULL),
_threadMeshing(false)
,_degenerate_faces(0),_mc(_brickLength,_brickLength)
,_treeinfo(NULL,_brickLength,_brickSize,
		std::min((double)_framesAdded,MIN_WEIGHT_FOR_SURFACE),
		_offset,_scale,&_degenerate_faces,_nBranchesUsed,_nLeavesUsed,_tree,
		_leafPos,_leafScale,_distance,_weights,_color)
,_meshingDone(0)
,_meshSeparateCurrent(NULL), _meshSeparateNext(NULL)
,_meshCurrent(NULL), _meshNext(NULL)
,_updateCurrent(new CellUpdate())
,_updateNext(new CellUpdate())

,_loopClosureLogging(false)
,_loopClosureEnabled(false)
,_loopClosureDone(true)
,_maxPosesPerLoop(MAX_NUMBER_POSES_PER_LOOP)
,_nLeavesLoopTotal(0)
,_nLeavesLoopUsed(0)
,_loopClosureFusion(NULL)
,_imageSaveThread(NULL)
,_loopClosureThread(NULL)
,_loopClosureMode(true)
,_weightsLoop(NULL)
,_leafMapLoop(NULL)
,_nLeavesMapped(0)
,_numCamsPerLeaf(NULL)
,_maxImageBufferSize(MAX_NUMBER_POSES_PER_LOOP)
,_numCheckImages(0)
{
	_nLeavesLoopTotal = MAX_LOOP_CLOSURE_LEAVES;
	fprintf(stderr,"\nAllocating CPU Memory on Loop-Closure Fusion");
	allocateMemoryCPU();
	fprintf(stderr,"\nCPU Memory on Loop-Closure Fusion allocated");

	_nLeavesUsed = 0; _nLeavesQueuedSurface = 0;
	_boxMin.x = _boxMin.y = _boxMin.z = _boxMax.x = _boxMax.y = _boxMax.z = 0;

	fprintf(stderr,"\nLoop Closure Constructor done");
}

FusionMipMapCPU::~FusionMipMapCPU()
{
	fprintf(stderr,"\nDeleting Fusion Object");
	char input[256];

	fprintf(stderr,"\nDeleting image-related fields...");
	if(_pxp) delete _pxp; if(_pyp) delete _pyp;
	fprintf(stderr,"\nImage-related fields deleted");

	if(_newBudsSinceMeshingToQueue.subtreeBuds) delete _newBudsSinceMeshingToQueue.subtreeBuds;
	if(_newBudsSinceMeshingToQueue.subtreeBudsParentLeaf) delete _newBudsSinceMeshingToQueue.subtreeBudsParentLeaf;
	if(_newBudsSinceMeshingToQueue.leafBuds) delete _newBudsSinceMeshingToQueue.leafBuds;

	if(_newBudsSinceMeshingToAccumulate.subtreeBuds) delete _newBudsSinceMeshingToAccumulate.subtreeBuds;
	if(_newBudsSinceMeshingToAccumulate.subtreeBudsParentLeaf) delete _newBudsSinceMeshingToAccumulate.subtreeBudsParentLeaf;
	if(_newBudsSinceMeshingToAccumulate.leafBuds) delete _newBudsSinceMeshingToAccumulate.leafBuds;

	if(_newBudsSinceMeshingToClear.subtreeBuds) delete _newBudsSinceMeshingToClear.subtreeBuds;
	if(_newBudsSinceMeshingToClear.subtreeBudsParentLeaf) delete _newBudsSinceMeshingToClear.subtreeBudsParentLeaf;
	if(_newBudsSinceMeshingToClear.leafBuds) delete _newBudsSinceMeshingToClear.leafBuds;

	fprintf(stderr,"\nThere are %i Branches, %i Leaves, %li Mesh Cells",
			_nBranchesUsed,_nLeavesUsed,_meshCells.size());


	ProcessMemoryStats usedMemory = getProcessMemory();
	fprintf(stderr,"\nUsed Memory: %li Bytes",usedMemory.size);
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to delete Cells");
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}

	fprintf(stderr,"\nDeleting Meshes in Mesh Cells...");
	for(size_t i=0;i<_meshCells.size();i++){
//		delete _meshCells[i].mesh;
		delete _meshCells[i].meshinterleaved;
	}


	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to clear _meshCells");
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
	_meshCells.clear();

	if(_meshCellsCopy.size()){
		if(_interactiveMemoryManagement){
			fprintf(stderr,"\nPress Enter to clear _meshCellsCopy");
			fprintf(stderr,"%s",fgets(input,256,stdin));
		}
		_meshCellsCopy.clear();
	}


	//	usedMemory = getProcessMemory();
	//	fprintf(stderr,"\nUsed Memory: %li Bytes",usedMemory.size);
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to clear _meshCellIndicesBranch Contents");
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
	size_t elements = 0;
	for(size_t i=0;i<_meshCellIndicesBranch.size();i++){
#ifdef BRANCHNEIGHBORHOOD_REFERECE
		if(_meshCellIndicesBranch[i]){
			for(size_t j=0;j<6;j++){
				elements += _meshCellIndicesBranch[i]->indexWallWall[j].size();
				elements += _meshCellIndicesBranch[i]->indexWallEdge[j].size();
				elements += _meshCellIndicesBranch[i]->indexWallCorner[j].size();
				elements += _meshCellIndicesBranch[i]->indexEdgeEdge[j].size();
				elements += _meshCellIndicesBranch[i]->indexEdgeCorner[j].size();
			}
			for(size_t j=6;j<12;j++){
				elements += _meshCellIndicesBranch[i]->indexEdgeEdge[j].size();
				elements += _meshCellIndicesBranch[i]->indexEdgeCorner[j].size();
			}
		}
#else
		for(size_t j=0;j<6;j++){
			elements += _meshCellIndicesBranch[i].indexWallWall[j].size();
			elements += _meshCellIndicesBranch[i].indexWallEdge[j].size();
			elements += _meshCellIndicesBranch[i].indexWallCorner[j].size();
			elements += _meshCellIndicesBranch[i].indexEdgeEdge[j].size();
			elements += _meshCellIndicesBranch[i].indexEdgeCorner[j].size();
		}
		for(size_t j=6;j<12;j++){
			elements += _meshCellIndicesBranch[i].indexEdgeEdge[j].size();
			elements += _meshCellIndicesBranch[i].indexEdgeCorner[j].size();
		}
#endif
	}
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nThis should be at least %li * %li * 3 = %li Bytes",
				elements, sizeof(size_t),elements*(sizeof(size_t)+2*sizeof(size_t)));
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
	for(size_t i=0;i<_meshCellIndicesBranch.size();i++){
#ifdef BRANCHNEIGHBORHOOD_REFERECE
		if(_meshCellIndicesBranch[i]) delete _meshCellIndicesBranch[i];
#else
		for(size_t j=0;j<6;j++){
			_meshCellIndicesBranch[i].indexWallWall[j].clear();
			_meshCellIndicesBranch[i].indexWallEdge[j].clear();
			_meshCellIndicesBranch[i].indexWallCorner[j].clear();
			_meshCellIndicesBranch[i].indexEdgeEdge[j].clear();
			_meshCellIndicesBranch[i].indexEdgeCorner[j].clear();
		}
		for(size_t j=6;j<12;j++){
			_meshCellIndicesBranch[i].indexEdgeEdge[j].clear();
			_meshCellIndicesBranch[i].indexEdgeCorner[j].clear();
		}
		_meshCellIndicesBranch[i].clear();
#endif
	}

//	usedMemory = getProcessMemory();
//	fprintf(stderr,"\nUsed Memory: %li Bytes",usedMemory.size);
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to clear _meshCellIndicesBranch");
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
	_meshCellIndicesBranch.clear();


	//	usedMemory = getProcessMemory();
	//	fprintf(stderr,"\nUsed Memory: %li Bytes",usedMemory.size);
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to clear _meshCellIndicesLeaf Contents");
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
	elements = 0;
	for(size_t i=0;i<_meshCellIndicesLeaf.size();i++){
		elements += _meshCellIndicesLeaf[i].size();
	}
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nThis should be at least %li * %li * 3 = %li Bytes",
				elements, sizeof(size_t),elements*(sizeof(size_t)+2*sizeof(size_t)));
		fprintf(stderr,"%s",fgets(input,256,stdin));

	}
	for(size_t i=0;i<_meshCellIndicesLeaf.size();i++){
		_meshCellIndicesLeaf[i].clear();
	}

//	usedMemory = getProcessMemory();
//	fprintf(stderr,"\nUsed Memory: %li Bytes",usedMemory.size);
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to clear _meshCellIndicesLeaf");
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
	_meshCellIndicesLeaf.clear();


//	usedMemory = getProcessMemory();
//	fprintf(stderr,"\nUsed Memory: %li Bytes",usedMemory.size);
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to clear _leafParent");
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
	_leafParent.clear();
	if(_leafParentCopy.size()){
		if(_interactiveMemoryManagement){
			fprintf(stderr,"\nPress Enter to clear _leafParentCopy");
			fprintf(stderr,"%s",fgets(input,256,stdin));
		}
		_leafParentCopy.clear();
	}

	fprintf(stderr,"\nBranches and Leaves need %li MB memory",
			((long int)_nBranchesUsed*16*(long int)sizeof(volumetype) + (long int)_nLeavesUsed*((long int)sizeof(sidetype3)+(long int)sizeof(sidetype)+(long int)sizeof(volumetype)
					+(long int)_brickSize*((long int)sizeof(float)+(long int)sizeof(weighttype)+(long int)sizeof(colortype3))))/(1024*1024));

	if(_loggingEnabled && _frameStatistics.size()){
		{
			fprintf(stderr,"\nWriting Logfiles...");
			std::fstream logfileTime, logfileBranches, logfileLeaves, logfileMeshCells, logfileQueue;
			logfileTime.open("logTime.txt",std::ios::out);
			logfileBranches.open("logBranches.txt",std::ios::out);
			logfileLeaves.open("logLeaves.txt",std::ios::out);
			logfileMeshCells.open("logMeshCells.txt",std::ios::out);
			logfileQueue.open("logQueue.txt",std::ios::out);

			for(size_t i=0;i<_frameStatistics.size();i++){
				logfileTime      << i << " " <<
						(_frameStatistics[i].timeSDFUpdate + _frameStatistics[i].timeTraversal)/cv::getTickFrequency()*1000.0
						<< " " << _frameStatistics[i].timeSDFUpdate/cv::getTickFrequency()*1000.0 << " " << _frameStatistics[i].timeTraversal/cv::getTickFrequency()*1000.0
						<< std::endl;
				logfileBranches  << i << " " << _frameStatistics[i].newBranches << std::endl;
				logfileLeaves    << i << " " << _frameStatistics[i].newLeaves << std::endl;
				logfileMeshCells << i << " " << _frameStatistics[i].newMeshCells << std::endl;
				logfileQueue     << i << " " << _frameStatistics[i].leavesQueued << std::endl;
			}

			logfileTime.close();
			logfileBranches.close();
			logfileLeaves.close();
			logfileMeshCells.close();
			logfileQueue.close();
			fprintf(stderr,"\nLogfiles written.");


//			double sumTicksTraversal = 0.0; double sumTicksSDF = 0.0;
//			double sumTimeTraversal = 0.0; double sumTimeSDF = 0.0;
//
//			for(size_t i=0;i<_frameStatistics.size();i++){
//				sumTicksTraversal += _frameStatistics[i].timeTraversal;
//				sumTicksSDF += _frameStatistics[i].timeSDFUpdate;
//				double timeTraversal = _frameStatistics[i].timeTraversal/cv::getTickFrequency()*1000.0;
//				double timeSDF = _frameStatistics[i].timeSDFUpdate/cv::getTickFrequency()*1000.0;
//				sumTimeTraversal += timeTraversal;
//				sumTimeSDF += timeSDF;
//				fprintf(stderr,"\n %f %f %f %f",sumTicksTraversal/cv::getTickFrequency()*1000.0,sumTimeTraversal,
//						sumTicksSDF/cv::getTickFrequency()*1000.0,sumTimeSDF);
//			}
//
//			fprintf(stderr,"\nFrames Added: %i, Statistics Size: %li",_framesAdded,_frameStatistics.size());
//			fprintf(stderr,"\nTick Frequency: %f",cv::getTickFrequency());
//
//			fprintf(stderr,"\nOriginal Sums: Traversal: %f vs %f, SDF: %f vs %f",
//					sumTicksTraversal,_avgTimeQueueSurface,sumTicksSDF,_avgTimeBricksSurface);
//			fprintf(stderr,"\nTime Sums: Traversal: %f vs %f, SDF: %f vs %f",
//					sumTicksTraversal/cv::getTickFrequency()*1000.0,_avgTimeQueueSurface/cv::getTickFrequency()*1000.0,
//					sumTicksSDF/cv::getTickFrequency()*1000.0,_avgTimeBricksSurface/cv::getTickFrequency()*1000.0);
		}
		if(_meshTimes.size()>1){
			std::fstream logfileMesh;
			logfileMesh.open("logMesh.txt",std::ios::out);
			_meshTimes.back().frameNumber = _framesAdded;
			for(size_t i=1;i<_meshTimes.size();i++){
				logfileMesh << _meshTimes[i].frameNumber << " "
						<< _meshTimes[i].queueSize << " " << _meshTimes[i].timeUpdate/cv::getTickFrequency()*1000.0f << " "
						<< _meshTimes[i].arraySize << " " << _meshTimes[i].timeSum/cv::getTickFrequency()*1000.0f << std::endl;
			}
			logfileMesh.close();
		}

	}

//	usedMemory = getProcessMemory();
//	fprintf(stderr,"\nUsed Memory: %li Bytes",usedMemory.size);
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to free Arrays");
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}

	if(_meshThread) _meshThread->join();
	freeMemoryCPU();
	if(_meshCellIsQueuedCurrent) delete _meshCellIsQueuedCurrent;
	if(_meshCellIsQueuedNext) delete _meshCellIsQueuedNext;
	if(_meshSeparateCurrent) delete _meshSeparateCurrent;
	if(_meshSeparateNext)    delete _meshSeparateNext;

	if(_loopClosureFusion) delete _loopClosureFusion;
	if(_loopClosureThread) delete _loopClosureThread;

	if(_updateCurrent) delete _updateCurrent;
	if(_updateNext) delete _updateNext;

	if(_verbose) fprintf(stderr,"\nAverage Time for Surface Traversal: %f",
			_avgTimeQueueSurface/(cv::getTickFrequency()*_framesAdded));
	if(_verbose) fprintf(stderr,"\nAverage Time for Frustum Traversal: %f",
			_avgTimeQueueFrustum/(cv::getTickFrequency()*_framesAdded));
	if(_verbose) fprintf(stderr,"\nAverage Time for Brick Queue Surface: %f",
			_avgTimeBricksSurface/(cv::getTickFrequency()*_framesAdded));
	if(_verbose) fprintf(stderr,"\nAverage Time for Brick Queue Frustum: %f",
			_avgTimeBricksFrustum/(cv::getTickFrequency()*_framesAdded));
	if(_verbose) fprintf(stderr,"\nAverage Time for Marching Cubes Mesh Reconstruction: %f",
			_avgTimeMesh/(cv::getTickFrequency()*_framesAdded));
	if(_verbose) fprintf(stderr,"\nAverage Time for Summing Up the Mesh Cells: %f",
			_avgTimeSumMesh/(cv::getTickFrequency()*_framesAdded));
	if(_verbose) fprintf(stderr,"\nAverage Number of Leaves: %i",_averageLeaves/_framesAdded);
	fprintf(stderr,"\nFusionOctreeGPU deleted");


//	usedMemory = getProcessMemory();
//	fprintf(stderr,"\nUsed Memory: %li Bytes",usedMemory.size);
	fprintf(stderr,"\nEnd of FusionMipMapCPU Destructor.");
	if(_interactiveMemoryManagement){
		fprintf(stderr,"\nPress Enter to leave FusionMipMapCPU Destructor");
		fprintf(stderr,"%s",fgets(input,256,stdin));
	}
}

bool FusionMipMapCPU:: allocateMemoryCPU()
{
	long unsigned int totalMemAlloc = 0;
  _tree = new volumetype[_nBranchesTotal];
  for(volumetype i=0;i<_nBranchesTotal;i++) _tree[i] = BRANCHINIT;
  int allocResult;
#ifdef OWNAVX
  allocResult = posix_memalign((void**)&_distance,32,_nLeavesTotal*_brickSize*sizeof(float));
#else
  allocResult = posix_memalign((void**)&_distance,16,_nLeavesTotal*_brickSize*sizeof(float));
#endif
  if(allocResult) fprintf(stderr,"\nERROR: Aligned Allocation of the Distance Memory failed!");
  totalMemAlloc += _nLeavesTotal*_brickSize*sizeof(float);
  if(_loopClosureMode){
#ifdef OWNAVX
    allocResult = posix_memalign((void**)&_weightsLoop,32,_nLeavesTotal*_brickSize*sizeof(float));
#else
    allocResult = posix_memalign((void**)&_weightsLoop,16,_nLeavesTotal*_brickSize*sizeof(float));
#endif
    if(allocResult) fprintf(stderr,"\nERROR: Aligned Allocation of the Weights Memory for the Loop failed!");
    totalMemAlloc += _nLeavesTotal*_brickSize*sizeof(float);
    memset(_weightsLoop,0,_nLeavesTotal*_brickSize*sizeof(float));
  }
  else{
#ifdef OWNAVX
    allocResult = posix_memalign((void**)&_weights,32,_nLeavesTotal*_brickSize*sizeof(weighttype));
#else
    allocResult = posix_memalign((void**)&_weights,16,_nLeavesTotal*_brickSize*sizeof(weighttype));
#endif
    if(allocResult) fprintf(stderr,"\nERROR: Aligned Allocation of the Weights Memory failed!");
    totalMemAlloc += _nLeavesTotal*_brickSize*sizeof(weighttype);
    memset(_weights,0,_nLeavesTotal*_brickSize*sizeof(weighttype));
  }
  memset(_distance,0,_nLeavesTotal*_brickSize*sizeof(float));
  if(_useColor){
		_color   = new colortype3[_nLeavesTotal*_brickSize];
    totalMemAlloc += _nLeavesTotal*_brickSize*sizeof(colortype3);
		memset(_color,  0,_nLeavesTotal*_brickSize*sizeof(colortype3));
  }



  _queueIndexOfLeaf = new volumetype[_nLeavesTotal];
  _leafNumberSurface = new volumetype[_nLeavesTotal];
  _leafNumberFrustum = new volumetype[_nLeavesTotal];
  _leafPos = new sidetype3[_nLeavesTotal];
  _leafScale = new sidetype[_nLeavesTotal];


  if(_loopClosureMode){
  	_leafMapLoop = new volumetype[_nLeavesTotal];
  	for(volumetype i=0;i<_nLeavesTotal;i++) _leafMapLoop[i] = BRANCHINIT;
  }

  _branchNumber = new volumetype[2*MAXDEPTH];
  _child = new uchar[4*MAXDEPTH];

  totalMemAlloc += _nLeavesTotal*(4*sizeof(volumetype)+sizeof(sidetype3) + sizeof(sidetype) + sizeof(volumetype)*_loopClosureMode);
  fprintf(stderr,"\nAllocated %li Bytes of Memory on CPU",totalMemAlloc);
  fprintf(stderr,"\n= %i * %i * (%li + %li + %li) + %i * (4*%li + %li + %li + %li)",
  		_nLeavesTotal,_brickSize,sizeof(float),_loopClosureMode ? sizeof(float) : sizeof(weighttype),
  				sizeof(colortype3),_nLeavesTotal,sizeof(volumetype),sizeof(sidetype3),sizeof(sidetype),sizeof(volumetype));

	return true;
}



void FusionMipMapCPU::freeMemoryCPU()
{

	if(_tree)                   delete [] _tree;
	if(_queueIndexOfLeaf)       delete [] _queueIndexOfLeaf;
	if(_leafNumberSurface)      delete [] _leafNumberSurface;
	if(_leafNumberFrustum)      delete [] _leafNumberFrustum;
	if(_leafPos)                delete [] _leafPos;
	if(_leafScale)              delete [] _leafScale;
	if(_leafMapLoop)            delete [] _leafMapLoop;

	if(_distance)               free(_distance);
	if(_weights)                free(_weights);
	if(_weightsLoop)            free(_weightsLoop);
	if(_color)                  delete [] _color;

	if(_branchNumber) delete [] _branchNumber;
	if(_child) delete [] _child;


	_tree = NULL;
	_queueIndexOfLeaf = NULL;
	_leafNumberSurface = NULL;
	_leafNumberFrustum = NULL;
	_leafPos = NULL;
	_leafScale = NULL;

	_leafMapLoop = NULL;

	_distance = NULL;
	_weights = NULL;
	_color = NULL;

	_branchNumber = NULL;
	_child = NULL;

}

void FusionMipMapCPU::setThreadMeshing(bool threadMeshing)
{
	_threadMeshing = threadMeshing;
	fprintf(stderr,"\n%s threading the meshing",_threadMeshing ? "Activated" : "Deactivated");
}

void FusionMipMapCPU::setDepthChecks(int depthchecks)
{
	_numCheckImages = depthchecks; if (_numCheckImages<0) _numCheckImages = 0;
	fprintf(stderr,"\nSet the Number of Backward Depth Checks to %li",_numCheckImages);
}

void FusionMipMapCPU::setIncrementalMeshing(bool incrementalMeshing)
{
	_performIncrementalMeshing = incrementalMeshing;
	fprintf(stderr,"\nIncremental Meshing is %s.",_performIncrementalMeshing ? "activated" : "deactivated");
}

std::string FusionMipMapCPU::getDescription()
{
	char temp[300];
	sprintf(temp,"%.10f_%.10f_%.10f_%.10f_%.7i",_offset.x,_offset.y,_offset.z,_scale,_n);
	return std::string(temp);
}

bool FusionMipMapCPU::copyCPU2HDD()
{
	std::string filename = std::string("resource/") + getDescription() + ".dat";
	std::fstream file;
	file.open(filename.c_str(),std::ios::binary|std::ios::out);
	if(!file.is_open()){
		fprintf(stderr,"\n\nERROR: Could not open File \"%s\" for writing!",filename.c_str());
		return false;
	}

	file.write((char*)&_nBranchesUsed,sizeof(volumetype));
	file.write((char*)&_nLeavesUsed,sizeof(volumetype));
	if(_nBranchesUsed) file.write((char*)_tree,_nBranchesUsed*sizeof(volumetype));
	if(_nLeavesUsed){
		file.write((char*)_leafPos,_nLeavesUsed*sizeof(sidetype3));
		file.write((char*)_leafScale,_nLeavesUsed*sizeof(sidetype));
		file.write((char*)_distance,_nLeavesUsed*sizeof(float)*_brickSize);
		file.write((char*)_weights,_nLeavesUsed*sizeof(weighttype)*_brickSize);
		if(_useColor) file.write((char*)_color,_nLeavesUsed*sizeof(colortype3)*_brickSize);
	}
	file.close();

	return true;
}


bool FusionMipMapCPU::copyHDD2CPU()
{
	std::string filename = std::string("resource/") + getDescription() + ".dat";
	std::fstream file;
	file.open(filename.c_str(),std::ios::binary|std::ios::in);
	if(!file.is_open()){
		fprintf(stderr,"\n\nERROR: Could not open File \"%s\" for reading!",filename.c_str());
		return false;
	}

	file.read((char*)&_nBranchesUsed,sizeof(volumetype));
	file.read((char*)&_nLeavesUsed,sizeof(volumetype));
	if(_nBranchesUsed) file.read((char*)_tree,_nBranchesUsed*sizeof(volumetype));
	if(_nLeavesUsed){
		file.read((char*)_leafPos,_nLeavesUsed*sizeof(sidetype3));
		file.read((char*)_leafScale,_nLeavesUsed*sizeof(sidetype));
		file.read((char*)_distance,_nLeavesUsed*sizeof(float)*_brickSize);
		file.read((char*)_weights,_nLeavesUsed*sizeof(weighttype)*_brickSize);
		if(_useColor) file.read((char*)_color,_nLeavesUsed*sizeof(colortype3)*_brickSize);
	}
	file.close();

	return true;
}

bool FusionMipMapCPU::setInitialVolume(int minX, int minY, int minZ, int maxX, int maxY, int maxZ)
{
	if(minX>=maxX || minY>=maxY || minZ>=maxZ){
		return false;
	}

	fprintf(stderr,"\nSetting initial Volume");

	sidetype highX = maxX-minX;
	sidetype highY = maxY-minY;
	sidetype highZ = maxZ-minZ;
	sidetype high = std::max(std::max(highX,highY),highZ);
	fprintf(stderr,"\nMaximum Side Length is %i",high);
	for(_n=2*_brickLength;_n<=high;_n*=2);

	_offset.x = (minX-(_n-highX)/2)*_scale;
	_offset.y = (minY-(_n-highY)/2)*_scale;
	_offset.z = (minZ-(_n-highZ)/2)*_scale;

	fprintf(stderr,"\nInitial empty Tree will have Side Length %i",_n);

#ifdef MESHCELLINDICES_SPLIT
	initializeMeshCellStructures(_n,_meshCellIndicesBranch,_meshCellsSplit,_boundary);
#endif
#ifdef MESHCELLINDICES_COMPACT
	initializeMeshCellStructures_compact(_n,_meshCellIndicesBranchCompact,_meshCellsCompact,_boundaryCompact);
#endif

#if defined MESHCELLINDICES_SPLIT && defined MESHCELLINDICES_COMPACT
	if(_meshCellIndicesBranch.size() != _meshCellIndicesBranchCompact.size()){
		fprintf(stderr,"\nERROR: Initial neighborhooods have different sizes:"
				" %li vs %li",_meshCellIndicesBranch.size(),_meshCellIndicesBranchCompact.size());
		char c; std::cin >> c;
	}
	bool equal_neighborhoods = true;
	for(size_t i=0;i<_meshCellIndicesBranch.size();i++){
		MCNSplit compare = _meshCellIndicesBranchCompact[i];
		equal_neighborhoods &= compare == _meshCellIndicesBranch[i];
		if(!equal_neighborhoods){
			fprintf(stderr,"\nERROR: Split Neighborhood %li is different from Compact",i);
		}
	}
	if(equal_neighborhoods) fprintf(stderr,"\nCompact neighborhoods are equal to split ones"
			" after initialization");
#endif

	return true;
}


void computeBoundingboxIntCPU
(
		camPamsFloat p,
		const float *depth, int imWidth, int imHeight,
		int bandwidth,
		int *minX, int *minY, int *minZ,
		int *maxX, int *maxY, int *maxZ
)
{
	for(int y=0;y<imHeight;y++){
		for(int x=0;x<imWidth;x++){
			float pz = depth[y*imWidth+x];
			if(TRAVERSALHEIGHTFINITECPU){
				float px = ((float)x-p.cx)/p.fx*pz;
				float py = ((float)y-p.cy)/p.fy*pz;
				int vx = (int)((p.r11*px + p.r12*py + p.r13*pz + p.t1));
				int vy = (int)((p.r21*px + p.r22*py + p.r23*pz + p.t2));
				int vz = (int)((p.r31*px + p.r32*py + p.r33*pz + p.t3));


				if(*minX > vx - bandwidth) *minX = vx - bandwidth;
				if(*minY > vy - bandwidth) *minY = vy - bandwidth;
				if(*minZ > vz - bandwidth) *minZ = vz - bandwidth;

				if(*maxX < vx + bandwidth) *maxX = vx + bandwidth;
				if(*maxY < vy + bandwidth) *maxY = vy + bandwidth;
				if(*maxZ < vz + bandwidth) *maxZ = vz + bandwidth;
			}
			else{
			}
		}
	}
}

void computeBoundingboxIntCPU
(
		camPamsFloat p,
		const ushort *depth, float scaling, float maxcamdistance,
		int imWidth, int imHeight,
		int bandwidth,
		int *minX, int *minY, int *minZ,
		int *maxX, int *maxY, int *maxZ
)
{
	for(int y=0;y<imHeight;y++){
		for(int x=0;x<imWidth;x++){
			float pz = depth[y*imWidth+x]*scaling;
			if(pz>0.0f && pz<maxcamdistance){
				float px = ((float)x-p.cx)/p.fx*pz;
				float py = ((float)y-p.cy)/p.fy*pz;
				int vx = (int)((p.r11*px + p.r12*py + p.r13*pz + p.t1));
				int vy = (int)((p.r21*px + p.r22*py + p.r23*pz + p.t2));
				int vz = (int)((p.r31*px + p.r32*py + p.r33*pz + p.t3));


				if(*minX > vx - bandwidth) *minX = vx - bandwidth;
				if(*minY > vy - bandwidth) *minY = vy - bandwidth;
				if(*minZ > vz - bandwidth) *minZ = vz - bandwidth;

				if(*maxX < vx + bandwidth) *maxX = vx + bandwidth;
				if(*maxY < vy + bandwidth) *maxY = vy + bandwidth;
				if(*maxZ < vz + bandwidth) *maxZ = vz + bandwidth;
			}
			else{
			}
		}
	}
}

#include "traversal_wrapper_functions.cpp"


int FusionMipMapCPU::addMap(cv::Mat &depth, CameraInfo caminfo,
			std::vector<cv::Mat> rgb)
{
	//Parameter Helpers
	cv::Mat rot = caminfo.getRotation();
	cv::Mat trans = caminfo.getTranslation();
	cv::Mat intr = caminfo.getIntrinsic();

	_nLeavesBeforeLastFrame = _nLeavesUsed;
	volumetype branchesBeforeLastFrame = _nBranchesUsed;
	size_t meshCellsBeforeLastFrame = _meshCells.size();


	camPamsFloat p(
			rot.at<double>(0,0)/_scale, rot.at<double>(0,1)/_scale, rot.at<double>(0,2)/_scale,
			rot.at<double>(1,0)/_scale, rot.at<double>(1,1)/_scale, rot.at<double>(1,2)/_scale,
			rot.at<double>(2,0)/_scale, rot.at<double>(2,1)/_scale, rot.at<double>(2,2)/_scale,
			(trans.at<double>(0,0) - _offset.x)/_scale,
			(trans.at<double>(1,0) - _offset.y)/_scale,
			(trans.at<double>(2,0) - _offset.z)/_scale,
			intr.at<double>(0,0), intr.at<double>(1,1),
			intr.at<double>(0,2), intr.at<double>(1,2));

	float *data = (float*)depth.data;


	//Empty the Data Queue
	for(volumetype i=0;i<_nLeavesTotal;i++) _queueIndexOfLeaf[i] = MAXLEAFNUMBER;
	_nLeavesQueuedSurface = 0;
	_nLeavesQueuedFrustum = 0;

	bool firstMap = _nLeavesUsed == 0;

	if(!_n){
		_boxMin.x = _boxMin.y = _boxMin.z = 1e6;
		_boxMax.x = _boxMax.y = _boxMax.z = -1e6;
		fprintf(stderr,"\nComputing Initial Bounding Box");
		computeBoundingboxIntCPU(p,(float*)depth.data,
				depth.cols,depth.rows,_bandwidth,
				&_boxMin.x,&_boxMin.y,&_boxMin.z,&_boxMax.x,&_boxMax.y,&_boxMax.z);

		if(setInitialVolume(_boxMin.x,_boxMin.y,_boxMin.z,_boxMax.x,_boxMax.y,_boxMax.z)){
			fprintf(stderr,"\nInteger Bounding Box of first frame: [%i %i %i]-[%i %i %i]",
					_boxMin.x,_boxMin.y,_boxMin.z,_boxMax.x,_boxMax.y,_boxMax.z);
		}
		else{
			fprintf(stderr,"\nERROR: Initial Bounding Box Computation failed!");
			fprintf(stderr,"\nErroneous Bounding Box of first frame: [%i %i %i]-[%i %i %i]",
					_boxMin.x,_boxMin.y,_boxMin.z,_boxMax.x,_boxMax.y,_boxMax.z);
		}
		p = 	camPamsFloat(
				rot.at<double>(0,0)/_scale, rot.at<double>(0,1)/_scale, rot.at<double>(0,2)/_scale,
				rot.at<double>(1,0)/_scale, rot.at<double>(1,1)/_scale, rot.at<double>(1,2)/_scale,
				rot.at<double>(2,0)/_scale, rot.at<double>(2,1)/_scale, rot.at<double>(2,2)/_scale,
				(trans.at<double>(0,0) - _offset.x)/_scale,
				(trans.at<double>(1,0) - _offset.y)/_scale,
				(trans.at<double>(2,0) - _offset.z)/_scale,
				intr.at<double>(0,0), intr.at<double>(1,1),
				intr.at<double>(0,2), intr.at<double>(1,2));
	}


	double time1 = (double)cv::getTickCount();

	_differentImageSize |= _imageWidth!=depth.cols || _imageHeight!=depth.rows;
	if(_imageWidth!=depth.cols || _imageHeight!=depth.rows){
		_imageWidth = depth.cols; _imageHeight = depth.rows;
		if(_sharedBoxes) delete [] _sharedBoxes;
		_sharedBoxes = new sidetype3[_imageWidth*_imageHeight*2];
		if(_pxp) delete _pxp; if(_pyp) delete _pyp;
	  int alignResult = posix_memalign((void**)&_pxp,16,_imageWidth*sizeof(float));
	  alignResult += posix_memalign((void**)&_pyp,16,_imageHeight*sizeof(float));
	  if(alignResult) fprintf(stderr,"\nERROR: Aligned Alloc in addMap failed!");
		for(int x=0;x<_imageWidth;x++){
			_pxp[x] = ((float)x-p.cx)/p.fx;
		}
		for(int y=0;y<_imageHeight;y++){
			_pyp[y] = ((float)y-p.cy)/p.fy;
		}
		_newBudsSinceMeshingToQueue.subtreeBuds->resize(_imageWidth*_imageHeight);
		_newBudsSinceMeshingToQueue.subtreeBudsParentLeaf->resize(_imageWidth*_imageHeight);
		_newBudsSinceMeshingToQueue.leafBuds->resize(_imageWidth*_imageHeight);
	  if(alignResult) fprintf(stderr,"\nERROR: Aligned Alloc in addMap failed!");
	}

	_boxMin.x = _boxMin.y = _boxMin.z = 0;
	_boxMax.x = _boxMax.y = _boxMax.z = _n-1;


	_threadValid = true;
	CameraInfo caminfo2 = caminfo;
	caminfo2.setExtrinsic(caminfo2.getExtrinsicInverse());
	cv::Mat rot2 = caminfo2.getRotation();
	cv::Mat trans2 = caminfo2.getTranslation();
	cv::Mat intr2 = caminfo2.getIntrinsic();
	camPamsFloat pInv(
	rot2.at<double>(0,0), rot2.at<double>(0,1), rot2.at<double>(0,2),
	rot2.at<double>(1,0), rot2.at<double>(1,1), rot2.at<double>(1,2),
	rot2.at<double>(2,0), rot2.at<double>(2,1), rot2.at<double>(2,2),
	rot2.at<double>(0,0)*_offset.x + rot2.at<double>(0,1)*_offset.y + rot2.at<double>(0,2)*_offset.z + trans2.at<double>(0,0),
	rot2.at<double>(1,0)*_offset.x + rot2.at<double>(1,1)*_offset.y + rot2.at<double>(1,2)*_offset.z + trans2.at<double>(1,0),
	rot2.at<double>(2,0)*_offset.x + rot2.at<double>(2,1)*_offset.y + rot2.at<double>(2,2)*_offset.z + trans2.at<double>(2,0),
	intr2.at<double>(0,0), intr2.at<double>(1,1),
	intr2.at<double>(0,2), intr2.at<double>(1,2));


	float m11 = pInv.r11; float m12 = pInv.r12;
	float m13 = pInv.r13; float m14 = pInv.t1 ;
	float m21 = pInv.r21; float m22 = pInv.r22;
	float m23 = pInv.r23; float m24 = pInv.t2 ;
	float m31 = pInv.r31; float m32 = pInv.r32;
	float m33 = pInv.r33; float m34 = pInv.t3;

	boost::thread *distanceUpdateThread = NULL;

	if(_threaded){
	distanceUpdateThread = new boost::thread(	updateWrapperFloat,SDFUpdateParameterFloat(
			(const float*)data,(const uchar*)rgb[2].data,(const uchar*)rgb[1].data,(const uchar*)rgb[0].data,
			_imageWidth,_imageHeight,
			m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,
			pInv.fx,pInv.fy,pInv.cx,pInv.cy,_scale,_distanceThreshold,
			_leafNumberSurface,_leafPos,_leafScale,
			_distance,_weights,_color,_brickLength),&_nLeavesQueuedSurface,&_threadValid,0);
	}

	__attribute__ ((aligned (16))) float qxp1[_imageWidth];
	__attribute__ ((aligned (16))) float qxp2[_imageWidth];
	__attribute__ ((aligned (16))) float qxp3[_imageWidth];
	for(int x=0;x<_imageWidth;x++){
		qxp1[x] = p.r11*_pxp[x];
		qxp2[x] = p.r21*_pxp[x];
		qxp3[x] = p.r31*_pxp[x];
	}
	__attribute__ ((aligned (16))) float qyp1[_imageHeight];
	__attribute__ ((aligned (16))) float qyp2[_imageHeight];
	__attribute__ ((aligned (16))) float qyp3[_imageHeight];
	for(int y=0;y<_imageHeight;y++){
		qyp1[y] = p.r12*_pyp[y] + p.r13;
		qyp2[y] = p.r22*_pyp[y] + p.r23;
		qyp3[y] = p.r32*_pyp[y] + p.r33;
	}

	fprintf(stderr, "T");

//	transformLoopSimPrecalculated(qxp1,qxp2,qxp3,qyp1,qyp2,qyp3,p.t1,p.t2,p.t3,
//			_n,_bandwidth,_brickLength,_imageWidth,_imageHeight,_boxMin,_boxMax,data,
//			_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueued,
//			_leafNumber,_leafPos,_leafScale,_queueIndexOfLeaf,_child,_branchNumber);

//	transformLoopSimPrecalculatedNeg(qxp1,qxp2,qxp3,qyp1,qyp2,qyp3,p.t1,p.t2,p.t3,
//			_n,_bandwidth,_brickLength,_imageWidth,_imageHeight,_boxMin,_boxMax,data,
//			_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueuedSurface,
//			_leafNumberSurface,_leafPos,_leafScale,_queueIndexOfLeaf,_child,_branchNumber);

	transformLoopSimPrecalculatedNeg_vis(qxp1,qxp2,qxp3,qyp1,qyp2,qyp3,p.t1,p.t2,p.t3,
			_n,_bandwidth,_brickLength,_imageWidth,_imageHeight,_boxMin,_boxMax,data,
			_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueuedSurface,
			_leafNumberSurface,_leafPos,_leafScale,_queueIndexOfLeaf,_child,_branchNumber
			,_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf,_leafParent,_boundary,_performIncrementalMeshing);


	fprintf(stderr,"!");
	double time2 = (double)cv::getTickCount();

	fprintf(stderr,"F");
	Frustum frustum(caminfo,depth.cols,depth.rows,FRUSTUM_FAR);

//	float tx = trans.at<double>(0,0);
//	float ty = trans.at<double>(2,0);
//	float tz = trans.at<double>(2,0);

	fprintf(stderr,"[%i|%i]",_nLeavesQueuedSurface,_nLeavesQueuedFrustum);
	fprintf(stderr,"!");


	double time3 = (double)cv::getTickCount();

	if(_nLeavesUsed < _nLeavesTotal && _nBranchesUsed < _nBranchesTotal &&
			(_boxMin.x<0 || _boxMin.y<0 || _boxMin.z<0 || _boxMax.x>=_n || _boxMax.y>=_n || _boxMax.z>=_n)){
		grow();
	}


	double time4;


	_threadValid = false;
	if(_threaded){
		distanceUpdateThread->join();
		delete distanceUpdateThread;
		time3 = (double)cv::getTickCount();
		time4 = (double)cv::getTickCount();
	}
	else{
		fprintf(stderr, "U");
		updateWrapperFloat(SDFUpdateParameterFloat(
				(const float*)data,(const uchar*)rgb[2].data,(const uchar*)rgb[1].data,(const uchar*)rgb[0].data,
				_imageWidth,_imageHeight,
				m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,
				pInv.fx,pInv.fy,pInv.cx,pInv.cy,_scale,_distanceThreshold,
				_leafNumberSurface,_leafPos,_leafScale,
				_distance,_weights,_color,_brickLength),&_nLeavesQueuedSurface,&_threadValid,0);

		time4 = (double)cv::getTickCount();

		fprintf(stderr,"!");
	}

	if(_nLeavesUsed < _nLeavesTotal && _nBranchesUsed < _nBranchesTotal &&
			(_boxMin.x<0 || _boxMin.y<0 || _boxMin.z<0 || _boxMax.x>=_n || _boxMax.y>=_n || _boxMax.z>=_n)){
	}


	double time5 = (double)cv::getTickCount();



	if(_nBranchesUsed > _nBranchesTotal){
		fprintf(stderr,"\nTree is out of Memory by at least %i Nodes: %i vs. %i",
				_nBranchesUsed+1-_nLeavesTotal,_nBranchesUsed,_nBranchesTotal);
	}
	if(_nLeavesUsed > _nLeavesTotal){
		fprintf(stderr,"\nTree is out of Memory by at least %i Leaves: %i vs. %i",
				_nLeavesUsed+1-_nLeavesTotal,_nLeavesUsed,_nLeavesTotal);
	}

	if(firstMap && _nLeavesUsed != _nLeavesQueuedSurface){
		fprintf(stderr,"\nNot all allocated Leaves are queued:\n");
		if(_nLeavesUsed >= _nLeavesTotal){
			fprintf(stderr,"\nThis is due to the Tree being full.");
		}
	}

	if(_performIncrementalMeshing){
		pushMeshCellQueue();
	}


#ifdef CRAPPY_LAPPY
	fprintf(stderr, "M");
	updateMeshes();
	fprintf(stderr,"!");
#endif


	_avgTimeQueueSurface += time2-time1;
	_avgTimeQueueFrustum += time3-time2;
	_avgTimeBricksSurface += time4-time3;
	_avgTimeBricksFrustum += time5-time4;
	_sumTimeOfAllFrames += time5-time1;
	_framesAdded++;

	if(_loggingEnabled){
		_frameStatistics.push_back(FrameStatistic());
		_frameStatistics.back().leavesQueued = _nLeavesQueuedSurface;
		_frameStatistics.back().newBranches = _nBranchesUsed-branchesBeforeLastFrame;
		_frameStatistics.back().newLeaves = _nLeavesUsed-_nLeavesBeforeLastFrame;
		_frameStatistics.back().newMeshCells = _meshCells.size()-meshCellsBeforeLastFrame;
		_frameStatistics.back().timeSDFUpdate = time4-time3;
		_frameStatistics.back().timeTraversal = time2-time1;
	}

	_averageLeaves += _nLeavesQueuedSurface;

	size_t neighborhoodsize = 0;
	for(size_t i=0;i<_meshCellIndicesBranch.size();i++){
#ifdef BRANCHNEIGHBORHOOD_REFERECE
		if(_meshCellIndicesBranch[i]) neighborhoodsize += _meshCellIndicesBranch[i]->size();
#else
		neighborhoodsize += _meshCellIndicesBranch[i].size();
#endif
	}

	fprintf(stderr,"\nMesh Cell Indices Branch use %li Bytes",neighborhoodsize);
	fprintf(stderr,"\nSize of a Mesh Cell: %li = %li + %li + 4*%li + 16*%li",sizeof(MeshCell),
			sizeof(int),sizeof(MeshSeparate*),sizeof(sidetype),sizeof(volumetype));
	fprintf(stderr,"\nSize of all Mesh Cells: %li",_meshCells.capacity()*sizeof(MeshCell));
	fprintf(stderr,"\nSize of a size_t-Vector: %li",sizeof(std::vector<size_t>));
	size_t leafassociationsize = 0;
	for(size_t i=0;i<_meshCellIndicesLeaf.size();i++){
		leafassociationsize +=
				_meshCellIndicesLeaf[i].size()
//				_meshCellIndicesLeaf[i].capacity()
				*sizeof(size_t)+sizeof(std::vector<size_t>);
	}
	for(size_t i=_meshCellIndicesLeaf.size();i<_meshCellIndicesLeaf.capacity();i++){
		leafassociationsize += sizeof(std::vector<size_t>);
	}
	fprintf(stderr,"\nMesh Cell Indices Leaf use %li Bytes",leafassociationsize);

	return _nLeavesQueuedSurface;

}


int FusionMipMapCPU::addMap(const cv::Mat &depth, CameraInfo caminfo, const cv::Mat &rgb,
		float scaling, float maxcamdistance)
{
	fprintf(stderr,"\nI[%i]",_framesAdded);
	//Parameter Helpers
	cv::Mat rot = caminfo.getRotation();
	cv::Mat trans = caminfo.getTranslation();
	cv::Mat intr = caminfo.getIntrinsic();

	_nLeavesBeforeLastFrame = _nLeavesUsed;
	volumetype branchesBeforeLastFrame = _nBranchesUsed;
	size_t meshCellsBeforeLastFrame = _meshCells.size();


	camPamsFloat p(
			rot.at<double>(0,0)/_scale, rot.at<double>(0,1)/_scale, rot.at<double>(0,2)/_scale,
			rot.at<double>(1,0)/_scale, rot.at<double>(1,1)/_scale, rot.at<double>(1,2)/_scale,
			rot.at<double>(2,0)/_scale, rot.at<double>(2,1)/_scale, rot.at<double>(2,2)/_scale,
			(trans.at<double>(0,0) - _offset.x)/_scale,
			(trans.at<double>(1,0) - _offset.y)/_scale,
			(trans.at<double>(2,0) - _offset.z)/_scale,
			intr.at<double>(0,0), intr.at<double>(1,1),
			intr.at<double>(0,2), intr.at<double>(1,2));

	const ushort *depthdata = (ushort*)depth.data;

//	if(_numCheckImages){
//		_lastDepthImages.push_front(depth);
//		_lastCameraInfos.push_front(caminfo);
//		if(_lastDepthImages.size()>_numCheckImages+1){
//			_lastDepthImages.pop_back();
//			_lastCameraInfos.pop_back();
//		}
////		ushort *depthdata2 = (ushort*)_lastDepthImages.back().data;
//		ushort *depthdata2 = (ushort*)depth.data;
//		int nx = _lastDepthImages.back().cols;
//		int ny = _lastDepthImages.back().rows;
//
////		cv::Mat oldImage = depth.clone();
////		cv::imshow("Old",oldImage);
//
//		std::list<cv::Mat>::iterator itImage = _lastDepthImages.begin(); itImage++;
//		std::list<CameraInfo>::iterator itCaminfo = _lastCameraInfos.begin(); itCaminfo++;
//		while(itImage!=_lastDepthImages.end() && itCaminfo!=_lastCameraInfos.end()){
//
//			ushort *checkdata = (ushort*)itImage->data;
//			int nxCheck = itImage->cols;
//			int nyCheck = itImage->rows;
//			cv::Mat intCheck = itCaminfo->getIntrinsic();
//			float fxCheck = intCheck.at<double>(0,0);
//			float fyCheck = intCheck.at<double>(1,1);
//			float cxCheck = intCheck.at<double>(0,2);
//			float cyCheck = intCheck.at<double>(1,2);
//
//			CameraInfo relative = caminfo;
//			relative.setExtrinsic(itCaminfo->getExtrinsicInverse()*relative.getExtrinsic());
//
//			cv::Mat rotCheck = relative.getRotation();
//			cv::Mat transCheck = relative.getTranslation();
//			cv::Mat intrCheck = relative.getIntrinsic();
//
//			camPamsFloat pCheck(
//					rotCheck.at<double>(0,0), rotCheck.at<double>(0,1), rotCheck.at<double>(0,2),
//					rotCheck.at<double>(1,0), rotCheck.at<double>(1,1), rotCheck.at<double>(1,2),
//					rotCheck.at<double>(2,0), rotCheck.at<double>(2,1), rotCheck.at<double>(2,2),
//					transCheck.at<double>(0,0),transCheck.at<double>(1,0),transCheck.at<double>(2,0),
//					intrCheck.at<double>(0,0), intrCheck.at<double>(1,1),
//					intrCheck.at<double>(0,2), intrCheck.at<double>(1,2));
//
//
//			for(int y=0;y<ny;y++){
//				for(int x=0;x<nx;x++){
//					float h = depthdata2[y*nx+x]*scaling;
//					if(h>0.0f && h<maxcamdistance){
//						float px = (x-pCheck.cx)/pCheck.fx*h;
//						float py = (y-pCheck.cy)/pCheck.fy*h;
//						float pz = h;
//						float qx = pCheck.r11*px + pCheck.r12*py + pCheck.r13*pz + pCheck.t1;
//						float qy = pCheck.r21*px + pCheck.r22*py + pCheck.r23*pz + pCheck.t2;
//						float qz = pCheck.r31*px + pCheck.r32*py + pCheck.r33*pz + pCheck.t3;
//
//						int imx = (int)(floor(qx/qz*fxCheck+cxCheck));
//						int imy = (int)(floor(qy/qz*fyCheck+cyCheck));
//
//						if(imx>=0 && imx<nxCheck && imy>=0 && imy<nyCheck){
//							float h2 = checkdata[imy*nxCheck+imx]*scaling;
//							if(h2<maxcamdistance && h2>qz){
//								depthdata2[y*nx+x] = 0;
//							}
//						}
//					}
////					depthdata2[y*nx+x] = 0;
//				}
//			}
//
////			cv::imshow("New",depth);
//
//			itImage++; itCaminfo++;
//		}
////		cv::waitKey(0);
//	}


	//Empty the Data Queue
	//TODO: Instead O(n) operation go through the queue
	for(volumetype i=0;i<_nLeavesTotal;i++) _queueIndexOfLeaf[i] = MAXLEAFNUMBER;
	_nLeavesQueuedSurface = 0;
	_nLeavesQueuedFrustum = 0;

	bool firstMap = _nLeavesUsed == 0;

	if(!_n){
		_boxMin.x = _boxMin.y = _boxMin.z = 1e6;
		_boxMax.x = _boxMax.y = _boxMax.z = -1e6;
		fprintf(stderr,"\nComputing Initial Bounding Box");
		computeBoundingboxIntCPU(p,depthdata,scaling,maxcamdistance,
				depth.cols,depth.rows,_bandwidth,
				&_boxMin.x,&_boxMin.y,&_boxMin.z,&_boxMax.x,&_boxMax.y,&_boxMax.z);

		if(setInitialVolume(_boxMin.x,_boxMin.y,_boxMin.z,_boxMax.x,_boxMax.y,_boxMax.z)){
			fprintf(stderr,"\nInteger Bounding Box of first frame: [%i %i %i]-[%i %i %i]",
					_boxMin.x,_boxMin.y,_boxMin.z,_boxMax.x,_boxMax.y,_boxMax.z);
		}
		else{
			fprintf(stderr,"\nERROR: Initial Bounding Box Computation failed!");
			fprintf(stderr,"\nErroneous Bounding Box of first frame: [%i %i %i]-[%i %i %i]",
					_boxMin.x,_boxMin.y,_boxMin.z,_boxMax.x,_boxMax.y,_boxMax.z);
		}
		p = 	camPamsFloat(
				rot.at<double>(0,0)/_scale, rot.at<double>(0,1)/_scale, rot.at<double>(0,2)/_scale,
				rot.at<double>(1,0)/_scale, rot.at<double>(1,1)/_scale, rot.at<double>(1,2)/_scale,
				rot.at<double>(2,0)/_scale, rot.at<double>(2,1)/_scale, rot.at<double>(2,2)/_scale,
				(trans.at<double>(0,0) - _offset.x)/_scale,
				(trans.at<double>(1,0) - _offset.y)/_scale,
				(trans.at<double>(2,0) - _offset.z)/_scale,
				intr.at<double>(0,0), intr.at<double>(1,1),
				intr.at<double>(0,2), intr.at<double>(1,2));
	}


	double time1 = (double)cv::getTickCount();

	_differentImageSize |= _imageWidth!=depth.cols || _imageHeight!=depth.rows;
	if(_imageWidth!=depth.cols || _imageHeight!=depth.rows){
		_imageWidth = depth.cols; _imageHeight = depth.rows;
		if(_sharedBoxes) delete [] _sharedBoxes;
		_sharedBoxes = new sidetype3[_imageWidth*_imageHeight*2];
		if(_pxp) delete _pxp; if(_pyp) delete _pyp;
	  int alignResult = posix_memalign((void**)&_pxp,16,_imageWidth*sizeof(float));
	  alignResult += posix_memalign((void**)&_pyp,16,_imageHeight*sizeof(float));
	  if(alignResult) fprintf(stderr,"\nERROR: Aligned Alloc in addMap failed!");
		for(int x=0;x<_imageWidth;x++){
			_pxp[x] = ((float)x-p.cx)/p.fx;
		}
		for(int y=0;y<_imageHeight;y++){
			_pyp[y] = ((float)y-p.cy)/p.fy;
		}
		_newBudsSinceMeshingToQueue.subtreeBuds->resize(_imageWidth*_imageHeight);
		_newBudsSinceMeshingToQueue.subtreeBudsParentLeaf->resize(_imageWidth*_imageHeight);
		_newBudsSinceMeshingToQueue.leafBuds->resize(_imageWidth*_imageHeight);
	  if(alignResult) fprintf(stderr,"\nERROR: Aligned Alloc in addMap failed!");
	}

	_boxMin.x = _boxMin.y = _boxMin.z = 0;
	_boxMax.x = _boxMax.y = _boxMax.z = _n-1;


	_threadValid = true;
	CameraInfo caminfo2 = caminfo;
	caminfo2.setExtrinsic(caminfo2.getExtrinsicInverse());
	cv::Mat rot2 = caminfo2.getRotation();
	cv::Mat trans2 = caminfo2.getTranslation();
	cv::Mat intr2 = caminfo2.getIntrinsic();
	camPamsFloat pInv(
	rot2.at<double>(0,0), rot2.at<double>(0,1), rot2.at<double>(0,2),
	rot2.at<double>(1,0), rot2.at<double>(1,1), rot2.at<double>(1,2),
	rot2.at<double>(2,0), rot2.at<double>(2,1), rot2.at<double>(2,2),
	rot2.at<double>(0,0)*_offset.x + rot2.at<double>(0,1)*_offset.y + rot2.at<double>(0,2)*_offset.z + trans2.at<double>(0,0),
	rot2.at<double>(1,0)*_offset.x + rot2.at<double>(1,1)*_offset.y + rot2.at<double>(1,2)*_offset.z + trans2.at<double>(1,0),
	rot2.at<double>(2,0)*_offset.x + rot2.at<double>(2,1)*_offset.y + rot2.at<double>(2,2)*_offset.z + trans2.at<double>(2,0),
	intr2.at<double>(0,0), intr2.at<double>(1,1),
	intr2.at<double>(0,2), intr2.at<double>(1,2));

//	bool colorMap2 = _useColor && (rgb.cols==depth.cols&&rgb.rows==depth.rows);

	float m11 = pInv.r11; float m12 = pInv.r12;
	float m13 = pInv.r13; float m14 = pInv.t1 ;
	float m21 = pInv.r21; float m22 = pInv.r22;
	float m23 = pInv.r23; float m24 = pInv.t2 ;
	float m31 = pInv.r31; float m32 = pInv.r32;
	float m33 = pInv.r33; float m34 = pInv.t3;

	boost::thread *distanceUpdateThread = NULL;

	if(_threaded){
	distanceUpdateThread = new boost::thread(	updateWrapperInteger,SDFUpdateParameterInteger(
			(const ushort*)depthdata, scaling, maxcamdistance, (const uchar*)rgb.data,
			_imageWidth,_imageHeight,
			m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,
			pInv.fx,pInv.fy,pInv.cx,pInv.cy,_scale,_distanceThreshold,
			_leafNumberSurface,_leafPos,_leafScale,
			_distance,_weights,_color,_brickLength),&_nLeavesQueuedSurface,&_threadValid,0);
	}

	__attribute__ ((aligned (16))) float qxp1[_imageWidth];
	__attribute__ ((aligned (16))) float qxp2[_imageWidth];
	__attribute__ ((aligned (16))) float qxp3[_imageWidth];
	for(int x=0;x<_imageWidth;x++){
		qxp1[x] = p.r11*_pxp[x];
		qxp2[x] = p.r21*_pxp[x];
		qxp3[x] = p.r31*_pxp[x];
	}
	__attribute__ ((aligned (16))) float qyp1[_imageHeight];
	__attribute__ ((aligned (16))) float qyp2[_imageHeight];
	__attribute__ ((aligned (16))) float qyp3[_imageHeight];
	for(int y=0;y<_imageHeight;y++){
		qyp1[y] = p.r12*_pyp[y] + p.r13;
		qyp2[y] = p.r22*_pyp[y] + p.r23;
		qyp3[y] = p.r32*_pyp[y] + p.r33;
	}

//	fprintf(stderr, "T");

#ifndef SEPARATE_MESHCELL_STRUCTURE
	transformLoopSimPrecalculatedNeg_vis(qxp1,qxp2,qxp3,qyp1,qyp2,qyp3,p.t1,p.t2,p.t3,
			_n,_bandwidth,_brickLength,_imageWidth,_imageHeight,_boxMin,_boxMax,
			depthdata,scaling,maxcamdistance,
			_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueuedSurface,
			_leafNumberSurface,_leafPos,_leafScale,_queueIndexOfLeaf,_child,_branchNumber
			,_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf,_leafParent,_boundary,_performIncrementalMeshing);

#else
	transformLoopSimPrecalculatedNeg_subtree(qxp1,qxp2,qxp3,qyp1,qyp2,qyp3,p.t1,p.t2,p.t3,
			_n,_bandwidth,_brickLength,_imageWidth,_imageHeight,_boxMin,_boxMax,
			depthdata,scaling,maxcamdistance,
			_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueuedSurface,
			_leafNumberSurface,_leafPos,_leafScale,_queueIndexOfLeaf,_child,_branchNumber,_leafParent,
			_newBudsSinceMeshingToQueue.subtreeBuds->data(),
			_newBudsSinceMeshingToQueue.subtreeBudsParentLeaf->data(),
			_newBudsSinceMeshingToQueue.leafBuds->data(),
			_numberOfQueuedTreeBuds,_numberOfQueuedLeafBuds,_treeSizeSinceMeshing);
	eprintf("\n%i new Subtrees and %i new Leaves this Map",
			_numberOfQueuedTreeBuds,_numberOfQueuedLeafBuds);
	for(size_t i=0;i<_numberOfQueuedTreeBuds;i++){
		_newBudsSinceMeshingToAccumulate.subtreeBuds->push_back((*_newBudsSinceMeshingToQueue.subtreeBuds)[i]);
		_newBudsSinceMeshingToAccumulate.subtreeBudsParentLeaf->push_back((*_newBudsSinceMeshingToQueue.subtreeBudsParentLeaf)[i]);
	}
	for(size_t i=0;i<_numberOfQueuedLeafBuds;i++){
		_newBudsSinceMeshingToAccumulate.leafBuds->push_back((*_newBudsSinceMeshingToQueue.leafBuds)[i]);
	}
	_numberOfQueuedTreeBuds = _numberOfQueuedLeafBuds = 0;

#endif


//	fprintf(stderr,"!");
	double time2 = (double)cv::getTickCount();
//
//	fprintf(stderr,"F");
//	Frustum frustum(caminfo,depth.cols,depth.rows,FRUSTUM_FAR);
//
//	float tx = trans.at<double>(0,0);
//	float ty = trans.at<double>(2,0);
//	float tz = trans.at<double>(2,0);
//
//	fprintf(stderr,"[%i|%i]",_nLeavesQueuedSurface,_nLeavesQueuedFrustum);
//	fprintf(stderr,"!");
//
//
	double time3 = (double)cv::getTickCount();

	if(_nLeavesUsed < _nLeavesTotal && _nBranchesUsed < _nBranchesTotal &&
			(_boxMin.x<0 || _boxMin.y<0 || _boxMin.z<0 || _boxMax.x>=_n || _boxMax.y>=_n || _boxMax.z>=_n)){
		grow();
	}


	double time4;


	_threadValid = false;
	if(_threaded){
		distanceUpdateThread->join();
		delete distanceUpdateThread;
		time3 = (double)cv::getTickCount();
		time4 = (double)cv::getTickCount();
	}
	else{
//		fprintf(stderr, "U");
		updateWrapperInteger(SDFUpdateParameterInteger(
				(const ushort*)depthdata, scaling, maxcamdistance, (const uchar*)rgb.data,
				_imageWidth,_imageHeight,
				m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,
				pInv.fx,pInv.fy,pInv.cx,pInv.cy,_scale,_distanceThreshold,
				_leafNumberSurface,_leafPos,_leafScale,
				_distance,_weights,_color,_brickLength),&_nLeavesQueuedSurface,&_threadValid,0);

		time4 = (double)cv::getTickCount();

//		fprintf(stderr,"!");
	}

	if(_nLeavesUsed < _nLeavesTotal && _nBranchesUsed < _nBranchesTotal &&
			(_boxMin.x<0 || _boxMin.y<0 || _boxMin.z<0 || _boxMax.x>=_n || _boxMax.y>=_n || _boxMax.z>=_n)){
	}


	double time5 = (double)cv::getTickCount();



	if(_nBranchesUsed > _nBranchesTotal){
		_performIncrementalMeshing = false;
		fprintf(stderr,"\nTree is out of Memory by at least %i Branches: %i vs. %i",
				_nBranchesUsed+1-_nBranchesTotal,_nBranchesUsed,_nBranchesTotal);
	}
	if(_nLeavesUsed > _nLeavesTotal){
		_performIncrementalMeshing = false;
		fprintf(stderr,"\nTree is out of Memory by at least %i Leaves: %i vs. %i",
				_nLeavesUsed+1-_nLeavesTotal,_nLeavesUsed,_nLeavesTotal);
	}

	if(firstMap && _nLeavesUsed != _nLeavesQueuedSurface){
		fprintf(stderr,"\nNot all allocated Leaves are queued:\n");
		if(_nLeavesUsed >= _nLeavesTotal){
			fprintf(stderr,"\nThis is due to the Tree being full.");
		}
	}


#ifndef DEBUG_NO_LEAFCELLS
	if(_performIncrementalMeshing){

#ifdef SEPARATE_MESHCELL_STRUCTURE
		//TODO: Das hier in den Nachbarthread migrieren
//		if(_newBudsSinceMeshingToClear.subtreeBuds->size() ||
//				_newBudsSinceMeshingToClear.subtreeBudsParentLeaf->size() ||
//				_newBudsSinceMeshingToClear.leafBuds->size()){
//			fprintf(stderr,"\nERROR: Vector of Branches queued for MeshCell Structure"
//					" Creation not yet empty: %li %li %li",
//					_newBudsSinceMeshingToClear.subtreeBuds->size(),
//					_newBudsSinceMeshingToClear.subtreeBudsParentLeaf->size(),
//					_newBudsSinceMeshingToClear.leafBuds->size());
//		}
//		BudsAnchor temp = _newBudsSinceMeshingToClear;
//		_newBudsSinceMeshingToClear = _newBudsSinceMeshingToAccumulate;
//		_newBudsSinceMeshingToAccumulate = temp;
//		_treeSizeForMeshing = _nBranchesUsed;

		beforeUpdateMeshCellStructure();

		updateMeshCellStructure();

		afterUpdateMeshCellStructure();

//		_treeSizeSinceMeshing = _treeSizeForMeshing;
//
//		pushLeafQueueForMeshing();

#else

		eprintf("\nPushing Mesh Cell queue");
		pushMeshCellQueue();
		eprintf("\nMesh Cell queue pushed");
#endif


	}
#endif


	_avgTimeQueueSurface += time2-time1;
	_avgTimeQueueFrustum += time3-time2;
	_avgTimeBricksSurface += time4-time3;
	_avgTimeBricksFrustum += time5-time4;
	_sumTimeOfAllFrames += time5-time1;
	_framesAdded++;

	if(_loggingEnabled){
		_frameStatistics.push_back(FrameStatistic());
		_frameStatistics.back().leavesQueued = _nLeavesQueuedSurface;
		_frameStatistics.back().newBranches = _nBranchesUsed-branchesBeforeLastFrame;
		_frameStatistics.back().newLeaves = _nLeavesUsed-_nLeavesBeforeLastFrame;
		_frameStatistics.back().newMeshCells = _meshCells.size()-meshCellsBeforeLastFrame;
		_frameStatistics.back().timeSDFUpdate = time4-time3;
		_frameStatistics.back().timeTraversal = time2-time1;
	}

	_averageLeaves += _nLeavesQueuedSurface;

	if((_framesAdded-1)%25==0){

		size_t meshIndicesBranchSize = 0;
		size_t meshIndicesBranchEmptySize = 0;
		for(size_t i=0;i<_meshCellIndicesBranch.size();i++){
#ifdef BRANCHNEIGHBORHOOD_REFERECE
		if(_meshCellIndicesBranch[i]) meshIndicesBranchSize += _meshCellIndicesBranch[i]->size();
#else
			meshIndicesBranchSize += _meshCellIndicesBranch[i].size();
#endif
			meshIndicesBranchEmptySize += sizeof(_meshCellIndicesBranch[i]);
		}

		eprintf("\nMesh Cell Indices Branch use %li Bytes, and %li Bytes empty\n",
				meshIndicesBranchSize,meshIndicesBranchEmptySize);
		eprintf("\nSize of a Mesh Cell: %li = %li + %li + 4*%li + 16*%li",sizeof(MeshCell),
				sizeof(int),sizeof(MeshInterleaved*),sizeof(sidetype),sizeof(volumetype));
		eprintf("\nSize of all Mesh Cells: %li * %li = %li",_meshCells.capacity(),sizeof(MeshCell),_meshCells.capacity()*sizeof(MeshCell));

		eprintf("\nEmpty Size of a Mesh: %li, empty Size of all Meshes %li * %li = %li",
				sizeof(MeshInterleaved),_meshCells.size(),sizeof(MeshInterleaved),_meshCells.size()*sizeof(MeshInterleaved));
		eprintf("\nMesh Cell Indices Leaf use %li Bytes",_meshCellIndicesLeaf.capacity()*sizeof(LeafNeighborhood));

		size_t verticesSize = 0;
		size_t facesSize = 0;
		size_t colorsSize = 0;
		for(size_t i=0;i<_meshCells.size();i++){
			verticesSize += _meshCells[i].meshinterleaved->vertices.size();
			facesSize += _meshCells[i].meshinterleaved->faces.size();
			colorsSize += _meshCells[i].meshinterleaved->colors.size();
		}
		verticesSize *= sizeof(Vertex3f);
		facesSize *= sizeof(unsigned int);
		colorsSize *= sizeof(Color3b);
		eprintf("\nMeshesSize: Vertices: %li , Faces: %li , Color: %li",
				verticesSize,facesSize,colorsSize);
	}


	return _nLeavesQueuedSurface;

}


void FusionMipMapCPU::queryNeighborsWithoutMeshCells(){

	//TEST

//	for(volumetype i=0;i<_nLeavesQueuedSurface;i++){
//		volumetype leaf = _leafNumberSurface[i];
//		sidetype3 leafpos = _leafPos[leaf];
//		sidetype leafscale = _leafScale[leaf];
//
//		for(sidetype lpx=leafpos.x-leafscale*_brickLength;lpx<=leafpos.x+leafscale*_brickLength;lpx+=leafscale*_brickLength){
//			for(sidetype lpy=leafpos.y-leafscale*_brickLength;lpy<=leafpos.y+leafscale*_brickLength;lpy+=leafscale*_brickLength){
//				for(sidetype lpz=leafpos.z-leafscale*_brickLength;lpz<=leafpos.z+leafscale*_brickLength;lpz+=leafscale*_brickLength){
//					queryPointDepthSingle_func_subtree(lpx,lpy,lpz,leafscale,
//							_n,_brickLength,_tree,_nBranchesUsed,_nLeavesTotal,_nLeavesUsed,_nLeavesQueuedSurface,
//							_leafNumberSurface,_leafPos,_leafScale,_queueIndexOfLeaf,_leafParent,
//							_newBudsSinceMeshingToQueue.subtreeBuds->data(),
//							_newBudsSinceMeshingToQueue.subtreeBudsParentLeaf->data(),
//							_newBudsSinceMeshingToQueue.leafBuds->data(),
//							_numberOfQueuedTreeBuds,_numberOfQueuedLeafBuds,
//							_treeSizeSinceMeshing);
//				}
//			}
//		}
//
//
//	}
}

void FusionMipMapCPU::beforeUpdateMeshCellStructure(){

	if(_newBudsSinceMeshingToClear.subtreeBuds->size() ||
			_newBudsSinceMeshingToClear.subtreeBudsParentLeaf->size() ||
			_newBudsSinceMeshingToClear.leafBuds->size()){
		fprintf(stderr,"\nERROR: Vector of Branches queued for MeshCell Structure"
				" Creation not yet empty: %li %li %li",
				_newBudsSinceMeshingToClear.subtreeBuds->size(),
				_newBudsSinceMeshingToClear.subtreeBudsParentLeaf->size(),
				_newBudsSinceMeshingToClear.leafBuds->size());
	}
	BudsAnchor temp = _newBudsSinceMeshingToClear;
	_newBudsSinceMeshingToClear = _newBudsSinceMeshingToAccumulate;
	_newBudsSinceMeshingToAccumulate = temp;
	_treeSizeForMeshing = _nBranchesUsed;
	//FIXME: Ob das man hier richtig ist ...
	_treeSizeSinceMeshing = _treeSizeForMeshing;

}

void FusionMipMapCPU::afterUpdateMeshCellStructure(){



	pushLeafQueueForMeshing();

	eprintf("\nPushing Mesh Cell queue");
	pushMeshCellQueue();
	eprintf("\nMesh Cell queue pushed");
}



void imageSaveFunctionFloat(std::vector<cv::Mat> depthImages, std::vector<std::vector<cv::Mat> > rgbImages,size_t oldSize){
	for(size_t p=0;p<depthImages.size();p++){
		cv::Mat output;
		depthImages[p].convertTo(output,CV_16UC1,5000);
		cv::imwrite(getNameD(p+oldSize),output);
		cv::imwrite(getNameR(p+oldSize),rgbImages[p][0]);
		cv::imwrite(getNameG(p+oldSize),rgbImages[p][1]);
		cv::imwrite(getNameB(p+oldSize),rgbImages[p][2]);
	}
}

void imageSaveFunctionInteger(std::vector<cv::Mat> depthImages, std::vector<cv::Mat> rgbImages, size_t oldSize){
	for(size_t p=0;p<depthImages.size();p++){
		cv::imwrite(getNameD(p+oldSize),depthImages[p]);
		cv::imwrite(getNameRGB(p+oldSize),rgbImages[p]);
	}
}

std::vector<int> FusionMipMapCPU::addMap(std::vector<cv::Mat> depthImages, std::vector<CameraInfo> trajectory,
			std::vector<std::vector<cv::Mat> > rgbImages,
			volatile long int *poseNumber)
{
	if(trajectory.empty()) {
		fprintf(stderr,"\nERROR: Empty Trajectory!");
		return std::vector<int>();
	}

	if(trajectory.size()<_posesFused.size()) {
		fprintf(stderr,"\nERROR: Trajectory File is smaller than already accumulated Trajectory!");
		return std::vector<int>();
	}

	if(!_loopClosureLogging)
		fprintf(stderr,"\nWARNING: You feed me a new trajectory, but you did not enable Loop Closures!");

	size_t oldSize = _posesFused.size();

	_posesLoop = trajectory;
	_posesLoop.resize(oldSize);

	if(_loopClosureEnabled && _posesLoop.size()){
		fprintf(stderr,"\nCalling startLoopClosure()");
		startLoopClosure();
	}

	if(_loopClosureLogging){
		if(_imageSaveThread){
			double timeJoin= (double)cv::getTickCount();
			_imageSaveThread->join();
			delete _imageSaveThread;
			fprintf(stderr,"\nTime for joining Image Save Thread: %f",((double)cv::getTickCount()-timeJoin)/cv::getTickFrequency());
		}
		_imageSaveThread = new boost::thread(imageSaveFunctionFloat,depthImages,rgbImages,oldSize);
	}

	std::vector<int> result;
	fprintf(stderr,"\nSizes: Old Trajectory: %li, New Trajectory: %li, Depth Images: %li, RGB Images: %li",
			oldSize,trajectory.size(),depthImages.size(),rgbImages.size());
	for(size_t p=_posesFused.size();p<trajectory.size();p++){
		fprintf(stderr,"\nAdding Image %li",p);
		result.push_back(addMap(depthImages[p-oldSize],trajectory[p],rgbImages[p-oldSize]));
		if(poseNumber){
			fprintf(stderr,"\nTransmitting Frame...");
			*poseNumber = p;
			fprintf(stderr,"done");
		}
		_posesFused.push_back(trajectory[p]);
		if(_loopClosureLogging){
			_leafQueues.push_back(std::vector<volumetype>(_leafNumberSurface,_leafNumberSurface+_nLeavesQueuedSurface));
		}
		fprintf(stderr,"\nImage %li added",p);
	}

	return result;
}


std::vector<int> FusionMipMapCPU::addMap(std::vector<cv::Mat> depthImages, std::vector<CameraInfo> trajectory,
			std::vector<cv::Mat> rgbImages,volatile long int *poseNumber)
{
	if(trajectory.empty()) {
		fprintf(stderr,"\nERROR: Empty Trajectory!");
		return std::vector<int>();
	}

	if(trajectory.size()<_posesFused.size()) {
		fprintf(stderr,"\nERROR: Trajectory File is smaller than already accumulated Trajectory!");
		return std::vector<int>();
	}

	if(!_loopClosureLogging)
		fprintf(stderr,"\nWARNING: You feed me a new trajectory, but you did not enable Loop Closures!");

	size_t oldSize = _posesFused.size();

	_posesLoop = trajectory;
	_posesLoop.resize(oldSize);

	if(_loopClosureEnabled && _posesLoop.size()){
		fprintf(stderr,"\nCalling startLoopClosure()");
		startLoopClosure();
	}

	if(_loopClosureLogging){
		if(_imageSaveThread){
			double timeJoin= (double)cv::getTickCount();
			_imageSaveThread->join();
			delete _imageSaveThread;
			fprintf(stderr,"\nTime for joining Image Save Thread: %f",((double)cv::getTickCount()-timeJoin)/cv::getTickFrequency());
		}
		_imageSaveThread = new boost::thread(imageSaveFunctionInteger,depthImages,rgbImages,oldSize);
	}

	std::vector<int> result;
	fprintf(stderr,"\nSizes: Old Trajectory: %li, New Trajectory: %li, Depth Images: %li, RGB Images: %li",
			oldSize,trajectory.size(),depthImages.size(),rgbImages.size());
	for(size_t p=_posesFused.size();p<trajectory.size();p++){
		fprintf(stderr,"\nAdding Image %li",p);
		result.push_back(addMap(depthImages[p-oldSize],trajectory[p],rgbImages[p-oldSize]));
		if(poseNumber){
			fprintf(stderr,"\nTransmitting Frame...");
			*poseNumber = p;
			fprintf(stderr,"done");
		}
		_posesFused.push_back(trajectory[p]);
		if(_loopClosureLogging){
			_leafQueues.push_back(std::vector<volumetype>(_leafNumberSurface,_leafNumberSurface+_nLeavesQueuedSurface));
		}
		fprintf(stderr,"\nImage %li added",p);
	}

	return result;
}


inline sidetype highestPowerOf2(sidetype in){
	sidetype result = 1;
	while(result*2<=in) result *= 2;
	return result;
}



int updateMeshCellStructureBranches_recursive
(
		FusionMipMapCPU::MeshCellArray &_meshCells,
		FusionMipMapCPU::BranchIndicesArray &_meshCellIndicesBranch,
		FusionMipMapCPU::LeafIndicesArray &_meshCellIndicesLeaf,
		MCNSplit &_boundary,
		volumetype *_tree,
		volumetype child,
		volumetype pl,
		sidetype _n,
		sidetype _brickLength,
		volumetype _treeSizeForMeshing,
		const sidetype *_leafScale
)
{
//	fprintf(stderr," start[%i]",child);
	volumetype childoffset = child&7;
	volumetype branch = child-childoffset;
#ifdef BRANCHNEIGHBORHOOD_REFERECE
	size_t index = _meshCellIndicesBranch[child]->indexInterior;
#else
	size_t index = _meshCellIndicesBranch[child].indexInterior;
#endif
	if(index>=MAXCELLINDEX) fprintf(stderr,"\nERROR: Interior Index is wrong at [%i %i]: %li!",branch,childoffset,index);
	sidetype half = _meshCells[index].size;
	volumetype subtree = _tree[child+8];
	if(subtree==BRANCHINIT) fprintf(stderr,"\nERROR: Empty Subtree at MeshCell"
			" Structuree Creation!");
	uchar childBranchSize = CHILDBRANCHSIZEFUNCTION;
//	fprintf(stderr,"\nNumber of MeshCells before split branch subdivision: %li",_meshCells.size());
	createMeshCellStructuresForBranch_list_vector_polymorph(
			_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf,_boundary,
			child,subtree,pl,childBranchSize,_n);
//	fprintf(stderr,"\nNumber of MeshCells after split branch subdivision: %li",_meshCells.size());
#ifdef BRANCHNEIGHBORHOOD_REFERECE
	delete _meshCellIndicesBranch[child];
	_meshCellIndicesBranch[child] = 0;
#endif


	if(_tree[child]<BRANCHINIT){
		pl = _tree[child];
	}
	int result = 1;
	if(half>_brickLength){
		for(volumetype co=0;co<8;co++){
			volumetype childLeaf = pl;
			if(_tree[subtree+co+8]<_treeSizeForMeshing){
//				fprintf(stderr," rec %i",co);
				result +=
				updateMeshCellStructureBranches_recursive(_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf,_boundary,_tree,
						subtree+co,childLeaf,_n,_brickLength,_treeSizeForMeshing,_leafScale);
			}
		}
	}
//	fprintf(stderr," return[%i]",child);
	return result;
}

int updateMeshCellStructureBranches_recursive_compact
(
		FusionMipMapCPU::MeshCellArray &_meshCells,
		FusionMipMapCPU::BranchIndicesCompactArray &_meshCellIndicesBranch,
		FusionMipMapCPU::LeafIndicesArray &_meshCellIndicesLeaf,
		MCNCompact &_boundary,
		volumetype *_tree,
		volumetype child,
		volumetype pl,
		sidetype _n,
		sidetype _brickLength,
		volumetype _treeSizeForMeshing,
		const sidetype *_leafScale
)
{
	volumetype childoffset = child&7;
	volumetype branch = child-childoffset;
	size_t index = _meshCellIndicesBranch[child].indexInterior;
	if(index>=MAXCELLINDEX) fprintf(stderr,"\nERROR: Interior Index in MCNCompact is wrong at [%i %i]: %li!",branch,childoffset,index);
	if(index>=_meshCells.size()){fprintf(stderr,"\nERROR: Wrong Index!"); char c; std::cin >> c;}
	sidetype half = _meshCells[index].size;
	volumetype subtree = _tree[child+8];
	if(subtree==BRANCHINIT) fprintf(stderr,"\nERROR: Empty Subtree at MeshCell"
			" Structuree Creation!");
	uchar childBranchSize = CHILDBRANCHSIZEFUNCTION;
//	fprintf(stderr,"\nNumber of MeshCells before compact branch subdivision: %li",_meshCells.size());
	createMeshCellStructuresForBranch_list_vector_polymorph_compact(
			_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf,_boundary,
			child,subtree,pl,childBranchSize,_n);
//	fprintf(stderr,"\nNumber of MeshCells after compact branch subdivision: %li",_meshCells.size());


	if(_tree[child]<BRANCHINIT){
		pl = _tree[child];
	}
	int result = 1;
	if(half>_brickLength){
		for(volumetype co=0;co<8;co++){
			volumetype childLeaf = pl;
			if(_tree[subtree+co+8]<_treeSizeForMeshing){
				result +=
				updateMeshCellStructureBranches_recursive_compact(_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf,_boundary,_tree,
						subtree+co,childLeaf,_n,_brickLength,_treeSizeForMeshing,_leafScale);
			}
		}
	}
	return result;
}

int updateMeshCellStructureBranches_recursive_splitcompactdebug
(
		FusionMipMapCPU::MeshCellArray &_meshCellsSplit,
		FusionMipMapCPU::BranchIndicesArray &_meshCellIndicesBranch,
		MCNSplit &_boundarySplit,
		FusionMipMapCPU::MeshCellArray &_meshCellsCompact,
		FusionMipMapCPU::BranchIndicesCompactArray &_meshCellIndicesBranchCompact,
		FusionMipMapCPU::LeafIndicesArray &_meshCellIndicesLeaf,
		MCNCompact &_boundaryCompact,
		volumetype *_tree,
		volumetype child,
		volumetype pl,
		sidetype _n,
		sidetype _brickLength,
		volumetype _treeSizeForMeshing,
		const sidetype *_leafScale
)
{
	volumetype childoffset = child&7;
	volumetype branch = child-childoffset;
	size_t index = _meshCellIndicesBranchCompact[child].indexInterior;
	if(index>=MAXCELLINDEX) fprintf(stderr,"\nERROR: Interior Index in MCNCompact is wrong at [%i %i]: %li!",branch,childoffset,index);
	if(index>=_meshCellsCompact.size()){fprintf(stderr,"\nERROR: Wrong Index!"); char c; std::cin >> c;}
	sidetype half = _meshCellsCompact[index].size;
	volumetype subtree = _tree[child+8];
	if(subtree==BRANCHINIT) fprintf(stderr,"\nERROR: Empty Subtree at MeshCell"
			" Structuree Creation!");
	uchar childBranchSize = CHILDBRANCHSIZEFUNCTION;
	fprintf(stderr,"\nNumber of MeshCells before split branch subdivision: %li",_meshCellsSplit.size());
	createMeshCellStructuresForBranch_list_vector_polymorph(
			_meshCellsSplit,_meshCellIndicesBranch,_meshCellIndicesLeaf,_boundarySplit,
			child,subtree,pl,childBranchSize,_n);
	fprintf(stderr,"\nNumber of MeshCells after split branch subdivision: %li",_meshCellsSplit.size());
	fprintf(stderr,"\nNumber of MeshCells before compact branch subdivision: %li",_meshCellsCompact.size());
	createMeshCellStructuresForBranch_list_vector_polymorph_compact(
			_meshCellsCompact,_meshCellIndicesBranchCompact,_meshCellIndicesLeaf,_boundaryCompact,
			child,subtree,pl,childBranchSize,_n);
	fprintf(stderr,"\nNumber of MeshCells after compact branch subdivision: %li",_meshCellsCompact.size());

	if(_meshCellIndicesBranch.size() != _meshCellIndicesBranchCompact.size()){
		fprintf(stderr,"\nERROR: Initial neighborhooods have different sizes:"
				" %li vs %li",_meshCellIndicesBranch.size(),_meshCellIndicesBranchCompact.size());
		char c; std::cin >> c;
	}
//	bool equal_neighborhoods = true;
//	for(size_t j=0;j<_meshCellIndicesBranch.size();j++){
//		MeshCellNeighborhood compare = _meshCellIndicesBranchCompact[j];
//		bool equal_neighborhood = compare == _meshCellIndicesBranch[j];
//		equal_neighborhoods &= equal_neighborhood;
//		if(!equal_neighborhood){
//			fprintf(stderr,"\nERROR: Split Neighborhood %li is different from Compact",j);
//		}
//	}
//	if(equal_neighborhoods) {
//		fprintf(stderr,"\nDEBUG: Compact neighborhoods are equal to split "
//				"ones after MeshCell Structure Creation %i->%i",child,subtree);
//	}

	if(_tree[child]<BRANCHINIT){
		pl = _tree[child];
	}
	int result = 1;
	if(half>_brickLength){
		for(volumetype co=0;co<8;co++){
			volumetype childLeaf = pl;
			if(_tree[subtree+co+8]<_treeSizeForMeshing){
				result +=
						updateMeshCellStructureBranches_recursive_splitcompactdebug(
								_meshCellsSplit,_meshCellIndicesBranch,_boundarySplit,
								_meshCellsCompact,_meshCellIndicesBranchCompact,_meshCellIndicesLeaf,_boundaryCompact,_tree,
						subtree+co,childLeaf,_n,_brickLength,_treeSizeForMeshing,_leafScale);
			}
		}
	}
	return result;
}

void populate_branch_separate
(
		const volumetype *_tree, volumetype branch,
		sidetype half, sidetype _brickLength,
		volumetype leaf,
		const sidetype3 *_leafPos, const sidetype *_leafScale
		,FusionMipMapCPU::MeshCellArray &_meshCells
		,FusionMipMapCPU::BranchIndicesArray &_meshCellIndicesBranch
		,FusionMipMapCPU::LeafNeighborhood &meshCellIndicesLeafSingle
		,volumetype _treeSizeForMeshing
)
{
	for(uchar childoffset=0;childoffset<8;childoffset++){
		if(half>_brickLength && _tree[branch+8+childoffset]<_treeSizeForMeshing){
			populate_branch_separate(_tree,_tree[branch+8+childoffset],
					half>>1,_brickLength,leaf,_leafPos,_leafScale,
					_meshCells,_meshCellIndicesBranch,meshCellIndicesLeafSingle,
					_treeSizeForMeshing);
		}
		else{
			volumetype pointertochildindex = branch+childoffset;
#ifdef BRANCHNEIGHBORHOOD_REFERECE
			MCNSplit &branchCells = *(_meshCellIndicesBranch[pointertochildindex]);
#else
			MCNSplit &branchCells = _meshCellIndicesBranch[pointertochildindex];
#endif

			createMeshCellStructuresForLeaf(
					leaf,branchCells,meshCellIndicesLeafSingle,_meshCells,_leafScale);
		}
	}
}

void populate_branch_separate_compact
(
		const volumetype *_tree, volumetype branch,
		sidetype half, sidetype _brickLength,
		volumetype leaf,
		const sidetype3 *_leafPos, const sidetype *_leafScale
		,FusionMipMapCPU::MeshCellArray &_meshCells
		,FusionMipMapCPU::BranchIndicesCompactArray &_meshCellIndicesBranch
		,FusionMipMapCPU::LeafNeighborhood &meshCellIndicesLeafSingle
		,volumetype _treeSizeForMeshing
)
{
	for(uchar childoffset=0;childoffset<8;childoffset++){
		if(half>_brickLength && _tree[branch+8+childoffset]<_treeSizeForMeshing){
			populate_branch_separate_compact(_tree,_tree[branch+8+childoffset],
					half>>1,_brickLength,leaf,_leafPos,_leafScale,
					_meshCells,_meshCellIndicesBranch,meshCellIndicesLeafSingle,
					_treeSizeForMeshing);
		}
		else{
			volumetype pointertochildindex = branch+childoffset;
			MCNCompact &branchCells = _meshCellIndicesBranch[pointertochildindex];

			createMeshCellStructuresForLeaf(
					leaf,branchCells,meshCellIndicesLeafSingle,_meshCells,_leafScale);
		}
	}
}

void FusionMipMapCPU::updateMeshCellStructure()
{
	eprintf("\nUpdating MeshCell Structure...");

//	fprintf(stderr,"\nInterior Indices Regular:\n");
//	for(size_t i=0;i<_meshCellIndicesBranch.size();i++){
//		fprintf(stderr," [%li %li]",i,_meshCellIndicesBranch[i].indexInterior);
//	}
//	fprintf(stderr,"\nInterior Indices Compact:\n");
//	for(size_t i=0;i<_meshCellIndicesBranchCompact.size();i++){
//		fprintf(stderr," [%li %li]",i,_meshCellIndicesBranchCompact[i].indexInterior);
//	}
#if defined MESHCELLINDICES_SPLIT && defined MESHCELLINDICES_COMPACT
	if(_meshCellIndicesBranch.size()!=_meshCellIndicesBranchCompact.size()){
		fprintf(stderr,"\nERROR: Branch Indices Sizes do not match: %li vs %li",
				_meshCellIndicesBranch.size(),_meshCellIndicesBranchCompact.size());
	} else{
		fprintf(stderr,"\nThere are %li Branch Indices before Update",_meshCellIndicesBranch.size());
	}
	if(_meshCells.size()!=_meshCellsCompact.size()){
		fprintf(stderr,"\nERROR: _meshCells Sizes do not match: %li vs %li",
				_meshCells.size(),_meshCellsCompact.size());
	}
#endif
	_meshCellIndicesLeaf.resize(_nLeavesUsed);
#ifdef MESHCELLINDICES_SPLIT
#ifdef BRANCHNEIGHBORHOOD_REFERECE
	size_t oldSize = _meshCellIndicesBranch.size();
	_meshCellIndicesBranch.resize(_nBranchesUsed);
	for(size_t i=oldSize;i<_meshCellIndicesBranch.size();i++){
		_meshCellIndicesBranch[i] = new MCNSplit();
	}
#else
		_meshCellIndicesBranch.resize(_nBranchesUsed);
#endif
#endif
#ifdef MESHCELLINDICES_COMPACT
	_meshCellIndicesBranchCompact.resize(_nBranchesUsed);
#endif

	uchar maxLevel = 0;
	for(sidetype length=_brickLength;length<_n/2;length*=2) maxLevel++;
	for(size_t i=0;i<_newBudsSinceMeshingToClear.subtreeBuds->size();i++){
#if defined MESHCELLINDICES_SPLIT && defined MESHCELLINDICES_COMPACT
		fprintf(stderr,"\nUpdating Branch Indices from Debug Both for bud %i",(*_newBudsSinceMeshingToClear.subtreeBuds)[i]);
		updateMeshCellStructureBranches_recursive_splitcompactdebug
		(_meshCellsSplit,_meshCellIndicesBranch,_boundary,
				_meshCellsCompact,_meshCellIndicesBranchCompact,_meshCellIndicesLeaf,_boundaryCompact,_tree,
				(*_newBudsSinceMeshingToClear.subtreeBuds)[i],
				(*_newBudsSinceMeshingToClear.subtreeBudsParentLeaf)[i],
				_n,_brickLength,_treeSizeForMeshing,_leafScale);
		fprintf(stderr,"\nBranch Indices from Debug Both for bud %i updated",(*_newBudsSinceMeshingToClear.subtreeBuds)[i]);
#else
#ifdef MESHCELLINDICES_SPLIT
//		fprintf(stderr,"\nUpdating Branch Indices from Split for bud %i",(*_newBudsSinceMeshingToClear.subtreeBuds)[i]);
		updateMeshCellStructureBranches_recursive
		(_meshCellsSplit,_meshCellIndicesBranch,_meshCellIndicesLeaf,_boundary,_tree,
				(*_newBudsSinceMeshingToClear.subtreeBuds)[i],
				(*_newBudsSinceMeshingToClear.subtreeBudsParentLeaf)[i],
				_n,_brickLength,_treeSizeForMeshing,_leafScale);
//		fprintf(stderr,"\nBranch Indices from Split for bud %i updated",(*_newBudsSinceMeshingToClear.subtreeBuds)[i]);
#endif

#ifdef MESHCELLINDICES_COMPACT
		fprintf(stderr,"\nUpdating Branch Indices from Compact for bud %i",(*_newBudsSinceMeshingToClear.subtreeBuds)[i]);
		updateMeshCellStructureBranches_recursive_compact
		(_meshCellsCompact,_meshCellIndicesBranchCompact,_meshCellIndicesLeaf,_boundaryCompact,_tree,
				(*_newBudsSinceMeshingToClear.subtreeBuds)[i],
				(*_newBudsSinceMeshingToClear.subtreeBudsParentLeaf)[i],
				_n,_brickLength,_treeSizeForMeshing,_leafScale);
		fprintf(stderr,"\nBranch Indices from Compact for bud %i updated",(*_newBudsSinceMeshingToClear.subtreeBuds)[i]);
#endif

#endif

	}





	_newBudsSinceMeshingToClear.subtreeBuds->clear();
	_newBudsSinceMeshingToClear.subtreeBudsParentLeaf->clear();

	for(size_t i=0;i<_newBudsSinceMeshingToClear.leafBuds->size();i++){
		volumetype pointertochildindex = (*_newBudsSinceMeshingToClear.leafBuds)[i];
		volumetype leaf = _tree[pointertochildindex];
		sidetype half = _leafScale[leaf]*_brickLength;
		FusionMipMapCPU::LeafNeighborhood &meshCellIndicesLeafSingle = _meshCellIndicesLeaf[leaf];
		if(half > _brickLength && _tree[pointertochildindex+8]<BRANCHINIT){
#ifdef MESHCELLINDICES_SPLIT
//			fprintf(stderr,"\nPopulating Branch with Leaves split");
			populate_branch_separate(_tree,_tree[pointertochildindex+8],
					half>>1,_brickLength,
					leaf,_leafPos, _leafScale,_meshCellsSplit,
					_meshCellIndicesBranch,meshCellIndicesLeafSingle,
					_treeSizeForMeshing);
//			fprintf(stderr,"\nBranch with Leaves populated split");
#endif
#ifdef MESHCELLINDICES_COMPACT
//			fprintf(stderr,"\nPopulating Branch with Leaves compact");
			populate_branch_separate_compact(_tree,_tree[pointertochildindex+8],
					half>>1,_brickLength,
					leaf,_leafPos, _leafScale,_meshCellsCompact,
					_meshCellIndicesBranchCompact,meshCellIndicesLeafSingle,
					_treeSizeForMeshing);
//			fprintf(stderr,"\nBranch with Leaves populated compact");
#endif
		}
		else{
#ifdef MESHCELLINDICES_SPLIT
#ifdef BRANCHNEIGHBORHOOD_REFERECE
			createMeshCellStructuresForLeaf(leaf,*(_meshCellIndicesBranch[pointertochildindex]),
					meshCellIndicesLeafSingle,_meshCellsSplit,_leafScale);
#else
			createMeshCellStructuresForLeaf(leaf,_meshCellIndicesBranch[pointertochildindex],
					meshCellIndicesLeafSingle,_meshCellsSplit,_leafScale);
#endif
#endif
#if defined  MESHCELLINDICES_COMPACT
			createMeshCellStructuresForLeaf(leaf,_meshCellIndicesBranchCompact[pointertochildindex],
					meshCellIndicesLeafSingle,_meshCellsCompact,_leafScale);
#endif
		}
	}

	_newBudsSinceMeshingToClear.leafBuds->clear();
	eprintf("\nMeshCell Structure updated");
}

void FusionMipMapCPU::pushLeafQueueForMeshing()
{
	eprintf("\nPushing new leaves in the queue for meshing");
	for(volumetype i=0;i<_nLeavesQueuedSurface;i++){
		volumetype leaf = _leafNumberSurface[i];
		if(!_leafIsQueuedForMeshing[leaf]){
			_leafIsQueuedForMeshing[leaf] = true;
			_leafQueueForMeshing.push_back(leaf);
		}
	}
	eprintf("\nLeaves pushed in meshing queue.");
}

void FusionMipMapCPU::pushMeshCellQueue()
{


	_meshCellIsQueuedNext->resize(_meshCells.size(),false);

	for(std::deque<volumetype>::iterator it=_leafQueueForMeshing.begin();it!=_leafQueueForMeshing.end();it++){
		volumetype leaf = *it;
//	for(volumetype i=0;i<_nLeavesQueuedSurface;i++){
//		volumetype leaf = _leafNumberSurface[i];

		for(LeafNeighborhood::iterator j=_meshCellIndicesLeaf[leaf].begin();j!=_meshCellIndicesLeaf[leaf].end();j++){
			if((*j)>=_meshCells.size()) fprintf(stderr,"\nERROR: Leaf Queue Index %li >= _meshCells.size() of %li",
					*j,_meshCells.size());
			if(!(*_meshCellIsQueuedNext)[*j]){
				if(*j >= MAXCELLINDEX){
					fprintf(stderr,"\nERROR: Wrong Index at Leaf %i: ",leaf);
					for(LeafNeighborhood::iterator k=_meshCellIndicesLeaf[leaf].begin();k!=_meshCellIndicesLeaf[leaf].end();k++){
						fprintf(stderr," %li",*k);
					}
				}
				MeshCell &cell = _meshCells[*j];
				if(cell.type==0
						|| ((cell.type==1 || cell.type==2 || cell.type==3) &&
					 	(cell.lastLeaf[0]<BRANCHINIT && cell.lastLeaf[1]<BRANCHINIT))
					 	||((cell.type==4 || cell.type==5 || cell.type==6) &&
					 	(cell.lastLeaf[0]<BRANCHINIT && cell.lastLeaf[1]<BRANCHINIT &&
						 cell.lastLeaf[2]<BRANCHINIT && cell.lastLeaf[3]<BRANCHINIT))
						 ||((cell.type==7) &&
						(cell.lastLeaf[0]<BRANCHINIT && cell.lastLeaf[1]<BRANCHINIT &&
						 cell.lastLeaf[2]<BRANCHINIT && cell.lastLeaf[3]<BRANCHINIT &&
						 cell.lastLeaf[4]<BRANCHINIT && cell.lastLeaf[5]<BRANCHINIT &&
						 cell.lastLeaf[6]<BRANCHINIT && cell.lastLeaf[7]<BRANCHINIT))
						 ){
					_meshCellQueueNext.push_back(*j);
					(*_meshCellIsQueuedNext)[*j] = true;
				}
			}
		}

		_leafIsQueuedForMeshing[leaf] = false;
	}

	_leafQueueForMeshing.clear();

	size_t interiorFullCells = 0;
	size_t interiorCells = 0;
	size_t wallFullCells = 0;
	size_t wallCells = 0;
	size_t edgeFullCells = 0;
	size_t edgeCells = 0;
	size_t cornerFullCells = 0;
	size_t cornerCells = 0;

	for(std::list<size_t>::iterator i=_meshCellQueueNext.begin();i!=_meshCellQueueNext.end();i++){
		MeshCell &cell = _meshCells[*i];

		if(cell.type==0){
			if(cell.size==_leafScale[cell.lastLeaf[0]]*_brickLength){
				interiorFullCells++;
			}
			interiorCells++;
		}
		if(cell.type==1 || cell.type==2 || cell.type==3){
			if(cell.size==_leafScale[cell.lastLeaf[0]]*_brickLength &&
				 cell.size==_leafScale[cell.lastLeaf[1]]*_brickLength){
				wallFullCells++;
			}
			wallCells++;
		}
		if(cell.type==4 || cell.type==5 || cell.type==6){
			if(cell.size==_leafScale[cell.lastLeaf[0]]*_brickLength &&
				 cell.size==_leafScale[cell.lastLeaf[1]]*_brickLength &&
				 cell.size==_leafScale[cell.lastLeaf[2]]*_brickLength &&
				 cell.size==_leafScale[cell.lastLeaf[3]]*_brickLength){
				edgeFullCells++;
			}
			edgeCells++;
		}
		if(cell.type==7){
			if(cell.size==_leafScale[cell.lastLeaf[0]]*_brickLength &&
				 cell.size==_leafScale[cell.lastLeaf[1]]*_brickLength &&
				 cell.size==_leafScale[cell.lastLeaf[2]]*_brickLength &&
				 cell.size==_leafScale[cell.lastLeaf[3]]*_brickLength &&
				 cell.size==_leafScale[cell.lastLeaf[4]]*_brickLength &&
				 cell.size==_leafScale[cell.lastLeaf[5]]*_brickLength &&
				 cell.size==_leafScale[cell.lastLeaf[6]]*_brickLength &&
				 cell.size==_leafScale[cell.lastLeaf[7]]*_brickLength){
				cornerFullCells++;
			}
			cornerCells++;
		}
	}

}


//typedef struct MeshUpdateParameter_ {
//	std::list<size_t> *meshCellQueue;
//	std::vector<bool> *meshCellIsQueued;
//	std::vector<MeshCell> *meshCells;
//	std::vector<volumetype> *leafParent;
//	MarchingCubesIndexed *mc;
//	treeinfo *info;
//} MeshUpdateParameter;

//class MeshUpdateCallback
//{
//public:
//	virtual void beginUpdate(int updatedMeshCells) = 0;
//	virtual void update(int idx, MeshCell* cell) = 0;
//	virtual void endUpdate() = 0;
//
//};
//
//class RosMeshUpdateCallback : public MeshUpdateCallback
//{
//public:
//	virtual void beginUpdate(int updatedMeshCells)
//	{
//		currentMeshUpdate.cells.resize(updatedMeshCells);
//	}
//	virtual void update(int idx, MeshCell* cell)
//	{
//		MeshCell c;
//		// copy data to message
//		currentMeshUpdate.cells.push_back();
//	}
//	virtual void endUpdate()
//	{
//		// send message
//	}
//};

//void meshWrapperSeparate
//(
//		std::list<size_t> *meshCellQueue,
//		std::vector<bool> *meshCellIsQueued,
//		std::vector<MeshCell> *meshCells,
//		ParentArray *leafParent,
//		MarchingCubesIndexed *mc,
//		treeinfo *info,
//		volatile int *meshingDone,
//		MeshSeparate *mesh,
//		std::vector<FusionMipMapCPU::MeshStatistic> *meshTimes
//)
//{
//	size_t numVertices = 0;
//	size_t numIndices = 0;
//
//	size_t oldSize = meshCellQueue->size();
//	size_t meshcellsSize = meshCells->size();
//	double timeBefore = (double)cv::getTickCount();
//
//	for(std::list<size_t>::iterator i=meshCellQueue->begin();i!=meshCellQueue->end();){
//		(*meshCells)[*i].updateMesh(*info,*leafParent,*mc);
//		numVertices += (*meshCells)[*i].mesh->x.size();
//		numIndices += (*meshCells)[*i].mesh->f.size();
//		i++;
//		meshCellQueue->pop_front();
//	}
//	double timeMiddle = (double)cv::getTickCount();
//
//	*mesh = MeshSeparate(3);
//	for(unsigned int i=0;i<meshcellsSize;i++){
//				*mesh += *((*meshCells)[i].mesh);
//	}
//	double timeAfter = (double)cv::getTickCount();
//
//	double timeUpdate = timeMiddle-timeBefore;
//	double timeSum = timeAfter-timeMiddle;
//
//	if(meshTimes) meshTimes->push_back(FusionMipMapCPU::MeshStatistic(0,oldSize,meshcellsSize,timeUpdate,timeSum));
//
//	*meshingDone = 0;
//}


MeshInterleaved fusedMesh(
		const FusionMipMapCPU::MeshCellArray &meshes,
		const FusionMipMapCPU::BranchIndicesArray &branches
		) {
std::vector<std::vector<size_t> > adjacency(meshes.size(),std::vector<size_t>());

MeshInterleaved result(3);

return result;
}

void meshWrapperInterleaved
(
		std::list<size_t> *meshCellQueue,
		std::vector<bool> *meshCellIsQueued,
		FusionMipMapCPU::MeshCellArray *meshCells,
		ParentArray *leafParent,
		MarchingCubesIndexed *mc,
		treeinfo *info,
		volatile int *meshingDone,
		MeshInterleaved *mesh,
		std::vector<FusionMipMapCPU::MeshStatistic> *meshTimes
)
{
	size_t numVerticesQueue = 0;
	size_t numFacesQueue = 0;

	size_t oldSize = meshCellQueue->size();
	size_t meshcellsSize = meshCells->size();
	double timeBefore = (double)cv::getTickCount();

	for(std::list<size_t>::iterator i=meshCellQueue->begin();i!=meshCellQueue->end();){
		if(*i>=meshCells->size()) fprintf(stderr,"\nERROR:Wrong Index in MeshCell Queue!: %li >= %li",*i,meshCells->size());
		(*meshCells)[*i].updateMesh(*info,*leafParent,*mc);
		numVerticesQueue += (*meshCells)[*i].meshinterleaved->vertices.size();
		numFacesQueue += (*meshCells)[*i].meshinterleaved->faces.size();
		i++;
		meshCellQueue->pop_front();
	}


//	(*meshCells)[11585].updateMesh(*info,*leafParent,*mc);
//	fprintf(stderr,"\nLast leaf for Mesh Cell %li is %i",11585,(*meshCells)[11585].lastLeaf[0]);
	double timeMiddle = (double)cv::getTickCount();

	*mesh = MeshInterleaved(3);

	size_t numVerticesTotal = 0;
	size_t numFacesTotal = 0;
	for(unsigned int i=0;i<meshcellsSize;i++){
		numVerticesTotal += (*meshCells)[i].meshinterleaved->vertices.size();
		numFacesTotal += (*meshCells)[i].meshinterleaved->faces.size();
	}
	mesh->vertices.reserve(numVerticesTotal);
	mesh->colors.reserve(numVerticesTotal);
	mesh->faces.reserve(numFacesTotal);

	eprintf("\nSumming up %li Mesh Cells...",meshcellsSize);
	for(size_t i=0;i<meshcellsSize;i++){
//		fprintf(stderr," %li",i);
		*mesh += *((*meshCells)[i].meshinterleaved);
	}
//	*mesh = *((*meshCells)[11585].meshinterleaved);
	double timeAfter = (double)cv::getTickCount();

	double timeUpdate = timeMiddle-timeBefore;
	double timeSum = timeAfter-timeMiddle;

	if(meshTimes) meshTimes->push_back(FusionMipMapCPU::MeshStatistic(0,oldSize,meshcellsSize,timeUpdate,timeSum));
	eprintf("\nMeshes summed up.");
	*meshingDone = 0;
}



bool FusionMipMapCPU::updateMeshes()
{

	if(_meshingDone==0){
		double time5 = (double)cv::getTickCount();
		fprintf(stderr, "U[Q:%li]",_meshCellQueueNext.size());
		_meshingDone = 1;
		_treeinfo  = treeinfo(NULL,_brickLength,_brickSize,
				std::min((double)_framesAdded,MIN_WEIGHT_FOR_SURFACE),
				_offset,_scale,&_degenerate_faces,_nBranchesUsed,_nLeavesUsed,_tree,
				_leafPos,_leafScale,_distance,_weights,_color);

		for(std::list<size_t>::iterator i=_meshCellQueueOld.begin();i!=_meshCellQueueOld.end();i++){
			(*_meshCellIsQueuedCurrent)[*i] = false;
		}

		eprintf("\nSwitching MeshCellQueue");
		_meshCellQueueCurrent = _meshCellQueueOld = _meshCellQueueNext;
		_meshCellQueueNext.clear();

		std::vector<bool> *temp1 = _meshCellIsQueuedCurrent;
		_meshCellIsQueuedCurrent = _meshCellIsQueuedNext;
		_meshCellIsQueuedNext = temp1;
		for(std::vector<bool>::iterator i=_meshCellIsQueuedNext->begin();i!=_meshCellIsQueuedNext->end();i++){
			*i = false;
		}

		eprintf("\nMeshCellQueue switched");

		_meshCellsCopy = _meshCells;
		_leafParentCopy = _leafParent;

		if(_threadMeshing){
			if(_meshThread){
				_meshThread->join();
				if(_loggingEnabled){
					if(_meshTimes.size()) _meshTimes.back().frameNumber = _meshingStartFrame;
				}
				delete _meshThread; _meshThread = NULL;
			}
//			MeshSeparate *separate = _meshSeparateCurrent; _meshSeparateCurrent = _meshSeparateNext; _meshSeparateNext = separate;
			MeshInterleaved *interleaved = _meshCurrent; _meshCurrent = _meshNext; _meshNext = interleaved;
			_meshingStartFrame = _framesAdded;
//			_meshThread = new boost::thread(meshWrapperSeparate,&_meshCellQueueCurrent,_meshCellIsQueuedCurrent,
//					&_meshCellsCopy,&_leafParentCopy,&_mc,&_treeinfo,&_meshingDone,_meshSeparateNext,&_meshTimes);
			_meshThread = new boost::thread(meshWrapperInterleaved,&_meshCellQueueCurrent,_meshCellIsQueuedCurrent,
					&_meshCellsCopy,&_leafParentCopy,&_mc,&_treeinfo,&_meshingDone,_meshNext,&_meshTimes);
		}
		else{
//			fprintf(stderr,"\nUpdating Meshes in same thread");
//			MeshSeparate *separate = _meshSeparateCurrent; _meshSeparateCurrent = _meshSeparateNext; _meshSeparateNext = separate;
			MeshInterleaved *interleaved = _meshCurrent; _meshCurrent = _meshNext; _meshNext = interleaved;
//			meshWrapperSeparate(&_meshCellQueueCurrent,_meshCellIsQueuedCurrent,&_meshCellsCopy,
//					&_leafParentCopy,&_mc,&_treeinfo,&_meshingDone,_meshSeparateNext,&_meshTimes);
			eprintf("\nCalling meshWrapperInterleaved without Threading");
			meshWrapperInterleaved(&_meshCellQueueCurrent,_meshCellIsQueuedCurrent,&_meshCellsCopy,
					&_leafParentCopy,&_mc,&_treeinfo,&_meshingDone,_meshNext,&_meshTimes);
//			separate = _meshSeparateCurrent; _meshSeparateCurrent = _meshSeparateNext; _meshSeparateNext = separate;
			interleaved = _meshCurrent; _meshCurrent = _meshNext; _meshNext = interleaved;
			double diffTime;
			double time6 = (double)cv::getTickCount();
			diffTime = time6-time5;
			_avgTimeMesh += diffTime;
		}


		return true;
	}
	return false;

}

FloatVertex::FloatVertex_(float px, float py, float pz)
: x(px), y(py), z(pz){}
ByteColor::ByteColor_(uchar pr, uchar pg, uchar pb)
: r(pr), g(pg), b(pb){}

void cellupdateWrapper
(
		std::list<size_t> *meshCellQueue,
		std::vector<bool> *meshCellIsQueued,
		FusionMipMapCPU::MeshCellArray *meshCells,
		ParentArray *leafParent,
		MarchingCubesIndexed *mc,
		treeinfo *info,
		volatile int *meshingDone,
		CellUpdate *update
)
{
	size_t numVertices = 0;
	size_t numIndices = 0;

	std::vector<InterleavedMeshCell> &cells = update->cells;
	cells.clear();

	for(std::list<size_t>::iterator i=meshCellQueue->begin();i!=meshCellQueue->end();){
		(*meshCells)[*i].updateMesh(*info,*leafParent,*mc);

		MeshInterleaved &mesh = *((*meshCells)[*i].meshinterleaved);
		cells.push_back(InterleavedMeshCell());
		InterleavedMeshCell &cell = cells.back();
		InterleavedMesh &intermesh = cell.mesh;
		cell.index = *i;
		std::vector<Vertex3f> &vertices = intermesh.vertices;
		std::vector<Color3b> &colors = intermesh.colors;
		std::vector<uint32_t> &indices = intermesh.indices;
		for(uint32_t j=0;j<mesh.vertices.size();j++){
			vertices.push_back(mesh.vertices[j]);
			colors.push_back(mesh.colors[j]);
		}
		for(uint32_t j=0;j<mesh.faces.size();j++){
			indices.push_back(mesh.faces[j]);
		}

		numVertices += (*meshCells)[*i].meshinterleaved->vertices.size();
		numIndices += (*meshCells)[*i].meshinterleaved->faces.size();
		i++;
		meshCellQueue->pop_front();
	}

}

CellUpdate &FusionMipMapCPU::getMeshCellsUpdate()
{

	_treeinfo  = treeinfo(NULL,_brickLength,_brickSize,
			std::min((double)_framesAdded,MIN_WEIGHT_FOR_SURFACE),
			_offset,_scale,&_degenerate_faces,_nBranchesUsed,_nLeavesUsed,_tree,
			_leafPos,_leafScale,_distance,_weights,_color);

	for(std::list<size_t>::iterator i=_meshCellQueueOld.begin();i!=_meshCellQueueOld.end();i++){
		(*_meshCellIsQueuedCurrent)[*i] = false;
	}

	_meshCellQueueCurrent = _meshCellQueueOld = _meshCellQueueNext;
	_meshCellQueueNext.clear();

	std::vector<bool> *temp1 = _meshCellIsQueuedCurrent;
	_meshCellIsQueuedCurrent = _meshCellIsQueuedNext;
	_meshCellIsQueuedNext = temp1;
	for(std::vector<bool>::iterator i=_meshCellIsQueuedNext->begin();i!=_meshCellIsQueuedNext->end();i++){
		*i = false;
	}

	_meshCellsCopy = _meshCells;
	_leafParentCopy = _leafParent;

	if(_threadMeshing){
		if(_updateMessageThread){
			_updateMessageThread->join();
			delete _updateMessageThread;
		}
		MeshSeparate *temp = _meshSeparateCurrent; _meshSeparateCurrent = _meshSeparateNext; _meshSeparateNext = temp;
		_updateMessageThread = new boost::thread(cellupdateWrapper,&_meshCellQueueCurrent,_meshCellIsQueuedCurrent,&_meshCellsCopy,&_leafParentCopy,&_mc,&_treeinfo,&_meshingDone,_updateNext);
	}
	else{
		cellupdateWrapper(&_meshCellQueueCurrent,_meshCellIsQueuedCurrent,
				&_meshCellsCopy,&_leafParentCopy,&_mc,&_treeinfo,&_meshingDone,_updateNext);
	}
	CellUpdate *updateTemp = _updateCurrent;
	_updateCurrent = _updateNext;
	_updateNext = updateTemp;
	return *_updateCurrent;
}

std::pair<size_t,size_t> compareMeshCellArrays(const FusionMipMapCPU::MeshCellArray &a1, const FusionMipMapCPU::MeshCellArray &a2){
	std::pair<size_t,size_t> result = std::pair<size_t,size_t>(0,0);
	for(size_t i=0;i<a1.size();i++){
		bool found = false;
		for(size_t j=0;j<a2.size();j++){
			found |= a1[i]==a2[j];
		}
		result.first += !found;
	}

	for(size_t i=0;i<a2.size();i++){
		bool found = false;
		for(size_t j=0;j<a1.size();j++){
			found |= a1[j]==a2[i];
		}
		result.second += !found;
	}

	return result;
}


bool FusionMipMapCPU::grow()
{
	fprintf(stderr,"\nGrowing Tree...");
	if(_meshThread) {
		_meshThread->join();
		delete _meshThread;
		_meshThread = NULL;
	}

	double time2 = (double)cv::getTickCount();
	double timeBegin = time2;
	fprintf(stderr,"\nBoundingBox: [%i %i %i]-[%i %i %i](%i) ... ",
			_boxMin.x,_boxMin.y,_boxMin.z,_boxMax.x,_boxMax.y,_boxMax.z,_n);
	sidetype xp = (_boxMax.x>=_n)*((_boxMax.x-_n)/_n+1);
	sidetype yp = (_boxMax.y>=_n)*((_boxMax.y-_n)/_n+1);
	sidetype zp = (_boxMax.z>=_n)*((_boxMax.z-_n)/_n+1);
	sidetype xm = (_boxMin.x<0)*((-_boxMin.x)/_n+1);
	sidetype ym = (_boxMin.y<0)*((-_boxMin.y)/_n+1);
	sidetype zm = (_boxMin.z<0)*((-_boxMin.z)/_n+1);
//	fprintf(stderr,"\nDescriptors before processing: [%i %i %i]-[%i %i %i]",xm,ym,zm,xp,yp,zp);
	sidetype cut = xp&xm;
	if(cut){
		if(xm>xp) xm = 2*highestPowerOf2(xm) + (xm&(~cut));
		else xp = 2*highestPowerOf2(xp) + (xp&(~cut));
	}
	cut = yp&ym;
	if(cut){
		if(ym>yp) ym = 2*highestPowerOf2(ym) + (ym&(~cut));
		else yp = 2*highestPowerOf2(yp) + (yp&(~cut));
	}
	cut = zp&zm;
	if(cut){
		if(zm>zp) zm = 2*highestPowerOf2(zm) + (zm&(~cut));
		else zp = 2*highestPowerOf2(zp) + (zp&(~cut));
	}
//	fprintf(stderr,"\nDescriptors after processing: [%i %i %i]-[%i %i %i]",xm,ym,zm,xp,yp,zp);
	sidetype maxAddLevel = highestPowerOf2(xm);
	if(maxAddLevel < highestPowerOf2(xp)) maxAddLevel = highestPowerOf2(xp);
	if(maxAddLevel < highestPowerOf2(ym)) maxAddLevel = highestPowerOf2(ym);
	if(maxAddLevel < highestPowerOf2(yp)) maxAddLevel = highestPowerOf2(yp);
	if(maxAddLevel < highestPowerOf2(zm)) maxAddLevel = highestPowerOf2(zm);
	if(maxAddLevel < highestPowerOf2(zp)) maxAddLevel = highestPowerOf2(zp);


	double time1 = (double)cv::getTickCount();
	fprintf(stderr," %f ms",(time1-time2)/cv::getTickFrequency()*1000.0f);

	fprintf(stderr,"\nGrowing the Tree for %i Levels",maxAddLevel);
	if(maxAddLevel*16>_nBranchesTotal-_nBranchesUsed){
		fprintf(stderr,"\nThere are more Levels to add than the Tree can carry."
				" Therefore the Tree will split");
		split();
		if(maxAddLevel*16>_nBranchesTotal-_nBranchesUsed){
			fprintf(stderr,"\nERROR: Tree is still too small! This should not happen!");
			return false;
		}
	}
	sidetype xadd = 0, yadd = 0, zadd = 0;


	//Shift the tree backwards
	fprintf(stderr,"\nShifting Branches");
	shiftBranchesMipMap(_tree,0,_n,_brickLength,BRANCHINIT,maxAddLevel*16);
	time2 = (double)cv::getTickCount();
	fprintf(stderr," %f ms",(time2-time1)/cv::getTickFrequency()*1000.0f);

	fprintf(stderr,"\nShifting Tree...");
	volumetype *treeTemp = new volumetype[_nBranchesUsed];
	memcpy(treeTemp,_tree,_nBranchesUsed*sizeof(volumetype));
	memcpy(_tree+maxAddLevel*16,treeTemp,_nBranchesUsed*sizeof(volumetype));
	for(volumetype i=0;i<maxAddLevel*16;i++) _tree[i] = BRANCHINIT;
	delete [] treeTemp;

	time1 = (double)cv::getTickCount();
	fprintf(stderr," %f ms",(time1-time2)/cv::getTickFrequency()*1000.0f);



	size_t meshCellsSizeBefore = _meshCells.size();


	if(_performIncrementalMeshing){
		fprintf(stderr,"\nShifting _meshCellIndicesBranch...");
#ifdef MESHCELLINDICES_SPLIT
		_meshCellIndicesBranch.resize(_meshCellIndicesBranch.size()+maxAddLevel*16);
		for(long int i=_meshCellIndicesBranch.size()-1;i>=maxAddLevel*16;i--){
			_meshCellIndicesBranch[i] = _meshCellIndicesBranch[i-maxAddLevel*16];
		}
		for(size_t i=0;i<maxAddLevel*16;i++){
#ifdef BRANCHNEIGHBORHOOD_REFERECE
			_meshCellIndicesBranch[i] = new MCNSplit();
#else
			_meshCellIndicesBranch[i].indexInterior = MAXCELLINDEX;
			for(int j=0;j<6;j++) _meshCellIndicesBranch[i].indexWallWall[j].clear();
			for(int j=0;j<6;j++) _meshCellIndicesBranch[i].indexWallEdge[j].clear();
			for(int j=0;j<6;j++) _meshCellIndicesBranch[i].indexWallCorner[j].clear();
			for(int j=0;j<12;j++) _meshCellIndicesBranch[i].indexEdgeEdge[j].clear();
			for(int j=0;j<12;j++) _meshCellIndicesBranch[i].indexEdgeCorner[j].clear();
			for(int j=0;j<8;j++) _meshCellIndicesBranch[i].indexCorner[j] = MAXCELLINDEX;
#endif
		}
		fprintf(stderr,"\nShifting Branch References in MeshCells...");
		for(size_t i=0;i<_meshCellsSplit.size();i++){
			if(_meshCellsSplit[i].lastBranch[0]<BRANCHINIT) _meshCellsSplit[i].lastBranch[0] += maxAddLevel*16;
			if(_meshCellsSplit[i].lastBranch[1]<BRANCHINIT) _meshCellsSplit[i].lastBranch[1] += maxAddLevel*16;
			if(_meshCellsSplit[i].lastBranch[2]<BRANCHINIT) _meshCellsSplit[i].lastBranch[2] += maxAddLevel*16;
			if(_meshCellsSplit[i].lastBranch[3]<BRANCHINIT) _meshCellsSplit[i].lastBranch[3] += maxAddLevel*16;
			if(_meshCellsSplit[i].lastBranch[4]<BRANCHINIT) _meshCellsSplit[i].lastBranch[4] += maxAddLevel*16;
			if(_meshCellsSplit[i].lastBranch[5]<BRANCHINIT) _meshCellsSplit[i].lastBranch[5] += maxAddLevel*16;
			if(_meshCellsSplit[i].lastBranch[6]<BRANCHINIT) _meshCellsSplit[i].lastBranch[6] += maxAddLevel*16;
			if(_meshCellsSplit[i].lastBranch[7]<BRANCHINIT) _meshCellsSplit[i].lastBranch[7] += maxAddLevel*16;
		}
#endif
#ifdef MESHCELLINDICES_COMPACT
		_meshCellIndicesBranchCompact.resize(_meshCellIndicesBranchCompact.size()+maxAddLevel*16);
		for(long int i=_meshCellIndicesBranchCompact.size()-1;i>=maxAddLevel*16;i--){
			_meshCellIndicesBranchCompact[i] = _meshCellIndicesBranchCompact[i-maxAddLevel*16];
		}
		for(size_t i=0;i<maxAddLevel*16;i++){
			_meshCellIndicesBranchCompact[i].indexInterior = MAXCELLINDEX;
//			_meshCellIndicesBranchCompact[i].cells.clear();
			_meshCellIndicesBranchCompact[i].clear();
		}
		fprintf(stderr,"\nShifting Branch References in MeshCells...");
		for(size_t i=0;i<_meshCellsCompact.size();i++){
			if(_meshCellsCompact[i].lastBranch[0]<BRANCHINIT) _meshCellsCompact[i].lastBranch[0] += maxAddLevel*16;
			if(_meshCellsCompact[i].lastBranch[1]<BRANCHINIT) _meshCellsCompact[i].lastBranch[1] += maxAddLevel*16;
			if(_meshCellsCompact[i].lastBranch[2]<BRANCHINIT) _meshCellsCompact[i].lastBranch[2] += maxAddLevel*16;
			if(_meshCellsCompact[i].lastBranch[3]<BRANCHINIT) _meshCellsCompact[i].lastBranch[3] += maxAddLevel*16;
			if(_meshCellsCompact[i].lastBranch[4]<BRANCHINIT) _meshCellsCompact[i].lastBranch[4] += maxAddLevel*16;
			if(_meshCellsCompact[i].lastBranch[5]<BRANCHINIT) _meshCellsCompact[i].lastBranch[5] += maxAddLevel*16;
			if(_meshCellsCompact[i].lastBranch[6]<BRANCHINIT) _meshCellsCompact[i].lastBranch[6] += maxAddLevel*16;
			if(_meshCellsCompact[i].lastBranch[7]<BRANCHINIT) _meshCellsCompact[i].lastBranch[7] += maxAddLevel*16;
		}
#endif

		fprintf(stderr,"\nShifting Subtree and New Leaf Queues");
		for(size_t i=0;i<_newBudsSinceMeshingToAccumulate.subtreeBuds->size();i++){
			(*_newBudsSinceMeshingToAccumulate.subtreeBuds)[i] += maxAddLevel*16;
		}
		for(size_t i=0;i<_newBudsSinceMeshingToAccumulate.leafBuds->size();i++){
			(*_newBudsSinceMeshingToAccumulate.leafBuds)[i] += maxAddLevel*16;
		}
	}


	time2 = (double)cv::getTickCount();
	fprintf(stderr," %f ms",(time2-time1)/cv::getTickFrequency()*1000.0f);


	_nBranchesUsed += maxAddLevel*16;



	time1 = (double)cv::getTickCount();
	fprintf(stderr," %f ms",(time1-time2)/cv::getTickFrequency()*1000.0f);

	//Fill the top layers
	fprintf(stderr,"\nFilling Top Layers...");
	volumetype index = (maxAddLevel-1)*16;
	for(sidetype level=0;level<maxAddLevel;level++){
//		fprintf(stderr,"\nIndex: %i",index);
		unsigned int dec = 1<<level;
		volumetype co = 0;
		sidetype oxchild = 0; sidetype oychild = 0; sidetype ozchild = 0;
		if((xm&dec) || (!(xp&dec) && (xp>xm))){
			xadd += _n;
			oxchild += _n;
			co += 1;
		}
		if((ym&dec) || (!(yp&dec) && (yp>ym))){
			yadd += _n;
			oychild += _n;
			co += 2;
		}
		if((zm&dec) || (!(zp&dec) && (zp>zm))){
			zadd += _n;
			ozchild += _n;
			co += 4;
		}
		_tree[index+co+8] = index+16;

		fprintf(stderr,"\nADDs for Layer %i: [%i %i %i]",index/16,xadd!=0,yadd!=0,zadd!=0);

		if(_performIncrementalMeshing){
#if defined MESHCELLINDICES_SPLIT && defined MESHCELLINDICES_COMPACT
			fprintf(stderr,"\nBranch Indices Sizes before: %li %li",_meshCellIndicesBranch.size(),_meshCellIndicesBranchCompact.size());
			fprintf(stderr,"\nMeshCells Sizes before: %li %li",_meshCellsSplit.size(),_meshCellsCompact.size());



#endif
#ifdef MESHCELLINDICES_SPLIT
			growMeshCellStructures(_meshCellsSplit,_meshCellIndicesBranch,_boundary,index,level,co,oxchild,oychild,ozchild,_n);
#endif
#ifdef MESHCELLINDICES_COMPACT
			growMeshCellStructures_compact(_meshCellsCompact,_meshCellIndicesBranchCompact,_boundaryCompact,index,level,co,oxchild,oychild,ozchild,_n);
#endif
			fprintf(stderr,"\nBranch Indices Sizes after: %li %li",_meshCellIndicesBranch.size(),_meshCellIndicesBranchCompact.size());
			fprintf(stderr,"\nMeshCells Sizes after: %li %li",_meshCellsSplit.size(),_meshCellsCompact.size());

#if defined MESHCELLINDICES_SPLIT && defined MESHCELLINDICES_COMPACT
	if(_meshCellIndicesBranch.size() != _meshCellIndicesBranchCompact.size()){
		fprintf(stderr,"\nERROR: Initial neighborhooods have different sizes:"
				" %li vs %li",_meshCellIndicesBranch.size(),_meshCellIndicesBranchCompact.size());
		char c; std::cin >> c;
	}
	bool equal_neighborhoods = true;
	for(size_t i=0;i<_meshCellIndicesBranch.size();i++){
		MCNSplit compare = _meshCellIndicesBranchCompact[i];
		equal_neighborhoods &= compare == _meshCellIndicesBranch[i];
		if(!equal_neighborhoods){
			fprintf(stderr,"\nERROR: Split Neighborhood %li is different from Compact",i);
		}
	}
	if(equal_neighborhoods) {
		fprintf(stderr,"\nDEBUG: Compact neighborhoods are equal to split ones after one Grow Level.");
	}
#endif

		}

		_n *= 2;
		index -= 16;
	}

	time2 = (double)cv::getTickCount();
	fprintf(stderr,"\nTime for the new levels: %f ms",(time2-time1)/cv::getTickFrequency()*1000.0f);


	fprintf(stderr,"\nADDS: %i %i %i",xadd,yadd,zadd);
	std::cerr << "\nThe Tree has now Size " << _n;

	_offset.x -= xadd*_scale;
	_offset.y -= yadd*_scale;
	_offset.z -= zadd*_scale;
	for(volumetype i=0;i<_nLeavesUsed;i++){
		_leafPos[i].x += xadd; _leafPos[i].y += yadd; _leafPos[i].z += zadd;
	}

	if(_performIncrementalMeshing){
		fprintf(stderr,"\nAdded %li Mesh Cells from %li to %li",_meshCells.size()-meshCellsSizeBefore,meshCellsSizeBefore,_meshCells.size());
	}

	double timeEnd = (double)cv::getTickCount();
	fprintf(stderr,"\nWhole Tree growing took %f ms",(timeEnd-timeBegin)/cv::getTickFrequency()*1000.0f);

	return true;
}

bool FusionMipMapCPU::split()
{
	fprintf(stderr,"\nTODO: Implementieren");
	return true;
}






void bla()
{
	int a=2; a*= a;
}

void blub(int b)
{
	int a=2; a*= b;
}




MeshSeparate FusionMipMapCPU::getMeshSeparateMarchingCubes(MeshSeparate mesh)
{


	eprintf("\nGetting Nonindexed Mesh Recursively");
	return getMeshRecursive(mesh);

//	fprintf(stderr,"\nGetting Nonindexed Mesh Recursively with Parent Information");
//	return getMeshRecursiveIncremental(mesh);

//	fprintf(stderr,"\nGetting Indexed Mesh Approximate");
//	return getMeshMarchingCubesApproximate(mesh);
}

MeshInterleaved FusionMipMapCPU::getMeshInterleavedMarchingCubes(MeshInterleaved mesh)
{


//	fprintf(stderr,"\nGetting Nonindexed Mesh Recursively");
//	return getMeshRecursive(mesh);

//	fprintf(stderr,"\nGetting Nonindexed Mesh Recursively with Parent Information");
//	return getMeshRecursiveIncremental(mesh);

	eprintf("\nGetting Indexed Interleaved Mesh Approximate");

	eprintf("\nInterleaved Mesh-Cell Mesh has %li vertices and %li indices",
			_meshCurrent->vertices.size(),_meshCurrent->faces.size());
	return *_meshCurrent;
}


//MeshSeparate FusionMipMapCPU::getMeshMarchingCubesApproximate(MeshSeparate mesh)
//{
//
////	unsigned int degenerate_faces = 0;
////	fprintf(stderr,"\nUpdating Mesh Cells before Summation");
////	for(size_t i=0;i<_meshCellIndicesLeaf.size();i++){
////		for(size_t j=0;j<_meshCellIndicesLeaf[i].size();j++){
////			_meshCells[_meshCellIndicesLeaf[i][j]].updateMesh(
////					treeinfo(&(_meshCells[_meshCellIndicesLeaf[i][j]].mesh),_brickLength,_brickSize,
////							std::min((double)_framesAdded,MIN_WEIGHT_FOR_SURFACE),
////							_offset,_scale,&degenerate_faces,_nBranchesUsed,_nLeavesUsed,_tree,
////							_leafPos,_leafScale,_distance,_weights,_color),_leafParent);
////		}
////	}
//
////	if(_meshingDone==0){
////	double time0 = (double)cv::getTickCount();
////
////	mesh = Mesh(3);
////	for(unsigned int i=0;i<_meshCells.size();i++){
////		if(!_meshCellIsQueued[i])
////			if(_meshCells[i].mesh)
////				mesh += *(_meshCells[i].mesh);
////	}
////
////	double time1 = (double)cv::getTickCount();
////	_avgTimeSumMesh += time1-time0;
////	}
////
////	fprintf(stderr,"\n Mesh Size (%li %li)",mesh.x.size(),mesh.f.size());
////	return mesh;
//
//	fprintf(stderr,"\nMesh-Cell Mesh has %li vertices and %li indices",_meshSeparateCurrent->x.size(),_meshSeparateCurrent->f.size());
//	return *_meshSeparateCurrent;
//}



//MeshSeparate FusionMipMapCPU::getMeshMarchingCubesSlowOpenMP(MeshSeparate mesh)
//{
//	volumetype nNodes = _nBranchesTotal;
//	volumetype nLeaves = std::min(_nLeavesUsed,_nLeavesTotal);
//
//	float minWeight = std::min((double)_framesAdded,MIN_WEIGHT_FOR_SURFACE);
//
//
////		sidetype n = _n;
////	sidetype xmin = 0;  sidetype ymin = 0;  sidetype zmin = 0;
////	sidetype xmax = n-1; sidetype ymax = n-1; sidetype zmax = n-1;
//
//
//
//
//
////	sidetype n = 8192;
////	sidetype xmin = 500+16384;  sidetype ymin = 0+8192;  sidetype zmin = n/4;
////	sidetype xmax = n/2+16384; sidetype ymax = n/2+8192; sidetype zmax = 3*n/4;
//
//
//
//	//Das liefert auf hoechster Aufloesung schonmal CLaudias Buero und die Sozialniesche aus 10-13
//// 	sidetype xdim = 4096;
//// 	sidetype ydim = 3072;
//// 	sidetype zdim = 8192;
//// 	sidetype n = _n;
//// 	sidetype xmin = sidetype(std::max(0,int(-_offset.x/_scale)-1000));
//// 	sidetype ymin = sidetype(std::max(0,int(-_offset.y/_scale)-1500));
//// 	sidetype zmin = sidetype(std::max(0,int(-_offset.z/_scale)-500));
//// 	sidetype xmax = std::min(n-1,xmin+xdim);
//// 	sidetype ymax = std::min(n-1,ymin+ydim);
//// 	sidetype zmax = std::min(n-1,zmin+zdim);
//
//	sidetype xdim = 8192;
//	sidetype ydim = 3072;
//	sidetype zdim = 10000;
//	sidetype n = _n;
//	sidetype xmin = sidetype(std::max(0,int(-_offset.x/_scale)-4096));
//	sidetype ymin = sidetype(std::max(0,int(-_offset.y/_scale)-1500));
//	sidetype zmin = sidetype(std::max(0,int(-_offset.z/_scale)-500));
//	sidetype xmax = std::min(n-1,xmin+xdim);
//	sidetype ymax = std::min(n-1,ymin+ydim);
//	sidetype zmax = std::min(n-1,zmin+zdim);
//
////	sidetype xdim = 2048;
////	sidetype ydim = 2048;
////	sidetype zdim = 16192;
////	sidetype n = _n;
////	sidetype xmin = sidetype(std::max(0,int(-_offset.x/_scale)-110));
////	sidetype ymin = sidetype(std::max(0,int(-_offset.y/_scale)-1024));
////	sidetype zmin = sidetype(std::max(0,int(-_offset.z/_scale)-500));
////	sidetype xmax = std::min(n-1,xmin+xdim);
////	sidetype ymax = std::min(n-1,ymin+ydim);
////	sidetype zmax = std::min(n-1,zmin+zdim);
//
//
////	sidetype xdim = 4096;
////	sidetype ydim = 3072;
////	sidetype zdim = 32384;
////	sidetype n = _n;
////	sidetype xmin = sidetype(std::max(0,int(-_offset.x/_scale)-2200));
////	sidetype ymin = sidetype(std::max(0,int(-_offset.y/_scale)-2048));
////	sidetype zmin = sidetype(std::max(0,int(-_offset.z/_scale)-1000));
////	sidetype xmax = std::min(n-1,xmin+xdim);
////	sidetype ymax = std::min(n-1,ymin+ydim);
////	sidetype zmax = std::min(n-1,zmin+zdim);
//
//
//	fprintf(stderr,"\nThe Tree has the Offset (%f %f %f) which gives a range"
//	" (%i %i %i)-(%i % i%i)",_offset.x,_offset.y,_offset.z,xmin,ymin,zmin,xmax,ymax,zmax);
//
//
//
//	sidetype blockLength = 500;
//	sidetype step = 1;
//
//
//	volumetype *index = new volumetype[blockLength*blockLength*blockLength];
//
//	for(sidetype ox=xmin;ox<xmax;ox+=step*(blockLength-1)){
//		for(sidetype oy=ymin;oy<ymax;oy+=step*(blockLength-1)){
//			for(sidetype oz=zmin;oz<zmax;oz+=step*(blockLength-1)){
//				fprintf(stderr," block(%i %i %i)",ox,oy,oz);
//
//#ifdef USE_OPENMP_MARCHINGCUBESSLOW
//			omp_set_dynamic(OPENMP_THREADS);
//#pragma omp parallel for
//#endif
//				for(sidetype x=0;x<blockLength;x++){
//					for(sidetype y=0;y<blockLength;y++){
//						for(sidetype z=0;z<blockLength;z++){
//							index[(z*blockLength+y)*blockLength+x] =
//								getBrickIndexMipMap2(ox+x,oy+y,oz+z,_n,_brickLength,_brickSize,nNodes,nLeaves,_tree,DEADBRANCH);
//							if(index[(z*blockLength+y)*blockLength+x]>=DEADBRANCH &&
//									((oz+z)%_brickLength==0) && ((oy+y)%_brickLength) && ((ox+x)%_brickLength)){
//								z+=_brickLength-1;
//							}
//						}
//					}
//				}
//				fprintf(stderr,"...");
//
//#ifdef USE_OPENMP_MARCHINGCUBESSLOW
//			omp_set_dynamic(OPENMP_THREADS);
//#pragma omp parallel for
//#endif
//				for(sidetype x=0;x<blockLength-1;x++){
//					MeshSeparate mx(3);
//					Vertex3f vertlist[12];
//					VertexColor collist[12];
//					MarchingCubes mc;
//					for(sidetype y=0;y<blockLength-1;y++){
//						for(sidetype z=0;z<blockLength-1;z++){
////							if(false){
//							if(((oz+z)%_brickLength==0) && index[(z*blockLength+y)*blockLength+x]>=DEADBRANCH){
//								z+=_brickLength-1;
//							}
//							else{
//								volumetype idx[8] = {
//								index[(z*blockLength+y)*blockLength+x],
//								index[(z*blockLength+y)*blockLength+x+1],
//								index[(z*blockLength+y+1)*blockLength+x+1],
//								index[(z*blockLength+y+1)*blockLength+x],
//								index[((z+1)*blockLength+y)*blockLength+x],
//								index[((z+1)*blockLength+y)*blockLength+x+1],
//								index[((z+1)*blockLength+y+1)*blockLength+x+1],
//								index[((z+1)*blockLength+y+1)*blockLength+x]
//								};
//
//								bool valid = true;
//								for(volumetype i=0;i<8;i++) valid &= (idx[i]<DEADBRANCH);
//								if(valid){
//									sidetype3 o[8]; for(volumetype i=0;i<8;i++) o[i] = _leafPos[idx[i]];
//									sidetype  s[8]; for(volumetype i=0;i<8;i++) s[i] = _leafScale[idx[i]];
//
//									sidetype bx[8] = {
//											(ox+x  -o[0].x)/s[0],
//											(ox+x+1-o[1].x)/s[1],
//											(ox+x+1-o[2].x)/s[2],
//											(ox+x  -o[3].x)/s[3],
//											(ox+x  -o[4].x)/s[4],
//											(ox+x+1-o[5].x)/s[5],
//											(ox+x+1-o[6].x)/s[6],
//											(ox+x  -o[7].x)/s[7]
//									};
//									sidetype by[8] = {
//											(oy+y  -o[0].y)/s[0],
//											(oy+y  -o[1].y)/s[1],
//											(oy+y+1-o[2].y)/s[2],
//											(oy+y+1-o[3].y)/s[3],
//											(oy+y  -o[4].y)/s[4],
//											(oy+y  -o[5].y)/s[5],
//											(oy+y+1-o[6].y)/s[6],
//											(oy+y+1-o[7].y)/s[7]
//									};
//									sidetype bz[8] = {
//											(oz+z  -o[0].z)/s[0],
//											(oz+z  -o[1].z)/s[1],
//											(oz+z  -o[2].z)/s[2],
//											(oz+z  -o[3].z)/s[3],
//											(oz+z+1-o[4].z)/s[4],
//											(oz+z+1-o[5].z)/s[5],
//											(oz+z+1-o[6].z)/s[6],
//											(oz+z+1-o[7].z)/s[7]
//									};
//
//
//									float val[8]; weighttype w[8]; VertexColor col[8];
//									for(volumetype i=0;i<8;i++){
//										val[i] = _distance[idx[i]*_brickSize + (bz[i]*_brickLength+by[i])*_brickLength+bx[i]];
//										w[i] =    _weights[idx[i]*_brickSize + (bz[i]*_brickLength+by[i])*_brickLength+bx[i]];
//										if(_useColor){
//											col[i] = VertexColor(_color[idx[i]*_brickSize + (bz[i]*_brickLength+by[i])*_brickLength+bx[i]].x,
//																					 _color[idx[i]*_brickSize + (bz[i]*_brickLength+by[i])*_brickLength+bx[i]].y,
//																					 _color[idx[i]*_brickSize + (bz[i]*_brickLength+by[i])*_brickLength+bx[i]].z);
//										}
//									}
//
//									if(weightInfluence(minWeight,w[0],w[1],w[2],w[3],w[4],w[5],w[6],w[7])){
//
//										int *table = mc.getColoredTrianglesForCubePrecomputed(vertlist,collist,ox+x,oy+y,oz+z,
//												_offset.x,_offset.y,_offset.z,
//												_scale,_scale,_scale,
//												val[0],val[1],val[2],val[3],val[4],val[5],val[6],val[7],
//												w[0],w[1],w[2],w[3],w[4],w[5],w[6],w[7],
//												col[0],col[1],col[2],col[3],col[4],col[5],col[6],col[7],_color);
//
//										for (unsigned int i=0;table[i]!=-1;i+=3) {
//											Vertex3f v0 = vertlist[table[i  ]];
//											Vertex3f v1 = vertlist[table[i+1]];
//											Vertex3f v2 = vertlist[table[i+2]];
//											if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
//
//											}
//											else{
//												mx.x.push_back(v0.x); mx.y.push_back(v0.y); mx.z.push_back(v0.z);
//												mx.f.push_back(mx.x.size()-1);
//												mx.x.push_back(v1.x); mx.y.push_back(v1.y); mx.z.push_back(v1.z);
//												mx.f.push_back(mx.x.size()-1);
//												mx.x.push_back(v2.x); mx.y.push_back(v2.y); mx.z.push_back(v2.z);
//												mx.f.push_back(mx.x.size()-1);
//
//												if(_useColor){
//													VertexColor c0 = collist[table[i  ]];
//													VertexColor c1 = collist[table[i+1]];
//													VertexColor c2 = collist[table[i+2]];
//													mx.r.push_back(c0.x/COLOR_MULTIPLICATOR);
//													mx.g.push_back(c0.y/COLOR_MULTIPLICATOR);
//													mx.b.push_back(c0.z/COLOR_MULTIPLICATOR);
//													mx.r.push_back(c1.x/COLOR_MULTIPLICATOR);
//													mx.g.push_back(c1.y/COLOR_MULTIPLICATOR);
//													mx.b.push_back(c1.z/COLOR_MULTIPLICATOR);
//													mx.r.push_back(c2.x/COLOR_MULTIPLICATOR);
//													mx.g.push_back(c2.y/COLOR_MULTIPLICATOR);
//													mx.b.push_back(c2.z/COLOR_MULTIPLICATOR);
//												}
//											}
//										}
//									}
//								}
//							}
//						}
//					}
//					fprintf(stderr," %i",(int)mx.x.size());
//#pragma omp critical
//					mesh += mx;
//				}
//				fprintf(stderr,"!");
//			}
//		}
//	}
//
//	delete [] index;
//
//
//	return mesh;
//}
//
//
//
//
//
MeshSeparate FusionMipMapCPU::getMeshRecursive(MeshSeparate mesh)
{
	volumetype nNodes = _nBranchesTotal;
	volumetype nLeaves = std::min(_nLeavesUsed,_nLeavesTotal);


	unsigned int degenerate_faces = 0;

	MarchingCubesIndexed mc(_brickLength,_brickLength);

	addInterior(leafstack(),
			treeinfo(&mesh,_brickLength,_brickSize,std::min((double)_framesAdded,MIN_WEIGHT_FOR_SURFACE),_offset,_scale,
					&degenerate_faces,nNodes,nLeaves,_tree,_leafPos,_leafScale,_distance,_weights,_color),
					0,_n,0,0,0,BRANCHINIT,mc);

	return mesh;
}

//MeshSeparate FusionMipMapCPU::getMeshRecursiveIncremental(MeshSeparate mesh)
//{
//	volumetype nNodes = _nBranchesTotal;
//	volumetype nLeaves = std::min(_nLeavesUsed,_nLeavesTotal);
//
//
//	unsigned int degenerate_faces = 0;
//
//	MarchingCubesIndexed mc(_brickLength,_brickLength);
//
//
//	std::vector<MeshCell> meshCells;
//	std::list<size_t> meshCellQueue;
//	fprintf(stderr,"\nFilling MeshCells");
//
//	addInterior_incremental(leafstack(),
//			treeinfo(&mesh,_brickLength,_brickSize,std::min((double)_framesAdded,MIN_WEIGHT_FOR_SURFACE),_offset,_scale,
//					&degenerate_faces,nNodes,nLeaves,_tree,_leafPos,_leafScale,_distance,_weights,_color),
//					0,_n,0,0,0,BRANCHINIT,mc,_leafParent
////					,&meshCells
//					);
//
//	fprintf(stderr,"\nUpdating MeshCells");
//	for(size_t i=0;i<meshCells.size();i++) meshCellQueue.push_back(i);
//	size_t fullCells = 0;
//
//	size_t interiorFullCells = 0;
//	size_t interiorCells = 0;
//	size_t wallFullCells = 0;
//	size_t wallCells = 0;
//	size_t edgeFullCells = 0;
//	size_t edgeCells = 0;
//	size_t cornerFullCells = 0;
//	size_t cornerCells = 0;
//
//	std::vector<size_t> cellsOnlyInRecursive;
//	std::vector<size_t> cellsOnlyInIncremental;
//	std::vector<size_t> cellsRecursive(meshCellQueue.begin(),meshCellQueue.end());
//
//	for(size_t i=0;i<_meshCells.size();i++){
//		bool found = false;
//		for(size_t j=0;j<meshCells.size();j++){
//			if(_meshCells[i].ox  ==meshCells[j].ox &&
//				 _meshCells[i].oy  ==meshCells[j].oy &&
//				 _meshCells[i].oz  ==meshCells[j].oz &&
//				 _meshCells[i].size==meshCells[j].size &&
//				 _meshCells[i].type==meshCells[j].type){
//				found = true; break;
//			}
//		}
//		if(!found){
//			if(_meshCells[i].type==7 && _meshCells[i].size==8) cellsOnlyInIncremental.push_back(i);
//		}
//	}
//	for(size_t j=0;j<meshCells.size();j++){
//		bool found = false;
//		for(size_t i=0;i<_meshCells.size();i++){
//		if(_meshCells[i].ox  ==meshCells[j].ox &&
//			  _meshCells[i].oy  ==meshCells[j].oy &&
//			  _meshCells[i].oz  ==meshCells[j].oz &&
//				_meshCells[i].size==meshCells[j].size &&
//			  _meshCells[i].type==meshCells[j].type){
//				found = true; break;
//			}
//		}
//		if(!found){
//			cellsOnlyInRecursive.push_back(j);
//		}
//	}
//
//	fprintf(stderr,"\nComparisons done");
//
////	std::vector<size_t> drawQueue = cellsOnlyInIncremental;
//	std::vector<size_t> drawQueue = cellsRecursive;
//
//
//	for(size_t j=0;j<drawQueue.size();j++){
////		MeshCell &cell = _meshCells[drawQueue[j]];
//		MeshCell &cell = meshCells[drawQueue[j]];
//
//		if(( (cell.type==0&&cell.lastLeaf[0]<BRANCHINIT)||
//				 ((cell.type==1 || cell.type==2 || cell.type==3)&&
//					(cell.lastLeaf[0]<BRANCHINIT && cell.lastLeaf[1]<BRANCHINIT))||
//				 ((cell.type==4 || cell.type==5 || cell.type==6)&&
//					(cell.lastLeaf[0]<BRANCHINIT && cell.lastLeaf[1]<BRANCHINIT &&
//				   cell.lastLeaf[2]<BRANCHINIT && cell.lastLeaf[3]<BRANCHINIT))||
//				 ( cell.type==7 &&
//					(cell.lastLeaf[0]<BRANCHINIT && cell.lastLeaf[1]<BRANCHINIT &&
//					 cell.lastLeaf[2]<BRANCHINIT && cell.lastLeaf[3]<BRANCHINIT&&
//					 cell.lastLeaf[4]<BRANCHINIT && cell.lastLeaf[5]<BRANCHINIT &&
//					 cell.lastLeaf[6]<BRANCHINIT && cell.lastLeaf[7]<BRANCHINIT)))){
//
////			fprintf(stderr,"\n%li->%li",j,drawQueue[j]);
////			fprintf(stderr," %i[%i %i %i]%i",cell.type,cell.ox,cell.oy,cell.oz,cell.size);
//			if(cell.type==0){
//				if(cell.size==_leafScale[cell.lastLeaf[0]]*_brickLength){
//					interiorFullCells++;
//				}
//				interiorCells++;
//			}
//			if(cell.type==1 || cell.type==2 || cell.type==3){
//				if(cell.size==_leafScale[cell.lastLeaf[0]]*_brickLength &&
//					 cell.size==_leafScale[cell.lastLeaf[1]]*_brickLength){
//					wallFullCells++;
//				}
//				wallCells++;
//			}
//			if(cell.type==4 || cell.type==5 || cell.type==6){
//				if(cell.size==_leafScale[cell.lastLeaf[0]]*_brickLength &&
//					 cell.size==_leafScale[cell.lastLeaf[1]]*_brickLength &&
//					 cell.size==_leafScale[cell.lastLeaf[2]]*_brickLength &&
//					 cell.size==_leafScale[cell.lastLeaf[3]]*_brickLength){
//					edgeFullCells++;
//				}
//				edgeCells++;
//			}
//			if(cell.type==7){
//				if(cell.size==_leafScale[cell.lastLeaf[0]]*_brickLength &&
//					 cell.size==_leafScale[cell.lastLeaf[1]]*_brickLength &&
//					 cell.size==_leafScale[cell.lastLeaf[2]]*_brickLength &&
//					 cell.size==_leafScale[cell.lastLeaf[3]]*_brickLength &&
//					 cell.size==_leafScale[cell.lastLeaf[4]]*_brickLength &&
//					 cell.size==_leafScale[cell.lastLeaf[5]]*_brickLength &&
//					 cell.size==_leafScale[cell.lastLeaf[6]]*_brickLength &&
//					 cell.size==_leafScale[cell.lastLeaf[7]]*_brickLength){
//					cornerFullCells++;
//				}
//				cornerCells++;
//			}
//
//			if(cell.size==_leafScale[cell.lastLeaf[0]]*_brickLength){
//				fullCells++;
//			}
//		}
//		cell.updateMesh(
//				treeinfo(NULL,_brickLength,_brickSize,
//						std::min((double)_framesAdded,MIN_WEIGHT_FOR_SURFACE),
//						_offset,_scale,&degenerate_faces,_nBranchesUsed,_nLeavesUsed,_tree,
//						_leafPos,_leafScale,_distance,_weights,_color),_leafParent,mc);
//	}
//
//	fprintf(stderr,"\nCells [Full/All] for Recursive Incremental:\nInt: [%li %li] Wall: [%li %li] Edge: [%li %li] Cor: [%li %li]",
//			interiorFullCells,interiorCells,wallFullCells,wallCells,
//			edgeFullCells,edgeCells,cornerFullCells,cornerCells);
//
//
//	if(meshCells.size()){
//		fprintf(stderr,"\nAdding %li MeshCells together",meshCells.size());
//		mesh = MeshSeparate(3);
//		for(size_t j=0;j<drawQueue.size();j++){
//	//		MeshCell &cell = _meshCells[drawQueue[j]];
//			MeshCell &cell = meshCells[drawQueue[j]];
////			fprintf(stderr,"\n%li->%li",j,drawQueue[j]);
////			fprintf(stderr," %i[%i %i %i]%i",cell.type,cell.ox,cell.oy,cell.oz,cell.size);
////			if(cell.type==7) fprintf(stderr,"\n [%i %i %i %i %i %i %i %i]",
////					cell.lastLeaf[0],cell.lastLeaf[1],cell.lastLeaf[2],cell.lastLeaf[3],
////					cell.lastLeaf[4],cell.lastLeaf[5],cell.lastLeaf[6],cell.lastLeaf[7]);
////			if(cell.type==7) fprintf(stderr,"[%i %i %i %i %i %i %i %i]",
////							_leafScale[cell.lastLeaf[0]],_leafScale[cell.lastLeaf[1]],_leafScale[cell.lastLeaf[2]],_leafScale[cell.lastLeaf[3]],
////							_leafScale[cell.lastLeaf[4]],_leafScale[cell.lastLeaf[5]],_leafScale[cell.lastLeaf[6]],_leafScale[cell.lastLeaf[7]]);
////			if(cell.type==7) fprintf(stderr," [%i %i %i %i %i %i %i %i]",
////					cell.lastBranch[0],cell.lastBranch[1],cell.lastBranch[2],cell.lastBranch[3],
////					cell.lastBranch[4],cell.lastBranch[5],cell.lastBranch[6],cell.lastBranch[7]);
//				mesh += *cell.mesh;
//		}
//	}
//	else{
//		fprintf(stderr,"\nThere are no MeshCells, the Update took place in the Tree Traversal");
//	}
//
//
//
//
//	return mesh;
//}



MeshSeparate FusionMipMapCPU::getMeshStructural(unsigned int structureType,MeshSeparate mesh)
{
	fprintf(stderr," TODO: Mit Meshaddition anpassen");
	if(structureType==0){
		fprintf(stderr,"\nComputing Octree Line Mesh");
		unsigned int prevIndex = mesh.x.size();
		float width = 5.0f;
//		for(int w=_brickLength;w<=_n;w*=2) width += 1.0f;
		getTreeLinesBoxMipMap(_tree,0,_brickLength,_nBranchesTotal,_n/2,0,0,0,mesh,width);
		for(unsigned int i=prevIndex;i<mesh.x.size();i++){
			mesh.x[i] = _offset.x+mesh.x[i]*_scale;
			mesh.y[i] = _offset.y+mesh.y[i]*_scale;
			mesh.z[i] = _offset.z+mesh.z[i]*_scale;
		}
	}
	else if(structureType==1){
		fprintf(stderr,"\nComputing Brick Line Mesh");
		volumetype nLeaves = std::min(_nLeavesUsed,_nLeavesTotal);
		for(volumetype i=0;i<nLeaves;i++){
//			fprintf(stderr," [%i %i %i]",leafPosition[i].x,leafPosition[i].y,leafPosition[i].z);
			if(_leafScale[i]){
				mesh.addAlignedLineCube(_offset.x+_scale*_leafPos[i].x,
						_offset.y+_scale*_leafPos[i].y,_offset.z+_scale*_leafPos[i].z,
						_scale*_brickLength*_leafScale[i],
						_scale*_brickLength*_leafScale[i],
						_scale*_brickLength*_leafScale[i],1.0f);
			}
		}
	}
	else{
		fprintf(stderr,"\nERROR:Structure Type is not supported!");
	}
	return mesh;
}


void loopClosureWrapper
(
		FusionMipMapCPU *fusion,
		std::multimap<float,size_t> *posemap,
		volatile bool *loopClosureDone
)
{
	fprintf(stderr,"\nLoop Closure Thread started");

//	fprintf(stderr,"\nDeleting old Poses");
	fusion->deleteOldLoopPoses();

	fprintf(stderr,"\nAdding new Poses");
	fusion->addNewLoopPoses();

	fprintf(stderr,"\nLoop Closure Thread finished");
	*loopClosureDone = true;
}


void FusionMipMapCPU::updateArraysForLoopClosure(FusionMipMapCPU *fusion)
{
	_meshCells = fusion->_meshCells;
	_meshCellIndicesBranch = fusion->_meshCellIndicesBranch;
	_meshCellIndicesLeaf = fusion->_meshCellIndicesLeaf;
	_leafParent = fusion->_leafParent;
	_boundary = fusion->_boundary;
	_posemap = fusion->_posemap;
	_leafQueues = fusion->_leafQueues;
	_posesFused = fusion->_posesFused;
	_posesLoop = fusion->_posesLoop;

	if(fusion->_nLeavesUsed-_nLeavesUsed>0){
		memcpy(_leafPos  +_nLeavesUsed,fusion->_leafPos  +_nLeavesUsed,(fusion->_nLeavesUsed-_nLeavesUsed)*sizeof(sidetype3));
		memcpy(_leafScale+_nLeavesUsed,fusion->_leafScale+_nLeavesUsed,(fusion->_nLeavesUsed-_nLeavesUsed)*sizeof(sidetype));
		_nLeavesUsed = fusion->_nLeavesUsed;
	}
	_nLeavesMapped = 0;
	memset(_distance,0,_nLeavesTotal*sizeof(float));
	memset(_weightsLoop,0,_nLeavesTotal*sizeof(float));
	memset(_color,0,_nLeavesTotal*sizeof(colortype3));
	for(volumetype i=0;i<_nLeavesTotal;i++) _leafMapLoop[i] = BRANCHINIT;
}

void FusionMipMapCPU::deleteOldLoopPoses()
{
	fprintf(stderr,"\nLeaves mapped before delete Loop: %i",_nLeavesMapped);
	unsigned int counter = 0;
	fprintf(stderr,"\nDeleting old Poses from Loop of Size %li",_posemap.size());
	fprintf(stderr,"\nThere are %li References and %li Images in the CPU Buffer",_depthImageReferences.size(),_depthImageBuffer.size());
	for(std::multimap<float,size_t>::iterator i=_posemap.begin();i!=_posemap.end() && counter<_maxPosesPerLoop;i++,counter++){
		size_t p = i->second;
		std::vector<volumetype> &queue = _leafQueues[p];
		fprintf(stderr,"\nPose %li: %li Leaves",p,queue.size());

		//Getting the Images
		_depthImageReferences.resize(p+1,_depthImageBuffer.end());
		if(_depthImageReferences[p]==_depthImageBuffer.end()){
//			fprintf(stderr,"\nLoading Depth Image from Hard Drive");
			_depthImageBuffer.push_front(std::pair<cv::Mat,size_t>(cv::imread(getNameD(p),-1),p));
			_depthImageBuffer.front().first.convertTo(_depthImageBuffer.front().first,CV_32FC1,1.0/5000.0);
			_depthImageBuffer.front().first.setTo(IMAGEINFINITE,_depthImageBuffer.front().first == 0.0);
			_depthImageReferences[p] = _depthImageBuffer.begin();
			if(_depthImageBuffer.size()>_maxImageBufferSize){
//				fprintf(stderr,"\nDeleting least used Depth Image from Memory");
				_depthImageReferences[_depthImageBuffer.back().second] = _depthImageBuffer.end();
				_depthImageBuffer.pop_back();
			}
		}
		else{
//			fprintf(stderr,"\nDepth Image %li is in CPU Memory",p);
			_depthImageBuffer.push_front(*_depthImageReferences[p]);
			_depthImageBuffer.erase(_depthImageReferences[p]);
		}

		_rgbImageReferences.resize(p+1,_rgbImageBuffer.end());
		if(_rgbImageReferences[p]==_rgbImageBuffer.end()){
//			fprintf(stderr,"\nLoading RGB Images from Hard Drive");
			_rgbImageBuffer.push_front(std::pair<std::vector<cv::Mat>,size_t>(std::vector<cv::Mat>(3),p));
			_rgbImageBuffer.front().first[0] = cv::imread(getNameR(p),-1);
			_rgbImageBuffer.front().first[1] = cv::imread(getNameG(p),-1);
			_rgbImageBuffer.front().first[2] = cv::imread(getNameB(p),-1);
			_rgbImageReferences[p] = _rgbImageBuffer.begin();
			if(_rgbImageBuffer.size()>_maxImageBufferSize){
//				fprintf(stderr,"\nDeleting least used RGB Images from Memory");
				_rgbImageReferences[_rgbImageBuffer.back().second] = _rgbImageBuffer.end();
				_rgbImageBuffer.pop_back();
			}
		}
		else{
//			fprintf(stderr,"\nRGB Images are in CPU Memory");
			_rgbImageBuffer.push_front(*_rgbImageReferences[p]);
			_rgbImageBuffer.erase(_rgbImageReferences[p]);
		}

		const float *depthData = (const float*)(_depthImageReferences[p]->first.data);
		const uchar *rData = (const uchar*)(_rgbImageReferences[p]->first[2].data);
		const uchar *gData = (const uchar*)(_rgbImageReferences[p]->first[1].data);
		const uchar *bData = (const uchar*)(_rgbImageReferences[p]->first[0].data);

		CameraInfo caminfo2 = _posesLoop[p];
		caminfo2.setExtrinsic(caminfo2.getExtrinsicInverse());
		cv::Mat rot2 = caminfo2.getRotation();
		cv::Mat trans2 = caminfo2.getTranslation();
		cv::Mat intr2 = caminfo2.getIntrinsic();
		camPamsFloat pInv(
		rot2.at<double>(0,0), rot2.at<double>(0,1), rot2.at<double>(0,2),
		rot2.at<double>(1,0), rot2.at<double>(1,1), rot2.at<double>(1,2),
		rot2.at<double>(2,0), rot2.at<double>(2,1), rot2.at<double>(2,2),
		rot2.at<double>(0,0)*_offset.x + rot2.at<double>(0,1)*_offset.y + rot2.at<double>(0,2)*_offset.z + trans2.at<double>(0,0),
		rot2.at<double>(1,0)*_offset.x + rot2.at<double>(1,1)*_offset.y + rot2.at<double>(1,2)*_offset.z + trans2.at<double>(1,0),
		rot2.at<double>(2,0)*_offset.x + rot2.at<double>(2,1)*_offset.y + rot2.at<double>(2,2)*_offset.z + trans2.at<double>(2,0),
		intr2.at<double>(0,0), intr2.at<double>(1,1),
		intr2.at<double>(0,2), intr2.at<double>(1,2));

		float m11 = pInv.r11; float m12 = pInv.r12;
		float m13 = pInv.r13; float m14 = pInv.t1 ;
		float m21 = pInv.r21; float m22 = pInv.r22;
		float m23 = pInv.r23; float m24 = pInv.t2 ;
		float m31 = pInv.r31; float m32 = pInv.r32;
		float m33 = pInv.r33; float m34 = pInv.t3;

		updateWrapper_deleteloop(threadparameterLoop(
				depthData,rData,gData,bData,
				_imageWidth,_imageHeight,
				m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,
				pInv.fx,pInv.fy,pInv.cx,pInv.cy,_scale,_distanceThreshold,
				queue.data(),_leafPos,_leafScale,
				_distance,_weightsLoop,_color,_brickLength),queue.size(),_leafMapLoop,_nLeavesTotal,&_nLeavesMapped);


		_posesFused[p] = _posesLoop[p];
	}
	fprintf(stderr,"\nDeleted %i Poses in %i Leaves",counter,_nLeavesUsed);
}

void FusionMipMapCPU::addNewLoopPoses()
{

}

void FusionMipMapCPU::startLoopClosure()
{
	if(!_loopClosureDone){
		fprintf(stderr,"\nLoopClosure already running, do not start another one");
		return;
	}

	if(!_loopClosureFusion){
		fprintf(stderr,"\nCreating Loop Closure Partner-Fusion.");
		_loopClosureFusion = new FusionMipMapCPU(this,MAX_LOOP_CLOSURE_LEAVES);
		fprintf(stderr,"\nLoop Closure Partner-Fusion created.");
	}

	fprintf(stderr,"\nFilling Pose-Difference Map with %li Poses...",_posesLoop.size());
	_posemap.clear();
	for(size_t i=0;i<_posesLoop.size();i++){
		cv::Mat t1 = _posesLoop[i].getTranslation();
		cv::Mat t2 = _posesFused[i].getTranslation();
		float posedifference =
				-((t1.at<double>(0,0) - t2.at<double>(0,0))*(t1.at<double>(0,0) - t2.at<double>(0,0))+
				  (t1.at<double>(1,0) - t2.at<double>(1,0))*(t1.at<double>(1,0) - t2.at<double>(1,0))+
				  (t1.at<double>(2,0) - t2.at<double>(2,0))*(t1.at<double>(2,0) - t2.at<double>(2,0)));
		_posemap.insert(std::pair<float,size_t>(posedifference,i));
	}
	fprintf(stderr,"done");


	fprintf(stderr,"\nCopying Arrays to Loop-Closure-Fusion Object...");
	_loopClosureFusion->updateArraysForLoopClosure(this);
	fprintf(stderr,"done");

	_loopClosureDone = false;
	if(_loopClosureThread) delete _loopClosureThread;

	_loopClosureThread = new boost::thread(loopClosureWrapper,_loopClosureFusion,&_posemap,&_loopClosureDone);

}


void FusionMipMapCPU::saveZimages()
//void FusionMipMapCPU::saveZimages(std::vector<volumetype> indices, std::string prefix)
{
	fprintf(stderr,"\nSaving Debug Z Images");

	typedef struct bgra_ {uchar b; uchar g; uchar r; uchar a;} bgra;

	std::vector<volumetype> indices(_nLeavesUsed);
	for(volumetype i=0;i<indices.size();i++) indices[i] = i;

//	std::vector<volumetype> indices(_nLeavesUsed-_nLeavesBeforeLastFrame);
//	for(volumetype i=0;i<indices.size();i++) indices[i] = i+_nLeavesBeforeLastFrame;

//	std::vector<volumetype> indices(_leafNumberSurface,_leafNumberSurface+_nLeavesQueuedSurface);

//	fprintf(stderr,"\nComputing Negatives");
//	std::vector<volumetype> indices;
//	for(volumetype i=0;i<_nLeavesUsed;i++){
//		bool queued = false;
//		for(volumetype j=0;j<_nLeavesQueuedSurface;j++){
//			if(_leafNumberSurface[j]==i){
//				queued = true;
//				break;
//			}
//		}
//		if(!queued) indices.push_back(i);
//	}
//	fprintf(stderr,"\nNegatives Computed");

	std::string prefix = "debug";
	sidetype maxScale = _n/2/_brickLength;
	sidetype minScale = 256;
//	size_t maxMemForDistance = 268435456;
	size_t maxMemForDistance = 1073741824;

	fprintf(stderr,"\nFilling Leaf Maps ordered by Z-Offset...");
	std::vector<std::map<sidetype,std::vector<volumetype> > > queues;

//	for(sidetype s=1;s<=maxScale;s*=2){
//		fprintf(stderr,"\nScale %i:",s);
//		queues.push_back(std::map<sidetype,std::vector<volumetype> >());
//		for(size_t i=0;i<indices.size();i++){
//			if(_leafScale[indices[i]]==s)
//			queues.back()[_leafPos[indices[i]].z].push_back(indices[i]);
//		}
//	}

	volumetype daLeaf = 510;
	for(sidetype s=1;s<=maxScale;s*=2){
		fprintf(stderr,"\nScale %i:",s);
		queues.push_back(std::map<sidetype,std::vector<volumetype> >());
		if(daLeaf<BRANCHINIT && _leafScale[daLeaf]==s){
			queues.back()[_leafPos[daLeaf].z].push_back(daLeaf);
			daLeaf = _leafParent[daLeaf];
		}
	}

	fprintf(stderr,"\nLeaf Map filled");
	fprintf(stderr,"\nLeaves on different scales:\n");
	size_t k=0;
	for(sidetype s=1;s<=maxScale;s*=2){
		fprintf(stderr," %i->%li",s,queues[k].size());
		k++;
	}

	std::set<sidetype> imageWritten;

	if(maxScale<=1){

	}
	else{
		{

			fprintf(stderr,"\nInitializing Images...");
			std::vector<cv::Mat> images(_n,cv::Mat());
			cv::Mat initimage(_n,_n,CV_8UC4);
			bgra *data = (bgra*) initimage.data;
			memset(data,0,_n*_n*4);

			std::set<sidetype> zset;
			fprintf(stderr,"\nFilling Set...");
			sidetype initsize = 8;
			for(size_t s=0;s<queues.size();s++,initsize*=2){
				fprintf(stderr,"\nSize %i",initsize);
				for(std::map<sidetype,std::vector<volumetype> >::iterator k=queues[s].begin();k!=queues[s].end();k++){
					if(_leafScale[k->second[0]]*_brickLength != initsize){
						fprintf(stderr,"\nERROR: Invalid Sizes: %i vs %i",_leafScale[k->second[0]]*_brickLength,initsize);
					}
					fprintf(stderr,"\n%i , %i : ",k->first,initsize);
					for(sidetype z=k->first;z<k->first+initsize;z++){
						if(zset.insert(z).second) fprintf(stderr," %i",z);
					}
				}
			}
			fprintf(stderr,"\nSet filled.\n");

			size_t counter=0;
			for(std::set<sidetype>::iterator k=zset.begin();k!=zset.end();k++,counter++){
				sidetype z = *k;
				fprintf(stderr," %i(%li|%li)",z,counter,zset.size());
				char buffer[500];
				sprintf(buffer,"%s/dis%.5i.png",prefix.c_str(),z);
				cv::imwrite(buffer,initimage);
				sprintf(buffer,"%s/col%.5i.png",prefix.c_str(),z);
				cv::imwrite(buffer,initimage);
				imageWritten.insert(z);
			}
//			return;
		}

		std::set<sidetype> checkSet;


		fprintf(stderr,"\nFinding Range...");
		float distMin = 1000000.0f;
		float distMax = -1000000.0f;
		for(volumetype i=0;i<_nLeavesUsed*_brickSize;i++){
			if(_weights[i]>0){
				if(distMin>_distance[i]) distMin = _distance[i];
				if(distMax<_distance[i]) distMax = _distance[i];
			}
		}
		fprintf(stderr,"\nBefore: Minimum: %f   Maximum: %f",distMin,distMax);
		if(-distMin<distMax) distMin = -distMax;
		else if(-distMin>distMax) distMax = -distMin;
		fprintf(stderr,"\nAfter: Minimum: %f   Maximum: %f",distMin,distMax);

		size_t s = queues.size()-1;
		sidetype scale=maxScale;
		while(scale>=minScale){
			fprintf(stderr,"\nScale %i: %li Leaves",scale,queues[s].size());

			if(queues[s].size()){
				size_t imagesInMemory = scale;
				if(imagesInMemory*(size_t)_n*(size_t)_n*sizeof(bgra)*2> maxMemForDistance){
					imagesInMemory = maxMemForDistance/((size_t)_n*(size_t)_n*sizeof(bgra)*2);
				}
				fprintf(stderr,"\nWill read %li Images on Block into Memory",imagesInMemory);
				fprintf(stderr,"\nLeaves for Size %i: ",scale);
				for(std::map<sidetype,std::vector<volumetype> >::iterator k=queues[s].begin();k!=queues[s].end();k++){
					fprintf(stderr," %i:%li",k->first,k->second.size());
				}
				sidetype z=0;
				std::map<sidetype,std::vector<volumetype> >::iterator it = queues[s].begin();
				while(z<_n){
					if(it->first==z){
						std::vector<volumetype> &leaves = it->second;
						fprintf(stderr,"\n%li Leaves for Z == %i",leaves.size(),z);
						for(sidetype z1=0;z1<_brickLength;z1++){
							fprintf(stderr,"\nIn-Brick-Z: %i: ",z1);
							cv::Mat slice[scale];
							cv::Mat colorslice[scale];

							for(sidetype largMemOff=0;largMemOff<scale;largMemOff+=imagesInMemory){
//								for(sidetype z2=0;z2<scale;z2++){
								fprintf(stderr,"\nReading Images from %i to %li",
										z+z1*scale+largMemOff,z+z1*scale+largMemOff+imagesInMemory);
								for(sidetype z2=largMemOff;z2<largMemOff+imagesInMemory;z2++){
									sidetype zcomb = z+z1*scale+z2;
									char disbuffer[500]; char colbuffer[500];
									sprintf(disbuffer,"%s/dis%.5i.png",prefix.c_str(),zcomb);
									slice[z2] = cv::imread(disbuffer,-1);
									sprintf(colbuffer,"%s/col%.5i.png",prefix.c_str(),zcomb);
									colorslice[z2] = cv::imread(colbuffer,-1);
	//								fprintf(stderr,"\n%s %s",disbuffer,colbuffer);
	//								fprintf(stderr," (%i %i|%i)(%i %i|%i)",
	//										slice[z2].cols,slice[z2].rows,slice[z2].channels(),
	//										colorslice[z2].cols,colorslice[z2].rows,colorslice[z2].channels());
								}
								fprintf(stderr,"\nImages read.");
								for(size_t k=0;k<leaves.size();k++){
									fprintf(stderr," %i",leaves[k]);
									const sidetype3 &o = _leafPos[leaves[k]];
									for(sidetype y1=0;y1<_brickLength;y1++){
										for(sidetype x1=0;x1<_brickLength;x1++){
											weighttype weight = _weights[leaves[k]*_brickSize+(z1*_brickLength+y1)*_brickLength+x1];
											float dist = _distance[leaves[k]*_brickSize+(z1*_brickLength+y1)*_brickLength+x1];
											colortype3 col = _color[leaves[k]*_brickSize+(z1*_brickLength+y1)*_brickLength+x1];
											bgra source;
											source.a = 255;
											bgra colorsource; colorsource.a = 255;
											if(weight==0){
												source.r = source.g = 0; source.b = 255;
												colorsource.r = colorsource.g = colorsource.b = 0;
											}
											else{
												source.b = 0;
												if(dist<0){
													source.r = 255.0f-dist/distMin*255.0f; source.g = 0;
												}
												else if(dist==0){
													source.r = source.g = 255;
												}
												else {
													source.g = 255.0f-dist/distMax*255.0f; source.r = 0;
												}
												colorsource.r = col.x/COLOR_MULTIPLICATOR; colorsource.g = col.y/COLOR_MULTIPLICATOR; colorsource.b = col.z/COLOR_MULTIPLICATOR;
											}
//											for(sidetype z2=0;z2<scale;z2++){
											for(sidetype z2=largMemOff;z2<largMemOff+imagesInMemory;z2++){
												for(sidetype y2=0;y2<scale;y2++){
													for(sidetype x2=0;x2<scale;x2++){
	//													((bgra*)slice[z3].data)[(o.y+y1*scale+y3)*_n+_n-1-o.x-x1*scale-x3] = source;
														sidetype y = o.y+y1*scale+y2;
														sidetype x = o.x+x1*scale+x2;
														long long j = (long long)y*(long long)_n+(long long)_n-1-(long long)x;
	//													fprintf(stderr," (%i %i %i):(%i %i):%lli",x3,y3,z3,x,y,j);
														if(((bgra*)slice[z2].data)[j].a==0 || weight!=0){
															((bgra*)slice[z2].data)[j] = source;
															((bgra*)colorslice[z2].data)[j] = colorsource;
														}
													}
												}
											}
										}
									}
								}
								fprintf(stderr,"\nWriting back...");
//								for(sidetype z2=0;z2<scale;z2++){
								for(sidetype z2=largMemOff;z2<largMemOff+imagesInMemory;z2++){
									sidetype zcomb = z+z1*scale+z2;
									checkSet.insert(zcomb);
									char buffer[500];
									sprintf(buffer,"%s/dis%.5i.png",prefix.c_str(),zcomb);
									cv::imwrite(buffer,slice[z2]);
									slice[z2].release();
									sprintf(buffer,"%s/col%.5i.png",prefix.c_str(),zcomb);
									cv::imwrite(buffer,colorslice[z2]);
									colorslice[z2].release();
								}
								fprintf(stderr,"done");
							}

						}
						it++;
						fprintf(stderr,"\nIterator Incremented");
					}
					else{
						fprintf(stderr,"\nNo Leaves for Z == %i",z);
					}
					z += scale*_brickLength;
				}
			}



			s--;
			scale/=2;
		}
		fprintf(stderr,"\nThe following Images have been written on:\n");
		for(std::set<sidetype>::iterator i=checkSet.begin();i!=checkSet.end();i++){
			fprintf(stderr," %i",*i);
		}
	}

	if(_meshCurrent && _meshCurrent->vertices.size()){
		fprintf(stderr,"\nDrawing Mesh Vertices into Images");

		std::map<sidetype,std::vector<size_t> > vertexmap;
		for(size_t i=0;i<_meshCurrent->vertices.size();i++){
			float zFloat = (_meshCurrent->vertices[i].z-_offset.z)/_scale;
			if(zFloat<0.0f){
				fprintf(stderr,"\nERROR: (%f - %f) / %f < 0",_meshCurrent->vertices[i].z,_offset.z,_scale);
			}
			else{
				vertexmap[(sidetype)(floor(zFloat))].push_back(i);
			}
		}

		for(std::map<sidetype,std::vector<size_t> >::iterator i=vertexmap.begin();i!=vertexmap.end();i++){
			sidetype z = i->first;
			if(imageWritten.count(z)){
				std::vector<size_t> &verts = i->second;
				fprintf(stderr,"\nImage %i",z);
				char buffer[500];
				sprintf(buffer,"%s/dis%.5i.png",prefix.c_str(),z);
				cv::Mat disImage = cv::imread(buffer,-1);

				for(size_t j=0;j<verts.size();j++){
					float xFloat = (_meshCurrent->vertices[verts[j]].x-_offset.x)/_scale;
					float yFloat = (_meshCurrent->vertices[verts[j]].y-_offset.y)/_scale;

					if(xFloat<0 || xFloat >=_n || yFloat  < 0 || yFloat >= _n){
						fprintf(stderr,"\nVertex ERROR: (%f %f) in %i",xFloat,yFloat,_n);
					}
					else{
						sidetype x = floor(xFloat);
						sidetype y = floor(yFloat);
						((bgra*)disImage.data)[y*_n+_n-1-x].r =
						((bgra*)disImage.data)[y*_n+_n-1-x].g =
						((bgra*)disImage.data)[y*_n+_n-1-x].b =
						((bgra*)disImage.data)[y*_n+_n-1-x].a = 255;
					}
				}

				cv::imwrite(buffer,disImage);
			}
			else{
				fprintf(stderr,"\nERROR: Image %i has not been written",z);
			}
		}

		fprintf(stderr,"\nMesh Vertices drawn into Images");
	}


}

void FusionMipMapCPU::saveZimagesFull()
//void FusionMipMapCPU::saveZimages(std::vector<volumetype> indices, std::string prefix)
{
	fprintf(stderr,"\nSaving Debug Z Images");

	typedef struct bgra_ {uchar b; uchar g; uchar r; uchar a;} bgra;

	std::vector<volumetype> indices(_nLeavesUsed);
	for(volumetype i=0;i<indices.size();i++) indices[i] = i;

//	std::vector<volumetype> indices(_nLeavesUsed-_nLeavesBeforeLastFrame);
//	for(volumetype i=0;i<indices.size();i++) indices[i] = i+_nLeavesBeforeLastFrame;

//	std::vector<volumetype> indices(_leafNumberSurface,_leafNumberSurface+_nLeavesQueuedSurface);

//	fprintf(stderr,"\nComputing Negatives");
//	std::vector<volumetype> indices;
//	for(volumetype i=0;i<_nLeavesUsed;i++){
//		bool queued = false;
//		for(volumetype j=0;j<_nLeavesQueuedSurface;j++){
//			if(_leafNumberSurface[j]==i){
//				queued = true;
//				break;
//			}
//		}
//		if(!queued) indices.push_back(i);
//	}
//	fprintf(stderr,"\nNegatives Computed");


	std::string prefix = "debug";
//	sidetype maxScale = 2;
	sidetype maxScale = _n/2/_brickLength;

	fprintf(stderr,"\nFilling Leaf Maps ordered by Z-Offset...");
	std::vector<std::map<sidetype,std::vector<volumetype> > > queues;
	for(sidetype s=1;s<=maxScale;s*=2){
		fprintf(stderr,"\nScale %i:",s);
		queues.push_back(std::map<sidetype,std::vector<volumetype> >());
		for(size_t i=0;i<indices.size();i++){
//			fprintf(stderr," %li->%i : %i",i,indices[i],_leafScale[indices[i]]);
			if(_leafScale[indices[i]]==s)
			queues.back()[_leafPos[indices[i]].z].push_back(indices[i]);
		}
	}
	fprintf(stderr,"\nLeaf Map filled");
	fprintf(stderr,"\nLeaves on different scales:\n");
	size_t k=0;
	for(sidetype s=1;s<=maxScale;s*=2){
		fprintf(stderr," %i->%li",s,queues[k].size());
		k++;
	}

	if(maxScale<=1){

	}
	else{

		if(_n>4096){
			fprintf(stderr,"\nWe do not have enough memory to store %i*%i*%i !",_n,_n,_n);
			return;
		}

		fprintf(stderr,"\nAllocating and Initializing Memory");
		long long hugeSize = (long long)_n;
		hugeSize *= hugeSize*hugeSize;
		char *memory = new char[hugeSize];
		for(long long i=0;i<hugeSize;i++) memory[i] = -128;
		fprintf(stderr,"\nMemory Initialized");

		fprintf(stderr,"\nFinding Range...");
		float distMin = 1000000.0f;
		float distMax = -1000000.0f;
		for(volumetype i=0;i<_nLeavesUsed*_brickSize;i++){
			if(_weights[i]>0){
				if(distMin>_distance[i]) distMin = _distance[i];
				if(distMax<_distance[i]) distMax = _distance[i];
			}
		}
		fprintf(stderr,"\nBefore: Minimum: %f   Maximum: %f",distMin,distMax);
		if(-distMin<distMax) distMin = -distMax;
		else if(-distMin>distMax) distMax = -distMin;
		fprintf(stderr,"\nAfter: Minimum: %f   Maximum: %f",distMin,distMax);

		size_t s = queues.size()-1;
		sidetype scale=maxScale;
		while(scale>=1){
			fprintf(stderr,"\nScale %i: %li Leaves",scale,queues[s].size());

			if(queues[s].size()){
				fprintf(stderr,"\nLeaves for Size %i: ",scale);
				for(std::map<sidetype,std::vector<volumetype> >::iterator k=queues[s].begin();k!=queues[s].end();k++){
					fprintf(stderr,"\n%i:%li",k->first,k->second.size());
					std::vector<volumetype> &leaves = k->second;
					for(size_t test=0;test<leaves.size();test++){
						fprintf(stderr," (%i)[%i %i %i]:%i",leaves[test],_leafPos[leaves[test]].x,_leafPos[leaves[test]].y,_leafPos[leaves[test]].z,_leafScale[leaves[test]]);
					}
					for(size_t l=0;l<leaves.size();l++){
						const sidetype3 &o = _leafPos[leaves[l]];
//						fprintf(stderr," [%i %i %i]",o.x,o.y,o.z);
						volumetype start = leaves[l]*_brickSize;
						for(sidetype z1=0;z1<_brickLength;z1++){
							for(sidetype y1=0;y1<_brickLength;y1++){
								for(sidetype x1=0;x1<_brickLength;x1++){
									volumetype index = start+(z1*_brickLength+y1)*_brickLength+x1;
									weighttype weight = _weights[index];
									float dist = _distance[index];
									char source = -127;
									if(weight>0) source = (char)(dist/distMax*125.0f);
//									fprintf(stderr,"W");
									for(sidetype z2=0;z2<scale;z2++){
										for(sidetype y2=0;y2<scale;y2++){
											for(sidetype x2=0;x2<scale;x2++){
												sidetype z = o.z+z1*scale+z2;
												sidetype y = o.y+y1*scale+y2;
												sidetype x = o.x+x1*scale+x2;
												long long j = ((long long)z*(long long)_n+(long long)y)*(long long)_n+(long long)_n-1-(long long)x;
//												fprintf(stderr," (%i %i %i):%lli",z,y,x,j);
												memory[j] = source;
//												fprintf(stderr,".");
											}
										}
									}
//									fprintf(stderr,"!");
								}
							}
						}
					}
				}

			}
			s--;
			scale/=2;
		}


		if(_meshCurrent && _meshCurrent->vertices.size()){
			fprintf(stderr,"\nDrawing Mesh Vertices into Images");
			for(size_t i=0;i<_meshCurrent->vertices.size();i++){
				float xFloat = (_meshCurrent->vertices[i].x-_offset.x)/_scale;
				float yFloat = (_meshCurrent->vertices[i].y-_offset.y)/_scale;
				float zFloat = (_meshCurrent->vertices[i].z-_offset.z)/_scale;
				if(xFloat<0 || xFloat >=_n || yFloat  < 0 || yFloat >= _n || zFloat  < 0 || zFloat >= _n){
					fprintf(stderr,"\nVertex ERROR: (%f %f %f) in %i",xFloat,yFloat,zFloat,_n);
				}
				else{
					sidetype x = floor(xFloat);
					sidetype y = floor(yFloat);
					sidetype z = floor(zFloat);
					long long j = (((long long)_n-1-(long long)z)*(long long)_n+(long long)y)*(long long)_n+(long long)_n-1-(long long)x;
					memory[j] = 127;
				}
			}

		}


		fprintf(stderr,"\nWriting Images...");
		cv::Mat output(_n,_n,CV_8UC4);
		bgra *data = (bgra*)output.data;
		bgra noleaf, invalid;
		noleaf.a = noleaf.r = noleaf.g = noleaf.b = 0;
		invalid.r = invalid.g = 0; invalid.b = invalid.a = 255;
		bgra vertexcolor;
		vertexcolor.a = vertexcolor.r = vertexcolor.g = vertexcolor.b = 255;
		long long slicemem = _n*_n;
		for(long long z=0;z<_n;z++){
			fprintf(stderr,"\n%lli",z);
			bool somethinghere = false;
			for(long long p=0;p<slicemem;p++){
				somethinghere |= memory[z*slicemem+p]!= -128;
			}
			if(somethinghere){
				fprintf(stderr,"!");
				for(long long p=0;p<slicemem;p++){

					data[p] = noleaf;
					if(memory[z*slicemem+p] != -128){
						if(memory[z*slicemem+p] == -127){
							data[p] = invalid;
						}
						else if(memory[z*slicemem+p] == 127){
							fprintf(stderr," v(%lli %lli)",p/_n,p%_n);
							data[p] = vertexcolor;
						}
						else{
							data[p].a =255; data[p].b = data[p].r = data[p].g = 0;
							if(memory[z*slicemem+p]<0){
								data[p].r = 255 + 2*memory[z*slicemem+p];
							}
							else{
								data[p].g = 255 - 2*memory[z*slicemem+p];
							}
						}
					}


				}
				char buffer[500];
				sprintf(buffer,"%s/full%.5lli.png",prefix.c_str(),z);
				cv::imwrite(buffer,output);
			}
		}
	}

}








