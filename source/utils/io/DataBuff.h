

#ifndef _DATA_BUFF_DEF__
#define _DATA_BUFF_DEF__

#include <cstdio>

class DataBuff
{
	friend class FileLoader;
public:
	DataBuff();
	~DataBuff();

	DataBuff(const DataBuff&);
	DataBuff& operator=(const DataBuff&);

	char *ptr();
	std::size_t size();

private:
	void deinit();
	DataBuff& copy(const DataBuff&);
	void take(char *data, std::size_t size);

private:
	char *buff_;
	std::size_t size_;
	int *_refCount;
};



#endif