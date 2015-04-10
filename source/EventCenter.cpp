
#include "EventCenter.h"


static EventCenter* _instance = nullptr;

EventCenter::EventCenter()
{

}

EventCenter* EventCenter::instance()
{
	if(!_instance)
	{
		_instance = new EventCenter();
	}
	return _instance;
}