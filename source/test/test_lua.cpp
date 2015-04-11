
#include <stdio.h>
#include <stdlib.h>
#include <cassert>

extern "C"{
#include "lauxlib.h"
#include "lualib.h"
#include "lua.h"

}

#include <functional>
#include "utils/signalsinc.hpp"

#include "png.h"
#include "utils/io/FileLoader.h"

//namespace cocos2d{
enum class PixelFormat
{
	//! auto detect the type
	AUTO,
	//! 32-bit texture: BGRA8888
	BGRA8888,
	//! 32-bit texture: RGBA8888
	RGBA8888,
	//! 24-bit texture: RGBA888
	RGB888,
	//! 16-bit texture without Alpha channel
	RGB565,
	//! 8-bit textures used as masks
	A8,
	//! 8-bit intensity texture
	I8,
	//! 16-bit textures used as masks
	AI88,
	//! 16-bit textures: RGBA4444
	RGBA4444,
	//! 16-bit textures: RGB5A1
	RGB5A1,
	//! 4-bit PVRTC-compressed texture: PVRTC4
	PVRTC4,
	//! 4-bit PVRTC-compressed texture: PVRTC4 (has alpha channel)
	PVRTC4A,
	//! 2-bit PVRTC-compressed texture: PVRTC2
	PVRTC2,
	//! 2-bit PVRTC-compressed texture: PVRTC2 (has alpha channel)
	PVRTC2A,
	//! ETC-compressed texture: ETC
	ETC,
	//! S3TC-compressed texture: S3TC_Dxt1
	S3TC_DXT1,
	//! S3TC-compressed texture: S3TC_Dxt3
	S3TC_DXT3,
	//! S3TC-compressed texture: S3TC_Dxt5
	S3TC_DXT5,
	//! ATITC-compressed texture: ATC_RGB
	ATC_RGB,
	//! ATITC-compressed texture: ATC_EXPLICIT_ALPHA
	ATC_EXPLICIT_ALPHA,
	//! ATITC-compresed texture: ATC_INTERPOLATED_ALPHA
	ATC_INTERPOLATED_ALPHA,
	//! Default texture format: AUTO
	DEFAULT = AUTO,

	NONE = -1
};

//}//end of cocos2d

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


int callFunc(lua_State *L)
{
	lua_getglobal(L, "f");
	lua_pushinteger(L,3);
	lua_pushinteger(L,7);
	if(lua_pcall(L,2,1,0)!=0){
		printf("something not right.\n");
		//abort
	}

	if(!lua_isnumber(L,-1)){
		printf("not a number!\n");
		//abort
	}

	int z = lua_tonumber(L,-1);
	lua_pop(L,1);
	return z;
}


void onDo3(unsigned a, int b, char c)
{
	printf("onDo3 %u, %d, %c\n", a,b,c);
}

void onDo2(int a, int b)
{
	printf("onDo2(%d,%d)\n", a,b);
}

void onDo1(int a)
{
	printf("onDo1 : %d\n", a);
}



DD::Signal<void(int)> s;
DD::SlotHandle one;

void s1(int a)
{
	printf("s1:%d\n",a);
}

void s2(int a)
{
	printf("s2:%d\n", a);
}

void s3(int a)
{
	printf("s3:%d\n", a);
	if(one){
		s.disconnect(one);
		one = nullptr;
	}
}

void test3()
{
	DataBuff db;
	if(!FileLoader::load("../resource/Mario.png", db)){
		fprintf(stderr, "Failed to load some file\n");
		return;
	}

	char *data = db.ptr();
	std::size_t dataLen = db.size();
	int _width = 0;
	int _height= 0;
	int _dataLen = 0;
	unsigned char* _data = 0;
	bool _hasPremultipliedAlpha = true;


#define PNGSIGSIZE  8
    //bool ret = false;
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

	PixelFormat _renderFormat = PixelFormat::AUTO;
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

	_dataLen = rowbytes * _height;
	_data = static_cast<unsigned char*>(malloc(_dataLen * sizeof(unsigned char)));
	if (!_data)
	{
		if (row_pointers != nullptr)
		{
			free(row_pointers);
		}
		fprintf(stderr, "Error allocating memories\n");
		return;
	}

	for (unsigned short i = 0; i < _height; ++i)
	{
		row_pointers[i] = _data + i*rowbytes;
	}
	png_read_image(png_ptr, row_pointers);

	png_read_end(png_ptr, nullptr);

	// premultiplied alpha for RGBA8888
	if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		//premultipliedAlpha();
		_hasPremultipliedAlpha = true;
	}
	else
	{
		_hasPremultipliedAlpha = false;
	}

	if (row_pointers != nullptr)
	{
		free(row_pointers);
	}

	if (png_ptr)
	{
		png_destroy_read_struct(&png_ptr, (info_ptr) ? &info_ptr : 0, 0);
	}
}


void test2()
{
	//DD::Signal<void(int,int)> s;
	
	//auto k = std::bind(&onDo,_1,_2);
	////k(1,2);
	//s.connect(std::bind(&onDo,_1,_2));
	//s.trigger(20,30);

	using namespace std::placeholders;

	do{
		s.connect(std::bind(&s1,_1), 1);
		one = s.connect(std::bind(&s2,_1));
		s.connect(std::bind(&s3,_1));
		printf("############################\n\n");
		s.trigger(101);
		printf("############################\n\n");
		s.trigger(101);
		printf("############################\n\n");
	}while(0);

	do{
		DD::Signal<void(int)> s;
		s.connect(std::bind(&onDo1,_1));
		s.trigger(200);
	}while(0);

	do{
		DD::Signal<void(int)> s;
		s.connect(std::bind(&onDo2, 200,_1));
		s.trigger(250);
	}while(0);

	do 
	{
		DD::Signal<void(unsigned,int,char)> s;
		s.connect(std::bind(&onDo3, _1,_2,_3));
		s.trigger(255, 100, 'c');
	}while(0);
}

int mainZ()
{
	lua_State *L = luaL_newstate();
	luaL_openlibs(L);

	char *buf = "function f(x,y) return x^2 + y; end";
	int error = luaL_loadstring(L,buf);
	lua_pcall(L,0,0,0);
	int z = callFunc(L);

	lua_close(L);
	return 0;
}
