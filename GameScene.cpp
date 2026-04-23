#include "GameScene.h"

GameScene::~GameScene() { Model2::StaticFinalize(); }

void GameScene::Initialize() 
{

	textureHandle_ = TextureManager::Load("uvChecker.png");

	Model2::StaticInitialize();

	// モデル生成（まずは簡単に四角）
	model_ = Model2::CreateSquare();

	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();

	// カメラ初期化
	camera_.Initialize();
	camera_.translation_ = {0, 0, -10.0f};

	upData_ = new UpData();
	assert(upData_);
}

void GameScene::UpDate() 
{
	// ★これ追加（超重要）
	upData_->WorldTransformUpData(worldTransform_);

	camera_.UpdateMatrix();
}

void GameScene::Draw() 
{
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	// Model描画開始
	Model2::PreDraw(commandList);

	// ★ここで描画
	model_->Draw(worldTransform_, camera_, textureHandle_);

	// Model描画終了
	Model2::PostDraw();

}
