#include "Zombie.h"
#include "tinyxml.h"

Zombie::Zombie()
{

}

Zombie::~Zombie()
{

}

void Zombie::LoadResource()
{

	//Load Item
	this->item = new Items();
	this->item->LoadResource();
	this->item->SetState(I_HEART_BIG);
	this->item->SetInvisible(true);

	//load resource zombie
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_ZOMBIE, L"Resource\\sprites\\Enemies\\ZOMBIE.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_DEAD, L"Resource\\sprites\\Effect\\DEAD.png", D3DCOLOR_XRGB(255, 0, 255));

	//sprites
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texZom = textures->Get(ID_TEX_ZOMBIE);
	LPDIRECT3DTEXTURE9 texdead = textures->Get(ID_TEX_DEAD);

	//animations
	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	//init XML
	TiXmlDocument doc("ResourceXML\\zombie.xml");
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
			sprites->Add(idFrame, left, top, right, bottom, texZom);
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
	animations->Add(ZOMBIE_DIE, ani);

	this->AddAnimation(ZOMBIE_DIE);

	//this->SetState(ZOMBIE_MOVE_LEFT);
}

void Zombie::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT> *coObjectEnemy)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);
	vy += ZOMBIE_GRAVITY* dt;

	//Update limit zombie
	//this->UpdateLimit();
	this->UpLimit();

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	CalcPotentialCollisions(coObjects, coEvents); //tao ra danh sach cac doi tuong co kha nang va cham

	// No collision occured, proceed normally
	if (coEvents.size() == 0) //danh sach kha nang va cham rong  x, y mario cap nhat binh thuong
	{
		if (!isDead)
		{
			x += dx;
			y += dy;
		}
	}
	else	//co doi tuong co kha nang va cham voi mario
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += dx;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0; //khi mario co va cham theo huong RIGHT-> nx = 1/LEFT->ny = -1
		if (ny != 0) vy = 0; //khi mario co va cham theo huong DOWN-> ny = -1/UP->ny = 1 cho vy = 0 de khong bi roi tu do

		// Collision logic with Object after collision vs mario
		for (UINT i = 0; i < coEventsResult.size(); i++)//UNIT
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

		}
	}
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void Zombie::Render(float xViewport, float yViewport)
{
	CGameObject::SetState(this->state);

	//this->UpdateLimit(xViewport, yViewport);

	this->X_view = x - xViewport;
	this->Y_view = y - yViewport;


	//opacity of sprite
	int alpha = 255;
	this->isLeft = this->nx == 1 ? true : false;

	switch (this->state)
	{
		case ZOMBIE_MOVE_LEFT:
		{
			if (!(isDead && isvisible))
			{
				animations[ZOMBIE_MOVE_LEFT]->Render(x - xViewport, y - yViewport, alpha, isLeft);
				break;
			}
			break;
		}

		case ZOMBIE_DIE:
		{
			if (isDead)
			{
				animations[ZOMBIE_DIE]->Render(x - xViewport, y - yViewport);

				if (animations[ZOMBIE_DIE]->CheckDoAllFrame())
				{
					this->SetState(ZOMBIE_MOVE_LEFT);
					animations[ZOMBIE_DIE]->SetDoAllFrame(false);
				}
				break;
			}
			break;
		}
	}

	RenderBoundingBox();
}

void Zombie::StateZom(int _state)
{
	switch (_state)
	{

		break;
	}
}

void Zombie::is_Item()
{
	item->SetPosition(this->x, this->y);
	item->SetInvisible(false);
}

void Zombie::UpdateLimit(float xViewport, float yViewport)
{
	if (this->x - xViewport < 0.0f)
	{
		this->SetInvisible(true);
		this->SetDead(true);
	}
}

void Zombie::UpdateLimit()
{
	if (this->x > this->limx)
	{
		this->SetNX(-1);
		this->SetSpeed(-ZOMBIE_WALKING_SPEED, 0.0f);
	}
	else if (this->x < this->limx - this->Slimx)
	{
		this->SetNX(1);
		this->SetSpeed(ZOMBIE_WALKING_SPEED, 0.0f);
	}
}

void Zombie::UpLimit()
{
	
	if (this->x >= this->rlim.right)
	{
		this->SetNX(-1);
		this->SetSpeed(-ZOMBIE_WALKING_SPEED, 0.0f);
		
	}
	else if (this->x <= this->rlim.left)
	{
		this->SetNX(1);
		this->SetSpeed(ZOMBIE_WALKING_SPEED, 0.0f);
		
	}
}


void Zombie::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (!isDead  && !isvisible)
	{
		left = x;
		top = y;
		right = x + ZOMBIE_BBOX_WIDTH;
		bottom = y + ZOMBIE_BBOX_HEIGHT;
	}
}
