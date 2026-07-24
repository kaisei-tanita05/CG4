#pragma once
#include "Effect.h"
#include "KamataEngine.h"
#include "Model2.h"
#include "Particle.h"
#include "UpData.h"
#include <vector>
#include "stage.h"




class GameScene {
public:
	~GameScene();

	void Initialize();

	void UpDate();

	void Draw();

	void CreateEffect(KamataEngine::Vector3 position);

	/// <summary>
	/// パーティクル発生
	/// </summary>
	/// <param name="position">発生位置</param>
	void ParticleBorn(KamataEngine::Vector3 position);

private:
	uint32_t textureHandle_ = 0;
	Model2* model_ = nullptr;
	Effect* model2_ = nullptr;
	KamataEngine::Camera camera_;
	UpData* upData_ = nullptr;

	// パーティクル3Dモデルデータ
	KamataEngine::Model* modelParticle_ = nullptr;

	// パーティクル
	Particle* particle_ = nullptr;

	struct EffectData {

		KamataEngine::WorldTransform* worldTransform = nullptr;

		KamataEngine::Vector3 velocity = {0.0f, 0.0f, 0.0f};

		float alpha = 1.0f;

		float scaleSpeed = 0.08f;

		int lifeTime = 30;

		int currentTime = 0;

		KamataEngine::Vector4 color = {1, 1, 1, 1}; // ←追加

		KamataEngine::ObjectColor colorData;
	};

	std::vector<EffectData> effects_;

	std::list<Particle*> particles_;

	stage* stage_ = nullptr;
};