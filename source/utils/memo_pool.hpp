
#ifndef _MEMO_OBJ_DEF__
#define _MEMO_OBJ_DEF__

#include <vector>

//T stands for a POD(not ctor and no dtor. just memories)
template<typename T>
class MemoPool
{
private:
	struct MemoNode
	{
		MemoNode *next;
	};
public:
	MemoPool()
		:_headfree(nullptr)
	{

	}

	~MemoPool()
	{
		purge();
	}

	void purge()
	{
		for(auto &o:_memo){
			::free(o);
		}
		_memo.clear();
	}

	void free(T *obj)
	{
		MemoNode *node = reinterpret_cast<MemoNode*>(obj);
		node->next = _headfree;
		_headfree = node;
	}

	T* alloc()
	{
		if(!_headfree) {
			alloc_memo(512);
		}
		assert(_headfree);
		T *rv = (T*)_headfree;
		_headfree = _headfree->next;
		return rv;
	}

private:
	void alloc_memo(int count) {
		//printf("alloc_memo: more at %f \n", glfwGetTime());
		size_t nodesize = sizeof(MemoNode);//Default chunk size is 
		if (nodesize < sizeof(T)) {
			nodesize = sizeof(T);
		}
		void *ptr = malloc(nodesize * count);
		_memo.push_back(ptr);
		T *k_ptr = (T*)ptr;
		for(int i=0;i<count;++i,++k_ptr){
			free(k_ptr);
		}
	}

private:
	std::vector<void*> _memo;
	MemoNode *_headfree;
};

#endif
