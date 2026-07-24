#include "TitleScene.h"
#include "Math.h"
#include <numbers>
#include <cassert>
#include "Fade.h"

using namespace KamataEngine;

TitleScene::~TitleScene() {
	delete titleModel_;
	titleModel_ = nullptr;

	delete worldTransformTitle_;
	worldTransformTitle_ = nullptr;

	delete worldTransformEnter_;
	worldTransformEnter_ = nullptr;
	
	delete fade_;
	fade_ = nullptr;
}


void TitleScene::Initialize() {

	// カメラ初期化
	camera_.Initialize();

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	const float kScaleTitle = 6.0f;
	const float kScaleEnter = 3.0f;
	// 3Dモデル生成
	titleModel_ = Model::CreateFromOBJ("title", true);

	enterModel_ = Model::CreateFromOBJ("enterKey", true);
	// ワールドトランスフォーム初期化
	worldTransformTitle_ = new WorldTransform();
	worldTransformTitle_->Initialize();
	worldTransformTitle_->translation_ = {0.0f, 20.0f, 0.0f};	

	worldTransformTitle_->scale_ = {kScaleTitle, kScaleTitle, kScaleTitle};

	worldTransformEnter_ = new WorldTransform();
	worldTransformEnter_->Initialize();

	worldTransformEnter_->scale_ = {kScaleEnter, kScaleEnter, kScaleEnter};
	// タイトルの下に配置
	worldTransformEnter_->translation_ = {0.0f, -10.0f, 0.0f};

	upData_ = new UpData();
	assert(upData_);

	objectColorEnter_.Initialize();
}

void TitleScene::UpDate() {
	// カメラ更新
	//camera_.UpdateMatrix();

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:

		if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}


	const float kGoalY = 0.0f;

	if (worldTransformTitle_->translation_.y > kGoalY) {
		worldTransformTitle_->translation_.y -= titleSpeed_;

		// 行き過ぎ防止
		if (worldTransformTitle_->translation_.y < kGoalY) {
			worldTransformTitle_->translation_.y = kGoalY;
		}
	}

	enterTimer_ += 0.05f;

	// α値を0～1で変化させる
	enterAlpha_ = (std::sin(enterTimer_) + 1.0f) * 0.5f;

	objectColorEnter_.SetColor({1.0f, 1.0f, 1.0f, enterAlpha_});

	// ワールドトランスフォーム更新
	upData_->WorldTransformUpData(*worldTransformTitle_);
	upData_->WorldTransformUpData(*worldTransformEnter_);
	// ワールドトランスフォーム転送
	//worldTransformTitle_->TransferMatrix();
	//worldTransformEnter_->TransferMatrix();

}

void TitleScene::Draw() {

	Model::PreDraw();
	// 3Dモデル描画
	titleModel_->Draw(*worldTransformTitle_, camera_);
	enterModel_->Draw(*worldTransformEnter_, camera_, &objectColorEnter_);
	Model::PostDraw();
	fade_->Draw();
}