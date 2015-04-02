


#ifndef _MODEL_OBJECT_DEF__
#define _MODEL_OBJECT_DEF__

#include "nodes/graphics/DDGraphics.h"
#include <vector>

class ObjectModel
{
public:
	static void genGrids_third(std::vector<GridPoint> &grids, 	float lower = -10, float higher = 10, int ac = 20);
	static void genBox(std::vector<GridPoint> &points, float width, float height, float vheight);
	static void genSolidBox(std::vector<GridPoint> &points, float width, float height, float vheight, DDColor defaultColor=DDColor(1,0,0,1));

	static void loadFromJson(const char* path, VDDPoint &vertex, std::vector<GLushort> &indice);
	static void loadFromJsonWithNormal(const char* path, VDDPoint &vertex, VDDPoint &normal, VDDColorEx &colors);
	static void loadFromJsonWithoutNormal(const char* path, VDDPointEx &vertex, std::vector<GLushort> &indice);
};



#endif