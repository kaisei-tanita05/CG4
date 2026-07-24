#include <KamataEngine.h>
#include "UpData.h" 


class TitleScene {

public:
	/// <summary>
	/// 解放処理
	/// </summary>
	~TitleScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void UpDate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	UpData* upData_ = nullptr;
	// ビュープロジェクション
	KamataEngine::Camera camera_;
	KamataEngine::WorldTransform* worldTransformTitle_ = nullptr;
	KamataEngine::WorldTransform* worldTransformEnter_ = nullptr;

	KamataEngine::Model* titleModel_ = nullptr;
	KamataEngine::Model* enterModel_ = nullptr;
	float titleSpeed_ = 0.2f; // 落ちてくる速さ

	float enterAlpha_ = 1.0f;
	float enterTimer_ = 0.0f;

	KamataEngine::ObjectColor objectColorEnter_;
};