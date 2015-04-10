

#ifndef _EVENT_CENTER__DEF__
#define _EVENT_CENTER__DEF__

#include "utils/signalsinc.hpp"

class EventCenter
{
private:
	EventCenter();

public:
	static EventCenter* instance();

public:
	DD::Signal<void()> Go;
};

#endif