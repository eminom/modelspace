

#ifndef _GLOBJS_DEF__HEADER___
#define _GLOBJS_DEF__HEADER___

#include <GL/glew.h>
#include <cassert>

namespace DD
{

template<typename T>
class ObjRef
{
public:
	ObjRef()
		:obj_(0)
		,refCounter_(nullptr)
	{
	}

	ObjRef(const ObjRef &rhs)
		:obj_(0)
		,refCounter_(nullptr)
	{
		copy(rhs);
	}

	~ObjRef()
	{
		deinit();
	}

	void take(GLint obj)
	{
		deinit();
		obj_ = obj;
		refCounter_ = new int(1);
	}

	ObjRef& operator=(const ObjRef&rhs)
	{
		return copy(rhs);
	}

	ObjRef& copy(const ObjRef&rhs){
		deinit();
		if(rhs.refCounter_){
			assert( *rhs.refCounter_ > 0);
			obj_ = rhs.obj_;
			refCounter_ = rhs.refCounter_;
			++(*refCounter_);
		}
		return *this;
	}

	operator bool()const{
		return refCounter_;
	}

	void deinit(){
		if(refCounter_){
			--(*refCounter_);
			if(0 == *refCounter_){
				T::destruct(obj_);
				obj_ = 0;
				refCounter_ = 0;
			}
		}
	}

	operator GLuint()const{return obj_;}

private:
	GLuint obj_;
	int *refCounter_;
};

#define MakeClass(T, deletor)\
class T:public ObjRef<T>{\
public: static void destruct(GLuint &o){ deletor(1, &o); }\
};

MakeClass(Texture, glDeleteTextures)
MakeClass(Framebuffer, glDeleteFramebuffers)
MakeClass(Renderbuffer, glDeleteRenderbuffers)
MakeClass(ArrayBuffer, glDeleteBuffers)

}	// End of namespace

#endif