#include "messageDialog.h"

//=============================================================================
// コンストラクタ
//=============================================================================
MessageDialog::MessageDialog()
{
	initialized = false;                // 正常に初期化された場合、trueを設定
	visible = false;                    // 非表示にする
	fontColor = messageDialogNS::FONT_COLOR;
	borderColor = messageDialogNS::BORDER_COLOR;
	backColor = messageDialogNS::BACK_COLOR;
	buttonColor = messageDialogNS::BUTTON_COLOR;
	buttonFontColor = messageDialogNS::BUTTON_FONT_COLOR;
	x = messageDialogNS::X;                // 開始位置
	y = messageDialogNS::Y;
	height = messageDialogNS::HEIGHT;
	width = messageDialogNS::WIDTH;
	textRect.bottom = messageDialogNS::Y + messageDialogNS::HEIGHT - messageDialogNS::MARGIN;
	textRect.left = messageDialogNS::X + messageDialogNS::MARGIN;
	textRect.right = messageDialogNS::X + messageDialogNS::WIDTH - messageDialogNS::MARGIN;
	textRect.top = messageDialogNS::Y + messageDialogNS::MARGIN;
	dialogVerts = NULL;
	borderVerts = NULL;
	buttonVerts = NULL;
	button2Verts = NULL;
	buttonType = 0;     // OK/Cancel
}

//=============================================================================
// デストラクタ
//=============================================================================
MessageDialog::~MessageDialog()
{
	onLostDevice();            // すべてのグラフィックスアイテムについてonLostDevice()を呼び出す
}

//=============================================================================
// MessageDialogを初期化
//=============================================================================
bool MessageDialog::initialize(HWND h)
{
	hwnd = h;

	// DirectXフォントを初期化
	dxFont = getDebugFont();
	initialized = true;
	return true;
}

//=============================================================================
// ダイアログの背景とボタンを描画するために頂点バッファを準備
//=============================================================================
void MessageDialog::prepareVerts()
{
	safeRelease(dialogVerts);
	safeRelease(borderVerts);
	safeRelease(buttonVerts);
	safeRelease(button2Verts);

	// 枠の左上
	vtx[0].x = x;
	vtx[0].y = y;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = borderColor;

	// 枠の右上
	vtx[1].x = x + width;
	vtx[1].y = y;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = borderColor;

	// 枠の右下
	vtx[2].x = x + width;
	vtx[2].y = y + height;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = borderColor;

	// 枠の左下
	vtx[3].x = x;
	vtx[3].y = y + height;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = borderColor;

	createVertexBuffer(vtx, sizeof vtx, borderVerts);

	// 背景の左上
	vtx[0].x = x + messageDialogNS::BORDER;
	vtx[0].y = y + messageDialogNS::BORDER;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = backColor;

	// 背景の右上
	vtx[1].x = x + width - messageDialogNS::BORDER;
	vtx[1].y = y + messageDialogNS::BORDER;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = backColor;

	// 背景の右下
	vtx[2].x = x + width - messageDialogNS::BORDER;
	vtx[2].y = y + height - messageDialogNS::BORDER;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = backColor;

	// 背景の左下
	vtx[3].x = x + messageDialogNS::BORDER;
	vtx[3].y = y + height - messageDialogNS::BORDER;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = backColor;

	createVertexBuffer(vtx, sizeof vtx, dialogVerts);

	// ボタンの左上
	vtx[0].x = x + width / 2.0f - messageDialogNS::BUTTON_WIDTH / 2.0f;
	vtx[0].y = y + height - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = buttonColor;

	// ボタンの右上
	vtx[1].x = x + width / 2.0f + messageDialogNS::BUTTON_WIDTH / 2.0f;
	vtx[1].y = vtx[0].y;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = buttonColor;

	// ボタンの右下
	vtx[2].x = vtx[1].x;
	vtx[2].y = vtx[0].y + messageDialogNS::BUTTON_HEIGHT;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = buttonColor;

	// ボタンの左下
	vtx[3].x = vtx[0].x;
	vtx[3].y = vtx[2].y;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = buttonColor;

	createVertexBuffer(vtx, sizeof vtx, buttonVerts);

	// ボタン矩形のセット
	buttonRect.left = (long)vtx[0].x;
	buttonRect.right = (long)vtx[1].x;
	buttonRect.top = (long)vtx[0].y;
	buttonRect.bottom = (long)vtx[2].y;

	// ボタン2の左上
	vtx[0].x = x + width - messageDialogNS::BUTTON_WIDTH*1.2f;
	vtx[0].y = y + height - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = buttonColor;
	// ボタン2の右上
	vtx[1].x = vtx[0].x + messageDialogNS::BUTTON_WIDTH;
	vtx[1].y = vtx[0].y;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = buttonColor;
	// ボタン2の右下
	vtx[2].x = vtx[1].x;
	vtx[2].y = vtx[0].y + messageDialogNS::BUTTON_HEIGHT;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = buttonColor;
	// ボタン2の左下
	vtx[3].x = vtx[0].x;
	vtx[3].y = vtx[2].y;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = buttonColor;
	createVertexBuffer(vtx, sizeof vtx, button2Verts);

	// ボタン2の矩形をセット
	button2Rect.left = (long)vtx[0].x;
	button2Rect.right = (long)vtx[1].x;
	button2Rect.top = (long)vtx[0].y;
	button2Rect.bottom = (long)vtx[2].y;
}

//=============================================================================
// MessageDialogを描画
//=============================================================================
const void MessageDialog::draw()
{
	if (!visible || !initialized)
	{
		return;
	}

	drawQuad(borderVerts);        // 枠を描画
	drawQuad(dialogVerts);        // 背景を描画
	drawQuad(buttonVerts);        // ボタンを描画
	drawQuad(button2Verts);       // ボタン2を描画

	// スプライトの描画を開始
	getSprite()->Begin(D3DXSPRITE_ALPHABLEND);

	if (text.size() == 0)
	{
		return;
	}
	// テキストをMessageDialogに表示
	dxFont->color = fontColor;
	printTextDX(dxFont,text, textRect.left,textRect.top);

	// テキストをボタンに表示
	dxFont->color = buttonFontColor;
	printTextDX(dxFont, messageDialogNS::BUTTON1_TEXT[buttonType], buttonRect.left, buttonRect.top);
	printTextDX(dxFont, messageDialogNS::BUTTON2_TEXT[buttonType], button2Rect.left, button2Rect.top);

	// スプライトの描画を終了
	getSprite()->End();
}

//=============================================================================
// DIALOG_CLOSE_KEYとOKボタンのクリックをチェック
//=============================================================================
void MessageDialog::update()
{
	if (!initialized || !visible)
	{
		return;
	}

	if (GetKeyboardTrigger(messageDialogNS::DIALOG_CLOSE_KEY))
	{
		visible = false;
		buttonClicked = 1;              // ボタン1がクリックされた
		return;
	}

	// ウィンドウがサイズ変更されている場合を考慮して、画面の比率を計算
	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	screenRatioX = (float)WINDOW_WIDTH / clientRect.right;
	screenRatioY = (float)WINDOW_HEIGHT / clientRect.bottom;

	if (getMouseLButton())       // マウスの左ボタンの場合
	{
		// マウスクリックがボタン1(OK)の内側で発生した場合
		if (getMouseX()*screenRatioX >= buttonRect.left &&
			getMouseX()*screenRatioX <= buttonRect.right &&
			getMouseY()*screenRatioY >= buttonRect.top &&
			getMouseY()*screenRatioY <= buttonRect.bottom)
		{
			visible = false;            // メッセージダイアログを非表示
			buttonClicked = 1;          // ボタン1がクリックされた
			return;
		}

		// マウスクリックがボタン2(cancel)の内側で発生した場合
		if (getMouseX()*screenRatioX >= button2Rect.left &&
			getMouseX()*screenRatioX <= button2Rect.right &&
			getMouseY()*screenRatioY >= button2Rect.top &&
			getMouseY()*screenRatioY <= button2Rect.bottom)
		{
			visible = false;            // メッセージダイアログを非表示
			buttonClicked = 2;          // ボタン2がクリックされた
		}
	}
}

//=============================================================================
// テキスト文字列を設定、ダイアログの下端をテキストが収まるサイズに変更、
// visible = trueを設定
//=============================================================================
void MessageDialog::print(const std::string &str)
{
	// 初期化されていない、又は既に使用中の場合
	if (!initialized || visible)
	{
		return;
	}
	text = str + "\n\n\n\n"; // ボタン用に空間を空ける

	// textRectをダイアログのテキスト領域として設定
	textRect.left = (long)(x + messageDialogNS::MARGIN);
	textRect.right = (long)(x + messageDialogNS::WIDTH - messageDialogNS::MARGIN);
	textRect.top = (long)(y + messageDialogNS::MARGIN);
	textRect.bottom = (long)(y + messageDialogNS::HEIGHT - messageDialogNS::MARGIN);

	// textRect.bottomをテキストに必要な正確な高さに設定
	// DT_CALDRECTオプションの場合、テキストは出力されない
	printTextDX(dxFont, text, textRect.left, textRect.top);
	height = textRect.bottom - (int)y + messageDialogNS::BORDER + messageDialogNS::MARGIN;

	prepareVerts();    // 頂点バッファを準備
	buttonClicked = 0; // buttonClickedをクリア
	visible = true;
}

//=============================================================================
// グラフィックスデバイスが消失したときに呼び出される
//=============================================================================
void MessageDialog::onLostDevice()
{
	if (!initialized)
	{
		return;
	}
	safeRelease(dxFont->dxFont);
	safeRelease(dialogVerts);
	safeRelease(borderVerts);
	safeRelease(buttonVerts);
	safeRelease(button2Verts);
}

//=============================================================================
// グラフィックスデバイスをリセットしたときに呼び出される
//=============================================================================
void MessageDialog::onResetDevice()
{
	if (!initialized)
	{
		return;
	}
	prepareVerts();
}
