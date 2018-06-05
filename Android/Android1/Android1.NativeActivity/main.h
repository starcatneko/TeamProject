#pragma once
#include "Player.h"
#include "Camera.h"
#include <memory>

#define GET_CAM_POS cam->GetPos()
#define SET_CAM_POS(X) cam->SetPos(X);


//-----関数定義-----
// システム初期化
int SystemInit(void);

// インスタンス
void Create(void);

// 破棄
void Destroy(void);

// 描画
void Draw(void);

// 処理
void UpData(void);


//-----変数定義-----
// プレイヤー
std::shared_ptr<Player> pl;

Camera *cam;