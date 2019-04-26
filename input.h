#pragma once
//=============================================================================
// 入力処理 [input.h]
//=============================================================================
#include "main.h"
#include <string>
//*****************************************************************************
// マクロ定義
//*****************************************************************************

// プログラム分けするときに使う
#define	USE_KEYBOARD										// 宣言するとキーボードで操作可能になる
#define	USE_MOUSE											// 宣言するとマウスで操作可能になる
#define	USE_PAD												// 宣言するとパッドで操作可能になる


/* game pad情報 */
#define BUTTON_UP		0x00000001l	// 方向キー上(.IY<0)
#define BUTTON_DOWN		0x00000002l	// 方向キー下(.IY>0)
#define BUTTON_LEFT		0x00000004l	// 方向キー左(.IX<0)
#define BUTTON_RIGHT	0x00000008l	// 方向キー右(.IX>0)
#define BUTTON_A		0x00000010l	// Ａボタン(.rgbButtons[0]&0x80)
#define BUTTON_B		0x00000020l	// Ｂボタン(.rgbButtons[1]&0x80)
#define BUTTON_C		0x00000040l	// Ｃボタン(.rgbButtons[2]&0x80)
#define BUTTON_X		0x00000080l	// Ｘボタン(.rgbButtons[3]&0x80)
#define BUTTON_Y		0x00000100l	// Ｙボタン(.rgbButtons[4]&0x80)
#define BUTTON_Z		0x00000200l	// Ｚボタン(.rgbButtons[5]&0x80)
#define BUTTON_L		0x00000400l	// Ｌボタン(.rgbButtons[6]&0x80)
#define BUTTON_R		0x00000800l	// Ｒボタン(.rgbButtons[7]&0x80)
#define BUTTON_START	0x00001000l	// ＳＴＡＲＴボタン(.rgbButtons[8]&0x80)
#define BUTTON_M		0x00002000l	// Ｍボタン(.rgbButtons[9]&0x80)
#define BUTTON_MENU		0x00004000l	
#define BUTTON_HOME		0x00008000l	
#define GAMEPADMAX		4			// 同時に接続するジョイパッドの最大数をセット

const UCHAR MOUSE = 4;

// JoyConのスティック入力（上下左右）
#define STICK_UP		0x00010000l
#define STICK_RIGHT		0x00020000l
#define STICK_DOWN		0x00040000l
#define STICK_LEFT		0x00080000l

// JoyConのスティック入力（斜め）
#define STICK_RIGHTUP		0x00100000l
#define STICK_RIGHTDOWN		0x00200000l
#define STICK_LEFTDOWN		0x00400000l
#define STICK_LEFTUP		0x00800000l

// LRとZLRボタン
#define BUTTON_LR		0x01000000l
#define BUTTON_ZLR		0x02000000l


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInst, HWND hWnd,bool capture);
void UninitInput(void);
void UpdateInput(void);

void keyIn(WPARAM wParam);
// クリアテキスト入力バッファ
void clearTextIn(); 
// テキスト入力を文字列として返す
std::string getTextIn();
// 最後に入力した文字を返します
char getCharIn();

//---------------------------- keyboard
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardRelease(int nKey);

//---------------------------- mouse
// マウスの位置をmouseX、mouseYに読み込みます
void mouseIn(LPARAM);
void mouseClear(UCHAR what);
// 生のマウスデータをmouseRawX、mouseRawYに読み込みます
// このルーチンは高精細マウスと互換性があります
void mouseRawIn(LPARAM);
// マウスボタンの状態を保存
void setMouseLButton(bool b);
// マウスボタンの状態を保存
void setMouseMButton(bool b);
// マウスボタンの状態を保存
void setMouseRButton(bool b);
// マウスボタンの状態を保存
void setMouseXButton(WPARAM wParam);
// マウスのX位置を戻す
int getMouseX();
// マウスのY位置を戻す
int getMouseY();
// マウスのX位置の移動のローデータを戻す。左への移動は<0、右への移動は>0
//高精細マウスと互換性があります。
int getMouseRawX();
// マウスのY位置の移動のローデータを戻す。左への移動は<0、右への移動は>0
// 高精細マウスと互換性があります。
int getMouseRawY();
// 左マウスボタンの状態を戻す
bool getMouseLButton();
// 左マウスボタンのトリガー情報を戻す
bool getMouseLButtonTrigger();
// 中央マウスボタンの状態を戻す
bool getMouseMButton();
// 中央マウスボタンのトリガー情報を戻す
bool getMouseMButtonTrigger();
// 右マウスボタンの状態を戻す
bool getMouseRButton();
// 右マウスボタンのトリガー情報を戻す
bool getMouseRButtonTrigger();
// X1マウスボタンの状態を戻す
bool getMouseX1Button();
// X1マウスボタンのトリガー情報を戻す
bool getMouseX1ButtonTrigger();
// X2マウスボタンの状態を戻す
bool getMouseX2Button();
// X2マウスボタンのトリガー情報を戻す
bool getMouseX2ButtonTrigger();

//---------------------------- game pad
BOOL IsButtonPressed(int padNo, DWORD button);
BOOL IsButtonTriggered(int padNo, DWORD button);

// JoyConのスティック入力
enum POV
{
	UP = 0,
	RIGHTUP = 4500,
	RIGHT = 9000,
	RIGHTDOWN = 13500,
	DOWN = 18000,
	LEFTDOWN = 22500,
	LEFT = 27000,
	LEFTUP = 31500,
};