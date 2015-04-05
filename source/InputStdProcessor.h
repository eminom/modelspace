

#ifndef _INPUT_STD_PROCESSOR__DEF__
#define _INPUT_STD_PROCESSOR__DEF__

#include "InputHost.h"

class InputStdProcessor:public InputHost
{
public:
	InputStdProcessor();

private:
	virtual void processKeyboard(int key, int scanCode, int action, int modes)override;
	virtual void processMouse(int key, int action, int mods, double, double)override;
	virtual void processScroll(double, double)override;
	virtual void processCursorPos(double, double)override;
	virtual glm::mat4 getMVP()override;
	virtual glm::mat4 getView()override;
	virtual glm::mat4 getProjection()override;
	virtual void turnPreset()override;
	virtual const char* info()override;

private:
	float _verticalAngle;
	float _horizontalAngle;
	float _fov;
	glm::vec3 _camera;

	bool _leftPressed;
	double _preCursorX;
	double _preCursorY;
	int _presetIndex;
	const char *_currentInfo;
};


#endif