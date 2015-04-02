

#ifndef _DDAPI_DEF__
#define _DDAPI_DEF__

#include "DDStruct.h"

inline void addPoint(VDDPoint &v, float x, float y, float z)
{
	v.push_back(DDPoint(x,y,z));
}

#endif