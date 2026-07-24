#include "stage.h"

using namespace KamataEngine;

stage::~stage() {
	delete background_[0];
	delete background_[1];
}

void stage::Initialize() {

	// 背景画像を読み込む
	textureHandle_ = TextureManager::Load("./Resources/backGround/backGround.png");

	// 2枚作成
	background_[0] = Sprite::Create(textureHandle_, {0.0f, 0.0f});
	background_[1] = Sprite::Create(textureHandle_, {backgroundWidth_, 0.0f});
}

void stage::Update() {

	for (int i = 0; i < 2; i++) {

		Vector2 pos = background_[i]->GetPosition();

		// 左へ移動
		pos.x -= scrollSpeed_;

		// 画面外へ出たら右へ移動
		if (pos.x <= -backgroundWidth_) {
			pos.x += backgroundWidth_ * 2.0f;
		}

		background_[i]->SetPosition(pos);
	}
}

void stage::Draw() {

	background_[0]->Draw();
	background_[1]->Draw();
}