#include "image.h"

void InitImage(Image* image, LPDIRECT3DTEXTURE9* texture,
	float x, float y, float width, float height)
{
	LPDIRECT3DDEVICE9 pDevice = getDevice();
	image->g_pD3DTexture = *texture;
	image->position = D3DXVECTOR3(x, y, 0.0f);
	image->width = width;
	image->height = height;
	image->color[0] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	image->color[1] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	image->color[2] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	image->color[3] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	image->playAnime = false;
	image->g_nCountAnime = 0;
	image->g_nPatternAnime = 0;
	image->ANIME_TIME = 1;
	image->ANIME_PATTERN = 1;
	image->DIVIDE_U = 1;
	image->DIVIDE_V = 1;
	image->renderFlag = true;
	// 頂点情報の作成
	MakeVertex(image, pDevice);
}

void InitAnimeImage(Image* image, LPDIRECT3DTEXTURE9* texture,
	float x, float y, float width, float height,
	int time, int pattern, int divide_u, int divide_v)
{
	InitImage(image, texture, x, y, width, height);//標準初期化
	image->playAnime = true;
	image->g_nCountAnime = 0;
	image->g_nPatternAnime = 0;
	image->ANIME_TIME = time;
	if (pattern == 0) { image->ANIME_PATTERN = 1; }
	else { image->ANIME_PATTERN = pattern; }
	image->DIVIDE_U = divide_u;
	image->DIVIDE_V = divide_v;
}

void UninitImage(Image* image)
{
	if (image->g_pD3DTexture != NULL)
	{// テクスチャの開放
		image->g_pD3DTexture->Release();
		image->g_pD3DTexture = NULL;
	}
	if (image->g_pD3DVtxBuffer != NULL)
	{// テクスチャの開放
		image->g_pD3DVtxBuffer->Release();
		image->g_pD3DVtxBuffer = NULL;
	}
}

void DrawImage(Image* image)
{
	if (!image->renderFlag)return;//描画フラグがtrueでない場合、描画しない
	LPDIRECT3DDEVICE9 pDevice = getDevice();

	if (image->playAnime) {
		// アニメーション
		image->g_nCountAnime++;

		// アニメーションWaitチェック
		if ((image->g_nCountAnime % image->ANIME_TIME) == 0)
		{
			// パターンの切り替え
			image->g_nPatternAnime = (image->g_nPatternAnime + 1) % image->ANIME_PATTERN;
			SetTexture(image);
		}
	}
	// レンダリングモードの設定
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, image->g_pD3DVtxBuffer, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, image->g_pD3DTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, NUM_POLYGON);
}


void setSize(Image* image, float _width, float _height) {
	image->width = _width;
	image->height = _height;

	float x = image->position.x;
	float y = image->position.y;
	float width = image->width;
	float height = image->height;
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		image->g_pD3DVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(x, y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(x + width, y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(x, y + height, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(x + width, y + height, 0.0f);

		// 頂点データをアンロックする
		image->g_pD3DVtxBuffer->Unlock();
	}
};

void setPosition(Image* image, float _x, float _y)
{
	image->position.x = _x;
	image->position.y = _y;
	float x = image->position.x;
	float y = image->position.y;
	float width = image->width;
	float height = image->height;
	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		image->g_pD3DVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(x, y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(x + width, y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(x, y + height, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(x + width, y + height, 0.0f);

		// 頂点データをアンロックする
		image->g_pD3DVtxBuffer->Unlock();
	}
};

// Imageのカラーを変更する
void SetColorImage(Image* image, D3DXCOLOR color)
{
	image->color[0] = color;
	image->color[1] = color;
	image->color[2] = color;
	image->color[3] = color;

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		image->g_pD3DVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// 反射光の設定
		pVtx[0].diffuse = image->color[0];
		pVtx[1].diffuse = image->color[1];
		pVtx[2].diffuse = image->color[2];
		pVtx[3].diffuse = image->color[3];

		// 頂点データをアンロックする
		image->g_pD3DVtxBuffer->Unlock();
	}

}



void SetTexture(Image* image)
{
	// テクスチャ座標の設定
	int x = image->g_nPatternAnime % image->DIVIDE_U;
	int y = image->g_nPatternAnime / image->DIVIDE_U;
	float sizeX = 1.0f / image->DIVIDE_U;
	float sizeY = 1.0f / image->DIVIDE_V;

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		image->g_pD3DVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		image->g_pD3DVtxBuffer->Unlock();
	}
}

void SetTexture(Image* image, float ratioU, float ratioV)
{
	// テクスチャ座標の設定
	int x = 0;
	int y = 0;
	float sizeX = ratioU;
	float sizeY = ratioV;

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		image->g_pD3DVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		pVtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		pVtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		pVtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);

		// 頂点データをアンロックする
		image->g_pD3DVtxBuffer->Unlock();
	}
}

HRESULT MakeVertex(Image* image, LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * NUM_VERTEX,	// 頂点データ用に確保するバッファサイズ(バイト単位)
		D3DUSAGE_WRITEONLY,			// 頂点バッファの使用法　
		FVF_VERTEX_2D,				// 使用する頂点フォーマット
		D3DPOOL_MANAGED,			// リソースのバッファを保持するメモリクラスを指定
		&image->g_pD3DVtxBuffer,		// 頂点バッファインターフェースへのポインタ
		NULL)))						// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_2D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		image->g_pD3DVtxBuffer->Lock(0, 0, (void**)&pVtx, 0);

		float x = image->position.x;
		float y = image->position.y;
		float width = image->width;
		float height = image->height;


		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(x, y, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(x + width, y, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(x, y + height, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(x + width, y + height, 0.0f);

		// テクスチャのパースペクティブコレクト用
		pVtx[0].rhw =
			pVtx[1].rhw =
			pVtx[2].rhw =
			pVtx[3].rhw = 1.0f;

		// 反射光の設定
		pVtx[0].diffuse = image->color[0];
		pVtx[1].diffuse = image->color[1];
		pVtx[2].diffuse = image->color[2];
		pVtx[3].diffuse = image->color[3];

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 頂点データをアンロックする
		image->g_pD3DVtxBuffer->Unlock();
	}

	return S_OK;
}
