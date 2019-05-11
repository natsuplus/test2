//西川
#pragma once

#include "main.h"
#include "Image.h"
#include "input.h"
enum objTypes{
	NO_TYPE = -1,
		CHARA_PLAYER,
		CHARA_BLACKHOLE,
		CHARA_WHITEHOLE,
		CHARA_COMET,
		CHARA_KEY,
		CHARA_COIN,
		STAGE_HURDLE,
		STAGE_WALL,
		STAGE_LOCK,
		EVENT_GOAL,
	FROMFILE_TYPE_MAX,
		UI_CURSOR,
		UI_EFFECT,
		UI_HP,
	TYPE_MAX
};

//shortは-0.5±32767.5、intは-0.5±2147483647.5

//************************************************************************
//オブジェクトの一つ一つ
// とりあえずメンバ変数は暫定。必要に応じて増やしたり減らしたりするかも。
// その場合は関連項目(関数とか)修正してくれるとありがたい。
//************************************************************************
typedef struct _obj{
	objTypes m_type;					//オブジェクトタイプ
	short	 m_id;						//全タイプが入っている配列内の、自分の番号
	bool	 m_use;						//useフラグ
	_obj*	 m_ptr;						//自身へのポインタ
	_obj*	 m_tar;						//ターゲットのポインタ(基本はブラックホール)

	D3DXVECTOR2	m_pos;					//座標
	float		m_rot;					//角度
	D3DXVECTOR2 m_scl;					//大きさ

	D3DXVECTOR2 m_speed;				//速度
	D3DXVECTOR2 m_accel;				//加速度(他のオブジェクトから及ぼされる力、慣性とか)
	D3DXVECTOR2 m_attract;				//引力　(自分以外のオブジェクトに及ぼす力とか)

	int		m_time;						//プレイヤーとは独立して勝手に動くオブジェクトがあれば使う(ステージギミック系)
	short	m_mode;						//同上、行動パターンとかはこっち


	float m_rad;						//半径(プレイヤーの当たり判定とか)
	D3DXVECTOR2 m_rect;					//矩形の辺長(ステージの当たり判定とか)
	//衝突以外でも使うなら配列になるかも(近寄ると動き出すとか、ブラックホールが吸い寄せ始める範囲とか)

	Image m_image;						//描画情報
}ObjStr;//Structure

typedef struct {//全部入ってる構造体
	int m_STAGE;						//ステージ番号
	int m_OBJNUM;						//オブジェクト数
	ObjStr *m_Obj;						//オブジェクト
}StageObj;

#define STUB_OBJNUM (100)	//ステージのデータ内にあるオブジェクトの総数/実際には外からもらう値
#define PLUS_OBJNUM (5)		//ステージ側で保存する必要のないオブジェクト情報(UIなど)があればそれの数


void initializeObject(StageObj* p_stgobj,int stage);
void uninitializeObject(StageObj* p_stgobj);
void updateObject(StageObj* p_stgobj);
void drawObject(StageObj* p_stgobj);
void printObject(StageObj* p_stgobj);
