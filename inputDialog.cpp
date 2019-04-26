#include "inputDialog.h"

//=============================================================================
// コンストラクタ
//=============================================================================
InputDialog::InputDialog()
{
	textBackColor = inputDialogNS::TEXT_BACK_COLOR;
	textFontColor = inputDialogNS::TEXT_COLOR;
	inTextVerts = NULL;
	inText = "";
}

//=============================================================================
// デストラクタ
//=============================================================================
InputDialog::~InputDialog()
{
	onLostDevice();            // すすべてのグラフィックスアイテムについてonLostDevice()を呼び出す
}

//=============================================================================
// ダイアログの背景とボタンを描画するために頂点バッファを準備
//=============================================================================
void InputDialog::prepareVerts()
{
	MessageDialog::prepareVerts(); // 基本クラスのprepareVertsを呼び出す
	safeRelease(inTextVerts);

	// inText左上
	vtx[0].x = x + messageDialogNS::BORDER * 2;
	vtx[0].y = y + height - messageDialogNS::BORDER - messageDialogNS::MARGIN - messageDialogNS::BUTTON_HEIGHT*2.5f;
	vtx[0].z = 0.0f;
	vtx[0].rhw = 1.0f;
	vtx[0].color = textBackColor;
	// inText右上
	vtx[1].x = x + width - messageDialogNS::BORDER * 2;
	vtx[1].y = vtx[0].y;
	vtx[1].z = 0.0f;
	vtx[1].rhw = 1.0f;
	vtx[1].color = textBackColor;
	// inText右下
	vtx[2].x = vtx[1].x;
	vtx[2].y = vtx[0].y + messageDialogNS::BUTTON_HEIGHT;
	vtx[2].z = 0.0f;
	vtx[2].rhw = 1.0f;
	vtx[2].color = textBackColor;
	// inText左下
	vtx[3].x = vtx[0].x;
	vtx[3].y = vtx[2].y;
	vtx[3].z = 0.0f;
	vtx[3].rhw = 1.0f;
	vtx[3].color = textBackColor;
	createVertexBuffer(vtx, sizeof vtx, inTextVerts);

	// inText矩形を設定
	inTextRect.left = (long)vtx[0].x;
	inTextRect.right = (long)vtx[1].x;
	inTextRect.top = (long)vtx[0].y;
	inTextRect.bottom = (long)vtx[2].y;
}


//=============================================================================
// InputDialogを描画
//=============================================================================
const void InputDialog::draw()
{
	if (!visible || !initialized)
		return;

	drawQuad(borderVerts);        // 枠を描画
	drawQuad(dialogVerts);        // 背景を描画
	drawQuad(buttonVerts);        // ボタンを描画
	drawQuad(button2Verts);       // ボタン2を描画
	drawQuad(inTextVerts);        // テキスト入力領域を描画

	getSprite()->Begin(D3DXSPRITE_ALPHABLEND);                // スプライトの描画を開始

	if (text.size() == 0)
	{
		return;
	}
	// MessageDialogでテキストを表示
	dxFont->color = fontColor;
	printTextDX(dxFont, text, textRect.left, textRect.top);

	// ボタンのテキストを表示
	dxFont->color = buttonFontColor;
	printTextDX(dxFont, messageDialogNS::BUTTON1_TEXT[buttonType], buttonRect.left, buttonRect.top);
	printTextDX(dxFont, messageDialogNS::BUTTON2_TEXT[buttonType], button2Rect.left, button2Rect.top);

	// 入力テキストを表示
	dxFont->color=textFontColor;
	tempRect = inTextRect;      // 保存
	
	// DT_CALDRECTオプションの場合、テキストは出力されない。矩形を右へ移動
	printTextDX(dxFont,inText, tempRect.left, tempRect.top);

	// スプライトの描画を終了
	getSprite()->End();
}

//=============================================================================
// DIALOG_CLOSE_KEYとマウスクリックボタンをチェック
//=============================================================================
void InputDialog::update()
{
	MessageDialog::update();        // 基本クラスのupdateを呼び出す
	if (!initialized || !visible)
	{
		if (buttonClicked == 2)      // Cancelボタンの場合
		{
			inText = "";            // 入力テキストをクリア
		}
		return;
	}
	inText = getTextIn();    // 入力テキスト取得
}

//=============================================================================
// Set text string, size dialog bottom to fit text and set visible = true
//=============================================================================
void InputDialog::print(const std::string &str)
{
	// 初期化されていない、又は既に使用中の場合
	if (!initialized || visible)
	{
		return;
	}
	text = str + "\n\n\n\n\n";   // ボタン用に空間を空ける

								 // textRectをダイアログのテキスト領域として設定
	textRect.left = (long)(x + messageDialogNS::MARGIN);
	textRect.right = (long)(x + messageDialogNS::WIDTH - messageDialogNS::MARGIN);
	textRect.top = (long)(y + messageDialogNS::MARGIN);
	textRect.bottom = (long)(y + messageDialogNS::HEIGHT - messageDialogNS::MARGIN);

	// textRect.bottomをテキストに必要な正確な高さに設定
	// No text is printed with DT_CALDRECTオプションの場合、テキストは出力されない.
	printTextDX(dxFont, text, textRect.left, textRect.top);
	height = textRect.bottom - (int)y + messageDialogNS::BORDER + messageDialogNS::MARGIN;

	prepareVerts();                 // 頂点バッファを準備
	inText = "";                    // 古い入力をクリア
	clearTextIn();
	buttonClicked = 0;              // buttonClickedをクリア
	visible = true;
}

//=============================================================================
// グラフィックスデバイスが消失したときに呼び出される
//=============================================================================
void InputDialog::onLostDevice()
{
	if (!initialized)
		return;
	MessageDialog::onLostDevice();
	SAFE_RELEASE(inTextVerts);
}



