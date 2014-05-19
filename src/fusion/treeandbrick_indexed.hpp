#ifndef TREEANDBRICK_INDEXED_HPP
#define TREEANDBRICK_INDEXED_HPP


#include "treeandbrick.hpp"


void addBrickEdgeX_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3,
		const MarchingCubesIndexed &mc
);

void addBrickEdgeY_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3,
		const MarchingCubesIndexed &mc
);

void addBrickEdgeZ_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3,
		const MarchingCubesIndexed &mc
);

void addBrickWallX_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0,
		leafstack leaves1,
		const MarchingCubesIndexed &mc
);

void addBrickWallY_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0,
		leafstack leaves1,
		const MarchingCubesIndexed &mc
);
void addBrickWallZ_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0,
		leafstack leaves1,
		const MarchingCubesIndexed &mc
);



void addBrickInterior_indexed
(
		leafstack leaves,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		treeinfo info,
		const MarchingCubesIndexed &mc
);

void addBrickMiddle_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3,
		leafstack leaves4, leafstack leaves5, leafstack leaves6, leafstack leaves7,
		const MarchingCubesIndexed &mc
);


#endif


