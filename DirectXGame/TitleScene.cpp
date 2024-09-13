#include "TitleScene.h"
#include"Input.h"
#include<cmath>
#include<numbers>
#include"DirectXCommon.h"


TitleScene::~TitleScene() { delete model_; }

void TitleScene::Initialize() {
	model_ = Model::CreateFromOBJ("player");
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	finished_ = false;
}

void TitleScene::Update() { 

	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTitleMove);
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	model_->Draw(worldTransformPlayer_, viewProjection_);
	Model::PostDraw();
}
