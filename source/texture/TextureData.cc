

#include "TextureData.h"



TextureData::TextureData()
{

}

DD::Texture& TextureData::texture()
{
	return _tex;
}

bool TextureData::initWithPngData(PngImageData &image)
{
	int imageWidth = image.getWidth();
	int imageHeight = image.getHeight();

    //Configuration *conf = Configuration::getInstance();

    //int maxTextureSize = conf->getMaxTextureSize();
    //if (imageWidth > maxTextureSize || imageHeight > maxTextureSize) 
    //{
    //    CCLOG("cocos2d: WARNING: Image (%u x %u) is bigger than the supported %u x %u", imageWidth, imageHeight, maxTextureSize, maxTextureSize);
    //    return false;
    //}
	DataBuff &d = image.data();

    unsigned char*   tempData = d.ptr();
    //Size             imageSize = Size((float)imageWidth, (float)imageHeight);
    //PixelFormat      pixelFormat = ((PixelFormat::NONE == format) || (PixelFormat::AUTO == format)) ? image->getRenderFormat() : format;
    PixelFormat      renderFormat = image.getRenderFormat();
	PixelFormat pixelFormat = renderFormat;
    size_t	         tempDataLen = d.size();

    //pixelFormat = convertDataToFormat(tempData, tempDataLen, renderFormat, pixelFormat, &outTempData, &outTempDataLen);
    initWithData(tempData, tempDataLen, pixelFormat, imageWidth, imageHeight);//, imageSize);

    // set the premultiplied tag
    //_hasPremultipliedAlpha = image->hasPremultipliedAlpha();
      
    return true;
}

bool TextureData::initWithData(const void *data, std::size_t dataLen, PixelFormat pixelFormat, int pixelsWide, int pixelsHigh)
{
    //CCASSERT(dataLen>0 && pixelsWide>0 && pixelsHigh>0, "Invalid size");

    //if data has no mipmaps, we will consider it has only one mipmap
    MipmapInfo mipmap;
    mipmap.address = (unsigned char*)data;
    mipmap.len = static_cast<int>(dataLen);
    return initWithMipmaps(&mipmap, 1, pixelFormat, pixelsWide, pixelsHigh);
}

bool TextureData::initWithMipmaps(MipmapInfo* mipmaps, int mipmapsNum, PixelFormat pixelFormat, int pixelsWide, int pixelsHigh)
{
	   // //the pixelFormat must be a certain value 
    //CCASSERT(pixelFormat != PixelFormat::NONE && pixelFormat != PixelFormat::AUTO, "the \"pixelFormat\" param must be a certain value!");
    //CCASSERT(pixelsWide>0 && pixelsHigh>0, "Invalid size");

    //if (mipmapsNum <= 0)
    //{
    //    CCLOG("cocos2d: WARNING: mipmap number is less than 1");
    //    return false;
    //}
   

    //if(_pixelFormatInfoTables.find(pixelFormat) == _pixelFormatInfoTables.end())
    //{
    //    CCLOG("cocos2d: WARNING: unsupported pixelformat: %lx", (unsigned long)pixelFormat );
    //    return false;
    //}

    //const PixelFormatInfo& info = _pixelFormatInfoTables.at(pixelFormat);

    //if (info.compressed && !Configuration::getInstance()->supportsPVRTC()
    //                    && !Configuration::getInstance()->supportsETC()
    //                    && !Configuration::getInstance()->supportsS3TC()
    //                    && !Configuration::getInstance()->supportsATITC())
    //{
    //    CCLOG("cocos2d: WARNING: PVRTC/ETC images are not supported");
    //    return false;
    //}

    //Set the row align only when mipmapsNum == 1 and the data is uncompressed
    if(true) //if (mipmapsNum == 1/* && !info.compressed*/)
    {
        unsigned int bytesPerRow = pixelsWide * 32 / 8;

        if(bytesPerRow % 8 == 0)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 8);
        }
        else if(bytesPerRow % 4 == 0)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
        }
        else if(bytesPerRow % 2 == 0)
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
        }
        else
        {
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        }
    }else
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    }

 /*   if(_name != 0)
    {
        GL::deleteTexture(_name);
        _name = 0;
    }*/

	GLuint name = 0;
    glGenTextures(1, &name);
	glBindTexture(GL_TEXTURE_2D, name);
	_tex.take_and_own(name);

    if (mipmapsNum == 1)    {
        //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _antialiasEnabled ? GL_LINEAR : GL_NEAREST);
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }else   {
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _antialiasEnabled ? GL_LINEAR_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_NEAREST);
    }
    
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _antialiasEnabled ? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	

    //CHECK_GL_ERROR_DEBUG(); // clean possible GL error
    
    // Specify OpenGL texture image
    int width = pixelsWide;
    int height = pixelsHigh;
    
    //for (int i = 0; i < mipmapsNum; ++i)
    //{
        unsigned char *data = mipmaps[0].address;
        GLsizei datalen = mipmaps[0].len;

        //if (info.compressed)
        //{
        //    glCompressedTexImage2D(GL_TEXTURE_2D, i, info.internalFormat, (GLsizei)width, (GLsizei)height, 0, datalen, data);
        //}
        //else
        //{
        //    glTexImage2D(GL_TEXTURE_2D, i, info.internalFormat, (GLsizei)width, (GLsizei)height, 0, info.format, info.type, data);
        //}

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        //if (i > 0 && (width != height || ccNextPOT(width) != width ))
        //{
        //    CCLOG("cocos2d: Texture2D. WARNING. Mipmap level %u is not squared. Texture won't render correctly. width=%d != height=%d", i, width, height);
        //}

        GLenum err = glGetError();
        if (err != GL_NO_ERROR)
        {
            //CCLOG("cocos2d: Texture2D: Error uploading compressed texture level: %u . glError: 0x%04X", i, err);
            return false;
        }

        //width = MAX(width >> 1, 1);
        //height = MAX(height >> 1, 1);
    //}

    //_contentSize = Size((float)pixelsWide, (float)pixelsHigh);
    //_pixelsWide = pixelsWide;
    //_pixelsHigh = pixelsHigh;
    //_pixelFormat = pixelFormat;
    //_maxS = 1;
    //_maxT = 1;

    //_hasPremultipliedAlpha = false;
    //_hasMipmaps = mipmapsNum > 1;

    // shader
    //setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
    return true;
}
