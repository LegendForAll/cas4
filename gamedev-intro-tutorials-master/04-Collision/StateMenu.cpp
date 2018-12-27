#include "StateMenu.h"
#include "tinyxml.h"


StateMenu::StateMenu()
{

}

StateMenu::~StateMenu()
{

}

void StateMenu::InitState(LPDIRECT3DDEVICE9 dv, LPDIRECT3DSURFACE9 bb)
{

}

void StateMenu::LoadState(int AS)
{
	CGameState::LoadState(AS);
	AniUI *anim = new AniUI();
	anim->LoadResource();
	anim->SetState(MI_BAT_MENU);
	anim->SetPosition(450, 215);
	objecttsStatic.push_back(anim);
}

void StateMenu::UpdateState(DWORD dt)
{
	CGameState::UpdateState(dt);
}

void StateMenu::RenderState(LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 bb, LPD3DXSPRITE spriteHandler)
{
	tilemap->UpdateScrollPosition();
	CGameState::RenderState(d3ddv, bb, spriteHandler);
}
