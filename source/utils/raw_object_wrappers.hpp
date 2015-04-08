

#ifndef _RAW_OBJECT_WRAPPER_DEF__
#define _RAW_OBJECT_WRAPPER_DEF__

#include <cassert>

namespace DD{


//Warning 
//Codes like:
/////////////////////////////////
/////////////////////////////////
//   RawMemo<GLfloat> k;
//   delete k;
//   delete [] k; 
// will compile without any issues. 
// Fix it.
template<typename T>
class RawMemo
{ 
public:
	RawMemo():ptr_(0)
	{

	}

	~RawMemo()
	{
		deinit();
	}

	void alloc(std::size_t count)
	{
		deinit();
		ptr_ = new T[count];
		size_ = count;
	}

	//operator T*(){
	//	return ptr_;
	//}

	//operator const T*()const{
	//	return ptr_;
	//}

	const T* ptr()const{
		return ptr_;
	}

	template<typename P0>
	void take(int offset, P0 p0)
	{
		checkOffset(offset, 1);
		ptr_[offset] = p0;
	}

	template<typename P0, typename P1>
	void take(int offset, P0 p0, P1 p1)
	{
		checkOffset(offset, 2);
		ptr_[offset] = p0;
		ptr_[offset + 1] = p1;
	}

	template<typename P0, typename P1, typename P2>
	void take(int offset, P0 p0, P1 p1, P2 p2)
	{
		checkOffset(offset, 3);
		ptr_[offset] = p0;
		ptr_[offset + 1] = p1;
		ptr_[offset + 2] = p2;
	}

	template<typename P0, typename P1, typename P2, typename P3>
	void take(int offset, P0 p0, P1 p1, P2 p2, P3 p3)
	{
		checkOffset(offset, 4);
		ptr_[offset] = p0;
		ptr_[offset + 1] = p1;
		ptr_[offset + 2] = p2;
		ptr_[offset + 3] = p3;
	}

	void checkOffset(int offset, int count)
	{
		assert(offset >= 0 && offset + count <= size_);
	}

private:
	void deinit()
	{
		delete [] ptr_;
		size_ = 0;
	}

private:
	static void operator delete(void *ptr, std::size_t);

private:
	std::size_t size_;
	T* ptr_;

private:
	RawMemo(const RawMemo&);
	RawMemo& operator=(const RawMemo&);
};



}//End of namespace DD

#endif