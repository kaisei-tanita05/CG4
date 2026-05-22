#include "GameScene.h"
#include <numbers>

GameScene::~GameScene() {
	delete model_;
	model_ = nullptr;

	delete model2_;
	model2_ = nullptr;

	Model2::StaticFinalize();
	Effect::StaticFinalize();
	
}

void GameScene::Initialize() 
{

	textureHandle_ = TextureManager::Load("uvChecker.png");

	Model2::StaticInitialize();

	Effect::StaticInitialize();


	// モデル生成（まずは簡単に四角）
	model_ = Model2::CreateSquare();

	model2_ = Effect::CreateSquare();

	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();


	//worldTransform_.rotation_.x = std::numbers::pi_v<float> / 2.0f;
	//worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	worldTransform_.rotation_.z = std::numbers::pi_v<float> / 4.0f;

	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	// カメラ初期化
	camera_.Initialize();
	camera_.translation_ = {0, 0, -10.0f};

	upData_ = new UpData();
	assert(upData_);
}

void GameScene::UpDate() 
{
	// ★これ追加（超重要）
	camera_.UpdateMatrix();

	upData_->WorldTransformUpData(worldTransform_);

	worldTransform_.TransferMatrix();
}

void GameScene::Draw() 
{
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	//// Model描画開始
	//Model2::PreDraw(commandList);

	//// ★ここで描画
	//model_->Draw(worldTransform_, camera_, textureHandle_);

	//// Model描画終了
	//Model2::PostDraw();

	Effect::PreDraw(commandList);

	model2_->Draw(worldTransform_, camera_);

	Effect::PostDraw();
}
