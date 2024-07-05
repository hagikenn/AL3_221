#include "GameScene.h"
#include "Model.h"
#include "TextureManager.h"
#include "WorldTransform.h"
#include <cassert>

GameScene::GameScene() {}

// デストラクタ
GameScene::~GameScene() {
	delete sprite_;
	delete player_;
	delete model_;
	delete modelBlock_;
	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;

	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	// 自キャラの生成
	player_ = new Player();

	// スカイドームの生成
	skydome_ = new Skydome();

	model_ = Model::CreateFromOBJ("player", true);

	// 3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	// ビュープロジェクションの初期化
	viewProjection_.farZ;
	viewProjection_.Initialize();

	// 自キャラの初期化
	// player_->Initialize(model_, &viewProjection_,position);

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	player_->Initialize(model_, &viewProjection_, playerPosition);

	// スカイドームの初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	// 3Dモデルの生成(block)
	modelBlock_ = Model::CreateFromOBJ("block");

	// 3Dモデルの生成(skydome)
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();

	// ブロックの表示される数字
	const uint32_t kNumBlockVirtical = 20;
	const uint32_t kNumBlockHorizontal = 100;

	/*const float kBlockWodth = 2.0f;
	const float kBlockHeight = 2.0f;*/

	worldTransformBlocks_.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	debugCamera_ = new DebugCamera(kNumBlockHorizontal, kNumBlockVirtical);
}

void GameScene::Update() {
	Vector2 position = sprite_->GetPosition();
	position.x += 2.0f;
	position.y += 1.0f;
	sprite_->SetPosition(position);

	// 自キャラの更新
	player_->Update();
	skydome_->Update();

	debugCamera_->Update();

	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			if (!worldTransformBlock)

				continue;

			// 平行移動
			Matrix4x4 result{
			    1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, worldTransformBlock->translation_.x, worldTransformBlock->translation_.y, worldTransformBlock->translation_.z,
			    1.0f};

			worldTransformBlock->matWorld_ = result;

			worldTransformBlock->UpdateMatrix();
		}
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_M)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG

	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {
		viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	// sprite_->Draw();
	// 自キャラの描画
	player_->Draw();
	// スカイドームの描画
	skydome_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlocksLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlocksLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; i++) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}
