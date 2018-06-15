#include "Debug.h"
#include "Typedef.h"
#include "DxLib.h"
\


Debug::Debug()
{
}


Debug::~Debug()
{
}
//s :�ϐ��̖��O���e�L�X�g, i :�ϐ�(int), offset :�\���ʒu(����:�� 4�A�c �C�Ӂj,color :�F
void Debug::DebugText(std::string s,int i,int offset_x,int offset_y,int color)
{
	DrawBox(offset_x*(WINDOW_X / 4), offset_y*(GetFontSize() * 2 + 2),
		offset_x*(WINDOW_X / 4) + GetStringLength(s.c_str()) + GetFontSize() * 6,
		offset_y*(GetFontSize() * 2 + 2) + offset_y + offset_y * (GetFontSize()/2), 0xBB0000, true);
	DrawFormatString(offset_x*(WINDOW_X / 4), offset_y*(GetFontSize() * 2 + 2), color, "%d", i);
	DrawFormatString(offset_x*(WINDOW_X / 4)+ GetStringLength(s.c_str()), offset_y*(GetFontSize() * 2 + 2), color, s.c_str(), i);
}