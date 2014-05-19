/*
 * treeandbrick.cpp
 *
 *  Created on: Mar 1, 2013
 *      Author: steinbrf
 */

#include "treeandbrick.hpp"
#include "treeandbrick_indexed.hpp"


int expint(int base, int exponent){
	unsigned int result =1;
	while(exponent>0){
		result *= base;
		exponent--;
	}
	return result;
}

long expint(long base, long exponent){
	long result =1;
	while(exponent>0){
		result *= base;
		exponent--;
	}
	return result;
}

long long expint(long long base, long long exponent){
	long long result =1;
	while(exponent>0){
		result *= base;
		exponent--;
	}
	return result;
}

std::string getMaximalNumberOfLeavesForDynamicTree
(
		long long availableBytes,
		sidetype brickLength,
		long long bytesForDistance,
		long long bytesForWeight,
		long long bytesForColor,
		volumetype *nLeaves,
		volumetype *nBranches
)
{
	std::stringstream stream;
	if(!nLeaves || !nBranches){
		stream << "\nERROR: You need to give correct Pointers!";
		return stream.str();
	}
	long long brickSize = (long long)brickLength*(long long)brickLength*(long long)brickLength;
	long long bytesForOneLeafWithoutBrick = 2*sizeof(volumetype) + 3*sizeof(sidetype);
	long long bytesForOneBrick = brickSize * (bytesForDistance+bytesForColor+bytesForWeight);
	long long bytesForOneLeaf =  bytesForOneLeafWithoutBrick + bytesForOneBrick;


	long long levels=1;
	long long branchMemory = 8*sizeof(volumetype);
	long long leafMemoryFullTree = 8*bytesForOneLeaf;
	while(branchMemory+leafMemoryFullTree < availableBytes){
		levels++;
		branchMemory += (long long)sizeof(volumetype)*expint((long long)(8),levels);
		leafMemoryFullTree = bytesForOneLeaf*expint((long long)(8),levels);
	}
  *nLeaves = (availableBytes-branchMemory)/bytesForOneLeaf;
  long long leafMemory = *nLeaves * bytesForOneLeaf;
	*nBranches = branchMemory/sizeof(volumetype);
	long long sideLength = expint((long long)(2),levels);

	stream << "\nFor every Leaf you need " << bytesForOneLeafWithoutBrick << " Bytes for Indices: ";
	stream << bytesForDistance << " Bytes for the Distance Field, "
			   << bytesForWeight << " Bytes for Weights and "
			   << bytesForColor << " Bytes for Color";
	stream << "\nThe Brick Size is " << brickSize;
	stream << "\nFor every Brick you need " << bytesForOneBrick << " Bytes";
	stream << "\nThis makes a total of " << bytesForOneLeaf << " Bytes for one Leaf";
	stream << "\nYou have " << availableBytes << " Bytes available.";

	stream << "\nMaximum Side Length for a dense Tree: " << sideLength*brickLength;
	stream << "\nMemory for Branches: " << branchMemory << " Bytes";
	stream << "\nMemory for Leaves for a full Tree: " << leafMemoryFullTree << " Bytes";
	stream << "\nMemory for Leaves: " << leafMemory << " Bytes";
	stream << "\nNumber of Leaves: " << *nLeaves;
	stream << "\nNumber of Branches: " << *nBranches;

	stream << "\nSubtracting " << LEAVES_LESS << " Leaves";
	*nLeaves -= LEAVES_LESS;
	stream << "\nLeaving " << *nLeaves << " Leaves";
	if(*nLeaves < expint((long long)(8),levels-1)){
		*nBranches -= expint((long long)(8),levels);
		stream << "\nThis is less than " << expint((long long)(8),levels-1)
				   << " Leaves for the next lower Level, to the branches are reduced to "
				   << *nBranches;
	}

	return stream.str();
}

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
)
{
	std::stringstream stream;
	if(!nLeaves || !nBranches){
		stream << "\nERROR: You need to give correct Pointers!";
		return stream.str();
	}
	long long brickSize = (long long)brickLength*(long long)brickLength*(long long)brickLength;
	long long bytesForOneLeafWithoutBrick = 2*sizeof(volumetype) + sizeof(sidetype3) + useMultiscale*sizeof(sidetype);
	long long bytesForOneBrick = brickSize * (bytesForDistance+bytesForColor+bytesForWeight);
	long long bytesForOneLeaf =  bytesForOneLeafWithoutBrick + bytesForOneBrick;


	long long levels=1;
	long long branchMemory = 8*sizeof(volumetype);
	long long leafMemoryFullTree = 8*bytesForOneLeaf;
	while(branchMemory+leafMemoryFullTree < availableBytes){
		levels++;
		branchMemory += (long long)sizeof(volumetype)*expint((long long)(8),levels);
		leafMemoryFullTree = bytesForOneLeaf*expint((long long)(8),levels);
	}
  *nLeaves = (availableBytes-branchMemory)/bytesForOneLeaf;
  long long leafMemory = *nLeaves * bytesForOneLeaf;
	*nBranches = branchMemory/sizeof(volumetype);
	long long sideLength = expint((long long)(2),levels);

	stream << "\nFor every Leaf you need " << bytesForOneLeafWithoutBrick << " Bytes for Indices: ";
	stream << bytesForDistance << " Bytes for the Distance Field, "
			   << bytesForWeight << " Bytes for Weights and "
			   << bytesForColor << " Bytes for Color";
	stream << "\nThe Brick Size is " << brickSize;
	stream << "\nFor every Brick you need " << bytesForOneBrick << " Bytes";
	stream << "\nThis makes a total of " << bytesForOneLeaf << " Bytes for one Leaf";
	stream << "\nYou have " << availableBytes << " Bytes available.";

	stream << "\nMaximum Side Length for a dense Tree: " << sideLength*brickLength;
	stream << "\nMemory for Branches: " << branchMemory << " Bytes";
	stream << "\nMemory for Leaves for a full Tree: " << leafMemoryFullTree << " Bytes";
	stream << "\nMemory for Leaves: " << leafMemory << " Bytes";
	stream << "\nNumber of Leaves: " << *nLeaves;
	stream << "\nNumber of Branches: " << *nBranches;

	stream << "\nSubtracting " << LEAVES_LESS << " Leaves";
	*nLeaves -= LEAVES_LESS;
	stream << "\nLeaving " << *nLeaves << " Leaves";
	if(*nLeaves < expint((long long)(8),levels-1)){
		*nBranches -= expint((long long)(8),levels);
		stream << "\nThis is less than " << expint((long long)(8),levels-1)
				   << " Leaves for the next lower Level, to the branches are reduced to "
				   << *nBranches;
	}

	return stream.str();
}


std::string getMaximalNumberOfLeavesForDynamicTreeMipMap
(
		long long availableBytes,
		sidetype brickLength,
		long long bytesForDistance,
		long long bytesForWeight,
		long long bytesForColor,
		volumetype *nLeaves,
		volumetype *nBranches
)
{
	std::stringstream stream;
	if(!nLeaves || !nBranches){
		stream << "\nERROR: You need to give correct Pointers!";
		return stream.str();
	}
	long long brickSize = (long long)brickLength*(long long)brickLength*(long long)brickLength;
	long long bytesForOneLeafWithoutBrick = 4*sizeof(volumetype) + sizeof(sidetype3) + sizeof(sidetype);
	long long bytesForOneBrick = brickSize * (bytesForDistance+bytesForColor+bytesForWeight);
	long long bytesForOneLeaf =  bytesForOneLeafWithoutBrick + bytesForOneBrick;


	long long levels=1;
	long long branchMemory = 16*sizeof(volumetype);
	long long leafMemoryFullTree = 8*bytesForOneLeaf;
	while(branchMemory+leafMemoryFullTree < availableBytes){
		levels++;
		branchMemory += (long long)sizeof(volumetype)*expint((long long)(8),levels)*2;
		leafMemoryFullTree = bytesForOneLeaf*expint((long long)(8),levels);
	}
  *nLeaves = (availableBytes-branchMemory)/bytesForOneLeaf;
  long long leafMemory = *nLeaves * bytesForOneLeaf;
	*nBranches = branchMemory/sizeof(volumetype);
	long long sideLength = expint((long long)(2),levels);

	stream << "\nFor every Leaf you need " << bytesForOneLeafWithoutBrick << " Bytes for Indices: ";
	stream << bytesForDistance << " Bytes for the Distance Field, "
			   << bytesForWeight << " Bytes for Weights and "
			   << bytesForColor << " Bytes for Color";
	stream << "\nThe Brick Size is " << brickSize;
	stream << "\nFor every Brick you need " << bytesForOneBrick << " Bytes";
	stream << "\nThis makes a total of " << bytesForOneLeaf << " Bytes for one Leaf";
	stream << "\nYou have " << availableBytes << " Bytes available.";

	stream << "\nMaximum Side Length for a dense Tree: " << sideLength*brickLength;
	stream << "\nMemory for Branches: " << branchMemory << " Bytes";
	stream << "\nMemory for Leaves for a full Tree: " << leafMemoryFullTree << " Bytes";
	stream << "\nMemory for Leaves: " << leafMemory << " Bytes";
	stream << "\nNumber of Leaves: " << *nLeaves;
	stream << "\nNumber of Branches: " << *nBranches;

	stream << "\nSubtracting " << LEAVES_LESS << " Leaves";
	*nLeaves -= LEAVES_LESS;
	stream << "\nLeaving " << *nLeaves << " Leaves";
	if(*nLeaves < expint((long long)(8),levels-1)){
		*nBranches -= expint((long long)(8),levels);
		stream << "\nThis is less than " << expint((long long)(8),levels-1)
				   << " Leaves for the next lower Level, to the branches are reduced to "
				   << *nBranches;
	}

	return stream.str();
}



std::string getMaximalNumberOfLeavesForFixedTree
(
		long long availableBytes,
		sidetype sideLength,
		sidetype brickLength,
		long long bytesForDistance,
		long long bytesForWeight,
		long long bytesForColor,
		volumetype *maxnLeaves,
		volumetype *maxnBranches,
		volumetype *nLeavesFullTree
)
{
	std::stringstream stream;
	volumetype nBranches = 1;
	volumetype nLeaves = 1;
	volumetype brickSize = brickLength*brickLength*brickLength;
	long long bytesForOneLeafWithoutBrick = 2*sizeof(volumetype) + 3*sizeof(sidetype);
	long long bytesForOneBrick = brickSize * (bytesForDistance+bytesForColor+bytesForWeight);
	long long bytesForOneLeaf =  bytesForOneLeafWithoutBrick + bytesForOneBrick;

	for(sidetype length = sideLength; length>brickLength;length>>=1,nLeaves<<=3, nBranches+=nLeaves);
	long long bytesForTree = (long long)nBranches*sizeof(volumetype);
	long long mnLeaves = (availableBytes-bytesForTree)/bytesForOneLeaf;

	stream << "\n\nA Full tree would have " << nLeaves << " Leaves.";
	stream << "\nThis yields a Number of " << nBranches << " Branches.";
	stream << "\nFor every Leaf you need " << bytesForOneLeafWithoutBrick << " Bytes for Indices: ";
	stream << bytesForDistance << " Bytes for the Distance Field, "
			   << bytesForWeight << " Bytes for Weights and "
			   << bytesForColor << " Bytes for Color";
	stream << "\nFor every Brick you need " << bytesForOneBrick << " Bytes";
	stream << "\nThis makes a total of " << bytesForOneLeaf << " Bytes for one Leaf";
	stream << "\nYou have " << availableBytes << " Bytes available.";
	stream << "\nMinus " << bytesForTree << " Bytes for the Tree Branch Indices, that makes "
			<< availableBytes-bytesForTree << " Bytes.";

	stream << "\nTherefore you can afford " << mnLeaves << " Leaves";
	if(mnLeaves > nLeaves){
		stream << "\nThis is more than you need, therefore, you will only get " << nLeaves << " Leaves";
		mnLeaves = nLeaves;
	}

	if(maxnLeaves){
		if(*maxnLeaves){
			stream << "\nYou requested " << *maxnLeaves << " Leaves.";
		}
		*maxnLeaves = mnLeaves;
	}
	if(maxnBranches){
		if(*maxnBranches){
			stream << "\nYou requested " << *maxnBranches << " Branches.";
		}
		*maxnBranches = nBranches;
	}
	stream << "\n\n";
	if(nLeavesFullTree) *nLeavesFullTree = nLeaves;


	return stream.str();
}


void printTreeStatistics(volumetype *tree, sidetype start, sidetype end, bool printwholetree)
{
	if(printwholetree) fprintf(stderr,"\n\nTree:\n");
	std::vector<volumetype> queueIndex;
	std::vector<volumetype> queueSize;
	int leavesAllocated = 0;
	int leavesNotAllocated = 0;
	queueIndex.push_back(0); queueSize.push_back(start);
	sidetype level = 0;
	volumetype levelMax = 1;
//	VINDEXTYPE leaves = 0;
	for(volumetype index=0;index<queueIndex.size();){
		volumetype current = queueIndex[index];
		volumetype size = queueSize[index];
		if(printwholetree) fprintf(stderr," (%i|%i:",index,current);

		if(size/2<=end){
			for(volumetype childoffset=0;childoffset<8;childoffset++){
				if(printwholetree) fprintf(stderr," %s",tree[current+childoffset]<MAXLEAFNUMBER ? "!" : ".");
				if(tree[current+childoffset]<MAXLEAFNUMBER) leavesAllocated++;
				else leavesNotAllocated++;
			}
		}
		else
		{
			for(volumetype childoffset=0;childoffset<8;childoffset++){
				if(tree[current+childoffset]<BRANCHINIT){
					if(printwholetree) fprintf(stderr," %i",tree[current+childoffset]);
					queueIndex.push_back(tree[current+childoffset]);
					queueSize.push_back(size/2);
				}
				else{
					if(printwholetree) fprintf(stderr," -");
				}
			}
		}
		if(printwholetree) fprintf(stderr,")");


		index++;
		if(index==levelMax){
			level++;
			levelMax = queueIndex.size();
			if(printwholetree) fprintf(stderr,"\n\n");
		}
	}

	if(printwholetree) fprintf(stderr,"\n\n");
	fprintf(stderr,"\nTree Statistics: %i Branch Nodes in total, %i Leaves in total,"
			"\n%i Allocated Leaves, %i not allocated Leaves",
			(int)queueIndex.size(),leavesAllocated+leavesNotAllocated,leavesAllocated,leavesNotAllocated);
}

//volumetype getBrickIndex(
//		sidetype x, sidetype y, sidetype z, float *distance,
//		sidetype n, sidetype brickLength, sidetype brickSize,
//		volumetype nNodes, volumetype nAllocatedLeaves,
//		volumetype *tree,
//		float *distanceField
//)
//{
//	if(x>=n || y>=n || z>=n){
//		return MAXLEAFNUMBER;
//	}
//	volumetype index = 0;
//	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
//	for(sidetype w=n;w>brickLength;w/=2){
//		if(index >= nNodes){
//			return MAXLEAFNUMBER;
//		}
//		sidetype half = w/2;
//		sidetype childoffset = 4*(z>=oz+half) + 2*(y>=oy+half) + (x>=ox+half);
//		ox += half*(x>=ox+half);
//		oy += half*(y>=oy+half);
//		oz += half*(z>=oz+half);
//		index = tree[index + childoffset];
//	}
//	if(index >= nAllocatedLeaves){
//		return MAXLEAFNUMBER;
//	}
//	volumetype idx = index*brickSize+(((z-oz)*brickLength+(y-oy))*brickLength+(x-ox));
//	*distance = distanceField[idx];
//	return index;
//}

void shiftBranches
(
		volumetype *tree,
		volumetype index,
		sidetype sideLength,
		sidetype brickLength,
		volumetype nullValue,
		volumetype shiftValue
)
{
	sidetype half = sideLength/2;
	if(half<=brickLength) return;
	for(volumetype i=0;i<8;i++){
		if(tree[index+i] != nullValue){
			shiftBranches(tree,tree[index+i],half,brickLength,nullValue,shiftValue);
			tree[index+i] += shiftValue;
		}
	}
}

void shiftBranchesMultiscale
(
		volumetype *tree,
		volumetype index,
		volumetype maxNumberBranches,
		volumetype shiftValue
)
{
	for(volumetype i=0;i<8;i++){
		if(tree[index+i] < maxNumberBranches){
			shiftBranchesMultiscale(tree,tree[index+i],maxNumberBranches,shiftValue);
			tree[index+i] += shiftValue;
		}
	}
}

void shiftBranchesMipMap
(
		volumetype *tree,
		volumetype index,
		sidetype sideLength,
		sidetype brickLength,
		volumetype nullValue,
		volumetype shiftValue
)
{
	sidetype half = sideLength/2;
//	if(half<=brickLength) return;
	for(volumetype i=0;i<8;i++){
		if(half>brickLength && tree[index+8+i] != nullValue){
			shiftBranchesMipMap(tree,tree[index+8+i],half,brickLength,nullValue,shiftValue);
			tree[index+8+i] += shiftValue;
		}
	}
}

volumetype getBrickIndex(
		sidetype x, sidetype y, sidetype z,
		sidetype n, sidetype brickLength, sidetype brickSize,
		volumetype nNodes, volumetype nAllocatedLeaves,
		volumetype *tree
)
{
	if(x>=n || y>=n || z>=n){
		return MAXLEAFNUMBER;
	}
	volumetype index = 0;
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	for(sidetype w=n;w>brickLength;w/=2){
		if(index >= nNodes){
			return MAXLEAFNUMBER;
		}
		sidetype half = w/2;
		sidetype childoffset = 4*(z>=oz+half) + 2*(y>=oy+half) + (x>=ox+half);
		ox += half*(x>=ox+half);
		oy += half*(y>=oy+half);
		oz += half*(z>=oz+half);
		index = tree[index + childoffset];
	}
	if(index >= nAllocatedLeaves){
		return MAXLEAFNUMBER;
	}
//	fprintf(stderr," HIT: %i",index);
	return index;
}

volumetype getBrickIndexMultiscale(
		sidetype x, sidetype y, sidetype z,
		sidetype n, sidetype brickLength, sidetype brickSize,
		volumetype nNodes, volumetype nAllocatedLeaves,
		volumetype *tree, sidetype *leafScale
)
{
	if(x>=n || y>=n || z>=n){
		return MAXLEAFNUMBER;
	}
	volumetype index = 0;
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	for(sidetype w=n;w>brickLength;w/=2){
		if(index >= DEADBRANCH){
			*leafScale = 0;
			return MAXLEAFNUMBER;
		}
		if(index >= nNodes){
			*leafScale = w;
			return index-nNodes;
		}
		sidetype half = w/2;
		sidetype childoffset = 4*(z>=oz+half) + 2*(y>=oy+half) + (x>=ox+half);
		ox += half*(x>=ox+half);
		oy += half*(y>=oy+half);
		oz += half*(z>=oz+half);
		index = tree[index + childoffset];
	}
	if(index >= DEADBRANCH || index < nNodes){
		*leafScale = 0;
		return MAXLEAFNUMBER;
	}
	*leafScale = brickLength;
	return index-nNodes;
}

volumetype getBrickIndexMipMap(
		sidetype x, sidetype y, sidetype z,
		sidetype n, sidetype brickLength,
		sidetype brickSize,
		volumetype nNodes, volumetype nAllocatedLeaves,
		volumetype *tree, sidetype *leafScale
)
{
//	fprintf(stderr,"\n");
	if(x>=n || y>=n || z>=n){
		return MAXLEAFNUMBER;
	}
	volumetype index = 0;
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	for(sidetype w=n;w>brickLength;w/=2){
//		fprintf(stderr," %i",w);
		if(index >= DEADBRANCH){
			*leafScale = 0;
//			fprintf(stderr," o");
			return MAXLEAFNUMBER;
		}
		sidetype half = w/2;
		sidetype childoffset = 4*(z>=oz+half) + 2*(y>=oy+half) + (x>=ox+half) + 8*(half>brickLength);
		ox += half*(x>=ox+half);
		oy += half*(y>=oy+half);
		oz += half*(z>=oz+half);
		index = tree[index + childoffset];
	}
	if(index >= DEADBRANCH){
		*leafScale = 0;
		return MAXLEAFNUMBER;
	}
	*leafScale = brickLength;
//	fprintf(stderr," !");
	return index;
}

volumetype getBrickIndexMipMap2
(
		sidetype x, sidetype y, sidetype z,
		sidetype n, sidetype brickLength,
		sidetype brickSize,
		volumetype nNodes, volumetype nLeaves,
		volumetype *tree, volumetype nullValue
)
{
	volumetype result = nullValue;
		if(x>=n || y>=n || z>=n){
			return result;
		}
		volumetype index = 0;
		sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
		for(sidetype w=n;w>brickLength;w/=2){
			if(index >= nNodes){
				return result;
			}
			sidetype half = w/2;
			sidetype childoffset = 4*(z>=oz+half) + 2*(y>=oy+half) + (x>=ox+half);
			if(tree[index+childoffset]<nullValue) result = tree[index+childoffset];
			ox += half*(x>=ox+half);
			oy += half*(y>=oy+half);
			oz += half*(z>=oz+half);
			if(half>brickLength && tree[index + childoffset+8]<nullValue){
				index = tree[index + childoffset+8];
			}
			else return result;
		}
		//For the compiler
		return result;
}


volumetype getIndexFromTree(
		sidetype x, sidetype y, sidetype z,
		sidetype n, sidetype brickLength, sidetype brickSize,
		volumetype nNodes, volumetype nAllocatedLeaves,
		volumetype *tree)
{
	if(x>=n || y>=n || z>=n){
		return MAXINDEX;
	}
	volumetype index = 0;
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	for(sidetype w=n;w>brickLength;w/=2){
		if(index >= nNodes){
			return MAXINDEX;
		}
		sidetype half = w/2;
		sidetype childoffset = 4*(z>=oz+half) + 2*(y>=oy+half) + (x>=ox+half);
		ox += half*(x>=ox+half);
		oy += half*(y>=oy+half);
		oz += half*(z>=oz+half);
		index = tree[index + childoffset];
	}
	if(index >= nAllocatedLeaves){
		return MAXINDEX;
	}
	return index*brickSize+(((z-oz)*brickLength+(y-oy))*brickLength+(x-ox));
}


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
		unsigned int *degenerateFaces
)
{
	if(useColor && ((color && (r || g || b)) || ((!color) && (!r || !g || !b)))){
		fprintf(stderr,"\n\nERROR:\nYou should choose either interleaved color or separate red, green and blue!\n\n");
		return;
	}

	Vertex3f vertlist[12];
	VertexColor collist[12];
	MarchingCubes mc;
	unsigned int degenerate_faces = 0;
	sidetype inc = 0;


	volumetype neighborX = (ox<n-1-brickLength) ?
			getBrickIndex(ox+brickLength+inc,oy+inc,oz+inc,
			n,brickLength,brickSize,nNodes,nLeavesUsed,tree) : MAXLEAFNUMBER;
	volumetype neighborY = (oy<n-1-brickLength) ?
			getBrickIndex(ox+inc,oy+brickLength+inc,oz+inc,
			n,brickLength,brickSize,nNodes,nLeavesUsed,tree) : MAXLEAFNUMBER;
	volumetype neighborZ = (oz<n-1-brickLength) ?
			getBrickIndex(ox+inc,oy+inc,oz+brickLength+inc,
			n,brickLength,brickSize,nNodes,nLeavesUsed,tree) : MAXLEAFNUMBER;
	volumetype neighborXY = (ox<n-1-brickLength && oy<n-1-brickLength) ?
			getBrickIndex(ox+brickLength+inc,oy+brickLength+inc,oz+inc,
			n,brickLength,brickSize,nNodes,nLeavesUsed,tree) : MAXLEAFNUMBER;
	volumetype neighborXZ = (ox<n-1-brickLength && oz<n-1-brickLength) ?
			getBrickIndex(ox+brickLength+inc,oy+inc,oz+brickLength+inc,
			n,brickLength,brickSize,nNodes,nLeavesUsed,tree) : MAXLEAFNUMBER;
	volumetype neighborYZ = (oy<n-1-brickLength && oz<n-1-brickLength) ?
			getBrickIndex(ox+inc,oy+brickLength+inc,oz+brickLength+inc,
			n,brickLength,brickSize,nNodes,nLeavesUsed,tree) : MAXLEAFNUMBER;
	volumetype neighborXYZ = (ox<n-1-brickLength && oy<n-1-brickLength && oz<n-1-brickLength) ?
			getBrickIndex(ox+brickLength+inc,oy+brickLength+inc,oz+brickLength+inc,
			n,brickLength,brickSize,nNodes,nLeavesUsed,tree) : MAXLEAFNUMBER;


	for(sidetype x=0;x<brickLength;x++){
		for(sidetype y=0;y<brickLength;y++){
			for(sidetype z=0;z<brickLength;z++){
				distanceBrick[(z*blp1+y)*blp1+x] = distance[brickIdx*brickSize+(z*brickLength+y)*brickLength+x];
				weightsBrick[(z*blp1+y)*blp1+x] = weights[brickIdx*brickSize+(z*brickLength+y)*brickLength+x];
				//Brick Boundaries
				//X
				if(x==blm1){
					distanceBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
							distance[neighborX*brickSize+(z*brickLength+y)*brickLength] : 0.0f;
					weightsBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
							weights[neighborX*brickSize+(z*brickLength+y)*brickLength] : 0.0f;
					//XY
					if(y==blm1){
						distanceBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
								distance[neighborXY*brickSize+z*brickLength*brickLength] : 0.0f;
						weightsBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
								weights[neighborXY*brickSize+z*brickLength*brickLength] : 0.0f;
						//XYZ
						if(z==blm1){
							distanceBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
									distance[neighborXYZ*brickSize] : 0.0f;
							weightsBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
									weights[neighborXYZ*brickSize] : 0.0f;
						}
					}
					//XZ
					if(z==blm1){
						distanceBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
								distance[neighborXZ*brickSize+y*brickLength] : 0.0f;
						weightsBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
								weights[neighborXZ*brickSize+y*brickLength] : 0.0f;
					}
				}
				//Y
				if(y==blm1){
					distanceBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
							distance[neighborY*brickSize+z*brickLength*brickLength+x] : 0.0f;
					weightsBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
							weights[neighborY*brickSize+z*brickLength*brickLength+x] : 0.0f;
					//YZ
					if(z==blm1){
						distanceBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
								distance[neighborYZ*brickSize+x] : 0.0f;
						weightsBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
								weights[neighborYZ*brickSize+x] : 0.0f;
					}
				}
				//Z
				if(z==blm1){
					distanceBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
							distance[neighborZ*brickSize+y*brickLength+x] : 0.0f;
					weightsBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
							weights[neighborZ*brickSize+y*brickLength+x] : 0.0f;
				}
				if(useColor){
					volumetype idx = brickIdx*brickSize+(z*brickLength+y)*brickLength+x;
					rBrick[(z*blp1+y)*blp1+x] = r ? r[idx] : color[3*(idx)+0];
					gBrick[(z*blp1+y)*blp1+x] = g ? g[idx] : color[3*(idx)+1];
					bBrick[(z*blp1+y)*blp1+x] = b ? b[idx] : color[3*(idx)+2];
					//Brick Boundaries
					//X
					if(x==blm1){
						volumetype idx = neighborX*brickSize+(z*brickLength+y)*brickLength;
						rBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
								(r ? r[idx] : color[3*idx+0]) : 0;
						gBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
								(g ? g[idx] : color[3*idx+1]) : 0;
						bBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
								(b ? b[idx] : color[3*idx+2]) : 0;
						//XY
						if(y==blm1){
							volumetype idx = neighborXY*brickSize+z*brickLength*brickLength;
							rBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
									(r ? r[idx] : color[3*idx+0]) : 0;
							gBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
									(g ? g[idx] : color[3*idx+1]) : 0;
							bBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
									(b ? b[idx] : color[3*idx+2]) : 0;
							//XYZ
							if(z==blm1){
								volumetype idx = neighborXYZ*brickSize;
								rBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
										(r ? r[idx] : color[3*idx+0]) : 0;
								gBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
										(g ? g[idx] : color[3*idx+1]) : 0;
								bBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
										(b ? b[idx] : color[3*idx+2]) : 0;
							}
						}
						//XZ
						if(z==blm1){
							volumetype idx = neighborXZ*brickSize+y*brickLength;
							rBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
									(r ? r[idx] : color[3*idx+0]) : 0;
							gBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
									(g ? g[idx] : color[3*idx+1]) : 0;
							bBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
									(b ? b[idx] : color[3*idx+2]) : 0;
						}
					}
					//Y
					if(y==blm1){
						volumetype idx = neighborY*brickSize+z*brickLength*brickLength+x;
						rBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
								(r ? r[idx] : color[3*idx+0]) : 0;
						gBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
								(g ? g[idx] : color[3*idx+1]) : 0;
						bBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
								(b ? b[idx] : color[3*idx+2]) : 0;
						//YZ
						if(z==blm1){
							volumetype idx = neighborYZ*brickSize+x;
							rBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
									(r ? r[idx] : color[3*idx+0]) : 0;
							gBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
									(g ? g[idx] : color[3*idx+1]) : 0;
							bBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
									(b ? b[idx] : color[3*idx+2]) : 0;
						}
					}
					//Z
					if(z==blm1){
						volumetype idx = neighborZ*brickSize+y*brickLength+x;
						rBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
								(r ? r[idx] : color[3*idx+0]) : 0;
						gBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
								(g ? g[idx] : color[3*idx+1]) : 0;
						bBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
								(b ? b[idx] : color[3*idx+2]) : 0;
					}
				}
			}
		}
	}


	for(sidetype x=0;x<brickLength;x++){
		for(sidetype y=0;y<brickLength;y++){
			for(sidetype z=0;z<brickLength;z++){
				if(weightsBrick[(z*blp1+y)*blp1+x]>=std::min((double)framesAdded,MIN_WEIGHT_FOR_SURFACE)){

					volumetype idx0 = (z*blp1+y)*blp1+x;
					volumetype idx1 = (z*blp1+y)*blp1+x+1;
					volumetype idx2 = (z*blp1+y+1)*blp1+x+1;
					volumetype idx3 = (z*blp1+y+1)*blp1+x;
					volumetype idx4 = ((z+1)*blp1+y)*blp1+x;
					volumetype idx5 = ((z+1)*blp1+y)*blp1+x+1;
					volumetype idx6 = ((z+1)*blp1+y+1)*blp1+x+1;
					volumetype idx7 = ((z+1)*blp1+y+1)*blp1+x;
					if(idx0>=MAXINDEX||idx1>=MAXINDEX||idx2>=MAXINDEX||idx3>=MAXINDEX||
						 idx4>=MAXINDEX||idx5>=MAXINDEX||idx6>=MAXINDEX||idx7>=MAXINDEX){
						continue;
					}


					int *table = mc.getColoredTrianglesForCube(vertlist,collist,x,y,z,
							offsetX+ox*scale,offsetY+oy*scale,offsetZ+oz*scale,scale,
							idx0,idx1,idx2,idx3,idx4,idx5,idx6,idx7,distanceBrick,weightsBrick,rBrick,gBrick,bBrick);


					for (unsigned int i=0;table[i]!=-1;i+=3) {
						Vertex3f v0 = vertlist[table[i  ]];
						Vertex3f v1 = vertlist[table[i+1]];
						Vertex3f v2 = vertlist[table[i+2]];
						if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
							degenerate_faces++;

//							float val0 = distanceBrick[idx0]; float val1 = distanceBrick[idx1];
//							float val2 = distanceBrick[idx2]; float val3 = distanceBrick[idx3];
//							float val4 = distanceBrick[idx4]; float val5 = distanceBrick[idx5];
//							float val6 = distanceBrick[idx6]; float val7 = distanceBrick[idx7];
//
//							weighttype w0 = weightsBrick[idx0]; weighttype w1 = weightsBrick[idx1];
//							weighttype w2 = weightsBrick[idx2]; weighttype w3 = weightsBrick[idx3];
//							weighttype w4 = weightsBrick[idx4]; weighttype w5 = weightsBrick[idx5];
//							weighttype w6 = weightsBrick[idx6]; weighttype w7 = weightsBrick[idx7];

//							fprintf(stderr,"\nVertex-Identical at (%i %i %i): (%f %f %f) (%f %f %f) (%f %f %f)"
//									"\nFor Cube [%f %f] [%f %f] [%f %f] [%f %f] [%f %f] [%f %f] [%f %f] [%f %f]",
//									x,y,z,v0.x,v0.y,v0.z,v1.x,v1.y,v1.z,v2.x,v2.y,v2.z,
//									val0,(float)w0,val1,(float)w1,val2,(float)w2,val3,(float)w3,
//									val4,(float)w4,val5,(float)w5,val6,(float)w6,val7,(float)w7);
						}
						else{
							mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
							mesh.f.push_back(mesh.x.size()-1);
							mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
							mesh.f.push_back(mesh.x.size()-1);
							mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
							mesh.f.push_back(mesh.x.size()-1);

							if(useColor){
								VertexColor c0 = collist[table[i  ]];
								VertexColor c1 = collist[table[i+1]];
								VertexColor c2 = collist[table[i+2]];
								mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
								mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
								mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
							}
						}

					}
				}
			}
		}
	}
	if(degenerateFaces) *degenerateFaces = degenerate_faces;
}


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
		unsigned int *degenerateFaces
)
{
	float scaleInBrick = scaleToBrick*leafScale;

	if(useColor && ((color && (r || g || b)) || ((!color) && (!r || !g || !b)))){
		fprintf(stderr,"\n\nERROR:\nYou should choose either interleaved color or separate red, green and blue!\n\n");
		return;
	}

	Vertex3f vertlist[12];
	VertexColor collist[12];
	MarchingCubes mc;
	unsigned int degenerate_faces = 0;
	sidetype inc = 0;

	sidetype addLength = brickLength*leafScale;
	sidetype neighborLength;


	volumetype neighborX = (ox<n-1-addLength) ?
			getBrickIndexMultiscale(ox+addLength+inc,oy+inc,oz+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
	if(neighborLength != addLength) neighborX = MAXLEAFNUMBER;
	volumetype neighborY = (oy<n-1-addLength) ?
			getBrickIndexMultiscale(ox+inc,oy+addLength+inc,oz+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
	if(neighborLength != addLength) neighborY = MAXLEAFNUMBER;
	volumetype neighborZ = (oz<n-1-addLength) ?
			getBrickIndexMultiscale(ox+inc,oy+inc,oz+addLength+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
	if(neighborLength != addLength) neighborZ = MAXLEAFNUMBER;
	volumetype neighborXY = (ox<n-1-addLength && oy<n-1-addLength) ?
			getBrickIndexMultiscale(ox+addLength+inc,oy+addLength+inc,oz+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
	if(neighborLength != addLength) neighborXY = MAXLEAFNUMBER;
	volumetype neighborXZ = (ox<n-1-addLength && oz<n-1-addLength) ?
			getBrickIndexMultiscale(ox+addLength+inc,oy+inc,oz+addLength+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
	if(neighborLength != addLength) neighborXZ = MAXLEAFNUMBER;
	volumetype neighborYZ = (oy<n-1-addLength && oz<n-1-addLength) ?
			getBrickIndexMultiscale(ox+inc,oy+addLength+inc,oz+addLength+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
	if(neighborLength != addLength) neighborYZ = MAXLEAFNUMBER;
	volumetype neighborXYZ = (ox<n-1-addLength && oy<n-1-addLength && oz<n-1-addLength) ?
			getBrickIndexMultiscale(ox+addLength+inc,oy+addLength+inc,oz+addLength+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
	if(neighborLength != addLength) neighborXYZ = MAXLEAFNUMBER;

//	volumetype neighborX = MAXLEAFNUMBER;
//	volumetype neighborY = MAXLEAFNUMBER;
//	volumetype neighborZ = MAXLEAFNUMBER;
//	volumetype neighborXY = MAXLEAFNUMBER;
//	volumetype neighborXZ = MAXLEAFNUMBER;
//	volumetype neighborYZ = MAXLEAFNUMBER;
//	volumetype neighborXYZ = MAXLEAFNUMBER;


	for(sidetype x=0;x<brickLength;x++){
		for(sidetype y=0;y<brickLength;y++){
			for(sidetype z=0;z<brickLength;z++){
				distanceBrick[(z*blp1+y)*blp1+x] = distance[brickIdx*brickSize+(z*brickLength+y)*brickLength+x];
				weightsBrick[(z*blp1+y)*blp1+x] = weights[brickIdx*brickSize+(z*brickLength+y)*brickLength+x];
				//Brick Boundaries
				//X
				if(x==blm1){
					distanceBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
							distance[neighborX*brickSize+(z*brickLength+y)*brickLength] : 0.0f;
					weightsBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
							weights[neighborX*brickSize+(z*brickLength+y)*brickLength] : 0.0f;
					//XY
					if(y==blm1){
						distanceBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
								distance[neighborXY*brickSize+z*brickLength*brickLength] : 0.0f;
						weightsBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
								weights[neighborXY*brickSize+z*brickLength*brickLength] : 0.0f;
						//XYZ
						if(z==blm1){
							distanceBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
									distance[neighborXYZ*brickSize] : 0.0f;
							weightsBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
									weights[neighborXYZ*brickSize] : 0.0f;
						}
					}
					//XZ
					if(z==blm1){
						distanceBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
								distance[neighborXZ*brickSize+y*brickLength] : 0.0f;
						weightsBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
								weights[neighborXZ*brickSize+y*brickLength] : 0.0f;
					}
				}
				//Y
				if(y==blm1){
					distanceBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
							distance[neighborY*brickSize+z*brickLength*brickLength+x] : 0.0f;
					weightsBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
							weights[neighborY*brickSize+z*brickLength*brickLength+x] : 0.0f;
					//YZ
					if(z==blm1){
						distanceBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
								distance[neighborYZ*brickSize+x] : 0.0f;
						weightsBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
								weights[neighborYZ*brickSize+x] : 0.0f;
					}
				}
				//Z
				if(z==blm1){
					distanceBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
							distance[neighborZ*brickSize+y*brickLength+x] : 0.0f;
					weightsBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
							weights[neighborZ*brickSize+y*brickLength+x] : 0.0f;
				}
				if(useColor){
					volumetype idx = brickIdx*brickSize+(z*brickLength+y)*brickLength+x;
					rBrick[(z*blp1+y)*blp1+x] = r ? r[idx] : color[3*(idx)+0];
					gBrick[(z*blp1+y)*blp1+x] = g ? g[idx] : color[3*(idx)+1];
					bBrick[(z*blp1+y)*blp1+x] = b ? b[idx] : color[3*(idx)+2];
					//Brick Boundaries
					//X
					if(x==blm1){
						volumetype idx = neighborX*brickSize+(z*brickLength+y)*brickLength;
						rBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
								(r ? r[idx] : color[3*idx+0]) : 0;
						gBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
								(g ? g[idx] : color[3*idx+1]) : 0;
						bBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
								(b ? b[idx] : color[3*idx+2]) : 0;
						//XY
						if(y==blm1){
							volumetype idx = neighborXY*brickSize+z*brickLength*brickLength;
							rBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
									(r ? r[idx] : color[3*idx+0]) : 0;
							gBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
									(g ? g[idx] : color[3*idx+1]) : 0;
							bBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
									(b ? b[idx] : color[3*idx+2]) : 0;
							//XYZ
							if(z==blm1){
								volumetype idx = neighborXYZ*brickSize;
								rBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
										(r ? r[idx] : color[3*idx+0]) : 0;
								gBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
										(g ? g[idx] : color[3*idx+1]) : 0;
								bBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
										(b ? b[idx] : color[3*idx+2]) : 0;
							}
						}
						//XZ
						if(z==blm1){
							volumetype idx = neighborXZ*brickSize+y*brickLength;
							rBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
									(r ? r[idx] : color[3*idx+0]) : 0;
							gBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
									(g ? g[idx] : color[3*idx+1]) : 0;
							bBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
									(b ? b[idx] : color[3*idx+2]) : 0;
						}
					}
					//Y
					if(y==blm1){
						volumetype idx = neighborY*brickSize+z*brickLength*brickLength+x;
						rBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
								(r ? r[idx] : color[3*idx+0]) : 0;
						gBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
								(g ? g[idx] : color[3*idx+1]) : 0;
						bBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
								(b ? b[idx] : color[3*idx+2]) : 0;
						//YZ
						if(z==blm1){
							volumetype idx = neighborYZ*brickSize+x;
							rBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
									(r ? r[idx] : color[3*idx+0]) : 0;
							gBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
									(g ? g[idx] : color[3*idx+1]) : 0;
							bBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
									(b ? b[idx] : color[3*idx+2]) : 0;
						}
					}
					//Z
					if(z==blm1){
						volumetype idx = neighborZ*brickSize+y*brickLength+x;
						rBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
								(r ? r[idx] : color[3*idx+0]) : 0;
						gBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
								(g ? g[idx] : color[3*idx+1]) : 0;
						bBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
								(b ? b[idx] : color[3*idx+2]) : 0;
					}
				}
			}
		}
	}


	for(sidetype x=0;x<brickLength;x++){
		for(sidetype y=0;y<brickLength;y++){
			for(sidetype z=0;z<brickLength;z++){
				if(weightsBrick[(z*blp1+y)*blp1+x]>=std::min((double)framesAdded,MIN_WEIGHT_FOR_SURFACE)){

					volumetype idx0 = (z*blp1+y)*blp1+x;
					volumetype idx1 = (z*blp1+y)*blp1+x+1;
					volumetype idx2 = (z*blp1+y+1)*blp1+x+1;
					volumetype idx3 = (z*blp1+y+1)*blp1+x;
					volumetype idx4 = ((z+1)*blp1+y)*blp1+x;
					volumetype idx5 = ((z+1)*blp1+y)*blp1+x+1;
					volumetype idx6 = ((z+1)*blp1+y+1)*blp1+x+1;
					volumetype idx7 = ((z+1)*blp1+y+1)*blp1+x;
					if(idx0>=MAXINDEX||idx1>=MAXINDEX||idx2>=MAXINDEX||idx3>=MAXINDEX||
						 idx4>=MAXINDEX||idx5>=MAXINDEX||idx6>=MAXINDEX||idx7>=MAXINDEX){
						continue;
					}


					int *table = mc.getColoredTrianglesForCube(vertlist,collist,x,y,z,
							offsetX+ox*scaleToBrick,offsetY+oy*scaleToBrick,offsetZ+oz*scaleToBrick,scaleInBrick,
							idx0,idx1,idx2,idx3,idx4,idx5,idx6,idx7,distanceBrick,weightsBrick,rBrick,gBrick,bBrick);


					for (unsigned int i=0;table[i]!=-1;i+=3) {
						Vertex3f v0 = vertlist[table[i  ]];
						Vertex3f v1 = vertlist[table[i+1]];
						Vertex3f v2 = vertlist[table[i+2]];
						if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
							degenerate_faces++;

						}
						else{
							mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
							mesh.f.push_back(mesh.x.size()-1);
							mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
							mesh.f.push_back(mesh.x.size()-1);
							mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
							mesh.f.push_back(mesh.x.size()-1);

							if(useColor){
								VertexColor c0 = collist[table[i  ]];
								VertexColor c1 = collist[table[i+1]];
								VertexColor c2 = collist[table[i+2]];
								mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
								mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
								mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
							}
						}

					}
				}
			}
		}
	}
	if(degenerateFaces) *degenerateFaces = degenerate_faces;
}



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
)
{
	float scaleInBrick = scaleToBrick*leafScale;

	if(useColor && ((color && (r || g || b)) || ((!color) && (!r || !g || !b)))){
		fprintf(stderr,"\n\nERROR:\nYou should choose either interleaved color or separate red, green and blue!\n\n");
		return;
	}

	Vertex3f vertlist[12];
	VertexColor collist[12];
	MarchingCubes mc;
	unsigned int degenerate_faces = 0;
	sidetype inc = 0;

	sidetype addLength = brickLength*leafScale;
	sidetype neighborLength;


	volumetype neighborX = (ox<n-1-addLength) ?
			getBrickIndexMipMap(ox+addLength+inc,oy+inc,oz+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
//	if(neighborLength != addLength) neighborX = MAXLEAFNUMBER;
	volumetype neighborY = (oy<n-1-addLength) ?
			getBrickIndexMipMap(ox+inc,oy+addLength+inc,oz+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
//	if(neighborLength != addLength) neighborY = MAXLEAFNUMBER;
	volumetype neighborZ = (oz<n-1-addLength) ?
			getBrickIndexMipMap(ox+inc,oy+inc,oz+addLength+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
//	if(neighborLength != addLength) neighborZ = MAXLEAFNUMBER;
	volumetype neighborXY = (ox<n-1-addLength && oy<n-1-addLength) ?
			getBrickIndexMipMap(ox+addLength+inc,oy+addLength+inc,oz+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
//	if(neighborLength != addLength) neighborXY = MAXLEAFNUMBER;
	volumetype neighborXZ = (ox<n-1-addLength && oz<n-1-addLength) ?
			getBrickIndexMipMap(ox+addLength+inc,oy+inc,oz+addLength+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
//	if(neighborLength != addLength) neighborXZ = MAXLEAFNUMBER;
	volumetype neighborYZ = (oy<n-1-addLength && oz<n-1-addLength) ?
			getBrickIndexMipMap(ox+inc,oy+addLength+inc,oz+addLength+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
//	if(neighborLength != addLength) neighborYZ = MAXLEAFNUMBER;
	volumetype neighborXYZ = (ox<n-1-addLength && oy<n-1-addLength && oz<n-1-addLength) ?
			getBrickIndexMipMap(ox+addLength+inc,oy+addLength+inc,oz+addLength+inc,
			n,addLength,brickSize,nNodes,nLeavesUsed,tree,&neighborLength) : MAXLEAFNUMBER;
//	if(neighborLength != addLength) neighborXYZ = MAXLEAFNUMBER;

//	volumetype neighborX = MAXLEAFNUMBER;
//	volumetype neighborY = MAXLEAFNUMBER;
//	volumetype neighborZ = MAXLEAFNUMBER;
//	volumetype neighborXY = MAXLEAFNUMBER;
//	volumetype neighborXZ = MAXLEAFNUMBER;
//	volumetype neighborYZ = MAXLEAFNUMBER;
//	volumetype neighborXYZ = MAXLEAFNUMBER;


	for(sidetype x=0;x<brickLength;x++){
		for(sidetype y=0;y<brickLength;y++){
			for(sidetype z=0;z<brickLength;z++){
				distanceBrick[(z*blp1+y)*blp1+x] = distance[brickIdx*brickSize+(z*brickLength+y)*brickLength+x];
				weightsBrick[(z*blp1+y)*blp1+x] = weights[brickIdx*brickSize+(z*brickLength+y)*brickLength+x];
				//Brick Boundaries
				//X
				if(x==blm1){
					distanceBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
							distance[neighborX*brickSize+(z*brickLength+y)*brickLength] : 0.0f;
					weightsBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
							weights[neighborX*brickSize+(z*brickLength+y)*brickLength] : 0.0f;
					//XY
					if(y==blm1){
						distanceBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
								distance[neighborXY*brickSize+z*brickLength*brickLength] : 0.0f;
						weightsBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
								weights[neighborXY*brickSize+z*brickLength*brickLength] : 0.0f;
						//XYZ
						if(z==blm1){
							distanceBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
									distance[neighborXYZ*brickSize] : 0.0f;
							weightsBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
									weights[neighborXYZ*brickSize] : 0.0f;
						}
					}
					//XZ
					if(z==blm1){
						distanceBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
								distance[neighborXZ*brickSize+y*brickLength] : 0.0f;
						weightsBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
								weights[neighborXZ*brickSize+y*brickLength] : 0.0f;
					}
				}
				//Y
				if(y==blm1){
					distanceBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
							distance[neighborY*brickSize+z*brickLength*brickLength+x] : 0.0f;
					weightsBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
							weights[neighborY*brickSize+z*brickLength*brickLength+x] : 0.0f;
					//YZ
					if(z==blm1){
						distanceBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
								distance[neighborYZ*brickSize+x] : 0.0f;
						weightsBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
								weights[neighborYZ*brickSize+x] : 0.0f;
					}
				}
				//Z
				if(z==blm1){
					distanceBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
							distance[neighborZ*brickSize+y*brickLength+x] : 0.0f;
					weightsBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
							weights[neighborZ*brickSize+y*brickLength+x] : 0.0f;
				}
				if(useColor){
					volumetype idx = brickIdx*brickSize+(z*brickLength+y)*brickLength+x;
					rBrick[(z*blp1+y)*blp1+x] = r ? r[idx] : color[3*(idx)+0];
					gBrick[(z*blp1+y)*blp1+x] = g ? g[idx] : color[3*(idx)+1];
					bBrick[(z*blp1+y)*blp1+x] = b ? b[idx] : color[3*(idx)+2];
					//Brick Boundaries
					//X
					if(x==blm1){
						volumetype idx = neighborX*brickSize+(z*brickLength+y)*brickLength;
						rBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
								(r ? r[idx] : color[3*idx+0]) : 0;
						gBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
								(g ? g[idx] : color[3*idx+1]) : 0;
						bBrick[(z*blp1+y)*blp1+x+1] = (neighborX<MAXLEAFNUMBER) ?
								(b ? b[idx] : color[3*idx+2]) : 0;
						//XY
						if(y==blm1){
							volumetype idx = neighborXY*brickSize+z*brickLength*brickLength;
							rBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
									(r ? r[idx] : color[3*idx+0]) : 0;
							gBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
									(g ? g[idx] : color[3*idx+1]) : 0;
							bBrick[(z*blp1+y+1)*blp1+x+1] = (neighborXY<MAXLEAFNUMBER) ?
									(b ? b[idx] : color[3*idx+2]) : 0;
							//XYZ
							if(z==blm1){
								volumetype idx = neighborXYZ*brickSize;
								rBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
										(r ? r[idx] : color[3*idx+0]) : 0;
								gBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
										(g ? g[idx] : color[3*idx+1]) : 0;
								bBrick[((z+1)*blp1+y+1)*blp1+x+1] = (neighborXYZ<MAXLEAFNUMBER) ?
										(b ? b[idx] : color[3*idx+2]) : 0;
							}
						}
						//XZ
						if(z==blm1){
							volumetype idx = neighborXZ*brickSize+y*brickLength;
							rBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
									(r ? r[idx] : color[3*idx+0]) : 0;
							gBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
									(g ? g[idx] : color[3*idx+1]) : 0;
							bBrick[((z+1)*blp1+y)*blp1+x+1] = (neighborXZ<MAXLEAFNUMBER) ?
									(b ? b[idx] : color[3*idx+2]) : 0;
						}
					}
					//Y
					if(y==blm1){
						volumetype idx = neighborY*brickSize+z*brickLength*brickLength+x;
						rBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
								(r ? r[idx] : color[3*idx+0]) : 0;
						gBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
								(g ? g[idx] : color[3*idx+1]) : 0;
						bBrick[(z*blp1+y+1)*blp1+x] = (neighborY<MAXLEAFNUMBER) ?
								(b ? b[idx] : color[3*idx+2]) : 0;
						//YZ
						if(z==blm1){
							volumetype idx = neighborYZ*brickSize+x;
							rBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
									(r ? r[idx] : color[3*idx+0]) : 0;
							gBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
									(g ? g[idx] : color[3*idx+1]) : 0;
							bBrick[((z+1)*blp1+y+1)*blp1+x] = (neighborYZ<MAXLEAFNUMBER) ?
									(b ? b[idx] : color[3*idx+2]) : 0;
						}
					}
					//Z
					if(z==blm1){
						volumetype idx = neighborZ*brickSize+y*brickLength+x;
						rBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
								(r ? r[idx] : color[3*idx+0]) : 0;
						gBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
								(g ? g[idx] : color[3*idx+1]) : 0;
						bBrick[((z+1)*blp1+y)*blp1+x] = (neighborZ<MAXLEAFNUMBER) ?
								(b ? b[idx] : color[3*idx+2]) : 0;
					}
				}
			}
		}
	}


	for(sidetype x=0;x<brickLength;x++){
		for(sidetype y=0;y<brickLength;y++){
			for(sidetype z=0;z<brickLength;z++){
				if(weightsBrick[(z*blp1+y)*blp1+x]>=std::min((double)framesAdded,MIN_WEIGHT_FOR_SURFACE)){

					volumetype idx0 = (z*blp1+y)*blp1+x;
					volumetype idx1 = (z*blp1+y)*blp1+x+1;
					volumetype idx2 = (z*blp1+y+1)*blp1+x+1;
					volumetype idx3 = (z*blp1+y+1)*blp1+x;
					volumetype idx4 = ((z+1)*blp1+y)*blp1+x;
					volumetype idx5 = ((z+1)*blp1+y)*blp1+x+1;
					volumetype idx6 = ((z+1)*blp1+y+1)*blp1+x+1;
					volumetype idx7 = ((z+1)*blp1+y+1)*blp1+x;
					if(idx0>=MAXINDEX||idx1>=MAXINDEX||idx2>=MAXINDEX||idx3>=MAXINDEX||
						 idx4>=MAXINDEX||idx5>=MAXINDEX||idx6>=MAXINDEX||idx7>=MAXINDEX){
						continue;
					}


					int *table = mc.getColoredTrianglesForCube(vertlist,collist,x,y,z,
							offsetX+ox*scaleToBrick,offsetY+oy*scaleToBrick,offsetZ+oz*scaleToBrick,scaleInBrick,
							idx0,idx1,idx2,idx3,idx4,idx5,idx6,idx7,distanceBrick,weightsBrick,rBrick,gBrick,bBrick);


					for (unsigned int i=0;table[i]!=-1;i+=3) {
						Vertex3f v0 = vertlist[table[i  ]];
						Vertex3f v1 = vertlist[table[i+1]];
						Vertex3f v2 = vertlist[table[i+2]];
						if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
							degenerate_faces++;

						}
						else{
							mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
							mesh.f.push_back(mesh.x.size()-1);
							mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
							mesh.f.push_back(mesh.x.size()-1);
							mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
							mesh.f.push_back(mesh.x.size()-1);

							if(useColor){
								VertexColor c0 = collist[table[i  ]];
								VertexColor c1 = collist[table[i+1]];
								VertexColor c2 = collist[table[i+2]];
								mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
								mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
								mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
							}
						}

					}
				}
			}
		}
	}
	if(degenerateFaces) *degenerateFaces = degenerate_faces;
}




void getTreeLinesMiddle(volumetype *tree, volumetype idx, sidetype brickLength,
		sidetype half, sidetype ox, sidetype oy, sidetype oz, MeshSeparate &mesh)
{
	for(sidetype childoffset=0;childoffset<8;childoffset++){
		if(half>brickLength){
			if(tree[idx+childoffset]!=BRANCHINIT){
				mesh.x.push_back(ox+half); mesh.x.push_back(ox+half*((childoffset&1)!=0)+half/2);
				mesh.y.push_back(oy+half); mesh.y.push_back(oy+half*((childoffset&2)!=0)+half/2);
				mesh.z.push_back(oz+half); mesh.z.push_back(oz+half*((childoffset&4)!=0)+half/2);
				mesh.e.push_back(mesh.x.size()-2); mesh.e.push_back(mesh.x.size()-1);
				getTreeLinesMiddle(tree,tree[idx+childoffset],brickLength,half/2, ox+half*((childoffset&1)!=0),
						oy+half*((childoffset&2)!=0), oz+half*((childoffset&4)!=0),mesh);
			}
		}
		else if(tree[idx+childoffset]!=MAXLEAFNUMBER){
			mesh.x.push_back(ox+half); mesh.x.push_back(ox+ half * ((childoffset & 1) != 0)+half/2);
			mesh.y.push_back(oy+half); mesh.y.push_back(oy+ half * ((childoffset & 2) != 0)+half/2);
			mesh.z.push_back(oz+half); mesh.z.push_back(oz+ half * ((childoffset & 4) != 0)+half/2);
			mesh.e.push_back(mesh.x.size()-2); mesh.e.push_back(mesh.x.size()-1);
		}
	}
}

void getTreeLinesBox(volumetype *tree, volumetype idx, sidetype brickLength,
		volumetype nBranchesTotal,
		sidetype half, sidetype ox, sidetype oy, sidetype oz, MeshSeparate &mesh, float width)
{
	mesh.addAlignedLineCube(ox,oy,oz,half*2,half*2,half*2,std::max(width,0.25f));
	for(sidetype childoffset=0;childoffset<8;childoffset++){
		if(half>brickLength){
			if(tree[idx+childoffset]<nBranchesTotal){
				getTreeLinesBox(tree,tree[idx+childoffset],brickLength,nBranchesTotal,half/2, ox+half*((childoffset&1)!=0),
						oy+half*((childoffset&2)!=0), oz+half*((childoffset&4)!=0),mesh,width-1.0f);
			}
		}
		else if(tree[idx+childoffset]!=MAXLEAFNUMBER){
			mesh.addAlignedLineCube(ox+half*((childoffset&1)!=0),oy+half*((childoffset&2)!=0),oz+half*((childoffset&4)!=0),half,half,half,std::max(width-1.0f,0.25f));
		}
	}
}

void getTreeLinesBoxMipMap(volumetype *tree, volumetype idx, sidetype brickLength,
		volumetype nBranchesTotal,
		sidetype half, sidetype ox, sidetype oy, sidetype oz, MeshSeparate &mesh, float width)
{
	mesh.addAlignedLineCube(ox,oy,oz,half*2,half*2,half*2,std::max(width,0.25f));
	for(sidetype childoffset=0;childoffset<8;childoffset++){
		if(half>brickLength){
			if(tree[idx+childoffset+8]<nBranchesTotal){
				getTreeLinesBoxMipMap(tree,tree[idx+childoffset+8],brickLength,nBranchesTotal,half/2, ox+half*((childoffset&1)!=0),
						oy+half*((childoffset&2)!=0), oz+half*((childoffset&4)!=0),mesh,width-1.0f);
			}
		}
		else if(tree[idx+childoffset]!=MAXLEAFNUMBER){
			mesh.addAlignedLineCube(ox+half*((childoffset&1)!=0),oy+half*((childoffset&2)!=0),oz+half*((childoffset&4)!=0),half,half,half,std::max(width-1.0f,0.25f));
		}
	}
}

void getTreeQuads(volumetype *tree, volumetype idx, sidetype brickLength,
		sidetype half, sidetype ox, sidetype oy, sidetype oz,
		MeshSeparate &mesh)
{
	for(sidetype childoffset=0;childoffset<8;childoffset++){
		if(half>brickLength){
			if(tree[idx+childoffset]!=BRANCHINIT){
				getTreeQuads(tree,tree[idx+childoffset],brickLength,half/2, ox+half*((childoffset&1)!=0),
						oy+half*((childoffset&2)!=0), oz+half*((childoffset&4)!=0),mesh);
			}
		}
		else if(tree[idx+childoffset]!=MAXLEAFNUMBER){

			mesh.addGridFace(0,ox+half/4+half*((childoffset&1)!=0),oy+half/4+half*((childoffset&2)!=0),oz+half/4+half*((childoffset&4)!=0),half/2,half/2,half/2);
			mesh.addGridFace(1,ox+half/4+half*((childoffset&1)!=0),oy+half/4+half*((childoffset&2)!=0),oz+half/4+half*((childoffset&4)!=0),half/2,half/2,half/2);
			mesh.addGridFace(2,ox+half/4+half*((childoffset&1)!=0),oy+half/4+half*((childoffset&2)!=0),oz+half/4+half*((childoffset&4)!=0),half/2,half/2,half/2);
			mesh.addGridFace(0,ox+half/4*3+half*((childoffset&1)!=0),oy+half/4+half*((childoffset&2)!=0),oz+half/4+half*((childoffset&4)!=0),half/2,half/2,half/2);
			mesh.addGridFace(1,ox+half/4+half*((childoffset&1)!=0),oy+half/4*3+half*((childoffset&2)!=0),oz+half/4+half*((childoffset&4)!=0),half/2,half/2,half/2);
			mesh.addGridFace(2,ox+half/4+half*((childoffset&1)!=0),oy+half/4+half*((childoffset&2)!=0),oz+half/4*3+half*((childoffset&4)!=0),half/2,half/2,half/2);

		}
	}
}

bool weightInfluence(float minWeight,
weighttype w0, weighttype w1, weighttype w2, weighttype w3,
weighttype w4, weighttype w5, weighttype w6, weighttype w7
)
{
	bool result = w0>=minWeight

//			&& w1>0.0f
//			&& w2>0.0f
//			&& w3>0.0f
//			&& w4>0.0f
//			&& w5>0.0f
//			&& w6>0.0f
//			&& w7>0.0f

//			&& w1>=minWeight
//			&& w2>=minWeight
//			&& w3>=minWeight
//			&& w4>=minWeight
//			&& w5>=minWeight
//			&& w6>=minWeight
//			&& w7>=minWeight
			;

//	if(!result) fprintf(stderr," w(%f %f %f %f %f %f %f %f)",w0,w1,w2,w3,w4,w5,w6,w7);
	return result;
//	return true;
}




void addBrickInterior
(
		leafstack leaves,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		treeinfo info,
		const MarchingCubesIndexed &mc
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];

	weighttype wf[8]; float df[8]; colortype3 cf[8];
	Vertex3f vertlist[12];
	VertexColor collist[12];

//	fprintf(stderr,"\n(%i %i %i | %i):[ ",ox,oy,oz,size);
//	for(unsigned int i=0;i<leaves.size();i++) fprintf(stderr,"%i%",leaves[i]);
//	fprintf(stderr,"]");

	sidetype sizeMin = leafScale[leaves[leaves.size()-1]];
//	fprintf(stderr,"->%i",sizeMin);
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			for(sidetype x=ox;x<ox+size;x+=sizeMin){
				sidetype bx = (x-ox)/sizeMin;
				volumetype idx = (bz*bl+by)*bl+bx;
				w[idx] = 0;

				int level;
				for(level = leaves.size()-1;level>=0 && w[idx]<=0.0f;level--){
					volumetype leaf = leaves[level];
					volumetype start = leaf*brickSize;
					sidetype3 lo = leafPos[leaf];
					sidetype ls = leafScale[leaf];

					sidetype lxl = (x-lo.x)/ls;
					sidetype lxr = (x-lo.x)%ls;
					float rx = (float)lxr/(float)ls;
					sidetype lxh = lxl+(rx>0.0 && lxl<bl-1);

					sidetype lyl = (y-lo.y)/ls;
					sidetype lyr = (y-lo.y)%ls;
					float ry = (float)lyr/(float)ls;
					sidetype lyh = lyl+(ry>0.0 && lyl<bl-1);

					sidetype lzl = (z-lo.z)/ls;
					sidetype lzr = (z-lo.z)%ls;
					float rz = (float)lzr/(float)ls;
					sidetype lzh = lzl+(rz>0.0 && lzl<bl-1);

					if(level==(int)(leaves.size())-1 && (rx || ry || rz))
						fprintf(stderr,"\nERROR: Finest Size does not match: "
							"(%i %i %i):(%i %i) (%i %i %i): %i  - (%i %i %i) (%f %f %f)",
							ox,oy,oz,size,sizeMin,lo.x,lo.y,lo.z,ls,x,y,z,rx,ry,rz);

					volumetype idxLeaf[8] = {
							(volumetype)((lzl*bl+lyl)*bl+lxl),
							(volumetype)((lzl*bl+lyl)*bl+lxh),
							(volumetype)((lzl*bl+lyh)*bl+lxl),
							(volumetype)((lzl*bl+lyh)*bl+lxh),
							(volumetype)((lzh*bl+lyl)*bl+lxl),
							(volumetype)((lzh*bl+lyl)*bl+lxh),
							(volumetype)((lzh*bl+lyh)*bl+lxl),
							(volumetype)((lzh*bl+lyh)*bl+lxh)
					};

					for(volumetype i=0;i<8;i++) {
						wf[i] = weights[start+idxLeaf[i]];
						df[i] = distance[start+idxLeaf[i]];
						if(color) cf[i] = color[start+idxLeaf[i]];
					}

					w[idx] = (1.0f-rz)*(1.0f-ry)*(1.0f-rx)*(float)wf[0]+
							     (1.0f-rz)*(1.0f-ry)*(     rx)*(float)wf[1]+
							     (1.0f-rz)*(     ry)*(1.0f-rx)*(float)wf[2]+
								   (1.0f-rz)*(     ry)*(     rx)*(float)wf[3]+
								   (     rz)*(1.0f-ry)*(1.0f-rx)*(float)wf[4]+
									 (     rz)*(1.0f-ry)*(     rx)*(float)wf[5]+
									 (     rz)*(     ry)*(1.0f-rx)*(float)wf[6]+
									 (     rz)*(     ry)*(     rx)*(float)wf[7];

					d[idx] = (1.0f-rz)*(1.0f-ry)*(1.0f-rx)*df[0]+
							     (1.0f-rz)*(1.0f-ry)*(     rx)*df[1]+
							     (1.0f-rz)*(     ry)*(1.0f-rx)*df[2]+
								   (1.0f-rz)*(     ry)*(     rx)*df[3]+
								   (     rz)*(1.0f-ry)*(1.0f-rx)*df[4]+
									 (     rz)*(1.0f-ry)*(     rx)*df[5]+
									 (     rz)*(     ry)*(1.0f-rx)*df[6]+
									 (     rz)*(     ry)*(     rx)*df[7];

					if(color)
						c[idx] = VertexColor(
								     (1.0f-rz)*(1.0f-ry)*(1.0f-rx)*(float)cf[0].x+
								     (1.0f-rz)*(1.0f-ry)*(     rx)*(float)cf[1].x+
								     (1.0f-rz)*(     ry)*(1.0f-rx)*(float)cf[2].x+
									   (1.0f-rz)*(     ry)*(     rx)*(float)cf[3].x+
									   (     rz)*(1.0f-ry)*(1.0f-rx)*(float)cf[4].x+
										 (     rz)*(1.0f-ry)*(     rx)*(float)cf[5].x+
										 (     rz)*(     ry)*(1.0f-rx)*(float)cf[6].x+
										 (     rz)*(     ry)*(     rx)*(float)cf[7].x,
								     (1.0f-rz)*(1.0f-ry)*(1.0f-rx)*(float)cf[0].y+
								     (1.0f-rz)*(1.0f-ry)*(     rx)*(float)cf[1].y+
								     (1.0f-rz)*(     ry)*(1.0f-rx)*(float)cf[2].y+
									   (1.0f-rz)*(     ry)*(     rx)*(float)cf[3].y+
									   (     rz)*(1.0f-ry)*(1.0f-rx)*(float)cf[4].y+
										 (     rz)*(1.0f-ry)*(     rx)*(float)cf[5].y+
										 (     rz)*(     ry)*(1.0f-rx)*(float)cf[6].y+
										 (     rz)*(     ry)*(     rx)*(float)cf[7].y,
								     (1.0f-rz)*(1.0f-ry)*(1.0f-rx)*(float)cf[0].z+
								     (1.0f-rz)*(1.0f-ry)*(     rx)*(float)cf[1].z+
								     (1.0f-rz)*(     ry)*(1.0f-rx)*(float)cf[2].z+
									   (1.0f-rz)*(     ry)*(     rx)*(float)cf[3].z+
									   (     rz)*(1.0f-ry)*(1.0f-rx)*(float)cf[4].z+
										 (     rz)*(1.0f-ry)*(     rx)*(float)cf[5].z+
										 (     rz)*(     ry)*(1.0f-rx)*(float)cf[6].z+
										 (     rz)*(     ry)*(     rx)*(float)cf[7].z);
				}
//				if((unsigned int)level<leaves.size()-2 && w[idx]>0.0f) w[idx] = minWeight;
			}
		}
	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
				sidetype bx = (x-ox)/sizeMin;
				volumetype idx = (bz*bl+by)*bl+bx;


				if(weightInfluence(minWeight,
						w[idx],w[idx+1],
						w[idx+bl],w[idx+bl+1],
						w[idx+bl*bl],w[idx+bl*bl+1],
						w[idx+bl*bl+bl],w[idx+bl*bl+bl+1])){
					int *table = mc.getColoredTrianglesForCubePrecomputed(vertlist,collist,0,0,0,
							offset.x+x*scale,offset.y+y*scale,offset.z+z*scale,
							scale*sizeMin,scale*sizeMin,scale*sizeMin,
							d[idx],d[idx+1],
							d[idx+bl+1],d[idx+bl],
							d[idx+bl*bl],d[idx+bl*bl+1],
							d[idx+bl*bl+bl+1],d[idx+bl*bl+bl],
							w[idx],w[idx+1],
							w[idx+bl+1],w[idx+bl],
							w[idx+bl*bl],w[idx+bl*bl+1],
							w[idx+bl*bl+bl+1],w[idx+bl*bl+bl],
							c[idx],c[idx+1],
							c[idx+bl+1],c[idx+bl],
							c[idx+bl*bl],c[idx+bl*bl+1],
							c[idx+bl*bl+bl+1],c[idx+bl*bl+bl],
							color);

					for (unsigned int i=0;table[i]!=-1;i+=3) {
						Vertex3f v0 = vertlist[table[i  ]];
						Vertex3f v1 = vertlist[table[i+1]];
						Vertex3f v2 = vertlist[table[i+2]];
						if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
							degenerate_faces++;
						}
						else{
							mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
							mesh.f.push_back(mesh.x.size()-1);
							mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
							mesh.f.push_back(mesh.x.size()-1);
							mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
							mesh.f.push_back(mesh.x.size()-1);

							if(color){
								VertexColor c0 = collist[table[i  ]];
								VertexColor c1 = collist[table[i+1]];
								VertexColor c2 = collist[table[i+2]];
								mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
								mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
								mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
								mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
								mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
							}
						}
					}
				}
			}
		}
	}

	delete [] d; delete [] w; delete [] c;
}









void addBrickMiddle
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3,
		leafstack leaves4, leafstack leaves5, leafstack leaves6, leafstack leaves7
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	float d[8];
	weighttype w[8] = {0,0,0,0,0,0,0,0};
	VertexColor c[8];

	Vertex3f vertlist[12];
	VertexColor collist[12];
	MarchingCubes mc;


	leafstack stacks[8] = {leaves0,leaves1,leaves2,leaves3,
			leaves4,leaves5,leaves6,leaves7};

	sidetype sizeStretch = leafScale[stacks[0][stacks[0].size()-1]];
	for(unsigned int i=1;i<7;i++){
		if(sizeStretch < leafScale[stacks[i][stacks[i].size()-1]])
			 sizeStretch = leafScale[stacks[i][stacks[i].size()-1]];
	}

	sidetype x[8] = {
			(sidetype)(ox+size-sizeStretch), (sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch), (sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch), (sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch), (sidetype)(ox+size)
	};

	sidetype y[8] = {
			(sidetype)(oy+size-sizeStretch), (sidetype)(oy+size-sizeStretch),
			(sidetype)(oy+size), (sidetype)(oy+size),
			(sidetype)(oy+size-sizeStretch), (sidetype)(oy+size-sizeStretch),
			(sidetype)(oy+size), (sidetype)(oy+size)
	};

	sidetype z[8] = {
			(sidetype)(oz+size-sizeStretch), (sidetype)(oz+size-sizeStretch),
			(sidetype)(oz+size-sizeStretch), (sidetype)(oz+size-sizeStretch),
			(sidetype)(oz+size), (sidetype)(oz+size),
			(sidetype)(oz+size), (sidetype)(oz+size)
	};

	for(unsigned int i=0;i<8;i++){
		int level;
		for(level = stacks[i].size()-1;level>=0 && w[i]<=0.0;level--){
			volumetype leaf = stacks[i][level];
			volumetype start = leaf*brickSize;
			sidetype3 lo = leafPos[leaf];
			sidetype ls = leafScale[leaf];


			sidetype lx = (x[i]-lo.x)/ls;
			sidetype ly = (y[i]-lo.y)/ls;
			sidetype lz = (z[i]-lo.z)/ls;

			w[i] = weights[start+(lz*bl+ly)*bl+lx];
			d[i] = distance[start+(lz*bl+ly)*bl+lx];
			if(color) c[i] = VertexColor(
					color[start+(lz*bl+ly)*bl+lx].x,
					color[start+(lz*bl+ly)*bl+lx].y,
					color[start+(lz*bl+ly)*bl+lx].z);
		}
//		if((unsigned int)level<stacks[i].size()-2 && w[i]>0.0f) w[i] = minWeight;
	}
	if(weightInfluence(minWeight,
			w[0],w[1],w[2],w[3],w[4],w[5],w[6],w[7])){
		int *table = mc.getColoredTrianglesForCubePrecomputed(vertlist,collist,0,0,0,
				offset.x+(ox+size-sizeStretch)*scale,
				offset.y+(oy+size-sizeStretch)*scale,
				offset.z+(oz+size-sizeStretch)*scale,
				scale*sizeStretch,scale*sizeStretch,scale*sizeStretch,
				d[0],d[1],d[3],d[2],d[4],d[5],d[7],d[6],
				w[0],w[1],w[3],w[2],w[4],w[5],w[7],w[6],
				c[0],c[1],c[3],c[2],c[4],c[5],c[7],c[6],
				color);
		for (unsigned int i=0;table[i]!=-1;i+=3) {
			Vertex3f v0 = vertlist[table[i  ]];
			Vertex3f v1 = vertlist[table[i+1]];
			Vertex3f v2 = vertlist[table[i+2]];
			if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
				degenerate_faces++;
			}
			else{
				mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
				mesh.f.push_back(mesh.x.size()-1);
				mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
				mesh.f.push_back(mesh.x.size()-1);
				mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
				mesh.f.push_back(mesh.x.size()-1);

				if(color){
					VertexColor c0 = collist[table[i  ]];
					VertexColor c1 = collist[table[i+1]];
					VertexColor c2 = collist[table[i+2]];
					mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
					mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
					mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
				}
			}
		}
	}
}




void addBrickEdgeX
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	float *d = new float[bl*4];
	weighttype *w = new weighttype[bl*4];
	VertexColor *c = new VertexColor[bl*4];

	weighttype wf[2]; float df[2]; colortype3 cf[2];
	Vertex3f vertlist[12];
	VertexColor collist[12];
	MarchingCubes mc;


	leafstack stacks[4] = {leaves0,leaves1,leaves2,leaves3};

	sidetype sizeStretch = std::max(std::max(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
                                  leafScale[leaves2[leaves2.size()-1]]);
	sidetype sizeMin =
	std::min(std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
			     std::min(leafScale[leaves2[leaves2.size()-1]],leafScale[leaves3[leaves3.size()-1]]));

	sidetype y[4] = {(sidetype)(oy+size-sizeStretch) , (sidetype)(oy+size), (sidetype)(oy+size-sizeStretch), (sidetype)(oy+size)};
	sidetype z[4] = {(sidetype)(oz+size-sizeStretch) , (sidetype)(oz+size-sizeStretch), (sidetype)(oz+size), (sidetype)(oz+size)};

	for(sidetype x=ox;x<ox+size;x+=sizeMin){
		sidetype bx = (x-ox)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = bx*4+i;
			w[idx] = 0;
			int level;
			for(level = stacks[i].size()-1;level>=0 && w[idx]<=0.0;level--){
				volumetype leaf = stacks[i][level];
				volumetype start = leaf*brickSize;
				sidetype3 lo = leafPos[leaf];
				sidetype ls = leafScale[leaf];

				sidetype lxl = (x-lo.x)/ls;
				sidetype lxr = (x-lo.x)%ls;
				float rx = (float)lxr/(float)ls;
				sidetype lxh = lxl+(rx>0.0 && lxl<bl-1);

				if(lxl>=bl)
					fprintf(stderr,"\nERROR: Position in Leaf = %i %i vs %i",
							lxl,x,lo.x);

				sidetype ly = (y[i]-lo.y)/ls;
				sidetype lz = (z[i]-lo.z)/ls;

				volumetype idxLeaf[2] = {
						(volumetype)((lz*bl+ly)*bl+lxl),
						(volumetype)((lz*bl+ly)*bl+lxh)
				};

				for(volumetype i=0;i<2;i++) {
					wf[i] = weights[start+idxLeaf[i]];
					df[i] = distance[start+idxLeaf[i]];
					if(color) cf[i] = color[start+idxLeaf[i]];
				}

//				w[idx] = (1.0f-rx)*(float)wf[0] + rx*(float)wf[1];
				w[idx] = std::max(wf[0] , wf[1]);
				d[idx] = (1.0f-rx)*df[0] + rx*df[1];
				if(color)
				c[idx] = VertexColor(
						(1.0f-rx)*cf[0].x + rx*cf[1].x,
						(1.0f-rx)*cf[0].y + rx*cf[1].y,
						(1.0f-rx)*cf[0].z + rx*cf[1].z);

			}
//			if((unsigned int)level<stacks[i].size()-2 && w[idx]>0.0f) w[idx] = minWeight;
		}
	}


	for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
		sidetype bx = (x-ox)/sizeMin;
		volumetype idx = bx*4;
		if(weightInfluence(minWeight,
				w[idx],w[idx+4],
				w[idx+1],w[idx+1+4],
				w[idx+2],w[idx+2+4],
				w[idx+3],w[idx+3+4])){
			int *table = mc.getColoredTrianglesForCubePrecomputed(vertlist,collist,0,0,0,
					offset.x+x*scale,offset.y+(oy+size-sizeStretch)*scale,offset.z+(oz+size-sizeStretch)*scale,
					scale*sizeMin,scale*sizeStretch,scale*sizeStretch,
					d[idx],d[idx+4],
					d[idx+1+4],d[idx+1],
					d[idx+2],d[idx+4+2],
					d[idx+1+4+2],d[idx+1+2],
					w[idx],w[idx+4],
					w[idx+1+4],w[idx+1],
					w[idx+2],w[idx+4+2],
					w[idx+1+4+2],w[idx+1+2],
					c[idx],c[idx+4],
					c[idx+1+4],c[idx+1],
					c[idx+2],c[idx+4+2],
					c[idx+1+4+2],c[idx+1+2],
					color);

			for (unsigned int i=0;table[i]!=-1;i+=3) {
				Vertex3f v0 = vertlist[table[i  ]];
				Vertex3f v1 = vertlist[table[i+1]];
				Vertex3f v2 = vertlist[table[i+2]];
				if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
					degenerate_faces++;
				}
				else{
					mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
					mesh.f.push_back(mesh.x.size()-1);
					mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
					mesh.f.push_back(mesh.x.size()-1);
					mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
					mesh.f.push_back(mesh.x.size()-1);

					if(color){
						VertexColor c0 = collist[table[i  ]];
						VertexColor c1 = collist[table[i+1]];
						VertexColor c2 = collist[table[i+2]];
						mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
						mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
						mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
						mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
						mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
						mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
						mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
						mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
						mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
					}
				}
			}
		}
	}
	delete [] d; delete [] w; delete [] c;
}

void addBrickEdgeY
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	float *d = new float[bl*4];
	weighttype *w = new weighttype[bl*4];
	VertexColor *c = new VertexColor[bl*4];

	weighttype wf[2]; float df[2]; colortype3 cf[2];
	Vertex3f vertlist[12];
	VertexColor collist[12];
	MarchingCubes mc;


	leafstack stacks[4] = {leaves0,leaves1,leaves2,leaves3};

	sidetype sizeStretch = std::max(std::max(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
                                  leafScale[leaves2[leaves2.size()-1]]);
	sidetype sizeMin =
	std::min(std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
			     std::min(leafScale[leaves2[leaves2.size()-1]],leafScale[leaves3[leaves3.size()-1]]));

	sidetype x[4] = {(sidetype)(ox+size-sizeStretch),(sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch) , (sidetype)(ox+size)};
	sidetype z[4] = {(sidetype)(oz+size-sizeStretch),(sidetype)(oz+size-sizeStretch),
			(sidetype)(oz+size) , (sidetype)(oz+size)};


	for(sidetype y=oy;y<oy+size;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = by*4+i;
			w[idx] = 0;
			int level;
			for(level = stacks[i].size()-1;level>=0 && w[idx]<=0.0;level--){
				volumetype leaf = stacks[i][level];
				volumetype start = leaf*brickSize;
				sidetype3 lo = leafPos[leaf];
				sidetype ls = leafScale[leaf];

				sidetype lyl = (y-lo.y)/ls;
				sidetype lyr = (y-lo.y)%ls;
				float ry = (float)lyr/(float)ls;
				sidetype lyh = lyl+(ry>0.0 && lyl<bl-1);

				sidetype lx = (x[i]-lo.x)/ls;
				sidetype lz = (z[i]-lo.z)/ls;

				volumetype idxLeaf[2] = {
						(volumetype)((lz*bl+lyl)*bl+lx),
						(volumetype)((lz*bl+lyh)*bl+lx)
				};

				for(volumetype i=0;i<2;i++) {
					wf[i] = weights[start+idxLeaf[i]];
					df[i] = distance[start+idxLeaf[i]];
					if(color) cf[i] = color[start+idxLeaf[i]];
				}

				w[idx] = (1.0f-ry)*(float)wf[0] + ry*(float)wf[1];
				d[idx] = (1.0f-ry)*df[0] + ry*df[1];
				if(color)
				c[idx] = VertexColor(
						(1.0f-ry)*cf[0].x + ry*cf[1].x,
						(1.0f-ry)*cf[0].y + ry*cf[1].y,
						(1.0f-ry)*cf[0].z + ry*cf[1].z);

			}
//			if((unsigned int)level<stacks[i].size()-2 && w[idx]>0.0f) w[idx] = minWeight;
		}
	}


	for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		volumetype idx = by*4;
		if(weightInfluence(minWeight,
				w[idx],w[idx+1],
				w[idx+4],w[idx+1+4],
				w[idx+2],w[idx+2+1],
				w[idx+2+4],w[idx+1+2+4])){
			int *table = mc.getColoredTrianglesForCubePrecomputed(vertlist,collist,0,0,0,
					offset.x+(ox+size-sizeStretch)*scale,offset.y+y*scale,offset.z+(oz+size-sizeStretch)*scale,
					scale*sizeStretch,scale*sizeMin,scale*sizeStretch,
					d[idx],d[idx+1],
					d[idx+1+4],d[idx+4],
					d[idx+2],d[idx+1+2],
					d[idx+1+4+2],d[idx+4+2],
					w[idx],w[idx+1],
					w[idx+1+4],w[idx+4],
					w[idx+2],w[idx+1+2],
					w[idx+1+4+2],w[idx+4+2],
					c[idx],c[idx+1],
					c[idx+1+4],c[idx+4],
					c[idx+2],c[idx+1+2],
					c[idx+1+4+2],c[idx+4+2],
					color);

			for (unsigned int i=0;table[i]!=-1;i+=3) {
				Vertex3f v0 = vertlist[table[i  ]];
				Vertex3f v1 = vertlist[table[i+1]];
				Vertex3f v2 = vertlist[table[i+2]];
				if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
					degenerate_faces++;
				}
				else{
					mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
					mesh.f.push_back(mesh.x.size()-1);
					mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
					mesh.f.push_back(mesh.x.size()-1);
					mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
					mesh.f.push_back(mesh.x.size()-1);

					if(color){
						VertexColor c0 = collist[table[i  ]];
						VertexColor c1 = collist[table[i+1]];
						VertexColor c2 = collist[table[i+2]];
						mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
						mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
						mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
						mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
						mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
						mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
						mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
						mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
						mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
					}
				}
			}
		}
	}
	delete [] d; delete [] w; delete [] c;
}

void addBrickEdgeZ
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	float *d = new float[bl*4];
	weighttype *w = new weighttype[bl*4];
	VertexColor *c = new VertexColor[bl*4];

	weighttype wf[2]; float df[2]; colortype3 cf[2];
	Vertex3f vertlist[12];
	VertexColor collist[12];
	MarchingCubes mc;


	leafstack stacks[4] = {leaves0,leaves1,leaves2,leaves3};

	sidetype sizeStretch = std::max(std::max(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
                                  leafScale[leaves2[leaves2.size()-1]]);
	sidetype sizeMin =
	std::min(std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
			     std::min(leafScale[leaves2[leaves2.size()-1]],leafScale[leaves3[leaves3.size()-1]]));

	sidetype x[4] = {(sidetype)(ox+size-sizeStretch),(sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch),(sidetype)(ox+size)};
	sidetype y[4] = {(sidetype)(oy+size-sizeStretch),(sidetype)(oy+size-sizeStretch),
			(sidetype)(oy+size),(sidetype)(oy+size)};


	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = bz*4+i;
			w[idx] = 0;
			int level;
			for(level = stacks[i].size()-1;level>=0 && w[idx]<=0.0;level--){
				volumetype leaf = stacks[i][level];
				volumetype start = leaf*brickSize;
				sidetype3 lo = leafPos[leaf];
				sidetype ls = leafScale[leaf];

				sidetype lzl = (z-lo.z)/ls;
				sidetype lzr = (z-lo.z)%ls;
				float rz = (float)lzr/(float)ls;
				sidetype lzh = lzl+(rz>0.0 && lzl<bl-1);

				sidetype lx = (x[i]-lo.x)/ls;
				sidetype ly = (y[i]-lo.y)/ls;

				volumetype idxLeaf[2] = {
						(volumetype)((lzl*bl+ly)*bl+lx),
						(volumetype)((lzh*bl+ly)*bl+lx)
				};

				for(volumetype i=0;i<2;i++) {
					wf[i] = weights[start+idxLeaf[i]];
					df[i] = distance[start+idxLeaf[i]];
					if(color) cf[i] = color[start+idxLeaf[i]];
				}

				w[idx] = (1.0f-rz)*(float)wf[0] + rz*(float)wf[1];
				d[idx] = (1.0f-rz)*df[0] + rz*df[1];
				if(color)
				c[idx] = VertexColor(
						(1.0f-rz)*cf[0].x + rz*cf[1].x,
						(1.0f-rz)*cf[0].y + rz*cf[1].y,
						(1.0f-rz)*cf[0].z + rz*cf[1].z);

			}
//			if((unsigned int)level<stacks[i].size()-2 && w[idx]>0.0f) w[idx] = minWeight;
		}
	}


	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		volumetype idx = bz*4;
		if(weightInfluence(minWeight,
				w[idx],w[idx+1],
				w[idx+2],w[idx+1+2],
				w[idx+4],w[idx+4+1],
				w[idx+4+2],w[idx+4+1+2])){
			int *table = mc.getColoredTrianglesForCubePrecomputed(vertlist,collist,0,0,0,
					offset.x+(ox+size-sizeStretch)*scale,offset.y+(oy+size-sizeStretch)*scale,offset.z+z*scale,
					scale*sizeStretch,scale*sizeStretch,scale*sizeMin,
					d[idx],d[idx+1],
					d[idx+1+2],d[idx+2],
					d[idx+4],d[idx+4+1],
					d[idx+4+1+2],d[idx+4+2],
					w[idx],w[idx+1],
					w[idx+1+2],w[idx+2],
					w[idx+4],w[idx+4+1],
					w[idx+4+1+2],w[idx+4+2],
					c[idx],c[idx+1],
					c[idx+1+2],c[idx+2],
					c[idx+4],c[idx+4+1],
					c[idx+4+1+2],c[idx+4+2],
					color);

			for (unsigned int i=0;table[i]!=-1;i+=3) {
				Vertex3f v0 = vertlist[table[i  ]];
				Vertex3f v1 = vertlist[table[i+1]];
				Vertex3f v2 = vertlist[table[i+2]];
				if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
					degenerate_faces++;
				}
				else{
					mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
					mesh.f.push_back(mesh.x.size()-1);
					mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
					mesh.f.push_back(mesh.x.size()-1);
					mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
					mesh.f.push_back(mesh.x.size()-1);

					if(color){
						VertexColor c0 = collist[table[i  ]];
						VertexColor c1 = collist[table[i+1]];
						VertexColor c2 = collist[table[i+2]];
						mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
						mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
						mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
						mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
						mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
						mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
						mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
						mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
						mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
					}
				}
			}
		}
	}
	delete [] d; delete [] w; delete [] c;
}


void addBrickWallX
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0,
		leafstack leaves1
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	float *d = new float[bl*bl*2];
	weighttype *w = new weighttype[bl*bl*2];
	VertexColor *c = new VertexColor[bl*bl*2];

	weighttype wf[4]; float df[4]; colortype3 cf[4];
	Vertex3f vertlist[12];
	VertexColor collist[12];
	MarchingCubes mc;

	leafstack stacks[2] = {leaves0,leaves1};

	sidetype sizeMin = std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]);
	sidetype sizeStretch = leafScale[leaves0[leaves0.size()-1]];
	sidetype x[2] = {(sidetype)(ox+size-sizeStretch),(sidetype)(ox+size)};
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;

			for(unsigned int i=0;i<2;i++){
				volumetype idx = (bz*bl+by)*2+i;
				w[idx] = 0;
				int level;
				for(level = stacks[i].size()-1;level>=0 && w[idx]<=0.0;level--){
					volumetype leaf = stacks[i][level];
					volumetype start = leaf*brickSize;
					sidetype3 lo = leafPos[leaf];
					sidetype ls = leafScale[leaf];

					sidetype lx = (x[i]-lo.x)/ls;

					sidetype lyl = (y-lo.y)/ls;
					sidetype lyr = (y-lo.y)%ls;
					float ry = (float)lyr/(float)ls;
					sidetype lyh = lyl+(ry>0.0 && lyl<bl-1);

					sidetype lzl = (z-lo.z)/ls;
					sidetype lzr = (z-lo.z)%ls;
					float rz = (float)lzr/(float)ls;
					sidetype lzh = lzl+(rz>0.0 && lzl<bl-1);

					volumetype idxLeaf[4] = {
							(volumetype)((lzl*bl+lyl)*bl+lx),
							(volumetype)((lzl*bl+lyh)*bl+lx),
							(volumetype)((lzh*bl+lyl)*bl+lx),
							(volumetype)((lzh*bl+lyh)*bl+lx)
					};

					for(volumetype i=0;i<4;i++) {
						wf[i] = weights[start+idxLeaf[i]];
						df[i] = distance[start+idxLeaf[i]];
						if(color) cf[i] = color[start+idxLeaf[i]];
					}

					w[idx] = (1.0f-rz)*(1.0f-ry)*(float)wf[0]+
									 (1.0f-rz)*(     ry)*(float)wf[1]+
									 (     rz)*(1.0f-ry)*(float)wf[2]+
									 (     rz)*(     ry)*(float)wf[3];

					d[idx] = (1.0f-rz)*(1.0f-ry)*df[0]+
									 (1.0f-rz)*(     ry)*df[1]+
									 (     rz)*(1.0f-ry)*df[2]+
									 (     rz)*(     ry)*df[3];

					if(color)
					c[idx] = VertexColor(
									 (1.0f-rz)*(1.0f-ry)*(float)cf[0].x+
									 (1.0f-rz)*(     ry)*(float)cf[1].x+
									 (     rz)*(1.0f-ry)*(float)cf[2].x+
									 (     rz)*(     ry)*(float)cf[3].x,
									 (1.0f-rz)*(1.0f-ry)*(float)cf[0].y+
									 (1.0f-rz)*(     ry)*(float)cf[1].y+
									 (     rz)*(1.0f-ry)*(float)cf[2].y+
									 (     rz)*(     ry)*(float)cf[3].y,
									 (1.0f-rz)*(1.0f-ry)*(float)cf[0].z+
									 (1.0f-rz)*(     ry)*(float)cf[1].z+
									 (     rz)*(1.0f-ry)*(float)cf[2].z+
									 (     rz)*(     ry)*(float)cf[3].z);
				}
//				if((unsigned int)level<stacks[i].size()-2 && w[idx]>0.0f) w[idx] = minWeight;
			}
		}
	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;

			volumetype idx = (bz*bl+by)*2;
			if(weightInfluence(minWeight,
					w[idx],w[idx+1],
					w[idx+2],w[idx+2+1],
					w[idx+bl*2],w[idx+bl*2+1],
					w[idx+bl*2+2],w[idx+bl*2+2+1])){
				int *table = mc.getColoredTrianglesForCubePrecomputed(vertlist,collist,0,0,0,
						offset.x+x[0]*scale,offset.y+y*scale,offset.z+z*scale,
						scale*sizeStretch,scale*sizeMin,scale*sizeMin,
						d[idx],d[idx+1],
						d[idx+2+1],d[idx+2],
						d[idx+bl*2],d[idx+bl*2+1],
						d[idx+bl*2+2+1],d[idx+bl*2+2],
						w[idx],w[idx+1],
						w[idx+2+1],w[idx+2],
						w[idx+bl*2],w[idx+bl*2+1],
						w[idx+bl*2+2+1],w[idx+bl*2+2],
						c[idx],c[idx+1],
						c[idx+2+1],c[idx+2],
						c[idx+bl*2],c[idx+bl*2+1],
						c[idx+bl*2+2+1],c[idx+bl*2+2],
						color);

				for (unsigned int i=0;table[i]!=-1;i+=3) {
					Vertex3f v0 = vertlist[table[i  ]];
					Vertex3f v1 = vertlist[table[i+1]];
					Vertex3f v2 = vertlist[table[i+2]];
					if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
						degenerate_faces++;
					}
					else{
						mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
						mesh.f.push_back(mesh.x.size()-1);
						mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
						mesh.f.push_back(mesh.x.size()-1);
						mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
						mesh.f.push_back(mesh.x.size()-1);

						if(color){
							VertexColor c0 = collist[table[i  ]];
							VertexColor c1 = collist[table[i+1]];
							VertexColor c2 = collist[table[i+2]];
							mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
							mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
							mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
							mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
							mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
							mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
							mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
							mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
							mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
						}
					}
				}
			}
		}
	}
	delete [] d; delete [] w; delete [] c;
}

void addBrickWallY
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0,
		leafstack leaves1
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	float *d = new float[bl*bl*2];
	weighttype *w = new weighttype[bl*bl*2];
	VertexColor *c = new VertexColor[bl*bl*2];

	weighttype wf[4]; float df[4]; colortype3 cf[4];
	Vertex3f vertlist[12];
	VertexColor collist[12];
	MarchingCubes mc;

	leafstack stacks[2] = {leaves0,leaves1};

	sidetype sizeMin = std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]);
	sidetype sizeStretch = leafScale[leaves0[leaves0.size()-1]];
	sidetype y[2] = {(sidetype)(oy+size-sizeStretch) , (sidetype)(oy+size)};
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			for(unsigned int i=0;i<2;i++){
				volumetype idx = (bz*bl+bx)*2+i;
				w[idx] = 0;
				int level;
				for(level = stacks[i].size()-1;level>=0 && w[idx]<=0.0;level--){
					volumetype leaf = stacks[i][level];
					volumetype start = leaf*brickSize;
					sidetype3 lo = leafPos[leaf];
					sidetype ls = leafScale[leaf];

					sidetype ly = (y[i]-lo.y)/ls;

					sidetype lxl = (x-lo.x)/ls;
					sidetype lxr = (x-lo.x)%ls;
					float rx = (float)lxr/(float)ls;
					sidetype lxh = lxl+(rx>0.0 && lxl<bl-1);

					sidetype lzl = (z-lo.z)/ls;
					sidetype lzr = (z-lo.z)%ls;
					float rz = (float)lzr/(float)ls;
					sidetype lzh = lzl+(rz>0.0 && lzl<bl-1);

					volumetype idxLeaf[4] = {
							(volumetype)((lzl*bl+ly)*bl+lxl),
							(volumetype)((lzl*bl+ly)*bl+lxh),
							(volumetype)((lzh*bl+ly)*bl+lxl),
							(volumetype)((lzh*bl+ly)*bl+lxh)
					};

					for(volumetype i=0;i<4;i++) {
						wf[i] = weights[start+idxLeaf[i]];
						df[i] = distance[start+idxLeaf[i]];
						if(color) cf[i] = color[start+idxLeaf[i]];
					}

					w[idx] = (1.0f-rz)*(1.0f-rx)*(float)wf[0]+
									 (1.0f-rz)*(     rx)*(float)wf[1]+
									 (     rz)*(1.0f-rx)*(float)wf[2]+
									 (     rz)*(     rx)*(float)wf[3];

					d[idx] = (1.0f-rz)*(1.0f-rx)*df[0]+
									 (1.0f-rz)*(     rx)*df[1]+
									 (     rz)*(1.0f-rx)*df[2]+
									 (     rz)*(     rx)*df[3];

					if(color)
					c[idx] = VertexColor(
									 (1.0f-rz)*(1.0f-rx)*(float)cf[0].x+
									 (1.0f-rz)*(     rx)*(float)cf[1].x+
									 (     rz)*(1.0f-rx)*(float)cf[2].x+
									 (     rz)*(     rx)*(float)cf[3].x,
									 (1.0f-rz)*(1.0f-rx)*(float)cf[0].y+
									 (1.0f-rz)*(     rx)*(float)cf[1].y+
									 (     rz)*(1.0f-rx)*(float)cf[2].y+
									 (     rz)*(     rx)*(float)cf[3].y,
									 (1.0f-rz)*(1.0f-rx)*(float)cf[0].z+
									 (1.0f-rz)*(     rx)*(float)cf[1].z+
									 (     rz)*(1.0f-rx)*(float)cf[2].z+
									 (     rz)*(     rx)*(float)cf[3].z);
				}
//				if((unsigned int)level<stacks[i].size()-2 && w[idx]>0.0f) w[idx] = minWeight;
			}
		}
	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			volumetype idx = (bz*bl+bx)*2;
			if(weightInfluence(minWeight,
					w[idx],w[idx+2],
					w[idx+1],w[idx+1+2],
					w[idx+bl*2],w[idx+bl*2+2],
					w[idx+bl*2+1],w[idx+bl*2+1+2])){
				int *table = mc.getColoredTrianglesForCubePrecomputed(vertlist,collist,0,0,0,
						offset.x+x*scale,offset.y+y[0]*scale,offset.z+z*scale,
						scale*sizeMin,scale*sizeStretch,scale*sizeMin,
						d[idx],d[idx+2],
						d[idx+1+2],d[idx+1],
						d[idx+bl*2],d[idx+bl*2+2],
						d[idx+bl*2+1+2],d[idx+bl*2+1],
						w[idx],w[idx+2],
						w[idx+1+2],w[idx+1],
						w[idx+bl*2],w[idx+bl*2+2],
						w[idx+bl*2+1+2],w[idx+bl*2+1],
						c[idx],c[idx+2],
						c[idx+1+2],c[idx+1],
						c[idx+bl*2],c[idx+bl*2+2],
						c[idx+bl*2+1+2],c[idx+bl*2+1],
						color);

				for (unsigned int i=0;table[i]!=-1;i+=3) {
					Vertex3f v0 = vertlist[table[i  ]];
					Vertex3f v1 = vertlist[table[i+1]];
					Vertex3f v2 = vertlist[table[i+2]];
					if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
						degenerate_faces++;
					}
					else{
						mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
						mesh.f.push_back(mesh.x.size()-1);
						mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
						mesh.f.push_back(mesh.x.size()-1);
						mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
						mesh.f.push_back(mesh.x.size()-1);

						if(color){
							VertexColor c0 = collist[table[i  ]];
							VertexColor c1 = collist[table[i+1]];
							VertexColor c2 = collist[table[i+2]];
							mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
							mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
							mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
							mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
							mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
							mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
							mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
							mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
							mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
						}
					}
				}
			}
		}
	}
	delete [] d; delete [] w; delete [] c;
}

void addBrickWallZ
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0,
		leafstack leaves1
)
{
	sidetype &bl = info.brickLength;
	sidetype &brickSize = info.brickSize;
	const sidetype *leafScale = info.leafScale;
	const sidetype3 *leafPos = info.leafPos;
	const float *distance = info.distance;
	const weighttype *weights = info.weights;
	const colortype3 *color = info.color;
	float minWeight = info.minWeight;
	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	float *d = new float[bl*bl*2];
	weighttype *w = new weighttype[bl*bl*2];
	VertexColor *c = new VertexColor[bl*bl*2];

	weighttype wf[4]; float df[4]; colortype3 cf[4];
	Vertex3f vertlist[12];
	VertexColor collist[12];
	MarchingCubes mc;

	leafstack stacks[2] = {leaves0,leaves1};

	sidetype sizeMin = std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]);
	sidetype sizeStretch = leafScale[leaves0[leaves0.size()-1]];
	sidetype z[2] = {(sidetype)(oz+size-sizeStretch) , (sidetype)(oz+size)};
	for(sidetype y=oy;y<oy+size;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			for(unsigned int i=0;i<2;i++){
				volumetype idx = (by*bl+bx)*2+i;
				w[idx] = 0;
				int level;
				for(level = stacks[i].size()-1;level>=0 && w[idx]<=0.0;level--){
					volumetype leaf = stacks[i][level];
					volumetype start = leaf*brickSize;
					sidetype3 lo = leafPos[leaf];
					sidetype ls = leafScale[leaf];

					sidetype lz = (z[i]-lo.z)/ls;

					sidetype lxl = (x-lo.x)/ls;
					sidetype lxr = (x-lo.x)%ls;
					float rx = (float)lxr/(float)ls;
					sidetype lxh = lxl+(rx>0.0 && lxl<bl-1);

					sidetype lyl = (y-lo.y)/ls;
					sidetype lyr = (y-lo.y)%ls;
					float ry = (float)lyr/(float)ls;
					sidetype lyh = lyl+(ry>0.0 && lyl<bl-1);

					volumetype idxLeaf[4] = {
							(volumetype)((lz*bl+lyl)*bl+lxl),
							(volumetype)((lz*bl+lyl)*bl+lxh),
							(volumetype)((lz*bl+lyh)*bl+lxl),
							(volumetype)((lz*bl+lyh)*bl+lxh)
					};

					for(volumetype i=0;i<4;i++) {
						wf[i] = weights[start+idxLeaf[i]];
						df[i] = distance[start+idxLeaf[i]];
						if(color) cf[i] = color[start+idxLeaf[i]];
					}

					w[idx] = (1.0f-ry)*(1.0f-rx)*(float)wf[0]+
									 (1.0f-ry)*(     rx)*(float)wf[1]+
									 (     ry)*(1.0f-rx)*(float)wf[2]+
									 (     ry)*(     rx)*(float)wf[3];

					d[idx] = (1.0f-ry)*(1.0f-rx)*df[0]+
									 (1.0f-ry)*(     rx)*df[1]+
									 (     ry)*(1.0f-rx)*df[2]+
									 (     ry)*(     rx)*df[3];

					if(color)
					c[idx] = VertexColor(
									 (1.0f-ry)*(1.0f-rx)*(float)cf[0].x+
									 (1.0f-ry)*(     rx)*(float)cf[1].x+
									 (     ry)*(1.0f-rx)*(float)cf[2].x+
									 (     ry)*(     rx)*(float)cf[3].x,
									 (1.0f-ry)*(1.0f-rx)*(float)cf[0].y+
									 (1.0f-ry)*(     rx)*(float)cf[1].y+
									 (     ry)*(1.0f-rx)*(float)cf[2].y+
									 (     ry)*(     rx)*(float)cf[3].y,
									 (1.0f-ry)*(1.0f-rx)*(float)cf[0].z+
									 (1.0f-ry)*(     rx)*(float)cf[1].z+
									 (     ry)*(1.0f-rx)*(float)cf[2].z+
									 (     ry)*(     rx)*(float)cf[3].z);
//						c[idx] = VertexColor(0,0,65280);
				}
			}
		}
	}

	for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			volumetype idx = (by*bl+bx)*2;
			if(weightInfluence(minWeight,
					w[idx],w[idx+2],
					w[idx+bl*2],w[idx+bl*2+2],
					w[idx+1],w[idx+2+1],
					w[idx+bl*2+1],w[idx+bl*2+2+1])){
				int *table = mc.getColoredTrianglesForCubePrecomputed(vertlist,collist,0,0,0,
						offset.x+x*scale,offset.y+y*scale,offset.z+z[0]*scale,
						scale*sizeMin,scale*sizeMin,scale*sizeStretch,
						d[idx],d[idx+2],
						d[idx+bl*2+2],d[idx+bl*2],
						d[idx+1],d[idx+2+1],
						d[idx+bl*2+2+1],d[idx+bl*2+1],
						w[idx],w[idx+2],
						w[idx+bl*2+2],w[idx+bl*2],
						w[idx+1],w[idx+2+1],
						w[idx+bl*2+2+1],w[idx+bl*2+1],
						c[idx],c[idx+2],
						c[idx+bl*2+2],c[idx+bl*2],
						c[idx+1],c[idx+2+1],
						c[idx+bl*2+2+1],c[idx+bl*2+1],
						color);

				for (unsigned int i=0;table[i]!=-1;i+=3) {
					Vertex3f v0 = vertlist[table[i  ]];
					Vertex3f v1 = vertlist[table[i+1]];
					Vertex3f v2 = vertlist[table[i+2]];
					if((v0.x==v1.x&&v0.y==v1.y&&v0.z==v1.z)||(v2.x==v1.x&&v2.y==v1.y&&v2.z==v1.z)||(v0.x==v2.x&&v0.y==v2.y&&v0.z==v2.z)){
						degenerate_faces++;
					}
					else{
						mesh.x.push_back(v0.x); mesh.y.push_back(v0.y); mesh.z.push_back(v0.z);
						mesh.f.push_back(mesh.x.size()-1);
						mesh.x.push_back(v1.x); mesh.y.push_back(v1.y); mesh.z.push_back(v1.z);
						mesh.f.push_back(mesh.x.size()-1);
						mesh.x.push_back(v2.x); mesh.y.push_back(v2.y); mesh.z.push_back(v2.z);
						mesh.f.push_back(mesh.x.size()-1);

						if(color){
							VertexColor c0 = collist[table[i  ]];
							VertexColor c1 = collist[table[i+1]];
							VertexColor c2 = collist[table[i+2]];
							mesh.r.push_back(c0.x/COLOR_MULTIPLICATOR);
							mesh.g.push_back(c0.y/COLOR_MULTIPLICATOR);
							mesh.b.push_back(c0.z/COLOR_MULTIPLICATOR);
							mesh.r.push_back(c1.x/COLOR_MULTIPLICATOR);
							mesh.g.push_back(c1.y/COLOR_MULTIPLICATOR);
							mesh.b.push_back(c1.z/COLOR_MULTIPLICATOR);
							mesh.r.push_back(c2.x/COLOR_MULTIPLICATOR);
							mesh.g.push_back(c2.y/COLOR_MULTIPLICATOR);
							mesh.b.push_back(c2.z/COLOR_MULTIPLICATOR);
						}
					}
				}
			}
		}
	}
	delete [] d; delete [] w; delete [] c;
}






























void addMiddle
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
		const MarchingCubesIndexed &mc
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
		addMiddle(half,info,ox+half,oy+half,oz+half,
				childBranch[0],childBranch[1],childBranch[2],childBranch[3],
				childBranch[4],childBranch[5],childBranch[6],childBranch[7],
				childLeaf[0],childLeaf[1],childLeaf[2],childLeaf[3],
				childLeaf[4],childLeaf[5],childLeaf[6],childLeaf[7],
				childLeaves[0],childLeaves[1],childLeaves[2],childLeaves[3],
				childLeaves[4],childLeaves[5],childLeaves[6],childLeaves[7],
				mc);
	} else if(valid){
//		addBrickMiddle(info,ox,oy,oz,size,
//				childLeaves[0],childLeaves[1],childLeaves[2],childLeaves[3],
//				childLeaves[4],childLeaves[5],childLeaves[6],childLeaves[7]);
		addBrickMiddle_indexed(info,ox,oy,oz,size,
				childLeaves[0],childLeaves[1],childLeaves[2],childLeaves[3],
				childLeaves[4],childLeaves[5],childLeaves[6],childLeaves[7],
				mc);

	}
}


void addEdge
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
		const MarchingCubesIndexed &mc
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
		addEdge(dimShift,half,info,
				ox+half*(dimShift!=1),oy+half*(dimShift!=2),oz+half*(dimShift!=4),
				childBranch[0],childBranch[dimL],childBranch[dimH],childBranch[dimL+dimH],
				  childLeaf[0],  childLeaf[dimL],  childLeaf[dimH],  childLeaf[dimL+dimH],
				childLeaves[0],childLeaves[dimL],childLeaves[dimH],childLeaves[dimL+dimH],
				mc);
		addEdge(dimShift,half,info,
				ox+half,oy+half,oz+half,
				childBranch[dimShift],childBranch[dimL+dimShift],childBranch[dimH+dimShift],childBranch[dimL+dimH+dimShift],
				  childLeaf[dimShift],  childLeaf[dimL+dimShift],  childLeaf[dimH+dimShift],  childLeaf[dimL+dimH+dimShift],
				childLeaves[dimShift],childLeaves[dimL+dimShift],childLeaves[dimH+dimShift],childLeaves[dimL+dimH+dimShift],
				mc);

		addMiddle(half,info,
				ox+half*(dimShift!=1),oy+half*(dimShift!=2),oz+half*(dimShift!=4),
				childBranch[0],childBranch[1],childBranch[2],childBranch[3],
				childBranch[4],childBranch[5],childBranch[6],childBranch[7],
				childLeaf[0],childLeaf[1],childLeaf[2],childLeaf[3],
				childLeaf[4],childLeaf[5],childLeaf[6],childLeaf[7],
				childLeaves[0],childLeaves[1],childLeaves[2],childLeaves[3],
				childLeaves[4],childLeaves[5],childLeaves[6],childLeaves[7],
				mc);
	}
	else if(lastLeaf0<maxLeaves && lastLeaf1<maxLeaves && lastLeaf2<maxLeaves && lastLeaf3<maxLeaves){
		if(dimShift==1){
			addBrickEdgeX_indexed(info,ox,oy,oz,size,leaves0,leaves1,leaves2,leaves3,mc);
		}
		if(dimShift==2){
			addBrickEdgeY_indexed(info,ox,oy,oz,size,leaves0,leaves1,leaves2,leaves3,mc);
		}
		if(dimShift==4){
			addBrickEdgeZ_indexed(info,ox,oy,oz,size,leaves0,leaves1,leaves2,leaves3,mc);
		}
	}

}


void addWall
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
		const MarchingCubesIndexed &mc
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

		addWall(dimShift,info,
				ox+half*(dimShift==1),oy+half*(dimShift==2),oz+half*(dimShift==4),half,
				childBranch[0],childLeaf[0],childLeaves[0],childBranch[dimShift],childLeaf[dimShift],childLeaves[dimShift],
				mc);
		addWall(dimShift,info,
				ox+half,oy+half*(dimShift==2 || dimShift==1),oz+half*(dimShift==4),half,
				childBranch[dimL],childLeaf[dimL],childLeaves[dimL],childBranch[dimL+dimShift],childLeaf[dimL+dimShift],childLeaves[dimL+dimShift],
				mc);
		addWall(dimShift,info,
				ox+half*(dimShift==1),oy+half*(dimShift==2 || dimShift==4),oz+half,half,
				childBranch[dimH],childLeaf[dimH],childLeaves[dimH],childBranch[dimH+dimShift],childLeaf[dimH+dimShift],childLeaves[dimH+dimShift],
				mc);
		addWall(dimShift,info,
				ox+half,oy+half,oz+half,half,
				childBranch[dimL+dimH],childLeaf[dimL+dimH],childLeaves[dimL+dimH],childBranch[dimL+dimH+dimShift],childLeaf[dimL+dimH+dimShift],childLeaves[dimL+dimH+dimShift],
				mc);

		addEdge(dimL,half,info,
				ox+half*(dimShift==1),
				oy+half*(dimShift==2),
				oz+half*(dimShift==4),
				childBranch[0],childBranch[dimLEdgeL],
				childBranch[dimLEdgeH],childBranch[dimLEdgeL+dimLEdgeH],
				childLeaf[0],childLeaf[dimLEdgeL],
				childLeaf[dimLEdgeH],childLeaf[dimLEdgeL+dimLEdgeH],
				childLeaves[0],childLeaves[dimLEdgeL],
				childLeaves[dimLEdgeH],childLeaves[dimLEdgeL+dimLEdgeH],
				mc);
		addEdge(dimL,half,info,
				ox+half*(dimShift==1 || dimL==1),
				oy+half*(dimShift==2 || dimL==2),
				oz+half*(dimShift==4 || dimL==4),
				childBranch[dimL],childBranch[dimL+dimLEdgeL],
				childBranch[dimL+dimLEdgeH],childBranch[dimL+dimLEdgeL+dimLEdgeH],
				childLeaf[dimL],childLeaf[dimL+dimLEdgeL],
				childLeaf[dimL+dimLEdgeH],childLeaf[dimL+dimLEdgeL+dimLEdgeH],
				childLeaves[dimL],childLeaves[dimL+dimLEdgeL],
				childLeaves[dimL+dimLEdgeH],childLeaves[dimL+dimLEdgeL+dimLEdgeH],
				mc);
		addEdge(dimH,half,info,
				ox+half*(dimShift==1),
				oy+half*(dimShift==2),
				oz+half*(dimShift==4),
				childBranch[0],childBranch[dimHEdgeL],
				childBranch[dimHEdgeH],childBranch[dimHEdgeL+dimHEdgeH],
				childLeaf[0],childLeaf[dimHEdgeL],
				childLeaf[dimHEdgeH],childLeaf[dimHEdgeL+dimHEdgeH],
				childLeaves[0],childLeaves[dimHEdgeL],
				childLeaves[dimHEdgeH],childLeaves[dimHEdgeL+dimHEdgeH],
				mc);
		addEdge(dimH,half,info,
				ox+half*(dimShift==1 || dimH==1),
				oy+half*(dimShift==2 || dimH==2),
				oz+half*(dimShift==4 || dimH==4),
				childBranch[dimH],childBranch[dimH+dimHEdgeL],
				childBranch[dimH+dimHEdgeH],childBranch[dimH+dimHEdgeL+dimHEdgeH],
				childLeaf[dimH],childLeaf[dimH+dimHEdgeL],
				childLeaf[dimH+dimHEdgeH],childLeaf[dimH+dimHEdgeL+dimHEdgeH],
				childLeaves[dimH],childLeaves[dimH+dimHEdgeL],
				childLeaves[dimH+dimHEdgeH],childLeaves[dimH+dimHEdgeL+dimHEdgeH],
				mc);

		addMiddle(half,info,
				ox+half*(dimShift==1),oy+half*(dimShift==2),oz+half*(dimShift==4),
				childBranch[0],childBranch[1],childBranch[2],childBranch[3],
				childBranch[4],childBranch[5],childBranch[6],childBranch[7],
				childLeaf[0],childLeaf[1],childLeaf[2],childLeaf[3],
				childLeaf[4],childLeaf[5],childLeaf[6],childLeaf[7],
				childLeaves[0],childLeaves[1],childLeaves[2],childLeaves[3],
				childLeaves[4],childLeaves[5],childLeaves[6],childLeaves[7],
				mc);

	}
	else if(lastLeafLow<maxLeaves && lastLeafHigh<maxLeaves){
		if(dimShift==1){
			addBrickWallX_indexed(info,ox,oy,oz,size,leavesLow,leavesHigh,mc);
		}
		if(dimShift==2){
			addBrickWallY_indexed(info,ox,oy,oz,size,leavesLow,leavesHigh,mc);
		}
		if(dimShift==4){
			addBrickWallZ_indexed(info,ox,oy,oz,size,leavesLow,leavesHigh,mc);
		}
	}
}




void addInterior
(
		leafstack leaves,
		treeinfo info,
		volumetype index, sidetype size,
		sidetype ox, sidetype oy, sidetype oz,
		volumetype lastLeaf,
		const MarchingCubesIndexed &mc
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

			addWall(dimShift,info,
					ox,oy,oz,half,
					childBranch[0],childLeaf[0],childLeaves[0],childBranch[dimShift],childLeaf[dimShift],childLeaves[dimShift],
					mc);
			addWall(dimShift,info,
					ox+half*(dimL==1),oy+half*(dimL==2),oz,half,
					childBranch[dimL],childLeaf[dimL],childLeaves[dimL],childBranch[dimL+dimShift],childLeaf[dimL+dimShift],childLeaves[dimL+dimShift],
					mc);
			addWall(dimShift,info,
					ox,oy+half*(dimH==2),oz+half*(dimH==4),half,
					childBranch[dimH],childLeaf[dimH],childLeaves[dimH],childBranch[dimH+dimShift],childLeaf[dimH+dimShift],childLeaves[dimH+dimShift],
					mc);
			addWall(dimShift,info,
					ox+half*(dimL==1),oy+half*(dimL==2 || dimH==2),oz+half*(dimH==4),half,
					childBranch[dimL+dimH],childLeaf[dimL+dimH],childLeaves[dimL+dimH],childBranch[dimL+dimH+dimShift],childLeaf[dimL+dimH+dimShift],childLeaves[dimL+dimH+dimShift],
					mc);

			addEdge(dimShift,half,info,
					ox,oy,oz,
					childBranch[0],childBranch[dimL],childBranch[dimH],childBranch[dimL+dimH],
					  childLeaf[0],  childLeaf[dimL],  childLeaf[dimH],  childLeaf[dimL+dimH],
					  childLeaves[0],childLeaves[dimL],childLeaves[dimH],childLeaves[dimL+dimH],
					  mc);
			addEdge(dimShift,half,info,
					ox+half*(dimShift==1),oy+half*(dimShift==2),oz+half*(dimShift==4),
					childBranch[dimShift],childBranch[dimL+dimShift],childBranch[dimH+dimShift],childBranch[dimL+dimH+dimShift],
					  childLeaf[dimShift],  childLeaf[dimL+dimShift],  childLeaf[dimH+dimShift],  childLeaf[dimL+dimH+dimShift],
					  childLeaves[dimShift],childLeaves[dimL+dimShift],childLeaves[dimH+dimShift],childLeaves[dimL+dimH+dimShift],
					  mc);

			addMiddle(half,info,
					ox,oy,oz,
					childBranch[0],childBranch[1],childBranch[2],childBranch[3],
					childBranch[4],childBranch[5],childBranch[6],childBranch[7],
					childLeaf[0],childLeaf[1],childLeaf[2],childLeaf[3],
					childLeaf[4],childLeaf[5],childLeaf[6],childLeaf[7],
					childLeaves[0],childLeaves[1],childLeaves[2],childLeaves[3],
					childLeaves[4],childLeaves[5],childLeaves[6],childLeaves[7],
					mc);
		}

		for(volumetype childoffset=0;childoffset<8;childoffset++){
			sidetype oxChild = ox+half*((childoffset&1)!=0);
			sidetype oyChild = oy+half*((childoffset&2)!=0);
			sidetype ozChild = oz+half*((childoffset&4)!=0);
			if(childBranch[childoffset]){
				addInterior(childLeaves[childoffset],info,
						tree[index+8+childoffset],half,
						oxChild,oyChild,ozChild,
						childLeaf[childoffset],mc);
			}
			else if(childLeaf[childoffset]<maxLeaves){
//				addBrickInterior(childLeaves[childoffset],oxChild,oyChild,ozChild,half,info,mc);
				addBrickInterior_indexed(childLeaves[childoffset],oxChild,oyChild,ozChild,half,info,mc);
			}
		}
	}
	else{
		fprintf(stderr,"\nERROR: Branch without Children!");
	}
}


































