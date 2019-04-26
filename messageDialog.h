#pragma once


#ifndef _MESSAGEDIALOG_H        // このファイルが複数の箇所でインクルードされる場合に、 
#define _MESSAGEDIALOG_H        // 多重に定義されることを防ぎます。
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "main.h"
#include "textDX.h"
#include "input.h"

namespace messageDialogNS
{
	const UINT WIDTH = 400;             // ダイアログのデフォルトの幅
	const UINT HEIGHT = 100;            // デフォルトの高さ
	const UINT BORDER = 5;
	const UINT MARGIN = 5;              // 枠からテキストまでの余白
	const char FONT[] = "Arial";        // フォント
	const int FONT_HEIGHT = 18;         // フォントの高さ
	const COLOR_ARGB FONT_COLOR = colorNS::WHITE;        // テキストの色
	const COLOR_ARGB BORDER_COLOR = D3DCOLOR_ARGB(192, 192, 192, 192); // 枠線の色
	const COLOR_ARGB BACK_COLOR = SETCOLOR_ARGB(255, 100, 100, 192);   // 背景色
	const UINT X = WINDOW_WIDTH / 2 - WIDTH / 2; // デフォルトの位置
	const UINT Y = WINDOW_HEIGHT / 4 - HEIGHT / 2;
	const UINT BUTTON_WIDTH = (UINT)(FONT_HEIGHT * 4.5);
	const UINT BUTTON_HEIGHT = FONT_HEIGHT + 4;
	const int MAX_TYPE = 2;
	const int OK_CANCEL = 0;            // OK/Cancel タイプのボタン
	const int YES_NO = 1;               // Yes/No タイプのボタン
	static const char* BUTTON1_TEXT[MAX_TYPE] = { "OK", "YES" };
	static const char* BUTTON2_TEXT[MAX_TYPE] = { "CANCEL", "NO" };
	const byte DIALOG_CLOSE_KEY = VK_RETURN;    // Enterキー
	const COLOR_ARGB BUTTON_COLOR = colorNS::GRAY;       // button背景
	const COLOR_ARGB BUTTON_FONT_COLOR = colorNS::WHITE; // buttonテキストの色
}

// メッセージダイアログ
class MessageDialog
{
protected:
	TextDX*      dxFont;                 // DirectXフォント
	float       x, y;                    // 画面の位置
	UINT        height;                 // ダイアログの高さ、print()で計算
	UINT        width;                  // ダイアログの幅
	std::string text;                   // ダイアログテキスト
	RECT        textRect;               // テキストの矩形
	RECT        buttonRect;             // ボタンの矩形
	RECT        button2Rect;            // ボタン2の矩形
	COLOR_ARGB  fontColor;              // フォントの色(a,r,g,b)
	COLOR_ARGB  borderColor;            // 枠の色 (a,r,g,b)
	COLOR_ARGB  backColor;              // 背景の色(a,r,g,b)
	COLOR_ARGB  buttonColor;            // ボタンの色
	COLOR_ARGB  buttonFontColor;        // ボタンフォントの色
	VertexC vtx[4];                     // 頂点データ
	LPDIRECT3DVERTEXBUFFER9 dialogVerts;        // ダイアログの頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 borderVerts;        // 枠の頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 buttonVerts;        // ボタンの頂点バッファ
	LPDIRECT3DVERTEXBUFFER9 button2Verts;       // ボタン2の頂点バッファ
	int buttonClicked;                  // どのボタンがクリックされたか(1または2)
	int buttonType;                     // 0 = OK/Cancel, 1 = Yes/No
	bool    initialized;                // 正常に初期化された場合、true
	bool    visible;                    // trueの場合は表示する
	HWND    hwnd;                       // ウィンドウへのハンドル
	float   screenRatioX, screenRatioY;

public:
	// コンストラクタ
	MessageDialog();
	// デストラクタ
	virtual ~MessageDialog();

	// MessageDialogの初期化
	// 実行前: *gはGraphics オブジェクトを指す
	//      *inはInput オブジェクトを指す
	//      hwnd = ウィンドウハンドル
	bool initialize(HWND hwnd);

	// 頂点バッファを準備
	void prepareVerts();

	// MessageDialogを表示
	const void draw();

	// クリックされたボタンを戻す
	// 0 = ボタンはクリックされていない
	// 1は左ボタン, 2は右ボタン
	int getButtonClicked() { return buttonClicked; }

	// 表示/非表示の状態を戻す
	bool getVisible() { return visible; }

	// フォントの色を設定
	void setFontColor(COLOR_ARGB fc) { fontColor = fc; }

	// 枠の色を設定
	void setBorderColor(COLOR_ARGB bc) { borderColor = bc; }

	// 背景の色を設定
	void setBackColor(COLOR_ARGB bc) { backColor = bc; }

	// ボタンの色を設定
	void setButtonColor(COLOR_ARGB bc) { buttonColor = bc; }

	// ボタンフォントの色を設定
	void setButtonFontColor(COLOR_ARGB bfc) { buttonFontColor = bfc; }

	// 表示/非表示を設定
	void setVisible(bool v) { visible = v; }

	// ボタンタイプを設定(0 = OK/CANCEL, 1 = YES/NO)
	void setButtonType(UINT t)
	{
		if (t < messageDialogNS::MAX_TYPE)
			buttonType = t;
	}

	// テキストstrをMessageDialogに表示
	void print(const std::string &str);

	// 「閉じる」イベントをチェック
	void update();

	// グラフィックスデバイスが消失したときに呼び出す
	void onLostDevice();

	// グラフィックスデバイスがリセットされたときに呼び出す
	void onResetDevice();
};

#endif