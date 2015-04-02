
#ifndef _SIMPLE_STRUCT_DEF__
#define _SIMPLE_STRUCT_DEF__

#include "glm/glm.hpp"
#include <vector>
typedef glm::vec3 DDPoint;
typedef std::vector<DDPoint> VDDPoint;

typedef glm::vec4 DDColor;
typedef std::vector<DDColor> VDDColor;

#define DDColor_White	DDColor(1,1,1,1)
#define DDColor_Red		DDColor(1,0,0,1)
#define DDColor_Green	DDColor(0,1,0,1)
#define DDColor_Blue	DDColor(0,0,1,1)

struct GridPoint
{
	GridPoint(const glm::vec3&pos, const glm::vec4& clr)
		:position(pos),
		color(clr)
	{
	}

	GridPoint(float x, float y, float z, const glm::vec4&clr)
		:position(x,y,z)
		,color(clr)
	{
	}

	glm::vec3 position;
	DDColor color;
};

typedef std::vector<GridPoint> VGridPoint;
struct VGridPointEx:private VGridPoint
{
	void add(float x, float y, float z){
		push_back(GridPoint(x,y,z, DDColor(1,1,1,1)));
	}
	void add(float x, float y, float z, const DDColor&color){
		push_back(GridPoint(x,y,z,color));
	}
	operator VGridPoint&(){
		return *(static_cast<VGridPoint*>(this));
	}
	using VGridPoint::size;
	using VGridPoint::operator[];
};

struct VDDPointEx:public VDDPoint
{
	void add(float x, float y, float z)
	{
		push_back(DDPoint(x,y,z));
	}
};

struct VDDColorEx:public VDDColor
{
	void add(float r, float g, float b, float a)
	{
		push_back(DDColor(r,g,b,a));
	}
};


#endif