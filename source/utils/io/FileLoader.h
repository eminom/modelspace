

#ifndef _FILELOADER__
#define _FILELOADER__

#include "databuff.h"

class FileLoader
{
public:
	static bool load(const char *path, DataBuff &output);

};





#endif