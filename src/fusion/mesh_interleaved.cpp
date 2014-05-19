/*
 * mesh_interleaved.cpp
 *
 *  Created on: Sep 18, 2013
 *      Author: steinbrf
 */

#include "mesh_interleaved.hpp"
#include <fstream>

Vertex2f::Vertex2f(float x_p, float y_p):x(x_p),y(y_p){}
//Vertex3f::Vertex3f(float x_p, float y_p, float z_p):x(x_p),y(y_p),z(z_p){}
Color3b::Color3b(uchar r_p, uchar g_p, uchar b_p):r(r_p),g(g_p),b(b_p){}


MeshInterleaved::MeshInterleaved(unsigned int verticesPerFace)
:_verticesPerFace(verticesPerFace){}

bool MeshInterleaved::writeOBJ(std::string filename)
{
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
			&& texcoords.size() == faces.size();
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
	for(unsigned int i=0;i<vertices.size();i++){
		file << "\nv " << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z;
		if(colors.size()){
			file << " " << (float)(colors[i].r)/255.0f << " " << (float)(colors[i].g)/255.0f << " " << (float)(colors[i].b)/255.0f;
		}
	}

	//Normals
	for(unsigned int i=0;i<normals.size();i++){
		file << "\nvn " << normals[i].x << " " << normals[i].y << " " << normals[i].z;
	}


	//Texture
	if(useTexture){
		for(unsigned int t=0;t<texcoords.size();t++){
			file << "\nvt " << texcoords[t].x << " " << texcoords[t].y;
		}
//		file << "\nusemtl material0";
	}

	if(normals.size()){
		//Normals are given
		for(unsigned int i=0;i<faces.size();i+=_verticesPerFace){
			file << "\nf";
			for(unsigned int j=0;j<_verticesPerFace;j++)
				file << " " << faces[i+j]+1 << "//" << faces[i+j];
		}
	}
	else{
		//No normals
		if(useTexture){
			unsigned int currentMaterialNumber = 0;
			unsigned int nextMaterialIndex = 0;
			//No Normals, Texture Coordinates
			for(unsigned int i=0;i<faces.size();i+=_verticesPerFace){
				if(nextMaterialIndex==i){
					file << "\nusemtl " << materialNames[currentMaterialNumber++];
					nextMaterialIndex =
							materialIndices.size()>currentMaterialNumber ?
									materialIndices[currentMaterialNumber] : faces.size();
				}
				file << "\nf";
				for(unsigned int j=0;j<_verticesPerFace;j++)
					file << " " << faces[i+j]+1 << "/" << (i+j+1);
			}
		}
		else{
			//No Texture, No Normals
			for(unsigned int i=0;i<faces.size();i+=_verticesPerFace){
				file << "\nf";
				for(unsigned int j=0;j<_verticesPerFace;j++)
					file << " " << faces[i+j]+1;
			}
		}
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


bool MeshInterleaved::writePLY(std::string filename, bool binary){
	std::fstream file;
	file.open((filename+".ply").c_str(),std::ios::out|std::ios::binary);
	if(!file.is_open()){
		fprintf(stderr,"\nERROR: Could not open File %s for writing!",(filename+".ply").c_str());
		return false;
	}

	file << "ply";
	if(binary)file << "\nformat binary_little_endian 1.0";
	else file << "\nformat ascii 1.0";
	file << "\nelement vertex " << vertices.size();
	file << "\nproperty float32 x\nproperty float32 y\nproperty float32 z";
	if(colors.size())
		file << "\nproperty uchar red\nproperty uchar green\nproperty uchar blue";
	if(faces.size()){
		file << "\nelement face " << faces.size()/_verticesPerFace;
		file << "\nproperty list uint8 int32 vertex_indices";
	}
	if(edges.size()){
		file << "\nElement edge " << edges.size()/2;
		file << "\nproperty int vertex1\nproperty int vertex2";
	}
	file << "\nend_header";
	if(binary) file << "\n";

	for(unsigned int i=0;i<vertices.size();i++){
		if(binary){
			file.write((char*)(&(vertices[i])),sizeof(Vertex3f));
		}
		else file << "\n" << vertices[i].x << " " << vertices[i].y << " " << vertices[i].z;
		if(colors.size()){
			if(binary){
				file.write((char*)(&(colors[i])),sizeof(Color3b));
			}
			else file << " " << (int)(colors[i].r) << " " << (int)(colors[i].g) << " " << (int)(colors[i].b);
		}
	}
	for(unsigned int i=0;i<faces.size();i+=_verticesPerFace){
		if(binary){
			file.write((char*)(&_verticesPerFace),sizeof(uchar));
		}
		else file << "\n" << (int)_verticesPerFace;
		for(unsigned int j=0;j<_verticesPerFace;j++)
			if(binary){
				unsigned int idx = faces[i+j];
				file.write((char*)(&idx),sizeof(unsigned int));
			}
			else file << " " << (faces[i+j]);
	}
	for(unsigned int i=0;i<edges.size();i+=2){
		if(binary){
			unsigned int idx = edges[i];
			file.write((char*)(&idx),sizeof(unsigned int));
			idx = edges[i+1]; file.write((char*)(&idx),sizeof(unsigned int));
		}
		else file << "\n " << edges[i] << " " << edges[i+1];
	}

	file.close();
	return true;
}


MeshInterleaved &MeshInterleaved::operator+=(const MeshInterleaved &mesh)
{

	unsigned int oldVertexSize = vertices.size();
	unsigned int oldFaceSize = faces.size();

	for(unsigned int i=0;i<mesh.vertices.size();i++){
		vertices.push_back(mesh.vertices[i]);
	}
	if(mesh.colors.size()){
		for(unsigned int i=0;i<mesh.colors.size();i++){
			colors.push_back(mesh.colors[i]);
		}
	}
	if(mesh.faces.size()){
		for(unsigned int i=0;i<mesh.faces.size();i++){
			faces.push_back(mesh.faces[i]+oldVertexSize);
		}
	}
	if(mesh.edges.size()){
		for(unsigned int i=0;i<mesh.edges.size();i++){
			edges.push_back(mesh.edges[i]+oldVertexSize);
		}
	}

	if(mesh.textures.size()){
		for(unsigned int i=0;i<mesh.textures.size();i++){
			materialIndices.push_back(mesh.materialIndices[i]+oldFaceSize);
			textures.push_back(mesh.textures[i].clone());
		}
	}
	if(mesh.texcoords.size()){
		for(unsigned int i=0;i<mesh.texcoords.size();i++){
			texcoords.push_back(mesh.texcoords[i]);
		}
	}

	return *this;
}






