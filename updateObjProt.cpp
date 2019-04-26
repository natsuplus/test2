#include "object.h"
#include "updateObjProt.h"
#include "textDX.h"
void updatePlayer(ObjStr* p_obj);
void setBlackHole(ObjStr* p_obj);
void updateBlackHole(ObjStr* p_obj);
void setWhiteHole(ObjStr* p_obj);
void updateWhiteHole(ObjStr* p_obj);

void updateObjProt(StageObj* p_stgobj) {
	objTypes k;//���܂�Ӗ��͂Ȃ�
	for (int i = 0; i < p_stgobj->m_OBJNUM; i++) {
		if (p_stgobj->m_Obj[i].m_use == false) continue;
		k = (objTypes)(i);
		switch (p_stgobj->m_Obj[i].m_type) {//�^�C�v���Ƃɏ�������
		case CHARA_PLAYER:
			//��
			updatePlayer(p_stgobj->m_Obj[i].m_ptr);
			break;
		case CHARA_BLACKHOLE:
			//�u���b�N�z�[��
			if (getMouseLButtonTrigger) // �E�N���b�N�ŃZ�b�g
				setBlackHole(p_stgobj->m_Obj[i].m_ptr);

			updateBlackHole(p_stgobj->m_Obj[i].m_ptr);
			break;
		case CHARA_WHITEHOLE:
			//�z���C�g�z�[��
			updateBlackHole(p_stgobj->m_Obj[i].m_ptr);
			break;
		case CHARA_COMET:
			//覐�
			break;
		case CHARA_KEY:
			//��
			break;
		case CHARA_COIN:
			//���W�A�C�e��
			break;
		case STAGE_HURDLE:
			//���������Q��
			break;
		case STAGE_WALL:
			//�������Ȃ��ǁ@�n�ʂƂ�
			break;
		case STAGE_LOCK:
			//���ŊJ�������
			break;
		case EVENT_GOAL:
			//�S�[��
			break;
		case UI_CURSOR:
			//�u���b�N�z�[���u���J�[�\��
			break;
		case UI_EFFECT:
			//�G�t�F�N�g
			break;
		case UI_HP:
			//�c�@�\��
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