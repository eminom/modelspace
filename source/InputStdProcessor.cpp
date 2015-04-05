
#include <cstdio>
#include <cstdlib>
#include <glfw3.h>
#include "InputStdProcessor.h"
#include <glm/gtx/transform.hpp>

//#define VertiEpislon		0.01f
//#define HoriEpislon		0.01f
#define MoveForwardStep  0.03f

const double pi = acosf(0) * 2;

struct PresetCamera{
	PresetCamera(float x, float y, float z, float v, float h, const char *text)
		:text(text)
	{
		camera = glm::vec3(x,y,z);
		vAngle = v;
		hAngle = h;
	}
	glm::vec3 camera;
	float vAngle;
	float hAngle;
	const char *text;
};

PresetCamera presetc[]={
	PresetCamera(0,2,4, asinf(-1/sqrtf(5)), pi,"standard"),
	PresetCamera(0,0,2, 0, pi,"front. (to -z)"),
	PresetCamera(0,0,-2,0, 0, "back. (to +z)"),				
	PresetCamera(-2,0,0, 0, pi/2, "left side(to +x)"),
	PresetCamera(2,0,0,  0, -pi/2, "right side(to -x)"),
	PresetCamera(0,2,0,  -pi/2, pi, "from top side(to -y)"),
	PresetCamera(0,-2,0, pi/2,  0, "from bottom(to +y)"),
	PresetCamera(-2, 2, 2, asinf(-1/sqrtf(2)), pi * 3 /4, "")
};

#define MaxPreset	(sizeof(presetc)/sizeof(presetc[0]))

void InputStdProcessor::turnPreset()
{
	_presetIndex = (_presetIndex+1) % MaxPreset;
	const PresetCamera &ref = presetc[_presetIndex];
	_horizontalAngle = ref.hAngle;
	_verticalAngle = ref.vAngle;
	_camera = ref.camera;
	_currentInfo = ref.text;
}


InputStdProcessor::InputStdProcessor()
{
	_currentInfo = "";
	_presetIndex = -1;
	turnPreset();

	//_camera = glm::vec3(0,0,2);
	//_verticalAngle = 0;

	_fov = 30;
	_leftPressed = false;
}

glm::mat4 InputStdProcessor::getMVP()
{
	glm::vec3 direction = glm::vec3(
		cosf(_verticalAngle) * sinf(_horizontalAngle),
		sinf(_verticalAngle),
		cosf(_verticalAngle) * cosf(_horizontalAngle)
	);

	glm::vec3 right = glm::vec3(
		sinf(_horizontalAngle - pi / 2),
		0,
		cosf(_horizontalAngle - pi / 2)
	);

	glm::vec3 up = glm::cross(right, direction);

	glm::mat4 projection = glm::perspective(float(_fov / 180.0f * pi), 4/3.0f, 0.1f, 100.0f);
	glm::mat4 view = glm::lookAt(_camera,
		_camera + direction,
		up
	);
	return projection * view;
}

glm::mat4 InputStdProcessor::getView()
{
	glm::vec3 direction = glm::vec3(
		cosf(_verticalAngle) * sinf(_horizontalAngle),
		sinf(_verticalAngle),
		cosf(_verticalAngle) * cosf(_horizontalAngle)
	);
	glm::vec3 right = glm::vec3(
		sinf(_horizontalAngle - pi / 2),
		0,
		cosf(_horizontalAngle - pi / 2)
	);
	glm::vec3 up = glm::cross(right, direction);
	glm::mat4 view = glm::lookAt(_camera, _camera + direction, up);
	return view;
}

glm::mat4 InputStdProcessor::getProjection()
{
	glm::mat4 projection = glm::perspective(float(_fov / 180.0f * pi), 4/3.0f, 0.1f, 100.0f);
	return projection;
}

void InputStdProcessor::processCursorPos(double x, double y)
{
	if(_leftPressed){
		double delta_x = x - _preCursorX;
		double delta_y = y - _preCursorY;
		_verticalAngle += delta_y * 0.001;
		_horizontalAngle += delta_x * 0.001;
		_preCursorX = x;
		_preCursorY = y;
	}
}

void InputStdProcessor::processMouse(int key, int action, int mods, double x, double y)
{
	if(GLFW_PRESS == action){
		switch(key){
		case GLFW_MOUSE_BUTTON_MIDDLE:
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			_leftPressed = true;
			_preCursorX = x;
			_preCursorY = y;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			break;
		}
	} else if(GLFW_RELEASE == action){
		switch(key){
		case GLFW_MOUSE_BUTTON_MIDDLE:
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			_leftPressed = false;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			break;
		}
	} else {
		//printf("Impossible !\n");
		assert(0);
	}
}

////////
//
void InputStdProcessor::processScroll(double x, double y)
{
	_fov += y;
}

// Now we change a bit
// We shall act like counter-strike
void InputStdProcessor::processKeyboard(int key, int scanCode, int action, int modes){
	glm::vec3 direction = glm::vec3(
		cosf(_verticalAngle) * sinf(_horizontalAngle),
		sinf(_verticalAngle),
		cosf(_verticalAngle) * cosf(_horizontalAngle)
	);
	glm::vec3 right = glm::vec3(
		sinf(_horizontalAngle - pi / 2),
		0,
		cosf(_horizontalAngle - pi / 2)
	);
	glm::vec3 move(0);
	switch(key){
	case GLFW_KEY_LEFT:
		move = right * -MoveForwardStep;
		break;
	case GLFW_KEY_RIGHT:
		move = right * MoveForwardStep;
		break;
	case GLFW_KEY_UP:
		move = direction * MoveForwardStep;
		break;
	case GLFW_KEY_DOWN:
		move = direction * -MoveForwardStep;
		break;
	case GLFW_KEY_SPACE:
		if( GLFW_RELEASE == action){
			turnPreset();
		}
		return;
	}
	_camera += move;
}


const char* InputStdProcessor::info()
{
	return _currentInfo;
}