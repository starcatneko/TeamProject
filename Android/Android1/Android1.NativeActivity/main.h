#pragma once
#include "Player.h"
#include <memory>


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