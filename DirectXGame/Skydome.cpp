#include "Skydome.h"
#include<cassert>

void Skydome::Initialize(Model* model,ViewProjection* viewProjection) { 
	assert(model);
	worldTransform_.Initialize();
	model_ = model;
	viewProjection_ = viewProjection;
}

void Skydome::Update() {
	worldTransform_.UpdateMatrix();
}


void Skydome::Draw() {

	model_->Draw(worldTransform_,*viewProjection_);
}
