#include "audio.h"
//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
Audio::Audio()
{
	xactEngine = NULL;
	waveBank = NULL;
	soundBank = NULL;
	cueI = 0;
	mapWaveBank = NULL;
	soundBankData = NULL;

	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (SUCCEEDED(hr))
	{
		coInitialized = true;
	}
	else
	{
		coInitialized = false;
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
Audio::~Audio()
{
	// XACTをシャットダウン
	if (xactEngine)
	{
		// XACTエンジンをシャットダウンし、リソースを解放
		xactEngine->ShutDown();
		xactEngine->Release();
	}

	if (soundBankData)
	{
		delete[] soundBankData;
	}
	soundBankData = NULL;

	// xactEngine->ShutDown()から戻った後、メモリマップドファイルを解放
	if (mapWaveBank)
	{
		UnmapViewOfFile(mapWaveBank);
	}
	mapWaveBank = NULL;

	// CoInitializeExが成功した場合
	if (coInitialized)
	{
		CoUninitialize();
	}

}

//=============================================================================
// 初期化
// この関数は次の処理を行う
//		1.xactEngine->Initializeによって呼び出されたXACTを初期化
//		2.使用するXACTウェイブバンクを作成する
//		3.使用するXACTサウンドバンクを作成する
//		4.ゲームが使用するXACTキューにインデックスを格納する
//=============================================================================
HRESULT Audio::initialize()
{
	HRESULT result = E_FAIL;
	HANDLE hFile;
	DWORD fileSize;
	DWORD bytesRead;
	HANDLE hMapFile;

	if (coInitialized == false)
	{
		return E_FAIL;
	}

	result = XACT3CreateEngine(0, &xactEngine);
	if (FAILED(result) || xactEngine == NULL)
	{
		return E_FAIL;
	}

	// XACTランタイムを初期化し、作成する
	XACT_RUNTIME_PARAMETERS xactParams = { 0 };
	xactParams.lookAheadTime = XACT_ENGINE_LOOKAHEAD_DEFAULT;
	result = xactEngine->Initialize(&xactParams);
	if (FAILED(result))
	{
		return result;
	}

	// メモリマップトファイルIOを使用する "インメモリ"の XACTウェイブバンクファイルを作成
	result = E_FAIL; // 失敗をデフォルトとして、成功時に置き換える
	hFile = CreateFile(WAVE_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(hFile, NULL);
		if (fileSize != -1)
		{
			hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, fileSize, NULL);
			if (hMapFile)
			{
				mapWaveBank = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
				if (mapWaveBank)
				{
					result = xactEngine->CreateInMemoryWaveBank(mapWaveBank, fileSize, 0, 0, &waveBank);
				}
				CloseHandle(hMapFile);	// mapWaveBankがファイルへのハンドルを保持するのでこの不要なハンドルを閉じる
			}
		}
		CloseHandle(hFile);		// mapWaveBankはがファイルへのハンドルを保持するのでこの不要なハンドルを閉じる
	}

	if (FAILED(result))
	{
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	}

	// XACTで作成したサウンドバンクファイルを読み込んで予約
	result = E_FAIL;	// 失敗をデフォルトとして、成功時に置き換える
	hFile = CreateFile(SOUND_BANK, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		fileSize = GetFileSize(hFile, NULL);
		if (fileSize != -1)
		{
			soundBankData = new BYTE[fileSize];		// サウンドバンクのメモリを確保する
			if (soundBankData)
			{
				if (0 != ReadFile(hFile, soundBankData, fileSize, &bytesRead, NULL))
				{
					result = xactEngine->CreateSoundBank(soundBankData, fileSize, 0, 0, &soundBank);
				}
			}
		}
		CloseHandle(hFile);
	}
	if (FAILED(result))
	{
		return HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
	}
	return S_OK;
}

//=============================================================================
// 周期的なサウンドエンジンタスクを実行
//=============================================================================
void Audio::run()
{
	if (xactEngine == NULL)
	{
		return;
	}
	xactEngine->DoWork();
}

//=============================================================================
// キューで指定されたサウンドをサウンドバンクらから再生
// キューが存在しない場合は、サウンドが再生されないだけで、エラーは発生しない
//=============================================================================
void Audio::playCue(const char cue[])
{
	if (soundBank == NULL)
	{
		return;
	}
	// サウンドバンクからキューインデックスを取得
	cueI = soundBank->GetCueIndex(cue);	
	soundBank->Play(cueI, 0, 0, NULL);
}

//=============================================================================
// キューで指定されたサウンドをサウンドバンクで停止
// キューが存在しない場合、エラーは発生しない
//=============================================================================
void Audio::stopCue(const char cue[])
{
	if (soundBank == NULL)
	{
		return;
	}
	cueI = soundBank->GetCueIndex(cue);
	soundBank->Stop(cueI, XACT_FLAG_SOUNDBANK_STOP_IMMEDIATE);
}

