

#ifndef _TEST_CASE_DEF__
#define _TEST_CASE_DEF__


#define _TestParticleTex()\
	{\
		PngImageData data;\
		data.load("../resource/Mario.png");\
		TextureData tex;\
		tex.initWithPngData(data);\
		ParticleTex *pt = ParticleTex::create();\
		pt->setTexture(tex.texture());\
		root->addSub(pt);\
	}


#define _TestFlagSprite()\
	{\
		FlagSprite *fs = FlagSprite::create("../resource/Mario.png");\
		root->addSub(fs);\
	}


#endif