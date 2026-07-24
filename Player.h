#pragma once
#include <KamataEngine.h>
#include "UpData.h"

class Player {
public:

	~Player();
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, KamataEngine::Vector3 position);
	void Update();
	void Draw();

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

private:
	KamataEngine::Model* playerModel_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;
	UpData* upData_ = nullptr;
	// 移動速度
	float moveSpeed_ = 0.3f;
};