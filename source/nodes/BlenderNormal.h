

#ifndef _BLENDER_NORMAL_DEF__
#define _BLENDER_NORMAL_DEF__

#include "BlenderNode.h"
#include "graphics/DDGraphics.h"


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
	~BlenderNormal();

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

protected:
	bool init(const char *path, bool complex);
	void draw(const glm::mat4 &mat)override;
	void setColor(float r, float g, float b, float a)override;

private:
	bool initComplex(const char *path);
	bool initSimple(const char *path);

	void drawComplex(const glm::mat4 &mat);
	void drawSimple(const glm::mat4 &mat);
	

private:
	VDDPointEx _normal;
	VDDColorEx _colors;

	GLuint _normalVBO;
	GLuint _colorVBO;

	GLint _lightPosSlot;
	GLint _modelSlot;
	GLint _normalMatrixSlot;

	BlenderNormalType _type;
};


#endif