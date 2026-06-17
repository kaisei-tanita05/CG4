#include "Particle.h"
#include <cassert>

void Particle::Initialize(Model* model) {
	//NULLポインタチェック
	assert(model);
	model_ = model;

	 worldTransform_.Initialize();

	upData_ = new UpData();
	assert(upData_);
}

void Particle::UpDate() {
	// 行列を定数バッファに転送
	if (upData_) {
		upData_->WorldTransformUpData(worldTransform_);
	}
	worldTransform_.TransferMatrix();
}

void Particle::Draw(const Camera& camera) {
	// モデルを描画
	if (model_) {
		model_->Draw(worldTransform_, camera);
	}
}
