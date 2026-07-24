#include "Player.h"
#include <cassert>

using namespace KamataEngine;

Player::~Player() {}

void Player::Initialize(Model* model, Camera* camera, Vector3 position) {

	assert(model);
	assert(camera);

	playerModel_ = model;
	camera_ = camera;

	worldTransform_.Initialize();

	worldTransform_.translation_ = position;
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.rotation_ = {0.0f, 90.0f, 0.0f};

	upData_ = new UpData();
	assert(upData_);
	
}

void Player::Update() {

	// Dキー：右
	if (Input::GetInstance()->PushKey(DIK_D)) {
		worldTransform_.translation_.x += moveSpeed_;
	}

	// Aキー：左
	if (Input::GetInstance()->PushKey(DIK_A)) {
		worldTransform_.translation_.x -= moveSpeed_;
	}

	// Wキー：上
	if (Input::GetInstance()->PushKey(DIK_W)) {
		worldTransform_.translation_.y += moveSpeed_;
	}

	// Sキー：下
	if (Input::GetInstance()->PushKey(DIK_S)) {
		worldTransform_.translation_.y -= moveSpeed_;
	}
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};

	upData_->WorldTransformUpData(worldTransform_);

	worldTransform_.TransferMatrix();
}

void Player::Draw() { 
	playerModel_->Draw(worldTransform_, *camera_);
}