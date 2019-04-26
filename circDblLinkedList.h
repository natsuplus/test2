#pragma once
// 循環・重連結リスト(ヘッダ部)

#include "data.h"

// ノード
typedef struct __node {
	Data	data;						// データ
	struct __node *prev;		// 先行ノードへのポインタ
	struct __node *next;		// 後続ノードへのポインタ
}Dnode;

// 循環・重連結リスト
typedef struct {
	Dnode *head;				// 先頭ダミーノードへのポインタ
	Dnode *crnt;					// 着目ノードへのポインタ
}Dlist;

// リストを初期化
void Initialize(Dlist *list);

// 着目ノードのデータを表示
void PrintCurrent(const Dlist *list);

// 着目ノードのデータを表示（改行付き）
void PrintLnCurrent(const Dlist *list);

// 関数compareによってxと一致すると判定されるノードを探索
Dnode* Search(Dlist *list, const Data* x, int compare(const Data* x, const Data* y));

// 全ノードのデータをリスト順に表示
void Print(const Dlist* list);

// 全ノードのデータをリスの逆順に表示
void PrintReverse(const Dlist* list);

// 着目ノードを一つ後方に進める
int Next(Dlist* list);

// 着目ノードを一つ前方に戻す
int Prev(Dlist* list);

// pが指すノードの直後にノードを挿入
void InsertAfter(Dlist* list, Dnode* p, const Data* x);

// 先頭にノードを挿入
void InsertFront(Dlist* list, const Data* x);

// 末尾にノードを挿入
void InsertRear(Dlist* list, const Data* x);

// pが指すノードを削除
void Remove(Dlist* list, Dnode* p);

// 先頭ノードを削除
void RemoveFront(Dlist* list);

// 末尾ノードを削除
void RemoveRear(Dlist* list);

// 着目ノードを削除
void RemoveCurrent(Dlist* list);

// 全ノードを削除
void Clear(Dlist* list);

// 循環・重連結リストの後始末
void Terminate(Dlist* list);
