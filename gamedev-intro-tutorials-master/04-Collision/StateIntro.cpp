#include "StateIntro.h"

StateIntro::StateIntro()
{

}

StateIntro::~StateIntro()
{

}

void StateIntro::InitState(LPDIRECT3DDEVICE9 dv, LPDIRECT3DSURFACE9 bb)
{

}

void StateIntro::LoadState(int AS)
{
	CGameState::LoadState(AS);
	AniUI *bat = new AniUI();
	bat->LoadResource();
	bat->SetState(MI_BAT_INTRO);
	bat->SetPosition(260, 80);
	objecttsStatic.push_back(bat);

	AniUI *sky = new AniUI();
	sky->LoadResource();
	sky->SetState(MI_SKY_INTRO);
	sky->SetPosition(443, 85);
	objecttsStatic.push_back(sky);
}

void StateIntro::UpdateState(DWORD dt)
{
	CGameState::UpdateState(dt);
}

void StateIntro::RenderState(LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 bb, LPD3DXSPRITE spriteHandler)
{
	tilemap->UpdateScrollPosition();
	CGameState::RenderState(d3ddv, bb, spriteHandler);
}
