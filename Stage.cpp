#include "Stage.h"
#include "textDX.h"
#include "input.h"
#include "Image.h"
#include"textureLoader.h"
#include "messageDialog.h"
#include "inputDialog.h"
#include "object.h"

MessageDialog mDialog;
InputDialog inDialog;

StageObj stageObj;//西川 0.02
Image gameBG;
void initializeStage() {
	inDialog.initialize(getHWnd());
	mDialog.initialize(getHWnd());
	mDialog.print("Hello World");
	initializeObject(&stageObj,0);//西川 0.02 ひとまず0ステージで
	InitImage(&gameBG, getTexture(textureLoaderNS::BACK_GROUND), 0, 0, 1200, 900);
};
void updateStage() {
	inDialog.update();
	mDialog.update();
	updateObject(&stageObj);
	if (getMouseMButton())
	{
		inDialog.print("please InputText");
	}

};
void drawStage() {
	inDialog.draw();
	//mDialog.draw();
	drawObject(&stageObj);

};

void printStage() {
	printTextDX(getDebugFont(), "Stage", 0, 0);
	printTextDX(getDebugFont(), "マウス中ボタンを押下で入力ダイアログボックスを表示する", 500, 30);
	printTextDX(getDebugFont(), "mouseX:", 1000, 0, getMouseX());
	printTextDX(getDebugFont(), "mouseY:", 1000, 30, getMouseY());

	DrawImage(&gameBG);
	printObject(&stageObj);//西川 0.22
};
void unInitializeStage() {
	uninitializeObject(&stageObj);
};