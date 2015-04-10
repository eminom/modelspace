
#include <cstdio>

namespace DD{
	static int gCount = 0;
	void incSlotCount()
	{
		gCount++;
		printf("Slots up to %d\n", gCount);
	}

	void decSlotCount()
	{
		gCount--;
		printf("Slots down to %d\n", gCount);
	}

}