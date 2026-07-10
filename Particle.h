#pragma once

#include <KamataEngine.h>
#include "UpData.h"

using namespace KamataEngine;

//パーティクル
class Particle {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, Vector3 position, Vector3 velocity);

	/// <summary>
	/// 更新
	/// </summary>
	void UpDate();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(const Camera& camera);


private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	UpData* upData_ = nullptr;

	//色変更オブジェクト
	ObjectColor objectColor_;

	//色の数値
	Vector4 color_;

	//移動量
	Vector3 velocity_;
};
