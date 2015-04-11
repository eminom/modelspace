

#include "fileloader.h"
#include <cstdlib>
#include <cstdio>
#include <cassert>

bool FileLoader::load(const char *path, DataBuff &output)
{
	FILE *file = fopen(path, "rb");
	if (!file) {
		fprintf(stderr, "Failed to open target resource.\n");
		return false;
	}

	fseek(file, 0, SEEK_END);
	long sz = ftell(file);
	fseek(file,0, SEEK_SET);

	char *data = (char*)malloc(sizeof(unsigned char)*sz);
	long read = fread(data, 1, sz, file);
	assert( read == sz);
	fclose(file);

	output.take(data, read);
	return true;
}