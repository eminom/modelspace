

#ifndef _BLENDER_NORMAL_DEF__
#define _BLENDER_NORMAL_DEF__

#include "BlenderNode.h"
#include "graphics/DDGraphics.h"
#include "utils/gl_object_wrappers.hpp"

enum class BlenderNormalType
{
	Error,
	Simple,
	Complex
};

class BlenderNormal:public BlenderNode
{
protected:
	BlenderNormal();

public:
	static BlenderNormal* create(const char *path, bool complex = true);
	void setColor(const DDColor&color);

protected:
	template<class T> 
	static T* create(const char *path, bool complex)
	{
		T* rv= new T;
		BlenderNormal *ptr = static_cast<BlenderNormal*>(rv);
		ptr->init(path, complex);
		return rv;
	}

	template<class T, class _P1, class _P2>
	static T* create(const char *path, bool complex, _P1 p1, _P2 p2)
	{
		T* rv = new T(p1,p2);
		BlenderNormal *ptr = static_cast<BlenderNormal*>(rv);
		ptr->init(path, complex);
		return rv;
	}

protected:
	bool init(const char *path, bool complex);
	void draw(const glm::mat4 &mat)override;
	void setColor(float r, float g, float b, float a)override;

protected:
	virtual bool initComplex(const char *path);
	bool initSimple(const char *path);

	virtual void drawComplex(const glm::mat4 &mat);
	virtual void drawSimple(const glm::mat4 &mat);
	

protected:
	VDDPointEx _normal;
	VDDColorEx _colors;

	DD::ArrayBuffer _normalVBO;
	DD::ArrayBuffer _colorVBO;

	GLint _lightPosSlot;
	GLint _modelSlot;
	GLint _normalMatrixSlot;

	BlenderNormalType _type;
};


#endif