#pragma once
#include "KamataEngine.h"
#include "Model2.h"
#include "UpData.h"
#include "Effect.h"
#include <vector>
#include "Particle.h"

using namespace KamataEngine;

class GameScene {
public:

	~GameScene();

	void Initialize();

	void UpDate();

	void Draw();

	void CreateEffect(Vector3 position);

private:
	uint32_t textureHandle_ = 0;
	Model2* model_ = nullptr;
	Effect* model2_ = nullptr;
	Camera camera_;
	UpData* upData_ = nullptr;
	
	//パーティクル3Dモデルデータ
	Model* modelParticle_ = nullptr;

	// パーティクル
	Particle* particle_ = nullptr;


	struct EffectData {

		WorldTransform* worldTransform;

		Vector3 velocity;

		float alpha = 1.0f;

		float scaleSpeed = 0.08f;

		int lifeTime = 30;

		int currentTime = 0;

		Vector4 color = {1, 1, 1, 1}; // ←追加

		ObjectColor colorData;
	};

	std::vector<EffectData> effects_;
};