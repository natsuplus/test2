#pragma once
#include "main.h"
#include <string>

struct TextDX
{
	COLOR_ARGB color; // フォントカラー(a,r,g,b)
	LPD3DXFONT dxFont; // フォントポインタ
	RECT fontRect; // テキスト矩形
	// テキストの回転行列
	D3DXMATRIX matrix;
	float angle; // テキストの回転角度（ラジアン角）
	
};


void initializeTextDX();
void unInitializeTextDX();

bool createTextDX(TextDX* text,int height, bool bold,bool italic,const std::string &fontName);
int printTextDX(TextDX* text, const std::string &str, int x, int y);
int printTextDX(TextDX* text, const std::string &str, int x, int y, float value);
int printTextDX(TextDX* text, const std::string &str, int x, int y, int value);
TextDX* getDebugFont();

int txtLineReset(int newtop);//西川 0.02
int txtLineBreak();//西川 0.02
