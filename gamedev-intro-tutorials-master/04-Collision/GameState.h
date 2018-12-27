#pragma once
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "debug.h"
#include "define.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"

#include "Mario.h"
#include "Brick.h"
#include "Goomba.h"
#include "Candle.h"
#include "CandleSmall.h"
#include "Zombie.h"
#include "Panther.h"
#include "BatRed.h"
#include "BossBat.h"
#include "Merman.h"
#include "Items.h"
#include "BoxStair.h"
#include "BoxChangeMap.h"
#include "Effect.h"
#include "Door.h"
#include "LimitTransparent.h"

#include "FireBall.h"
#include "UI.h"
#include "AniUI.h"

class CGameState
{
public:

	CMario *simon;

	TileMap *tilemap = new TileMap();
	bool marioInView = false;
	bool isInitSimon = false;

	bool stop = false;
	int untouchable = 0;
	DWORD untouchable_start;

	DWORD dt;
	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> objecttsStatic;
	vector<LPGAMEOBJECT> objectsEnemy;
	vector<D3DXVECTOR2> pointdoor;

public:

	CGameState();
	~CGameState();

	virtual void InitState(LPDIRECT3DDEVICE9 dv, LPDIRECT3DSURFACE9 bb);
	virtual void LoadState(int AS);
	virtual void UpdateState(DWORD dt);
	virtual void RenderState(LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 bb, LPD3DXSPRITE spriteHandler);

	bool simonChangeMap();
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }

};