#pragma once
#include "Effect.h"
#include "KamataEngine.h"
#include "Model2.h"
#include "Particle.h"
#include "UpData.h"
#include <vector>

using namespace KamataEngine;

class GameScene {
public:
	~GameScene();

	void Initialize();

	void UpDate();

	void Draw();

	void CreateEffect(Vector3 position);

	/// <summary>
	/// パーティクル発生
	/// </summary>
	/// <param name="position">発生位置</param>
	void ParticleBorn(Vector3 position);

private:
	uint32_t textureHandle_ = 0;
	Model2* model_ = nullptr;
	Effect* model2_ = nullptr;
	Camera camera_;
	UpData* upData_ = nullptr;

	// パーティクル3Dモデルデータ
	Model* modelParticle_ = nullptr;

	// パーティクル
	Particle* particle_ = nullptr;

	struct EffectData {

		WorldTransform* worldTransform = nullptr;

		Vector3 velocity = {0.0f, 0.0f, 0.0f};

		float alpha = 1.0f;

		float scaleSpeed = 0.08f;

		int lifeTime = 30;

		int currentTime = 0;

		Vector4 color = {1, 1, 1, 1}; // ←追加

		ObjectColor colorData;
	};

	std::vector<EffectData> effects_;

	std::list<Particle*> particles_;
};