#include "TextView.h"

CText::CText()
{

}

CText::~CText()
{

}

void CText::initTextView(LPDIRECT3DDEVICE9 d3ddv)
{
	font = NULL;
	AddFontResourceEx(L"textures\\font\\prstart.ttf", FR_PRIVATE, NULL);
	HRESULT result = D3DXCreateFont(
		d3ddv, 20, 0, FW_NORMAL, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"Press Start", &font);

	this->TG = 0;

	SetRect(&rect, 5, 20, 640, 480);

	text = "SCORE_000000 TIME 0000 STAGE 00\n";
	text += "PLAYER                =62\n";
	text += "ENEMY                P=3\n";
}

void CText::updateTextView()
{
	this->timeString = std::to_string((int)this->TG);
	while (timeString.length() < 4)
	{
		timeString = "0" + timeString;
		this->TG = this->TG + 0.01f;
	}

	text = "SCORE_00000 TIME " + timeString + " STAGE 00\n ";
	text += "PLAYER                =62\n";
	text += "ENEMY                P=3\n";
}

void CText::renderTextView()
{
	if (font)
		font->DrawTextA(NULL, text.c_str(), -1, &rect, DT_LEFT, D3DCOLOR_XRGB(255, 255, 255));
}
