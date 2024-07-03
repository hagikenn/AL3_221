#include "Player.h"
#include<cassert>

void Player::Initialize(Model* model, ViewProjection* viewProjection,const Vector3&position) {
	assert(model); 
	worldTransform_.Initialize();
	model_ =model;
	//textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position;
}

void Player::Update() {
	worldTransform_.UpdateMatrix();
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_/*, textureHandle_*/);
	
}
