
#include <cstdio>
#include <cstdlib>
#include <glfw3.h>
#include <glm/gtx/transform.hpp>

#include "InputProcessor.h"

//#define VertiEpislon	0.01f
//#define HoriEpislon		0.01f
#define MoveForwardStep  0.03


InputProcessor::InputProcessor()
{
	_camera = glm::vec3(0, 1, 2);
	_center = glm::vec3(0,0,0);
	_up = glm::vec3(0, 2, -1);
}

void InputProcessor::processMouse(int key, int action, int mods, double, double)
{
	if(GLFW_PRESS == action){
		switch(key){
		case GLFW_MOUSE_BUTTON_MIDDLE:
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			break;
		}
	} else if(GLFW_RELEASE == action){
		switch(key){
		case GLFW_MOUSE_BUTTON_MIDDLE:
			break;
		case GLFW_MOUSE_BUTTON_LEFT:
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			break;
		}
	} else {
		printf("Impossible !\n");
		assert(0);
	}
}

////////
//
void InputProcessor::processScroll(double x, double y)
{
	glm::vec3 eyeVec = _center - _camera;
	glm::vec3 theCross = glm::cross(eyeVec, _up);
	glm::mat4 i = glm::rotate(float(y) * 1.0f / 180.0f, theCross);

	glm::vec4 newEyeVec = i * glm::vec4(eyeVec, 0);
	glm::vec4 newUp     = i * glm::vec4(_up, 0);

	_center = _camera + glm::vec3( glm::normalize(newEyeVec) * glm::length(eyeVec));
	_up = glm::vec3(glm::normalize(newUp));

	//glm::vec3 vertShift = 0.01f * float(y) * glm::vec3(0,1,0);
	//_camera += vertShift;
}


// Now we change a bit
// We shall act like counter-strike
void InputProcessor::processKeyboard(int key, int scanCode, int action, int modes){
	const glm::vec3 eyeVec = _center - _camera;
	float horiRotate = 0;
	glm::vec3 lookAxis = glm::normalize(eyeVec);

	bool isHori = false;
	switch(key){
	case GLFW_KEY_LEFT:
		isHori = true;
		horiRotate = 1.0f;
		break;
	case GLFW_KEY_RIGHT:
		horiRotate = -1.0f;
		isHori = true;
		break;
	case GLFW_KEY_UP:
		lookAxis *= MoveForwardStep;
		break;
	case GLFW_KEY_DOWN:
		lookAxis *= -MoveForwardStep;
		break;
	case GLFW_KEY_SPACE:
		return;
	}

	/// DO IT
	if (isHori) {  // Rotate along the up vector.
		glm::mat4 rot = glm::rotate(glm::mat4(1), horiRotate / 180.0f, _up);
		glm::vec4 newEyeVec = rot * glm::vec4(eyeVec, 0);
		newEyeVec = glm::normalize(newEyeVec);
		_center = glm::length(eyeVec) * glm::vec3(newEyeVec) + _camera;
	} else {
		//Forward, backward
		_center += lookAxis;
		_camera += lookAxis;
	}
}


// The old view
//void AppController::processKeyboard(int key, int scanCode, int action, int modes){
//	glm::vec3 eyeVec = theCenter - _camera;
//	glm::vec3 horiShift = glm::cross(eyeVec, _up);
//	horiShift = glm::normalize(horiShift);
//
//	glm::vec3 vertiShift = glm::normalize(_up);
//
//	bool isHori = false;
//	switch(key){
//	case GLFW_KEY_LEFT:
//		horiShift *= -HoriEpislon;
//		isHori = true;
//		break;
//	case GLFW_KEY_RIGHT:
//		horiShift *= HoriEpislon;
//		isHori = true;
//		break;
//	case GLFW_KEY_UP:
//		vertiShift *= VertiEpislon;
//		break;
//	case GLFW_KEY_DOWN:
//		vertiShift *= -VertiEpislon;
//		break;
//	}
//
//	/// DO IT
//	if (isHori) {
//		_camera += horiShift;
//	} else {
//		_camera += vertiShift;
//	}
//}

void InputProcessor::processCursorPos(double,double)
{}




glm::mat4 InputProcessor::getMVP()
{
	glm::mat4 projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.5f, 140.0f);
	glm::mat4 view = glm::lookAt( _camera, _center, _up );
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mvp = projection * view * model;
	return mvp;
}