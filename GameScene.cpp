#include "GameScene.h"

GameScene::~GameScene() 
{ 
	Model2::StaticFinalize(); 
}

void GameScene::Initialize() 
{
	Model2::StaticInitialize();
}

void GameScene::UpDate() {}

void GameScene::Draw() {}
