#include "UI.h"

UI::UI()
{

}

UI::~UI()
{

}

void UI::LoadResource()
{
	//load resource
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_BACK_BOARD, L"Resource\\sprites\\UI\\blackboard.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_HP, L"Resource\\sprites\\UI\\heal.png", D3DCOLOR_XRGB(255, 0, 255));


	LPDIRECT3DTEXTURE9 texBack = textures->Get(ID_BACK_BOARD);
	LPDIRECT3DTEXTURE9 texHP = textures->Get(ID_HP);

	CSprites * sprites = CSprites::GetInstance();
	sprites->Add(300, 0, 0, 520, 80, texBack);		//background
	sprites->Add(301, 0, 0, 11, 18, texHP);			//HP simon
	sprites->Add(302, 22, 0, 33, 18, texHP);		//HP boss
	sprites->Add(303, 11, 0, 22, 18, texHP);		//HP no


	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	ani = new CAnimation(100);		//background
	ani->Add(300);
	animations->Add(BLACK_BOARD, ani);

	ani = new CAnimation(100);		//HP simon
	ani->Add(301);
	animations->Add(HP_SIMON, ani);

	ani = new CAnimation(100);		//HP boss
	ani->Add(302);
	animations->Add(HP_BOSS, ani);

	ani = new CAnimation(100);		//HP no
	ani->Add(303);
	animations->Add(HP_NO, ani);

	this->AddAnimation(BLACK_BOARD);
	this->AddAnimation(HP_SIMON);
	this->AddAnimation(HP_BOSS);
	this->AddAnimation(HP_NO);

	//init item
	this->item = new Items();
	this->item->LoadResource();
	this->item->SetPosition(330, 50);
}

void UI::initFont(LPDIRECT3DDEVICE9 dv)
{
	Font = NULL;
	AddFontResourceEx(GAME_FORMAT_FONT, FR_PRIVATE, NULL);
	HRESULT result = D3DXCreateFont(
		dv, 20, 0, FW_NORMAL, 1, false,
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY, FF_DONTCARE, L"Press Start", &Font);

	//rect render text
	SetRect(&rect, 0, 20, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void UI::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	//no code
}

void UI::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT>* coObjectEnemy)
{

}

void UI::Render(float xViewport, float yViewport)
{
	animations[BLACK_BOARD]->Render(this->x, this->y);

	this->setPositionHP(200, 50);
	for (int i = 0; i < 10; i++)
	{
		animations[HP_NO]->Render(posHP.x, 50);
		animations[HP_NO]->Render(posHP.x, 68);
		posHP.x += 11;
	}

	this->setPositionHP(200, 50);
	for (int i = 0; i < this->HPsimon; i++)
	{
		animations[HP_SIMON]->Render(posHP.x, 50);
		posHP.x += 11;
	}

	this->setPositionHP(200, 50);
	for (int i = 0; i < this->HPboss; i++)
	{
		animations[HP_BOSS]->Render(posHP.x, 68);
		posHP.x += 11;
	}

	this->item->Render(0, 0);
}

void UI::setPositionUI(float _x, float _y)
{
	this->x = _x;
	this->y = _y;
}

void UI::setPositionHP(float _x, float _y)
{
	posHP.x = _x;
	posHP.y = _y;
	posHP.z = 0;
}

void UI::resetUI()
{
	this->HPsimon = 10;
	this->HPboss = 6;
}

void UI::setHPSimon(int _HPsimon)
{
	this->HPsimon = _HPsimon;
}

void UI::setHPBoss(int _HPBoss)
{
	this->HPboss = _HPBoss;
}

void UI::UpdateItem(CMario * _simon)
{
	item->SetState(_simon->GetStateItemSimon());
	this->HPsimon = _simon->GetHP();
}
