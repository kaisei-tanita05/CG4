#include "Particle.h"
#include <cassert>
#include "Math.h"
#include <algorithm>

//using namespace MathUtility;

using namespace KamataEngine;

void Particle::Initialize(Model* model, Vector3 position, Vector3 velocity) {
	//NULLポインタチェック
	assert(model);
	model_ = model;

	 worldTransform_.Initialize();

	upData_ = new UpData();
	assert(upData_);

	//色の設定
	objectColor_.Initialize();
	color_ = {1, 1, 0, 1};

	worldTransform_.translation_ = position;

	velocity_ = velocity;
}

void Particle::UpDate() {
	//終了なら何もしない
	if (isFinished_) {
		return;
	}

	//カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	//存続時間の上限に達したら
	if (counter_ >= kDuration_) {
		counter_ = kDuration_;
		//終了扱いにする
		isFinished_ = true;
	}
	// 行列を定数バッファに転送
	if (upData_) {
		upData_->WorldTransformUpData(worldTransform_);
	}

	//移動
	worldTransform_.translation_ += velocity_;

	worldTransform_.scale_ = {0.2f, 0.2f, 0.2f};

	worldTransform_.TransferMatrix();

	//色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);

	color_.w = std::clamp(1.0f - (counter_ / kDuration_), 0.0f, 1.0f);
}

void Particle::Draw(const Camera& camera) {
	// モデルを描画
	if (model_) {
		model_->Draw(worldTransform_, camera, &objectColor_);
	}
}
