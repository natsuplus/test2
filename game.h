#pragma once
#include "main.h"

enum SceneList {
	TITLE,
	STAGE,
	RESULT,
	SCENE_NUM,
};

void initializeGame();
void updateGame();
void drawGame();
void printGame();
void unInitializeGame();
