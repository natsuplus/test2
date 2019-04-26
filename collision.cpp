#include "collision.h"

//オブジェクト同士の当たり判定(円*円)
bool checkHitObjCC(ObjStr* p_obj1, ObjStr* p_obj2){
	//現状、オブジェクトのm_posは左上の座標なので、まず中心を出す
	D3DXVECTOR2 pos1 = { (p_obj1->m_pos.x + p_obj1->m_rect.x / 2),(p_obj1->m_pos.y + p_obj1->m_rect.y / 2) };
	D3DXVECTOR2 pos2 = { (p_obj2->m_pos.x + p_obj2->m_rect.x / 2),(p_obj2->m_pos.y + p_obj2->m_rect.y / 2) };

	if (((pos1.x - pos2.x)*(pos1.x - pos2.x)) + ((pos1.y - pos2.y)*(pos1.y - pos2.y))
		<= ((p_obj1->m_rad + p_obj2->m_rad) * (p_obj1->m_rad + p_obj2->m_rad))){
		return true;
	}

	return false;
};

//オブジェクト同士の当たり判定(矩形*矩形)
bool checkHitObjRR(ObjStr* p_obj1, ObjStr* p_obj2) {

	if (((p_obj1->m_pos.x > p_obj2->m_pos.x && p_obj1->m_pos.x < p_obj2->m_pos.x + p_obj2->m_rect.x) ||
		(p_obj2->m_pos.x > p_obj1->m_pos.x && p_obj2->m_pos.x < p_obj1->m_pos.x + p_obj1->m_rect.x)) &&
		((p_obj1->m_pos.y > p_obj2->m_pos.y && p_obj1->m_pos.y < p_obj2->m_pos.y + p_obj2->m_rect.y) ||
		(p_obj2->m_pos.y > p_obj1->m_pos.y && p_obj2->m_pos.y < p_obj1->m_pos.y + p_obj1->m_rect.y))){
		return true;
	}
	else{
		return false;
	}

};

//オブジェクト同士の当たり判定(円*矩形)
bool checkHitObjCR(ObjStr* p_obj1, ObjStr* p_obj2) {
	//現状、オブジェクトのm_posは左上の座標なので、まず中心を出す
	D3DXVECTOR2 pos1 = { (p_obj1->m_pos.x + p_obj1->m_rect.x / 2),(p_obj1->m_pos.y + p_obj1->m_rect.y / 2) };

	//円の中心点から矩形の１番近いX座標
	D3DXVECTOR2 pos;
	if (pos1.x < p_obj1->m_pos.x){
		pos.x = p_obj1->m_pos.x;
	}
	else if (pos1.x > p_obj1->m_pos.x + p_obj1->m_rect.x) {
		pos.x = p_obj1->m_pos.x + p_obj1->m_rect.x;
	}
	else {
		pos.x = pos1.x;
	}
	//円の中心点から矩形の１番近いY座標
	if (pos1.y < p_obj1->m_pos.y) {
		pos.y = p_obj1->m_pos.y;
	}
	else if (pos1.y > p_obj1->m_pos.y + p_obj1->m_rect.y) {
		pos.y = p_obj1->m_pos.y + p_obj1->m_rect.y;
	}
	else {
		pos.x = pos1.y;
	}

	//それを元に衝突判定
	float distance[2] = {fabsf(pos1.x - pos.x),fabsf(pos1.y - pos.y)};
	float dis = sqrtf((distance[0]* distance[0])+(distance[1]*distance[1]));
	if (dis < p_obj1->m_rad){
		return true;
	}
	return false;

};
