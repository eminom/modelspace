

#ifndef _DATA_BUFF_DEF__
#define _DATA_BUFF_DEF__

#include <cstdio>

class DataBuff
{
	typedef unsigned char DataBuff_t;
	//friend class FileLoader;
public:
	explicit DataBuff(std::size_t l);
	DataBuff();
	~DataBuff();

	DataBuff(const DataBuff&);
	DataBuff& operator=(const DataBuff&);

	DataBuff_t* ptr();
	std::size_t size();

private:
	void deinit();
	DataBuff& copy(const DataBuff&);
	void take(DataBuff_t *data, std::size_t size);

private:
	DataBuff_t *buff_;
	std::size_t size_;
	int *_refCount;
};



#endif