

#ifndef _MACROS_FOR_MODELSPACE__
#define _MACROS_FOR_MODELSPACE__

#define CheckGL()\
	if(auto err = glGetError()){\
		fprintf(stderr, "OpenGL state error %d, %s in %s of line %d\n", err, __FILE__, __FUNCTION__, __LINE__);\
	}


#endif