#include "stdafx.h"
#include "system/system.h"
#include "Debug.h"
#include "Game.h"
#include "Scene/Title.h"
#include "Scene/Fade.h"
// K2EngineLowのグローバルアクセスポイント。
K2EngineLow* g_k2EngineLow = nullptr;

/// <summary>
/// メイン関数
/// </summary>
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// ゲームの初期化。
	InitGame(hInstance, hPrevInstance, lpCmdLine, nCmdShow, TEXT("Katamari"));
	
	// k2EngineLowの初期化。
	g_k2EngineLow = new K2EngineLow();
	g_k2EngineLow->Init(g_hWnd, FRAME_BUFFER_W, FRAME_BUFFER_H);
	g_camera3D->SetPosition(0, 100.0f, 350.0f);
	g_camera3D->SetTarget(0, 100.0f, 0);
	
	RenderingEngine::GetInstance()->Init();

	//PhysicsWorld::GetInstance()->EnableDrawDebugWireFrame();

	//NewGO<Game>(0, "game");
	NewGO<Title>(0, "title");
	NewGO<Fade>(1, "fade");

	g_camera3D->SetNear(0.5f);
	g_camera3D->SetFar(8000.0f);
	// ここからゲームループ。
	while (DispatchWindowMessage())
	{
		auto& renderContext = g_graphicsEngine->GetRenderContext();

		// フレームの開始時に呼び出す必要がある処理を実行
		g_k2EngineLow->BeginFrame();

		// ゲームオブジェクトマネージャーの更新処理を呼び出す。
		g_k2EngineLow->ExecuteUpdate();

		// ゲームオブジェクトマネージャーの描画処理を呼び出す。
		g_k2EngineLow->ExecuteRender();

		RenderingEngine::GetInstance()->Execute(renderContext);

		// デバッグ描画処理を実行する。
		g_k2EngineLow->DebubDrawWorld();

		// フレームの終了時に呼び出す必要がある処理を実行。
		g_k2EngineLow->EndFrame();
	}

	delete g_k2EngineLow;

	return 0;
}

