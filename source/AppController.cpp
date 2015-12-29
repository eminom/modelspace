

#include "AppController.h"
#include "LuaScriptor.h"
#include "InputStdProcessor.h"
#include "InputProcessor.h"
#include <common/text2D.hpp>
#include "EventCenter.h"

#define DefaultScreenWidth	1024
#define DefaultScreenHeight	768

AppController* _instance = nullptr;

const char* AppController::WindowsTitle = "ModelSpace";

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
	:_appObjRef(LuaScriptor::sharedInstance()->getLuaState())
{
	_window = nullptr;
	_director = nullptr;

	assert(!_instance);
	_instance = this;
	_inPro = nullptr;

	screen_width_ = DefaultScreenWidth;
	screen_height_= DefaultScreenHeight;
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

glm::mat4 AppController::getView()const
{
	return _inPro->getView();
}

glm::mat4 AppController::getProjection()const
{
	return _inPro->getProjection();
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
	//AppController *self = (AppController*)glfwGetWindowUserPointer(window);
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
	GLFWwindow *window = glfwCreateWindow( screen_width_, screen_height_, AppController::WindowsTitle, NULL, NULL);
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

	// glGetIntegerv for GL_SAMPLE_BUFFERS will be zero before window's creation.
	// and some glews will not be initialized.(we've been already 4.1)
	CheckGL()
	test();
	prepareFramebuffer();
	CheckGL()
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetKeyCallback(window, AppController::keyCallback);
	glfwSetCursorPosCallback(window, AppController::cursorPosCallback);
	glfwSetWindowCloseCallback(window, AppController::closeCallback);
	glfwSetMouseButtonCallback(window, AppController::mouseCallback);
	glfwSetScrollCallback(window, AppController::scrollCallback);
	glfwSetWindowUserPointer(window, this);

	////////////////////
	glClearColor(0.3f, 0.3f, 0.3f, 0.0f);
	_lastTime = glfwGetTime();
	setWindow(window);	//
	initText2D("objects/Holstein.DDS");
	_inPro = new InputStdProcessor();
	return 1;
}

bool AppController::loadObject(const char *path)
{
	return _appObjRef.require(path);
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
	CheckGL()

	if( ! vm_.loadScript("exec/inprogress.lua") ){
		return;
	}

	vm_.start("step");
	//vm_.start("step");
	CheckGL()

	float accDT = 0;
	do{
		double now = glfwGetTime();
		float dt = float(now - _lastTime);
		_lastTime = now;

		auto drawer = [dt, this](){
			glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
			_appObjRef.execVoid("Update");
			printText2D(_inPro->info(), 0 , 700, 30);
			_director->updateNode(dt);
			glm::mat4 mvp = getMVP();
			_director->render(mvp);
		};

		if(1){
			startFramebuffer();
			drawer();
			finalizeFramebuffer();
		} else {
			drawer();
		}

		accDT += dt;
		if(accDT>=2.0){
			accDT = 0;
			//vm_.step();
			EventCenter::instance()->Go.trigger();
		}

		glfwSwapBuffers(_window);
		glfwPollEvents();
	} while( glfwGetKey(_window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(_window) == 0 );
	glfwDestroyWindow(_window);
	_window = nullptr;
	glfwTerminate();
}
