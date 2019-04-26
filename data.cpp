// データData（ヘッダ部）
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "data.h"

#include "textDX.h"


// データの番号の比較関数
int DataNoCmp(const Data* x, const Data* y)
{
	// xの番号がyの番号より小さい時→-1を戻す
	// xの番号がyの番号より大きい時→1を戻す
	// xの番号とyの番号が等しい時→0を戻す
//	return x->no < y->no ? -1 : x->no > y->no ? 1 : 0;
	return &(x->p_objId) < &(y->p_objId) ? -1 : &(x->p_objId) > &(y->p_objId) ? 1 : 0;
};

// データの名前の比較関数
// 文字コードの値によって比較
// 一致すれば0を戻す
// 第1引数 < 第2引数　の場合はマイナスの値
// 第1引数 > 第2引数　の場合はプラスの値
int DataNameCmp(const Data* x, const Data* y)
{
//	return strcmp(x->name, y->name);
	return int(&(x->p_objType)) - int(&(y->p_objType));
};

// データの表示（改行なし）
void PrintData(const Data* d)
{
//	printf("%d %s", d->no, d->name);
	printTextDX(getDebugFont(), "id：", 0, txtLineBreak(), int(&(d->p_objId)));
	printTextDX(getDebugFont(), "type：", 0, txtLineBreak(), int(&(d->p_objType)));
};

/*
// データの表示（改行あり）
void PrintLnData(const Data* d)
{
	printf("%d %s\n", d->no, d->name);
};
*/

// データの読み込み
Data ScanData(const char* message, int sw)
{
	Data temp;
//	printf("%sするデータを入力してください。\n", message);
//	if (sw & DATA_NO) { printf("番号："); scanf("%d", &temp.no); }
//	if (sw & DATA_NAME) { printf("名前："); scanf("%s", temp.name); }
	printTextDX(getDebugFont(), "%sするデータを入力してください。\n", 0, txtLineBreak());
	if (sw & DATA_OBJ_POINTER) { printTextDX(getDebugFont(), "", 0, txtLineBreak(), int(&(temp.p_obj))); }
	if (sw & DATA_OBJ_ID) { printTextDX(getDebugFont(), "", 0, txtLineBreak(), int(&(temp.p_objId)));}
	if (sw & DATA_OBJ_TYPE) { printTextDX(getDebugFont(), "", 0, txtLineBreak(), int(&(temp.p_objType))); }
	return temp;
};

