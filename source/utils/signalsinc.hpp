
#ifndef _SIGNAL_INC_DEF__
#define _SIGNAL_INC_DEF__

#include "signals_conf.h"

namespace DD
{
	void incSlotCount();
	void decSlotCount();

	template<class T>
	class Slot;

	template<class T,class U>
	class FunctorSlot;

	template<class T>
	class Signal;


	typedef void* SlotHandle;
};
//And do some template partialization
//Four
#define ARG_SEP				,
#define ARG_SIGT			typename P1, typename P2, typename P3, typename P4
#define ARG_SIGV			P1 p1, P2 p2, P3 p3, P4 p4
#define ARG_SIGB			P1, P2, P3, P4
#define ARG_SIGS			p1, p2, p3, p4
#include "signals.hpp"
#undef ARG_SIGS
#undef ARG_SIGB
#undef ARG_SIGV
#undef ARG_SIGT
#undef ARG_SEP


//Three
#define ARG_SEP				,
#define ARG_SIGT			typename P1, typename P2, typename P3
#define ARG_SIGV			P1 p1, P2 p2, P3 p3
#define ARG_SIGB			P1, P2, P3
#define ARG_SIGS			p1, p2, p3
#include "signals.hpp"
#undef ARG_SIGS
#undef ARG_SIGB
#undef ARG_SIGV
#undef ARG_SIGT
#undef ARG_SEP
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
//Zero
#define ARG_SEP		
#define ARG_SIGT	
#define ARG_SIGV
#define ARG_SIGB
#define ARG_SIGS
#include "signals.hpp"
#undef ARG_SIGS
#undef ARG_SIGB
#undef ARG_SIGV
#undef ARG_SIGT
#undef ARG_SEP
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
