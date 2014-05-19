
#include "treeandbrick_indexed.hpp"


void addBrickEdgeX_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3,
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
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[2]; float df[2]; colortype3 cf[2];


	leafstack stacks[4] = {leaves0,leaves1,leaves2,leaves3};

	sidetype sizeStretch = std::max(std::max(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
                                  leafScale[leaves2[leaves2.size()-1]]);
	sidetype sizeMin =
	std::min(std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
			     std::min(leafScale[leaves2[leaves2.size()-1]],leafScale[leaves3[leaves3.size()-1]]));

	sidetype y[4] = {(sidetype)(oy+size-sizeStretch) , (sidetype)(oy+size),
			(sidetype)(oy+size-sizeStretch) , (sidetype)(oy+size)};
	sidetype z[4] = {(sidetype)(oz+size-sizeStretch) ,  (sidetype)(oz+size-sizeStretch),
	    (sidetype)(oz+size) , (sidetype)(oz+size)};

	for(sidetype x=ox;x<ox+size;x+=sizeMin){
		sidetype bx = (x-ox)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = ((i/2)*bl+(i%2))*bl+bx;
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

				w[idx] = std::max(wf[0] , wf[1]);
				d[idx] = (1.0f-rx)*df[0] + rx*df[1];
				if(color)
				c[idx] = VertexColor(
						(1.0f-rx)*cf[0].x + rx*cf[1].x,
						(1.0f-rx)*cf[0].y + rx*cf[1].y,
						(1.0f-rx)*cf[0].z + rx*cf[1].z);

			}
		}
	}

	for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
		sidetype bx = (x-ox)/sizeMin;
		volumetype idx = bx;
		if(weightInfluence(minWeight,
				w[idx],w[idx+1],
				w[idx+bl+1],w[idx+bl],
				w[idx+bs],w[idx+bs+1],
				w[idx+bs+bl+1],w[idx+bs+bl])){
			int *table = mc.offsetTable[mc.getCubeIndex(
					d[idx],d[idx+1],
					d[idx+bl+1],d[idx+bl],
					d[idx+bs],d[idx+bs+1],
					d[idx+bs+bl+1],d[idx+bs+bl],
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])];

			for (unsigned int i=0;table[i]!=-1;i+=3) {
				faceIsSet[3*idx+table[i  ]] = true;
				faceIsSet[3*idx+table[i+1]] = true;
				faceIsSet[3*idx+table[i+2]] = true;
			}
			tables[idx] = table;
		}
	}


	size_t runningIndex = mesh.x.size();
	for(sidetype x=ox;x<ox+size;x+=sizeMin){
		sidetype bx = (x-ox)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = ((i/2)*bl+(i%2))*bl+bx;
			if(
					x<ox+size-sizeMin &&
					faceIsSet[3*idx+0]
				&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+(x        )*scale,offset.y+y[i]*scale,offset.z+z[i]*scale),
						Vertex3f(offset.x+(x+sizeMin)*scale,offset.y+y[i]*scale,offset.z+z[i]*scale),
						d[idx],d[idx+1]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
				indices[3*idx+0] = runningIndex++;
				vertexIsSet[3*idx+0] = true;
			}
			if(
					faceIsSet[3*idx+1]
					&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x*scale,offset.y+(y[i]            )*scale,offset.z+z[i]*scale),
						Vertex3f(offset.x+x*scale,offset.y+(y[i]+sizeStretch)*scale,offset.z+z[i]*scale),
						d[idx],d[idx+bl]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
				indices[3*idx+1] = runningIndex++;
				vertexIsSet[3*idx+1] = true;
			}
			if(
				 faceIsSet[3*idx+2]
					&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x*scale,offset.y+y[i]*scale,offset.z+(z[i]            )*scale),
						Vertex3f(offset.x+x*scale,offset.y+y[i]*scale,offset.z+(z[i]+sizeStretch)*scale),
						d[idx],d[idx+bs]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
				indices[3*idx+2] = runningIndex++;
				vertexIsSet[3*idx+2] = true;
			}
		}
	}

	for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
		sidetype bx = (x-ox)/sizeMin;
		volumetype idx = bx;
		int *table = tables[idx];
		if(table){
			for (unsigned int i=0;table[i]!=-1;i+=3) {
				mesh.f.push_back(indices[3*idx+table[i  ]]);
				mesh.f.push_back(indices[3*idx+table[i+1]]);
				mesh.f.push_back(indices[3*idx+table[i+2]]);
			}
		}
	}

	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);


	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}

void addBrickEdgeY_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3,
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
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[2]; float df[2]; colortype3 cf[2];

	leafstack stacks[4] = {leaves0,leaves1,leaves2,leaves3};

	sidetype sizeStretch = std::max(std::max(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
                                  leafScale[leaves2[leaves2.size()-1]]);
	sidetype sizeMin =
	std::min(std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
			     std::min(leafScale[leaves2[leaves2.size()-1]],leafScale[leaves3[leaves3.size()-1]]));

	sidetype x[4] = {(sidetype)(ox+size-sizeStretch) , (sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch) , (sidetype)(ox+size)};
	sidetype z[4] = {(sidetype)(oz+size-sizeStretch) , (sidetype)(oz+size-sizeStretch),
	    (sidetype)(oz+size) , (sidetype)(oz+size)};


	for(sidetype y=oy;y<oy+size;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = ((i/2)*bl+by)*bl+(i%2);
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
		}
	}

	for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		volumetype idx = by*bl;
		if(weightInfluence(minWeight,
				w[idx],w[idx+1],
				w[idx+bl+1],w[idx+bl],
				w[idx+bs],w[idx+bs+1],
				w[idx+bs+bl+1],w[idx+bs+bl])){
			int *table = mc.offsetTable[mc.getCubeIndex(
					d[idx],d[idx+1],
					d[idx+bl+1],d[idx+bl],
					d[idx+bs],d[idx+bs+1],
					d[idx+bs+bl+1],d[idx+bs+bl],
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])];

			for (unsigned int i=0;table[i]!=-1;i+=3) {
				faceIsSet[3*idx+table[i  ]] = true;
				faceIsSet[3*idx+table[i+1]] = true;
				faceIsSet[3*idx+table[i+2]] = true;
			}
			tables[idx] = table;
		}
	}


	size_t runningIndex = mesh.x.size();
	for(sidetype y=oy;y<oy+size;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = ((i/2)*bl+by)*bl+(i%2);
			if(
					faceIsSet[3*idx+0]
				&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+(x[i]            )*scale,offset.y+y*scale,offset.z+z[i]*scale),
						Vertex3f(offset.x+(x[i]+sizeStretch)*scale,offset.y+y*scale,offset.z+z[i]*scale),
						d[idx],d[idx+1]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
				indices[3*idx+0] = runningIndex++;
				vertexIsSet[3*idx+0] = true;
			}
			if(
					y<oy+size-sizeMin &&
					faceIsSet[3*idx+1]
					&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x[i]*scale,offset.y+(y        )*scale,offset.z+z[i]*scale),
						Vertex3f(offset.x+x[i]*scale,offset.y+(y+sizeMin)*scale,offset.z+z[i]*scale),
						d[idx],d[idx+bl]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
				indices[3*idx+1] = runningIndex++;
				vertexIsSet[3*idx+1] = true;
			}
			if(
				 faceIsSet[3*idx+2]
					&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x[i]*scale,offset.y+y*scale,offset.z+(z[i]            )*scale),
						Vertex3f(offset.x+x[i]*scale,offset.y+y*scale,offset.z+(z[i]+sizeStretch)*scale),
						d[idx],d[idx+bs]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
				indices[3*idx+2] = runningIndex++;
				vertexIsSet[3*idx+2] = true;
			}
		}
	}



	for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		volumetype idx = by*bl;
		int *table = tables[idx];
		if(table){
			for (unsigned int i=0;table[i]!=-1;i+=3) {
				mesh.f.push_back(indices[3*idx+table[i  ]]);
				mesh.f.push_back(indices[3*idx+table[i+1]]);
				mesh.f.push_back(indices[3*idx+table[i+2]]);
			}
		}
	}


	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);


	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}

void addBrickEdgeZ_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3,
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
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[2]; float df[2]; colortype3 cf[2];

	leafstack stacks[4] = {leaves0,leaves1,leaves2,leaves3};

	sidetype sizeStretch = std::max(std::max(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
                                  leafScale[leaves2[leaves2.size()-1]]);
	sidetype sizeMin =
	std::min(std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]),
			     std::min(leafScale[leaves2[leaves2.size()-1]],leafScale[leaves3[leaves3.size()-1]]));

	sidetype x[4] = {(sidetype)(ox+size-sizeStretch) , (sidetype)(ox+size),
			(sidetype)(ox+size-sizeStretch) , (sidetype)(ox+size)};
	sidetype y[4] = {(sidetype)(oy+size-sizeStretch) , (sidetype)(oy+size-sizeStretch),
			(sidetype)(oy+size) , (sidetype)(oy+size)};


	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = (bz*bl+(i/2))*bl+(i%2);
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
		}
	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		volumetype idx = bz*bs;
		if(weightInfluence(minWeight,
				w[idx],w[idx+1],
				w[idx+bl+1],w[idx+bl],
				w[idx+bs],w[idx+bs+1],
				w[idx+bs+bl+1],w[idx+bs+bl])){
			int *table = mc.offsetTable[mc.getCubeIndex(
					d[idx],d[idx+1],
					d[idx+bl+1],d[idx+bl],
					d[idx+bs],d[idx+bs+1],
					d[idx+bs+bl+1],d[idx+bs+bl],
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])];

			for (unsigned int i=0;table[i]!=-1;i+=3) {
				faceIsSet[3*idx+table[i  ]] = true;
				faceIsSet[3*idx+table[i+1]] = true;
				faceIsSet[3*idx+table[i+2]] = true;
			}
			tables[idx] = table;
		}
	}


	size_t runningIndex = mesh.x.size();
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(unsigned int i=0;i<4;i++){
			volumetype idx = (bz*bl+(i/2))*bl+(i%2);
			if(
					faceIsSet[3*idx+0]
				&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+(x[i]            )*scale,offset.y+y[i]*scale,offset.z+z*scale),
						Vertex3f(offset.x+(x[i]+sizeStretch)*scale,offset.y+y[i]*scale,offset.z+z*scale),
						d[idx],d[idx+1]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
				indices[3*idx+0] = runningIndex++;
				vertexIsSet[3*idx+0] = true;
			}
			if(
					faceIsSet[3*idx+1]
					&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x[i]*scale,offset.y+(y[i]            )*scale,offset.z+z*scale),
						Vertex3f(offset.x+x[i]*scale,offset.y+(y[i]+sizeStretch)*scale,offset.z+z*scale),
						d[idx],d[idx+bl]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
				indices[3*idx+1] = runningIndex++;
				vertexIsSet[3*idx+1] = true;
			}
			if(
				 z<oz+size-sizeMin &&
				 faceIsSet[3*idx+2]
					&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
			){
				Vertex3f ver = MarchingCubes::VertexInterp(
						Vertex3f(offset.x+x[i]*scale,offset.y+y[i]*scale,offset.z+(z        )*scale),
						Vertex3f(offset.x+x[i]*scale,offset.y+y[i]*scale,offset.z+(z+sizeMin)*scale),
						d[idx],d[idx+bs]);
				VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
				mesh.x.push_back(ver.x);
				mesh.y.push_back(ver.y);
				mesh.z.push_back(ver.z);
				mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
				mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
				mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
				indices[3*idx+2] = runningIndex++;
				vertexIsSet[3*idx+2] = true;
			}
		}
	}



	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		volumetype idx = bz*bs;
		int *table = tables[idx];
		if(table){
			for (unsigned int i=0;table[i]!=-1;i+=3) {
				mesh.f.push_back(indices[3*idx+table[i  ]]);
				mesh.f.push_back(indices[3*idx+table[i+1]]);
				mesh.f.push_back(indices[3*idx+table[i+2]]);
			}
		}
	}


	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);
	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}





void addBrickWallX_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0,
		leafstack leaves1,
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
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[4]; float df[4]; colortype3 cf[4];

	leafstack stacks[2] = {leaves0,leaves1};

	sidetype sizeMin = std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]);
	sidetype sizeStretch = leafScale[leaves0[leaves0.size()-1]];
	sidetype x[2] = {(sidetype)(ox+size-sizeStretch) , (sidetype)(ox+size)};
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;

			for(unsigned int bx=0;bx<2;bx++){
				volumetype idx = (bz*bl+by)*bl+bx;
				w[idx] = 0;
				int level;
				for(level = stacks[bx].size()-1;level>=0 && w[idx]<=0.0;level--){
					volumetype leaf = stacks[bx][level];
					volumetype start = leaf*brickSize;
					sidetype3 lo = leafPos[leaf];
					sidetype ls = leafScale[leaf];

					sidetype lx = (x[bx]-lo.x)/ls;

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

					float ryInv = 1.0f-ry;
					float rzInv = 1.0f-rz;

					w[idx] = rzInv*ryInv*(float)wf[0]+
									 rzInv*ry   *(float)wf[1]+
									 rz   *ryInv*(float)wf[2]+
									 rz   *ry   *(float)wf[3];

					d[idx] = rzInv*ryInv*df[0]+
									 rzInv*ry   *df[1]+
									 rz   *ryInv*df[2]+
									 rz   *ry   *df[3];

					if(color)
					c[idx] = VertexColor(
									 rzInv*ryInv*(float)cf[0].x+
									 rzInv*ry   *(float)cf[1].x+
									 rz   *ryInv*(float)cf[2].x+
									 rz   *ry   *(float)cf[3].x,
									 rzInv*ryInv*(float)cf[0].y+
									 rzInv*ry   *(float)cf[1].y+
									 rz   *ryInv*(float)cf[2].y+
									 rz   *ry   *(float)cf[3].y,
									 rzInv*ryInv*(float)cf[0].z+
									 rzInv*ry   *(float)cf[1].z+
									 rz   *ryInv*(float)cf[2].z+
									 rz   *ry   *(float)cf[3].z);
				}
			}
		}
	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			volumetype idx = (bz*bl+by)*bl;

			if(weightInfluence(minWeight,
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])){
				int *table = mc.offsetTable[mc.getCubeIndex(
						d[idx],d[idx+1],
						d[idx+bl+1],d[idx+bl],
						d[idx+bs],d[idx+bs+1],
						d[idx+bs+bl+1],d[idx+bs+bl],
						w[idx],w[idx+1],
						w[idx+bl+1],w[idx+bl],
						w[idx+bs],w[idx+bs+1],
						w[idx+bs+bl+1],w[idx+bs+bl])];

				for (unsigned int i=0;table[i]!=-1;i+=3) {
					faceIsSet[3*idx+table[i  ]] = true;
					faceIsSet[3*idx+table[i+1]] = true;
					faceIsSet[3*idx+table[i+2]] = true;
				}
				tables[idx] = table;
			}
		}
	}

	size_t runningIndex = mesh.x.size();
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			for(sidetype bx=0;bx<2;bx++){
				volumetype idx = (bz*bl+by)*bl+bx;

				if(faceIsSet[3*idx+0]
				 	&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+(x[bx]            )*scale,offset.y+y*scale,offset.z+z*scale),
							Vertex3f(offset.x+(x[bx]+sizeStretch)*scale,offset.y+y*scale,offset.z+z*scale),
							d[idx],d[idx+1]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+0] = runningIndex++;
					vertexIsSet[3*idx+0] = true;
				}
				if(
						y<oy+size-sizeMin &&
						faceIsSet[3*idx+1]
						&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x[bx]*scale,offset.y+(y        )*scale,offset.z+z*scale),
							Vertex3f(offset.x+x[bx]*scale,offset.y+(y+sizeMin)*scale,offset.z+z*scale),
							d[idx],d[idx+bl]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+1] = runningIndex++;
					vertexIsSet[3*idx+1] = true;
				}
				if(
						z<oz+size-sizeMin &&
					 faceIsSet[3*idx+2]
						&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x[bx]*scale,offset.y+y*scale,offset.z+(z        )*scale),
							Vertex3f(offset.x+x[bx]*scale,offset.y+y*scale,offset.z+(z+sizeMin)*scale),
							d[idx],d[idx+bs]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+2] = runningIndex++;
					vertexIsSet[3*idx+2] = true;
				}
			}
		}
	}


	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			volumetype idx = (bz*bl+by)*bl;

			int *table = tables[idx];
			if(table){
				for (unsigned int i=0;table[i]!=-1;i+=3) {
					mesh.f.push_back(indices[3*idx+table[i  ]]);
					mesh.f.push_back(indices[3*idx+table[i+1]]);
					mesh.f.push_back(indices[3*idx+table[i+2]]);
				}
			}
		}
	}

	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);


	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}



void addBrickWallY_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0,
		leafstack leaves1,
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
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[4]; float df[4]; colortype3 cf[4];

	leafstack stacks[2] = {leaves0,leaves1};

	sidetype sizeMin = std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]);
	sidetype sizeStretch = leafScale[leaves0[leaves0.size()-1]];
	sidetype y[2] = {(sidetype)(oy+size-sizeStretch) , (sidetype)(oy+size)};
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			for(unsigned int by=0;by<2;by++){
				volumetype idx = (bz*bl+by)*bl+bx;
				w[idx] = 0;
				int level;
				for(level = stacks[by].size()-1;level>=0 && w[idx]<=0.0;level--){
					volumetype leaf = stacks[by][level];
					volumetype start = leaf*brickSize;
					sidetype3 lo = leafPos[leaf];
					sidetype ls = leafScale[leaf];

					sidetype ly = (y[by]-lo.y)/ls;

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

					float rxInv = 1.0f-rx;
					float rzInv = 1.0f-rz;

					w[idx] = rzInv*rxInv*(float)wf[0]+
									 rzInv*rx   *(float)wf[1]+
									 rz   *rxInv*(float)wf[2]+
									 rz   *rx   *(float)wf[3];

					d[idx] = rzInv*rxInv*df[0]+
									 rzInv*rx   *df[1]+
									 rz   *rxInv*df[2]+
									 rz   *rx   *df[3];

					if(color)
					c[idx] = VertexColor(
									 rzInv*rxInv*(float)cf[0].x+
									 rzInv*rx   *(float)cf[1].x+
									 rz   *rxInv*(float)cf[2].x+
									 rz   *rx   *(float)cf[3].x,
									 rzInv*rxInv*(float)cf[0].y+
									 rzInv*rx   *(float)cf[1].y+
									 rz   *rxInv*(float)cf[2].y+
									 rz   *rx   *(float)cf[3].y,
									 rzInv*rxInv*(float)cf[0].z+
									 rzInv*rx   *(float)cf[1].z+
									 rz   *rxInv*(float)cf[2].z+
									 rz   *rx   *(float)cf[3].z);
				}
			}
		}
	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			volumetype idx = (bz*bl)*bl+bx;
			if(weightInfluence(minWeight,
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])){
				int *table = mc.offsetTable[mc.getCubeIndex(
						d[idx],d[idx+1],
						d[idx+bl+1],d[idx+bl],
						d[idx+bs],d[idx+bs+1],
						d[idx+bs+bl+1],d[idx+bs+bl],
						w[idx],w[idx+1],
						w[idx+bl+1],w[idx+bl],
						w[idx+bs],w[idx+bs+1],
						w[idx+bs+bl+1],w[idx+bs+bl])];

				for (unsigned int i=0;table[i]!=-1;i+=3) {
					faceIsSet[3*idx+table[i  ]] = true;
					faceIsSet[3*idx+table[i+1]] = true;
					faceIsSet[3*idx+table[i+2]] = true;
				}
				tables[idx] = table;
			}
		}
	}

	size_t runningIndex = mesh.x.size();
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;
			for(sidetype by=0;by<2;by++){
				volumetype idx = (bz*bl+by)*bl+bx;

				if(
						x<ox+size-sizeMin &&
						faceIsSet[3*idx+0]
				 	&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+(x        )*scale,offset.y+y[by]*scale,offset.z+z*scale),
							Vertex3f(offset.x+(x+sizeMin)*scale,offset.y+y[by]*scale,offset.z+z*scale),
							d[idx],d[idx+1]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+0] = runningIndex++;
					vertexIsSet[3*idx+0] = true;
				}
				if(
						faceIsSet[3*idx+1]
						&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+(y[by]            )*scale,offset.z+z*scale),
							Vertex3f(offset.x+x*scale,offset.y+(y[by]+sizeStretch)*scale,offset.z+z*scale),
							d[idx],d[idx+bl]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+1] = runningIndex++;
					vertexIsSet[3*idx+1] = true;
				}
				if(
						z<oz+size-sizeMin &&
					 faceIsSet[3*idx+2]
						&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+y[by]*scale,offset.z+(z        )*scale),
							Vertex3f(offset.x+x*scale,offset.y+y[by]*scale,offset.z+(z+sizeMin)*scale),
							d[idx],d[idx+bs]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+2] = runningIndex++;
					vertexIsSet[3*idx+2] = true;
				}
			}
		}
	}

	for(sidetype z=oz;z<oz+size-sizeMin;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;
			volumetype idx = (bz*bl)*bl+bx;

			int *table = tables[idx];
			if(table){
				for (unsigned int i=0;table[i]!=-1;i+=3) {
					mesh.f.push_back(indices[3*idx+table[i  ]]);
					mesh.f.push_back(indices[3*idx+table[i+1]]);
					mesh.f.push_back(indices[3*idx+table[i+2]]);
				}
			}
		}
	}

	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);


	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}

void addBrickWallZ_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0,
		leafstack leaves1,
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
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[4]; float df[4]; colortype3 cf[4];

	leafstack stacks[2] = {leaves0,leaves1};

	sidetype sizeMin = std::min(leafScale[leaves0[leaves0.size()-1]],leafScale[leaves1[leaves1.size()-1]]);
	sidetype sizeStretch = leafScale[leaves0[leaves0.size()-1]];
	sidetype z[2] = {(sidetype)(oz+size-sizeStretch) , (sidetype)(oz+size)};
	for(sidetype y=oy;y<oy+size;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			for(unsigned int bz=0;bz<2;bz++){
				volumetype idx = (bz*bl+by)*bl+bx;
				w[idx] = 0;
				int level;
				for(level = stacks[bz].size()-1;level>=0 && w[idx]<=0.0;level--){
					volumetype leaf = stacks[bz][level];
					volumetype start = leaf*brickSize;
					sidetype3 lo = leafPos[leaf];
					sidetype ls = leafScale[leaf];

					sidetype lz = (z[bz]-lo.z)/ls;

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

					float rxInv = 1.0f-rx;
					float ryInv = 1.0f-ry;

					w[idx] = ryInv*rxInv*(float)wf[0]+
									 ryInv*rx   *(float)wf[1]+
									 ry   *rxInv*(float)wf[2]+
									 ry   *rx   *(float)wf[3];

					d[idx] = ryInv*rxInv*df[0]+
									 ryInv*rx   *df[1]+
									 ry   *rxInv*df[2]+
									 ry   *rx   *df[3];

					if(color)
					c[idx] = VertexColor(
									 ryInv*rxInv*(float)cf[0].x+
									 ryInv*rx   *(float)cf[1].x+
									 ry   *rxInv*(float)cf[2].x+
									 ry   *rx   *(float)cf[3].x,
									 ryInv*rxInv*(float)cf[0].y+
									 ryInv*rx   *(float)cf[1].y+
									 ry   *rxInv*(float)cf[2].y+
									 ry   *rx   *(float)cf[3].y,
									 ryInv*rxInv*(float)cf[0].z+
									 ryInv*rx   *(float)cf[1].z+
									 ry   *rxInv*(float)cf[2].z+
									 ry   *rx   *(float)cf[3].z);
				}
			}
		}
	}

	for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			volumetype idx = by*bl+bx;
			if(weightInfluence(minWeight,
					w[idx],w[idx+1],
					w[idx+bl+1],w[idx+bl],
					w[idx+bs],w[idx+bs+1],
					w[idx+bs+bl+1],w[idx+bs+bl])){
				int *table = mc.offsetTable[mc.getCubeIndex(
						d[idx],d[idx+1],
						d[idx+bl+1],d[idx+bl],
						d[idx+bs],d[idx+bs+1],
						d[idx+bs+bl+1],d[idx+bs+bl],
						w[idx],w[idx+1],
						w[idx+bl+1],w[idx+bl],
						w[idx+bs],w[idx+bs+1],
						w[idx+bs+bl+1],w[idx+bs+bl])];

				for (unsigned int i=0;table[i]!=-1;i+=3) {
					faceIsSet[3*idx+table[i  ]] = true;
					faceIsSet[3*idx+table[i+1]] = true;
					faceIsSet[3*idx+table[i+2]] = true;
				}
				tables[idx] = table;
			}
		}
	}

	size_t runningIndex = mesh.x.size();
	for(sidetype y=oy;y<oy+size;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(sidetype x=ox;x<ox+size;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;
			for(sidetype bz=0;bz<2;bz++){
				volumetype idx = (bz*bl+by)*bl+bx;

				if(
						x<ox+size-sizeMin &&
						faceIsSet[3*idx+0]
				 	&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+(x        )*scale,offset.y+y*scale,offset.z+z[bz]*scale),
							Vertex3f(offset.x+(x+sizeMin)*scale,offset.y+y*scale,offset.z+z[bz]*scale),
							d[idx],d[idx+1]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+0] = runningIndex++;
					vertexIsSet[3*idx+0] = true;
				}
				if(
						y<oy+size-sizeMin &&
						faceIsSet[3*idx+1]
						&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+(y        )*scale,offset.z+z[bz]*scale),
							Vertex3f(offset.x+x*scale,offset.y+(y+sizeMin)*scale,offset.z+z[bz]*scale),
							d[idx],d[idx+bl]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+1] = runningIndex++;
					vertexIsSet[3*idx+1] = true;
				}
				if(
					 faceIsSet[3*idx+2]
						&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
				){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+y*scale,offset.z+(z[bz]            )*scale),
							Vertex3f(offset.x+x*scale,offset.y+y*scale,offset.z+(z[bz]+sizeStretch)*scale),
							d[idx],d[idx+bs]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+2] = runningIndex++;
					vertexIsSet[3*idx+2] = true;
				}
			}
		}
	}

	for(sidetype y=oy;y<oy+size-sizeMin;y+=sizeMin){
		sidetype by = (y-oy)/sizeMin;
		for(sidetype x=ox;x<ox+size-sizeMin;x+=sizeMin){
			sidetype bx = (x-ox)/sizeMin;

			volumetype idx = by*bl+bx;
			int *table = tables[idx];
			if(table){
				for (unsigned int i=0;table[i]!=-1;i+=3) {
					mesh.f.push_back(indices[3*idx+table[i  ]]);
					mesh.f.push_back(indices[3*idx+table[i+1]]);
					mesh.f.push_back(indices[3*idx+table[i+2]]);
				}
			}
		}
	}

	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i - > [%i %i %i]%i",
			ox,oy,oz,lastWrongIndex/3,
			(lastWrongIndex/3)%bl,((lastWrongIndex/3)/bl)%bl,(lastWrongIndex/3)/bs,lastWrongIndex%3);

	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}



void addBrickInterior_indexed
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
//	unsigned int &degenerate_faces = *info.degenerate_faces;
	float3 offset = info.offset;
	float scale = info.scale;
	MeshSeparate &mesh = *info.mesh;

	sidetype bs = bl*bl;

	float *d = new float[brickSize];
	weighttype *w = new weighttype[brickSize];
	VertexColor *c = new VertexColor[brickSize];
	size_t *indices = new size_t[brickSize*3];
	bool *vertexIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) vertexIsSet[i] = false;
	bool *faceIsSet = new bool[brickSize*3];
	for(unsigned int i=0;i<brickSize*3;i++) faceIsSet[i] = false;
	int **tables = new int*[brickSize];
	for(unsigned int i=0;i<brickSize;i++) tables[i] = NULL;

	weighttype wf[8]; float df[8]; colortype3 cf[8];


	sidetype sizeMin = leafScale[leaves[leaves.size()-1]];
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

					float rxInv = 1.0f-rx;
					float ryInv = 1.0f-ry;
					float rzInv = 1.0f-rz;

					if(level==(int)leaves.size()-1 && (rx || ry || rz))
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

					w[idx] = rzInv*ryInv*rxInv*(float)wf[0]+
							     rzInv*ryInv*rx   *(float)wf[1]+
							     rzInv*ry   *rxInv*(float)wf[2]+
								   rzInv*ry   *rx   *(float)wf[3]+
								   rz   *ryInv*rxInv*(float)wf[4]+
									 rz   *ryInv*rx   *(float)wf[5]+
									 rz   *ry   *rxInv*(float)wf[6]+
									 rz   *ry   *rx   *(float)wf[7];

					d[idx] = rzInv*ryInv*rxInv*df[0]+
							     rzInv*ryInv*rx   *df[1]+
							     rzInv*ry   *rxInv*df[2]+
								   rzInv*ry   *rx   *df[3]+
								   rz   *ryInv*rxInv*df[4]+
									 rz   *ryInv*rx   *df[5]+
									 rz   *ry   *rxInv*df[6]+
									 rz   *ry   *rx   *df[7];

					if(color)
						c[idx] = VertexColor(
								     rzInv*ryInv*rxInv*(float)cf[0].x+
								     rzInv*ryInv*rx   *(float)cf[1].x+
								     rzInv*ry   *rxInv*(float)cf[2].x+
									   rzInv*ry   *rx   *(float)cf[3].x+
									   rz   *ryInv*rxInv*(float)cf[4].x+
										 rz   *ryInv*rx   *(float)cf[5].x+
										 rz   *ry   *rxInv*(float)cf[6].x+
										 rz   *ry   *rx   *(float)cf[7].x,
								     rzInv*ryInv*rxInv*(float)cf[0].y+
								     rzInv*ryInv*rx   *(float)cf[1].y+
								     rzInv*ry   *rxInv*(float)cf[2].y+
									   rzInv*ry   *rx   *(float)cf[3].y+
									   rz   *ryInv*rxInv*(float)cf[4].y+
										 rz   *ryInv*rx   *(float)cf[5].y+
										 rz   *ry   *rxInv*(float)cf[6].y+
										 rz   *ry   *rx   *(float)cf[7].y,
								     rzInv*ryInv*rxInv*(float)cf[0].z+
								     rzInv*ryInv*rx   *(float)cf[1].z+
								     rzInv*ry   *rxInv*(float)cf[2].z+
									   rzInv*ry   *rx   *(float)cf[3].z+
									   rz   *ryInv*rxInv*(float)cf[4].z+
										 rz   *ryInv*rx   *(float)cf[5].z+
										 rz   *ry   *rxInv*(float)cf[6].z+
										 rz   *ry   *rx   *(float)cf[7].z);
				}
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
						w[idx+bl+1],w[idx+bl],
						w[idx+bs],w[idx+bs+1],
						w[idx+bs+bl+1],w[idx+bs+bl])){
					int *table = mc.offsetTable[mc.getCubeIndex(
							d[idx],d[idx+1],
							d[idx+bl+1],d[idx+bl],
							d[idx+bs],d[idx+bs+1],
							d[idx+bs+bl+1],d[idx+bs+bl],
							w[idx],w[idx+1],
							w[idx+bl+1],w[idx+bl],
							w[idx+bs],w[idx+bs+1],
							w[idx+bs+bl+1],w[idx+bs+bl])];

					for (unsigned int i=0;table[i]!=-1;i+=3) {
						faceIsSet[3*idx+table[i  ]] = true;
						faceIsSet[3*idx+table[i+1]] = true;
						faceIsSet[3*idx+table[i+2]] = true;
					}
					tables[idx] = table;
				}
			}
		}
	}

	size_t runningIndex = mesh.x.size();
	for(sidetype z=oz;z<oz+size;z+=sizeMin){
		sidetype bz = (z-oz)/sizeMin;
		for(sidetype y=oy;y<oy+size;y+=sizeMin){
			sidetype by = (y-oy)/sizeMin;
			for(sidetype x=ox;x<ox+size;x+=sizeMin){
				sidetype bx = (x-ox)/sizeMin;
				volumetype idx = (bz*bl+by)*bl+bx;

				if(x<ox+size-sizeMin
						&& faceIsSet[3*idx+0]
						&& w[idx] && w[idx+1] && ((d[idx]<0)!=(d[idx+1]<0))
				     ){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+(x      )*scale,offset.y+y*scale,offset.z+z*scale),
							Vertex3f(offset.x+(x+sizeMin)*scale,offset.y+y*scale,offset.z+z*scale),
							d[idx],d[idx+1]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+1],d[idx],d[idx+1]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+0] = runningIndex++;
					vertexIsSet[3*idx+0] = true;
				}
				if(y<oy+size-sizeMin
						&& faceIsSet[3*idx+1]
						&& w[idx] && w[idx+bl] && ((d[idx]<0)!=(d[idx+bl]<0))
				     ){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+(y        )*scale,offset.z+z*scale),
							Vertex3f(offset.x+x*scale,offset.y+(y+sizeMin)*scale,offset.z+z*scale),
							d[idx],d[idx+bl]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bl],d[idx],d[idx+bl]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+1] = runningIndex++;
					vertexIsSet[3*idx+1] = true;
				}
				if(z<oz+size-sizeMin
						&& faceIsSet[3*idx+2]
						&& w[idx] && w[idx+bs] &&((d[idx]<0)!=(d[idx+bs]<0))
						){
					Vertex3f ver = MarchingCubes::VertexInterp(
							Vertex3f(offset.x+x*scale,offset.y+y*scale,offset.z+(z        )*scale),
							Vertex3f(offset.x+x*scale,offset.y+y*scale,offset.z+(z+sizeMin)*scale),
							d[idx],d[idx+bs]);
					VertexColor col = MarchingCubes::VertexInterp(c[idx],c[idx+bs],d[idx],d[idx+bs]);
					mesh.x.push_back(ver.x);
					mesh.y.push_back(ver.y);
					mesh.z.push_back(ver.z);
					mesh.r.push_back(col.x/COLOR_MULTIPLICATOR);
					mesh.g.push_back(col.y/COLOR_MULTIPLICATOR);
					mesh.b.push_back(col.z/COLOR_MULTIPLICATOR);
					indices[3*idx+2] = runningIndex++;
					vertexIsSet[3*idx+2] = true;
				}
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

				int *table = tables[idx];
				if(table){
					for (unsigned int i=0;table[i]!=-1;i+=3) {
						mesh.f.push_back(indices[3*idx+table[i  ]]);
						mesh.f.push_back(indices[3*idx+table[i+1]]);
						mesh.f.push_back(indices[3*idx+table[i+2]]);
					}
				}

			}
		}
	}

	bool loneVertices = false;
	bool wrongIndices = false;
	int lastWrongIndex = -1;
	for(unsigned int i=0;i<brickSize*3;i++) loneVertices |= (vertexIsSet[i]&& !faceIsSet[i]);
	for(unsigned int i=0;i<brickSize*3;i++) {
		wrongIndices |= (!vertexIsSet[i]&& faceIsSet[i]);
		if(!vertexIsSet[i]&& faceIsSet[i]) lastWrongIndex = i;
	}

	if(loneVertices) fprintf(stderr,"\nERROR: There were lone Vertices at [%i %i %i]",ox,oy,oz);
	if(wrongIndices) fprintf(stderr,"\nERROR: There were wrong Indices at [%i %i %i]:%i",
			ox,oy,oz,lastWrongIndex/3);


	delete [] d; delete [] w; delete [] c;
	delete [] indices;
	delete [] vertexIsSet; delete [] faceIsSet;
	delete [] tables;
}


//TODO: Das ist noch nicht wirklich indiziert
void addBrickMiddle_indexed
(
		treeinfo info,
		sidetype ox, sidetype oy, sidetype oz,
		sidetype size,
		leafstack leaves0, leafstack leaves1, leafstack leaves2, leafstack leaves3,
		leafstack leaves4, leafstack leaves5, leafstack leaves6, leafstack leaves7,
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

	float d[8];
	weighttype w[8] = {0,0,0,0,0,0,0,0};
	VertexColor c[8];

	Vertex3f vertlist[12];
	VertexColor collist[12];


	leafstack stacks[8] = {leaves0,leaves1,leaves2,leaves3,
			leaves4,leaves5,leaves6,leaves7};

	sidetype sizeStretch = leafScale[stacks[0][stacks[0].size()-1]];
	for(unsigned int i=1;i<7;i++){
		if(sizeStretch < leafScale[stacks[i][stacks[i].size()-1]])
			 sizeStretch = leafScale[stacks[i][stacks[i].size()-1]];
	}
//	sidetype sizeMin = std::min(sizeStretch,leafScale[stacks[7][stacks[7].size()-1]]);

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







