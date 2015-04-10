
#ifndef _SIGNAL_INC_DEF__
#define _SIGNAL_INC_DEF__

namespace DD
{
	static int gCount = 0;
	void incSlotCount()
	{
		gCount++;
		printf("gCount up to %d\n", gCount);
	}

	void decSlotCount()
	{
		gCount--;
		printf("gCount down to %d\n", gCount);
	}

	template<class T>
	class Slot;

	template<class T,class U>
	class FunctorSlot;

	template<class T>
	class Signal;
};
//And do some template partialization

//
//Two
#define ARG_SEP				,
#define ARG_SIGT			typename P1, typename P2
#define ARG_SIGV			P1 p1, P2 p2
#define ARG_SIGB			P1, P2
#define ARG_SIGS			p1, p2
#include "signals.hpp"
#undef ARG_SIGS
#undef ARG_SIGB
#undef ARG_SIGV
#undef ARG_SIGT
#undef ARG_SEP
//
////Zero
//#define ARG_SEP		
//#define ARG_SIGT	
//#define ARG_SIGV
//#define ARG_SIGB
//#define ARG_SIGS
//#include "signals.hpp"
//#undef ARG_SIGS
//#undef ARG_SIGB
//#undef ARG_SIGV
//#undef ARG_SIGT
//#undef ARG_SEP
//
////One
#define ARG_SEP			,
#define ARG_SIGT		typename P1
#define ARG_SIGV		P1 p1
#define ARG_SIGB		P1
#define ARG_SIGS		p1
#include "signals.hpp"
#undef ARG_SIGS
#undef ARG_SIGB
#undef ARG_SIGV
#undef ARG_SIGT
#undef ARG_SEP

//


#endif
