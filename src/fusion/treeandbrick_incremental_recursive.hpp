#ifndef TREEANDBRICK_INCREMENTAL_RECURSIVE_HPP
#define TREEANDBRICK_INCREMENTAL_RECURSIVE_HPP


void addInterior_incremental
(
		leafstack leaves,
		treeinfo info,
		volumetype index, sidetype size,
		sidetype ox, sidetype oy, sidetype oz,
		volumetype lastLeaf,
		const MarchingCubesIndexed &mc,
		const ParentArray &leafParent,
		std::vector<MeshCell> *meshCells = NULL
);

#endif
