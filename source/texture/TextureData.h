

#ifndef _TEXTURE_DATA__DEF___
#define _TEXTURE_DATA__DEF___

#include "PngImageData.h"
#include "utils/gl_object_wrappers.hpp"
#include "PngImageData.h"

class TextureData
{
public:
	TextureData();
	bool initWithPngData(PngImageData &image);
	DD::Texture &texture();

private:
	bool initWithData(const void *,std::size_t,PixelFormat,int,int);
	bool initWithMipmaps(MipmapInfo*, int, PixelFormat, int ,int);

private:
	DD::Texture _tex;
};

#endif