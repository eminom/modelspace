

#include "AppController.h"
#include "LuaScriptor.h"
#include "InputStdProcessor.h"
#include "InputProcessor.h"
#include <common/text2D.hpp>

AppController* _instance = nullptr;

AppController& AppController::sharedInstance()
{
	assert(_instance);
	return *_instance;
}

AppController::~AppController()
{
	_instance = nullptr;	//remove
	delete _inPro;
	cleanupText2D();
}

AppController::AppController()
{
	_window = nullptr;
	_director = nullptr;

	assert(!_instance);
	_instance = this;
	_inPro = nullptr;
}


void AppController::setWindow(GLFWwindow *win)
{
	_window = win;
}

GLFWwindow* AppController::getWindow()
{
	return _window;
}

glm::mat4 AppController::getMVP()const
{
	return _inPro->getMVP();
}

void AppController::keyCallback(GLFWwindow *window, int key, int scanCode, int action, int modes) {
	AppController *self = (AppController*)glfwGetWindowUserPointer(window);
	self->_inPro->processKeyboard(key, scanCode, action, modes);
}

void AppController::scrollCallback(GLFWwindow *window, double x, double y)
{
	AppController *self = (AppController*)glfwGetWindowUserPointer(window);
	self->_inPro->processScroll(x, y);
}

void AppController::cursorPosCallback(GLFWwindow *window, double x, double y)
{
	AppController *self = (AppController*)glfwGetWindowUserPointer(window);
	self->_inPro->processCursorPos(x, y);
}

void AppController::mouseCallback(GLFWwindow *window, int key, int action, int mods)
{
	AppController *self = (AppController*)glfwGetWindowUserPointer(window);
	double x = 0, y = 0;
	glfwGetCursorPos(window, &x, &y);
	self->_inPro->processMouse(key, action, mods, x, y);
}

void AppController::closeCallback(GLFWwindow *window)
{
	AppController *self = (AppController*)glfwGetWindowUserPointer(window);
	//
}


int AppController::initAppGL(int major, int minor, int hint)
{
	if( !glfwInit() ) 
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return 0;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, hint);	//GLFW_OPENGL_COMPAT_PROFILE  or GLFW_OPENGL_CORE_PROFILE
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);	// The depth buffer.
	glfwWindowHint(GLFW_ALPHA_BITS, 8);	//~ suggesting an alpha layer.

	// Open a window and create its OpenGL context
	GLFWwindow *window = glfwCreateWindow( 1024, 768, "Tutorial 02 - Red triangle", NULL, NULL);
	if (! window) {
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(window);

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return 0;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(window, AppController::keyCallback);
	glfwSetCursorPosCallback(window, AppController::cursorPosCallback);
	glfwSetWindowCloseCallback(window, AppController::closeCallback);
	glfwSetMouseButtonCallback(window, AppController::mouseCallback);
	glfwSetScrollCallback(window, AppController::scrollCallback);
	glfwSetWindowUserPointer(window, this);

	////////////////////
	glClearColor(0, 0, 0.1f, 0.0f);
	_lastTime = glfwGetTime();
	setWindow(window);	//

	initText2D("objects/Holstein.DDS");
	_inPro = new InputStdProcessor();
	return 1;
}

bool AppController::loadObject()
{
	lua_State *L = LuaScriptor::sharedInstance()->getLuaState();
	int top = lua_gettop(L);

	int traceback = 0;
	lua_getglobal(L, "__G__TRACKBACK__");
	if( lua_isfunction(L, -1)){
		traceback = -2;
	}
	luaL_loadfile(L, "script.lua");
	if( lua_pcall(L, 0, 1, traceback) ){
		lua_pop(L, 2);	/// skip the error message
		assert( top == lua_gettop(L) );
		printf("Error executing script\n");
		assert(0);
		return false;
	}
	if(!lua_istable(L, -1)){
		printf("I need a table for this\n");
		lua_pop(L, 2);
		assert( top == lua_gettop(L));
		assert( 0 );
		return false;
	}
	_appObjRef.loadFromTop();
	lua_pop(L, 1);
	assert( top == lua_gettop(L));	//Balance
}

void AppController::setDirector(EsDirector *director)
{
	assert( !_director );
	assert( director);
	_director = director;
}

void AppController::mainLoop()
{
	_appObjRef.execVoid("OnAppReady");
	do{
		glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
		_appObjRef.execVoid("Update");
		printText2D(_inPro->info(), 0 , 700, 30);
		double now = glfwGetTime();
		float dt = float(now - _lastTime);
		_lastTime = now;
		_director->updateNode(dt);

		glm::mat4 mvp = getMVP();
		_director->render(mvp);

		glfwSwapBuffers(_window);
		glfwPollEvents();
	} while( glfwGetKey(_window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(_window) == 0 );
	glfwDestroyWindow(_window);
	_window = nullptr;
	glfwTerminate();
}