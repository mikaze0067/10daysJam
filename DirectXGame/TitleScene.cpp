#include "TitleScene.h"
#define _USE_MATH_DEFINES
#include<math.h>
#include"Input.h"
#include<cmath>
#include<numbers>
#include"DirectXCommon.h"
#include "TextureManager.h"

TitleScene::~TitleScene() { 
	delete model_; 
	delete sprite_;
}

void TitleScene::Initialize() {
	model_ = Model::CreateFromOBJ("player");
	textureHandle_ = TextureManager::Load("TitleBG.png");
	sprite_ = Sprite::Create(textureHandle_, { 0,0, });

	// ビュープロジェクションの初期化
	titleWorldTransform_.Initialize();
	titleWorldTransform_.translation_.z -= 42.f;

	viewProjection_.Initialize();

	timer_ = 0.0f;
}

void TitleScene::Update() { 

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
	// タイマーを加算
	timer_ += 1.0f / 60.0f;
	// 上下アニメーション
	float param = std::sin((2 * std::numbers::pi_v<float>) * timer_ / TitleTime);
	float radian = TitlePositionStart + TitlePositionEnd * (param + 1.0f) / 2.0f;
	titleWorldTransform_.translation_.y = sinf((radian * float(M_PI)) / 2);

	titleWorldTransform_.UpdateMatrix();
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);
	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	// スプライト描画後処理
	Sprite::PostDraw();


	Model::PreDraw(commandList);
	model_->Draw(titleWorldTransform_, viewProjection_);
	Model::PostDraw();
}
