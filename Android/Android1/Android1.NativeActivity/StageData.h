﻿#pragma once
#include <vector>

// 敵
std::vector<int>e_data = {
	//1
	0,0,0,0,//←ダミー
	0,0,0,0,
	2,3,0,1,
	0,0,0,0,
	0,1,2,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	//2
	0,0,0,0,
	0,0,0,0,
	0,1,0,1,
	0,0,0,0,
	1,0,1,0,
	0,0,0,0,
	0,0,0,0,
	//3
	0,0,0,0,
	0,0,0,0,
	1,0,0,1,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	//4
	0,0,0,0,
	1,0,0,1,
	0,0,0,0,
	0,1,1,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0
};


// アイテム
/* 0：無し　1：リンゴ　2：青リンゴ　3：木 */
std::vector<int>i_data = {
//1
  0,0,0,0,0,0,0,0,//←ダミー
  0,0,0,0,0,0,0,0,
  0,0,0,0,1,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,1,
  0,0,2,0,0,1,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,2,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,1,0,0,0,0,
  0,0,2,0,0,0,0,0,
  0,0,0,0,1,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,1,
//2
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,2,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,1,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,1,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,1,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,2,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
//3
  0,0,0,0,0,0,0,0,
  0,0,1,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,1,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,1,
  0,0,0,0,0,1,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,1,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,2,0,2,
  0,0,0,0,0,0,0,0,
  0,0,0,1,0,0,0,0,
  0,0,0,0,0,0,0,0,
//4
  0,0,0,0,0,0,0,0,
  0,0,0,0,1,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,2,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,1,1,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,1,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,1,0,
  0,0,0,0,0,0,0,0,
  0,0,1,0,1,2,0,2,
  0,0,0,0,0,0,0,0
};