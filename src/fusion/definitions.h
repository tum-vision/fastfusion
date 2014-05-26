/*
 * definitions.h
 *
 *  Created on: Feb 27, 2013
 *      Author: steinbrf
 */

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_


#define UNENDLICH std::numeric_limits<float>::infinity()

//#define CRAPPY_LAPPY


#define READ_DISTANCE_PER_BRICK

//############################### CUDA ##############################

#define TEXTUREOFFSET 0.5f
#define FU_BW 16
#define FU_BH 16
#define FU_BS 256

//########################## Data Types ####################################


#ifndef uint
typedef unsigned int uint;
#endif

#ifndef uchar
typedef unsigned char uchar;
#endif

#ifndef weighttype
typedef float weighttype;
#endif

#ifndef sidetype
typedef unsigned short sidetype;
#endif

#ifndef volumetype
typedef unsigned int volumetype;
#define VOLUMETYPE_MAX 4294967295
#endif

#ifndef uchar
typedef unsigned char uchar;
#endif

#ifndef colortype
typedef unsigned short colortype;
#define COLOR_MULTIPLICATOR 256
#endif

#ifndef sidetype3
typedef struct sidetype3_{ sidetype x; sidetype y; sidetype z;} sidetype3;
#endif
//#ifndef sidetype4
//typedef struct sidetype4_{ sidetype x; sidetype y; sidetype z; sidetype s;} sidetype4;
//#endif

#ifndef colortype3
typedef struct colortype3_{ colortype x; colortype y; colortype z;} colortype3;
#endif

#ifndef camPamsFloat
typedef struct camPamsFloat_{
	camPamsFloat_(	float pr11, float pr12, float pr13,
	float pr21, float pr22, float pr23,
	float pr31, float pr32, float pr33,
	float pt1, float pt2, float pt3,
	float pfx, float pfy, float pcx, float pcy){
		r11 = pr11; r12 = pr12; r13 = pr13;
		r21 = pr21; r22 = pr22; r23 = pr23;
		r31 = pr31; r32 = pr32; r33 = pr33;
		t1 = pt1; t2 = pt2; t3 = pt3;
		fx = pfx; fy = pfy; cx = pcx; cy = pcy;
	}
	float r11; float r12; float r13;
	float r21; float r22; float r23;
	float r31; float r32; float r33;
	float t1; float t2; float t3;
	float fx; float fy; float cx; float cy;
} camPamsFloat;
#endif

#ifndef __VECTOR_TYPES_H__
typedef struct float3_ { float x; float y; float z;} float3;
#endif
#ifndef __VECTOR_FUNCTIONS_H__
inline float3 make_float3(float px, float py, float pz){
	float3 result; result.x = px; result.y = py; result.z = pz;
	return result;}
#endif

#ifndef __VECTOR_TYPES_H__
typedef struct int3_ { int x; int y; int z;} int3;
#endif


//############################  CPU ######################

//#define PREPROCESS_IMAGES

#define USE_OPENMP_IMAGEBUFFERING
//#define USE_OPENMP_TRAVERSAL
#define USE_OPENMP_DISTANCEUPDATE
#define USE_OPENMP_MARCHINGCUBES
#define USE_OPENMP_MARCHINGCUBESSLOW
//#define OPENMP_DYNAMIC
#define OPENMP_THREADS 8

#define USE_SSE

#define USE_THREADS false
#define THREAD_MESHING true

//############################  Loop Closure ######################

#define MAX_LOOP_CLOSURE_LEAVES 100000
#define MAX_NUMBER_POSES_PER_LOOP 50

//########################## Mutex #############################

#define MUTEXVALUE 4294967295
#define BRANCHINIT 4294967294
#define DEADBRANCH 4294967293
#define MAXCELLINDEX 343597383520
//#define MUTEXVALUE 1

#define MAXDEPTH 20


//########################### Tree ####################################

//#define SIDELENGTH 64
//#define MAXLEAFNUMBER 512 // 8^3
//#define MAXINDEX 262144 //MAXLEAFNUMBER*BRICKLENGTH^3

//#define SIDELENGTH 128
//#define MAXLEAFNUMBER 4096 // 8^4
//#define MAXINDEX 2097152 //MAXLEAFNUMBER*BRICKLENGTH^3

//#define SIDELENGTH 256
//#define MAXLEAFNUMBER 32768 // 8^5
//#define MAXINDEX 16777216 //MAXLEAFNUMBER*BRICKLENGTH^3

//#define SIDELENGTH 512
//#define MAXLEAFNUMBER 262144 // 8^6
//#define MAXINDEX 134217728 //MAXLEAFNUMBER*BRICKLENGTH^3

//#define SIDELENGTH 1024
//#define MAXLEAFNUMBER 2097152 // 8^7
//#define MAXINDEX 1073741824 //MAXLEAFNUMBER*BRICKLENGTH^3

//#define SIDELENGTH 2048
////#define MAXLEAFNUMBER 16777216 // 8^8
//#define MAXLEAFNUMBER 400000 // 8^8

#define SIDELENGTH 1024

//GTX 580
#define MAXLEAFNUMBER 10000000
//#define MAXINDEX 5120000000
#define MAXINDEX VOLUMETYPE_MAX
//#define CHECK_MEMORY

//For Tesla
//#define MAXLEAFNUMBER 890000
//#define MAXINDEX 455680000




//#define RETRAVERSE_OUTLIERS

//########################### Distance and Weighting Functions ###############

//#define DEBUG_NO_MESHES

#define MESHCELLINDICES_SPLIT
//#define MESHCELLINDICES_COMPACT

#ifdef MESHCELLINDICES_SPLIT
#define BRANCHNEIGHBORHOOD_REFERECE
#endif

#if defined MESHCELLINDICES_SPLIT && not defined MESHCELLINDICES_COMPACT
#define SWITCH_MESHCELLS_SPLIT
#endif
#if not defined MESHCELLINDICES_SPLIT && defined MESHCELLINDICES_COMPACT
#undef SWITCH_MESHCELLS_SPLIT
#endif
#if defined MESHCELLINDICES_SPLIT && defined MESHCELLINDICES_COMPACT
//#define SWITCH_MESHCELLS_SPLIT
#endif

#define MAXCAMDISTANCE 10.0
#define MIN_WEIGHT_FOR_SURFACE 0.0
#define ADD_WEIGHTS_TRANSITION_140424
//#define MAXCAMDISTANCE 6.0
//#define MIN_WEIGHT_FOR_SURFACE 0.0
#define FRUSTUM_FAR 1.0

#define REFERENCE_DEPTH 1.0f
//#define SCALE_VARIANCE
//#define SCALE_DEVIATION
#define SCALE_LINEAR
//#define SCALE_CONSTANT

//#define DISTANCE_POINT_FAST
#define DISTANCE_POINT_TRUE
//#define DISTANCE_PLANE

//#define DISTANCE_TEST
//#define DISTANCE_NORMAL
#define DISTANCE_TRUNCATED

#define ROUND_PROJECTED_POSITIONS_GPU


//#define DISTANCETHRESHOLD 0.5f // stable
//#define DISTANCETHRESHOLD 0.05f // current
//#define DISTANCETHRESHOLD 0.02f // current
#define DISTANCETHRESHOLD 0.01f // test
//#define DISTANCETHRESHOLD 0.002f // test
//#define DISTANCETHRESHOLD 1.0f

#define BANDWIDTHFACTOR 1.0
#define BRICKLENGTH 8


//#define BANDWIDTH 10

//#define DEFAULT_SCALE 0.05
//#define DEFAULT_SCALE 0.01
#define DEFAULT_SCALE 0.005

//#define WEIGHT_ONE
//#define WEIGHT_CONSTANT
//#define WEIGHT_CONSTANT_NARROW
#define WEIGHT_LINEAR
//#define WEIGHT_LINEAR_NARROW
//#define WEIGHT_GAUSS
//#define WEIGHT_GAUSS_NARROW
#define DISTANCEWEIGHTSIGMA 0.05f
#define DISTANCEWEIGHTEPSILON 0.005f
#define DISTANCEMINEXPWEIGHT 0.000001f

#define WEIGHT_FACTOR 3.0f

//float cutoff = expf(-distanceWeightSigma*(threshold-distanceWeightEpsilon)*(threshold-distanceWeightEpsilon));
//return (float)(distance<distanceWeightEpsilon) +
//		max((expf(-distanceWeightSigma*(distance-distanceWeightEpsilon)*(distance-distanceWeightEpsilon))-cutoff)/(1.0f-cutoff),DISTANCEMINEXPWEIGHT)
//		*(float)(distance>=distanceWeightEpsilon && distance<threshold);

#define QUEUEALLLEAVESINFRUSTRUM false

//#define NO_MEMORYCHECK

#define ARRAY_OF_STRUCTS

#define NEIGHBORS_USED 0

#define USE_MULTISCALE true


#define LEAVES_LESS 4000


#define CHILDBRANCHSIZEFUNCTION 16
//#define CHILDBRANCHSIZEFUNCTION (8 + ((half>>1)>=_brickLength)*8)

//#define SEQUENCE
//#define TEDDY1
//#define TEDDY3
//#define FRANKFACE
//#define FRANK
//#define DESK
//#define SYNTHETIC
//#define FLIGHT1
#define OFFICE
//#define ROOM1
//#define GARCHING1
//#define DEFAULTSCENE


#if defined (FRANK) || defined (FRANKFACE)
#define DEPTHSCALE 10000.0f
#else
#if defined (FLIGHT1)
#define DEPTHSCALE 2617.80
#else
#if defined (GARCHING1)
#define DEPTHSCALE 1000.0f
#else
#if defined (SEQUENCE)
#define DEPTHSCALE 5000.0f
#else
#define DEPTHSCALE 5000.0f
#endif
#endif
#endif
#endif

//#ifndef DEBUG_OUTPUT_DEFINED
//#define DEBUG_OUTPUT_DEFINED
//#ifdef PRINT_DEBUG
//#define DEBUG(X) (X)
//#else
//#define DEBUG(X)
//#endif
//#endif

#ifndef DEBUG_OUTPUT_DEFINED
#define DEBUG_OUTPUT_DEFINED
#ifdef PRINT_DEBUG
//#define WHERESTR  "[file %s, line %d]: "
//#define WHEREARG  __FILE__, __LINE__
//#define DEBUGPRINT2(...)       fprintf(stderr, __VA_ARGS__)
//#define DEBUGPRINT(_fmt, ...)  DEBUGPRINT2(WHERESTR _fmt, WHEREARG, __VA_ARGS__)
#define eprintf( ...)  fprintf(stderr, __VA_ARGS__)
#else
#define eprintf(...)
#endif
#endif

#define DEBUG bla


//#define IMAGEINFINITE 1000000.0f
//#define FILTERMODE cudaFilterModePoint
//#define FUSIONHEIGHTFINITE (fabs(distance) < IMAGEINFINITE)
//#define TRAVERSALHEIGHTFINITE (pz < IMAGEINFINITE)

#define IMAGEINFINITE std::numeric_limits<float>::quiet_NaN()
#define FILTERMODE cudaFilterModePoint
#define FUSIONHEIGHTFINITECPU (std::isfinite(distance))
#define FUSIONHEIGHTFINITEGPU (isfinite(distance))
#define TRAVERSALHEIGHTFINITECPU (std::isfinite(pz))
#define TRAVERSALHEIGHTFINITEGPU (isfinite(pz))

#ifdef OWNAVX
#define ALIGNED __attribute__ ((aligned (32)))
#else
#define ALIGNED __attribute__ ((aligned (16)))
#endif

#endif /* DEFINITIONS_H_ */
