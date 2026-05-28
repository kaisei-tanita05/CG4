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


	for (auto& e : effects_) {

		delete e.worldTransform;
		e.worldTransform = nullptr;
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


	//for (int i = 0; i < 15; i++) {

	//	effects_.emplace_back();

	//	EffectData& effect = effects_.back();

	//	effect.worldTransform = new WorldTransform();

	//	effect.worldTransform->Initialize();

	//	float angle = (float)(rand() % 360) * (std::numbers::pi_v<float> / 180.0f);

	//	effect.worldTransform->rotation_.z = angle;

	//	float length = (rand() % 100) / 20.0f + 2.0f;

	//	effect.worldTransform->scale_ = {0.05f, length, 1.0f};

	//	float speed = (rand() % 100) / 500.0f + 0.02f;

	//	effect.velocity = {cosf(angle) * speed, sinf(angle) * speed, 0.0f};

	//	effect.lifeTime = 20 + rand() % 20;
	//}

	for (int i = 0; i < 15; i++) {

		CreateEffect();
	}

	// カメラ初期化
	camera_.Initialize();
	camera_.translation_ = {0, 0, -10.0f};

	upData_ = new UpData();
	assert(upData_);
}

void GameScene::UpDate() 
{
	
	camera_.UpdateMatrix();

	for (size_t i = 0; i < effects_.size();) {

		auto& e = effects_[i];

		e.currentTime++;

		// 移動
		//e.worldTransform->translation_.x += e.velocity.x;
		//e.worldTransform->translation_.y += e.velocity.y;

		// 拡大
		e.worldTransform->scale_.x += e.scaleSpeed;

		// フェードアウト
		e.alpha = 1.0f - (float(e.currentTime) / float(e.lifeTime));

		// α適用
		model2_->SetAlpha(e.alpha);

		// 更新
		upData_->WorldTransformUpData(*e.worldTransform);

		e.worldTransform->TransferMatrix();

		// 寿命終了
		if (e.currentTime >= e.lifeTime) {

			delete e.worldTransform;
			e.worldTransform = nullptr;

			effects_.erase(effects_.begin() + i);

		} else {

			i++;
		}
	}

	// 全部消えたら一気に再生成
	if (effects_.empty()) {

		for (int i = 0; i < 15; i++) {

			CreateEffect();
		}
	}
}

void GameScene::CreateEffect() {

	effects_.emplace_back();

	EffectData& effect = effects_.back();

	effect.worldTransform = new WorldTransform();

	effect.worldTransform->Initialize();

	// ランダム角度
	float angle = (float)(rand() % 360) * (std::numbers::pi_v<float> / 180.0f);

	effect.worldTransform->rotation_.z = angle;

	// 長さ
	float length = (rand() % 200) / 20.0f + 1.0f;

	effect.worldTransform->scale_ = {0.01f, length, 1.0f};

	// 初期位置
	effect.worldTransform->translation_ = {0.0f, 0.0f, 0.0f};

	// 速度
	float speed = (rand() % 100) / 500.0f + 0.02f;

	effect.velocity = {cosf(angle) * speed, sinf(angle) * speed, 0.0f};

	// 拡大速度
	effect.scaleSpeed = 0.01f;

	// 寿命
	effect.lifeTime = 40;

	effect.currentTime = 0;

	effect.alpha = 1.0f;
}

void GameScene::Draw() 
{
	ID3D12GraphicsCommandList* commandList = DirectXCommon::GetInstance()->GetCommandList();

	Effect::PreDraw(commandList);

	for (auto& effect : effects_) {

		model2_->Draw(*effect.worldTransform, camera_);
	}


	Effect::PostDraw();
}
