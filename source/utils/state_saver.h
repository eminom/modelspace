

#ifndef _STATE_SAVER_DEF__
#define _STATE_SAVER_DEF__

#include <GL/glew.h>

namespace DD{
class StateSaver
{
public:
	StateSaver()
	{
		isBlending_ = glIsEnabled(GL_BLEND);
	}

	~StateSaver()
	{
		if(isBlending_){
			glEnable(GL_BLEND);
		} else {
			glDisable(GL_BLEND);
		}
	}

private:
	GLboolean isBlending_;
	GLuint co_src_, co_dst_;
};

}


#endif