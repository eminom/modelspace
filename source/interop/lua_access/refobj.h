

#ifndef _REF_OBJECT_DEF__
#define _REF_OBJECT_DEF__

#include "refproto.h"
#include <string>

class RefObject: public LuaRefProto {
public:
	RefObject();
	RefObject(const char *name);
	virtual ~RefObject();
	RefObject& copy(const RefObject&);
	void loadRef(const char *name);
	void loadFromFunc(const char *name);
	bool require(const char *path);			//Always pop the stack by 1.
	bool requireFunc(const char *path);
	bool requireFuncDo(const char *path);

public:
	virtual int tableRef()const override;
	virtual void createTableRef() override;

	//Now we can copy. and do assignment.
	RefObject& operator=(const RefObject&);
	RefObject(const RefObject&);

private:
	void deinit();
	void take(int refIndex);
	
private:
	int _ref;
	int *_ref_count;
};



#endif