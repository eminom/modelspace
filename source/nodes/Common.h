

#ifndef _COMMON_DEF__
#define _COMMON_DEF__

#define CREATE_FUNC(className)\
	static className* create(){\
		auto one = new className;\
		one->init();\
		return one;\
	}

#endif