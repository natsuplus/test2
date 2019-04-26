//=============================================================================
// 入力処理 [input.cpp]
//=============================================================================
#include "input.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_KEY_MAX			(256)

// game pad用設定値
#define DEADZONE		2500			// 各軸の25%を無効ゾーンとする
#define RANGE_MAX		1000			// 有効範囲の最大値
#define RANGE_MIN		-1000			// 有効範囲の最小値

// 高精細マウス用
#ifndef HID_USAGE_PAGE_GENERIC
#define HID_USAGE_PAGE_GENERIC		((USHORT) 0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#define HID_USAGE_GENERIC_MOUSE		((USHORT) 0x02)
#endif
//--------------------------

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);

HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow, bool capture); // マウスの初期化
void UpdateMouse(void);
void UninitMouse();						// マウスの終了処理

HRESULT InitializePad(void);			// パッド初期化
//BOOL CALLBACK SearchPadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID);	// パッド検査コールバック
void UpdatePad(void);
void UninitPad(void);

//*****************************************************************************
// グローバル変数
//*****************************************************************************

std::string textIn;	// ユーザーが入力したテキスト
char charIn;									// 最後に入力した文字
bool newLine;									// 改行の開始時にtrue
//------------------------------- keyboard
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
BYTE					g_keyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ

//--------------------------------- mouse
int mouseRawX, mouseRawY;						// 高解像度のマウスデータ
int mouseX, mouseY;								// マウスの画面座標
RAWINPUTDEVICE Rid[1];							// 高精細マウス用
bool mouseCaptured;								// マウスがキャプチャされた場合はtrue
bool mouseLButton;								// マウスの左ボタンが押されている場合はtrue
bool mouseMButton;								// 中マウスボタンが押されている場合はtrue
bool mouseRButton;								// マウスの右ボタンが押されている場合はtrue
bool mouseX1Button;								// X1のマウスボタンが押されている場合はtrue
bool mouseX2Button;								// X2のマウスボタンが押されている場合はtrue

bool flag[5] = { false,false,false,false,false };
bool mouseButtonTrigger[5] = { false,false,false,false,false };						// マウスのボタンが押された時一度だけtrue,順番は上準拠;						// マウスのボタンが押された時一度だけtrue,順番は上準拠


//--------------------------------- game pad
static LPDIRECTINPUTDEVICE8	pGamePad[GAMEPADMAX] = { NULL,NULL,NULL,NULL };// パッドデバイス

static DWORD	padState[GAMEPADMAX];	// パッド情報（複数対応）
static DWORD	padTrigger[GAMEPADMAX];
static int		padCount = 0;			// 検出したパッドの数


//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd,bool capture)
{
	HRESULT hr;

	if (!g_pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
			IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}
	newLine = true;			// 新しい行を開始
	textIn = "";			// textInをクリア
	charIn = 0;				// charInをクリア
	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

	// マウスの初期化
	InitializeMouse(hInst, hWnd,capture);

	// パッドの初期化
	InitializePad();

	return S_OK;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// マウスの終了処理
	UninitMouse();

	// パッドの終了処理
	UninitPad();

	if (g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
	// キーボードの更新
	UpdateKeyboard();

	// マウスの更新
	UpdateMouse();

	// パッドの更新
	UpdatePad();

}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if (FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if (FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if (g_pDIDevKeyboard)
	{
		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// キーボードの更新
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// 前回のデータを保存
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// デバイスからデータを取得
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if (SUCCEEDED(hr))
	{
		for (int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			g_keyStateTrigger[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt];
			g_keyStateRelease[cnt] = (keyStateOld[cnt] ^ g_keyState[cnt]) & ~g_keyState[cnt];
			g_keyStateRepeat[cnt] = g_keyStateTrigger[cnt];

			if (g_keyState[cnt])
			{
				g_keyStateRepeatCnt[cnt]++;
				if (g_keyStateRepeatCnt[cnt] >= 20)
				{
					g_keyStateRepeat[cnt] = g_keyState[cnt];
				}
			}
			else
			{
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		g_pDIDevKeyboard->Acquire();
	}

	return S_OK;
}

// クリアテキスト入力バッファ
void clearTextIn()
{
	textIn.clear(); 
}
// テキスト入力を文字列として返す
std::string getTextIn()
{
	return textIn; 
}

// 最後に入力した文字を返します
char getCharIn() 
{
	return charIn; 
}

//=============================================================================
// 入力された文字をtextIn文字列に保存
// 実行前：wParamに、文字が格納されている
//=============================================================================
void keyIn(WPARAM wParam)
{
	if (newLine)					// 新しい行の開始の場合
	{
		textIn.clear();
		newLine = false;
	}

	if (wParam == '\b')				// バックスペースキーの場合
	{
		if (textIn.length() > 0)		// 文字が存在する場合
		{
			textIn.erase(textIn.size() - 1);	// 最後に入力された文字を消去
		}
	}
	else {
		textIn += wParam;						// 文字をtextInに追加
		charIn = wParam;						// 最後に入力された文字を保存
	}
	if ((char)wParam == '\r')					// リターンキーの場合
	{
		newLine = true;							// 新しい行を開始
	}
}
//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int key)
{
	return (g_keyState[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return (g_keyStateTrigger[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return (g_keyStateRepeat[key] & 0x80) ? true : false;
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return (g_keyStateRelease[key] & 0x80) ? true : false;
}


//=============================================================================
// マウス関係の処理
//=============================================================================
// マウスの初期化
HRESULT InitializeMouse(HINSTANCE hInst, HWND hWindow,bool capture)
{
	// マウスデータ
	mouseX = 0;				// 画面X
	mouseY = 0;				// 画面Y
	mouseRawX = 0;			// 高精細X
	mouseRawY = 0;			// 高精細Y
	mouseLButton = false;	// 左マウスボタンが押されている場合にtrue
	mouseMButton = false;	// 中央マウスボタンが押されている場合にtrue
	mouseRButton = false;	// 右マウスボタンが押されている場合にtrue
	mouseX1Button = false;	// X1マウスボタンが押されている場合にtrue
	mouseX2Button = false;	// X2マウスボタンが押されている場合にtrue

	mouseCaptured = capture;

	// 高精細マウスを登録
	Rid[0].usUsagePage = HID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = HID_USAGE_GENERIC_MOUSE;
	Rid[0].dwFlags = RIDEV_INPUTSINK;
	Rid[0].hwndTarget = hWindow;
	RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));

	if (mouseCaptured)
	{
		SetCapture(hWindow);	// マウスをキャプチャ
	}
	return S_OK;
}
//---------------------------------------------------------
void UpdateMouse(void) 
{
	if (getMouseLButton())
	{
		if (flag[0] == false) 
		{
			mouseButtonTrigger[0] = true;
			flag[0] = true;
		}
		else
		{
			mouseButtonTrigger[0] = false;
		}
	}
	else
	{
		if (flag[0] == true)
		{

			flag[0] = false;
		}
	}

	if (getMouseMButton())
	{
		if (flag[1] == false)
		{
			mouseButtonTrigger[1] = true;
			flag[1] = true;
		}
		else
		{
			mouseButtonTrigger[1] = false;
		}
	}
	else
	{
		if (flag[1] == true)
		{

			flag[1] = false;
		}
	}

	if (getMouseRButton())
	{
		if (flag[2] == false)
		{
			mouseButtonTrigger[2] = true;
			flag[2] = true;
		}
		else
		{
			mouseButtonTrigger[2] = false;
		}
	}
	else
	{
		if (flag[2] == true)
		{

			flag[2] = false;
		}
	}
}
//---------------------------------------------------------
void UninitMouse()
{
	if (mouseCaptured)
	{
		ReleaseCapture(); // マウスを解放
	}
}
//=============================================================================
// マウスの画面位置を読み取り、mouseXとmouseYに保存
//=============================================================================

void mouseIn(LPARAM lParam)
{
	mouseX = GET_X_LPARAM(lParam);
	mouseY = GET_Y_LPARAM(lParam);
}
//=============================================================================
// マウスからのローデータを読み取り、mouseRawXとmouseRawYに保存
// このルーチンは、高精細マウスに対応しています。
//=============================================================================

void mouseRawIn(LPARAM lParam)
{
	UINT dwSize = 40;
	static BYTE lpb[40];
	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));

	RAWINPUT* raw = (RAWINPUT*)lpb;

	if (raw->header.dwType == RIM_TYPEMOUSE)
	{
		mouseRawX = raw->data.mouse.lLastX;
		mouseRawY = raw->data.mouse.lLastY;
	}
}
//-----------------------------------------------------------
void mouseClear(UCHAR what)
{
	// マウスをクリアする場合
	if (what & MOUSE)
	{
		mouseX = 0;
		mouseY = 0;
		mouseRawX = 0;
		mouseRawY = 0;
	}
}

//---------------------------- mouse
// マウスボタンの状態を保存
void setMouseLButton(bool b) { mouseLButton = b; }

// マウスボタンの状態を保存
void setMouseMButton(bool b) { mouseMButton = b; }

// マウスボタンの状態を保存
void setMouseRButton(bool b) { mouseRButton = b; }

// マウスボタンの状態を保存
void setMouseXButton(WPARAM wParam) {
	mouseX1Button = (wParam & MK_XBUTTON1) ? true : false;
	mouseX2Button = (wParam & MK_XBUTTON2) ? true : false;
}

// マウスのX位置を戻す
int getMouseX() { return mouseX; }

// マウスのY位置を戻す
int getMouseY() { return mouseY; }

// マウスのX位置の移動のローデータを戻す。左への移動は<0、右への移動は>0
//高精細マウスと互換性があります。
int getMouseRawX() { return mouseRawX; }

// マウスのY位置の移動のローデータを戻す。左への移動は<0、右への移動は>0
// 高精細マウスと互換性があります。
int getMouseRawY() { return mouseRawY; }

// 左マウスボタンの状態を戻す
bool getMouseLButton() { return mouseLButton; }

// 左マウスボタンのトリガー情報を戻す
bool getMouseLButtonTrigger() { return mouseButtonTrigger[0]; }

// 中央マウスボタンの状態を戻す
bool getMouseMButton() { return mouseMButton; }

// 中央マウスボタンのトリガー情報を戻す
bool getMouseMButtonTrigger() { return mouseButtonTrigger[1]; }

// 右マウスボタンの状態を戻す
bool getMouseRButton() { return mouseRButton; }

// 右マウスボタンのトリガー情報を戻す
bool getMouseRButtonTrigger() { return mouseButtonTrigger[2]; }

// X1マウスボタンの状態を戻す
bool getMouseX1Button() { return mouseX1Button; }

// X1マウスボタンのトリガー情報を戻す
bool getMouseX1ButtonTrigger() { return mouseButtonTrigger[3]; }

// X2マウスボタンの状態を戻す
bool getMouseX2Button() { return mouseX2Button; }

// X2マウスボタンのトリガー情報を戻す
bool getMouseX2ButtonTrigger() { return mouseButtonTrigger[4]; }

//================================================= game pad
//---------------------------------------- コールバック関数
BOOL CALLBACK SearchGamePadCallback(LPDIDEVICEINSTANCE lpddi, LPVOID)
{
	HRESULT result;

	result = g_pDInput->CreateDevice(lpddi->guidInstance, &pGamePad[padCount++], NULL);
	return DIENUM_CONTINUE;	// 次のデバイスを列挙

}
//---------------------------------------- 初期化
HRESULT InitializePad(void)			// パッド初期化
{
	HRESULT		result;
	int			i;

	padCount = 0;
	// ジョイパッドを探す
	g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, (LPDIENUMDEVICESCALLBACK)SearchGamePadCallback, NULL, DIEDFL_ATTACHEDONLY);
	// セットしたコールバック関数が、パッドを発見した数だけ呼ばれる。

	for (i = 0; i<padCount; i++) {
		// ジョイスティック用のデータ・フォーマットを設定
		result = pGamePad[i]->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
			return false; // データフォーマットの設定に失敗

						  // モードを設定（フォアグラウンド＆非排他モード）
						  //		result = pGamePad[i]->SetCooperativeLevel(hWindow, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
						  //		if ( FAILED(result) )
						  //			return false; // モードの設定に失敗

						  // 軸の値の範囲を設定
						  // X軸、Y軸のそれぞれについて、オブジェクトが報告可能な値の範囲をセットする。
						  // (max-min)は、最大10,000(?)。(max-min)/2が中央値になる。
						  // 差を大きくすれば、アナログ値の細かな動きを捕らえられる。(パッドの性能による)
		DIPROPRANGE				diprg;
		ZeroMemory(&diprg, sizeof(diprg));
		diprg.diph.dwSize = sizeof(diprg);
		diprg.diph.dwHeaderSize = sizeof(diprg.diph);
		diprg.diph.dwHow = DIPH_BYOFFSET;
		diprg.lMin = RANGE_MIN;
		diprg.lMax = RANGE_MAX;
		// X軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);
		// Y軸の範囲を設定
		diprg.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_RANGE, &diprg.diph);

		// 各軸ごとに、無効のゾーン値を設定する。
		// 無効ゾーンとは、中央からの微少なジョイスティックの動きを無視する範囲のこと。
		// 指定する値は、10000に対する相対値(2000なら20パーセント)。
		DIPROPDWORD				dipdw;
		dipdw.diph.dwSize = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwHow = DIPH_BYOFFSET;
		dipdw.dwData = DEADZONE;
		//X軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_X;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);
		//Y軸の無効ゾーンを設定
		dipdw.diph.dwObj = DIJOFS_Y;
		pGamePad[i]->SetProperty(DIPROP_DEADZONE, &dipdw.diph);

		//ジョイスティック入力制御開始
		pGamePad[i]->Acquire();
	}

	return true;

}
//------------------------------------------- 終了処理
void UninitPad(void)
{
	for (int i = 0; i<GAMEPADMAX; i++) {
		if (pGamePad[i])
		{
			pGamePad[i]->Unacquire();
			pGamePad[i]->Release();
		}
	}

}

//------------------------------------------ 更新
void UpdatePad(void)
{
	HRESULT			result;
	DIJOYSTATE2		dijs;
	int				i;

	for (i = 0; i<padCount; i++)
	{
		DWORD lastPadState;
		lastPadState = padState[i];
		padState[i] = 0x00000000l;	// 初期化

		result = pGamePad[i]->Poll();	// ジョイスティックにポールをかける
		if (FAILED(result)) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		result = pGamePad[i]->GetDeviceState(sizeof(DIJOYSTATE), &dijs);	// デバイス状態を読み取る
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			result = pGamePad[i]->Acquire();
			while (result == DIERR_INPUTLOST)
				result = pGamePad[i]->Acquire();
		}

		// ３２の各ビットに意味を持たせ、ボタン押下に応じてビットをオンにする
		//* y-axis (forward)
		if (dijs.lY < 0)					padState[i] |= BUTTON_UP;
		//* y-axis (backward)
		if (dijs.lY > 0)					padState[i] |= BUTTON_DOWN;
		//* x-axis (left)
		if (dijs.lX < 0)					padState[i] |= BUTTON_LEFT;
		//* x-axis (right)
		if (dijs.lX > 0)					padState[i] |= BUTTON_RIGHT;
		//* Ａボタン
		if (dijs.rgbButtons[0] & 0x80)	padState[i] |= BUTTON_A;
		//* Ｂボタン
		if (dijs.rgbButtons[1] & 0x80)	padState[i] |= BUTTON_B;
		//* Ｃボタン
		if (dijs.rgbButtons[2] & 0x80)	padState[i] |= BUTTON_C;
		//* Ｘボタン
		if (dijs.rgbButtons[3] & 0x80)	padState[i] |= BUTTON_X;
		//* Ｙボタン
		if (dijs.rgbButtons[4] & 0x80)	padState[i] |= BUTTON_Y;
		//* Ｚボタン
		if (dijs.rgbButtons[5] & 0x80)	padState[i] |= BUTTON_Z;
		//* Ｌボタン
		if (dijs.rgbButtons[6] & 0x80)	padState[i] |= BUTTON_L;
		//* Ｒボタン
		if (dijs.rgbButtons[7] & 0x80)	padState[i] |= BUTTON_R;
		//* ＳＴＡＲＴボタン（右スティック、-ボタン）
		if (dijs.rgbButtons[8] & 0x80)	padState[i] |= BUTTON_START;
		//* Ｍボタン(左スティック、+ボタン)
		if (dijs.rgbButtons[9] & 0x80)	padState[i] |= BUTTON_M;

		// ホームボタン（右スティック）
		if (dijs.rgbButtons[12] & 0x80)	padState[i] |= BUTTON_HOME;

		//ZRボタン（ジョイコン）
		if (dijs.rgbButtons[14] & 0x80)	padState[i] |= BUTTON_LR;

		if (dijs.rgbButtons[15] & 0x80)	padState[i] |= BUTTON_ZLR;

		//ジョイコンスティック操作*操作軸に注意*
		if (dijs.rgdwPOV[0] == POV::UP)				padState[i] |= STICK_UP;

		if (dijs.rgdwPOV[0] == POV::RIGHT)			padState[i] |= STICK_RIGHT;

		if (dijs.rgdwPOV[0] == POV::DOWN)			padState[i] |= STICK_DOWN;

		if (dijs.rgdwPOV[0] == POV::LEFT)			padState[i] |= STICK_LEFT;

		//斜め入力
		if (dijs.rgdwPOV[0] == POV::RIGHTUP)		padState[i] |= STICK_RIGHTUP;

		if (dijs.rgdwPOV[0] == POV::RIGHTDOWN)		padState[i] |= STICK_RIGHTDOWN;

		if (dijs.rgdwPOV[0] == POV::LEFTUP)			padState[i] |= STICK_LEFTUP;

		if (dijs.rgdwPOV[0] == POV::LEFTDOWN)		padState[i] |= STICK_LEFTDOWN;

		// Trigger設定
		padTrigger[i] = ((lastPadState ^ padState[i])	// 前回と違っていて
			& padState[i]);					// しかも今ONのやつ

	}

}
//----------------------------------------------- 検査
BOOL IsButtonPressed(int padNo, DWORD button)
{
	return (button & padState[padNo]);
}

BOOL IsButtonTriggered(int padNo, DWORD button)
{
	return (button & padTrigger[padNo]);
}