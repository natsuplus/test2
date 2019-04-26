#pragma once
#include "object.h"

// データData（ヘッダ部）
typedef struct {
//	int no;					// 番号
//	char name[20];			// 名前
	ObjStr *p_obj;
	objTypes *p_objType;
	short *p_objId;
}Data;

enum {
//	DATA_NO = 1,			// 番号を表す定数値
//	DATA_NAME,				// 名前を表す定数値
	DATA_OBJ_POINTER,
	DATA_OBJ_TYPE,			// 種類を表す定数値
	DATA_OBJ_ID,			// 番号を表す定数値
};

// データの番号の比較関数
int DataNoCmp(const Data* x, const Data* y);

// データの名前の比較関数
int DataNameCmp(const Data* x, const Data* y);

// データの表示（改行なし）
void PrintData(const Data* d);

/*
// データの表示（改行あり）
void PrintLnData(const Data* d);
*/

// データの読み込み
Data ScanData(const char* message, int sw);
