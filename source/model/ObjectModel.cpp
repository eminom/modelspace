
#include "objectmodel.h"
#include "cJSON.h"

const glm::vec4 colorRed(1,  0,  0,  1.0f);
const glm::vec4 colorGreen(0,  1.0f,  0,  1.0f);
const glm::vec4 colorBlue(0, 0,  1.0f,  1.0f);

#define AssertType(a,t)	(a->type == t)

void ObjectModel::genGrids_third(std::vector<GridPoint> &grids, float lower,float higher, int ac)
{
	grids.clear();

	float dt = (higher-lower) / ac;

	//XY
	//for(int k=0;k<=ac;++k){
	//	//grids.push_back(GridPoint(vec3(x, lower,  0),colorRed));
	//	//grids.push_back(GridPoint(vec3(x, higher, 0), colorRed));
	//	float x = lower + dt * k;
	//	grids.push_back(GridPoint(vec3(x, lower,  0), colorRed));
	//	grids.push_back(GridPoint(vec3(x, higher, 0), colorRed));
	//}
	//for(int k=0;k<=ac;++k){
	//	//grids.push_back(GridPoint(vec3(lower,  y, 0), colorRed));
	//	//grids.push_back(GridPoint(vec3(higher, y, 0), colorRed));
	//	float y = lower + k * dt;
	//	grids.push_back(GridPoint(vec3(lower, y,0), colorRed));
	//	grids.push_back(GridPoint(vec3(higher,y,0), colorRed));
	//}

	//YZ
	//for(int k=0;k<=ac;++k){
	//	float y = lower + k * dt;
	//	grids.push_back(GridPoint(vec3(0, y, lower),  colorBlue));
	//	grids.push_back(GridPoint(vec3(0, y, higher), colorBlue));
	//}
	//for(int k=0;k<=ac;++k){
	//	float z = lower + k * dt;
	//	grids.push_back(GridPoint(vec3(0, lower, z), colorBlue));
	//	grids.push_back(GridPoint(vec3(0, higher, z),colorBlue));
	//}

	//XZ
	for(int k=0;k<=ac;++k){
		float x = lower + k * dt;
		grids.push_back(GridPoint(glm::vec3(x,0,lower), colorGreen));
		grids.push_back(GridPoint(glm::vec3(x,0,higher),colorGreen));
	}
	for(int k=0;k<=ac;++k){
		float z = lower + k * dt;
		grids.push_back(GridPoint(glm::vec3(lower,0, z), colorGreen));
		grids.push_back(GridPoint(glm::vec3(higher,0,z), colorGreen));
	}
}

// On the xz plane.
void ObjectModel::genBox(std::vector<GridPoint> &points, float width, float height, float vheight)
{
	//BOTTOM
	points.push_back(GridPoint(width / 2,  -vheight / 2, height / 2, colorRed));
	points.push_back(GridPoint(-width / 2, -vheight /2, height / 2, colorRed));

	points.push_back(GridPoint(width / 2,  -vheight / 2, -height / 2, colorRed));
	points.push_back(GridPoint(-width / 2, -vheight /2,  -height / 2, colorRed));

	points.push_back(GridPoint(width / 2,  -vheight / 2, height / 2, colorRed));
	points.push_back(GridPoint(width / 2, -vheight /2,   -height / 2, colorRed));

	points.push_back(GridPoint(-width / 2,  -vheight / 2, height / 2, colorRed));
	points.push_back(GridPoint(-width / 2,  -vheight /2, -height / 2, colorRed));

	//TOP
	points.push_back(GridPoint(width / 2,  vheight / 2, height / 2, colorRed));
	points.push_back(GridPoint(-width / 2, vheight /2, height / 2, colorRed));

	points.push_back(GridPoint(width / 2,  vheight / 2, -height / 2, colorRed));
	points.push_back(GridPoint(-width / 2, vheight /2,  -height / 2, colorRed));

	points.push_back(GridPoint(width / 2,  vheight / 2, height / 2, colorRed));
	points.push_back(GridPoint(width / 2,  vheight /2,   -height / 2, colorRed));

	points.push_back(GridPoint(-width / 2,  vheight / 2, height / 2, colorRed));
	points.push_back(GridPoint(-width / 2,  vheight /2, -height / 2, colorRed));

	//THE FOUR
	points.push_back(GridPoint(width / 2,  vheight / 2, height / 2, colorRed));
	points.push_back(GridPoint(width / 2, -vheight /2,  height / 2, colorRed));

	points.push_back(GridPoint(width / 2,  vheight / 2,  -height / 2, colorRed));
	points.push_back(GridPoint(width / 2,  -vheight /2,  -height / 2, colorRed));

	points.push_back(GridPoint(-width / 2,  vheight / 2, -height / 2, colorRed));
	points.push_back(GridPoint(-width / 2,  -vheight /2, -height / 2, colorRed));

	points.push_back(GridPoint(-width / 2,  vheight / 2, height / 2, colorRed));
	points.push_back(GridPoint(-width / 2,  -vheight /2, height / 2, colorRed));
}

void ObjectModel::genSolidBox(std::vector<GridPoint> &points, float w, float h, float v, DDColor defaultColor)
{
	//ZX plane :bottom and top
	do{
	float pts[]={
		w/2,  h /2,
		w/2,  -h/2,
		-w/2,  h/2,

		-w/2,  h/2,
		w/2,  -h/2,
		-w/2,  -h/2
	};
	for(int i=0;i<sizeof(pts)/sizeof(pts[0]);i+=2){
		points.push_back(GridPoint(pts[i], -v/2, pts[i+1],defaultColor));
	}
	for(int i=0;i<sizeof(pts)/sizeof(pts[0]);i+=2){
		points.push_back(GridPoint(pts[i], v/2, pts[i+1],defaultColor));
	}
	}while(0);

	//XY plane
	//near z and far z
	do{
	float pts[]={
		w/2, -v/2,
		w/2, v/2,
		-w/2, -v/2,

		-w/2, -v/2,
		w/2, v/2,
		-w/2,v/2,
	};
	for(int i=0;i<sizeof(pts)/sizeof(pts[0]);i+=2){
		points.push_back(GridPoint(pts[i], pts[i+1], h/2, defaultColor));
	}
	for(int i=0;i<sizeof(pts)/sizeof(pts[0]);i+=2){
		points.push_back(GridPoint(pts[i], pts[i+1], -h/2, defaultColor));
	}
	}while(0);

	//ZY plane
	//near x and far x
	float pts[]={
		h/2, -v/2,
		-h/2, -v/2,
		-h/2, v/2,

		h/2, -v/2,
		-h/2, v/2,
		h/2, v/2,
	};
	for(int i=0;i<sizeof(pts)/sizeof(pts[0]);i+=2){
		points.push_back(GridPoint(w/2, pts[i+1], pts[i], defaultColor));
	}
	for(int i=0;i<sizeof(pts)/sizeof(pts[0]);i+=2){
		points.push_back(GridPoint(-w/2, pts[i+1], pts[i], defaultColor));
	}
}


void ObjectModel::loadFromJson(const char *name, std::vector<glm::vec3> &vertice, std::vector<GLushort> &indice)
{
	if(FILE *fin = fopen(name,"r")){
		fseek(fin, 0, SEEK_END);
		long sz = ftell(fin);
		fseek(fin,0, SEEK_SET);
		char *buffer = new char[sz+1];
		memset(buffer,0, (sz+1)*sizeof(char));
		fread(buffer, sizeof(char), sz, fin);
		cJSON* obj = cJSON_Parse(buffer);
		if(cJSON* vertex = cJSON_GetObjectItem(obj, "Vertex")){
			if (cJSON_Array == vertex->type){
				int count = cJSON_GetArraySize(vertex);
				for(int i=0;i<count;++i){
					cJSON* one = cJSON_GetArrayItem(vertex, i);
					assert( cJSON_Array == one->type);
					int trsz = cJSON_GetArraySize(one);
					GLfloat ones[3] = {0};
					for(int j=0;j<trsz && j <(sizeof(ones)/sizeof(ones[0]));++j){
						cJSON *v = cJSON_GetArrayItem(one, j);
						assert(cJSON_Number == v->type);
						ones[j] = v->valuedouble;	//
					}
					vertice.push_back(glm::vec3(ones[0], ones[1], ones[2]));
				}
			}
		}

		if(cJSON* face = cJSON_GetObjectItem(obj, "Face")){
			if (cJSON_Array == face->type){
				int count = cJSON_GetArraySize(face);
				for(int i=0;i<count;++i){
					cJSON *one = cJSON_GetArrayItem(face, i);
					assert(cJSON_Array == one->type);
					int trsz = cJSON_GetArraySize(one);
					GLuint index[3] = {0};
					for(int j=0;j<trsz&&j<(sizeof(index)/sizeof(index[0]));++j){
						cJSON *u = cJSON_GetArrayItem(one, j);
						assert( cJSON_Number == u->type);
						index[j] = u->valueint;
						indice.push_back(u->valueint - 1);	//offset by one
					}
				}
			}
		}
		cJSON_Delete(obj);
		fclose(fin);
	}

}

void ObjectModel::loadFromJsonWithNormal(const char* path, VDDPoint &vertex, VDDPoint &normal, VDDColorEx &colors)
{
	if(FILE *fin = fopen(path, "r")){
		fseek(fin, 0, SEEK_END);
		int size = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		char *buf = new char[size+1];
		memset(buf, 0, sizeof(char) * (size+1));
		fread(buf, 1, size, fin);
		fclose(fin);

		VDDPointEx vs, ns;
		cJSON *root = cJSON_Parse(buf);
		cJSON *va = cJSON_GetObjectItem(root, "Vertex");
		const int vCount = cJSON_GetArraySize(va);
		for(int i=0;i<vCount;++i){
			cJSON *e = cJSON_GetArrayItem(va, i);
			float pts[3] = {0};
			for(int j=0;j<3;++j){
				cJSON *value = cJSON_GetArrayItem(e, j);
				pts[j] = value->valuedouble;
			}
			vs.add(pts[0], pts[1], pts[2]);
		}

		cJSON *na = cJSON_GetObjectItem(root, "Normal");
		const int nCount = cJSON_GetArraySize(na);
		for(int i=0;i<nCount;++i){
			cJSON *e = cJSON_GetArrayItem(na, i);
			float pts[3] = {0};
			for(int j=0;j<3;++j){
				cJSON *value = cJSON_GetArrayItem(e, j);
				pts[j] = value->valuedouble;
			}
			ns.add(pts[0], pts[1], pts[2]);
		}

		cJSON *fna = cJSON_GetObjectItem(root, "FaceN");
		const int count = cJSON_GetArraySize(fna);
		for(int i=0;i<count;++i){
			cJSON *e = cJSON_GetArrayItem(fna, i);
			for(int j=0;j<3;++j){
				cJSON *value = cJSON_GetArrayItem(e, j);
				int i1 = 0, i2 = 0;
				sscanf(value->valuestring, "%d/%d", &i1, &i2);
				assert(i1>0);
				assert(i2>0);
				vertex.push_back(vs[i1-1]);
				normal.push_back(ns[i2-1]);
				colors.push_back(DDColor(0.3, 0.5, 0.7, 1));
			}
		}
		assert(cJSON_Array == va->type);
		assert(cJSON_Array == na->type);
	} else {
		fprintf(stderr, "Cannot open %s\n", path);
	}
}


void ObjectModel::loadFromJsonWithoutNormal(const char* path, VDDPointEx &vertex, std::vector<GLushort> &indice)
{
	if(FILE *fin = fopen(path, "r")){
		fseek(fin, 0, SEEK_END);
		int size = ftell(fin);
		fseek(fin, 0, SEEK_SET);
		char *buf = new char[size+1];
		memset(buf, 0, sizeof(char) * (size+1));
		fread(buf, 1, size, fin);
		fclose(fin);

		VDDPointEx &vs = vertex;
		cJSON *root = cJSON_Parse(buf);
		cJSON *va = cJSON_GetObjectItem(root, "Vertex");
		assert(cJSON_Array == va->type);
		const int vCount = cJSON_GetArraySize(va);
		for(int i=0;i<vCount;++i){
			cJSON *e = cJSON_GetArrayItem(va, i);
			float pts[3] = {0};
			for(int j=0;j<3;++j){
				cJSON *value = cJSON_GetArrayItem(e, j);
				pts[j] = value->valuedouble;
			}
			vs.add(pts[0], pts[1], pts[2]);
		}

		cJSON *fna = cJSON_GetObjectItem(root, "FaceN");
		const int count = cJSON_GetArraySize(fna);
		for(int i=0;i<count;++i){
			cJSON *e = cJSON_GetArrayItem(fna, i);
			for(int j=0;j<3;++j){
				cJSON *value = cJSON_GetArrayItem(e, j);
				int i1 = 0, i2 = 0;
				sscanf(value->valuestring, "%d/%d", &i1, &i2);
				assert(i1>0);
				assert(i2>0);
				indice.push_back(i1-1);
			}
		}
		int x = 100;
	} else {
		fprintf(stderr, "Cannot open %s\n", path);
	}
}