#pragma once
#include <Input.h>
#include "Model.h"
#include"ViewProjection.h"
#include"WorldTransform.h"


class TitleScene {
public:

	~TitleScene();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:

	ViewProjection viewProjection_;
	WorldTransform titleWorldTransform_;
	WorldTransform worldTransformPlayer_;

	Model* model_ = nullptr;

	float counter_ = 0.0f;
	// 終了フラグ
	bool finished_ = false;

	// 最初の角度
	static inline const float TitlePositionStart = 0.1f;

	// 最後の角度
	static inline const float TitlePositionEnd = -0.1f;

	// アニメーションの周期
	static inline const float TitleTime = 2.0f;

	// 経過時間
	float timer_ = 0.0f;
};