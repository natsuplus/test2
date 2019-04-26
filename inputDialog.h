#pragma once
#include "main.h"
#include <string>
#include "textDX.h"
#include "input.h"
#include "messageDialog.h"

namespace inputDialogNS
{
	const COLOR_ARGB TEXT_BACK_COLOR = colorNS::BLACK; // 入力テキストの背景
	const COLOR_ARGB TEXT_COLOR = colorNS::WHITE; // 入力テキストの色
}

class InputDialog:public MessageDialog
{
private:
		
	std::string inText; // 入力テキスト
	RECT inTextRect;// テキスト矩形
	RECT tempRect;
	COLOR_ARGB textBackColor;//テキスト領域の背景の色
	COLOR_ARGB textFontColor;//テキスト領域のフォントの色
	LPDIRECT3DVERTEXBUFFER9 inTextVerts;//テキスト領域の頂点バッファ

public:
	InputDialog();
	virtual ~InputDialog();

	// 頂点バッファを準備
	void prepareVerts();

	//InputDialogを表示
	const void draw();

	//入力テキストを戻す
	std::string getText()
	{
		if (!visible)
		{
			return inText;
		}
		else
		{
			return "";
		}
	}

	// 入力テキストのフォントの色を設定
	void setTextFontColor(COLOR_ARGB fc) { textFontColor = fc; }
	// 入力テキストの背景の色を設定
	void setTextBackColor(COLOR_ARGB bc) { textBackColor = bc; }

	// テキストstrをInputDialogに表示
	void print(const std::string &str);

	// 「閉じる」イベントをチェック
	void update();

	// グラフィックスデバイスが消失したときに呼び出す
	void onLostDevice();
};