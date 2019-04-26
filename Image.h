#pragma once

#include "main.h"

typedef struct {
	LPDIRECT3DTEXTURE9		g_pD3DTexture = NULL;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffer = NULL;		// 頂点バッファインターフェースへのポインタ
	D3DXCOLOR color[4]; // 画像の頂点カラー
	D3DXVECTOR3 position; // 画像の位置
	float width; // 幅
	float height; // 高さ
	bool playAnime; // アニメーション再生フラグ
	int	g_nCountAnime; // アニメーションカウント(更新フレーム数の保存)
	int	g_nPatternAnime;	// アニメーションパターンナンバー
	int ANIME_TIME; // アニメーション時間(フレーム数)
	int ANIME_PATTERN; // アニメーションパターン数
	int DIVIDE_U; // テクスチャU方向[横/x方向]分割数 
	int DIVIDE_V; // テクスチャV方向[縦/y方向]分割数
	bool renderFlag; // 描画フラグ
}Image;

// image: 初期化するImage構造体
// texture: Imageに使用するテクスチャポインタ
// x:画面上で配置するx位置
// y:画面上で配置するy位置
// width:Imageオブジェクトの幅
// height:Imageオブジェクトの高さ
void InitImage(Image* image, LPDIRECT3DTEXTURE9* texture,
	float x, float y, float width, float height);

// アニメーション付きImageの初期化
// image: 初期化するImage構造体
// texture: Imageに使用するテクスチャポインタ
// x:画面上で配置するx位置
// y:画面上で配置するy位置
// width:Imageオブジェクトの幅
// height:Imageオブジェクトの高さ
// time:１アニメーションあたりのフレーム数
// pattern:アニメーションのパターン数
// divide_u:横[U]方向の分割数
// divide_v:横[V]方向の分割数
void InitAnimeImage(Image* image, LPDIRECT3DTEXTURE9* texture,
	float x, float y, float width, float height,
	int time, int pattern, int divide_u, int divide_v);


void UninitImage(Image* image);
void DrawImage(Image* image);

// Imageのテクスチャ位置をセットする
void SetTexture(Image* image);
void SetTexture(Image* image, float ratioU, float ratioV);


// Imageの位置を変更する
void setPosition(Image* image, float _x, float _y);
// Imageのサイズを変更する
void setSize(Image* image, float _width, float _height);
// Imageのカラーを変更する
void SetColorImage(Image* image, D3DXCOLOR color);
HRESULT MakeVertex(Image* image, LPDIRECT3DDEVICE9 pDevice);

