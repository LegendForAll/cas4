#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <string>

class CText
{
	ID3DXFont *font;
	RECT rect;
	std::string text;
	float TG;
	std::string timeString;
	int core;
	int state;
	int hearts;

public:

	CText();
	~CText();

	void initTextView(LPDIRECT3DDEVICE9 d3ddv);
	void updateTextView();
	void renderTextView();

};