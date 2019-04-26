#include "main.h"
#include "input.h"
#include "game.h"
#include "textDX.h"
#include "Timer.h"

// グローバル変数
HWND g_hWnd = NULL;
LARGE_INTEGER timeStart;	// パフォーマンスカウンターの開始値
LARGE_INTEGER timeEnd;		// パフォーマンスカウンターの終了値
LARGE_INTEGER timerFreq;	// パフォーマンスカウンターの周波数
float frameTime;						// フレームタイム
float fps = FRAME_RATE;		// フレームレート（1秒あたりのフレーム数）
DWORD sleepTime;			// フレーム間でスリープする時間（ミリ秒）
Timer gameTimer = Timer(1000000000);//ゲームタイマー
Audio audio;
bool initialized = false;


LPDIRECT3D9 pD3D = nullptr;
LPDIRECT3DDEVICE9 pDevice = nullptr;
LPDIRECT3DVERTEXBUFFER9 pVBuffer = nullptr;
LPD3DXSPRITE sprite; // テキスト描画用スプライト

//関数プロトタイプの宣言
HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void initialize(HINSTANCE hInstance, HWND hWnd);
void update();
void draw();
void printText();
void unInitialize();

HRESULT InitDX9(HWND hwnd);
void ExitDX9();
void RenderDX9();

// エントリー関数
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR szStr, INT nCmdShow)
{
	//メモリリークデバッグ
#ifdef _DEBUG	
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// ウィンドウ作成
	if (FAILED(InitWindow(hInst, nCmdShow)))
	{
		return 0;//作成失敗
	}

	// DirectX9デバイスの初期化
	if (FAILED(InitDX9(g_hWnd))) {
		ExitDX9();
		return 0;
	}
	
	// 初期化処理
	initialize(hInst,g_hWnd);


	// メッセージループ
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{// メッセージがある場合
			TranslateMessage(&msg);// DispatchMessageと対に使用する
			DispatchMessage(&msg);// ウィンドウプロシージャにメッセージを転送する
		}
		else
		{// メッセージがない場合
		 

			// アプリケーション処理
			update();
			RenderDX9();
		}
	}

	// 終了処理
	unInitialize();
	ExitDX9();

	// 終了
	return (INT)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{// ウィンドウプロシージャ
	if (!initialized)
	{
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}
	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch ((char)wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);// アプリケーション終了
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CHAR:					// 文字が入力された
		keyIn(wParam);
		return 0;
	case WM_MOUSEMOVE:				// マウスが移動された
		mouseIn(lParam);
		return 0;
	case WM_INPUT:					// マウスからのローデータ入力
		mouseRawIn(lParam);
		return 0;
	case WM_LBUTTONDOWN:			// 左マウスボタンが押された
		setMouseLButton(true);
		mouseIn(lParam);
		return 0;
	case WM_LBUTTONUP:				// 左マウスボタンが離された
		setMouseLButton(false);
		mouseIn(lParam);
		return 0;
	case WM_MBUTTONDOWN:			// 中央マウスボタンが押された
		setMouseMButton(true);
		mouseIn(lParam);
		return 0;
	case WM_MBUTTONUP:				// 中央マウスボタンが離された
		setMouseMButton(false);
		mouseIn(lParam);
		return 0;
	case WM_RBUTTONDOWN:			// 右マウスボタンが押された
		setMouseRButton(true);
		mouseIn(lParam);
		return 0;
	case WM_RBUTTONUP:				// 右マウスボタンが離された
		setMouseRButton(false);
		mouseIn(lParam);
		return 0;
		// マウスのXボタンが押された/離された
	case WM_XBUTTONDOWN:case WM_XBUTTONUP:
		setMouseXButton(wParam);
		mouseIn(lParam);
		return 0;
	}

	// アプリケーションで処理しないメッセージをOSに処理してもらう
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	// ウィンドウの初期化
	static char szAppName[] = "GameProtType.ver.0.02";

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	// ウィンドウ構造体の定義
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = szAppName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// ウィンドウの登録
	if (!RegisterClassEx(&wc))
	{
		return E_FAIL;
	}

	//ウインドウのクライアント領域（=DirectXの描画領域）を指定
	RECT rc = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウの作成
	g_hWnd = CreateWindow(szAppName, szAppName, WS_OVERLAPPEDWINDOW,
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, hInstance, 0);

	// クライアント領域がGAME_WIDTH x GAME_HEIGHTとなるように
	// ウィンドウサイズを調整
	RECT clientRect;
	// ウィンドウのクライアント領域のサイズを取得
	GetClientRect(g_hWnd, &clientRect);
	MoveWindow(g_hWnd,
		0,												// 左
		0,												// 上
		WINDOW_WIDTH + (WINDOW_WIDTH - clientRect.right),	// 右
		WINDOW_HEIGHT + (WINDOW_HEIGHT - clientRect.bottom),// 下
		TRUE);											// ウィンドウを再描画

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	return S_OK;
}


template<typename LPD3D>
void Release(LPD3D& p)
{
	if (p)p->Release();
	p = nullptr;
}

HRESULT InitDX9(HWND hwnd)
{
	pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)return E_FAIL;

	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	HRESULT hr = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &pDevice);

	if (FAILED(hr))return hr;

	// スプライトの作成
	D3DXCreateSprite(pDevice, &sprite);

	return S_OK;
}

void ExitDX9()
{
	safeRelease(sprite);
	Release(pDevice);
	Release(pD3D);
}

void RenderDX9()
{
	pDevice->Clear(0, 0, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	pDevice->BeginScene();
	// 描画
	draw();

	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	// テキスト情報の描画
	printText();
	sprite->End();

	pDevice->EndScene();
	// バックバッファとフロントバッファの入れ替え
	pDevice->Present(NULL, NULL, NULL, NULL);
}

// 初期化処理
void initialize(HINSTANCE hInstance, HWND hWnd)
{
	// 高分解能タイマーの準備を試みる
	QueryPerformanceFrequency(&timerFreq);
	QueryPerformanceCounter(&timeStart); // 開始時間を取得
	
	audio.initialize();
	InitInput(hInstance,hWnd,false);
	initializeTextDX();
	initializeGame();
	initialized = true;
}

// 更新処理
void update()
{// 最後のフレームからの経過時間を計算、frameTimeに保存
	QueryPerformanceCounter(&timeEnd);
	frameTime = (float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart;

	// 省電力コード(winmm.hが必要)
	// 希望するフレームレートに対して経過時間が短い場合
	if (frameTime < MIN_FRAME_TIME)
	{
		sleepTime = (DWORD)((MIN_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);		// 1ミリ秒の分解能をWindowsタイマーに要求
		Sleep(sleepTime);		// sleepTimeの間、CPUを解放
		timeEndPeriod(1);		// 1ミリ秒のタイマー分解能を終了
		return;
	}

	if (frameTime > 0.0)
	{
		fps = (fps*0.99f) + (0.01f / frameTime);	// 平均fps
	}

	if (frameTime > MAX_FRAME_TIME)					// フレームレートが非常に遅い場合
	{
		frameTime = MAX_FRAME_TIME;					// 最大フレームレートを制限
	}

	timeStart = timeEnd;

	gameTimer.update(frameTime);

	audio.run();
	UpdateInput();
	updateGame();
}

// 描画処理
void draw()
{
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	drawGame();

}

void printText() 
{
	int base = 0;
	int line = 30;
	base += line;
	printTextDX(getDebugFont(), "gameTime:", 0, base,gameTimer.getTime()); base += line;
	printTextDX(getDebugFont(), "FPS:", 0, base,(int)fps); base += line;
	printGame();
}

// 終了処理
void unInitialize()
{
	UninitInput();
	unInitializeTextDX();
	unInitializeGame();
}

//=============================================================================
// ウィンドウハンドルの取得
//=============================================================================
HWND getHWnd()
{
	return g_hWnd;
}
//=============================================================================
// デバイスの取得
//=============================================================================
LPDIRECT3DDEVICE9 getDevice(void)
{
	return pDevice;
}
//=============================================================================
// スプライトの取得
//=============================================================================
Audio* getAudio(void)
{
	return &audio;
}
//=============================================================================
// スプライトの取得
//=============================================================================
LPD3DXSPRITE getSprite(void)
{
	return sprite;
}

//=============================================================================
// フレームタイムの取得
//=============================================================================
float getFrameTime(void)
{
	return frameTime;
}

//=============================================================================
// アルファ透明度を持つクワッド（四角形）を表示します。
// 実行前：createVertexBufferは、時計回りの順序でクワッドを定義する4つの頂点を含む
//         vertexBufferを作成するために使用された。
//         g3ddev-> BeginSceneが呼び出された。
//=============================================================================
bool drawQuad(LPDIRECT3DVERTEXBUFFER9 vertexBuffer)
{
	HRESULT result = E_FAIL;	// ウィンドウズの標準戻り値

	if (vertexBuffer == NULL)
	{
		return false;
	}

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);	// アルファブレンド有効にする

	pDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(VertexC));
	pDevice->SetFVF(D3DFVF_VERTEX);
	result = pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false); // アルファブレンド無効にする

	if (FAILED(result))
	{
		return false;
	}
	return true;

}
//=============================================================================
// 頂点バッファを作成します。
// 実行前：verts[]は頂点データを含んでいます。
//         size = verts[]のサイズ
// 実行後：成功した場合、&vertexBufferはバッファーを指します。
//=============================================================================
HRESULT createVertexBuffer(VertexC verts[], UINT size, LPDIRECT3DVERTEXBUFFER9 &vertexBuffer)
{
	// ウィンドウズの標準戻り値
	HRESULT result = E_FAIL;

	// 頂点バッファを作成
	result = pDevice->CreateVertexBuffer(size, D3DUSAGE_WRITEONLY, D3DFVF_VERTEX, D3DPOOL_DEFAULT, &vertexBuffer, NULL);

	if (FAILED(result))
	{
		return result;
	}

	void *ptr;

	//データが転送されてくる前に、バッファをロックする必要があります
	result = vertexBuffer->Lock(0, size, (void**)&ptr, 0);
	if (FAILED(result))
	{
		return result;
	}
	memcpy(ptr, verts, size);	// 頂点データをバッファにコピーする
	vertexBuffer->Unlock();		// バッファをアンロック

	return result;
}