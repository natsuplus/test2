#include "object.h"
#include "updateObjProt.h"
#include "textDX.h"
void updatePlayer(ObjStr* p_obj);
void setBlackHole(ObjStr* p_obj);
void updateBlackHole(ObjStr* p_obj);
void setWhiteHole(ObjStr* p_obj);
void updateWhiteHole(ObjStr* p_obj);

void updateObjProt(StageObj* p_stgobj) {
	objTypes k;//あまり意味はない
	for (int i = 0; i < p_stgobj->m_OBJNUM; i++) {
		if (p_stgobj->m_Obj[i].m_use == false) continue;
		k = (objTypes)(i);
		switch (p_stgobj->m_Obj[i].m_type) {//タイプごとに処理分け
		case CHARA_PLAYER:
			//星
			updatePlayer(p_stgobj->m_Obj[i].m_ptr);
			break;
		case CHARA_BLACKHOLE:
			//ブラックホール
			if (getMouseLButtonTrigger) // 右クリックでセット
				setBlackHole(p_stgobj->m_Obj[i].m_ptr);

			updateBlackHole(p_stgobj->m_Obj[i].m_ptr);
			break;
		case CHARA_WHITEHOLE:
			//ホワイトホール
			updateBlackHole(p_stgobj->m_Obj[i].m_ptr);
			break;
		case CHARA_COMET:
			//隕石
			break;
		case CHARA_KEY:
			//鍵
			break;
		case CHARA_COIN:
			//収集アイテム
			break;
		case STAGE_HURDLE:
			//動かせる障害物
			break;
		case STAGE_WALL:
			//動かせない壁　地面とか
			break;
		case STAGE_LOCK:
			//鍵で開けられる扉
			break;
		case EVENT_GOAL:
			//ゴール
			break;
		case UI_CURSOR:
			//ブラックホール置くカーソル
			break;
		case UI_EFFECT:
			//エフェクト
			break;
		case UI_HP:
			//残機表示
			break;
		}
	}
}

void updatePlayer(ObjStr* p_obj) {

};
void updateBlackHole(ObjStr* p_obj) {

};
void updateWhiteHole(ObjStr* p_obj) {

};

void  setBlackHole(ObjStr* p_obj) {


};

void  setWhiteHole(ObjStr* p_obj) {
};