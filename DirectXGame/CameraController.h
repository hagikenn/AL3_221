#include"MathUtilityForText.h"
#include"Player.h"
#include"MathUtilityForText.h"
#include "ViewProjection.h"
#include"MathUtilityForText.h"

class Player;

#pragma once
class CameraController {
public:
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	void Initialize();
	void Update();
	void SetTarget(Player* target) { target_ = target; }
	void Reset();

	//追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = {0, 0, -15.0f};

	const ViewProjection& GetViewProjection() const { return viewProjection_; }
	void SetMovableArea(Rect area) { movableArea_ = area; }

	const Vector3& targetVelocity = target_->GetVelocity();

private:
	Player* target_ = nullptr;
	ViewProjection viewProjection_;
	Rect movableArea_ = {0, 100, 0, 100};
	//カメラの目標座標
	Vector3 cameraTarget_;
	static inline const float kInterpolationRate = 0.1f;
	//速度掛け率
	static inline const float kVelocityBias = 0.1f;

};
