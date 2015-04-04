
//2015
//Coded by eminom

#ifndef _APP_CONTROLLER__DEF__
#define _APP_CONTROLLER__DEF__

#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Director.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

#include "interop/lua_access/refobj.h"

class InputHost;

class AppController
{
public:
	AppController();
	~AppController();
	static AppController& sharedInstance();

	//Hint: GLFW_OPENGL_CORE_PROFILE / GLFW_OPENGL_COMPAT_PROFILE
	int initAppGL(int major, int minor, int hint);
	void mainLoop();
	void setDirector(EsDirector *director);

	bool loadObject(const char *path);

private:
	glm::mat4 getMVP()const;
	void setWindow(GLFWwindow *win);
	GLFWwindow* getWindow();
	void test();
	
private:
	static void closeCallback(GLFWwindow *window);
	//
	static void keyCallback(GLFWwindow *window, int key, int scanCode, int action, int modes);
	static void mouseCallback(GLFWwindow *window, int key, int action, int mods);
	static void scrollCallback(GLFWwindow *window, double x, double y);
	static void cursorPosCallback(GLFWwindow *window, double x, double y);

private:
	GLFWwindow *_window;
	EsDirector *_director;
	double _lastTime;

	RefObject _appObjRef;
	InputHost *_inPro;
	int screen_width_;
	int screen_height_;

	static const char *WindowsTitle;
};

#endif