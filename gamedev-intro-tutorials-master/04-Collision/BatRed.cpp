#include "BatRed.h"
#include "tinyxml.h"

void BatRed::LoadResource()
{
	//load resource zombie
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_BAT_RED, L"Resource\\sprites\\Enemies\\BAT.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_DEAD, L"Resource\\sprites\\Effect\\DEAD.png", D3DCOLOR_XRGB(255, 0, 255));

	//sprite
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texBat = textures->Get(ID_TEX_BAT_RED);
	LPDIRECT3DTEXTURE9 texdead = textures->Get(ID_TEX_DEAD);

	//animations 
	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	//init XML
	TiXmlDocument doc("ResourceXML\\batred.xml");
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
	}
	int id;
	int idFrame;
	int left, top, right, bottom;
	TiXmlElement* anim = nullptr;

	//load XML candle
	TiXmlElement* mani = doc.RootElement();
	anim = mani->FirstChildElement("animation");
	for (anim; anim != NULL; anim = anim->NextSiblingElement())
	{
		ani = new CAnimation(100);
		anim->QueryIntAttribute("id", &id);

		for (TiXmlElement* a = anim->FirstChildElement(); a != NULL; a = a->NextSiblingElement())
		{
			a->QueryIntAttribute("id", &idFrame);
			a->QueryIntAttribute("left", &left);
			a->QueryIntAttribute("top", &top);
			a->QueryIntAttribute("right", &right);
			a->QueryIntAttribute("bottom", &bottom);

			//add frame to sprite
			sprites->Add(idFrame, left, top, right, bottom, texBat);
			ani->Add(idFrame);
		}

		//load 1 frame finish
		animations->Add(id, ani);
		this->AddAnimation(id);
	}

	//dead object
	sprites->Add(6000, 0, 0, 42, 44, texdead);
	sprites->Add(6001, 42, 0, 84, 44, texdead);
	sprites->Add(6002, 84, 0, 128, 44, texdead);

	ani = new CAnimation(100);		//Dead
	ani->Add(6000);
	ani->Add(6001);
	ani->Add(6002);
	animations->Add(BAT_RED_DIE, ani);

	this->AddAnimation(BAT_RED_DIE);
}

void BatRed::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT>* coObjectEnemy)
{
	CGameObject::Update(dt);
	
	//Sinusoidal wave
	x += dx;
	y = AMx * cos(((2 * CONST_PI * CURRENT_TIME) / double(Tms)) + ((2 * CONST_PI * x) / double(LAMDA_Ox))) + 350;//+MARGIN_TOP
}

void BatRed::Render(float xViewport, float yViewport)
{
	//Update limit
	this->UpdateLimit(xViewport, yViewport);

	//Update action
	this->Action(xViewport, yViewport);

	//Update state of panther
	this->SetState(state);

	this->X_view = x - xViewport;
	this->Y_view = y - yViewport;

	int alpha = 255;
	this->isLeft = this->nx == 1 ? true : false;

	if (!(isDead && isvisible))
	{
		animations[ani]->Render(x - xViewport, y - yViewport, alpha, this->isLeft);
		RenderBoundingBox();
	}
	else if (isDead)
	{
		if (this->state == BAT_RED_DIE)
		{
			animations[BAT_RED_DIE]->Render(x - xViewport, y - yViewport, alpha, this->isLeft);
			RenderBoundingBox();
		}
	}
}

void BatRed::SetState(int state)
{
	CGameObject::SetState(state);

	switch (this->state)
	{
		case BAT_RED_STATIC:
		{
			this->vx = 0;
			if (isAction)
			{
				this->state = BAT_RED_FLY;
				ani = BAT_RED_FLY;
				vx = -BAT_FLY_SPEED;
				break;
			}
			break;
		}
		case BAT_RED_FLY:
		{
			ani = BAT_RED_FLY;
			break;
		}

	}
}

void BatRed::UpdateLimit(float xViewport, float yViewport)
{
	if (this->x - xViewport < 0.0f)
	{
		this->SetInvisible(true);
		this->SetDead(true);
	}
}

void BatRed::Action(float xViewport, float yViewport)
{
	if ((this->x - (xViewport + SCREEN_WIDTH / 2)) <= DIS_BAT_ATTACK)
	{
		this->isAction = true;
	}
}

void BatRed::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if ((!isvisible && isDead))
	{
		left = x;
		top = y;
		right = left + BAT_RED_BBOX_WIDTH;
		bottom = top + BAT_RED_BBOX_HEIGHT;
	}
}
