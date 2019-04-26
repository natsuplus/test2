#pragma once

#include "main.h"

namespace textureLoaderNS
{
	enum TEXTURE_NUMBER {
//êºêÏ 0.22
		STAR,
		BLACK_HOLE,
		WHITE_HOLE,
		COMET,
		PLANET,

		BACK_GROUND,
		MAX_TEXTURE,
	};
}

class TextureLoader
{
private:
	const char* textureName[textureLoaderNS::MAX_TEXTURE];
	LPDIRECT3DTEXTURE9 g_pD3DTexture[textureLoaderNS::MAX_TEXTURE];

public:
	TextureLoader();
	~TextureLoader();

	void load(LPDIRECT3DDEVICE9 pDevice);
	void release();

	LPDIRECT3DTEXTURE9* getTexture(int i) { return &g_pD3DTexture[i]; }
};

TextureLoader* getTextureLoader();
LPDIRECT3DTEXTURE9* getTexture(int i);
