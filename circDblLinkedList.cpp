// 循環・重連結リスト（ソース部）
#include <stdio.h>
#include <stdlib.h>
#include "CircDblLinkedList.h"
#include "textDX.h"

// 一つのノードを動的に生成
static Dnode* AllocDnode(void)
{
	return (Dnode*)calloc(1, sizeof(Dnode));
}

// ノードの各メンバに値を設定
static void SetDnode(Dnode* n, const Data* x, Dnode* prev, Dnode* next)
{
	n->data = *x;		// データ
	n->prev = prev;		// 先行ノードへのポインタ
	n->next = next;		// 後続ノードへのポインタ
}

// リストは空か
static int IsEmpty(const Dlist* list)
{
	return list->head->next == list->head;
}

// リストを初期化
void Initialize(Dlist* list)
{
	Dnode* dummyNode = AllocDnode(); // ダミーノードを生成
	list->head = list->crnt = dummyNode;
	dummyNode->prev = dummyNode->next = dummyNode;
}

// 着目ノードのデータを表示
void PrintCurrent(const Dlist* list)
{
	if (IsEmpty(list))
//		printf("着目ノードがありません。");
		printTextDX(getDebugFont(), "着目ノードがありません。", 0, txtLineBreak());
	else
		PrintData(&list->crnt->data);
}

// 着目ノードのデータを表示（改行付き）
void PrintLnCurrent(const Dlist* list)
{
	PrintCurrent(list);
//	putchar('\n');
	printTextDX(getDebugFont(), "", 0, txtLineBreak());
}

// 関数compareによってxと一致すると判定されるノードを探索
Dnode* Search(Dlist *list, const Data* x, int compare(const Data* x, const Data* y))
{
	Dnode* ptr = list->head->next;
	while (ptr != list->head)
	{
		if (compare(&ptr->data, x) == 0)
		{
			list->crnt = ptr;
			return ptr;			// 探索成功
		}
		ptr = ptr->next;
	}
	return NULL;				// 探索失敗
};

// 全ノードのデータをリスト順に表示
void Print(const Dlist* list)
{
	if (IsEmpty(list))
//		puts("ノードがありません。");
		printTextDX(getDebugFont(), "ノードがありません。", 0, txtLineBreak());
	else {
		Dnode* ptr = list->head->next;
//		puts("【一覧表】");
		printTextDX(getDebugFont(), "【一覧表】", 0, txtLineBreak());
		while (ptr != list->head) {
//			PrintLnData(&ptr->data);
			PrintData(&ptr->data);
			ptr = ptr->next;		// 後続ノードに着目	
		}
	}
};


// 全ノードのデータをリストの逆順に表示
void PrintReverse(const Dlist* list)
{
	if (IsEmpty(list))
//		puts("ノードがありません。");
		printTextDX(getDebugFont(), "ノードがありません。", 0, txtLineBreak());
	else {
		Dnode* ptr = list->head->prev;
//		puts("【一覧表】");
		printTextDX(getDebugFont(), "【一覧表】", 0, txtLineBreak());
		while (ptr != list->head) {
//			PrintLnData(&ptr->data);
			PrintData(&ptr->data);
			ptr = ptr->prev;		// 先行ノードに着目
		}
	}
};

// 着目ノードを一つ後方に進める
int Next(Dlist* list)
{
	if (IsEmpty(list) || list->crnt->next == list->head)
		return 0;	// 進めることはできない
	list->crnt = list->crnt->next;
	return 1;
};

// 着目ノードを一つ前方に戻す
int Prev(Dlist* list)
{
	if (IsEmpty(list) || list->crnt->prev == list->head)
		return 0;	// 戻すことはできない
	list->crnt = list->crnt->prev;
	return 1;
};


// pが指すノードの直後にノードを挿入
void InsertAfter(Dlist* list, Dnode* p, const Data* x)
{
	Dnode* ptr = AllocDnode();
	Dnode* nxt = p->next;
	p->next = p->next->prev = ptr;
	SetDnode(ptr, x, p, nxt);
	list->crnt = ptr;
};

// 先頭にノードを挿入
void InsertFront(Dlist* list, const Data* x)
{
	InsertAfter(list, list->head, x);
};

// 末尾にノードを挿入
void InsertRear(Dlist* list, const Data* x)
{
	InsertAfter(list, list->head->prev, x);
};

// pが指すノードを削除
void Remove(Dlist* list, Dnode* p)
{
	p->prev->next = p->next;
	p->next->prev = p->prev;
	list->crnt = p->prev;	// 削除したノードの先行ノードに着目
	free(p);
	if (list->crnt == list->head)
		list->crnt = list->head->next;
};

// 先頭ノードを削除
void RemoveFront(Dlist* list)
{
	if (!IsEmpty(list))
		Remove(list, list->head->next);
};

// 末尾ノードを削除
void RemoveRear(Dlist* list)
{
	if (!IsEmpty(list))
		Remove(list, list->head->prev);
};

// 着目ノードを削除
void RemoveCurrent(Dlist* list)
{
	if (list->crnt != list->head)
		Remove(list, list->crnt);
};

// 全ノードを削除
void Clear(Dlist* list)
{
	while (!IsEmpty(list))	// 空になるまで
		RemoveFront(list);	// 先頭ノードを削除
};

// 循環・重連結リストの後始末
void Terminate(Dlist* list)
{
	Clear(list);					// 全ノードを削除
	free(list->head);		// ダミーノードを削除
};
