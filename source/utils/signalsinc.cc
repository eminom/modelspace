
#include <cstdio>

namespace DD{
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

}