#pragma once

#include <Windows.h>
#include <WindowsX.h>
#include <crtdbg.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "audio.h"

#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"

//*****************************************************************************
// ライブラリのリンク
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "winmm.lib")

//*****************************************************************************
// マクロ定義
//*****************************************************************************
// ２Ｄポリゴン頂点フォーマット( 頂点座標[2D] / 反射光 / テクスチャ座標 )
#define	FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	NUM_VERTEX		(4)		// 頂点数
#define	NUM_POLYGON		(2)		// ポリゴン数
// 上記２Ｄポリゴン頂点フォーマットに合わせた構造体を定義
typedef struct
{
	D3DXVECTOR3 vtx;		// 頂点座標
	float rhw;				// テクスチャのパースペクティブコレクト用
	D3DCOLOR diffuse;		// 反射光
	D3DXVECTOR2 tex;		// テクスチャ座標
} VERTEX_2D;

struct VertexC				// 頂点カラー
{
	float x, y, z;			// 頂点位置
	float rhw;				// 同次座標のｗ成分の逆数（1に設定）
	unsigned long color;	// 頂点カラー
};

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define WINDOW_CENTER_X	(WINDOW_WIDTH / 2)	// ウインドウの中心Ｘ座標
#define WINDOW_CENTER_Y	(WINDOW_HEIGHT / 2)	// ウインドウの中心Ｙ座標

const float FRAME_RATE = 100.0f;						//ターゲットフレームレート（フレーム/秒）
const float MIN_FRAME_RATE = 30.0f;						// 最小フレームレート
const float	MIN_FRAME_TIME = 1.0f / FRAME_RATE;			// 1フレームの最小必要時間
const float MAX_FRAME_TIME = 1.0f / MIN_FRAME_RATE;		// 計算に使用される最大時間

#define VECTOR2 D3DXVECTOR2
#define VECTOR3 D3DXVECTOR3
#define LP_DXFONT	LPD3DXFONT

//フレキシブル頂点フォーマットコード
// D3DFVF_XYZRHW = 頂点が変換されます。
// D3DFVF_DIFFUSE = 頂点には拡散色データが含まれています
#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

// 色の定義
#define COLOR_ARGB DWORD
#define SETCOLOR_ARGB(a,r,g,b) \
	((COLOR_ARGB)(((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff))
namespace colorNS
{
	// 共通で使用する色
	// ARGBの数値の範囲は0?255
	// A =アルファチャネル（透明度、255は不透明）
	// R = 赤, G = 緑, B = 青
	const COLOR_ARGB ORANGE = D3DCOLOR_ARGB(255, 255, 165, 0);
	const COLOR_ARGB BROWN = D3DCOLOR_ARGB(255, 139, 69, 19);
	const COLOR_ARGB LTGRAY = D3DCOLOR_ARGB(255, 192, 192, 192);
	const COLOR_ARGB GRAY = D3DCOLOR_ARGB(255, 128, 128, 128);
	const COLOR_ARGB OLIVE = D3DCOLOR_ARGB(255, 128, 128, 0);
	const COLOR_ARGB PURPLE = D3DCOLOR_ARGB(255, 128, 0, 128);
	const COLOR_ARGB MAROON = D3DCOLOR_ARGB(255, 128, 0, 0);
	const COLOR_ARGB TEAL = D3DCOLOR_ARGB(255, 0, 128, 128);
	const COLOR_ARGB GREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	const COLOR_ARGB NAVY = D3DCOLOR_ARGB(255, 0, 0, 128);
	const COLOR_ARGB WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	const COLOR_ARGB YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	const COLOR_ARGB MAGENTA = D3DCOLOR_ARGB(255, 255, 0, 255);
	const COLOR_ARGB RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	const COLOR_ARGB CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	const COLOR_ARGB LIME = D3DCOLOR_ARGB(255, 0, 255, 0);
	const COLOR_ARGB BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	const COLOR_ARGB BLACK = D3DCOLOR_ARGB(255, 0, 0, 0);
	const COLOR_ARGB SKYBLUE = D3DCOLOR_ARGB(255, 0, 204, 255);
	const COLOR_ARGB WATER = D3DCOLOR_ARGB(255, 0, 255, 255);
}




//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LPDIRECT3DDEVICE9 getDevice(void);
LPD3DXSPRITE getSprite(void);
HWND getHWnd();
Audio* getAudio();
bool drawQuad(LPDIRECT3DVERTEXBUFFER9 vertexBuffer);
HRESULT createVertexBuffer(VertexC verts[], UINT size, LPDIRECT3DVERTEXBUFFER9 &vertexBuffer);
//=============================================================================
// ポインタ参照項目を安全に処理するための関数テンプレート。
// これらのテンプレートによって定義された関数は、
// 通常の関数呼び出し構文を使用して呼び出すことができます。
// コンパイラは、Tを呼び出すパラメータの型に置き換える関数を作成します。
//=============================================================================
// ポインタ参照されるアイテムを安全に解放
template<typename T>
inline void safeRelease(T& ptr)
{
	if (ptr)
	{
		ptr->Release();
		ptr = NULL;
	}
}
#define SAFE_RELEASE safeRelease //後方互換性のために
