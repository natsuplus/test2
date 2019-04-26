#pragma once
#ifndef _READ_TEXT_H_
#define _READ_TEXT_H_

#include <fstream>//西川0.02
#include <iostream>//西川0.02
#include <string>//西川0.02
#include <sstream>//西川0.02
using namespace std;//西川0.02


// コメントの付け方や変数名についてアドバイスください。

//**************************************************************************
// オブジェクト構造体
//**************************************************************************
typedef struct
{
	int hp;
	int mp;
	int pos;
	// D3DXVECTOR2 pos;

}Object;

typedef struct
{

	//string *stage_file;
	Object *m_pObj = NULL;
	int num_m_pObj;	// 行数 = オブジェクトの数
}Stage;

enum
{
	HP = 0,
	MP,
	POS,
};


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************

// 呼び出し用関数　使うときはこの関数だけ
void read_Status_File(string file_name);

// 構造体Stageの初期化とテキストファイルを順番に渡していく関数
void open_All_txt(string file_name, stringstream &strstream, string *information);  // テキストファイルの数とファイル名をファイルの数だけ入れていく
void file_Open(string file_name, string *stream_txt_name, stringstream &strstream); // ファイルをオープンし、stringとstreamに情報を格納する。

// 構造体Objectに関する関数
void create_Origin_Object(stringstream &istream_single_txt, int current_txt); // 構造体を実際に作っていく関数
int how_Much_Line_and_Comma(int &line, string str, int current_txt); // 行数＝構造体の数と　,＝変数の数を数える
void substitution_Status_for_Structures_Var(istringstream &stream, int num_obj, int num_comma, int &obj_number, int current_txt); // 構造体内の変数の数だけ代入を繰り返す関数

// その他
void uninit_Stage_Struct();
Stage *get_g_pStage(int no);

#endif



