
#ifdef _DEBUGX
#define _TRACER_ADD()\
	DD::incSlotCount();

#define _TRACER_SUB()\
	DD::decSlotCount();

#else

#define _TRACER_ADD()
#define _TRACER_SUB()

#endif