#include "textDX.h"

TextDX debugFont;

struct {
	const int m_TOP = 100, m_SIZE = 25;//開始位置/行間隔
	int m_y = m_TOP;
}TxtLine;//西川 0.02

void initializeTextDX()
{
	createTextDX(&debugFont, 24, false, false, "Arial");
}
void unInitializeTextDX() 
{
	safeRelease(debugFont.dxFont);
};

bool createTextDX(TextDX* text,int height,bool bold,bool italic,const std::string &fontName ) {
	text->color = SETCOLOR_ARGB(255, 255, 255, 255); // 白色でデフォルト

	// フォント位置を設定する
	text->fontRect.top = 0;
	text->fontRect.left = 0;
	text->fontRect.right = WINDOW_WIDTH;
	text->fontRect.bottom= WINDOW_HEIGHT;
	text->dxFont = NULL;
	text->angle = 0;

	UINT weight = FW_NORMAL;
	if (bold)
	{
		weight = FW_BOLD;
	}

	// DirectXフォント作成
	if (FAILED(D3DXCreateFont(getDevice(), height, 0, weight, 1, italic,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, fontName.c_str(), &text->dxFont)))
	{
		return false;
	}

	// 変換行列を作成する
	D3DXMatrixTransformation2D(&text->matrix, NULL, 0.0f, NULL, NULL, 0.0f, NULL);
	return true;
};

int printTextDX(TextDX* text,const std::string &str, int x,int y) {
	if (text->dxFont == NULL)
	{
		return 0;
	}

	// フォント位置を設定
	text->fontRect.top = y;
	text->fontRect.left = x;
	
	// 回転中心
	D3DXVECTOR2 rCenter = D3DXVECTOR2((float)x, (float)y);

	// テキストを特定の角度で回転する行列を準備
	D3DXMatrixTransformation2D(&text->matrix, NULL, 0.0f, NULL, &rCenter, text->angle, NULL);

	// 行列をスプライトに適用
	getSprite()->SetTransform(&text->matrix);
	return text->dxFont->DrawTextA(getSprite(), str.c_str(), -1, &text->fontRect, DT_LEFT, text->color);
};


int printTextDX(TextDX* text,const std::string &str, int x, int y, float value)
{
	const int BUF_SIZE = 255;
	static char buffer[BUF_SIZE];
	// 引数floatをCstringに変換する
	_snprintf_s(buffer, BUF_SIZE, "%f", value);
	return printTextDX(text,str + buffer, x, y);
}
int printTextDX(TextDX* text,const std::string &str, int x, int y, int value)
{
	const int BUF_SIZE = 255;
	static char buffer[BUF_SIZE];
	// 引数floatをCstringに変換する
	_snprintf_s(buffer, BUF_SIZE, "%d", value);
	return printTextDX(text,str + buffer, x, y);
}

TextDX* getDebugFont()
{
	return &debugFont;
}

int txtLineReset(int newtop) {//西川 0.02
	TxtLine.m_y = newtop;//行位置をセット(毎フレーム最初)

	return TxtLine.m_TOP;//既定の初期値を取得
//	txtLineReset(txtLineReset(0));//と書くと初期値でリセットされる
}

int txtLineBreak() {//西川 0.02
	TxtLine.m_y += TxtLine.m_SIZE;//改行
	return TxtLine.m_y - TxtLine.m_SIZE;
}