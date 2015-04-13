

#ifndef _GLOBJS_DEF__HEADER___
#define _GLOBJS_DEF__HEADER___

#include <GL/glew.h>
#include <cassert>
#include <functional>

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
		T::check(obj);
		obj_ = obj;
		refCounter_ = new int(1);
	}

	void make_and_bind()
	{
		deinit();
		T::construct(obj_);
		T::check(obj_);
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
				delete refCounter_;
			}
		}
		obj_ = 0;
		refCounter_ = 0;
	}

	operator GLuint()const{return obj_;}

private:
	GLuint obj_;
	int *refCounter_;
};

#define MakeBinder(func, p0)\
	std::bind(func, p0, std::placeholders::_1)

#define MakeClass(T, constructor, binder, binder_obj, deletor, checker)\
class T:public ObjRef<T>{\
public: static void destruct(GLuint &o){ deletor(1, &o); }\
public: static void check(GLuint o){ assert(GL_TRUE == checker(o));}\
public: static void construct(GLuint &o){ assert(!o); constructor(1, &o); binder(binder_obj,o);}\
};

MakeClass(Texture, glGenTextures, glBindTexture, GL_TEXTURE_2D, glDeleteTextures, glIsTexture)
MakeClass(Framebuffer, glGenFramebuffers, glBindFramebuffer, GL_FRAMEBUFFER, glDeleteFramebuffers, glIsFramebuffer)
MakeClass(Renderbuffer,glGenRenderbuffers, glBindRenderbuffer, GL_RENDERBUFFER, glDeleteRenderbuffers, glIsRenderbuffer)
MakeClass(ArrayBuffer, glGenBuffers, glBindBuffer, GL_ARRAY_BUFFER, glDeleteBuffers, glIsBuffer)

class VertexArrayObject:public ObjRef<VertexArrayObject>{
public: static void destruct(GLuint &o){ glDeleteVertexArrays(1, &o);}
public: static void check(GLuint o) { assert(GL_TRUE == glIsVertexArray(o));}
public: static void construct(GLuint &o) { assert(!o); glGenVertexArrays(1, &o); glBindVertexArray(o);}
};

}	// End of namespace

#endif