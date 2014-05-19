/*
 * traversal_functions.cpp
 *
 *  Created on: May 10, 2013
 *      Author: steinbrf
 */

inline void queryPointDepthSingle_func
(
		sidetype x, sidetype y, sidetype z, sidetype brickLengthTarget,
		const sidetype &_n, const sidetype &_brickLength, volumetype *_tree,
		volumetype &_nBranchesUsed,
		const volumetype &_nLeavesTotal, volumetype &_nLeavesUsed, volumetype &_nLeavesQueued,
		volumetype *_leafNumber, sidetype3 *_leafPos, sidetype *_leafScale,
		volumetype *_queueIndexOfLeaf

)
{
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	sidetype half = _n>>1;
	volumetype branch = 0;

	while((half>>1)>=brickLengthTarget){
		sidetype childoffset = (x>=ox+half)*1 + (y>=oy+half)*2 + (z>=oz+half)*4;

		volumetype pointertochildindex = branch+childoffset+8;
		volumetype childindex = _tree[pointertochildindex];
		volumetype leafindex = _tree[pointertochildindex-8];

		if(leafindex < _nLeavesTotal &&_queueIndexOfLeaf[leafindex] >= _nLeavesQueued){
			//The Leaf Brick has not been queued yet
			_queueIndexOfLeaf[leafindex] = _nLeavesQueued;
			_leafNumber[_nLeavesQueued] = leafindex;
			_nLeavesQueued++;
		}

		if(childindex == BRANCHINIT){
			//The child is not allocated
			_tree[pointertochildindex] = childindex = _nBranchesUsed; //Atomic
			_nBranchesUsed += 16; //Atomic
		}

    ox += half * ((childoffset & 1) != 0);
    oy += half * ((childoffset & 2) != 0);
    oz += half * ((childoffset & 4) != 0);
    branch = childindex;
    half >>= 1;
	}
	sidetype childoffset = (x>=ox+half)*1 + (y>=oy+half)*2 + (z>=oz+half)*4;
	volumetype pointertochildindex = branch+childoffset;
	volumetype childindex = _tree[pointertochildindex];
	if(childindex >= BRANCHINIT){
		//The Leaf has not been allocated yet
		if((_tree[pointertochildindex] = childindex = _nLeavesUsed) < _nLeavesTotal){
			_leafPos[_nLeavesUsed].x = ox+half*((childoffset&1)!=0);
			_leafPos[_nLeavesUsed].y = oy+half*((childoffset&2)!=0);
			_leafPos[_nLeavesUsed].z = oz+half*((childoffset&4)!=0);
			_leafScale[childindex] = half/_brickLength;
		}
		_nLeavesUsed++;
	}
	//Put the Leaf onto the Data Queue, if not already there
	if(childindex < _nLeavesTotal &&_queueIndexOfLeaf[childindex] >= _nLeavesQueued){
    //The Leaf Brick has not been queued yet
		_queueIndexOfLeaf[childindex] = _nLeavesQueued;
		_leafNumber[_nLeavesQueued] = childindex;
		_nLeavesQueued++;
	}
}

inline void populate_branch
(
		const volumetype *_tree, volumetype branch,
		sidetype half, sidetype _brickLength,
		volumetype leaf,
		const sidetype3 *_leafPos, const sidetype *_leafScale
		,FusionMipMapCPU::MeshCellArray &_meshCells
		,FusionMipMapCPU::BranchIndicesArray &_meshCellIndicesBranch
		,FusionMipMapCPU::LeafNeighborhood &meshCellIndicesLeafSingle
)
{
	for(uchar childoffset=0;childoffset<8;childoffset++){
		if(half>_brickLength && _tree[branch+8+childoffset]<BRANCHINIT){
			populate_branch(_tree,_tree[branch+8+childoffset],
					half>>1,_brickLength,leaf,_leafPos,_leafScale,
					_meshCells,_meshCellIndicesBranch,meshCellIndicesLeafSingle);
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

inline void addLeaf_vis
(
		volumetype pointertochildindex,
		volumetype &childindex,
		volumetype &_nLeavesUsed,
		volumetype _nLeavesTotal,
		sidetype _brickLength,
		sidetype ox, sidetype oy, sidetype oz, sidetype half,
		sidetype childoffset,
		volumetype *_tree,
		sidetype3 *_leafPos,
		sidetype *_leafScale,
		volumetype pl,
		ParentArray &_leafParent,
		FusionMipMapCPU::BranchIndicesArray &_meshCellIndicesBranch,
		FusionMipMapCPU::LeafIndicesArray &_meshCellIndicesLeaf,
		FusionMipMapCPU::MeshCellArray &_meshCells,
		bool performIncrementalMeshing
)
{
	if((_tree[pointertochildindex] = childindex = _nLeavesUsed) < _nLeavesTotal){
		_leafPos[_nLeavesUsed].x = ox+half*((childoffset&1)!=0);
		_leafPos[_nLeavesUsed].y = oy+half*((childoffset&2)!=0);
		_leafPos[_nLeavesUsed].z = oz+half*((childoffset&4)!=0);
		_leafScale[_nLeavesUsed] = half/_brickLength;

		_leafParent.push_back(pl);

#ifndef DEBUG_NO_LEAFCELLS
		if(performIncrementalMeshing){
			_meshCellIndicesLeaf.push_back(FusionMipMapCPU::LeafNeighborhood());
			FusionMipMapCPU::LeafNeighborhood &meshCellIndicesLeafSingle = _meshCellIndicesLeaf.back();


			volumetype leaf = _nLeavesUsed;


			if(half > _brickLength && _tree[pointertochildindex+8]<BRANCHINIT){
				populate_branch(_tree,_tree[pointertochildindex+8],
						half>>1,_brickLength,
						leaf,_leafPos, _leafScale,_meshCells,
						_meshCellIndicesBranch,meshCellIndicesLeafSingle);
			}
			else{
#ifdef BRANCHNEIGHBORHOOD_REFERECE
				MCNSplit &branchCells = *(_meshCellIndicesBranch[pointertochildindex]);
#else
				MCNSplit &branchCells = _meshCellIndicesBranch[pointertochildindex];
#endif

				createMeshCellStructuresForLeaf(
						leaf,branchCells,meshCellIndicesLeafSingle,_meshCells,_leafScale);

			}
			if(meshCellIndicesLeafSingle.size()==0){
				fprintf(stderr,"\nERROR: Leaf %li has no associated Mesh Cells!",_meshCellIndicesLeaf.size()-1);
			}
			else{
				bool wrongIndex = false;
				for(FusionMipMapCPU::LeafNeighborhood::iterator
						i=meshCellIndicesLeafSingle.begin();i!=meshCellIndicesLeafSingle.end();i++){
					wrongIndex |= *i >= MAXCELLINDEX;
				}
				if(wrongIndex){
					fprintf(stderr,"\nERROR: Wrong Traversal Index at Leaf %li:",
							_meshCellIndicesLeaf.size()-1);

					for(FusionMipMapCPU::LeafNeighborhood::iterator
							i=meshCellIndicesLeafSingle.begin();i!=meshCellIndicesLeafSingle.end();i++){
						fprintf(stderr," %li",*i);
					}
				}
			}
		}
#endif

	}
	_nLeavesUsed++;
}


inline void addLeaf
(
		volumetype pointertochildindex,
		volumetype &childindex,
		volumetype &_nLeavesUsed,
		volumetype _nLeavesTotal,
		sidetype _brickLength,
		sidetype ox, sidetype oy, sidetype oz, sidetype half,
		sidetype childoffset,
		volumetype *_tree,
		sidetype3 *_leafPos,
		sidetype *_leafScale,
		volumetype pl,
		ParentArray &_leafParent
)
{
	if((_tree[pointertochildindex] = childindex = _nLeavesUsed) < _nLeavesTotal){
		_leafPos[_nLeavesUsed].x = ox+half*((childoffset&1)!=0);
		_leafPos[_nLeavesUsed].y = oy+half*((childoffset&2)!=0);
		_leafPos[_nLeavesUsed].z = oz+half*((childoffset&4)!=0);
		_leafScale[_nLeavesUsed] = half/_brickLength;

		_leafParent.push_back(pl);

	}
	_nLeavesUsed++;
}

inline void queryPointDepthSingle_func_vis
(
		sidetype x, sidetype y, sidetype z, sidetype brickLengthTarget,
		const sidetype &_n, const sidetype &_brickLength, volumetype *_tree,
		volumetype &_nBranchesUsed,
		const volumetype &_nLeavesTotal, volumetype &_nLeavesUsed, volumetype &_nLeavesQueued,
		volumetype *_leafNumber, sidetype3 *_leafPos, sidetype *_leafScale,
		volumetype *_queueIndexOfLeaf
		,FusionMipMapCPU::MeshCellArray &_meshCells
		,FusionMipMapCPU::BranchIndicesArray &_meshCellIndicesBranch
		,FusionMipMapCPU::LeafIndicesArray &_meshCellIndicesLeaf
		,ParentArray &_leafParent
		,MCNSplit &_boundary
		,bool performIncrementalMeshing
)
{
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	sidetype half = _n>>1;
	volumetype branch = 0;
	volumetype pl = BRANCHINIT; //parentleaf

	while((half>>1)>=brickLengthTarget){
		sidetype childoffset = (x>=ox+half)*1 + (y>=oy+half)*2 + (z>=oz+half)*4;

		volumetype pointertochildindex = branch+childoffset+8;
		volumetype childindex = _tree[pointertochildindex];
		volumetype leafindex = _tree[pointertochildindex-8];

//		sidetype quarter = half/2;
//		sidetype oxnew = ox + half * ((childoffset & 1) != 0);
//		sidetype oynew = oy + half * ((childoffset & 2) != 0);
//		sidetype oznew = oz + half * ((childoffset & 4) != 0);

		//New: Insert ALL leaves along the path
//		if(leafindex>=BRANCHINIT){
//			addLeaf_vis(branch+childoffset,leafindex,_nLeavesUsed,_nLeavesTotal,_brickLength,
//					ox,oy,oz,half,childoffset,_tree,_leafPos,_leafScale,pl,_leafParent,
//					_meshCellIndicesBranch,_meshCellIndicesLeaf,_meshCells,performIncrementalMeshing);
//		}
		if(leafindex < _nLeavesTotal){
			pl = leafindex;
			if(_queueIndexOfLeaf[leafindex] >= _nLeavesQueued){
				//The Leaf Brick has not been queued yet
				_queueIndexOfLeaf[leafindex] = _nLeavesQueued;
				_leafNumber[_nLeavesQueued] = leafindex;
				_nLeavesQueued++;
			}
		}

		if(childindex == BRANCHINIT){
			//The child is not allocated
			_tree[pointertochildindex] = childindex = _nBranchesUsed;
			uchar childBranchSize = CHILDBRANCHSIZEFUNCTION;
			_nBranchesUsed += childBranchSize;

			if(performIncrementalMeshing){
				_meshCellIndicesBranch.resize(childindex+childBranchSize);
//				createMeshCellStructuresForBranch_list_splice(
				createMeshCellStructuresForBranch_list_vector_polymorph(
						_meshCells,_meshCellIndicesBranch,_meshCellIndicesLeaf,_boundary,
						branch+childoffset,childindex,pl,childBranchSize,_n);
//				fprintf(stderr,"\nNumber of MeshCells after new Branch: %li",_meshCells.size());
//#define BRANCHSUBDIVISION
//#include "meshcelltraversal.cpp"
//#undef BRANCHSUBDIVISION
			}
		}

    ox += half * ((childoffset & 1) != 0);
    oy += half * ((childoffset & 2) != 0);
    oz += half * ((childoffset & 4) != 0);
    branch = childindex;
    half >>= 1;
	}
	sidetype childoffset = (x>=ox+half)*1 + (y>=oy+half)*2 + (z>=oz+half)*4;
	volumetype pointertochildindex = branch+childoffset;
	volumetype childindex = _tree[pointertochildindex];
	if(childindex >= BRANCHINIT){
		//The Leaf has not been allocated yet

		addLeaf_vis(pointertochildindex,childindex,_nLeavesUsed,_nLeavesTotal,_brickLength,
				ox,oy,oz,half,childoffset,_tree,_leafPos,_leafScale,pl,_leafParent,
				_meshCellIndicesBranch,_meshCellIndicesLeaf,_meshCells,performIncrementalMeshing);

	}
	//Put the Leaf onto the Data Queue, if not already there
	if(childindex < _nLeavesTotal &&_queueIndexOfLeaf[childindex] >= _nLeavesQueued){
    //The Leaf Brick has not been queued yet
		_queueIndexOfLeaf[childindex] = _nLeavesQueued;
		_leafNumber[_nLeavesQueued] = childindex;
		_nLeavesQueued++;
	}
}


inline void queryPointDepthSingle_func_subtree
(
		sidetype x, sidetype y, sidetype z, sidetype brickLengthTarget,
		const sidetype &_n, const sidetype &_brickLength, volumetype *_tree,
		volumetype &_nBranchesUsed,
		const volumetype &_nLeavesTotal, volumetype &_nLeavesUsed, volumetype &_nLeavesQueued,
		volumetype *_leafNumber, sidetype3 *_leafPos, sidetype *_leafScale,
		volumetype *_queueIndexOfLeaf
		,ParentArray &_leafParent
		,volumetype *anchorTreeBuds
		,volumetype *anchorTreeBudsParentLeaf
		,volumetype *anchorLeafBuds
		,volumetype &_numberOfQueuedTreeBuds
		,volumetype &_numberOfQueuedLeafBuds
		,volumetype _treeSizeSinceMeshing
)
{
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	sidetype half = _n>>1;
	volumetype branch = 0;
	volumetype pl = BRANCHINIT; //parentleaf

	while((half>>1)>=brickLengthTarget){
		sidetype childoffset = (x>=ox+half)*1 + (y>=oy+half)*2 + (z>=oz+half)*4;

		volumetype pointertochildindex = branch+childoffset+8;
		volumetype childindex = _tree[pointertochildindex];
		volumetype leafindex = _tree[pointertochildindex-8];

//		sidetype quarter = half/2;
//		sidetype oxnew = ox + half * ((childoffset & 1) != 0);
//		sidetype oynew = oy + half * ((childoffset & 2) != 0);
//		sidetype oznew = oz + half * ((childoffset & 4) != 0);

		//New: Insert ALL leaves along the path
//		if(leafindex>=BRANCHINIT){
//			addLeaf(branch+childoffset,leafindex,_nLeavesUsed,_nLeavesTotal,_brickLength,
//					ox,oy,oz,half,childoffset,_tree,_leafPos,_leafScale,pl,_leafParent,
//					_meshCellIndicesBranch,_meshCellIndicesLeaf,_meshCells,performIncrementalMeshing);
//		}
		if(leafindex < _nLeavesTotal){
			pl = leafindex;
			if(_queueIndexOfLeaf[leafindex] >= _nLeavesQueued){
				//The Leaf Brick has not been queued yet
				_queueIndexOfLeaf[leafindex] = _nLeavesQueued;
				_leafNumber[_nLeavesQueued] = leafindex;
				_nLeavesQueued++;
			}
		}

		if(childindex == BRANCHINIT){
			//The child is not allocated
			_tree[pointertochildindex] = childindex = _nBranchesUsed;
			if(pointertochildindex<_treeSizeSinceMeshing){
				anchorTreeBuds[_numberOfQueuedTreeBuds] = pointertochildindex-8;
				anchorTreeBudsParentLeaf[_numberOfQueuedTreeBuds] = pl;
				_numberOfQueuedTreeBuds++;
			}
			uchar childBranchSize = CHILDBRANCHSIZEFUNCTION;
			_nBranchesUsed += childBranchSize;

		}

    ox += half * ((childoffset & 1) != 0);
    oy += half * ((childoffset & 2) != 0);
    oz += half * ((childoffset & 4) != 0);
    branch = childindex;
    half >>= 1;
	}
	sidetype childoffset = (x>=ox+half)*1 + (y>=oy+half)*2 + (z>=oz+half)*4;
	volumetype pointertochildindex = branch+childoffset;
	volumetype childindex = _tree[pointertochildindex];
	if(childindex >= BRANCHINIT){
		//The Leaf has not been allocated yet

		addLeaf(pointertochildindex,childindex,_nLeavesUsed,_nLeavesTotal,_brickLength,
				ox,oy,oz,half,childoffset,_tree,_leafPos,_leafScale,pl,_leafParent);
		anchorLeafBuds[_numberOfQueuedLeafBuds++] = pointertochildindex;

	}
	//Put the Leaf onto the Data Queue, if not already there
	if(childindex < _nLeavesTotal &&_queueIndexOfLeaf[childindex] >= _nLeavesQueued){
    //The Leaf Brick has not been queued yet
		_queueIndexOfLeaf[childindex] = _nLeavesQueued;
		_leafNumber[_nLeavesQueued] = childindex;
		_nLeavesQueued++;
	}
}



void FusionMipMapCPU::queryPointDepthSingle
(
		sidetype x, sidetype y, sidetype z, sidetype brickLengthTarget)
{
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	sidetype half = _n>>1;
	volumetype branch = 0;

	while((half>>1)>=brickLengthTarget){
		sidetype childoffset = (x>=ox+half)*1 + (y>=oy+half)*2 + (z>=oz+half)*4;

		volumetype pointertochildindex = branch+childoffset+8;
		volumetype childindex = _tree[pointertochildindex];
		volumetype leafindex = _tree[pointertochildindex-8];

		if(leafindex < _nLeavesTotal &&_queueIndexOfLeaf[leafindex] >= MAXLEAFNUMBER){
			//The Leaf Brick has not been queued yet
			_queueIndexOfLeaf[leafindex] = _nLeavesQueuedFrustum;
			_leafNumberFrustum[_nLeavesQueuedFrustum] = leafindex;
			_nLeavesQueuedFrustum++;
		}

		if(childindex == BRANCHINIT){
			//The child is not allocated
			_tree[pointertochildindex] = childindex = _nBranchesUsed; //Atomic
			_nBranchesUsed += 16; //Atomic
		}

    ox += half * ((childoffset & 1) != 0);
    oy += half * ((childoffset & 2) != 0);
    oz += half * ((childoffset & 4) != 0);
    branch = childindex;
    half >>= 1;
	}
	sidetype childoffset = (x>=ox+half)*1 + (y>=oy+half)*2 + (z>=oz+half)*4;
	volumetype pointertochildindex = branch+childoffset;
	volumetype childindex = _tree[pointertochildindex];
	if(childindex >= BRANCHINIT){
		//The Leaf has not been allocated yet
		if((_tree[pointertochildindex] = childindex = _nLeavesUsed) < _nLeavesTotal){
			_leafPos[_nLeavesUsed].x = ox+half*((childoffset&1)!=0);
			_leafPos[_nLeavesUsed].y = oy+half*((childoffset&2)!=0);
			_leafPos[_nLeavesUsed].z = oz+half*((childoffset&4)!=0);
			_leafScale[childindex] = half/_brickLength;
		}
		_nLeavesUsed++;
	}
	//Put the Leaf onto the Data Queue, if not already there
	if(childindex < _nLeavesTotal &&_queueIndexOfLeaf[childindex] >= _nLeavesQueuedSurface){
    //The Leaf Brick has not been queued yet
		_queueIndexOfLeaf[childindex] = _nLeavesQueuedSurface;
		_leafNumberSurface[_nLeavesQueuedSurface] = childindex;
		_nLeavesQueuedSurface++;
	}
}

inline void queryBoxDepthSingle_func
(
		sidetype3 minPos, sidetype3 maxPos, sidetype brickLengthTarget,
		const sidetype &_n, const sidetype &_brickLength, volumetype *_tree,
		volumetype &_nBranchesUsed,
		const volumetype &_nLeavesTotal, volumetype &_nLeavesUsed, volumetype &_nLeavesQueued,
		volumetype *_leafNumber, sidetype3 *_leafPos, sidetype *_leafScale,
		volumetype *_queueIndexOfLeaf,
		uchar *child, volumetype *_branchNumber
)
{
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	sidetype size = _n;  sidetype half = _n>>1;
	int level = 0; _branchNumber[0] = 0; bool noRecursion = false;
	sidetype childoffset;
	child[0] =
			((minPos.x<ox+half)*85 | (maxPos.x>=ox+half)*170) &
      ((minPos.y<oy+half)*51 | (maxPos.y>=oy+half)*204) &
      ((minPos.z<oz+half)*15 | (maxPos.z>=oz+half)*240);
	if(child[0]==0) level--;
	while(level>=0){
		childoffset = 0;
		//At this point we have either gone up (towards the root) from a child,
		// gone down (towards a leaf) in the tree,
		// or we started in the root
		if(noRecursion){
			//At this point we have come up from a recursion.
			//We find the last processed child we have just returned from, erase it
			//from the stack and reconstruct
			for(; (((1<<childoffset)&(child[level]))==0) && childoffset<8 ;childoffset++);
			if((1<<childoffset)&child[level]) {
				child[level] &= ~(1<<childoffset);
		    ox -= half * ((childoffset & 1) != 0);
		    oy -= half * ((childoffset & 2) != 0);
		    oz -= half * ((childoffset & 4) != 0);
			}
			childoffset++;
		}
		for(noRecursion=true;noRecursion&&childoffset<8;childoffset++){
			if((1<<childoffset)&child[level]){
				if((half>>1)<brickLengthTarget){
					volumetype pointertochildindex = _branchNumber[level]+childoffset;
					volumetype childindex = _tree[pointertochildindex];
					//The Child Node is a leaf
					if(childindex >= BRANCHINIT){
						//The Leaf has not been allocated yet
						if((_tree[pointertochildindex] = childindex = _nLeavesUsed) < _nLeavesTotal){
							_leafPos[_nLeavesUsed].x = ox+half*((childoffset&1)!=0);
							_leafPos[_nLeavesUsed].y = oy+half*((childoffset&2)!=0);
							_leafPos[_nLeavesUsed].z = oz+half*((childoffset&4)!=0);
							_leafScale[childindex] = half/_brickLength;
						}
						_nLeavesUsed++;
					}
					//Put the Leaf onto the Data Queue, if not already there
					if(childindex < _nLeavesTotal &&_queueIndexOfLeaf[childindex] >= _nLeavesQueued){
				    //The Leaf Brick has not been queued yet
						_queueIndexOfLeaf[childindex] = _nLeavesQueued;
						_leafNumber[_nLeavesQueued] = childindex;
						_nLeavesQueued++;
					}
				}
				else{
					volumetype pointertochildindex = _branchNumber[level]+childoffset+8;
					volumetype childindex = _tree[pointertochildindex];
					volumetype leafindex = _tree[pointertochildindex-8];

					if(leafindex < _nLeavesTotal &&_queueIndexOfLeaf[leafindex] >= _nLeavesQueued){
						//The Leaf Brick has not been queued yet
						_queueIndexOfLeaf[leafindex] = _nLeavesQueued;
						_leafNumber[_nLeavesQueued] = leafindex;
						_nLeavesQueued++;
					}

					if(childindex == BRANCHINIT){
						//The child is not allocated
						_tree[pointertochildindex] = childindex = _nBranchesUsed; //Atomic
						_nBranchesUsed += 16; //Atomic
					}
					//Recurse into the child
					level++;
			    ox += half * ((childoffset & 1) != 0);
			    oy += half * ((childoffset & 2) != 0);
			    oz += half * ((childoffset & 4) != 0);
			    size >>= 1; half >>= 1;
			    child[level] =
			  			((minPos.x<ox+half)*85 | (maxPos.x>=ox+half)*170) &
			        ((minPos.y<oy+half)*51 | (maxPos.y>=oy+half)*204) &
			        ((minPos.z<oz+half)*15 | (maxPos.z>=oz+half)*240);
			    _branchNumber[level] = childindex;
					noRecursion = false;
				}
			}// Leaf or Recursion
		}//Loop through children
		if(noRecursion){
			size <<= 1; half <<= 1;
			level--;
		}
	}
}


void FusionMipMapCPU::queryBoxDepthSingle(sidetype3 minPos, sidetype3 maxPos, sidetype brickLengthTarget)
{
	uchar *child = (uchar*)_child;
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	sidetype size = _n;  sidetype half = _n>>1;
	int level = 0; _branchNumber[0] = 0; bool noRecursion = false;
	sidetype childoffset;
	child[0] =
			((minPos.x<ox+half)*85 | (maxPos.x>=ox+half)*170) &
      ((minPos.y<oy+half)*51 | (maxPos.y>=oy+half)*204) &
      ((minPos.z<oz+half)*15 | (maxPos.z>=oz+half)*240);
	if(child[0]==0) level--;
	while(level>=0){
		childoffset = 0;
		//At this point we have either gone up (towards the root) from a child,
		// gone down (towards a leaf) in the tree,
		// or we started in the root
		if(noRecursion){
			//At this point we have come up from a recursion.
			//We find the last processed child we have just returned from, erase it
			//from the stack and reconstruct
			for(; (((1<<childoffset)&(child[level]))==0) && childoffset<8 ;childoffset++);
			if((1<<childoffset)&child[level]) {
				child[level] &= ~(1<<childoffset);
		    ox -= half * ((childoffset & 1) != 0);
		    oy -= half * ((childoffset & 2) != 0);
		    oz -= half * ((childoffset & 4) != 0);
			}
			childoffset++;
		}
		for(noRecursion=true;noRecursion&&childoffset<8;childoffset++){
			if((1<<childoffset)&child[level]){
				if((half>>1)<brickLengthTarget){
					volumetype pointertochildindex = _branchNumber[level]+childoffset;
					volumetype childindex = _tree[pointertochildindex];
					//The Child Node is a leaf
					if(childindex >= BRANCHINIT){
						//The Leaf has not been allocated yet
						if((_tree[pointertochildindex] = childindex = _nLeavesUsed) < _nLeavesTotal){
							_leafPos[_nLeavesUsed].x = ox+half*((childoffset&1)!=0);
							_leafPos[_nLeavesUsed].y = oy+half*((childoffset&2)!=0);
							_leafPos[_nLeavesUsed].z = oz+half*((childoffset&4)!=0);
							_leafScale[childindex] = half/_brickLength;
						}
						_nLeavesUsed++;
					}
					//Put the Leaf onto the Data Queue, if not already there
					if(childindex < _nLeavesTotal &&_queueIndexOfLeaf[childindex] >= MAXLEAFNUMBER){
				    //The Leaf Brick has not been queued yet
						_queueIndexOfLeaf[childindex] = _nLeavesQueuedSurface;
						_leafNumberSurface[_nLeavesQueuedSurface] = childindex;
						_nLeavesQueuedSurface++;
					}
				}
				else{
					volumetype pointertochildindex = _branchNumber[level]+childoffset+8;
					volumetype childindex = _tree[pointertochildindex];
					volumetype leafindex = _tree[pointertochildindex-8];


					if(leafindex < _nLeavesTotal &&_queueIndexOfLeaf[leafindex] >= MAXLEAFNUMBER){
						//The Leaf Brick has not been queued yet
						_queueIndexOfLeaf[leafindex] = _nLeavesQueuedFrustum;
						_leafNumberFrustum[_nLeavesQueuedFrustum] = leafindex;
						_nLeavesQueuedFrustum++;
					}

					if(childindex == BRANCHINIT){
						//The child is not allocated
						_tree[pointertochildindex] = childindex = _nBranchesUsed; //Atomic
						_nBranchesUsed += 16; //Atomic
					}
					//Recurse into the child
					level++;
			    ox += half * ((childoffset & 1) != 0);
			    oy += half * ((childoffset & 2) != 0);
			    oz += half * ((childoffset & 4) != 0);
			    size >>= 1; half >>= 1;
			    child[level] =
			  			((minPos.x<ox+half)*85 | (maxPos.x>=ox+half)*170) &
			        ((minPos.y<oy+half)*51 | (maxPos.y>=oy+half)*204) &
			        ((minPos.z<oz+half)*15 | (maxPos.z>=oz+half)*240);
			    _branchNumber[level] = childindex;
					noRecursion = false;
				}
			}// Leaf or Recursion
		}//Loop through children
		if(noRecursion){
			size <<= 1; half <<= 1;
			level--;
		}
	}
}


//TODO: Auf MipMapping umschreiben
void FusionMipMapCPU::queryBoxDepth1Negative
(sidetype3 minPos, sidetype3 maxPos,sidetype3 n1MinPos, sidetype3 n1MaxPos, sidetype brickLengthTarget)
{
	uchar *child = (uchar*)_child;
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	sidetype size = _n;  sidetype half = _n>>1;
	int level = 0; _branchNumber[0] = 0; bool noRecursion = false;
	sidetype childoffset;
	child[0] =
 		 ((minPos.x<ox+half)*85 | (maxPos.x>=ox+half)*170) &
     ((minPos.y<oy+half)*51 | (maxPos.y>=oy+half)*204) &
     ((minPos.z<oz+half)*15 | (maxPos.z>=oz+half)*240) &
   	 (((n1MinPos.x>=ox) || (n1MaxPos.x<ox+half))*85 | ((n1MinPos.x>=ox+half) || (n1MaxPos.x<ox+size))*170  |
   		((n1MinPos.y>=oy) || (n1MaxPos.y<oy+half))*51 | ((n1MinPos.y>=oy+half) || (n1MaxPos.y<oy+size))*204  |
   		((n1MinPos.z>=oz) || (n1MaxPos.z<oz+half))*15 | ((n1MinPos.z>=oz+half) || (n1MaxPos.z<oz+size))*240 );

//	child[0] &= ~negative1;

	if(child[0]==0) level--;
	while(level>=0){
		childoffset = 0;
		//At this point we have either gone up (towards the root) from a child,
		// gone down (towards a leaf) in the tree,
		// or we started in the root
		if(noRecursion){
			//At this point we have come up from a recursion.
			//We find the last processed child we have just returned from, erase it
			//from the stack and reconstruct
			for(; (((1<<childoffset)&(child[level]))==0) && childoffset<8 ;childoffset++);
			if((1<<childoffset)&child[level]) {
				child[level] &= ~(1<<childoffset);
		    ox -= half * ((childoffset & 1) != 0);
		    oy -= half * ((childoffset & 2) != 0);
		    oz -= half * ((childoffset & 4) != 0);
			}
			childoffset++;
		}
		for(noRecursion=true;noRecursion&&childoffset<8;childoffset++){
			if((1<<childoffset)&child[level]){
				if((half>>1)<brickLengthTarget){
					volumetype pointertochildindex = _branchNumber[level]+childoffset;
					volumetype childindex = _tree[pointertochildindex];
					//The Child Node is a leaf

					if(childindex >= BRANCHINIT){
						//The Leaf has not been allocated yet
						if((_tree[pointertochildindex] = childindex = _nLeavesUsed) < _nLeavesTotal){
							_leafPos[_nLeavesUsed].x = ox+half*((childoffset&1)!=0);
							_leafPos[_nLeavesUsed].y = oy+half*((childoffset&2)!=0);
							_leafPos[_nLeavesUsed].z = oz+half*((childoffset&4)!=0);
							_leafScale[childindex] = half/_brickLength;
						}
						_nLeavesUsed++;
					}
					//Put the Leaf onto the Data Queue, if not already there
					if(childindex < _nLeavesTotal &&_queueIndexOfLeaf[childindex] >= MAXLEAFNUMBER){
				    //The Leaf Brick has not been queued yet
						_queueIndexOfLeaf[childindex] = _nLeavesQueuedSurface;
						_leafNumberSurface[_nLeavesQueuedSurface] = childindex;
						_nLeavesQueuedSurface++;
					}
				}
				else{
					volumetype pointertochildindex = _branchNumber[level]+childoffset+8;
					volumetype childindex = _tree[pointertochildindex];
					volumetype leafindex = _tree[pointertochildindex-8];

					if(leafindex < _nLeavesTotal &&_queueIndexOfLeaf[leafindex] >= MAXLEAFNUMBER){
						//The Leaf Brick has not been queued yet
						_queueIndexOfLeaf[leafindex] = _nLeavesQueuedFrustum;
						_leafNumberFrustum[_nLeavesQueuedFrustum] = leafindex;
						_nLeavesQueuedFrustum++;
					}

					if(childindex == BRANCHINIT){
						//The child is not allocated
						_tree[pointertochildindex] = childindex = _nBranchesUsed; //Atomic
						_nBranchesUsed += 16; //Atomic
					}
					//Recurse into the child
					level++;
			    ox += half * ((childoffset & 1) != 0);
			    oy += half * ((childoffset & 2) != 0);
			    oz += half * ((childoffset & 4) != 0);
			    size >>= 1; half >>= 1;
			    child[level] =
			    		 ((minPos.x<ox+half)*85 | (maxPos.x>=ox+half)*170) &
			         ((minPos.y<oy+half)*51 | (maxPos.y>=oy+half)*204) &
			         ((minPos.z<oz+half)*15 | (maxPos.z>=oz+half)*240) &
			      	 (((n1MinPos.x>=ox) || (n1MaxPos.x<ox+half))*85 | ((n1MinPos.x>=ox+half) || (n1MaxPos.x<ox+size))*170  |
			      		((n1MinPos.y>=oy) || (n1MaxPos.y<oy+half))*51 | ((n1MinPos.y>=oy+half) || (n1MaxPos.y<oy+size))*204  |
			      		((n1MinPos.z>=oz) || (n1MaxPos.z<oz+half))*15 | ((n1MinPos.z>=oz+half) || (n1MaxPos.z<oz+size))*240 );
			    _branchNumber[level] = childindex;
					noRecursion = false;
				}
			}// Leaf or Recursion
		}//Loop through children
		if(noRecursion){
			size <<= 1; half <<= 1;
			level--;
		}
	}
}

//TODO: Auf MipMapping umschreiben
void FusionMipMapCPU::queryBoxDepth2Negative
(sidetype3 minPos, sidetype3 maxPos,
    		sidetype3 n1MinPos, sidetype3 n1MaxPos,sidetype3 n2MinPos, sidetype3 n2MaxPos, sidetype brickLengthTarget)
{
	uchar *child = (uchar*)_child;
	sidetype ox = 0; sidetype oy = 0; sidetype oz = 0;
	sidetype size = _n;  sidetype half = _n>>1;
	int level = 0; _branchNumber[0] = 0; bool noRecursion = false;
	sidetype childoffset;

	child[0] =
 		 ((minPos.x<ox+half)*85 | (maxPos.x>=ox+half)*170) &
      ((minPos.y<oy+half)*51 | (maxPos.y>=oy+half)*204) &
      ((minPos.z<oz+half)*15 | (maxPos.z>=oz+half)*240) &
   	 (((n1MinPos.x>=ox)      || (n1MaxPos.x<ox+half) || (n2MinPos.x>=ox)      || (n2MaxPos.x<ox+half))*85 |
   		((n1MinPos.x>=ox+half) || (n1MaxPos.x<ox+size) || (n2MinPos.x>=ox+half) || (n2MaxPos.x<ox+size))*170  |
   		((n1MinPos.y>=oy)      || (n1MaxPos.y<oy+half) || (n2MinPos.y>=oy)      || (n2MaxPos.y<oy+half))*51 |
   		((n1MinPos.y>=oy+half) || (n1MaxPos.y<oy+size) || (n2MinPos.y>=oy+half) || (n2MaxPos.y<oy+size))*204  |
   		((n1MinPos.z>=oz)      || (n1MaxPos.z<oz+half) || (n2MinPos.z>=oz)      || (n2MaxPos.z<oz+half))*15 |
   		((n1MinPos.z>=oz+half) || (n1MaxPos.z<oz+size) || (n2MinPos.z>=oz+half) || (n2MaxPos.z<oz+size))*240 ) ;


//	child[0] &= ~(negative1 | negative2);
//	child[0] &= ~negative1;

	if(child[0]==0) level--;
	while(level>=0){
		childoffset = 0;
		//At this point we have either gone up (towards the root) from a child,
		// gone down (towards a leaf) in the tree,
		// or we started in the root
		if(noRecursion){
			//At this point we have come up from a recursion.
			//We find the last processed child we have just returned from, erase it
			//from the stack and reconstruct
			for(; (((1<<childoffset)&(child[level]))==0) && childoffset<8 ;childoffset++);
			if((1<<childoffset)&child[level]) {
				child[level] &= ~(1<<childoffset);
		    ox -= half * ((childoffset & 1) != 0);
		    oy -= half * ((childoffset & 2) != 0);
		    oz -= half * ((childoffset & 4) != 0);
			}
			childoffset++;
		}
		for(noRecursion=true;noRecursion&&childoffset<8;childoffset++){
			if((1<<childoffset)&child[level]){
				if((half>>1)<brickLengthTarget){
					volumetype pointertochildindex = _branchNumber[level]+childoffset;
					volumetype childindex = _tree[pointertochildindex];
					//The Child Node is a leaf
					if(childindex >= BRANCHINIT){
						//The Leaf has not been allocated yet
						if((_tree[pointertochildindex] = childindex = _nLeavesUsed) < _nLeavesTotal){
							_leafPos[_nLeavesUsed].x = ox+half*((childoffset&1)!=0);
							_leafPos[_nLeavesUsed].y = oy+half*((childoffset&2)!=0);
							_leafPos[_nLeavesUsed].z = oz+half*((childoffset&4)!=0);
							_leafScale[childindex] = half/_brickLength;
						}
						_nLeavesUsed++;
					}
					//Put the Leaf onto the Data Queue, if not already there
					if(childindex < _nLeavesTotal &&_queueIndexOfLeaf[childindex] >= MAXLEAFNUMBER){
				    //The Leaf Brick has not been queued yet
						_queueIndexOfLeaf[childindex] = _nLeavesQueuedSurface;
						_leafNumberSurface[_nLeavesQueuedSurface] = childindex;
						_nLeavesQueuedSurface++;
					}
				}
				else{
					volumetype pointertochildindex = _branchNumber[level]+childoffset+8;
					volumetype childindex = _tree[pointertochildindex];
					volumetype leafindex = _tree[pointertochildindex-8];

					if(leafindex < _nLeavesTotal &&_queueIndexOfLeaf[leafindex] >= MAXLEAFNUMBER){
						//The Leaf Brick has not been queued yet
						_queueIndexOfLeaf[leafindex] = _nLeavesQueuedFrustum;
						_leafNumberFrustum[_nLeavesQueuedFrustum] = leafindex;
						_nLeavesQueuedFrustum++;
					}

					if(childindex == BRANCHINIT){
						//The child is not allocated
						_tree[pointertochildindex] = childindex = _nBranchesUsed; //Atomic
						_nBranchesUsed += 16; //Atomic
					}
					//Recurse into the child
					level++;
			    ox += half * ((childoffset & 1) != 0);
			    oy += half * ((childoffset & 2) != 0);
			    oz += half * ((childoffset & 4) != 0);
			    size >>= 1; half >>= 1;
			    child[level] =
			    		 ((minPos.x<ox+half)*85 | (maxPos.x>=ox+half)*170) &
			         ((minPos.y<oy+half)*51 | (maxPos.y>=oy+half)*204) &
			         ((minPos.z<oz+half)*15 | (maxPos.z>=oz+half)*240) &
			      	 (((n1MinPos.x>=ox)      || (n1MaxPos.x<ox+half) || (n2MinPos.x>=ox)      || (n2MaxPos.x<ox+half))*85 |
			      		((n1MinPos.x>=ox+half) || (n1MaxPos.x<ox+size) || (n2MinPos.x>=ox+half) || (n2MaxPos.x<ox+size))*170  |
			      		((n1MinPos.y>=oy)      || (n1MaxPos.y<oy+half) || (n2MinPos.y>=oy)      || (n2MaxPos.y<oy+half))*51 |
			      		((n1MinPos.y>=oy+half) || (n1MaxPos.y<oy+size) || (n2MinPos.y>=oy+half) || (n2MaxPos.y<oy+size))*204  |
			      		((n1MinPos.z>=oz)      || (n1MaxPos.z<oz+half) || (n2MinPos.z>=oz)      || (n2MaxPos.z<oz+half))*15 |
			      		((n1MinPos.z>=oz+half) || (n1MaxPos.z<oz+size) || (n2MinPos.z>=oz+half) || (n2MaxPos.z<oz+size))*240 ) ;
			    _branchNumber[level] = childindex;
					noRecursion = false;
				}
			}// Leaf or Recursion
		}//Loop through children
		if(noRecursion){
			size <<= 1; half <<= 1;
			level--;
		}
	}
}
