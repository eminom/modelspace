

#include "AppController.h"
#include "LuaScriptor.h"
#include "InputStdProcessor.h"
#include "InputProcessor.h"
#include "interop/lua_access/lua_access_macros.h"
#include <common/text2D.hpp>


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

void AppController::test()
{
	// Not opened yet??
	GLint multisampling = 0;
	glGetIntegerv(GL_SAMPLE_BUFFERS , &multisampling);
	if (1==multisampling) {
		printf("Multi-sampling is enabled");
		glEnable(GL_MULTISAMPLE);

		GLint sampleCount = 0;
		glGetIntegerv(GL_SAMPLES, &sampleCount);
		for(int i=0;i<sampleCount;++i){
			GLfloat pos[4] = {0};   // a float pair indicating the offsets
			auto ptr = glGetMultisamplefv; // void(unsigned int, unsigned int, float *)
			if(ptr) {
				// Which is missing on window platform. (Mine)
				glGetMultisamplefv(GL_SAMPLE_POSITION, i, pos);
				printf("Sample[%d] offset is (%f,%f)\n", i, pos[0], pos[1]);
			}
		}
	}

	if( glIsEnabled(GL_SCISSOR_TEST) ){
		printf("Scissor test is enabled");
		GLint box[4];
		glGetIntegerv(GL_SCISSOR_BOX, box);
	} else {
		printf("Scissor test is disabled");
		glEnable(GL_SCISSOR_TEST);
		int h_margin = 0;
		int v_margin = 0;
		glScissor(h_margin, v_margin, screen_width_-h_margin * 2, screen_height_ - v_margin * 2);
	}

	/*  // Weird.
	if(glIsEnabled(GL_COLOR_LOGIC_OP)){
		glLogicOp(GL_OR);
	} else {
		printf("Logic op is disabled by default\n");
		glEnable(GL_COLOR_LOGIC_OP);
		assert(glIsEnabled(GL_COLOR_LOGIC_OP));
		glLogicOp(GL_COPY);
	}
	*/
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

	//
	test();

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
	return _appObjRef.require("script.lua");
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