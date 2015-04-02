

#ifndef _INPUT_HANDLER__DEF__
#define _INPUT_HANDLER__DEF__

#include <glm/glm.hpp>

class InputHost
{
public:
	virtual void processKeyboard(int key, int scanCode, int action, int modes) = 0;
	virtual void processMouse(int key, int action, int mods, double x, double y) = 0;
	virtual void processScroll(double, double) = 0;
	virtual void processCursorPos(double, double) = 0;
	virtual glm::mat4 getMVP() = 0;
	virtual void turnPreset(){}
	virtual const char* info(){return "";}
};


#endif