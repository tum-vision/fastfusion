/*
 * mesh_interleaved.hpp
 *
 *  Created on: Sep 18, 2013
 *      Author: steinbrf
 */

#ifndef MESH_INTERLEAVED_HPP_
#define MESH_INTERLEAVED_HPP_

#include "definitions.h"
#include "mesh.hpp"
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

class Vertex2f{
public:
	Vertex2f(float x_p, float y_p);
	float x; float y;
};

//class Vertex3f{
//public:
//	Vertex3f(float x_p, float y_p, float z_p);
//	float x; float y; float z;
//};

class Color3b{
public:
	Color3b(uchar r_p, uchar g_p, uchar b_p);
	uchar r; uchar g; uchar b;
};

class MeshInterleaved
{
public:
	MeshInterleaved(unsigned int verticesPerFace
//			,size_t reservedVertices = 0, size_t reservedIndices = 0
			);
	std::vector<Vertex3f> vertices;
	std::vector<unsigned int> faces;
	std::vector<Vertex3f> normals;
	std::vector<Color3b> colors;
	std::vector<unsigned int> edges;

	bool writeOBJ(std::string filename);
	bool writePLY(std::string filename, bool binary = true);

	size_t size();
	size_t emptysize();

//	MeshInterleaved reduce(bool verbose = true);

	MeshInterleaved &operator+=(const MeshInterleaved &mesh);

//	MeshInterleaved &operator=(const MeshInterleaved &mesh);

//	MeshInterleaved sortConnectedComponents();
//	MeshInterleaved componentsOfSize(unsigned int nv);

	uchar _verticesPerFace;
	//Texture Coordinates
	std::vector<Vertex2f> texcoords;

protected:
	std::vector<unsigned int> materialIndices;
	std::vector<cv::Mat> textures;
};


#endif /* MESH_INTERLEAVED_HPP_ */
