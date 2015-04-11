

#include "PngImageData.h"
#include "utils/io/FileLoader.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "png.h"


namespace
{
    typedef struct 
    {
        const unsigned char * data;
        std::size_t size;
        int offset;
    }tImageSource;
    
    static void pngReadCallback(png_structp png_ptr, png_bytep data, png_size_t length)
    {
        tImageSource* isource = (tImageSource*)png_get_io_ptr(png_ptr);
      
        if((int)(isource->offset + length) <= isource->size)
        {
            memcpy(data, isource->data+isource->offset, length);
            isource->offset += length;
        }
        else
        {
            png_error(png_ptr, "pngReaderCallback failed");
        }
    }
}

PngImageData::PngImageData()
	:_width(0),
	_height(0),
	_hasPremultipliedAlpha(false),
	_renderFormat(PixelFormat::AUTO)
{

}

bool PngImageData::load(const char *path)
{
	DataBuff db;
	if(!FileLoader::load(path, db)){
		fprintf(stderr, "Failed to load some file\n");
		return false;
	}

	unsigned char *data = db.ptr();
	std::size_t dataLen = db.size();

	const int PNGSIGSIZE = 8;
    png_byte        header[PNGSIGSIZE]   = {0}; 
    png_structp     png_ptr     =   0;
    png_infop       info_ptr    = 0;

	    // check the data is png or not
    memcpy(header, data, PNGSIGSIZE);
    auto isZero = png_sig_cmp(header, 0, PNGSIGSIZE);
	if(isZero){
		fprintf(stderr, "Is not a png file!\n");
	}

	// init png_struct
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
	info_ptr = png_create_info_struct(png_ptr);

	tImageSource imageSource;
	imageSource.data    = (unsigned char*)data;
	imageSource.size    = dataLen;
	imageSource.offset  = 0;
	png_set_read_fn(png_ptr, &imageSource, pngReadCallback);

	// read png header info

	// read png file info
	png_read_info(png_ptr, info_ptr);

	_width = png_get_image_width(png_ptr, info_ptr);
	_height = png_get_image_height(png_ptr, info_ptr);
	png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	png_uint_32 color_type = png_get_color_type(png_ptr, info_ptr);

	//CCLOG("color type %u", color_type);

	// force palette images to be expanded to 24-bit RGB
	// it may include alpha channel
	if (color_type == PNG_COLOR_TYPE_PALETTE)
	{
		png_set_palette_to_rgb(png_ptr);
	}
	// low-bit-depth grayscale images are to be expanded to 8 bits
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	{
		bit_depth = 8;
		png_set_expand_gray_1_2_4_to_8(png_ptr);
	}
	// expand any tRNS chunk data into a full alpha channel
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
	}  
	// reduce images with 16-bit samples to 8 bits
	if (bit_depth == 16)
	{
		png_set_strip_16(png_ptr);            
	} 

	// Expanded earlier for grayscale, now take care of palette and rgb
	if (bit_depth < 8)
	{
		png_set_packing(png_ptr);
	}
	// update info
	png_read_update_info(png_ptr, info_ptr);
	bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	color_type = png_get_color_type(png_ptr, info_ptr);

	_renderFormat = PixelFormat::AUTO;
	switch (color_type)
	{
	case PNG_COLOR_TYPE_GRAY:
		_renderFormat = PixelFormat::I8;
		break;
	case PNG_COLOR_TYPE_GRAY_ALPHA:
		_renderFormat = PixelFormat::AI88;
		break;
	case PNG_COLOR_TYPE_RGB:
		_renderFormat = PixelFormat::RGB888;
		break;
	case PNG_COLOR_TYPE_RGB_ALPHA:
		_renderFormat = PixelFormat::RGBA8888;
		break;
	default:
		break;
	}

	// read png data
	png_size_t rowbytes;
	png_bytep* row_pointers = (png_bytep*)malloc( sizeof(png_bytep) * _height );

	rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	//assert(! _dataLen );
	//assert(! _data);
	data_ = DataBuff(rowbytes * _height);
	data = data_.ptr();
	for (unsigned short i = 0; i < _height; ++i){
		row_pointers[i] = data + i*rowbytes;
	}
	png_read_image(png_ptr, row_pointers);

	png_read_end(png_ptr, nullptr);

	// premultiplied alpha for RGBA8888
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)	{
		premultipliedAlpha();
	}else{
		_hasPremultipliedAlpha = false;
	}

	if (row_pointers != nullptr){
		free(row_pointers);
	}

	if (png_ptr) {
		png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
	}

	return true;
}


void PngImageData::premultipliedAlpha()
{
	assert( PixelFormat::RGBA8888 == _renderFormat);
    
    unsigned int* fourBytes = (unsigned int*)data_.ptr();
    for(int i = 0; i < _width * _height; i++)
    {
        unsigned char* p = data_.ptr() + i * 4;
        fourBytes[i] = CC_RGB_PREMULTIPLY_ALPHA(p[0], p[1], p[2], p[3]);
    }
    
    _hasPremultipliedAlpha = true;
}

int PngImageData::getWidth()
{
	return _width;
}

int PngImageData::getHeight()
{
	return _height;
}

PixelFormat PngImageData::getRenderFormat()
{
	return _renderFormat;
}