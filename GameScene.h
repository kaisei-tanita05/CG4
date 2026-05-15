#pragma once
#include "KamataEngine.h"
#include "Model2.h"
#include "UpData.h"
#include "Effect.h"

using namespace KamataEngine;

class GameScene {
public:

	~GameScene();

	void Initialize();

	void UpDate();

	void Draw();

private:
	uint32_t textureHandle_ = 0;
	Model2* model_ = nullptr;
	Effect* model2_ = nullptr;
	WorldTransform worldTransform_;
	Camera camera_;
	UpData* upData_ = nullptr;
};