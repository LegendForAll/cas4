#include "Items.h"
#include "tinyxml.h"

Items::Items()
{

}

Items::~Items()
{

}

void Items::LoadResource()
{
	//load resource
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_ITEM_HEART_BIG, L"Resource\\sprites\\Items\\item.png", D3DCOLOR_XRGB(255, 0, 255));

	//sprite
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texItem = textures->Get(ID_ITEM_HEART_BIG);

	//animations
	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	//----------------------------------------------------------
	//init XML
	TiXmlDocument doc("ResourceXML\\item.xml");
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
			sprites->Add(idFrame, left, top, right, bottom, texItem);
			ani->Add(idFrame);
		}

		//load 1 frame finish
		animations->Add(id, ani);
		this->AddAnimation(id);
	}
	//----------------------------------------------------------

	this->SetState(I_HEART_BIG);
}

void Items::Render(float xViewport, float yViewport)
{
	this->X_view = x - xViewport;
	this->Y_view = y - yViewport;



	switch (this->state)
	{
		case I_HEART_BIG:
		{
			currentAni = I_HEART_BIG;
			break;
		}
		case I_MORNING_STAR:
		{
			currentAni = I_MORNING_STAR;
			break;
		}
		case I_MONEY_BLUE:
		{
			currentAni = I_MONEY_BLUE;
			break;
		}
		case I_ROSARY:
		{
			currentAni = I_ROSARY;
			break;
		}
		case I_ROSACT:
		{
			currentAni = I_ROSACT;
			break;
		}
		case I_STOPWATCH:
		{
			currentAni = I_STOPWATCH;
			break;
		}
		case I_DOUBLE_SHOT:
		{
			currentAni = I_DOUBLE_SHOT;
			break;
		}
		case I_BOSS_BAT:
		{
			currentAni = I_BOSS_BAT;
			break;
		}
		case I_KNIFE:
		{
			currentAni = I_KNIFE;
			break;
		}
		case I_AXE:
		{
			currentAni = I_AXE;
			break;
		}
		case I_HOLY_WATER:
		{
			currentAni = I_HOLY_WATER;
			break;
		}

		case I_GOLD_POTION:
		{
			currentAni = I_GOLD_POTION;
			break;
		}
	}

	if (!isvisible)
	{
		animations[currentAni]->Render(x - xViewport, y - yViewport);
		//animations[typeItem]->Render(x - xViewport, y - yViewport);
		RenderBoundingBox();
	}
}

void Items::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!(isvisible && isDead))
	{
		l = x;
		t = y;
		r = x + ITEM_BBOX_WIDTH;
		b = y+ ITEM_BBOX_HEIGHT;
	}
}

void Items::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects, vector<LPGAMEOBJECT> *coObjectStatic, vector<LPGAMEOBJECT> *coObjectEnemy)
{
	// Calculate dx, dy 
	if (!isvisible)
	{
		CGameObject::Update(dt);
		vy += ITEM_GRAVITY * dt;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();


		CalcPotentialCollisions(coObjects, coEvents); //tao ra danh sach cac doi tuong co kha nang va cham


		// No collision occured, proceed normally
		if (coEvents.size() == 0) //danh sach kha nang va cham rong  x, y mario cap nhat binh thuong
		{
			y += dy;
			
			if (this->equation_Fly == EQUATION_SIN)
			{
				x = AMx * cos(((2 * CONST_PI * CURRENT_TIME) / double(Tms)) + ((2 * CONST_PI * y) / double(LAMDA_Ox))) + this->current_Oy;
			}
			else
			{
				x += dx;
			}	

		}
		else	//co doi tuong co kha nang va cham voi mario
		{
			float min_tx, min_ty, nx = 0, ny;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

			// block 
			x += min_tx * dx + nx * 0.1f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.1f;

			if (nx != 0) vx = 0; //khi mario co va cham theo huong RIGHT-> nx = 1/LEFT->ny = -1
			if (ny != 0) vy = 0; //khi mario co va cham theo huong DOWN-> ny = -1/UP->ny = 1 cho vy = 0 de khong bi roi tu do

			// Collision logic with Object after collision vs mario
			for (UINT i = 0; i < coEventsResult.size(); i++)//UNIT
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				//xu ly sau va cham voi Candle => collision whip vs candle
				if (dynamic_cast<CBrick *>(e->obj))
				{
					//Candle *_candle = dynamic_cast<Candle *>(e->obj);
					//if (e->nx != 0)
					//{
					//	//Xu ly sau va cham
					//	_candle->SetDead(true);
					//	_candle->SetInvisible(true);
					//	_candle->SetState(CANDLE_STATE_DIE);
					//}
				}
			}
		}
		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}


}

void Items::Set_positionSimon(float _xSimon, float _ySimon)
{
	this->SetPosition(_xSimon, _ySimon);
}

void Items::Random_pre()
{
	this->typeItem = rand() % 7;
	this->currentAni = this->typeItem;
}
