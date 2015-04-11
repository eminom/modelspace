

#ifndef _PNG_IMAGEDATA_DEF__
#define _PNG_IMAGEDATA_DEF__

#include "ImageData.h"


class PngImageData:public ImageData
{
public:
	PngImageData();
	virtual bool load(const char *path)override;


private:
	void premultipliedAlpha();

private:
	int _width;
	int _height;
	bool _hasPremultipliedAlpha;
	PixelFormat _renderFormat;
};

#endif