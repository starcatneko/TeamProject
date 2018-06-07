#include "GamePlay.h"
#include "Game.h"
#include "Over.h"
#include "Camera.h"
#include "BackGround.h"
#include "Player.h"
#include "Dust.h"
#include "DxLib.h"

// コンストラクタ
GamePlay::GamePlay() : speed(60)
{
	Create();
	box = { 0, 0, WINDOW_X, WINDOW_Y };
	func = &GamePlay::NotStart;
}

// デストラクタ
GamePlay::~GamePlay()
{
}

// インスタンス化
void GamePlay::Create(void)
{
	cam.reset(new Camera());
	back.reset(new BackGround());
	pl.reset(new Player(cam));
	du.reset(new Dust(pl));
}

// ボックス描画
void GamePlay::DrawBoxx(void)
{
	DrawBox(box.pos.x, box.pos.y, (box.pos.x + box.size.x), (box.pos.y + box.size.y), GetColor(0, 0, 0), true);
}

// 描画
void GamePlay::Draw(void)
{
	back->Draw();
	pl->Draw();
	//pl->TestDraw(cam->GetPos());	// ミキオが追加
	du->Draw();
	cam->Scroll(pl->GetPos());		// ミキオが追加
	cam->Draw();					// ミキオが追加
	cam->SetPos(cam->GetPos());		// ミキオが追加

	DrawBoxx();
}

// 処理
void GamePlay::UpData(void)
{
	(this->*func)();
}

// 各クラスの処理前
void GamePlay::NotStart(void)
{
	box.pos.x += speed;
	if (box.pos.x >= WINDOW_X)
	{
		func = &GamePlay::Start;
	}
}

// 各クラスの処理
void GamePlay::Start(void)
{
	pl->Update();
	//pl->TestUpdate();
	du->Update();


#ifndef __ANDROID__
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		Game::Get().ChangeScene(new Over());
	}
#else
#endif
}
