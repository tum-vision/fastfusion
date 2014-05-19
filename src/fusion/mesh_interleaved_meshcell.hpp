#ifndef MESH_INTERLEAVED_MESHCELL_HPP
#define MESH_INTERLEAVED_MESHCELL_HPP

//#define BRICKVISUALIZATION
//#define COLORINVERSION

void addBrickInterior_indexed_incremental_optimized
(
		volumetype lastleaf, const ParentArray &leafParent,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		treeinfo info,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
);

void addBrickWallX_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
);


void addBrickWallY_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
);


void addBrickWallZ_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
);

void addBrickEdgeX_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1, volumetype lastleaf2, volumetype lastleaf3,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
);

void addBrickEdgeY_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1, volumetype lastleaf2, volumetype lastleaf3,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
);

void addBrickEdgeZ_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1, volumetype lastleaf2, volumetype lastleaf3,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
);

//TODO: Das ist noch nicht wirklich indiziert
void addBrickMiddle_indexed_incremental
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		volumetype lastleaf0, volumetype lastleaf1, volumetype lastleaf2, volumetype lastleaf3,
		volumetype lastleaf4, volumetype lastleaf5, volumetype lastleaf6, volumetype lastleaf7,
		const ParentArray &leafParent,
		const MarchingCubesIndexed &mc,
		MeshInterleaved *pmesh
);

#endif
