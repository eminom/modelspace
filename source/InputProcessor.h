


#ifndef _INPUT_PROCESSOR_DEF__
#define _INPUT_PROCESSOR_DEF__

#include <glm/glm.hpp>
#include "InputHost.h"

class InputProcessor:public InputHost
{
public:
	InputProcessor();

private:
	virtual void processKeyboard(int key, int scanCode, int action, int modes)override;
	virtual void processMouse(int key, int action, int mods, double, double)override;
	virtual void processScroll(double, double)override;
	virtual void processCursorPos(double,double)override;
	virtual glm::mat4 getMVP()override;

private:
	glm::vec3 _camera;
	glm::vec3 _center;
	glm::vec3 _up;
};


#endif