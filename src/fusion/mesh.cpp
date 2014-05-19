/*
 * mesh.cpp
 *
 *  Created on: Jan 16, 2013
 *      Author: steinbrf
 */
#include "mesh.hpp"

#include <set>
#include <map>
#include <list>
#include <iostream>
#include "boost/tuple/tuple.hpp"
#include <boost/tuple/tuple_comparison.hpp>
#include <math.h>




PointerMeshFusion::PointerMeshFusion(const MeshSeparate &mesh)
: x(NULL), y(NULL), z(NULL), f(NULL), r(NULL), g(NULL), b(NULL),
  nv(mesh.x.size()), nf(mesh.f.size())
{
	x = new float[nv];
	y = new float[nv];
	z = new float[nv];
	r = new vertexcolortype[nv];
	g = new vertexcolortype[nv];
	b = new vertexcolortype[nv];
	if(nf){
		f = new unsigned int[nf];
		for(unsigned int i=0;i<nf;i++){
			f[i] = mesh.f[i];
		}
	}

	for(unsigned int i=0;i<nv;i++){
		x[i] = mesh.x[i];
		y[i] = mesh.y[i];
		z[i] = mesh.z[i];
	}
	if(mesh.r.size()){
		for(unsigned int i=0;i<nv;i++){
			r[i] = mesh.r[i];
			g[i] = mesh.g[i];
			b[i] = mesh.b[i];
		}
	}
	else{
		for(unsigned int i=0;i<nv;i++){
			r[i] = 0;
			g[i] = 0;
			b[i] = 0;
		}
	}

}



PointerMeshDraw::PointerMeshDraw(const MeshSeparate &mesh, int typeParameter)
: v(NULL), c(NULL), n(NULL), f(NULL), e(NULL), ew(NULL),
  nv(mesh.x.size()), nf(mesh.f.size()),ne(mesh.e.size()), nn(0), colored(mesh.r.size()!=0), type(0)
{
//	std::cerr << "\nCreating PointerMeshDraw";

	if(mesh.x.size()!=mesh.y.size() || mesh.x.size() != mesh.z.size())
		std::cerr << "\nERROR: Vertices have unequal Size: "
		<< mesh.x.size() << " " << mesh.y.size() << " " << mesh.z.size();


	typedef struct floatvertex_ {float x; float y; float z;} floatvertex;
	typedef struct charvertex_ {char x; char y; char z;} charvertex;



	if(typeParameter==1){
//		fprintf(stderr,"\nGetting non-indexed PointerMeshDraw from Mesh");
		type = 1;
		if(nf){
			v = new char[3*sizeof(float)*nf];
			n = new char[3*sizeof(float)*nf];
			if(colored) c = new char[3*nf];
			floatvertex *vvert = (floatvertex*)v;
			floatvertex *nvert = (floatvertex*)n;
			charvertex *cvert = (charvertex*)c;
			for(unsigned int i=0;i<nf;i++){
				unsigned int index = mesh.f[i];
				if(index >= nv) fprintf(stderr," ERROR: Vertex %i->%i / %i",i,f[i],nv);
				vvert[i].x = mesh.x[index];
				vvert[i].y = mesh.y[index];
				vvert[i].z = mesh.z[index];
				if(colored){
					cvert[i].x = mesh.r[index];
					cvert[i].y = mesh.g[index];
					cvert[i].z = mesh.b[index];
				}
			}
			for(unsigned int i=0;i<nf;i+=3){
				float d1x = vvert[i+0].x - vvert[i+2].x;
				float d1y = vvert[i+0].y - vvert[i+2].y;
				float d1z = vvert[i+0].z - vvert[i+2].z;
				float d2x = vvert[i+1].x - vvert[i+2].x;
				float d2y = vvert[i+1].y - vvert[i+2].y;
				float d2z = vvert[i+1].z - vvert[i+2].z;
				float n1 = d1y*d2z-d1z*d2y;
				float n2 = d1z*d2x-d1x*d2z;
				float n3 = d1x*d2y-d1y*d2x;
				float length = sqrtf(n1*n1+n2*n2+n3*n3);
				nvert[i+0].x = nvert[i+1].x = nvert[i+2].x = n1/length;
				nvert[i+0].y = nvert[i+1].y = nvert[i+2].y = n2/length;
				nvert[i+0].z = nvert[i+1].z = nvert[i+2].z = n3/length;
			}
			nv = nf;
			nn = nf;
		}
	}
	else{
		if(nv){
			v = new char[3*sizeof(float)*nv];
			if(colored) c = new char[3*nv];
		}

		if(nf){
			f = new unsigned int[nf];
			for(unsigned int i=0;i<nf;i++){
				f[i] = mesh.f[i];
				if(f[i] >= nv) fprintf(stderr," ERROR: Vertex %i->%i / %i",i,f[i],nv);
			}

			memcpy(f,mesh.f.data(),nf*sizeof(float));
//			for(unsigned int i=0;i<nf;i++){
//				if(f[i] >= nv) fprintf(stderr," ERROR: Vertex %i->%i / %i",i,f[i],nv);
//			}
		}


		float *vfloat = (float*)v;
		for(unsigned int i=0;i<nv;i++){
			vfloat[3*i+0] = mesh.x[i];
			vfloat[3*i+1] = mesh.y[i];
			vfloat[3*i+2] = mesh.z[i];
		}
		if(colored){
			for(unsigned int i=0;i<nv;i++){
				c[3*i+0] = mesh.r[i];
				c[3*i+1] = mesh.g[i];
				c[3*i+2] = mesh.b[i];
			}
		}

		if(ne){
			e = new unsigned int[ne];
			for(unsigned int i=0;i<ne;i++){
				e[i] = mesh.e[i];
			}
		}
		if(mesh.ew.size()){
			ew = new float[mesh.ew.size()];
			for(unsigned int i=0;i<mesh.ew.size();i++){
				ew[i] = mesh.ew[i];
			}
		}
	}
}



PointerMeshDraw::PointerMeshDraw(const MeshInterleaved &mesh, int typeParameter)
: v(NULL), c(NULL), n(NULL), f(NULL), e(NULL), ew(NULL),
  nv(mesh.vertices.size()), nf(mesh.faces.size()),ne(mesh.edges.size()), nn(0), colored(mesh.colors.size()!=0), type(0)
{
//	std::cerr << "\nCreating PointerMeshDraw from MeshInterleaved";

	typedef struct floatvertex_ {float x; float y; float z;} floatvertex;
	typedef struct charvertex_ {char x; char y; char z;} charvertex;

	if(typeParameter==1){
//		fprintf(stderr,"\nGetting non-indexed PointerMeshDraw from Mesh");
		type = 1;
		if(nf){
			v = new char[3*sizeof(float)*nf];
			n = new char[3*sizeof(float)*nf];
			if(colored) c = new char[3*nf];
			floatvertex *vvert = (floatvertex*)v;
			floatvertex *nvert = (floatvertex*)n;
			charvertex *cvert = (charvertex*)c;
			for(unsigned int i=0;i<nf;i++){
				unsigned int index = mesh.faces[i];
				if(index >= nv) fprintf(stderr," ERROR: Vertex %i->%i / %i",i,f[i],nv);
				vvert[i].x = mesh.vertices[index].x;
				vvert[i].y = mesh.vertices[index].y;
				vvert[i].z = mesh.vertices[index].z;
				if(colored){
					cvert[i].x = mesh.colors[index].r;
					cvert[i].y = mesh.colors[index].g;
					cvert[i].z = mesh.colors[index].b;
				}
			}
			for(unsigned int i=0;i<nf;i+=3){
				float d1x = vvert[i+0].x - vvert[i+2].x;
				float d1y = vvert[i+0].y - vvert[i+2].y;
				float d1z = vvert[i+0].z - vvert[i+2].z;
				float d2x = vvert[i+1].x - vvert[i+2].x;
				float d2y = vvert[i+1].y - vvert[i+2].y;
				float d2z = vvert[i+1].z - vvert[i+2].z;
				float n1 = d1y*d2z-d1z*d2y;
				float n2 = d1z*d2x-d1x*d2z;
				float n3 = d1x*d2y-d1y*d2x;
				float length = sqrtf(n1*n1+n2*n2+n3*n3);
				nvert[i+0].x = nvert[i+1].x = nvert[i+2].x = n1/length;
				nvert[i+0].y = nvert[i+1].y = nvert[i+2].y = n2/length;
				nvert[i+0].z = nvert[i+1].z = nvert[i+2].z = n3/length;
			}
			nv = nf;
			nn = nf;
		}
	}
	else{
		if(nv){
			v = new char[3*sizeof(float)*nv];
			if(colored) c = new char[3*nv];
		}

		if(nf){
			f = new unsigned int[nf];
			for(unsigned int i=0;i<nf;i++){
				f[i] = mesh.faces[i];
				if(f[i] >= nv) fprintf(stderr," ERROR: Vertex %i->%i / %i",i,f[i],nv);
			}

			memcpy(f,mesh.faces.data(),nf*sizeof(float));
//			for(unsigned int i=0;i<nf;i++){
//				if(f[i] >= nv) fprintf(stderr," ERROR: Vertex %i->%i / %i",i,f[i],nv);
//			}
		}


		float *vfloat = (float*)v;
		for(unsigned int i=0;i<nv;i++){
			vfloat[3*i+0] = mesh.vertices[i].x;
			vfloat[3*i+1] = mesh.vertices[i].y;
			vfloat[3*i+2] = mesh.vertices[i].z;
		}
		if(colored){
			for(unsigned int i=0;i<nv;i++){
				c[3*i+0] = mesh.colors[i].r;
				c[3*i+1] = mesh.colors[i].g;
				c[3*i+2] = mesh.colors[i].b;
			}
		}

		if(ne){
			e = new unsigned int[ne];
			for(unsigned int i=0;i<ne;i++){
				e[i] = mesh.edges[i];
			}
		}
	}
}


PointerMeshDraw::PointerMeshDraw(const PointerMeshDraw &mesh, int typeParameter)
: v(NULL), c(NULL), n(NULL), f(NULL), e(NULL), ew(NULL),
  nv(0), nf(0),ne(0), nn(0), colored(mesh.colored), type(0)
{
//	std::cerr << "\nCreating PointerMeshDraw from other PointerMeshDraw";

	if(typeParameter == mesh.type){
		nv = mesh.nv;
		nf = mesh.nf;
		ne = mesh.ne;
		nn = mesh.nn;
		colored = mesh.colored;
		type = mesh.type;

		v = new char[3*sizeof(float)*nv];
		memcpy(v,mesh.v,3*sizeof(float)*nv);

		if(nf){
			f = new unsigned int [nf];
			memcpy(f,mesh.f,sizeof(unsigned int)*nf);
		}
		if(ne){
			e = new unsigned int [ne];
			memcpy(e,mesh.e,sizeof(unsigned int)*ne);
		}
		if(colored){
			c = new char[3*nv];
			memcpy(c,mesh.c,3*nv);
		}
		if(nn){
			n = new char[3*sizeof(float)*nv];
			memcpy(n,mesh.n,3*sizeof(float)*nv);
		}
	}
	else if(typeParameter == 1 && mesh.type == 0){
		nv = mesh.nf + mesh.ne;
		nf = mesh.nf;
		ne = mesh.ne;
		nn = nf;
		colored = mesh.colored;
		type = 1;
		v = new char[3*sizeof(float)*nv];
		n = new char[3*sizeof(float)*nn];
		if(colored) c = new char[3*nv];
		if(ne){
			for(unsigned int edge = 3*nf;edge<3*(nf+ne);edge++){
				c[edge+0] = c[edge+1] = c[edge+2] = 255;
			}
		}

		typedef struct floatvertex_ {float x; float y; float z;} floatvertex;
		typedef struct charvertex_ {char x; char y; char z;} charvertex;

		floatvertex *vin = (floatvertex*)mesh.v;
		floatvertex *vout = (floatvertex*)v;
		charvertex *cin = (charvertex*)mesh.c;
		charvertex *cout = (charvertex*)c;
		floatvertex *nvert = (floatvertex*)n;

		for(unsigned int i=0;i<nf;i++){
			unsigned int index = mesh.f[i];
			if(index >= nv) fprintf(stderr," ERROR: Vertex %i->%i / %i",i,f[i],nv);
			vout[i] = vin[index];
			if(colored){
				cout[i] = cin[index];
			}
		}
		for(unsigned int i=0;i<nn;i+=3){
			float d1x = vout[i+0].x - vout[i+2].x;
			float d1y = vout[i+0].y - vout[i+2].y;
			float d1z = vout[i+0].z - vout[i+2].z;
			float d2x = vout[i+1].x - vout[i+2].x;
			float d2y = vout[i+1].y - vout[i+2].y;
			float d2z = vout[i+1].z - vout[i+2].z;
			float n1 = d1y*d2z-d1z*d2y;
			float n2 = d1z*d2x-d1x*d2z;
			float n3 = d1x*d2y-d1y*d2x;
			float length = sqrtf(n1*n1+n2*n2+n3*n3);
			nvert[i+0].x = nvert[i+1].x = nvert[i+2].x = n1/length;
			nvert[i+0].y = nvert[i+1].y = nvert[i+2].y = n2/length;
			nvert[i+0].z = nvert[i+1].z = nvert[i+2].z = n3/length;
		}

//		fprintf(stderr,"\nCreated non-indexed PointerMeshDraw from indexed PointerMeshDraw");
	}
	else{
		fprintf(stderr,"\nERROR: Invalid PointerMesh Transformation!");
	}
}

PointerMeshDraw::~PointerMeshDraw()
{
	if(v){
		delete v;
	}
	if(c){
		delete c;
	}
	if(n){
		delete n;
	}
	if(f){
		delete f;
	}
	if(e) {
		delete e;
	}
	if(ew) {
		delete ew;
	}
}

PointerMeshFusion::~PointerMeshFusion()
{
	if(x) delete [] x;
	if(y) delete [] y;
	if(z) delete [] z;
	if(r) delete [] r;
	if(g) delete [] g;
	if(b) delete [] b;
	if(f) delete [] f;
}




MeshSeparate::MeshSeparate(unsigned int verticesPerFace
//		,size_t reservedVertices, size_t reservedIndices
		) :
_verticesPerFace(verticesPerFace)
{
//	if(reservedVertices){
//		x.reserve(reservedVertices);
//		y.reserve(reservedVertices);
//		z.reserve(reservedVertices);
//		r.reserve(reservedVertices);
//		g.reserve(reservedVertices);
//		b.reserve(reservedVertices);
//	}
//	if(reservedIndices){
//		f.reserve(reservedIndices);
//	}
}

MeshSeparate &MeshSeparate::operator+=(const MeshSeparate &mesh)
{

	unsigned int oldVertexSize = x.size();
	unsigned int oldFaceSize = f.size();

	for(unsigned int i=0;i<mesh.x.size();i++){
		x.push_back(mesh.x[i]);
		y.push_back(mesh.y[i]);
		z.push_back(mesh.z[i]);
	}
	if(mesh.r.size()){
		for(unsigned int i=0;i<mesh.r.size();i++){
			r.push_back(mesh.r[i]);
			g.push_back(mesh.g[i]);
			b.push_back(mesh.b[i]);
		}
	}
	if(mesh.f.size()){
		for(unsigned int i=0;i<mesh.f.size();i++){
			f.push_back(mesh.f[i]+oldVertexSize);
		}
	}
	if(mesh.e.size()){
		for(unsigned int i=0;i<mesh.e.size();i++){
			e.push_back(mesh.e[i]+oldVertexSize);
		}
	}
	if(mesh.ew.size()){
		for(unsigned int i=0;i<mesh.ew.size();i++){
			ew.push_back(mesh.ew[i]);
		}
	}

	if(mesh.textures.size()){
		for(unsigned int i=0;i<mesh.textures.size();i++){
			materialIndices.push_back(mesh.materialIndices[i]+oldFaceSize);
			textures.push_back(mesh.textures[i].clone());
		}
	}
	if(mesh.tx.size()){
		for(unsigned int i=0;i<mesh.tx.size();i++){
			tx.push_back(mesh.tx[i]);
			ty.push_back(mesh.ty[i]);
		}
	}

	return *this;
}

MeshSeparate &MeshSeparate::operator=(const MeshSeparate &mesh)
{

//	size_t oldVertexSize = x.size();
//	size_t oldFaceSize = f.size();

	x = mesh.x; y = mesh.y; z = mesh.z;
	r = mesh.r; g = mesh.g; b = mesh.b;
	f = mesh.f;
	e = mesh.e;
	ew = mesh.ew;

	textures = mesh.textures;
	tx = mesh.tx;

	return *this;
}

class compare_vectors {
   public:
      bool operator()(const std::vector<unsigned int> x,const std::vector<unsigned int> y)
      { return x.size()>y.size(); }
};

MeshSeparate MeshSeparate::componentsOfSize(unsigned int nv)
{
	if(!f.size() || e.size() || ew.size() || (r.size() && r.size()!=x.size())){
		fprintf(stderr,"\nERROR: Can only sort components for simple Face Mesh");
		return *this;
	}
	MeshSeparate intermediate = sortConnectedComponents();
	MeshSeparate result(3);

	fprintf(stderr,"\nIntermediate Mesh has %li components",intermediate.cc.size());
	for(unsigned int i=0;i<intermediate.cc.size();i++){
//		fprintf(stderr," %i",i);
		unsigned int size = i<intermediate.cc.size()-1 ? intermediate.cc[i+1]-intermediate.cc[i] : intermediate.x.size()-intermediate.cc[i];
//		fprintf(stderr,":%i",size);
		if(size >= nv){
			for(unsigned int j=0;j<size;j++){
				result.x.push_back(intermediate.x[intermediate.cc[i]+j]);
				result.y.push_back(intermediate.y[intermediate.cc[i]+j]);
				result.z.push_back(intermediate.z[intermediate.cc[i]+j]);
				if(r.size()) result.r.push_back(intermediate.r[intermediate.cc[i]+j]);
				if(g.size()) result.g.push_back(intermediate.g[intermediate.cc[i]+j]);
				if(b.size()) result.b.push_back(intermediate.b[intermediate.cc[i]+j]);
			}
		}
	}

	if(intermediate.f.size()){
		for(unsigned int i=0;i<intermediate.f.size();i+=_verticesPerFace){
			bool validface = intermediate.f[i] < result.x.size();
			for(unsigned int j=1;j<_verticesPerFace;j++){
				validface &= intermediate.f[i+j] < result.x.size();
			}
			if(validface){
				for(unsigned int j=0;j<_verticesPerFace;j++){
					result.f.push_back(intermediate.f[i+j]);
				}
			}
		}
	}


	return result;
}

MeshSeparate MeshSeparate::sortConnectedComponents()
{
	if(!f.size() || e.size() || ew.size() || (r.size() && r.size()!=x.size())){
		fprintf(stderr,"\nERROR: Can only sort components for simple Face Mesh");
		return *this;
	}
	MeshSeparate result(_verticesPerFace);

	std::vector<unsigned int> componentIndices(x.size());

	std::vector<std::vector<unsigned int> > facesForVertex(x.size());

	for(unsigned int i=0;i<f.size();i+=_verticesPerFace){
		for(unsigned int j=0;j<_verticesPerFace;j++){
			facesForVertex[f[i+j]].push_back(i);
		}
	}

//	fprintf(stderr,"\nNumber of Faces for each Vertex:\n");
//	for(unsigned int i=0;i<facesForVertex.size();i++){
//		fprintf(stderr," (%i %i)",i,facesForVertex[i].size());
//	}


	for(unsigned int i=0;i<componentIndices.size();i++){
		componentIndices[i] = i;
	}


	for(unsigned int i=0;i<componentIndices.size();i++){
		if(componentIndices[i] >= i){
//			fprintf(stderr,"\n\nNew Component %i:\n",i);
			std::list<unsigned int> queue;
			queue.push_back(i);
			while(!queue.empty()){
				unsigned int current = queue.front();
//				fprintf(stderr,"\nV(%i)",current);
				queue.pop_front();
				for(unsigned int j=0;j<facesForVertex[current].size();j++){
					unsigned int face = facesForVertex[current][j];
//					fprintf(stderr," F(%i)(",face);
					for(unsigned int k=0;k<_verticesPerFace;k++)
					{
//						fprintf(stderr,"%i ",f[face+k]);
						if(componentIndices[f[face+k]] > i){
							componentIndices[f[face+k]] = i;
							queue.push_back(f[face+k]);
						}
					}
//					fprintf(stderr,")");
				}
			}
		}
	}

//	fprintf(stderr,"\nComponent Indices:\n");
//	for(unsigned int i=0;i<componentIndices.size();i++){
//		fprintf(stderr," (%i %i)",i,componentIndices[i]);
//	}

	std::map<unsigned int,std::vector<unsigned int> > cmap;
	for(unsigned int i=0;i<componentIndices.size();i++){
		cmap[componentIndices[i]].push_back(i);
	}

	std::multiset<std::vector<unsigned int>,compare_vectors> components;
	unsigned int sum1 = 0;
	unsigned int i2 = 0;
	for(std::map<unsigned int,std::vector<unsigned int> >::iterator i=cmap.begin();
			i!=cmap.end();i++){
		components.insert(i->second);
		sum1 += i->second.size();
		if(i->second.size()<3){
			fprintf(stderr,"\nERROR Component: "); for(unsigned int j=0;j<i->second.size();j++){
				fprintf(stderr," (%i %i)",i->first,i->second[j]);
			}
		}
		i2++;
	}
	fprintf(stderr,"\nSum1: %i",sum1);

//	unsigned int counter = 0;
//	unsigned int sum = 0;
//	for(std::multiset<std::vector<unsigned int>,compare_vectors>::iterator i=components.begin();
//			i!=components.end();i++){
////		fprintf(stderr," (%i|%i)",counter++,(int)i->size());
//		sum += i->size();
//	}
//	fprintf(stderr,"\nAll components together have %i Vertices",sum);

	std::vector<unsigned int> indices(x.size(),x.size());
	unsigned int index = 0;
	unsigned int componentCounter = 0;
	for(std::multiset<std::vector<unsigned int>,compare_vectors>::iterator i=components.begin();
			i!=components.end();i++){
		result.cc.push_back(index);
		for(unsigned int j=0;j<i->size();j++){
			indices[(*i)[j]] = index++;
		}
		componentCounter++;
	}

	for(unsigned int i=0;i<indices.size();i++){
		if(indices[i] >= indices.size()){
			fprintf(stderr,"\nERROR: Index was not touched!");
		}
	}

//	fprintf(stderr,"\nIndices:\n");
//	for(unsigned int i=0;i<indices.size();i++) fprintf(stderr," %i",indices[i]);

	result.x.resize(x.size());
	result.y.resize(y.size());
	result.z.resize(z.size());
	if(r.size()) result.r.resize(r.size());
	if(g.size()) result.g.resize(g.size());
	if(b.size()) result.b.resize(b.size());

	for(unsigned int i=0;i<x.size();i++){
		unsigned int j=indices[i];
		result.x[j] = x[i];
		result.y[j] = y[i];
		result.z[j] = z[i];
		if(r.size()) result.r[j] = r[i];
		if(g.size()) result.g[j] = g[i];
		if(b.size()) result.b[j] = b[i];
	}

	for(unsigned int i=0;i<f.size();i+=_verticesPerFace){
		for(unsigned int j=0;j<_verticesPerFace;j++){
			result.f.push_back(indices[f[i+j]]);
		}
	}

//	fprintf(stderr,"\nComponents:\n");
//	for(unsigned int i=0;i<cc.size();i++) fprintf(stderr," %i",cc[i]);

	fprintf(stderr,"\nMesh has %i Components: ",(int)components.size());
	fprintf(stderr,"\nSorted Mesh has %i Vertices and %i Faces",(int)result.x.size(),(int)result.f.size());

	return result;
}

typedef boost::tuple<float,float,float> FVertex;
typedef std::map<FVertex,unsigned int> Vertexmap;

VertexMap::VertexMap()
{
	_offset.x = _offset.y = _offset.z = 0.0;
	_size = 0.0;
	_branches.resize(8,0);
	_root = 0;
	leaves.resize(1);
}
void VertexMap::insert(float x, float y, float z, unsigned int index, bool verbose)
{
	if(_size == 0.0){
		if(verbose) fprintf(stderr,"\nFirst Vertex: (%f %f %f)",x,y,z);
		_size = 2*VMR;
		_offset.x = x-VMR; _offset.y = y-VMR; _offset.z = z-VMR;
		_minimum.x = _maximum.x = x;
		_minimum.y = _maximum.y = y;
		_minimum.z = _maximum.z = z;
	}

	if(_minimum.x > x) _minimum.x = x;
	if(_minimum.y > y) _minimum.y = y;
	if(_minimum.z > z) _minimum.z = z;
	if(_maximum.x < x) _maximum.x = x;
	if(_maximum.y < y) _maximum.y = y;
	if(_maximum.z < z) _maximum.z = z;

	while(x < _offset.x || y < _offset.y || z < _offset.z ||
		 x >= _offset.x+_size || y >= _offset.y+_size || z >= _offset.z+_size){
		int childoffset = 0;
		if(x<_offset.x){
			_offset.x -= _size;
			childoffset+=1;
		}
		if(y<_offset.y){
			_offset.y -= _size;
			childoffset+=2;
		}
		if(z<_offset.z){
			_offset.z -= _size;
			childoffset+=4;
		}
		_branches.resize(_branches.size()+8,0);
		_branches[_branches.size()-8+childoffset] = _root-(_branches.size()-8+childoffset);
		_root = _branches.size()-8;
		_size *= 2.0f;
		if(verbose) fprintf(stderr,"\nVertex: (%f %f %f)",x,y,z);
		if(verbose) fprintf(stderr,"\nNew Map: [%f %f %f]-[%f %f %f] : %f",
				_offset.x,_offset.y,_offset.z,_offset.x+_size,_offset.y+_size,_offset.z+_size,_size);
	}

	int branch = _root;
	float ox = _offset.x; float oy = _offset.y; float oz = _offset.z;
	for(float s=_size;s>2*VMR;s*=0.5f){
		float half = s*0.5f;
		int childoffset = 1*(x>=ox+half) + 2*(y>=oy+half) + 4*(z>=oz+half);
		if(!_branches[branch+childoffset]){
			_branches.resize(_branches.size()+8,0);
			_branches[branch+childoffset] = _branches.size()-8-(branch+childoffset);
		}
		branch = branch+childoffset + _branches[branch+childoffset];
		ox += half*(x>=ox+half);
		oy += half*(y>=oy+half);
		oz += half*(z>=oz+half);
	}

	int childoffset = 1*(x>=ox+VMR) + 2*(y>=oy+VMR) + 4*(z>=oz+VMR);
	if(!_branches[branch+childoffset]){
		leaves.push_back(std::vector<entry>());
		_branches[branch+childoffset] = leaves.size()-1;
	}

	bool fused = false;
	for(unsigned int i=0;i<leaves[_branches[branch+childoffset]].size();i++){
		Vertex v = leaves[_branches[branch+childoffset]][i].first;
//		double distance = ((double)x-(double)v.x)*((double)x-(double)v.x) +
//											((double)y-(double)v.y)*((double)y-(double)v.y) +
//											((double)z-(double)v.z)*((double)z-(double)v.z);
		double distance = sqrt(((double)x-(double)v.x)*((double)x-(double)v.x) +
													 ((double)y-(double)v.y)*((double)y-(double)v.y) +
													 ((double)z-(double)v.z)*((double)z-(double)v.z));
		if(distance < VDISTTH){
			leaves[_branches[branch+childoffset]][i].second.push_back(index);
			fused = true;
			break;
		}
	}
	if(!fused){
		leaves[_branches[branch+childoffset]].push_back(
				std::pair<Vertex,std::vector<unsigned int> >(
						Vertex(x,y,z),std::vector<unsigned int>(1,index)));
	}
}

MeshSeparate MeshSeparate::reduce(bool verbose){
	fprintf(stderr,"\nReducing Mesh of %li Vertices and %li Faces",x.size(),f.size());
	MeshSeparate result(_verticesPerFace);

	if(verbose) fprintf(stderr,"\nFilling 3D tree ");
	VertexMap testmap;
	unsigned int i2=0;
	for(unsigned int i=0;i<x.size();i2=i,i++){
		if(i*100/x.size()!=i2*100/x.size()) if(verbose) fprintf(stderr," %i%%",i*100/(int)x.size());
//	for(unsigned int i=0;i<x.size();i++){
//		fprintf(stderr," %i",i);
		testmap.insert(x[i],y[i],z[i],i,verbose);
	}

	if(verbose) fprintf(stderr,"\nVertexMap has %li leaves",testmap.leaves.size());

	if(verbose) fprintf(stderr,"\nBounding Box: [%f %f %f]-[%f %f %f]",
			testmap._minimum.x,testmap._minimum.y,testmap._minimum.z,
			testmap._maximum.x,testmap._maximum.y,testmap._maximum.z);

	unsigned long sum =0;
	for(unsigned int i=0;i<testmap.leaves.size();i++){
		sum += testmap.leaves[i].size();
	}
	if(verbose) fprintf(stderr,"\nAll Clusters together: %li",sum);

	std::vector<unsigned int> indexshift(x.size());
	for(unsigned int i=0;i<indexshift.size();i++) indexshift[i] = i;



	for(unsigned int i=0;i<testmap.leaves.size();i++){
		for(unsigned int j=0;j<testmap.leaves[i].size();j++){
			for(unsigned int k=0;k<testmap.leaves[i][j].second.size();k++){
				indexshift[testmap.leaves[i][j].second[k]] = testmap.leaves[i][j].second[0];
			}
		}
	}

	std::vector<unsigned int> indexshiftcompressed(x.size());

	for(unsigned int i=0;i<x.size();i++){
		if(indexshift[i]==i){
			result.x.push_back(x[i]);
			result.y.push_back(y[i]);
			result.z.push_back(z[i]);
			if(r.size()) result.r.push_back(r[i]);
			if(g.size()) result.g.push_back(g[i]);
			if(b.size()) result.b.push_back(b[i]);
			indexshiftcompressed[i] = result.x.size()-1;
		}
		else{
			indexshiftcompressed[i] = indexshiftcompressed[indexshift[i]];
		}
	}
	if(verbose) fprintf(stderr,"\nReduced the Mesh from %li to %li Vertices",x.size(),result.x.size());

//	if(f.size()){
//		result.f.resize(f.size());
//		unsigned i2 = 0;
//		for(unsigned int i=0;i<f.size();i2=i,i++){
//			if(i*100/f.size()!=i2*100/f.size()) fprintf(stderr," %i%%",i*100/(int)f.size());
//			result.f[i] = indexshiftcompressed[f[i]];
//		}
//	}

	unsigned int degenerateFaces = 0;
	for(unsigned int i=0;i<f.size();i+=_verticesPerFace){
		bool validface = true;
		for(unsigned int j=1;j<_verticesPerFace;j++){
			for(unsigned int k=0;k<j;k++){
				validface &= indexshiftcompressed[f[i+j]] != indexshiftcompressed[f[i+k]];
			}
		}
		if(validface){
			for(unsigned int j=0;j<_verticesPerFace;j++){
				result.f.push_back(indexshiftcompressed[f[i+j]]);
			}
		}
		else degenerateFaces++;
	}
	if(verbose) fprintf(stderr,"\nThere are %i degenerate Faces",degenerateFaces);


//	Vertexmap vertexmap;
//	//Fill the Map
//	unsigned i2 = 0;
//	for(unsigned int i=0;i<x.size();i2=i,i++){
//		if(i*100/x.size()!=i2*100/x.size()) fprintf(stderr," %i%%",i*100/(int)x.size());
//		vertexmap[FVertex(x[i],y[i],z[i])];
//	}
//	//Read the Map
//	unsigned int idx = 0;
//	for(Vertexmap::iterator i= vertexmap.begin();i!=vertexmap.end();i++){
//		result.x.push_back(i->first.get<0>());
//		result.y.push_back(i->first.get<1>());
//		result.z.push_back(i->first.get<2>());
//		i->second = idx;
//		idx++;
//	}
//	if(r.size() && g.size() && b.size()){
//		result.r.resize(result.x.size());
//		result.g.resize(result.x.size());
//		result.b.resize(result.x.size());
//	}
//
//	if(f.size()){
//		result.f.resize(f.size());
//		unsigned i2 = 0;
//		for(unsigned int i=0;i<f.size();i2=i,i++){
//			if(i*100/f.size()!=i2*100/f.size()) fprintf(stderr," %i%%",i*100/(int)f.size());
//			result.f[i] = vertexmap[FVertex(x[f[i]],y[f[i]],z[f[i]])];
//		}
//	}
//
//	if(r.size() && g.size() && b.size()){
//		fprintf(stderr,"\nReducing the Colors");
//		unsigned i2 = 0;
//		for(unsigned int i=0;i<x.size();i2=i,i++){
//			if(i*100/x.size()!=i2*100/x.size()) fprintf(stderr," %i%%",i*100/(int)x.size());
//			idx = vertexmap[FVertex(x[i],y[i],z[i])];
//			result.r[idx] = r[i];
//			result.g[idx] = g[i];
//			result.b[idx] = b[i];
//		}
//		fprintf(stderr,"\nColors reduced from %i to %i Values",(int)r.size(),(int)result.r.size());
//	}
//
//	std::vector<unsigned int> eTemp;
//	if(e.size()){
//		eTemp.resize(e.size());
//		for(unsigned int i=0;i<e.size();i++){
//			eTemp[i] = vertexmap[FVertex(x[e[i]],y[e[i]],z[e[i]])];
//		}
//	}
//
//	fprintf(stderr,"\nReduced the Mesh from %i to %i Vertices",(unsigned int)x.size(),(unsigned int)result.x.size());
//	vertexmap.clear();
//
//	if(e.size()){
//		fprintf(stderr,"\nReducing Multiple Edges");
//		std::set<boost::tuple<unsigned int,unsigned int> > edgeset;
//		for(unsigned int i=0;i<eTemp.size();i+=2){
//			edgeset.insert(boost::tuple<unsigned int,unsigned int>(eTemp[i],eTemp[i+1]));
//		}
//		for(std::set<boost::tuple<unsigned int,unsigned int>,unsigned int>::iterator i= edgeset.begin();i!=edgeset.end();i++){
//			result.e.push_back(i->get<0>());
//			result.e.push_back(i->get<1>());
//		}
//		fprintf(stderr,"\nReduced the Amount of Edges from %i to %i",(int)e.size(),(int)result.e.size());
//	}

	if(!e.size()){
		fprintf(stderr,"\nReduced Mesh has %i Vertices and %i Faces",(int)result.x.size(),(int)result.f.size());
	}


	return result;
}




bool MeshSeparate::writeOBJ(std::string filename){

	std::string prefix = filename.substr(0,filename.find_last_of('/')+1);
	std::string stem = filename.substr(prefix.size(),filename.size());
	std::string filenameObject = filename + ".obj";
	std::string filenameMaterial = filename + ".mtl";
	std::string filenameMaterialRelative = stem + ".mtl";


	std::fstream file;
	file.open(filenameObject.c_str(),std::ios::out);
	if(!file.is_open()){
		fprintf(stderr,"\nERROR: Could not open File %s for writing!",filenameObject.c_str());
		return false;
	}

	std::vector<std::string> materialNames;
	bool useTexture = textures.size() > 0
			&& tx.size() == f.size() && ty.size() == f.size();
	if(useTexture){
		fprintf(stderr,"\nThe Mesh has %li Textures of the following Sizes:\n",textures.size());
		for(size_t i=0;i<textures.size();i++){
			fprintf(stderr," %i*%i",textures[i].cols,textures[i].rows);
			char buffer[500];
			sprintf(buffer,"material%.5li",i);
			materialNames.push_back(buffer);
		}
	}


	file << "\n#Written from Volume";
	if(useTexture) file << "\nmtllib " << filenameMaterialRelative;
	for(unsigned int i=0;i<x.size();i++){
		file << "\nv " << x[i] << " " << y[i] << " " << z[i];
		if(r.size() && g.size() && b.size()){
			file << " " << (float)r[i]/255.0f << " " << (float)g[i]/255.0f << " " << (float)b[i]/255.0f;
		}
	}

	//Normals
	for(unsigned int i=0;i<nx.size();i++){
		file << "\nvn " << nx[i] << " " << ny[i] << " " << nz[i];
	}


	//Texture
	if(useTexture){
		for(unsigned int t=0;t<tx.size();t++){
			file << "\nvt " << tx[t] << " " << ty[t];
		}
//		file << "\nusemtl material0";
	}

	if(nx.size()){
		//Normals are given
		for(unsigned int i=0;i<f.size();i+=_verticesPerFace){
			file << "\nf";
			for(unsigned int j=0;j<_verticesPerFace;j++)
				file << " " << f[i+j]+1 << "//" << f[i+j];
		}
	}
	else{
		//No normals
		if(useTexture){
			unsigned int currentMaterialNumber = 0;
			unsigned int nextMaterialIndex = 0;
			//No Normals, Texture Coordinates
			for(unsigned int i=0;i<f.size();i+=_verticesPerFace){
				if(nextMaterialIndex==i){
					file << "\nusemtl " << materialNames[currentMaterialNumber++];
					nextMaterialIndex =
							materialIndices.size()>currentMaterialNumber ?
									materialIndices[currentMaterialNumber] : f.size();
				}
				file << "\nf";
				for(unsigned int j=0;j<_verticesPerFace;j++)
					file << " " << f[i+j]+1 << "/" << (i+j+1);
			}
		}
		else{
			//No Texture, No Normals
			for(unsigned int i=0;i<f.size();i+=_verticesPerFace){
				file << "\nf";
				for(unsigned int j=0;j<_verticesPerFace;j++)
					file << " " << f[i+j]+1;
			}
		}
	}


	//Edges (Lines)
	for(unsigned int i=0;i<e.size();i+=2){
		file << "\nl " << e[i]+1 << " " <<  e[i+1]+1;
	}

	file.close();

	if(useTexture){
		std::fstream materialfile;
		materialfile.open(filenameMaterial.c_str(),std::ios::out);
		for(unsigned int i=0;i<textures.size();i++){
			char filenameTexture[500]; sprintf(filenameTexture,"%s_%.5i.png",filename.c_str(),i);
			char filenameTextureRelative[500]; sprintf(filenameTextureRelative,"%s_%.5i.png",stem.c_str(),i);
			if(!materialfile.is_open()){
				fprintf(stderr,"\nERROR: Could not open Material File %s for writing!",filenameMaterial.c_str());
				return false;
			}
			if(i) materialfile << "\n";
			materialfile << "newmtl " << materialNames[i]
			             << "\nKa 1.000000 1.000000 1.000000"
	                 << "\nKd 1.000000 1.000000 1.000000"
	                 << "\nKs 0.000000 0.000000 0.000000"
	                 << "\nTr 1.000000"
	                 << "\nillum 1"
	                 << "\nNs 0.000000"
	                 << "\nmap_Kd " << filenameTextureRelative;
			cv::imwrite(filenameTexture,textures[i]);
		}
		materialfile.close();
	}

	return true;
}

bool MeshSeparate::writePLY(std::string filename, bool binary){
	std::fstream file;
	file.open((filename+".ply").c_str(),std::ios::out|std::ios::binary);
	if(!file.is_open()){
		fprintf(stderr,"\nERROR: Could not open File %s for writing!",(filename+".ply").c_str());
		return false;
	}

	file << "ply";
	if(binary)file << "\nformat binary_little_endian 1.0";
	else file << "\nformat ascii 1.0";
	file << "\nelement vertex " << x.size();
	file << "\nproperty float32 x\nproperty float32 y\nproperty float32 z";
	if(r.size())
		file << "\nproperty uchar red\nproperty uchar green\nproperty uchar blue";
	if(f.size()){
		file << "\nelement face " << f.size()/_verticesPerFace;
		file << "\nproperty list uint8 int32 vertex_indices";
	}
	if(e.size()){
		file << "\nElement edge " << e.size()/2;
		file << "\nproperty int vertex1\nproperty int vertex2";
	}
	file << "\nend_header";
	if(binary) file << "\n";

	for(unsigned int i=0;i<x.size();i++){
		if(binary){
			file.write((char*)(&(x[i])),sizeof(float));
			file.write((char*)(&(y[i])),sizeof(float));
			file.write((char*)(&(z[i])),sizeof(float));
		}
		else file << "\n" << x[i] << " " << y[i] << " " << z[i];
		if(r.size()){
			if(binary){
				file.write((char*)(&(r[i])),sizeof(uchar));
				file.write((char*)(&(g[i])),sizeof(uchar));
				file.write((char*)(&(b[i])),sizeof(uchar));
			}
			else file << " " << (int)(r[i]) << " " << (int)(g[i]) << " " << (int)(b[i]);
		}
	}
	for(unsigned int i=0;i<f.size();i+=_verticesPerFace){
		if(binary){
			file.write((char*)(&_verticesPerFace),sizeof(uchar));
		}
		else file << "\n" << (int)_verticesPerFace;
		for(unsigned int j=0;j<_verticesPerFace;j++)
			if(binary){
				unsigned int idx = f[i+j];
				file.write((char*)(&idx),sizeof(unsigned int));
			}
			else file << " " << (f[i+j]);
	}
	for(unsigned int i=0;i<e.size();i+=2){
		if(binary){
			unsigned int idx = e[i];
			file.write((char*)(&idx),sizeof(unsigned int));
			idx = e[i+1]; file.write((char*)(&idx),sizeof(unsigned int));
		}
		else file << "\n " << e[i] << " " << e[i+1];
	}

	file.close();
	return true;
}


void MeshSeparate::addGridFace(int dim, float ox, float oy, float oz, float sx, float sy, float sz)
{
	if(_verticesPerFace == 3){
		switch(dim){
		case 0:
			x.push_back(ox);y.push_back(oy);   z.push_back(oz);   f.push_back(x.size()-1);
			x.push_back(ox);y.push_back(oy+sy);z.push_back(oz);   f.push_back(x.size()-1);
			x.push_back(ox);y.push_back(oy+sy);z.push_back(oz+sz);f.push_back(x.size()-1);
			x.push_back(ox);y.push_back(oy);   z.push_back(oz+sz);
			f.push_back(x.size()-4); f.push_back(x.size()-2); f.push_back(x.size()-1);
			break;
		case 1:
			x.push_back(ox);   y.push_back(oy);z.push_back(oz);   f.push_back(x.size()-1);
			x.push_back(ox+sx);y.push_back(oy);z.push_back(oz);   f.push_back(x.size()-1);
			x.push_back(ox+sx);y.push_back(oy);z.push_back(oz+sz);f.push_back(x.size()-1);
			x.push_back(ox);   y.push_back(oy);z.push_back(oz+sz);
			f.push_back(x.size()-4); f.push_back(x.size()-2); f.push_back(x.size()-1);
			break;
		case 2:
			x.push_back(ox);   y.push_back(oy);   z.push_back(oz);f.push_back(x.size()-1);
			x.push_back(ox+sx);y.push_back(oy);   z.push_back(oz);f.push_back(x.size()-1);
			x.push_back(ox+sx);y.push_back(oy+sy);z.push_back(oz);f.push_back(x.size()-1);
			x.push_back(ox);   y.push_back(oy+sy);z.push_back(oz);
			f.push_back(x.size()-4); f.push_back(x.size()-2); f.push_back(x.size()-1);
		}
	}
	else if(_verticesPerFace == 4){
		switch(dim){
		case 0:
			x.push_back(ox);y.push_back(oy);   z.push_back(oz);   f.push_back(x.size()-1);
			x.push_back(ox);y.push_back(oy+sy);z.push_back(oz);   f.push_back(x.size()-1);
			x.push_back(ox);y.push_back(oy+sy);z.push_back(oz+sz);f.push_back(x.size()-1);
			x.push_back(ox);y.push_back(oy);   z.push_back(oz+sz);f.push_back(x.size()-1);
			break;
		case 1:
			x.push_back(ox);   y.push_back(oy);z.push_back(oz);   f.push_back(x.size()-1);
			x.push_back(ox+sx);y.push_back(oy);z.push_back(oz);   f.push_back(x.size()-1);
			x.push_back(ox+sx);y.push_back(oy);z.push_back(oz+sz);f.push_back(x.size()-1);
			x.push_back(ox);   y.push_back(oy);z.push_back(oz+sz);f.push_back(x.size()-1);
			break;
		case 2:
			x.push_back(ox);   y.push_back(oy);   z.push_back(oz);f.push_back(x.size()-1);
			x.push_back(ox+sx);y.push_back(oy);   z.push_back(oz);f.push_back(x.size()-1);
			x.push_back(ox+sx);y.push_back(oy+sy);z.push_back(oz);f.push_back(x.size()-1);
			x.push_back(ox);   y.push_back(oy+sy);z.push_back(oz);f.push_back(x.size()-1);
		}
	}
}

void MeshSeparate::addAlignedLineCube(float ox, float oy, float oz, float sx, float sy, float sz, float edgeWidth)
{
		x.push_back(ox);    y.push_back(oy);    z.push_back(oz);
		x.push_back(ox+sx); y.push_back(oy);    z.push_back(oz);
		x.push_back(ox);    y.push_back(oy+sy); z.push_back(oz);
		x.push_back(ox+sx); y.push_back(oy+sy); z.push_back(oz);
		x.push_back(ox);    y.push_back(oy);    z.push_back(oz+sz);
		x.push_back(ox+sx); y.push_back(oy);    z.push_back(oz+sz);
		x.push_back(ox);    y.push_back(oy+sy); z.push_back(oz+sz);
		x.push_back(ox+sx); y.push_back(oy+sy); z.push_back(oz+sz);

		e.push_back(x.size()-8); e.push_back(x.size()-7); ew.push_back(edgeWidth);
		e.push_back(x.size()-6); e.push_back(x.size()-5); ew.push_back(edgeWidth);
		e.push_back(x.size()-4); e.push_back(x.size()-3); ew.push_back(edgeWidth);
		e.push_back(x.size()-2); e.push_back(x.size()-1); ew.push_back(edgeWidth);

		e.push_back(x.size()-8); e.push_back(x.size()-6); ew.push_back(edgeWidth);
		e.push_back(x.size()-7); e.push_back(x.size()-5); ew.push_back(edgeWidth);
		e.push_back(x.size()-4); e.push_back(x.size()-2); ew.push_back(edgeWidth);
		e.push_back(x.size()-3); e.push_back(x.size()-1); ew.push_back(edgeWidth);

		e.push_back(x.size()-8); e.push_back(x.size()-4); ew.push_back(edgeWidth);
		e.push_back(x.size()-7); e.push_back(x.size()-3); ew.push_back(edgeWidth);
		e.push_back(x.size()-6); e.push_back(x.size()-2); ew.push_back(edgeWidth);
		e.push_back(x.size()-5); e.push_back(x.size()-1); ew.push_back(edgeWidth);
}

cv::Mat &MeshSeparate::addTexture(const cv::Mat &texture){
	materialIndices.push_back(materialIndices.size()>0 ? f.size() : 0);
	textures.push_back(texture.clone());
	return textures.back();
}

cv::Mat &MeshSeparate::getLastTexture()
{
	return textures.back();
}







//int MarchingCubes::getCubeIndex(float d000, float d001, float d010, float d011,
//		float d100, float d101, float d110, float d111,
//		weighttype w000, weighttype w001, weighttype w010, weighttype w011,
//		weighttype w100, weighttype w101, weighttype w110, weighttype w111,
//		float isolevel)
////int MarchingCubes::getCubeIndex(const float &d000, const float &d001, const float &d010, const float &d011,
////		const float &d100, const float &d101, const float &d110, const float &d111,
////		const weighttype &w000, const weighttype &w001, const weighttype &w010, const weighttype &w011,
////		const weighttype &w100, const weighttype &w101, const weighttype &w110, const weighttype &w111,
////		float isolevel)
//{
//
//	if(w000==0 || w001==0 || w010==0 || w011==0 ||
//		 w100==0 || w101==0 || w110==0 || w111==0  )
//		return 0;
//	int cubeindex = 0;
//	if (d000 < isolevel) cubeindex |= 1;
//	if (d001 < isolevel) cubeindex |= 2;
//	if (d010 < isolevel) cubeindex |= 4;
//	if (d011 < isolevel) cubeindex |= 8;
//	if (d100 < isolevel) cubeindex |= 16;
//	if (d101 < isolevel) cubeindex |= 32;
//	if (d110 < isolevel) cubeindex |= 64;
//	if (d111 < isolevel) cubeindex |= 128;
// return cubeindex;
//
////	return
////	(d000 < isolevel) |
////	(d001 < isolevel)<<1 |
////	(d010 < isolevel)<<2 |
////	(d011 < isolevel)<<3 |
////	(d100 < isolevel)<<4 |
////	(d101 < isolevel)<<5 |
////	(d110 < isolevel)<<6 |
////	(d111 < isolevel)<<7 ;
//}

int MarchingCubes::getCubeIndex(float d000, float d001, float d010, float d011,
		float d100, float d101, float d110, float d111,
		float isolevel)
{

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
}

Vertex3f MarchingCubes::VertexInterp(Vertex3f p1,Vertex3f p2,double valp1,double valp2, double isolevel)
{
   double mu;
   Vertex3f p;
   if (fabs(isolevel-valp1) < DISTZEROEPSILON){
//  	 fprintf(stderr,"\nValue 1 too close to Isolevel");
     return(p1);
   }
   if (fabs(isolevel-valp2) < DISTZEROEPSILON){
//  	 fprintf(stderr,"\nValue 2 too close to Isolevel");
      return(p2);
   }
   if (fabs(valp1-valp2) < DISTZEROEPSILON){
//  	 fprintf(stderr,"\nDifference too small");
      return(p1);
   }
   mu = (isolevel - valp1) / (valp2 - valp1);

   //TODO: Rausnehmen:
//   if(mu<=0.0 || mu>=1.0) fprintf(stderr,"\nERROR: Mu = %f",mu);
//   p.x = p1.x + mu * (p2.x - p1.x);
//   p.y = p1.y + mu * (p2.y - p1.y);
//   p.z = p1.z + mu * (p2.z - p1.z);

//   p.x = (double)p1.x + mu * ((double)p2.x - (double)p1.x);
//   p.y = (double)p1.y + mu * ((double)p2.y - (double)p1.y);
//   p.z = (double)p1.z + mu * ((double)p2.z - (double)p1.z);

   double dx = (double)p2.x - (double)p1.x;
   double dy = (double)p2.y - (double)p1.y;
   double dz = (double)p2.z - (double)p1.z;
   p.x = (double)p1.x + mu * dx;
   p.y = (double)p1.y + mu * dy;
   p.z = (double)p1.z + mu * dz;

//  p.x = (1.0-mu)*(double)p1.x + mu*(double)p2.x;
//  p.y = (1.0-mu)*(double)p1.y + mu*(double)p2.y;
//  p.z = (1.0-mu)*(double)p1.z + mu*(double)p2.z;

//   if(mu==0.0 || mu==1.0) fprintf(stderr,"\nMu is %f",mu);

//   if(p.x==p1.x && p.y==p1.y && p.z==p1.z)
//  	 fprintf(stderr,"\nOutput identical to first,"
//  			 " (%f %f %f)  mu == %f, d = (%.10f %.10f %.10f)->(%.10f %.10f %.10f)",p.x,p.y,p.z,mu,dx,dy,dz,mu*dx,mu*dy,mu*dz);
//   if(p.x==p2.x && p.y==p2.y && p.z==p2.z)
//  	 fprintf(stderr,"\nOutput identical to second,"
//  			 " (%f %f %f)  mu == %f, d = (%.10f %.10f %.10f)->(%.10f %.10f %.10f)",p.x,p.y,p.z,mu,dx,dy,dz,mu*dx,mu*dy,mu*dz);

   return(p);
}

VertexColor MarchingCubes::VertexInterp(VertexColor p1,VertexColor p2,float valp1,float valp2, float isolevel)
{
   float mu;
   VertexColor p;
   if (fabs(isolevel-valp1) < DISTZEROEPSILON){
     return(p1);
   }
   if (fabs(isolevel-valp2) < DISTZEROEPSILON){
      return(p2);
   }
   if (fabs(valp1-valp2) < DISTZEROEPSILON){
      return(p1);
   }
   mu = (isolevel - valp1) / (valp2 - valp1);
   float px = ((float)p1.x + mu * ((float)p2.x - (float)p1.x));
   float py = ((float)p1.y + mu * ((float)p2.y - (float)p1.y));
   float pz = ((float)p1.z + mu * ((float)p2.z - (float)p1.z));
   p.x = px<=255.0f*COLOR_MULTIPLICATOR ? (px>=0.0f ? (colortype)px : 0) : 255*COLOR_MULTIPLICATOR;
   p.y = py<=255.0f*COLOR_MULTIPLICATOR ? (py>=0.0f ? (colortype)py : 0) : 255*COLOR_MULTIPLICATOR;
   p.z = pz<=255.0f*COLOR_MULTIPLICATOR ? (pz>=0.0f ? (colortype)pz : 0) : 255*COLOR_MULTIPLICATOR;

   return(p);
}


MarchingCubes::MarchingCubes()
{
	edgeTable = new int[256];
	triTable = new int*[256]; for(unsigned int i=0;i<256;i++) triTable[i] = new int[16];
	numberOfFaces = new int[256];

	int edgeTableTemp[256]={
	0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
	0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
	0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
	0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
	0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
	0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
	0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
	0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
	0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
	0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
	0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
	0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
	0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
	0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
	0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
	0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
	0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
	0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
	0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
	0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
	0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
	0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
	0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
	0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
	0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
	0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
	0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
	0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
	0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
	0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
	0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
	0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0   };

	int triTableTemp[256][16] =
	{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
	{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
	{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
	{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
	{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
	{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
	{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
	{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
	{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
	{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
	{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
	{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
	{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
	{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
	{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
	{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
	{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
	{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
	{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
	{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
	{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
	{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
	{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
	{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
	{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
	{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
	{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
	{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
	{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
	{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
	{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
	{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
	{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
	{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
	{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
	{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
	{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
	{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
	{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
	{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
	{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
	{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
	{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
	{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
	{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
	{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
	{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
	{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
	{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
	{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
	{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
	{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
	{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
	{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
	{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
	{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
	{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
	{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
	{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
	{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
	{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
	{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
	{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
	{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
	{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
	{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
	{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
	{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
	{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
	{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
	{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
	{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
	{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
	{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
	{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
	{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
	{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
	{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
	{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
	{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
	{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
	{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
	{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
	{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
	{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
	{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
	{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
	{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
	{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
	{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
	{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
	{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
	{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
	{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
	{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
	{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
	{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
	{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
	{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
	{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
	{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
	{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
	{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
	{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
	{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
	{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
	{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
	{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
	{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
	{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
	{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
	{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
	{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
	{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
	{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
	{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
	{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
	{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
	{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
	{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
	{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
	{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
	{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
	{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
	{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
	{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
	{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
	{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};

	for(unsigned int i=0;i<256;i++) edgeTable[i] = edgeTableTemp[i];
	for(unsigned int i=0;i<256;i++){
		numberOfFaces[i] = 0;
		for(unsigned int j=0;j<16;j++){
			triTable[i][j] = triTableTemp[i][j];
			if(triTable[i][j] >= 0) numberOfFaces[i]++;
		}
	}
}

MarchingCubes::~MarchingCubes()
{
	delete [] edgeTable;
	for(unsigned int i=0;i<256;i++) delete [] triTable[i];
	delete [] triTable;
	delete [] numberOfFaces;
}

int *MarchingCubes::getVertexList(Vertex3f *vertlist,
		sidetype x, sidetype y, sidetype z,
		float ox, float oy, float oz, float scale,
		float val0,float val1, float val2, float val3,
		float val4, float val5, float val6, float val7,
		uchar *cubeindex_return)
{
	uchar cubeindex = getCubeIndex(val0,val1,val2,val3,val4,val5,val6,val7);
	if(cubeindex_return) *cubeindex_return = cubeindex;
	Vertex3f p0(ox+x*scale,    oy+y*scale,    oz+z*scale);
	Vertex3f p1(ox+(x+1)*scale,oy+y*scale,    oz+z*scale);
	Vertex3f p2(ox+(x+1)*scale,oy+(y+1)*scale,oz+z*scale);
	Vertex3f p3(ox+x*scale,    oy+(y+1)*scale,oz+z*scale);
	Vertex3f p4(ox+x*scale,    oy+y*scale,    oz+(z+1)*scale);
	Vertex3f p5(ox+(x+1)*scale,oy+y*scale,    oz+(z+1)*scale);
	Vertex3f p6(ox+(x+1)*scale,oy+(y+1)*scale,oz+(z+1)*scale);
	Vertex3f p7(ox+x*scale,    oy+(y+1)*scale,oz+(z+1)*scale);

	/* Find the vertices where the surface intersects the cube */
	   if (edgeTable[cubeindex] & 1)
	      vertlist[0] = VertexInterp(p0,p1,val0,val1);
	   if (edgeTable[cubeindex] & 2)
	      vertlist[1] = VertexInterp(p1,p2,val1,val2);
	   if (edgeTable[cubeindex] & 4)
	      vertlist[2] = VertexInterp(p2,p3,val2,val3);
	   if (edgeTable[cubeindex] & 8)
	      vertlist[3] = VertexInterp(p3,p0,val3,val0);
	   if (edgeTable[cubeindex] & 16)
	      vertlist[4] = VertexInterp(p4,p5,val4,val5);
	   if (edgeTable[cubeindex] & 32)
	      vertlist[5] = VertexInterp(p5,p6,val5,val6);
	   if (edgeTable[cubeindex] & 64)
	      vertlist[6] = VertexInterp(p6,p7,val6,val7);
	   if (edgeTable[cubeindex] & 128)
	      vertlist[7] = VertexInterp(p7,p4,val7,val4);
	   if (edgeTable[cubeindex] & 256)
	      vertlist[8] = VertexInterp(p0,p4,val0,val4);
	   if (edgeTable[cubeindex] & 512)
	      vertlist[9] = VertexInterp(p1,p5,val1,val5);
	   if (edgeTable[cubeindex] & 1024)
	      vertlist[10] = VertexInterp(p2,p6,val2,val6);
	   if (edgeTable[cubeindex] & 2048)
	      vertlist[11] = VertexInterp(p3,p7,val3,val7);
	   return triTable[cubeindex];
}

int *MarchingCubes::getVertexColors(VertexColor *collist,
		uchar cubeindex,
		float val0,float val1, float val2, float val3,
		float val4, float val5, float val6, float val7,
		VertexColor c0, VertexColor c1, VertexColor c2, VertexColor c3,
		VertexColor c4, VertexColor c5, VertexColor c6, VertexColor c7)
{

	/* Find the vertices where the surface intersects the cube */
	   if (edgeTable[cubeindex] & 1)
	  	 collist[0] = VertexInterp(c0,c1,val0,val1);
	   if (edgeTable[cubeindex] & 2)
	  	 collist[1] = VertexInterp(c1,c2,val1,val2);
	   if (edgeTable[cubeindex] & 4)
	  	 collist[2] = VertexInterp(c2,c3,val2,val3);
	   if (edgeTable[cubeindex] & 8)
	  	 collist[3] = VertexInterp(c3,c0,val3,val0);
	   if (edgeTable[cubeindex] & 16)
	  	 collist[4] = VertexInterp(c4,c5,val4,val5);
	   if (edgeTable[cubeindex] & 32)
	  	 collist[5] = VertexInterp(c5,c6,val5,val6);
	   if (edgeTable[cubeindex] & 64)
	  	 collist[6] = VertexInterp(c6,c7,val6,val7);
	   if (edgeTable[cubeindex] & 128)
	  	 collist[7] = VertexInterp(c7,c4,val7,val4);
	   if (edgeTable[cubeindex] & 256)
	  	 collist[8] = VertexInterp(c0,c4,val0,val4);
	   if (edgeTable[cubeindex] & 512)
	  	 collist[9] = VertexInterp(c1,c5,val1,val5);
	   if (edgeTable[cubeindex] & 1024)
	  	 collist[10] = VertexInterp(c2,c6,val2,val6);
	   if (edgeTable[cubeindex] & 2048)
	  	 collist[11] = VertexInterp(c3,c7,val3,val7);
	   return triTable[cubeindex];
}



int *MarchingCubes::getColoredTrianglesForCube(
		Vertex3f *vertlist,VertexColor *collist,
		sidetype x, sidetype y, sidetype z,
		float ox, float oy, float oz, float scale,
		volumetype idx0, volumetype idx1, volumetype idx2, volumetype idx3,
		volumetype idx4, volumetype idx5, volumetype idx6, volumetype idx7,
		float *values, weighttype *weights, colortype *r, colortype *g, colortype *b)
{
	float val0 = values[idx0]; float val1 = values[idx1];
	float val2 = values[idx2]; float val3 = values[idx3];
	float val4 = values[idx4]; float val5 = values[idx5];
	float val6 = values[idx6]; float val7 = values[idx7];

	weighttype w0 = weights[idx0]; weighttype w1 = weights[idx1];
	weighttype w2 = weights[idx2]; weighttype w3 = weights[idx3];
	weighttype w4 = weights[idx4]; weighttype w5 = weights[idx5];
	weighttype w6 = weights[idx6]; weighttype w7 = weights[idx7];


	uchar cubeindex = getCubeIndex(val0,val1,val2,val3,val4,val5,val6,val7,
			w0,w1,w2,w3,w4,w5,w6,w7);

	Vertex3f p0(ox+x*scale,    oy+y*scale,    oz+z*scale);
	Vertex3f p1(ox+(x+1)*scale,oy+y*scale,    oz+z*scale);
	Vertex3f p2(ox+(x+1)*scale,oy+(y+1)*scale,oz+z*scale);
	Vertex3f p3(ox+x*scale,    oy+(y+1)*scale,oz+z*scale);
	Vertex3f p4(ox+x*scale,    oy+y*scale,    oz+(z+1)*scale);
	Vertex3f p5(ox+(x+1)*scale,oy+y*scale,    oz+(z+1)*scale);
	Vertex3f p6(ox+(x+1)*scale,oy+(y+1)*scale,oz+(z+1)*scale);
	Vertex3f p7(ox+x*scale,    oy+(y+1)*scale,oz+(z+1)*scale);

	VertexColor c0, c1,c2,c3,c4,c5,c6,c7;
	bool color = r && g && b;
	if(color){
		c0 = VertexColor(r[idx0],g[idx0],b[idx0]); c1 = VertexColor(r[idx1],g[idx1],b[idx1]);
		c2 = VertexColor(r[idx2],g[idx2],b[idx2]); c3 = VertexColor(r[idx3],g[idx3],b[idx3]);
		c4 = VertexColor(r[idx4],g[idx4],b[idx4]); c5 = VertexColor(r[idx5],g[idx5],b[idx5]);
		c6 = VertexColor(r[idx6],g[idx6],b[idx6]); c7 = VertexColor(r[idx7],g[idx7],b[idx7]);
	}


	/* Find the vertices where the surface intersects the cube */
	if (edgeTable[cubeindex] & 1){
		vertlist[0] = VertexInterp(p0,p1,val0,val1);
		if(color) collist[0] = VertexInterp(c0,c1,val0,val1);
	}
	if (edgeTable[cubeindex] & 2){
		vertlist[1] = VertexInterp(p1,p2,val1,val2);
		if(color) collist[1] = VertexInterp(c1,c2,val1,val2);
	}
	if (edgeTable[cubeindex] & 4){
		vertlist[2] = VertexInterp(p2,p3,val2,val3);
		if(color) collist[2] = VertexInterp(c2,c3,val2,val3);
	}
	if (edgeTable[cubeindex] & 8){
		vertlist[3] = VertexInterp(p3,p0,val3,val0);
		if(color) collist[3] = VertexInterp(c3,c0,val3,val0);
	}
	if (edgeTable[cubeindex] & 16){
		vertlist[4] = VertexInterp(p4,p5,val4,val5);
		if(color) collist[4] = VertexInterp(c4,c5,val4,val5);
	}
	if (edgeTable[cubeindex] & 32){
		vertlist[5] = VertexInterp(p5,p6,val5,val6);
		if(color) collist[5] = VertexInterp(c5,c6,val5,val6);
	}
	if (edgeTable[cubeindex] & 64){
		vertlist[6] = VertexInterp(p6,p7,val6,val7);
		if(color) collist[6] = VertexInterp(c6,c7,val6,val7);
	}
	if (edgeTable[cubeindex] & 128){
		vertlist[7] = VertexInterp(p7,p4,val7,val4);
		if(color) collist[7] = VertexInterp(c7,c4,val7,val4);
	}
	if (edgeTable[cubeindex] & 256){
		vertlist[8] = VertexInterp(p0,p4,val0,val4);
		if(color) collist[8] = VertexInterp(c0,c4,val0,val4);
	}
	if (edgeTable[cubeindex] & 512){
		vertlist[9] = VertexInterp(p1,p5,val1,val5);
		if(color) collist[9] = VertexInterp(c1,c5,val1,val5);
	}
	if (edgeTable[cubeindex] & 1024){
		vertlist[10] = VertexInterp(p2,p6,val2,val6);
		if(color) collist[10] = VertexInterp(c2,c6,val2,val6);
	}
	if (edgeTable[cubeindex] & 2048){
		vertlist[11] = VertexInterp(p3,p7,val3,val7);
		if(color) collist[11] = VertexInterp(c3,c7,val3,val7);
	}
	return triTable[cubeindex];
}

int *MarchingCubes::getInterleavedColoredTrianglesForCube(
		Vertex3f *vertlist,VertexColor *collist,
		sidetype x, sidetype y, sidetype z,
		float ox, float oy, float oz, float scale,
		volumetype idx0, volumetype idx1, volumetype idx2, volumetype idx3,
		volumetype idx4, volumetype idx5, volumetype idx6, volumetype idx7,
		const float *values, const weighttype *weights, const colortype3 *color)
{
	float val0 = values[idx0]; float val1 = values[idx1];
	float val2 = values[idx2]; float val3 = values[idx3];
	float val4 = values[idx4]; float val5 = values[idx5];
	float val6 = values[idx6]; float val7 = values[idx7];

	weighttype w0 = weights[idx0]; weighttype w1 = weights[idx1];
	weighttype w2 = weights[idx2]; weighttype w3 = weights[idx3];
	weighttype w4 = weights[idx4]; weighttype w5 = weights[idx5];
	weighttype w6 = weights[idx6]; weighttype w7 = weights[idx7];


	uchar cubeindex = getCubeIndex(val0,val1,val2,val3,val4,val5,val6,val7,
			w0,w1,w2,w3,w4,w5,w6,w7);

	Vertex3f p0(ox+x*scale,    oy+y*scale,    oz+z*scale);
	Vertex3f p1(ox+(x+1)*scale,oy+y*scale,    oz+z*scale);
	Vertex3f p2(ox+(x+1)*scale,oy+(y+1)*scale,oz+z*scale);
	Vertex3f p3(ox+x*scale,    oy+(y+1)*scale,oz+z*scale);
	Vertex3f p4(ox+x*scale,    oy+y*scale,    oz+(z+1)*scale);
	Vertex3f p5(ox+(x+1)*scale,oy+y*scale,    oz+(z+1)*scale);
	Vertex3f p6(ox+(x+1)*scale,oy+(y+1)*scale,oz+(z+1)*scale);
	Vertex3f p7(ox+x*scale,    oy+(y+1)*scale,oz+(z+1)*scale);

	VertexColor c0, c1,c2,c3,c4,c5,c6,c7;
	if(color){
		c0 = VertexColor(color[idx0].x,color[idx0].y,color[idx0].z);
		c1 = VertexColor(color[idx1].x,color[idx1].y,color[idx1].z);
		c2 = VertexColor(color[idx2].x,color[idx2].y,color[idx2].z);
		c3 = VertexColor(color[idx3].x,color[idx3].y,color[idx3].z);
		c4 = VertexColor(color[idx4].x,color[idx4].y,color[idx4].z);
		c5 = VertexColor(color[idx5].x,color[idx5].y,color[idx5].z);
		c6 = VertexColor(color[idx6].x,color[idx6].y,color[idx6].z);
		c7 = VertexColor(color[idx7].x,color[idx7].y,color[idx7].z);
	}


	/* Find the vertices where the surface intersects the cube */
	if (edgeTable[cubeindex] & 1){
		vertlist[0] = VertexInterp(p0,p1,val0,val1);
		if(color) collist[0] = VertexInterp(c0,c1,val0,val1);
	}
	if (edgeTable[cubeindex] & 2){
		vertlist[1] = VertexInterp(p1,p2,val1,val2);
		if(color) collist[1] = VertexInterp(c1,c2,val1,val2);
	}
	if (edgeTable[cubeindex] & 4){
		vertlist[2] = VertexInterp(p2,p3,val2,val3);
		if(color) collist[2] = VertexInterp(c2,c3,val2,val3);
	}
	if (edgeTable[cubeindex] & 8){
		vertlist[3] = VertexInterp(p3,p0,val3,val0);
		if(color) collist[3] = VertexInterp(c3,c0,val3,val0);
	}
	if (edgeTable[cubeindex] & 16){
		vertlist[4] = VertexInterp(p4,p5,val4,val5);
		if(color) collist[4] = VertexInterp(c4,c5,val4,val5);
	}
	if (edgeTable[cubeindex] & 32){
		vertlist[5] = VertexInterp(p5,p6,val5,val6);
		if(color) collist[5] = VertexInterp(c5,c6,val5,val6);
	}
	if (edgeTable[cubeindex] & 64){
		vertlist[6] = VertexInterp(p6,p7,val6,val7);
		if(color) collist[6] = VertexInterp(c6,c7,val6,val7);
	}
	if (edgeTable[cubeindex] & 128){
		vertlist[7] = VertexInterp(p7,p4,val7,val4);
		if(color) collist[7] = VertexInterp(c7,c4,val7,val4);
	}
	if (edgeTable[cubeindex] & 256){
		vertlist[8] = VertexInterp(p0,p4,val0,val4);
		if(color) collist[8] = VertexInterp(c0,c4,val0,val4);
	}
	if (edgeTable[cubeindex] & 512){
		vertlist[9] = VertexInterp(p1,p5,val1,val5);
		if(color) collist[9] = VertexInterp(c1,c5,val1,val5);
	}
	if (edgeTable[cubeindex] & 1024){
		vertlist[10] = VertexInterp(p2,p6,val2,val6);
		if(color) collist[10] = VertexInterp(c2,c6,val2,val6);
	}
	if (edgeTable[cubeindex] & 2048){
		vertlist[11] = VertexInterp(p3,p7,val3,val7);
		if(color) collist[11] = VertexInterp(c3,c7,val3,val7);
	}
	return triTable[cubeindex];
}

int *MarchingCubes::getColoredTrianglesForCubePrecomputed(
		Vertex3f *vertlist,VertexColor *collist,
		sidetype x, sidetype y, sidetype z,
		float ox, float oy, float oz, float sx, float sy, float sz,
		float val0, float val1, float val2, float val3,
		float val4, float val5, float val6, float val7,
		weighttype w0, weighttype w1, weighttype w2, weighttype w3,
		weighttype w4, weighttype w5, weighttype w6, weighttype w7,
		VertexColor c0, VertexColor c1, VertexColor c2, VertexColor c3,
		VertexColor c4, VertexColor c5, VertexColor c6, VertexColor c7,
		const colortype3 *color) const
{


	uchar cubeindex = getCubeIndex(val0,val1,val2,val3,val4,val5,val6,val7,
			w0,w1,w2,w3,w4,w5,w6,w7);

	Vertex3f p0(ox+x*sx,    oy+y*sy,    oz+z*sz);
	Vertex3f p1(ox+(x+1)*sx,oy+y*sy,    oz+z*sz);
	Vertex3f p2(ox+(x+1)*sx,oy+(y+1)*sy,oz+z*sz);
	Vertex3f p3(ox+x*sx,    oy+(y+1)*sy,oz+z*sz);
	Vertex3f p4(ox+x*sx,    oy+y*sy,    oz+(z+1)*sz);
	Vertex3f p5(ox+(x+1)*sx,oy+y*sy,    oz+(z+1)*sz);
	Vertex3f p6(ox+(x+1)*sx,oy+(y+1)*sy,oz+(z+1)*sz);
	Vertex3f p7(ox+x*sx,    oy+(y+1)*sy,oz+(z+1)*sz);


	/* Find the vertices where the surface intersects the cube */
	if (edgeTable[cubeindex] & 1){
		vertlist[0] = VertexInterp(p0,p1,val0,val1);
		if(color) collist[0] = VertexInterp(c0,c1,val0,val1);
	}
	if (edgeTable[cubeindex] & 2){
		vertlist[1] = VertexInterp(p1,p2,val1,val2);
		if(color) collist[1] = VertexInterp(c1,c2,val1,val2);
	}
	if (edgeTable[cubeindex] & 4){
		vertlist[2] = VertexInterp(p2,p3,val2,val3);
		if(color) collist[2] = VertexInterp(c2,c3,val2,val3);
	}
	if (edgeTable[cubeindex] & 8){
		vertlist[3] = VertexInterp(p3,p0,val3,val0);
		if(color) collist[3] = VertexInterp(c3,c0,val3,val0);
	}
	if (edgeTable[cubeindex] & 16){
		vertlist[4] = VertexInterp(p4,p5,val4,val5);
		if(color) collist[4] = VertexInterp(c4,c5,val4,val5);
	}
	if (edgeTable[cubeindex] & 32){
		vertlist[5] = VertexInterp(p5,p6,val5,val6);
		if(color) collist[5] = VertexInterp(c5,c6,val5,val6);
	}
	if (edgeTable[cubeindex] & 64){
		vertlist[6] = VertexInterp(p6,p7,val6,val7);
		if(color) collist[6] = VertexInterp(c6,c7,val6,val7);
	}
	if (edgeTable[cubeindex] & 128){
		vertlist[7] = VertexInterp(p7,p4,val7,val4);
		if(color) collist[7] = VertexInterp(c7,c4,val7,val4);
	}
	if (edgeTable[cubeindex] & 256){
		vertlist[8] = VertexInterp(p0,p4,val0,val4);
		if(color) collist[8] = VertexInterp(c0,c4,val0,val4);
	}
	if (edgeTable[cubeindex] & 512){
		vertlist[9] = VertexInterp(p1,p5,val1,val5);
		if(color) collist[9] = VertexInterp(c1,c5,val1,val5);
	}
	if (edgeTable[cubeindex] & 1024){
		vertlist[10] = VertexInterp(p2,p6,val2,val6);
		if(color) collist[10] = VertexInterp(c2,c6,val2,val6);
	}
	if (edgeTable[cubeindex] & 2048){
		vertlist[11] = VertexInterp(p3,p7,val3,val7);
		if(color) collist[11] = VertexInterp(c3,c7,val3,val7);
	}
	return triTable[cubeindex];
}

MarchingCubesIndexed::MarchingCubesIndexed(sidetype brickLength, sidetype brickHeight)
: MarchingCubes()
{
	offsetTable = new int*[256];
	for(unsigned int i=0;i<256;i++) offsetTable[i] = new int[16];


	int i0 = 0;
	int i1 = 4;
	int i2 = brickLength*3;
	int i3 = 1;
	int i4 = brickLength*brickHeight*3;
	int i5 = i4+i1;
	int i6 = i4+i2;
	int i7 = i4+i3;
	int i8 = 2;
	int i9 = 5;
	int i10 = i9+i2;
	int i11 = i8+i2;

	int offsetTableTemp[256][16] =
	{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i8, i3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i1, i9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i8, i3, i9, i8, i1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i2, i10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i8, i3, i1, i2, i10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i9, i2, i10, i0, i2, i9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i2, i8, i3, i2, i10, i8, i10, i9, i8, -1, -1, -1, -1, -1, -1, -1},
	{i3, i11, i2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i11, i2, i8, i11, i0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i9, i0, i2, i3, i11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i11, i2, i1, i9, i11, i9, i8, i11, -1, -1, -1, -1, -1, -1, -1},
	{i3, i10, i1, i11, i10, i3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i10, i1, i0, i8, i10, i8, i11, i10, -1, -1, -1, -1, -1, -1, -1},
	{i3, i9, i0, i3, i11, i9, i11, i10, i9, -1, -1, -1, -1, -1, -1, -1},
	{i9, i8, i10, i10, i8, i11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i4, i7, i8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i4, i3, i0, i7, i3, i4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i1, i9, i8, i4, i7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i4, i1, i9, i4, i7, i1, i7, i3, i1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i2, i10, i8, i4, i7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i3, i4, i7, i3, i0, i4, i1, i2, i10, -1, -1, -1, -1, -1, -1, -1},
	{i9, i2, i10, i9, i0, i2, i8, i4, i7, -1, -1, -1, -1, -1, -1, -1},
	{i2, i10, i9, i2, i9, i7, i2, i7, i3, i7, i9, i4, -1, -1, -1, -1},
	{i8, i4, i7, i3, i11, i2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i11, i4, i7, i11, i2, i4, i2, i0, i4, -1, -1, -1, -1, -1, -1, -1},
	{i9, i0, i1, i8, i4, i7, i2, i3, i11, -1, -1, -1, -1, -1, -1, -1},
	{i4, i7, i11, i9, i4, i11, i9, i11, i2, i9, i2, i1, -1, -1, -1, -1},
	{i3, i10, i1, i3, i11, i10, i7, i8, i4, -1, -1, -1, -1, -1, -1, -1},
	{i1, i11, i10, i1, i4, i11, i1, i0, i4, i7, i11, i4, -1, -1, -1, -1},
	{i4, i7, i8, i9, i0, i11, i9, i11, i10, i11, i0, i3, -1, -1, -1, -1},
	{i4, i7, i11, i4, i11, i9, i9, i11, i10, -1, -1, -1, -1, -1, -1, -1},
	{i9, i5, i4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i9, i5, i4, i0, i8, i3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i5, i4, i1, i5, i0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i8, i5, i4, i8, i3, i5, i3, i1, i5, -1, -1, -1, -1, -1, -1, -1},
	{i1, i2, i10, i9, i5, i4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i3, i0, i8, i1, i2, i10, i4, i9, i5, -1, -1, -1, -1, -1, -1, -1},
	{i5, i2, i10, i5, i4, i2, i4, i0, i2, -1, -1, -1, -1, -1, -1, -1},
	{i2, i10, i5, i3, i2, i5, i3, i5, i4, i3, i4, i8, -1, -1, -1, -1},
	{i9, i5, i4, i2, i3, i11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i11, i2, i0, i8, i11, i4, i9, i5, -1, -1, -1, -1, -1, -1, -1},
	{i0, i5, i4, i0, i1, i5, i2, i3, i11, -1, -1, -1, -1, -1, -1, -1},
	{i2, i1, i5, i2, i5, i8, i2, i8, i11, i4, i8, i5, -1, -1, -1, -1},
	{i10, i3, i11, i10, i1, i3, i9, i5, i4, -1, -1, -1, -1, -1, -1, -1},
	{i4, i9, i5, i0, i8, i1, i8, i10, i1, i8, i11, i10, -1, -1, -1, -1},
	{i5, i4, i0, i5, i0, i11, i5, i11, i10, i11, i0, i3, -1, -1, -1, -1},
	{i5, i4, i8, i5, i8, i10, i10, i8, i11, -1, -1, -1, -1, -1, -1, -1},
	{i9, i7, i8, i5, i7, i9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i9, i3, i0, i9, i5, i3, i5, i7, i3, -1, -1, -1, -1, -1, -1, -1},
	{i0, i7, i8, i0, i1, i7, i1, i5, i7, -1, -1, -1, -1, -1, -1, -1},
	{i1, i5, i3, i3, i5, i7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i9, i7, i8, i9, i5, i7, i10, i1, i2, -1, -1, -1, -1, -1, -1, -1},
	{i10, i1, i2, i9, i5, i0, i5, i3, i0, i5, i7, i3, -1, -1, -1, -1},
	{i8, i0, i2, i8, i2, i5, i8, i5, i7, i10, i5, i2, -1, -1, -1, -1},
	{i2, i10, i5, i2, i5, i3, i3, i5, i7, -1, -1, -1, -1, -1, -1, -1},
	{i7, i9, i5, i7, i8, i9, i3, i11, i2, -1, -1, -1, -1, -1, -1, -1},
	{i9, i5, i7, i9, i7, i2, i9, i2, i0, i2, i7, i11, -1, -1, -1, -1},
	{i2, i3, i11, i0, i1, i8, i1, i7, i8, i1, i5, i7, -1, -1, -1, -1},
	{i11, i2, i1, i11, i1, i7, i7, i1, i5, -1, -1, -1, -1, -1, -1, -1},
	{i9, i5, i8, i8, i5, i7, i10, i1, i3, i10, i3, i11, -1, -1, -1, -1},
	{i5, i7, i0, i5, i0, i9, i7, i11, i0, i1, i0, i10, i11, i10, i0, -1},
	{i11, i10, i0, i11, i0, i3, i10, i5, i0, i8, i0, i7, i5, i7, i0, -1},
	{i11, i10, i5, i7, i11, i5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i10, i6, i5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i8, i3, i5, i10, i6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i9, i0, i1, i5, i10, i6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i8, i3, i1, i9, i8, i5, i10, i6, -1, -1, -1, -1, -1, -1, -1},
	{i1, i6, i5, i2, i6, i1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i6, i5, i1, i2, i6, i3, i0, i8, -1, -1, -1, -1, -1, -1, -1},
	{i9, i6, i5, i9, i0, i6, i0, i2, i6, -1, -1, -1, -1, -1, -1, -1},
	{i5, i9, i8, i5, i8, i2, i5, i2, i6, i3, i2, i8, -1, -1, -1, -1},
	{i2, i3, i11, i10, i6, i5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i11, i0, i8, i11, i2, i0, i10, i6, i5, -1, -1, -1, -1, -1, -1, -1},
	{i0, i1, i9, i2, i3, i11, i5, i10, i6, -1, -1, -1, -1, -1, -1, -1},
	{i5, i10, i6, i1, i9, i2, i9, i11, i2, i9, i8, i11, -1, -1, -1, -1},
	{i6, i3, i11, i6, i5, i3, i5, i1, i3, -1, -1, -1, -1, -1, -1, -1},
	{i0, i8, i11, i0, i11, i5, i0, i5, i1, i5, i11, i6, -1, -1, -1, -1},
	{i3, i11, i6, i0, i3, i6, i0, i6, i5, i0, i5, i9, -1, -1, -1, -1},
	{i6, i5, i9, i6, i9, i11, i11, i9, i8, -1, -1, -1, -1, -1, -1, -1},
	{i5, i10, i6, i4, i7, i8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i4, i3, i0, i4, i7, i3, i6, i5, i10, -1, -1, -1, -1, -1, -1, -1},
	{i1, i9, i0, i5, i10, i6, i8, i4, i7, -1, -1, -1, -1, -1, -1, -1},
	{i10, i6, i5, i1, i9, i7, i1, i7, i3, i7, i9, i4, -1, -1, -1, -1},
	{i6, i1, i2, i6, i5, i1, i4, i7, i8, -1, -1, -1, -1, -1, -1, -1},
	{i1, i2, i5, i5, i2, i6, i3, i0, i4, i3, i4, i7, -1, -1, -1, -1},
	{i8, i4, i7, i9, i0, i5, i0, i6, i5, i0, i2, i6, -1, -1, -1, -1},
	{i7, i3, i9, i7, i9, i4, i3, i2, i9, i5, i9, i6, i2, i6, i9, -1},
	{i3, i11, i2, i7, i8, i4, i10, i6, i5, -1, -1, -1, -1, -1, -1, -1},
	{i5, i10, i6, i4, i7, i2, i4, i2, i0, i2, i7, i11, -1, -1, -1, -1},
	{i0, i1, i9, i4, i7, i8, i2, i3, i11, i5, i10, i6, -1, -1, -1, -1},
	{i9, i2, i1, i9, i11, i2, i9, i4, i11, i7, i11, i4, i5, i10, i6, -1},
	{i8, i4, i7, i3, i11, i5, i3, i5, i1, i5, i11, i6, -1, -1, -1, -1},
	{i5, i1, i11, i5, i11, i6, i1, i0, i11, i7, i11, i4, i0, i4, i11, -1},
	{i0, i5, i9, i0, i6, i5, i0, i3, i6, i11, i6, i3, i8, i4, i7, -1},
	{i6, i5, i9, i6, i9, i11, i4, i7, i9, i7, i11, i9, -1, -1, -1, -1},
	{i10, i4, i9, i6, i4, i10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i4, i10, i6, i4, i9, i10, i0, i8, i3, -1, -1, -1, -1, -1, -1, -1},
	{i10, i0, i1, i10, i6, i0, i6, i4, i0, -1, -1, -1, -1, -1, -1, -1},
	{i8, i3, i1, i8, i1, i6, i8, i6, i4, i6, i1, i10, -1, -1, -1, -1},
	{i1, i4, i9, i1, i2, i4, i2, i6, i4, -1, -1, -1, -1, -1, -1, -1},
	{i3, i0, i8, i1, i2, i9, i2, i4, i9, i2, i6, i4, -1, -1, -1, -1},
	{i0, i2, i4, i4, i2, i6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i8, i3, i2, i8, i2, i4, i4, i2, i6, -1, -1, -1, -1, -1, -1, -1},
	{i10, i4, i9, i10, i6, i4, i11, i2, i3, -1, -1, -1, -1, -1, -1, -1},
	{i0, i8, i2, i2, i8, i11, i4, i9, i10, i4, i10, i6, -1, -1, -1, -1},
	{i3, i11, i2, i0, i1, i6, i0, i6, i4, i6, i1, i10, -1, -1, -1, -1},
	{i6, i4, i1, i6, i1, i10, i4, i8, i1, i2, i1, i11, i8, i11, i1, -1},
	{i9, i6, i4, i9, i3, i6, i9, i1, i3, i11, i6, i3, -1, -1, -1, -1},
	{i8, i11, i1, i8, i1, i0, i11, i6, i1, i9, i1, i4, i6, i4, i1, -1},
	{i3, i11, i6, i3, i6, i0, i0, i6, i4, -1, -1, -1, -1, -1, -1, -1},
	{i6, i4, i8, i11, i6, i8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i7, i10, i6, i7, i8, i10, i8, i9, i10, -1, -1, -1, -1, -1, -1, -1},
	{i0, i7, i3, i0, i10, i7, i0, i9, i10, i6, i7, i10, -1, -1, -1, -1},
	{i10, i6, i7, i1, i10, i7, i1, i7, i8, i1, i8, i0, -1, -1, -1, -1},
	{i10, i6, i7, i10, i7, i1, i1, i7, i3, -1, -1, -1, -1, -1, -1, -1},
	{i1, i2, i6, i1, i6, i8, i1, i8, i9, i8, i6, i7, -1, -1, -1, -1},
	{i2, i6, i9, i2, i9, i1, i6, i7, i9, i0, i9, i3, i7, i3, i9, -1},
	{i7, i8, i0, i7, i0, i6, i6, i0, i2, -1, -1, -1, -1, -1, -1, -1},
	{i7, i3, i2, i6, i7, i2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i2, i3, i11, i10, i6, i8, i10, i8, i9, i8, i6, i7, -1, -1, -1, -1},
	{i2, i0, i7, i2, i7, i11, i0, i9, i7, i6, i7, i10, i9, i10, i7, -1},
	{i1, i8, i0, i1, i7, i8, i1, i10, i7, i6, i7, i10, i2, i3, i11, -1},
	{i11, i2, i1, i11, i1, i7, i10, i6, i1, i6, i7, i1, -1, -1, -1, -1},
	{i8, i9, i6, i8, i6, i7, i9, i1, i6, i11, i6, i3, i1, i3, i6, -1},
	{i0, i9, i1, i11, i6, i7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i7, i8, i0, i7, i0, i6, i3, i11, i0, i11, i6, i0, -1, -1, -1, -1},
	{i7, i11, i6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i7, i6, i11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i3, i0, i8, i11, i7, i6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i1, i9, i11, i7, i6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i8, i1, i9, i8, i3, i1, i11, i7, i6, -1, -1, -1, -1, -1, -1, -1},
	{i10, i1, i2, i6, i11, i7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i2, i10, i3, i0, i8, i6, i11, i7, -1, -1, -1, -1, -1, -1, -1},
	{i2, i9, i0, i2, i10, i9, i6, i11, i7, -1, -1, -1, -1, -1, -1, -1},
	{i6, i11, i7, i2, i10, i3, i10, i8, i3, i10, i9, i8, -1, -1, -1, -1},
	{i7, i2, i3, i6, i2, i7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i7, i0, i8, i7, i6, i0, i6, i2, i0, -1, -1, -1, -1, -1, -1, -1},
	{i2, i7, i6, i2, i3, i7, i0, i1, i9, -1, -1, -1, -1, -1, -1, -1},
	{i1, i6, i2, i1, i8, i6, i1, i9, i8, i8, i7, i6, -1, -1, -1, -1},
	{i10, i7, i6, i10, i1, i7, i1, i3, i7, -1, -1, -1, -1, -1, -1, -1},
	{i10, i7, i6, i1, i7, i10, i1, i8, i7, i1, i0, i8, -1, -1, -1, -1},
	{i0, i3, i7, i0, i7, i10, i0, i10, i9, i6, i10, i7, -1, -1, -1, -1},
	{i7, i6, i10, i7, i10, i8, i8, i10, i9, -1, -1, -1, -1, -1, -1, -1},
	{i6, i8, i4, i11, i8, i6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i3, i6, i11, i3, i0, i6, i0, i4, i6, -1, -1, -1, -1, -1, -1, -1},
	{i8, i6, i11, i8, i4, i6, i9, i0, i1, -1, -1, -1, -1, -1, -1, -1},
	{i9, i4, i6, i9, i6, i3, i9, i3, i1, i11, i3, i6, -1, -1, -1, -1},
	{i6, i8, i4, i6, i11, i8, i2, i10, i1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i2, i10, i3, i0, i11, i0, i6, i11, i0, i4, i6, -1, -1, -1, -1},
	{i4, i11, i8, i4, i6, i11, i0, i2, i9, i2, i10, i9, -1, -1, -1, -1},
	{i10, i9, i3, i10, i3, i2, i9, i4, i3, i11, i3, i6, i4, i6, i3, -1},
	{i8, i2, i3, i8, i4, i2, i4, i6, i2, -1, -1, -1, -1, -1, -1, -1},
	{i0, i4, i2, i4, i6, i2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i9, i0, i2, i3, i4, i2, i4, i6, i4, i3, i8, -1, -1, -1, -1},
	{i1, i9, i4, i1, i4, i2, i2, i4, i6, -1, -1, -1, -1, -1, -1, -1},
	{i8, i1, i3, i8, i6, i1, i8, i4, i6, i6, i10, i1, -1, -1, -1, -1},
	{i10, i1, i0, i10, i0, i6, i6, i0, i4, -1, -1, -1, -1, -1, -1, -1},
	{i4, i6, i3, i4, i3, i8, i6, i10, i3, i0, i3, i9, i10, i9, i3, -1},
	{i10, i9, i4, i6, i10, i4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i4, i9, i5, i7, i6, i11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i8, i3, i4, i9, i5, i11, i7, i6, -1, -1, -1, -1, -1, -1, -1},
	{i5, i0, i1, i5, i4, i0, i7, i6, i11, -1, -1, -1, -1, -1, -1, -1},
	{i11, i7, i6, i8, i3, i4, i3, i5, i4, i3, i1, i5, -1, -1, -1, -1},
	{i9, i5, i4, i10, i1, i2, i7, i6, i11, -1, -1, -1, -1, -1, -1, -1},
	{i6, i11, i7, i1, i2, i10, i0, i8, i3, i4, i9, i5, -1, -1, -1, -1},
	{i7, i6, i11, i5, i4, i10, i4, i2, i10, i4, i0, i2, -1, -1, -1, -1},
	{i3, i4, i8, i3, i5, i4, i3, i2, i5, i10, i5, i2, i11, i7, i6, -1},
	{i7, i2, i3, i7, i6, i2, i5, i4, i9, -1, -1, -1, -1, -1, -1, -1},
	{i9, i5, i4, i0, i8, i6, i0, i6, i2, i6, i8, i7, -1, -1, -1, -1},
	{i3, i6, i2, i3, i7, i6, i1, i5, i0, i5, i4, i0, -1, -1, -1, -1},
	{i6, i2, i8, i6, i8, i7, i2, i1, i8, i4, i8, i5, i1, i5, i8, -1},
	{i9, i5, i4, i10, i1, i6, i1, i7, i6, i1, i3, i7, -1, -1, -1, -1},
	{i1, i6, i10, i1, i7, i6, i1, i0, i7, i8, i7, i0, i9, i5, i4, -1},
	{i4, i0, i10, i4, i10, i5, i0, i3, i10, i6, i10, i7, i3, i7, i10, -1},
	{i7, i6, i10, i7, i10, i8, i5, i4, i10, i4, i8, i10, -1, -1, -1, -1},
	{i6, i9, i5, i6, i11, i9, i11, i8, i9, -1, -1, -1, -1, -1, -1, -1},
	{i3, i6, i11, i0, i6, i3, i0, i5, i6, i0, i9, i5, -1, -1, -1, -1},
	{i0, i11, i8, i0, i5, i11, i0, i1, i5, i5, i6, i11, -1, -1, -1, -1},
	{i6, i11, i3, i6, i3, i5, i5, i3, i1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i2, i10, i9, i5, i11, i9, i11, i8, i11, i5, i6, -1, -1, -1, -1},
	{i0, i11, i3, i0, i6, i11, i0, i9, i6, i5, i6, i9, i1, i2, i10, -1},
	{i11, i8, i5, i11, i5, i6, i8, i0, i5, i10, i5, i2, i0, i2, i5, -1},
	{i6, i11, i3, i6, i3, i5, i2, i10, i3, i10, i5, i3, -1, -1, -1, -1},
	{i5, i8, i9, i5, i2, i8, i5, i6, i2, i3, i8, i2, -1, -1, -1, -1},
	{i9, i5, i6, i9, i6, i0, i0, i6, i2, -1, -1, -1, -1, -1, -1, -1},
	{i1, i5, i8, i1, i8, i0, i5, i6, i8, i3, i8, i2, i6, i2, i8, -1},
	{i1, i5, i6, i2, i1, i6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i3, i6, i1, i6, i10, i3, i8, i6, i5, i6, i9, i8, i9, i6, -1},
	{i10, i1, i0, i10, i0, i6, i9, i5, i0, i5, i6, i0, -1, -1, -1, -1},
	{i0, i3, i8, i5, i6, i10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i10, i5, i6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i11, i5, i10, i7, i5, i11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i11, i5, i10, i11, i7, i5, i8, i3, i0, -1, -1, -1, -1, -1, -1, -1},
	{i5, i11, i7, i5, i10, i11, i1, i9, i0, -1, -1, -1, -1, -1, -1, -1},
	{i10, i7, i5, i10, i11, i7, i9, i8, i1, i8, i3, i1, -1, -1, -1, -1},
	{i11, i1, i2, i11, i7, i1, i7, i5, i1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i8, i3, i1, i2, i7, i1, i7, i5, i7, i2, i11, -1, -1, -1, -1},
	{i9, i7, i5, i9, i2, i7, i9, i0, i2, i2, i11, i7, -1, -1, -1, -1},
	{i7, i5, i2, i7, i2, i11, i5, i9, i2, i3, i2, i8, i9, i8, i2, -1},
	{i2, i5, i10, i2, i3, i5, i3, i7, i5, -1, -1, -1, -1, -1, -1, -1},
	{i8, i2, i0, i8, i5, i2, i8, i7, i5, i10, i2, i5, -1, -1, -1, -1},
	{i9, i0, i1, i5, i10, i3, i5, i3, i7, i3, i10, i2, -1, -1, -1, -1},
	{i9, i8, i2, i9, i2, i1, i8, i7, i2, i10, i2, i5, i7, i5, i2, -1},
	{i1, i3, i5, i3, i7, i5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i8, i7, i0, i7, i1, i1, i7, i5, -1, -1, -1, -1, -1, -1, -1},
	{i9, i0, i3, i9, i3, i5, i5, i3, i7, -1, -1, -1, -1, -1, -1, -1},
	{i9, i8, i7, i5, i9, i7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i5, i8, i4, i5, i10, i8, i10, i11, i8, -1, -1, -1, -1, -1, -1, -1},
	{i5, i0, i4, i5, i11, i0, i5, i10, i11, i11, i3, i0, -1, -1, -1, -1},
	{i0, i1, i9, i8, i4, i10, i8, i10, i11, i10, i4, i5, -1, -1, -1, -1},
	{i10, i11, i4, i10, i4, i5, i11, i3, i4, i9, i4, i1, i3, i1, i4, -1},
	{i2, i5, i1, i2, i8, i5, i2, i11, i8, i4, i5, i8, -1, -1, -1, -1},
	{i0, i4, i11, i0, i11, i3, i4, i5, i11, i2, i11, i1, i5, i1, i11, -1},
	{i0, i2, i5, i0, i5, i9, i2, i11, i5, i4, i5, i8, i11, i8, i5, -1},
	{i9, i4, i5, i2, i11, i3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i2, i5, i10, i3, i5, i2, i3, i4, i5, i3, i8, i4, -1, -1, -1, -1},
	{i5, i10, i2, i5, i2, i4, i4, i2, i0, -1, -1, -1, -1, -1, -1, -1},
	{i3, i10, i2, i3, i5, i10, i3, i8, i5, i4, i5, i8, i0, i1, i9, -1},
	{i5, i10, i2, i5, i2, i4, i1, i9, i2, i9, i4, i2, -1, -1, -1, -1},
	{i8, i4, i5, i8, i5, i3, i3, i5, i1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i4, i5, i1, i0, i5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i8, i4, i5, i8, i5, i3, i9, i0, i5, i0, i3, i5, -1, -1, -1, -1},
	{i9, i4, i5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i4, i11, i7, i4, i9, i11, i9, i10, i11, -1, -1, -1, -1, -1, -1, -1},
	{i0, i8, i3, i4, i9, i7, i9, i11, i7, i9, i10, i11, -1, -1, -1, -1},
	{i1, i10, i11, i1, i11, i4, i1, i4, i0, i7, i4, i11, -1, -1, -1, -1},
	{i3, i1, i4, i3, i4, i8, i1, i10, i4, i7, i4, i11, i10, i11, i4, -1},
	{i4, i11, i7, i9, i11, i4, i9, i2, i11, i9, i1, i2, -1, -1, -1, -1},
	{i9, i7, i4, i9, i11, i7, i9, i1, i11, i2, i11, i1, i0, i8, i3, -1},
	{i11, i7, i4, i11, i4, i2, i2, i4, i0, -1, -1, -1, -1, -1, -1, -1},
	{i11, i7, i4, i11, i4, i2, i8, i3, i4, i3, i2, i4, -1, -1, -1, -1},
	{i2, i9, i10, i2, i7, i9, i2, i3, i7, i7, i4, i9, -1, -1, -1, -1},
	{i9, i10, i7, i9, i7, i4, i10, i2, i7, i8, i7, i0, i2, i0, i7, -1},
	{i3, i7, i10, i3, i10, i2, i7, i4, i10, i1, i10, i0, i4, i0, i10, -1},
	{i1, i10, i2, i8, i7, i4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i4, i9, i1, i4, i1, i7, i7, i1, i3, -1, -1, -1, -1, -1, -1, -1},
	{i4, i9, i1, i4, i1, i7, i0, i8, i1, i8, i7, i1, -1, -1, -1, -1},
	{i4, i0, i3, i7, i4, i3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i4, i8, i7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i9, i10, i8, i10, i11, i8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i3, i0, i9, i3, i9, i11, i11, i9, i10, -1, -1, -1, -1, -1, -1, -1},
	{i0, i1, i10, i0, i10, i8, i8, i10, i11, -1, -1, -1, -1, -1, -1, -1},
	{i3, i1, i10, i11, i3, i10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i2, i11, i1, i11, i9, i9, i11, i8, -1, -1, -1, -1, -1, -1, -1},
	{i3, i0, i9, i3, i9, i11, i1, i2, i9, i2, i11, i9, -1, -1, -1, -1},
	{i0, i2, i11, i8, i0, i11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i3, i2, i11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i2, i3, i8, i2, i8, i10, i10, i8, i9, -1, -1, -1, -1, -1, -1, -1},
	{i9, i10, i2, i0, i9, i2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i2, i3, i8, i2, i8, i10, i0, i1, i8, i1, i10, i8, -1, -1, -1, -1},
	{i1, i10, i2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i1, i3, i8, i9, i1, i8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i9, i1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{i0, i3, i8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
	{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};

	for(unsigned int i=0;i<256;i++)
		for(unsigned int j=0;j<16;j++)
			offsetTable[i][j] = offsetTableTemp[i][j];

}

MarchingCubesIndexed::~MarchingCubesIndexed()
{
	for(unsigned int i=0;i<256;i++) delete [] offsetTable[i];
	delete [] offsetTable;
}











