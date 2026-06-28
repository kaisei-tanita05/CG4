#include "Particle.h"
#include <cassert>
#include "Math.h"

//using namespace MathUtility;



void Particle::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);
	model_ = model;

	 worldTransform_.Initialize();

	upData_ = new UpData();
	assert(upData_);

	//色の設定
	objectColor_.Initialize();
	color_ = {1, 1, 0, 1};
}

void Particle::UpDate() {
	// 行列を定数バッファに転送
	if (upData_) {
		upData_->WorldTransformUpData(worldTransform_);
	}

	//移動
	worldTransform_.translation_ += {0.0f, 0.1f, 0.0f};

	worldTransform_.TransferMatrix();

	//色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
}

void Particle::Draw(const Camera& camera) {
	// モデルを描画
	if (model_) {
		model_->Draw(worldTransform_, camera, &objectColor_);
	}
}
