#include "TextureLoader.h"

TextureLoader textureLoader;
TextureLoader* getTextureLoader()
{
	return &textureLoader;
}
LPDIRECT3DTEXTURE9* getTexture(int i)
{
	return textureLoader.getTexture(i);
}

TextureLoader::TextureLoader()
{
//西川 0.22
	textureName[textureLoaderNS::STAR] = { "TEXTURE/star.png" };
	textureName[textureLoaderNS::BLACK_HOLE] = { "TEXTURE/blackHole.png" };
	textureName[textureLoaderNS::WHITE_HOLE] = { "TEXTURE/whiteHole.png" };
	textureName[textureLoaderNS::COMET] = { "TEXTURE/comet.jpg" };
	textureName[textureLoaderNS::PLANET] = { "TEXTURE/planet.jpg" };

	textureName[textureLoaderNS::BACK_GROUND] = { "TEXTURE/gameBG.png" };
}


TextureLoader::~TextureLoader()
{
	release();
}


void TextureLoader::load(LPDIRECT3DDEVICE9 pDevice)
{
	for (int i = 0; i < textureLoaderNS::MAX_TEXTURE; i++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,	// デバイスへのポインタ
			textureName[i],		// ファイルの名前
			&g_pD3DTexture[i]);	// 読み込むメモリー
	}
}

void TextureLoader::release()
{
	for (int i = 0; i < textureLoaderNS::MAX_TEXTURE; i++) {
		if (g_pD3DTexture[i] != NULL)
		{// テクスチャの開放
			g_pD3DTexture[i]->Release();
			g_pD3DTexture[i] = NULL;
		}
	}
}
