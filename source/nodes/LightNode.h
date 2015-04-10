

#ifndef _LIGHT_NODE_DEF__
#define _LIGHT_NODE_DEF__

#include "LightProtocol.h"
#include "BlenderNormal.h"

class LightNode:public EsNode, public LightProtocol
{
	//So fucking twisted. 
	friend class BlenderNormal;
private:
	LightNode();
	bool init(const DDPoint &pos);

public:
	static LightNode* create(float x, float y, float z);
	virtual DDPoint position()override;
	virtual void update(float dt)override;
	void setTimeScale(float scale);
	void draw(const glm::mat4 &vp)override;
	void setVAngleSpeed(float val);
	void setHAngleSpeed(float val);

private:
	DDPoint _position;
	GLfloat _radius;
	GLfloat _vAngle;
	GLfloat _hAngle;
	GLfloat _vAngleSpeed;
	GLfloat _hAngleSpeed;
	float _timeScale;

	BlenderNormal *_mode;
};

#endif