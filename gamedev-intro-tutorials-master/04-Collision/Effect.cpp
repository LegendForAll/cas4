#include "Effect.h"

CEffect::CEffect()
{

}

CEffect::~CEffect()
{

}

void CEffect::LoadResource()
{
	//load resource
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_WATER, L"Resource\\sprites\\Effect\\2.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_EFFTHOR, L"Resource\\sprites\\Effect\\thor.png", D3DCOLOR_XRGB(255, 0, 255));

	LPDIRECT3DTEXTURE9 texWater = textures->Get(ID_TEX_WATER);
	LPDIRECT3DTEXTURE9 texthor = textures->Get(ID_TEX_EFFTHOR);

	CSprites * sprites = CSprites::GetInstance();
	sprites->Add(300, 0, 0, 14, 28, texWater);

	//thor
	sprites->Add(301, 0, 0, 640, 480, texthor);
	sprites->Add(302, 640, 0, 1280, 480, texthor);
	sprites->Add(303, 1280, 0, 1920, 480, texthor);

	//no effect
	sprites->Add(304, 0, 0, 0, 0, texthor);


	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	ani = new CAnimation(100);		//Water
	ani->Add(300);
	animations->Add(EFFECT_WATER, ani);

	ani = new CAnimation(100);		//thor
	ani->Add(301,50);
	ani->Add(302,50);
	ani->Add(303,50);
	animations->Add(EFFECT_THOR, ani);

	ani = new CAnimation(100);		//no
	ani->Add(304);
	animations->Add(EFFECT_NO, ani);

	this->AddAnimation(EFFECT_WATER);
	this->AddAnimation(EFFECT_THOR);
	this->AddAnimation(EFFECT_NO);
}

void CEffect::GetBoundingBox(float & l, float & t, float & r, float & b)
{

}

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT>* coObjectEnemy)
{
	CGameObject::Update(dt);
	
	if (this->nx == 0)
	{
		vy += 0.0001 * dt;
	}
	else
	{
		vy += 0.0002 * dt;
	}

	x += dx;
	y += dy;
}

void CEffect::Render(float xViewport, float yViewport)
{
	this->X_view = x - xViewport;
	this->Y_view = y - yViewport;
	int alpha = 80;

	this->SetState(this->state);
	
	if (!isDead)
	{
		switch (this->state)
		{

			case EFFECT_THOR:
			{
				animations[ani]->Render(0 , 100, alpha);
				break;
			}
			default:
			{
				animations[ani]->Render(x - xViewport, y - yViewport);
				break;
			}
		}
	}


	if (!isDead)
	{
		
	}
}

void CEffect::SetCurrentMerman(float _xMerman, float _yMerman, int _nx, bool _isStand)
{
	this->x = _xMerman;
	this->y = _yMerman;
}

void CEffect::SetState(int state)
{
	CGameObject::SetState(this->state);

	switch (state)
	{
		case EFFECT_WATER:
		{
			this->state = EFFECT_WATER;
			ani = EFFECT_WATER;
			vx = nx * 0.03f;
			break;
		}
		case EFFECT_THOR:
		{
			this->state = EFFECT_THOR;
			ani = EFFECT_THOR;
			vy = 0;
			if (animations[EFFECT_THOR]->CheckDoAllFrame())
			{
				
				this->SetDead(true);
				
				this->state = EFFECT_NO;
				ani = EFFECT_NO;
			}
			break;
		}
		case EFFECT_NO:
		{
			animations[EFFECT_THOR]->SetDoAllFrame(false);
			break;
		}
	}
}
