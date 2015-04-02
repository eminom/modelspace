


#ifndef _PRIMITIVE_NODE__DEF__
#define _PRIMITIVE_NODE__DEF__

#include "EsNode.h"

class PrimitiveNode:public EsNode
{
protected:
	PrimitiveNode();

protected:
	virtual void draw(const glm::mat4 &transform)override;

};


#endif