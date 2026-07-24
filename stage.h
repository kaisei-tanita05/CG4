#pragma once
#include <KamataEngine.h>


class stage {
public:
	~stage();

	void Initialize();
	void Update();
	void Draw();

private:
	// 背景
	KamataEngine::Sprite* background_[2] = {nullptr, nullptr};

	// テクスチャ番号
	uint32_t textureHandle_ = 0;

	// スクロール速度
	float scrollSpeed_ = 2.0f;

	// 背景の横幅
	const float backgroundWidth_ = 1280.0f;
};
