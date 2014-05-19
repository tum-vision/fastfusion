/*
 * treeandbrick.hpp
 *
 *  Created on: Mar 1, 2013
 *      Author: steinbrf
 */

#ifndef TREEANDBRICK_HPP_
#define TREEANDBRICK_HPP_

#include "mesh.hpp"
#include <fusion/definitions.h>


typedef std::vector<volumetype> leafstack;



typedef struct treeinfo_ {
	treeinfo_(MeshSeparate *pmesh,sidetype pbrickLength, sidetype pbrickSize, float pminWeight,
			float3 poffset, float pscale, unsigned int *pdegenerate_faces,
			volumetype pmaxBranches, volumetype pmaxLeaves,const volumetype *ptree,
			const sidetype3 *pleafPos, const sidetype *pleafScale,
			const float *pdistance, const weighttype *pweights, const colortype3 *pcolor)
	: mesh(pmesh), brickLength(pbrickLength), brickSize(pbrickSize), minWeight(pminWeight),
	  offset(poffset), scale(pscale), degenerate_faces(pdegenerate_faces),
	  maxBranches(pmaxBranches), maxLeaves(pmaxLeaves), tree(ptree),
	  leafPos(pleafPos), leafScale(pleafScale), distance(pdistance),
	  weights(pweights), color(pcolor){}
	MeshSeparate *mesh;
	sidetype brickLength;
	sidetype brickSize;
	float minWeight;
	float3 offset;
	float scale;
	unsigned int *degenerate_faces;
	volumetype maxBranches;
	volumetype maxLeaves;
	const volumetype *tree;
	const sidetype3 *leafPos;
	const sidetype *leafScale;
	const float *distance;
	const weighttype *weights;
	const colortype3 *color;
} treeinfo;


#include "treeandbrick_incremental.hpp"

void shiftBranches
(
		volumetype *tree,
		volumetype index,
		sidetype sideLength,
		sidetype brickLength,
		volumetype nullValue,
		volumetype shiftValue
);

void shiftBranchesMultiscale
(
		volumetype *tree,
		volumetype index,
		volumetype maxNumberBranches,
		volumetype shiftValue
);

void shiftBranchesMipMap
(
		volumetype *tree,
		volumetype index,
		sidetype sideLength,
		sidetype brickLength,
		volumetype nullValue,
		volumetype shiftValue
);

std::string getMaximalNumberOfLeavesForDynamicTree
(
		long long availableBytes,
		sidetype brickLength,
		long long bytesForDistance,
		long long bytesForWeight,
		long long bytesForColor,
		volumetype *nLeaves,
		volumetype *nBranches
);

std::string getMaximalNumberOfLeavesForDynamicTreeMultiscale
(
		long long availableBytes,
		sidetype brickLength,
		long long bytesForDistance,
		long long bytesForWeight,
		long long bytesForColor,
		volumetype *nLeaves,
		volumetype *nBranches,
		bool useMultiscale
);

std::string getMaximalNumberOfLeavesForDynamicTreeMipMap
(
		long long availableBytes,
		sidetype brickLength,
		long long bytesForDistance,
		long long bytesForWeight,
		long long bytesForColor,
		volumetype *nLeaves,
		volumetype *nBranches
);

std::string getMaximalNumberOfLeavesForDynamicTreeMultiscale
(
		long long availableBytes,
		sidetype brickLength,
		long long bytesForDistance,
		long long bytesForWeight,
		long long bytesForColor,
		volumetype *nLeaves,
		volumetype *nBranches
);


std::string getMaximalNumberOfLeavesForFixedTree
(
		long long availableBytes,
		sidetype sideLength,
		sidetype brickLength,
		long long bytesForDistance,
		long long bytesForWeight,
		long long bytesForColor,
		volumetype *maxNLeaves,
		volumetype *maxnBranches,
		volumetype *nLeavesFullTree = NULL
);

void printTreeStatistics(volumetype *tree, sidetype start, sidetype end, bool printwholetree = false);

volumetype getBrickIndex(
		sidetype x, sidetype y, sidetype z,
		sidetype n, sidetype brickLength, sidetype brickSize,
		volumetype nNodes, volumetype nAllocatedLeaves,
		volumetype *tree);

volumetype getBrickIndexMultiscale(
		sidetype x, sidetype y, sidetype z,
		sidetype n, sidetype brickLength, sidetype brickSize,
		volumetype nNodes, volumetype nAllocatedLeaves,
		volumetype *tree, sidetype *leafScale
);

volumetype getBrickIndexMipMap(
		sidetype x, sidetype y, sidetype z,
		sidetype n, sidetype brickLength,
		sidetype brickSize,
		volumetype nNodes, volumetype nAllocatedLeaves,
		volumetype *tree, sidetype *leafScale
);

volumetype getBrickIndex(
		sidetype x, sidetype y, sidetype z, float *distance,
		sidetype n, sidetype brickLength, sidetype brickSize,
		volumetype nNodes, volumetype nAllocatedLeaves,
		volumetype *tree, float *distanceField);

volumetype getIndexFromTree(
		sidetype x, sidetype y, sidetype z,
		sidetype n, sidetype brickLength, sidetype brickSize,
		volumetype nNodes, volumetype nAllocatedLeaves,
		volumetype *tree);

volumetype getBrickIndexMipMap2
(
		sidetype x, sidetype y, sidetype z,
		sidetype n, sidetype brickLength,
		sidetype brickSize,
		volumetype nNodes, volumetype nLeaves,
		volumetype *tree, volumetype nullValue
);


void addBrickToMesh
(
		MeshSeparate &mesh,
		float offsetX,
		float offsetY,
		float offsetZ,
		float scale,
		sidetype ox,
		sidetype oy,
		sidetype oz,
		volumetype brickIdx,
		sidetype brickLength,
		volumetype brickSize,
		sidetype blp1,
		sidetype blm1,
		volumetype *tree,
		float *distance,
		weighttype *weights,
		volumetype nNodes,
		sidetype n,
		volumetype nLeavesUsed,
		float *distanceBrick,
		weighttype *weightsBrick,
		bool useColor,
		colortype *color,
		colortype *r,
		colortype *g,
		colortype *b,
		colortype *rBrick,
		colortype *gBrick,
		colortype *bBrick,
		unsigned int framesAdded,
		unsigned int *degenerateFaces = NULL
);

void addBrickToMeshMultiscale
(
		MeshSeparate &mesh,
		float offsetX,
		float offsetY,
		float offsetZ,
		float scaleToBrick,
		sidetype leafScale,
		sidetype ox,
		sidetype oy,
		sidetype oz,
		volumetype brickIdx,
		sidetype brickLength,
		volumetype brickSize,
		sidetype blp1,
		sidetype blm1,
		volumetype *tree,
		float *distance,
		weighttype *weights,
		volumetype nNodes,
		sidetype n,
		volumetype nLeavesUsed,
		float *distanceBrick,
		weighttype *weightsBrick,
		bool useColor,
		colortype *color,
		colortype *r,
		colortype *g,
		colortype *b,
		colortype *rBrick,
		colortype *gBrick,
		colortype *bBrick,
		unsigned int framesAdded,
		unsigned int *degenerateFaces = NULL
);

void addBrickToMeshMipMap
(
		MeshSeparate &mesh,
		float offsetX,
		float offsetY,
		float offsetZ,
		float scaleToBrick,
		sidetype leafScale,
		sidetype ox,
		sidetype oy,
		sidetype oz,
		volumetype brickIdx,
		sidetype brickLength,
		volumetype brickSize,
		sidetype blp1,
		sidetype blm1,
		volumetype *tree,
		float *distance,
		weighttype *weights,
		volumetype nNodes,
		sidetype n,
		volumetype nLeavesUsed,
		float *distanceBrick,
		weighttype *weightsBrick,
		bool useColor,
		colortype *color,
		colortype *r,
		colortype *g,
		colortype *b,
		colortype *rBrick,
		colortype *gBrick,
		colortype *bBrick,
		unsigned int framesAdded,
		unsigned int *degenerateFaces
);



void getTreeLinesMiddle(volumetype *tree, volumetype idx, sidetype brickLength,
		sidetype half, sidetype ox, sidetype oy, sidetype oz, MeshSeparate &mesh);

void getTreeLinesBox(volumetype *tree, volumetype idx, sidetype brickLength,
		volumetype nBranchesTotal,
		sidetype half, sidetype ox, sidetype oy, sidetype oz, MeshSeparate &mesh, float width);

void getTreeLinesBoxMipMap(volumetype *tree, volumetype idx, sidetype brickLength,
		volumetype nBranchesTotal,
		sidetype half, sidetype ox, sidetype oy, sidetype oz, MeshSeparate &mesh, float width);

void getTreeQuads(volumetype *tree, volumetype idx, sidetype brickLength,
		sidetype half, sidetype ox, sidetype oy, sidetype oz,
		MeshSeparate &mesh);

bool weightInfluence(float minWeight,
weighttype w0, weighttype w1, weighttype w2, weighttype w3,
weighttype w4, weighttype w5, weighttype w6, weighttype w7
);



void addBrickWallZ
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0,
		leafstack leaves1
);


void addInterior
(
		leafstack leaves,
		treeinfo info,
		volumetype index, sidetype size,
		sidetype ox, sidetype oy, sidetype oz,
		volumetype lastLeaf,
		const MarchingCubesIndexed &mc
);


void addBrickInterior
(
		leafstack leaves,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		treeinfo info,
		const MarchingCubesIndexed &mc
);












#endif /* TREEANDBRICK_HPP_ */
