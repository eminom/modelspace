

#include "databuff.h"
#include <cstdlib>
#include <cassert>

#define _ZERO_INITLIST()\
	:buff_(nullptr), size_(0), _refCount(nullptr)

DataBuff::DataBuff()
	_ZERO_INITLIST()
{

}

DataBuff::~DataBuff() {
	deinit();
}

void DataBuff::take(char *data, std::size_t size) {
	assert(!buff_);
	assert(!size_);
	assert(!_refCount);
	buff_ = data;
	size_ = size;
	_refCount = new int(1);
}

void DataBuff::deinit() {
	if(_refCount) {
		--(*_refCount);
		if( 0 == *_refCount){
			free(buff_);
			delete _refCount;
		}
	}
	buff_ = 0;
	_refCount = 0;
	size_ = 0;
}

DataBuff& DataBuff::copy(const DataBuff &rhs){
	deinit();
	if(rhs._refCount){
		_refCount = rhs._refCount;
		++(*_refCount);
		buff_ = rhs.buff_;
		size_ = rhs.size_;
	}
	return *this;
}

DataBuff::DataBuff(const DataBuff&rhs)
	_ZERO_INITLIST()
{
	copy(rhs);
}

DataBuff& DataBuff::operator=(const DataBuff&rhs)
{
	return copy(rhs);
}

char* DataBuff::ptr()
{
	return buff_;
}

std::size_t DataBuff::size()
{
	return size_;
}