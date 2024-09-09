#define NOMINMAX
#include "Enemy.h"


void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {// NULLポインタチェック
	assert(model);

	model_ = model;
	//  ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;
	viewProjection_ = viewProjection;
	velocity_ = {-kWalkSpeed, 0, 0};
	walkTimer_ = 0.0f;
}

void Enemy::Update() {
	
	//移動
	worldTransform_.translation_ += velocity_;

	// タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	if (walkTimer_ >= 5.0f) {
		if (lrDirection_ != LRDirection::kRight) {
			lrDirection_ = LRDirection::kRight;
			// 旋回開始時の角度
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			// 旋回タイマー
			turnTimer_ = kTimeTurn;

			velocity_ *= -1;

			walkTimer_ = 0;
		}
		else if (lrDirection_ != LRDirection::kLeft) {
			lrDirection_ = LRDirection::kLeft;
			// 旋回開始時の角度
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			// 旋回タイマー
			turnTimer_ = kTimeTurn;

			velocity_ *= -1;

			walkTimer_ = 0;
		}

	}

	/*

	//回転アニメーション
	float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.x = radian;

	*/


	AnimateTurn();

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() {
	//3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}

Vector3 Enemy::GetworldPosition() {//ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetworldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) { (void)player; }

void Enemy::AnimateTurn() {
	// 旋回制御
	if (turnTimer_ > 0.0f) {
		turnTimer_ = std::max(turnTimer_ - (1.0f / 60.0f), 0.0f);

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = { std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> *3.0f / 2.0f };
		// 状態に応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = EaseInOut(destinationRotationY, turnFirstRotationY_, turnTimer_ / kTimeTurn);
	}
}