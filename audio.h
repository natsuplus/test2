#pragma once

#define WIN32_LEAN_AND_MEAN

const char MAIN_BGM[] = "mainBGM";

// audio.cppに必要なオーディオファイル
// WAVE_BANKは.xwbファイルの場所でなければなりません。
const char WAVE_BANK[] = "audio\\Win\\WavesBank.xwb";
// SOUND_BANKは.xsbファイルの場所でなければなりません。
const char SOUND_BANK[] = "audio\\Win\\Sound Bank.xsb";

#include <xact3.h>

class Audio
{
//プロパティ
private:
	IXACT3Engine * xactEngine;		// XACTエンジンへのポインタ
	IXACT3WaveBank* waveBank;		// XACTウェイブバンクへのポインタ
	IXACT3SoundBank* soundBank;		// XACTサウンドバンクへのポインタ
	XACTINDEX cueI;					// XACTサウンドインデックス
	void* mapWaveBank;				// UnmapViewOfFile()を呼び出してファイルを解放
	void* soundBankData;
	bool coInitialized;				// CoInitializeExが成功した場合、trueを設定

public:
	// コンストラクタ
	Audio();
	// デストラクタ
	virtual ~Audio();

	// メンバ関数
	// オーディオの初期化
	HRESULT initialize();

	// サウンドエンジン周期的なタスクを実行
	void run();

	// キューで指定されたサウンドをサウンドバンクから再生
	// キューが存在しない場合は、エサウンドが再生されないだけで、エラーは発生しない
	void playCue(const char cue[]);

	// キューで指定されたサウンドをサウンドバンクで停止
	// キューが存在しない場合、エラーは発生しない
	void stopCue(const char cue[]);

};
