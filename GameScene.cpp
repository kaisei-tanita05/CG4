#include "GameScene.h"
#include <numbers>
#include <cstdlib>
#include <ctime>

GameScene::~GameScene() {
	delete model_;
	model_ = nullptr;

	delete model2_;
	model2_ = nullptr;

	Model2::StaticFinalize();
	Effect::StaticFinalize();

	for (auto& wt : effects_) {

		delete wt;
	}

	effects_.clear();
	
}

void GameScene::Initialize() 
{

	srand((unsigned int)time(nullptr));

	textureHandle_ = TextureManager::Load("uvChecker.png");

	Model2::StaticInitialize();

	Effect::StaticInitialize();


	// モデル生成（まずは簡単に四角）
	model_ = Model2::CreateSquare();

	model2_ = Effect::CreateSquare();

	


	//worldTransform_.rotation_.x = std::numbers::pi_v<float> / 2.0f;
	//worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	//worldTransform_.rotation_.z = std::numbers::pi_v<float> / 4.0f;

	//worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};


	for (int i = 0; i < 15; i++) {
		// ワールドトランスフォーム初期化
		WorldTransform* wt = new WorldTransform();
		wt->Initialize();

		float angle = (float)(rand() % 360) * (std::numbers::pi_v<float> / 180.0f);

		wt->rotation_.z = angle;

		float length = (rand() % 100) / 20.0f + 2.0f;

		wt->scale_ = {0.05f, length, 1.0f};

		effects_.push_back(wt);
	}

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

	for (auto& wt : effects_) {

		upData_->WorldTransformUpData(*wt);

		wt->TransferMatrix();
	}

	//worldTransform_.TransferMatrix();
}

void GameScene::Draw() 
{
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	Effect::PreDraw(commandList);

	for (auto& wt : effects_) {

		model2_->Draw(*wt, camera_);
	}


	Effect::PostDraw();
}
