/*
 * mesh.hpp
 *
 *  Created on: Jan 14, 2013
 *      Author: steinbrf
 */

#ifndef MESH_HPP_
#define MESH_HPP_

#include <vector>
#include <string>
#include <fstream>
#include <fusion/definitions.h>
#include <opencv2/opencv.hpp>

#include "mesh.hpp"
//#include "mesh_interleaved.hpp"


//#define DISTZEROEPSILON 0.0000000000001
#define DISTZEROEPSILON 1e-40


#ifndef vertexcolortype
typedef unsigned short vertexcolortype;
#define VCMUL 256
#endif

#ifndef textype
typedef unsigned short textype;
#define cvtextype CV_8UC3
#define TEX_MULTIPLICATOR 256
#endif


//#include "geometryfusion_single_soa.cuh"
//#include "texturefusion.cuh"



#ifndef uchar
typedef unsigned char uchar;
#endif


#ifndef Vertex3f
typedef struct _Vertex3f
{
	_Vertex3f(float px=0.0f , float py=0.0f, float pz=0.0f){x=px;y=py;z=pz;}
	float x; float y; float z;
} Vertex3f;
#endif
//#ifndef VertexUchar
//typedef struct _VertexUchar
//{
//	_VertexUchar(uchar px=0 , uchar py=0, uchar pz=0){x=px;y=py;z=pz;}
//	uchar x; uchar y; uchar z;
//} VertexUchar;
//#endif

#ifndef VertexColor
typedef struct _VertexColor
{
	_VertexColor(colortype px=0 , colortype py=0, colortype pz=0){x=px;y=py;z=pz;}
	colortype x; colortype y; colortype z;
} VertexColor;
#endif




//int getCubeIndex(float d000, float d001, float d010, float d011,
//		float d100, float d101, float d110, float d111,
//		weighttype w000, weighttype w001, weighttype w010, weighttype w011,
//		weighttype w100, weighttype w101, weighttype w110, weighttype w111,
//		float isolevel = 0.0f);
//
//int getCubeIndex(float d000, float d001, float d010, float d011,
//		float d100, float d101, float d110, float d111,
//		float isolevel = 0.0f);

//#define BINARY true

#define VDISTTH 0.000001

typedef class vertex3d_ {
public:
	vertex3d_(double px = 0, double py = 0, double pz = 0): x(px), y(py), z(pz){}
	double x; double y; double z;
} Vertex;

#define VMR 0.005
class VertexMap
{
public:
	VertexMap();
	typedef std::pair<Vertex,std::vector<unsigned int> > entry;
	void insert(float x, float y, float z, unsigned int index, bool verbose=true);
	std::vector<int> _branches;
	std::vector<std::vector<entry> > leaves;
	Vertex _offset; Vertex _minimum; Vertex _maximum;
	double _size;
	int _root;
};


#include "mesh_interleaved.hpp"


class MeshSeparate
{
public:
	MeshSeparate(unsigned int verticesPerFace
//			,size_t reservedVertices = 0, size_t reservedIndices = 0
			);
	std::vector<float> x;
	std::vector<float> y;
	std::vector<float> z;
	std::vector<unsigned int> f;
	std::vector<float> nx;
	std::vector<float> ny;
	std::vector<float> nz;
	std::vector<uchar> r;
	std::vector<uchar> g;
	std::vector<uchar> b;
	std::vector<unsigned int> e;
	std::vector<float> ew;

	std::vector<unsigned int> cc;


	bool writeOBJ(std::string filename);
	bool writePLY(std::string filename, bool binary = true);
	MeshSeparate reduce(bool verbose = true);
	void addGridFace(int dim, float ox, float oy, float oz, float sx, float sy, float sz);
	void addAlignedLineCube(float ox, float oy, float oz, float sx, float sy, float sz, float edgeWidth = 2.0f);
	cv::Mat &addTexture(const cv::Mat &texture);
	cv::Mat &getLastTexture();

	MeshSeparate &operator+=(const MeshSeparate &mesh);

	MeshSeparate &operator=(const MeshSeparate &mesh);

	MeshSeparate sortConnectedComponents();
	MeshSeparate componentsOfSize(unsigned int nv);

	uchar _verticesPerFace;
	//Texture Coordinates
	std::vector<float> tx;
	std::vector<float> ty;

	std::vector<unsigned int> edge;
protected:
	std::vector<unsigned int> materialIndices;
	std::vector<cv::Mat> textures;
};

class PointerMeshFusion
{
public:
	PointerMeshFusion(const MeshSeparate &mesh);
	~PointerMeshFusion();
	float *x; float *y; float *z;
	unsigned int *f;
	vertexcolortype *r; vertexcolortype *g; vertexcolortype *b;
	unsigned int nv;
	unsigned int nf;
};

class MeshInterleaved;

class PointerMeshDraw
{
public:
	PointerMeshDraw(const MeshSeparate &mesh, int typeParameter = 0);
	PointerMeshDraw(const MeshInterleaved &mesh, int typeParameter = 0);
	PointerMeshDraw(const PointerMeshDraw &mesh, int typeParameter = 0);
	~PointerMeshDraw();
//	float *x; float *y; float *z;
	char *v;
	char *c;
	char *n;

	unsigned int *f;
	unsigned int *e;
	float *ew;
//	uchar *r; uchar *g; uchar *b;
	unsigned int nv;
	unsigned int nf;
	unsigned int ne;
	unsigned int nn;
	bool colored;

	int type;
};


class MarchingCubes
{
public:
	MarchingCubes();
	~MarchingCubes();
	int *getVertexList(Vertex3f *vertlist,
			sidetype x, sidetype y, sidetype z,
			float ox, float oy, float oz, float scale,
			float val0,float val1, float val2, float val3,
			float val4, float val5, float val6, float val7,uchar *cubeindex_return = 0);
	int *getVertexColors(VertexColor *collist,
			uchar cubeindex,
			float val0,float val1, float val2, float val3,
			float val4, float val5, float val6, float val7,
			VertexColor c0, VertexColor c1, VertexColor c2, VertexColor c3,
			VertexColor c4, VertexColor c5, VertexColor c6, VertexColor c7);
	int *getColoredTrianglesForCube(
			Vertex3f *vertlist, VertexColor *collist,
			sidetype x, sidetype y, sidetype z,
			float ox, float oy, float oz, float scale,
			volumetype idx0, volumetype idx1, volumetype idx2, volumetype idx3,
			volumetype idx4, volumetype idx5, volumetype idx6, volumetype idx7,
			float *values, weighttype *weights, colortype *r, colortype *g, colortype *b);
	int *getInterleavedColoredTrianglesForCube(
			Vertex3f *vertlist, VertexColor *collist,
			sidetype x, sidetype y, sidetype z,
			float ox, float oy, float oz, float scale,
			volumetype idx0, volumetype idx1, volumetype idx2, volumetype idx3,
			volumetype idx4, volumetype idx5, volumetype idx6, volumetype idx7,
			const float *values, const weighttype *weights, const colortype3 *color);
	int *getColoredTrianglesForCubePrecomputed(
			Vertex3f *vertlist,VertexColor *collist,
			sidetype x, sidetype y, sidetype z,
			float ox, float oy, float oz,  float sx, float sy, float sz,
			float val0, float val1, float val2, float val3,
			float val4, float val5, float val6, float val7,
			weighttype w0, weighttype w1, weighttype w2, weighttype w3,
			weighttype w4, weighttype w5, weighttype w6, weighttype w7,
			VertexColor c0, VertexColor c1, VertexColor c2, VertexColor c3,
			VertexColor c4, VertexColor c5, VertexColor c6, VertexColor c7,
			const colortype3 *color) const;

	static Vertex3f VertexInterp(Vertex3f p1,Vertex3f p2,double valp1,double valp2, double isolevel = 0.0f);
	static VertexColor VertexInterp(VertexColor p1,VertexColor p2,float valp1,float valp2, float isolevel = 0.0f);

	static int getCubeIndex(float d000, float d001, float d010, float d011,
			float d100, float d101, float d110, float d111,
			float isolevel = 0.0f);

//	static int getCubeIndex(float d000, float d001, float d010, float d011,
//			float d100, float d101, float d110, float d111,
//			weighttype w000, weighttype w001, weighttype w010, weighttype w011,
//			weighttype w100, weighttype w101, weighttype w110, weighttype w111,
//			float isolevel = 0.0f);
//	static int getCubeIndex(const float &d000, const float &d001, const float &d010, const float &d011,
//			const float &d100, const float &d101, const float &d110, const float &d111,
//			const weighttype &w000, const weighttype &w001, const weighttype &w010, const weighttype &w011,
//			const weighttype &w100, const weighttype &w101, const weighttype &w110, const weighttype &w111,
//			float isolevel = 0.0f);

	inline int getCubeIndex(float d000, float d001, float d010, float d011,
			float d100, float d101, float d110, float d111,
			weighttype w000, weighttype w001, weighttype w010, weighttype w011,
			weighttype w100, weighttype w101, weighttype w110, weighttype w111,
			float isolevel = 0.0f) const {
//	inline int getCubeIndex(const float &d000, const float &d001, const float &d010, const float &d011,
//						const float &d100, const float &d101, const float &d110, const float &d111,
//						const weighttype &w000, const weighttype &w001, const weighttype &w010, const weighttype &w011,
//						const weighttype &w100, const weighttype &w101, const weighttype &w110, const weighttype &w111,
//						float isolevel = 0.0f) const {
		if(w000==0 || w001==0 || w010==0 || w011==0 ||
			 w100==0 || w101==0 || w110==0 || w111==0  )
			return 0;
		int cubeindex = 0;
		if (d000 < isolevel) cubeindex |= 1;
		if (d001 < isolevel) cubeindex |= 2;
		if (d010 < isolevel) cubeindex |= 4;
		if (d011 < isolevel) cubeindex |= 8;
		if (d100 < isolevel) cubeindex |= 16;
		if (d101 < isolevel) cubeindex |= 32;
		if (d110 < isolevel) cubeindex |= 64;
		if (d111 < isolevel) cubeindex |= 128;
		return cubeindex;
//		return
//		(d000 < isolevel) |
//		(d001 < isolevel)<<1 |
//		(d010 < isolevel)<<2 |
//		(d011 < isolevel)<<3 |
//		(d100 < isolevel)<<4 |
//		(d101 < isolevel)<<5 |
//		(d110 < isolevel)<<6 |
//		(d111 < isolevel)<<7 ;
	}

	int *edgeTable;
	int **triTable;

	int *numberOfFaces;

};

class MarchingCubesIndexed : public MarchingCubes
{
public:
	MarchingCubesIndexed(sidetype brickLength, sidetype brickHeight);
  ~MarchingCubesIndexed();
  int **offsetTable;
};




#endif /* MESH_HPP_ */
