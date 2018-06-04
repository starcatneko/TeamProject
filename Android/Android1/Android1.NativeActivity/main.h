#pragma once
#include "Player.h"
#include <memory>


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


std::shared_ptr<Player> pl;