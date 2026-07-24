#include "GameScene.h"
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <math/MathUtility.h>
#include <numbers>
#include <random>

using namespace MathUtility;
using namespace KamataEngine;

std::random_device seedGenerator;
std::mt19937 randomEngine(seedGenerator());
std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

GameScene::~GameScene() {

	delete stage_;
	stage_ = nullptr;

	delete model_;
	model_ = nullptr;

	delete model2_;
	model2_ = nullptr;

	// 3Dモデルデータの解放
	delete modelParticle_;

	Model2::StaticFinalize();
	Effect::StaticFinalize();

	for (auto& e : effects_) {

		delete e.worldTransform;
		e.worldTransform = nullptr;
	}

	effects_.clear();

	for (Particle* particle : particles_) {
		delete particle;
	}
	particles_.clear();
}

void GameScene::Initialize() {

	srand((unsigned int)time(nullptr));

	textureHandle_ = TextureManager::Load("uvChecker.png");

	Model2::StaticInitialize();

	Effect::StaticInitialize();

	// モデル生成（まずは簡単に四角）
	model_ = Model2::CreateSquare();

	model2_ = Effect::CreateSquare();

	for (int g = 0; g < 5; g++) {

		// 爆発の中心
		Vector3 pos = {

		    (float)(rand() % 200 - 100) / 10.0f, (float)(rand() % 200 - 100) / 10.0f, 0.0f};

		// 1セット15本
		for (int i = 0; i < 15; i++) {

			CreateEffect(pos);
		}
	}

	// 3Dモデルデータの生成
	modelParticle_ = Model::CreateSphere(4, 4);

	// パーティクルの生成
	particle_ = new Particle();

	// 乱数の初期化
	srand((unsigned int)time(nullptr));

	// カメラ初期化
	camera_.Initialize();
	camera_.translation_ = {0, 0, -20.0f};

	upData_ = new UpData();
	assert(upData_);

	stage_ = new stage();
	stage_->Initialize();

	player_ = new Player();

	modelPlayer_ = Model::CreateFromOBJ("player", true);

	player_->Initialize(modelPlayer_, &camera_, {0.0f, 0.0f, 0.0f});

	// バーゲージ
	textureHandleGreenBar_ = TextureManager::Load("./Resources/bar/greenBar.png");
	textureHandleRedBar_ = TextureManager::Load("./Resources/bar/redBar.png");

	Vector2 barPos = {100.0f, 20.0f};

	redBar_ = Sprite::Create(textureHandleRedBar_, barPos);
	greenBar_ = Sprite::Create(textureHandleGreenBar_, barPos);

	// 初期サイズ
	redBar_->SetSize({320.0f, 40.0f});
	greenBar_->SetSize({320.0f, 40.0f});

	redBar_->SetColor({1.0f, 1.0f, 1.0f, 0.8f});
	greenBar_->SetColor({1.0f, 1.0f, 1.0f, 0.8f});

	// 数字画像
	textureHandleNumber_ = TextureManager::Load("./Resources/number/number.png");

	// 開始位置
	Vector2 startPos = {900.0f, 20.0f};

	// 5桁生成
	for (int i = 0; i < 5; i++) {

		spriteNumber_[i] = Sprite::Create(textureHandleNumber_, {startPos.x + numberSize_.x * i, startPos.y});

		// 1文字のサイズ
		spriteNumber_[i]->SetSize({numberSize_});

		// 最初は0
		spriteNumber_[i]->SetTextureRect({0.0f, 0.0f}, numberSize_);
	}
}

void GameScene::UpDate() {

	score_++;

	if (score_ > 99999) {
		score_ = 0;
	}

	int digit = 10000;
	int number = score_;

	for (int i = 0; i < 5; i++) {

		// 今の桁
		int nowNumber = number / digit;

		spriteNumber_[i]->SetTextureRect({numberSize_.x * nowNumber, 0.0f}, {numberSize_.x, numberSize_.y});

		// 次の桁へ
		number %= digit;
		digit /= 10;
	}

	camera_.UpdateMatrix();

	player_->Update();

	greenBarWidth_ -= greenBarSpeed_;

	if (greenBarWidth_ <= 0.0f) {
		greenBarWidth_ = maxGreenBarWidth_;
	}

	greenBar_->SetSize({greenBarWidth_, 40.0f});

	//particle_->UpDate();

	for (size_t i = 0; i < effects_.size();) {

		auto& e = effects_[i];

		e.currentTime++;

		e.worldTransform->rotation_.z += 0.1f;

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

		// 爆発を5セット生成
		for (int g = 0; g < 5; g++) {

			// 爆発中心
			Vector3 position = {

			    (float)(rand() % 200 - 100) / 10.0f, (float)(rand() % 200 - 100) / 10.0f, 0.0f};

			// 1セット15本
			for (int i = 0; i < 15; i++) {

				CreateEffect(position);
			}
		}
	}

	if (rand() % 20 == 0) {
		Vector3 position = {distribution(randomEngine) * 20.0f, distribution(randomEngine) * 20.0f, 0.0f};

		ParticleBorn(position);
	}

	// パーティクルの更新
	for (Particle* particle : particles_) {
		particle->UpDate();
	}

	particles_.remove_if([](Particle* particle) {
		if (particle->IsFinished()) {
			delete particle;
			return true;
		}
		return false;
	});

	stage_->Update();
}

void GameScene::CreateEffect(Vector3 position) {

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
	effect.worldTransform->translation_ = position;

	// 速度
	float speed = (rand() % 100) / 500.0f + 0.02f;

	effect.velocity = {cosf(angle) * speed, sinf(angle) * speed, 0.0f};

	// 拡大速度
	effect.scaleSpeed = 0.01f;

	// 寿命
	effect.lifeTime = 40;

	effect.currentTime = 0;

	effect.alpha = 1.0f;

	effect.colorData.Initialize();

	effect.colorData.SetColor({(float)(rand() % 256) / 255.0f, (float)(rand() % 256) / 255.0f, (float)(rand() % 256) / 255.0f, 1.0f});
}

// パーティクル発生
void GameScene::ParticleBorn(Vector3 position) {

	for (int i = 0; i < 50; i++) {

		Particle* particle = new Particle();

		Vector3 velocity = {distribution(randomEngine), distribution(randomEngine), 0.0f};

		Normalize(velocity);

		velocity *= distribution(randomEngine);
		velocity *= 0.1f;

		particle->Initialize(modelParticle_, position, velocity);

		particles_.push_back(particle);
	}
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Sprite::PreDraw();
	stage_->Draw();

	redBar_->Draw();
	greenBar_->Draw();

	for (int i = 0; i < 5; i++) {
		spriteNumber_[i]->Draw();
	}
	Sprite::PreDraw();

	dxCommon->ClearDepthBuffer();

	Model::PreDraw();

	for (Particle* particle : particles_) {
		particle->Draw(camera_);
	}

	player_->Draw();

	Model::PostDraw();
}
