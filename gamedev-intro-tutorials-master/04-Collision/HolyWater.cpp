#include "HolyWater.h"

void HolyWater::LoadResource()
{
	//load resource
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_HOLY_WARTER, L"Resource\\sprites\\Sub_weapons\\HOLY_WATER_ACTION.png", D3DCOLOR_XRGB(255, 0, 255));

	LPDIRECT3DTEXTURE9 texHOLY = textures->Get(ID_TEX_HOLY_WARTER);

	CSprites * sprites = CSprites::GetInstance();
	sprites->Add(300, 0, 0, 32, 26, texHOLY);

	sprites->Add(301, 32, 0, 64, 26, texHOLY);
	sprites->Add(302, 64, 0, 96, 26, texHOLY);



	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	ani = new CAnimation(100);		
	ani->Add(300, 90);
	animations->Add(WP_HOLY_WATER, ani);

	ani = new CAnimation(100);
	ani->Add(301, 150);
	ani->Add(302, 150);
	animations->Add(WP_HOLY_WATER_DEAD, ani);

	this->AddAnimation(WP_HOLY_WATER);
	this->AddAnimation(WP_HOLY_WATER_DEAD);

	this->SetState(WP_HOLY_WATER);
	this->isDead = true;
}

void HolyWater::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!isDead)
	{
		l = x;
		t = y;
		r = l + HOLY_BBOX_WIDTH;
		b = t + HOLY_BBOX_HEIGHT;
	}
}

void HolyWater::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT>* coObjectEnemy)
{
	CGameObject::Update(dt);
	vy += 0.001 * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
		x += dx;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.1f;
		y += min_ty * dy + ny * 0.1f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CandleSmall *>(e->obj))
			{
				CandleSmall *_canM = dynamic_cast<CandleSmall *>(e->obj);
				if (e->nx != 0)
				{
					_canM->SetState(CANDLESMALL_STATE_DIE);
					_canM->SetDead(true);
					_canM->SetInvisible(true);
					_canM->is_Item();
				}
			}

			if (dynamic_cast<CLimit *> (e->obj))
			{
				//this->isDead = true;
				vy = 0;
				vx = 0;
				this->state = WP_HOLY_WATER_DEAD;
			}
		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void HolyWater::Render(float xViewport, float yViewport)
{
	this->X_view = x - xViewport;
	this->Y_view = y - yViewport;
	int alpha = 255;

	this->SetState(this->state);

	//animation axe left or right
	this->isLeft = nx > 0 ? false : true;

	//choose animation axe stand or sit
	//this->ani = isStand ? 0 : 1;
	if (!isDead)
	{
		animations[ani]->Render(x - xViewport, y - yViewport, alpha, isLeft);
		//Render boundingBox at end frame of currentAnimation
		RenderBoundingBox();
	}
}

void HolyWater::SetCurrentPosSimon(float _xSimon, float _ySimon, int _nx, bool _isStand)
{
	this->x = _xSimon + 20.0f;
	this->y = _ySimon + 15.0f;
	this->nx = _nx;
	this->isStand = _isStand;
	this->BoxFly();
	this->SetDead(false);
}

void HolyWater::SetState(int state)
{
	CGameObject::SetState(state);

	switch (this->state)
	{
		case WP_HOLY_WATER:
		{
			this->state = WP_HOLY_WATER;
			ani = WP_HOLY_WATER;
			break;
		}
		case WP_HOLY_WATER_DEAD:
		{
			ani = WP_HOLY_WATER_DEAD;
			if (animations[ani]->CheckDoAllFrame())
			{
				this->isDead = true;
				animations[ani]->SetDoAllFrame(false);
				this->state = WP_HOLY_WATER;
			}
			break;
		}
	}
}
