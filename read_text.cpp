#include <stdio.h>
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS//西川0.02
//#include <fstream>//西川0.02
//#include <iostream>//西川0.02
//#include <string>//西川0.02
//#include <sstream>//西川0.02
//using namespace std;//西川0.02
#include "read_text.h"

////=============================================================================
////　グローバル変数
////=============================================================================
Stage *g_pStage;

// 呼び出し用関数
void read_Status_File(string file_name)
{
	string *information = NULL; // 読み込んだファイルの情報の格納先
	stringstream stream_txt_name; // 読み込んだファイル情報を文字列データへ変換する

	// インフォメーションファイルを開く
	file_Open(file_name, information, stream_txt_name);
	open_All_txt(file_name, stream_txt_name, information);
}

// テキストの数字をそれぞれの変数に入れ込んでいく。
void substitution_Status_for_Structures_Var(istringstream &stream, int num_obj, int num_comma, int &obj_number, int current_txt)
{
	int raw = NULL;	// 何列（,）目？
	string status;

	while (getline(stream, status, ','))
	{
		switch (raw)
		{
		case HP:
			g_pStage[current_txt].m_pObj[obj_number].hp = atoi(status.c_str());
			raw++;
			continue;

		case MP:
			g_pStage[current_txt].m_pObj[obj_number].mp = atoi(status.c_str());
			raw++;
			continue;

		case POS:
			g_pStage[current_txt].m_pObj[obj_number].pos = atoi(status.c_str());
			raw++;
			continue;
		}
	}
	printf("%d \n", g_pStage[current_txt].m_pObj[obj_number].hp);
	obj_number++;
}

// ファイルをオープンし、stringとstreamに情報を格納する。
void file_Open(string file_name, string *information, stringstream &strstream)
{
	ifstream fin; // ファイル読み込みクラス

	file_name.insert(0, "stage\\");

	fin.open(file_name, ios::in); // 指定したファイルを開く
	if (!fin) {//エラー検知
		cout << "Error: cannot open file(" << file_name << ")" << endl;
		// 終了待機
		cout << "Ctrl+Z  Enter で終了\n";
		char  ch;
		while (cin.get(ch));
		//return 0;
	}
	strstream << fin.rdbuf();// 読み込んだファイル情報を文字列ストリームへ格納
	information = &strstream.str();// 文字列ストリームの文字情報を格納
	fin.close(); // ファイルを閉じる
}

// テキストファイルの数だけ構造体を作り、オブジェクトを作っていく
void open_All_txt(string file_name, stringstream &stream_txt_name, string *information)
{
	string single_txt_name;
	stringstream stream_single_txt;
	int current_txt = 0;
	int number_of_txt = 0;

	while (getline(stream_txt_name, single_txt_name, ','))
	{
		if (current_txt > number_of_txt)
		{
			break;
		}

		istringstream isingle_txt_name(single_txt_name);

		if (number_of_txt == 0)
		{
			// テキストの数だけ構造体を作る
			number_of_txt = atoi(single_txt_name.c_str());
			g_pStage = (Stage*)malloc(number_of_txt * sizeof(Stage));
			current_txt++;
			continue;
		}

		stream_single_txt.str("");
		stream_single_txt.clear(stringstream::goodbit);
		file_Open(single_txt_name, information, stream_single_txt); // stageファイルを順番に読んでいく
		create_Origin_Object(stream_single_txt, current_txt);  // オブジェクトを作る
		current_txt++; // 次のテキストに移る

	}
}

// 実際に構造体に数字を代入していく
void create_Origin_Object(stringstream &istream_single_txt, int current_txt)
{
	int num_comma; // コンマの数
	int line_number = NULL;		// 今何行目？
	int obj_number = 0;
	current_txt--;

	string str_current_line;		// １行を保存する

	while (getline(istream_single_txt, str_current_line))
	{
		istringstream istr_current_line(str_current_line);

		// オブジェクトの数とコンマの数を代入
		if (line_number == 1)
		{
			num_comma = how_Much_Line_and_Comma(line_number, str_current_line, current_txt);
		}
		if (line_number == 0)
		{
			g_pStage[current_txt].num_m_pObj = how_Much_Line_and_Comma(line_number, str_current_line, current_txt);
		}

		if (line_number > 2)
		{
			substitution_Status_for_Structures_Var(istr_current_line, g_pStage[current_txt].num_m_pObj, num_comma, obj_number, current_txt);
		}

		if (line_number == 2)
		{
			line_number++;
		}
	}
}

// 行の数と , の数を数える
int how_Much_Line_and_Comma(int &line, string str, int current_txt)
{
	int num;

	if (line == 0)
	{
		num = atoi(str.c_str());
		g_pStage[current_txt].m_pObj = (Object*)malloc(num * sizeof(Object));
		line++;
		return num;
	}
	else if (line == 1)
	{
		num = atoi(str.c_str());
		line++;
		return num;
	}
}

Stage *get_g_pStage(int no) { return (&g_pStage[no]); }

void uninit_Stage_Struct()
{
	free(g_pStage);
}
