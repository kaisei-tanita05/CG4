#include <Windows.h>
#include <KamataEngine.h>
#include "TitleScene.h"
#include "GameScene.h"

using namespace KamataEngine;

TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;

enum class Scene
{ 
	kUnknown,
	kTitle,
	kGame,
};

Scene scene = Scene::kUnknown;

void ChangeScene()
{ 
	switch (scene) {
	case Scene::kTitle:
		gameScene->Initialize();
		break;
	case Scene::kGame:
		//gameScene->UpDate();
		break;
	}
}


void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:

		titleScene->UpDate();

		if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
			scene = Scene::kGame;
		}

		break;

	case Scene::kGame:
		gameScene->UpDate();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	Initialize(L"LE3D_17_タニタ_カイセイ_CG4");

	scene = Scene::kTitle;

	titleScene = new TitleScene();
	titleScene->Initialize();
	gameScene = new GameScene();
	gameScene->Initialize();

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// メインループ
	while (true) {
		// エンジンの更新
		if (Update()) {
			break;
		}

		// シーン切り替え
		ChangeScene();

		// 現在シーン更新
		UpdateScene();

		// 描画開始
		dxCommon->PreDraw();

		DrawScene();

		// 描画終了
		dxCommon->PostDraw();
	}

	titleScene->~TitleScene();
	delete gameScene;

	// エンジンの終了処理
	Finalize();

	return 0;
}