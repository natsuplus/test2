//西川
#include "object.h"

#include "updateObjProt.h"
//#include "stdlib.h"//malloc用
#include"textureLoader.h"
#include "read_text.h"
#include "textDX.h"

//仮置き構造体たち
struct StubObject {
	objTypes m_type;					//オブジェクトタイプ
	short	 m_id;						//全タイプが入っている配列内の、自分の番号
	bool	 m_use;						//useフラグ

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
}StubObj[STUB_OBJNUM];//実際には外からもらう値

struct StubTexture {
	objTypes m_type;

	textureLoaderNS::TEXTURE_NUMBER texture;
	float width;
	float height;
	bool playAnime;
	int ANIME_TIME;
	int ANIME_PATTERN;
	int DIVIDE_U;
	int DIVIDE_V;
}StubTex[TYPE_MAX];//実際には外からもらう値

//オブジェクトをセットする系ローカル関数
ObjStr cleanObj(int i);
Image cleanTex(void);

void setObjFromFile(ObjStr* p_obj, int i, Stage* p_fromstage);
void setObjNotFrom(ObjStr* p_obj, int i, int objnum);
void setObjTex(ObjStr* p_obj, int i);

/*コリジョンリストに必要そうなもの
obID/obType/pos/rot/scl/coltype

アクションリスト

エフェクトリスト
*/



void initializeObject(StageObj *p_stgobj,int stage){
	p_stgobj->m_STAGE = stage;
	Stage *p_fromstage = get_g_pStage(stage);//今から取得するステージ

	p_stgobj->m_OBJNUM = p_fromstage->num_m_pObj + PLUS_OBJNUM;
	objTypes k;//あまり意味はない

	//↓ステージのオブジェクトたちの初期値＝ステージデータ/実際には外からもらう値
	//今は仮としてStugObjを初期化
	for (int i = 0; i < p_stgobj->m_OBJNUM; i++) {
		k = (objTypes)(i);
		if (NO_TYPE < k && k < FROMFILE_TYPE_MAX) {
			StubObj[i].m_type = k;
			StubObj[i].m_id = i;
			StubObj[i].m_pos = { WINDOW_WIDTH * float(StubObj[i].m_type) / float(FROMFILE_TYPE_MAX),0.0f };
			if (k == CHARA_PLAYER)
				StubObj[i].m_pos = { 0.0f,(float)WINDOW_CENTER_Y };
			StubObj[i].m_rot = D3DX_PI * 0.5f;
			StubObj[i].m_scl = { 10.0f,10.0f };
			StubObj[i].m_speed = { 0.0f,0.0f };
			StubObj[i].m_accel = { 0.0f,0.0f };
			StubObj[i].m_attract = { 0.0f,0.0f };
			StubObj[i].m_time = -1;
			StubObj[i].m_mode = -1;
			StubObj[i].m_rad = 50.0f;
			StubObj[i].m_rect = { 100.0f,100.0f };
		}
		else {//ステージデータの中で空になっているもの(実際にはないかも)
			StubObj[i].m_type = NO_TYPE;
		}
	}
	//↑ステージのオブジェクトたちの初期値＝ステージデータ/実際には外からもらう値



	//↓ステージのテクスチャたちの初期値＝テクスチャリスト/実際には外からもらう値
	for (int j = 0; j < TYPE_MAX; j++) {
		k = (objTypes)(j);
		if (NO_TYPE < k && k < TYPE_MAX && k != FROMFILE_TYPE_MAX) {
			StubTex[j].m_type = k;
			switch (k) {
			case CHARA_PLAYER:
				StubTex[j].texture = (textureLoaderNS::STAR);
				StubTex[j].width = 100.0f;
				StubTex[j].height = 100.0f;
				StubTex[j].playAnime = false;
				StubTex[j].ANIME_TIME = 1;
				StubTex[j].ANIME_PATTERN = 1;
				StubTex[j].DIVIDE_U = 1;
				StubTex[j].DIVIDE_V = 1;
				break;
			case CHARA_BLACKHOLE:
				StubTex[j].texture = (textureLoaderNS::BLACK_HOLE);
				StubTex[j].width = 100.0f;
				StubTex[j].height = 100.0f;
				StubTex[j].playAnime = false;
				StubTex[j].ANIME_TIME = 1;
				StubTex[j].ANIME_PATTERN = 1;
				StubTex[j].DIVIDE_U = 1;
				StubTex[j].DIVIDE_V = 1;
				break;
			case CHARA_WHITEHOLE:
				StubTex[j].texture = (textureLoaderNS::WHITE_HOLE);
				StubTex[j].width = 100.0f;
				StubTex[j].height = 100.0f;
				StubTex[j].playAnime = false;
				StubTex[j].ANIME_TIME = 1;
				StubTex[j].ANIME_PATTERN = 1;
				StubTex[j].DIVIDE_U = 1;
				StubTex[j].DIVIDE_V = 1;
				break;
			case CHARA_COMET:
				StubTex[j].texture = (textureLoaderNS::COMET);
				StubTex[j].width = 100.0f;
				StubTex[j].height = 100.0f;
				StubTex[j].playAnime = false;
				StubTex[j].ANIME_TIME = 1;
				StubTex[j].ANIME_PATTERN = 1;
				StubTex[j].DIVIDE_U = 1;
				StubTex[j].DIVIDE_V = 1;
				break;
			case UI_EFFECT:
				StubTex[j].texture = (textureLoaderNS::BACK_GROUND);
				StubTex[j].width = 160.0f;
				StubTex[j].height = 80.0f;
				StubTex[j].playAnime = true;
				StubTex[j].ANIME_TIME = 3;
				StubTex[j].ANIME_PATTERN = 5;
				StubTex[j].DIVIDE_U = 10;
				StubTex[j].DIVIDE_V = 5;
				break;
			case UI_CURSOR:
				StubTex[j].texture = (textureLoaderNS::PLANET);
				StubTex[j].width = 240.0f;
				StubTex[j].height = 120.0f;
				StubTex[j].playAnime = true;
				StubTex[j].ANIME_TIME = 10;
				StubTex[j].ANIME_PATTERN = 4;
				StubTex[j].DIVIDE_U = 2;
				StubTex[j].DIVIDE_V = 2;
				break;
			default:
				StubTex[j].texture = (textureLoaderNS::BACK_GROUND);
				StubTex[j].width = 100.0f;
				StubTex[j].height = 100.0f;
				StubTex[j].playAnime = false;
				StubTex[j].ANIME_TIME = 1;
				StubTex[j].ANIME_PATTERN = 1;
				StubTex[j].DIVIDE_U = 1;
				StubTex[j].DIVIDE_V = 1;
				break;
			}
		}
		else {//テクスチャリストの中で空になっているもの(実際にはないかも)
			//現状FROMFILE_TYPE_MAXだったとき
			StubTex[j].m_type = NO_TYPE;
		}
	}
	//↑ステージのテクスチャたちの初期値＝テクスチャリスト/実際には外からもらう値

//	if (p_stgobj->m_Obj != NULL) uninitializeObject(p_stgobj);//空でなかったら空に

//	Obj = (ObjStr*)malloc(sizeof(ObjStr) * OBJNUM);
	p_stgobj->m_Obj = new ObjStr[p_stgobj->m_OBJNUM];

	if (p_stgobj->m_Obj == NULL) return;//malloc/newは確保に失敗することもあるらしく、以降がぜんぶおかしくなると思うのでfalseを返す(返された側で終了するなり無視するなりしてほしい)

	int BHid;
	for (int i = 0; i < p_stgobj->m_OBJNUM; i++) {
		if (i < p_stgobj->m_OBJNUM) {//オブジェクトにオブジェクトデータからもらった値をセット
			setObjFromFile(&(p_stgobj->m_Obj[i]),i, p_fromstage);
		}
		else {//オブジェクトにオブジェクトデータからもらっていない値をセット
			setObjNotFrom(&(p_stgobj->m_Obj[i]),i, p_stgobj->m_OBJNUM);
		}

		//オブジェクトにテクスチャデータからもらった値をセット
		setObjTex(&(p_stgobj->m_Obj[i]),i);

		switch (p_stgobj->m_Obj[i].m_type) {//タイプごとの、わざわざファイルに書く必要はないが、ゲーム向けにする必要はある初期化
		case CHARA_PLAYER:
			//星
			p_stgobj->m_Obj[i].m_use = true;
			break;
		case CHARA_BLACKHOLE:
			//ブラックホール
			BHid = p_stgobj->m_Obj[i].m_id;
			break;
		case CHARA_WHITEHOLE:
			//ホワイトホール
			p_stgobj->m_Obj[i].m_use = true;
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

	for (int i = 0; i < p_stgobj->m_OBJNUM; i++) {
		p_stgobj->m_Obj[i].m_tar = p_stgobj->m_Obj[BHid].m_ptr;
	}

}

void uninitializeObject(StageObj* p_stgobj) {
	if (p_stgobj->m_Obj != NULL) {
//		free(Obj);
		delete[]p_stgobj->m_Obj;
		p_stgobj->m_Obj = NULL;
		//誤ってfree/deleteを２連続でやるとヤバい かつ NULLぽがfree/deleteされる分には平気 らしいのでこうした
	}
}

void updateObject(StageObj* p_stgobj) {
	//仮なのでプロトタイプの方に処理飛ばす
	updateObjProt(p_stgobj);
}

void drawObject(StageObj* p_stgobj){
	//全てのオブジェクトを描画　※これだと描画順をタイプごとに揃えたりできないので、描画用のリストを作ることになるのかも
	for (int i = 0; i < p_stgobj->m_OBJNUM; i++) {
//		p_stgobj->m_Obj[i].m_pos.y += 0.01f;
		if (p_stgobj->m_Obj[i].m_use == false) continue;
		setPosition(&(p_stgobj->m_Obj[i].m_image), p_stgobj->m_Obj[i].m_pos.x , p_stgobj->m_Obj[i].m_pos.y);
		DrawImage(&(p_stgobj->m_Obj[i].m_image));
	}
}

void printObject(StageObj* p_stgobj) {
	for (int i = 0; i < p_stgobj->m_OBJNUM; i++) {
		txtLineReset(int(p_stgobj->m_Obj[i].m_pos.y + p_stgobj->m_Obj[i].m_image.height));//自動改行位置をセット
		printTextDX(getDebugFont(), "type", int(p_stgobj->m_Obj[i].m_pos.x), txtLineBreak(), int(p_stgobj->m_Obj[i].m_type));
		printTextDX(getDebugFont(), "id", int(p_stgobj->m_Obj[i].m_pos.x), txtLineBreak(), p_stgobj->m_Obj[i].m_id);
		printTextDX(getDebugFont(), "x", int(p_stgobj->m_Obj[i].m_pos.x), txtLineBreak(), p_stgobj->m_Obj[i].m_pos.x);
		printTextDX(getDebugFont(), "y", int(p_stgobj->m_Obj[i].m_pos.x), txtLineBreak(), p_stgobj->m_Obj[i].m_pos.y);
	}
}


/////以下オブジェクトセット系ローカル関数

ObjStr cleanObj(int i) {//Objをリセット
	return {
		NO_TYPE,
		short(i),
		false,
		NULL,
		NULL,
		{0.0f,0.0f},
		0.0f,
		{0.0f,0.0f},
		{0.0f,0.0f},
		{0.0f,0.0f},
		{0.0f,0.0f},
		int(0),
		short(0),
		0.0f,
		{0.0f,0.0f}
//		cleanTex()
	};
}

Image cleanTex(void) {//Texをリセット
	Image nullTex;
	InitImage(&nullTex, getTexture(textureLoaderNS::TEXTURE_NUMBER(0)), 0, 0, 1, 1);
	return nullTex;
}

void setObjFromFile(ObjStr *obj, int i, Stage* p_fromstage){
	//もらったステージデータの中からオブジェクトをセット

	if (StubObj[i].m_type != NO_TYPE) {//ステージデータから中身を直接もらえばいいもの
		obj->m_type = StubObj[i].m_type;
		obj->m_id = StubObj[i].m_id;
		obj->m_use = false;
		obj->m_ptr = obj;
		obj->m_tar = NULL;

		obj->m_pos = StubObj[i].m_pos;
		obj->m_rot = StubObj[i].m_rot;
		obj->m_scl = StubObj[i].m_scl;

		obj->m_speed = StubObj[i].m_pos;
		obj->m_accel = StubObj[i].m_accel;
		obj->m_attract = StubObj[i].m_attract;

		obj->m_time = StubObj[i].m_time;
		obj->m_mode = StubObj[i].m_mode;

		obj->m_rad = StubObj[i].m_rad;
		obj->m_rect = StubObj[i].m_rect;
	}
	else {//ステージデータが空のタイプになっていたら
		*obj = cleanObj(i);
	}
}

void setObjNotFrom(ObjStr *obj, int i, int objnum) {
	//UIなど、ステージごとに変わらないのでファイルに保存する必要もなく、毎回こちらで設定すればいいオブジェクトをセット

	obj->m_id = i;
	obj->m_use = true;
	switch (i - objnum) {
	case 0:
		obj->m_type = UI_CURSOR;
		obj->m_pos = { (float)WINDOW_CENTER_X,(float)WINDOW_CENTER_Y };//マウスカーソルの移動ってどうやるんだろ
		break;
	case 1:
		obj->m_type = UI_EFFECT;
		obj->m_pos = { 720.0f,480.0f };
		break;
	default:
		obj->m_type = UI_HP;
		obj->m_pos = { 0.0f,0.0f };
		break;
	}
}



void setObjTex(ObjStr* obj, int i) {//ObjにStubTexの中身をセット
	if (obj->m_type != NO_TYPE) {
		StubTexture *p_tex = &(StubTex[int(obj->m_type)]);
		if (p_tex->m_type != NO_TYPE) {
			if (obj->m_image.playAnime == false) {//静止画だったら
				InitImage(&(obj->m_image), getTexture(StubTex[int(obj->m_type)].texture), obj->m_pos.x, obj->m_pos.y,
					p_tex->width, p_tex->height);
			}
			else {//アニメだったら
				InitAnimeImage(&(obj->m_image), getTexture(StubTex[int(obj->m_type)].texture), obj->m_pos.x, obj->m_pos.y,
					p_tex->width, p_tex->height,
					p_tex->ANIME_TIME, p_tex->ANIME_PATTERN, p_tex->DIVIDE_U, p_tex->DIVIDE_V);
			}
		}
		else {//テクスチャデータが空のタイプになっていたら
			obj->m_image = cleanTex();
		}
	}
	else {//オブジェクトデータが空のタイプになっていたら
		obj->m_image = cleanTex();
	}
}
