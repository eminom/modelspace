

#ifndef _TEST_CASE_DEF__
#define _TEST_CASE_DEF__

#ifdef __APPLE__
#define _ResPath    "resource/Mario.png"
#else
#define _ResPath    "../resource/Mario.png"
#endif


#define _TestParticleTex()\
	{\
		PngImageData data;\
		data.load(_ResPath);\
		TextureData tex;\
		tex.initWithPngData(data);\
		ParticleTex *pt = ParticleTex::create();\
		pt->setTexture(tex.texture());\
		root->addSub(pt);\
	}


#define _TestFlagSprite()\
	{\
		FlagSprite *fs = FlagSprite::create(_ResPath);\
		root->addSub(fs);\
	}


#endif