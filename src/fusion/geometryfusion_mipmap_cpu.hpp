/*
 * geometry_mipmap_cpu.hpp
 *
 *  Created on: Apr 14, 2013
 *      Author: steinbrf
 */

#ifndef GEOMETRY_MIPMAP_CPU_HPP_
#define GEOMETRY_MIPMAP_CPU_HPP_

#include <opencv2/opencv.hpp>
#include <camerautils/camerautils.hpp>
#include "definitions.h"
#include "geometryfusion_aos.hpp"
#include "treeandbrick.hpp"
#include "treeandbrick_incremental.hpp"
#include <list>

#include <boost/thread.hpp>


typedef class FloatVertex_
{
public:
	FloatVertex_(float px, float py, float pz);
	float x;
	float y;
	float z;
} FloatVertex;

typedef class ByteColor_
{
public:
	ByteColor_(uchar pr, uchar pg, uchar pb);
	uchar r;
	uchar g;
	uchar b;
} ByteColor;

typedef class InterleavedMesh_
{
public:
	std::vector<Vertex3f> vertices;
	std::vector<Color3b> colors;
	std::vector<uint32_t> indices;
} InterleavedMesh;

typedef class InterleavedMeshCell_
{
public:
	uint32_t index;
	InterleavedMesh mesh;
} InterleavedMeshCell;

typedef class CellUpdate_
{
public:
	std::vector<InterleavedMeshCell> cells;
} CellUpdate;


//#define DEBUG_NO_LEAFCELLS

#define SEPARATE_MESHCELL_STRUCTURE

//#define LAPPY_MEMORY 200000000
//#define LAPPY_MEMORY 300000000 //Wirkliches Lappy Memory
//#define LAPPY_MEMORY 500000000
#define LAPPY_MEMORY 2000000000
//#define LAPPY_MEMORY 3000000000
#define RESERVE_VECTOR_MEMORY
//#define USE_OWN_VECTOR_CLASSES
#define INTERACTIVE_MEMORY_MANAGEMENT false

inline std::string getNameD(size_t p){
	char buffer[500];
	sprintf(buffer,"log/depth%.05li.png",p);
	return std::string(buffer);
}
inline std::string getNameR(size_t p){
	char buffer[500];
	sprintf(buffer,"log/rgb%.05li_r.png",p);
	return std::string(buffer);
}
inline std::string getNameG(size_t p){
	char buffer[500];
	sprintf(buffer,"log/rgb%.05li_g.png",p);
	return std::string(buffer);
}
inline std::string getNameB(size_t p){
	char buffer[500];
	sprintf(buffer,"log/rgb%.05li_b.png",p);
	return std::string(buffer);
}
inline std::string getNameRGB(size_t p){
	char buffer[500];
	sprintf(buffer,"log/rgb%.05li.png",p);
	return std::string(buffer);
}

typedef struct FrameStatistic_{
	double timeTraversal;
	double timeSDFUpdate;
	volumetype newBranches;
	volumetype newLeaves;
	size_t newMeshCells;
	size_t leavesQueued;
} FrameStatistic;


class FusionMipMapCPU : public Fusion_AoS
{
public:
	FusionMipMapCPU(float offsetX, float offsetY, float offsetZ,
			float scale, float distanceThreshold,
			sidetype n = 0, bool color = true);
	FusionMipMapCPU(FusionMipMapCPU *fusion, volumetype nLeavesTotalLoop);
	~FusionMipMapCPU();
	//Adding single images
	int addMap(cv::Mat &depth, CameraInfo caminfo,
			std::vector<cv::Mat> rgb = std::vector<cv::Mat>(3));
	int addMap(const cv::Mat &depth, CameraInfo caminfo, const cv::Mat &rgb,
			float scaling, float maxcamdistance);
	//Adding multiple images up to the next keyframe
	std::vector<int> addMap(std::vector<cv::Mat> depthImages, std::vector<CameraInfo> trajectories,
			std::vector<std::vector<cv::Mat> > rgbImages = std::vector<std::vector<cv::Mat> >(),
			volatile long int *poseNumber = NULL);
	std::vector<int> addMap(std::vector<cv::Mat> depthImages, std::vector<CameraInfo> trajectories,
			std::vector<cv::Mat> rgbImages, volatile long int *poseNumber = NULL);

	MeshSeparate getMeshSeparateMarchingCubes(MeshSeparate mesh = MeshSeparate(3));
	MeshInterleaved getMeshInterleavedMarchingCubes(MeshInterleaved mesh = MeshInterleaved(3));
//	MeshSeparate getMeshMarchingCubesNonindexed(MeshSeparate mesh = MeshSeparate(3));
	MeshSeparate getMeshRecursive(MeshSeparate mesh = MeshSeparate(3));
//	MeshSeparate getMeshRecursiveIncremental(MeshSeparate mesh = MeshSeparate(3));
//	MeshSeparate getMeshMarchingCubesSlowOpenMP(MeshSeparate mesh);
	MeshSeparate getMeshStructural(unsigned int structureType = 0,MeshSeparate mesh = MeshSeparate(4));
//	MeshSeparate getMeshMarchingCubesApproximate(MeshSeparate mesh = MeshSeparate(3));
	CellUpdate &getMeshCellsUpdate();
	bool updateMeshes();
	void updateMeshCellStructure();
	void beforeUpdateMeshCellStructure();
	void afterUpdateMeshCellStructure();
	void pushMeshCellQueue();
	void pushLeafQueueForMeshing();

	void queryNeighborsWithoutMeshCells();

	void updateArraysForLoopClosure(FusionMipMapCPU *fusion);
	void startLoopClosure();
	void catchLoopClosure();


	void deleteOldLoopPoses();
	void addNewLoopPoses();

	void saveZimages();
	void saveZimagesFull();
//	void saveZimages(std::vector<volumetype> indices, std::string prefix);

	void setThreadMeshing(bool threadMeshing);
	void setDepthChecks(int depthchecks);
	void setIncrementalMeshing(bool incrementalMeshing);

	typedef struct MeshStatistic_ {unsigned int frameNumber; size_t queueSize; size_t arraySize; double timeUpdate; double timeSum;
	MeshStatistic_(unsigned int frameNumber_p, size_t queueSize_p, size_t arraySize_p,  double timeUpdate_p, double timeSum_p)
	:frameNumber(frameNumber_p), queueSize(queueSize_p), arraySize(arraySize_p), timeUpdate(timeUpdate_p),timeSum(timeSum_p){}} MeshStatistic;

	class OwnBranchIndicesArray_{
	public:
		OwnBranchIndicesArray_();
		~OwnBranchIndicesArray_();
#ifdef BRANCHNEIGHBORHOOD_REFERECE
		MCNSplit* &operator[](size_t pos);
		void resize(size_t count,MCNSplit* value = NULL);
		void push_back(MCNSplit* const &value);
#else
		MCNSplit &operator[](size_t pos);
		void resize(size_t count,MCNSplit value = MCNSplit());
		void push_back(const MCNSplit &value);
#endif
		size_t size() const;
		size_t capacity() const;
		void reserve(size_t count);
		void clear();
	protected:
#ifdef BRANCHNEIGHBORHOOD_REFERECE
		MCNSplit **_array;
		MCNSplit *_dummy;
#else
		MCNSplit *_array;
		MCNSplit _dummy;
#endif
		size_t _size;
		size_t _capacity;
	};

	class OwnBranchIndicesCompactArray_{
	public:
		OwnBranchIndicesCompactArray_();
		~OwnBranchIndicesCompactArray_();
		MCNCompact &operator[](size_t pos);
		size_t size() const;
		size_t capacity() const;
		void resize(size_t count,MCNCompact value = MCNCompact());
		void reserve(size_t count);
		void clear();
		void push_back(const MCNCompact &value);
	protected:
		MCNCompact *_array;
		size_t _size;
		size_t _capacity;
		MCNCompact _dummy;
	};


	typedef std::list<size_t> LeafNeighborhood;

	class OwnLeafIndicesArray_{
	public:
		OwnLeafIndicesArray_();
		~OwnLeafIndicesArray_();
		LeafNeighborhood &operator[](size_t pos);
		size_t size() const;
		size_t capacity() const;
		void resize(size_t count,LeafNeighborhood value = LeafNeighborhood());
		void reserve(size_t count);
		void clear();
		void push_back(const LeafNeighborhood &value);
		LeafNeighborhood &back();
	protected:
		LeafNeighborhood *_array;
		size_t _size;
		size_t _capacity;
		LeafNeighborhood _dummy;
	};

	class OwnMeshCellArray_{
	public:
		OwnMeshCellArray_();
		~OwnMeshCellArray_();
		MeshCell &operator[](size_t pos);
		inline size_t size() const{return _size;}
		inline size_t capacity() const{return _capacity;}
		void resize(size_t count, MeshCell value = MeshCell());
		void reserve(size_t count);
		void clear();
		void push_back(const MeshCell &value);
		MeshCell &back();
		OwnMeshCellArray_ &operator=(const OwnMeshCellArray_ &value);
	protected:
		MeshCell *_array;
		size_t _size;
		size_t _capacity;
		MeshCell _dummy;
	};


#ifdef USE_OWN_VECTOR_CLASSES
	typedef OwnMeshCellArray_ MeshCellArray;
	typedef OwnBranchIndicesArray_ BranchIndicesArray;
	typedef OwnBranchIndicesCompactArray_ BranchIndicesCompactArray;
	typedef OwnLeafIndicesArray_ LeafIndicesArray;
#else
	typedef std::vector<MeshCell> MeshCellArray;
#ifdef BRANCHNEIGHBORHOOD_REFERECE
	typedef std::vector<MCNSplit*> BranchIndicesArray;
#else
	typedef std::vector<MCNSplit> BranchIndicesArray;
#endif
	typedef std::vector<MCNCompact> BranchIndicesCompactArray;
	typedef std::vector<LeafNeighborhood> LeafIndicesArray;
#endif










protected:

  void queryPointDepthSingle(sidetype px, sidetype py, sidetype pz, sidetype brickLengthTarget);
  void queryBoxDepthSingle(sidetype3 minPos, sidetype3 maxPos, sidetype brickLengthTarget);

  void queryBoxDepth1Negative(sidetype3 minPos, sidetype3 maxPos,
  		sidetype3 n1MinPos, sidetype3 n1MaxPos, sidetype brickLengthTarget);
  void queryBoxDepth2Negative(sidetype3 minPos, sidetype3 maxPos,
  		sidetype3 n1MinPos, sidetype3 n1MaxPos,sidetype3 n2MinPos, sidetype3 n2MaxPos, sidetype brickLengthTarget);


	bool allocateMemoryCPU();
	void freeMemoryCPU();

	std::string getDescription();
	bool copyCPU2HDD();
	bool copyHDD2CPU();
	bool grow();
	bool split();
	bool setInitialVolume(int minX, int minY, int minZ, int maxX, int maxY, int maxZ);




  sidetype _bandwidth;
  sidetype _brickLength;
  sidetype _brickSize;

  volumetype _nBranchesTotal;
  volumetype _nLeavesTotal;

  volumetype _nBranchesUsed;
  volumetype _nLeavesUsed;
  volumetype _nLeavesQueuedSurface;
  volumetype _nLeavesQueuedFrustum;
	int3 _boxMin;
	int3 _boxMax;

  volumetype *_tree;
  volumetype *_queueIndexOfLeaf;
  volumetype *_leafNumberSurface;
  volumetype *_leafNumberFrustum;
  sidetype3  *_leafPos;
  sidetype   *_leafScale;

  float      *_distance;
  weighttype *_weights;
  colortype3 *_color;

  volumetype *_branchNumber;
  uchar *_child;

  volumetype _nLeavesBeforeLastFrame;

	sidetype3 *_sharedBoxes;
  bool _threaded;
  bool _threadValid;


	int _imageWidth;
	int _imageHeight;
	unsigned int _deviceNumber;

	float *_pxp;
	float *_pyp;


	double _avgTimeQueueSurface;
	double _avgTimeQueueFrustum;
	double _avgTimeBricksSurface;
	double _avgTimeBricksFrustum;
	double _avgTimeMesh;
	double _avgTimeSumMesh;
	bool _verbose;

	volumetype _averageLeaves;


	bool _performIncrementalMeshing;


	class BudsAnchor{
	public:
		BudsAnchor(){
//			fprintf(stderr,"\nCreating SubtreeAnchor");
		subtreeBuds=NULL;subtreeBudsParentLeaf=NULL;leafBuds=NULL;}
		std::vector<volumetype> *subtreeBuds;
		std::vector<volumetype> *subtreeBudsParentLeaf;
		std::vector<volumetype> *leafBuds;
	};

	friend void transformLoopSimPrecalculatedNeg_subtree
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
			,FusionMipMapCPU::BudsAnchor &_newSubtreesSinceMeshingToQueue
			,volumetype &_numberOfQueuedSubtrees
			,volumetype _treeSizeSinceMeshing
	);
	friend void transformLoopSimPrecalculatedNeg_subtree
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
			,FusionMipMapCPU::BudsAnchor &_newSubtreesSinceMeshingToQueue
			,volumetype &_numberOfQueuedSubtrees
			,volumetype _treeSizeSinceMeshing
	);
	friend void queryPointDepthSingle_func_subtree
	(
			sidetype x, sidetype y, sidetype z, sidetype brickLengthTarget,
			const sidetype &_n, const sidetype &_brickLength, volumetype *_tree,
			volumetype &_nBranchesUsed,
			const volumetype &_nLeavesTotal, volumetype &_nLeavesUsed, volumetype &_nLeavesQueued,
			volumetype *_leafNumber, sidetype3 *_leafPos, sidetype *_leafScale,
			volumetype *_queueIndexOfLeaf
			,ParentArray &_leafParent
			,FusionMipMapCPU::BudsAnchor &_newSubtreesSinceMeshingToQueue
			,volumetype &_numberOfQueuedSubtrees
			,volumetype _treeSizeSinceMeshing);

	BudsAnchor _newBudsSinceMeshingToQueue;
	BudsAnchor _newBudsSinceMeshingToAccumulate;
	BudsAnchor _newBudsSinceMeshingToClear;

//	volumetype *_newSubtreesSinceMeshingToQueue;

//	volumetype *_newSubtreesSinceMeshingToProcess;
//	uchar *_newSubtreesSinceMeshingLevelsToQueue;
//	uchar *_newSubtreesSinceMeshingLevelsToProcess;
	volumetype _numberOfQueuedTreeBuds;
	volumetype _numberOfQueuedLeafBuds;
	volumetype _treeSizeSinceMeshing;
	volumetype _treeSizeForMeshing;
	bool _differentImageSize;
	std::vector<bool> _leafIsQueuedForMeshing;
	std::deque<volumetype> _leafQueueForMeshing;

	std::vector<bool> *_meshCellIsQueuedCurrent;
	std::vector<bool> *_meshCellIsQueuedNext;
	std::list<size_t> _meshCellQueueOld;
	std::list<size_t> _meshCellQueueCurrent;
	std::list<size_t> _meshCellQueueNext;

	MeshCellArray _meshCellsSplit;
	MeshCellArray _meshCellsCompact;

	MeshCellArray &_meshCells;
	//TODO: Debug
	BranchIndicesArray _meshCellIndicesBranch;
	BranchIndicesCompactArray _meshCellIndicesBranchCompact;
	LeafIndicesArray _meshCellIndicesLeaf;
	ParentArray _leafParent;

	bool _interactiveMemoryManagement;

//	std::vector<MeshCell> _meshCells;
//	std::vector<MeshCellNeighborhood> _meshCellIndicesBranch;
//	std::vector<LeafNeighborhood > _meshCellIndicesLeaf;
//	std::vector<volumetype> _leafParent;

	MCNSplit _boundary;
	MCNCompact _boundaryCompact;

	boost::thread *_meshThread;
	boost::thread *_updateMessageThread;
	bool _threadMeshing;
	unsigned int _degenerate_faces;
	MarchingCubesIndexed _mc;
	treeinfo _treeinfo;
	MeshCellArray _meshCellsCopy;
	ParentArray _leafParentCopy;
	int _meshingDone;
	MeshSeparate *_meshSeparateCurrent;
	MeshSeparate *_meshSeparateNext;
	MeshInterleaved *_meshCurrent;
	MeshInterleaved *_meshNext;

	CellUpdate *_updateCurrent;
	CellUpdate *_updateNext;

	//Functions Variables for Loop Closure


	bool _loopClosureLogging;
	bool _loopClosureEnabled;
	bool _loopClosureDone;
	unsigned int _maxPosesPerLoop;
	std::vector<CameraInfo> _posesFused;
	std::vector<CameraInfo> _posesLoop;
	std::vector<std::string> _loggedImages;
	std::vector<std::vector<volumetype> > _leafQueues;
	volumetype _nLeavesLoopTotal;
	volumetype _nLeavesLoopUsed;
	FusionMipMapCPU *_loopClosureFusion;
	std::multimap<float,size_t> _posemap;
	boost::thread *_imageSaveThread;
	boost::thread *_loopClosureThread;


	bool _loopClosureMode;
	float *_weightsLoop;
	volumetype *_leafMapLoop;
	volumetype _nLeavesMapped;
	std::vector<size_t> _meshCellsChangedLoop;
	short *_numCamsPerLeaf;

	//Logged Images
	std::list<std::pair<cv::Mat,size_t> > _depthImageBuffer;
	std::vector<std::list<std::pair<cv::Mat,size_t> >::iterator> _depthImageReferences;
	std::list<std::pair<std::vector<cv::Mat>,size_t> > _rgbImageBuffer;
	std::vector<std::list<std::pair<std::vector<cv::Mat>,size_t> >::iterator> _rgbImageReferences;
	size_t _maxImageBufferSize;

	//Logging
	bool _loggingEnabled;
	std::vector<FrameStatistic> _frameStatistics;
	unsigned int _meshingStartFrame;
	double _meshTime;
	std::vector<MeshStatistic> _meshTimes;


	//Backwards Depth Consistency Check
	size_t _numCheckImages;
	std::list<cv::Mat> _lastDepthImages;
	std::list<CameraInfo> _lastCameraInfos;

};




#endif /* GEOMETRY_MIPMAP_CPU_HPP_ */
