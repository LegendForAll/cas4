#pragma once
#include "GameState.h"

class StateTwo :public CGameState
{
	BossBat *boss;
	CEffect *ef;
	UI *uigame;
	RECT r1;
	RECT r2;
	RECT r3;

	int untouchable, spam;
	DWORD untouchable_start, spam_start;


public:

	StateTwo();
	~StateTwo();

	virtual void InitState(LPDIRECT3DDEVICE9 dv, LPDIRECT3DSURFACE9 bb);
	virtual void LoadState(int AS);
	virtual void UpdateState(DWORD dt);
	virtual void RenderState(LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 bb, LPD3DXSPRITE spriteHandler);

	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StartSpam() { spam = 1; spam_start = GetTickCount(); }
};