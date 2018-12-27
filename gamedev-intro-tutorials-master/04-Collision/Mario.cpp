#include <algorithm>
#include "debug.h"
#include "tinyxml.h"

#include "Mario.h"
#include "Game.h"
#include "Goomba.h"
#include "Candle.h"
#include "Items.h"
#include "Brick.h"
#include "BoxStair.h"
#include "BoxChangeMap.h"
#include "LimitTransparent.h"
#include "BossBat.h"
#include "Zombie.h"
#include "Merman.h"


bool douKey = false;

void CMario::LoadAnimation()
{

}

void CMario::LoadResource()
{
	//load resource
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_SIMON, L"Resource\\sprites\\Simon\\Simon_ver_editted.png", D3DCOLOR_XRGB(255, 0, 255));

	//sprite
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);

	//animations
	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	//init XML
	TiXmlDocument doc("ResourceXML\\simon.xml");
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
			sprites->Add(idFrame, left, top, right, bottom, texSimon);
			ani->Add(idFrame);
		}

		//load 1 frame finish
		animations->Add(id, ani);
		this->AddAnimation(id);
	}

	//int item simon
	this->item = new Items();
	this->item->SetState(I_HEART_BIG);
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects, vector<LPGAMEOBJECT> *coObjectStatic, vector<LPGAMEOBJECT> *coObjectEnemy)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += SM_GRAVITY * dt;

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > SM_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
		this->isTrans = false;
	}

	//-----------------------------------------------------
	vector<LPGAMEOBJECT> coEventStair;
	for (UINT i = 0; i < coObjectStatic->size(); i++)
	{
		if (this->AABB_BOX(this, coObjectStatic->at(i)))
		{
			coEventStair.push_back(coObjectStatic->at(i));
		}
	}

	if (coEventStair.size() == 0)
	{
		this->isCollisonBox = false;
		this->isCollisonTop = false;
		this->isAutoBox = -2;
	}

	else
	{
		for (int i = 0; i < coEventStair.size(); i++)
		{
			if (dynamic_cast<BOXStair *>(coEventStair[i]))
			{
				BOXStair *box = dynamic_cast<BOXStair *>(coEventStair[i]);

				if (box->state == BOX_STAIR_BOTTOM)
				{
					this->isCollisonBox = true;
					this->curNX_BOX = (box->GetcurrentBox());
				}

				else if (box->state == BOX_STAIR_TOP)
				{
					this->isCollisonTop = true;
					this->curNX_BOX = (box->GetcurrentBox());
				}

				else if (box->state == BOX_LEFT)
				{
					this->isAutoBox = -1;
				}

				else if (box->state == BOX_RIGHT)
				{
					this->isAutoBox = 1;
				}

				//else if (box->state == BOX_CHANGE_MAPNEXT)
				//{
				//	this->isAutoBox = 2;
				//}
			}

			if (dynamic_cast<BoxMap *>(coEventStair[i]))
			{
				BoxMap *box = dynamic_cast<BoxMap *>(coEventStair[i]);
				this->isAutoBox = 2;
				this->SetMap(box->GetAniBoxMap());
				this->ASdoor = box->GetASDoor();
				//phan biet box map ma no va cham
			}
		}
	}
	//-----------------------------------------------------

	vector<LPCOLLISIONEVENT> coEvents,coEnemy;
	vector<LPCOLLISIONEVENT> coEventsResult,coEnemyResult;

	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents); //tao ra danh sach cac doi tuong co kha nang va cham
	coEnemy.clear();
	CalcPotentialCollisions(coObjectEnemy, coEnemy);

	// No collision occured, proceed normally
	if (coEvents.size() == 0 && coEnemy.size() == 0) //danh sach kha nang va cham rong  x, y mario cap nhat binh thuong
	{
		x += dx;
		y += dy;
	}
	if (coEnemy.size() != 0 && !this->isTrans)
	{
		//co van cham vo enemy
		if (this->HP > 0 && this->untouchable == 0)
		{
			this->HP -= 1;
		}
		
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEnemy, coEnemyResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.4f;		//khi va cham van di chuyen theo truc Ox binh thuong
		y -= min_ty * dy + ny * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame

		if (nx != 0) vx = 0; //khi mario co va cham theo huong RIGHT-> nx = 1/LEFT->ny = -1
		if (ny != 0) vy = 0; //khi mario co va cham theo huong DOWN-> ny = -1/UP->ny = 1 cho vy = 0 de khong bi roi tu do

		// Collision logic with Object after collision vs mario
		for (UINT i = 0; i < coEnemyResult.size(); i++)//UNIT
		{
			LPCOLLISIONEVENT e = coEnemyResult[i];
			if (dynamic_cast<Zombie *>(e->obj))
			{	
				if (this->untouchable == 0 && e->nx != 0)
				{
					this->nx = e->nx;
					this->StartUntouchable();
					this->state = SM_HURTED;
				}			
			}

			if (dynamic_cast<BossBat *>(e->obj))
			{
				if (this->untouchable == 0)
				{
					this->nx = e->nx;
					this->StartUntouchable();
					this->state = SM_HURTED;
				}
			}

			if (dynamic_cast<Merman *>(e->obj))
			{
				if (this->untouchable == 0 && e->nx != 0)
				{
					this->nx = e->nx;
					this->StartUntouchable();
					this->state = SM_HURTED;
				}
			}
		}
	}
	if (coEvents.size() != 0)	//co doi tuong co kha nang va cham voi mario
	{
		float min_tx, min_ty, nx = 0, ny;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		// block 
		x += min_tx * dx + nx * 0.1f;		//khi va cham van di chuyen theo truc Ox binh thuong
		y += min_ty * dy + ny * 0.1f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame

		if (nx != 0) vx = 0; //khi mario co va cham theo huong RIGHT-> nx = 1/LEFT->ny = -1
		if (ny != 0) vy = 0; //khi mario co va cham theo huong DOWN-> ny = -1/UP->ny = 1 cho vy = 0 de khong bi roi tu do

		// Collision logic with Object after collision vs mario
		for (UINT i = 0; i < coEventsResult.size(); i++)//UNIT
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<Items *>(e->obj))
			{
				Sound::GetInstance()->Play(COLLECT_WEAPON);
				Items *_item = dynamic_cast<Items *>(e->obj);
				if (e->nx != 0)
				{
					_item->SetDead(true);
					_item->SetInvisible(true);
					this->SetItem(_item);
					//-------------------------------------------
					switch (this->item->state)
					{
						case I_AXE:
						{
							this->typeWeapon = USE_AXE;
							break;
						}
						case I_KNIFE:
						{
							this->typeWeapon = USE_KNIFE;
							break;
						}
						case I_HOLY_WATER:
						{
							this->typeWeapon = USE_HOLY;
							break;
						}
						
						case I_GOLD_POTION:
						{
							this->StartUntouchable();
							this->isTrans = true;
							break;
						}

					}
					//-------------------------------------------
					if (_item->state == I_MORNING_STAR)
					{
						this->state = SM_EAT_ITEM;
						ani = SM_EAT_ITEM;
					}

				}
			}

			else if (dynamic_cast<CLimit *>(e->obj))
			{
				if (e->ny == -1)
				{
					isGround = true;
				}
				else if (e->nx != 0)
				{
					vx = 0;
				}
			}
		}
	}

	whip->Update(dt, coObjects, coObjectStatic, coObjectEnemy);
	kni->Update(dt, coObjects, coObjectStatic, coObjectEnemy);
	axe->Update(dt, coObjects, coObjectStatic, coObjectEnemy);
	holy->Update(dt, coObjects, coObjectStatic, coObjectEnemy);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	for (UINT i = 0; i < coEnemy.size(); i++) delete coEnemy[i];

	//HP simon
	if (this->HP == 0)
	{
		this->state = SM_DEAD;
	}
}

void CMario::Render(float xViewport, float yViewport)
{
	this->X_view = x - xViewport;
	this->Y_view = y - yViewport;

	//Set left or right weapons
	whip->SetNX(this->nx);

	//opacity of sprite
	int alpha = 255;
	if (untouchable) alpha = 128;

	bool isStand;
	bool isWP = false;
	this->isLeft = this->nx == 1 ? true : false;

	//Using State Machine to render
	//if (this->typeWeapon == USE_WHIP)
	{
		switch (state)
		{
		case SM_STAND_ATTACK:
		{
			isStand = true;
			//if (this->typeWeapon == USE_WHIP)
			{
				isWP = true;
			}
			break;
		}

		case SM_SIT_ATTACK:
		{
			isStand = false;
			//if (this->typeWeapon == USE_WHIP)
			{
				isWP = true;
			}
			break;
		}

		default:
		{
			break;
		}
		}
	}

	if (!isvisible)
	{
		//Render animation of simon
		animations[ani]->Render(x - xViewport, y - yViewport, alpha, this->isLeft);
		//Render boundingBox of simon
		//RenderBoundingBox();
	}


	//Render animation of weapon
	if (isWP)
	{
		//sm use whip -> whip is life
		whip->SetPositionLR(x, y, isStand, this->isLeft);
		whip->SetDead(false);
		whip->Render(xViewport, yViewport);
	}

	switch (this->typeWeapon)
	{
		case USE_NOT:
		{
			break;
		}
		case USE_AXE:
		{
			axe->Render(xViewport, yViewport);
			break;
		}
		case USE_KNIFE:
		{
			kni->Render(xViewport, yViewport);
			break;
		}
		case USE_HOLY:
		{
			holy->Render(xViewport, yViewport);
			break;
		}
	}
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SM_STAND_STATIC:
	{
		if (IsKeyDown(DIK_RIGHT))								//sm move right && !douKey
		{
			this->state = SM_MOVE_LEFT;
			nx = 1;
			vx = SM_WALKING_SPEED;
			ani = SM_MOVE_LEFT;
			break;
		}
		else if (IsKeyDown(DIK_LEFT) || isIntro)							//sm move left && !douKey
		{
			this->state = SM_MOVE_LEFT;
			nx = -1;
			vx = -SM_WALKING_SPEED;
			ani = SM_MOVE_LEFT;
			break;
		}
		else if (IsKeyDown(DIK_DOWN) && isCollisonTop == false)							//sm sit
		{
			this->state = SM_SIT;
			vx = 0;
			ani = SM_SIT;
			break;
		}
		else if ((IsKeyPress(DIK_Z)) && isAttack == false)		//sm attack RIGHT/LEFT
		{
			Sound::GetInstance()->Play(USING_WHIP);
			this->isAttack = true;
			this->state = SM_STAND_ATTACK;
			vx = 0;
			ani = SM_STAND_ATTACK;
			break;
		}
		//-------------------------------------------------------------------
		else if ((IsKeyPress(DIK_LCONTROL)))
		{
			this->isAttack = true;
			this->state = SM_THROW_WEAPON;
			vx = 0;
			ani = SM_THROW_WEAPON;
			this->useWeapon(this->typeWeapon);
			break;
		}
		//-------------------------------------------------------------------
		else if (IsKeyPress(DIK_X) && isGround == true)			//sm jump
		{
			isGround = false;
			vy = -SM_JUMP_SPEED_Y;
			vx = 0;					//tam thoi = 0
			this->state = SM_JUMP;
			ani = SM_JUMP;
			break;
		}

		else if (IsKeyDown(DIK_UP) && isCollisonBox && isGround)					//sm stair left - top
		{
			this->isGround = false;
			this->isCollisonBox = false;

			this->isOnStair = true;
			this->state = SM_STAND_UP_STAIR;
			ani = SM_STAND_UP_STAIR;
			break;
		}

		else if (IsKeyDown(DIK_DOWN) && isCollisonTop && isGround)					//sm down stair
		{
			this->isGround = false;
			this->isCollisonTop = false;

			this->isOnStair = true;
			this->state = SM_STAND_DOWN_STAIR;
			ani = SM_STAND_DOWN_STAIR;
			break;
		}

	default:
	{
		vx = 0;
		break;
	}
	break;
	}

	//-------------------------------
	case SM_HURTED:
	{
		ani = SM_HURTED;
		this->vy = -0.1;
		this->vx = this->nx * 0.1;
		if (GetTickCount() - untouchable_start > 250 && animations[SM_HURTED]->CheckDoAllFrame())
		{
			animations[SM_HURTED]->SetDoAllFrame(false);
			this->state = SM_STAND_STATIC;
			ani = SM_STAND_STATIC;
		}
		break;
	}
	//-------------------------------
	case SM_THROW_WEAPON:
	{
		if (animations[ani]->CheckDoAllFrame())//finish attack
		{
			animations[ani]->SetDoAllFrame(false);
			this->state = SM_STAND_STATIC;
			ani = SM_STAND_STATIC;
			isAttack = false;
			break;
		}
		break;
		break;
	}
	//-------------------------------
	case SM_DEAD:
	{
		this->state = SM_DEAD;
		ani = SM_DEAD;
		break;
	}
	//-------------------------------
	case SM_MOVE_UP_STAIR:
	{
		this->vy = 0;
		this->x = this->x + curNX_BOX * SM_WALKING_STAIR_SPEED;
		//this->x += SM_WALKING_STAIR_SPEED;			//position x phu thuong vao nx box_stair
		this->y -= SM_WALKING_STAIR_SPEED;				//position y khong phu thuoc nx box_stair

		if (IsKeyRelease(DIK_UP))
		{
			this->state = SM_STAND_UP_STAIR;
			this->ani = SM_STAND_UP_STAIR;
			break;
		}

		//khi dang up stair
		else if (isCollisonTop)
		{
			this->state = SM_STAND_STATIC;
			this->ani = SM_STAND_STATIC;
			break;
		}

		break;
	}

	case SM_MOVE_DOWN_STAIR:
	{
		this->vy = 0;
		this->x = this->x - curNX_BOX * SM_WALKING_STAIR_SPEED;
		//this->x -= SM_WALKING_STAIR_SPEED;			//position x phu thuoc vao nx box_stair
		this->y += SM_WALKING_STAIR_SPEED;			//position y khong phu thuong nx box_stair

		if (IsKeyRelease(DIK_DOWN))
		{
			this->state = SM_STAND_DOWN_STAIR;
			this->ani = SM_STAND_DOWN_STAIR;
			break;
		}

		else if (isCollisonBox == true)
		{
			this->state = SM_SIT;
			ani = SM_SIT;
			break;
		}
		break;
	}

	case SM_STAND_DOWN_STAIR:
	{
		if (IsKeyDown(DIK_UP))
		{
			//this->SetNX(1);
			this->SetNX(1 * curNX_BOX);
			this->state = SM_MOVE_UP_STAIR;
			ani = SM_MOVE_UP_STAIR;
			break;
		}
		else if (IsKeyDown(DIK_DOWN))
		{
			//this->SetNX(-1);
			this->SetNX((-1)*curNX_BOX);
			this->state = SM_MOVE_DOWN_STAIR;
			ani = SM_MOVE_DOWN_STAIR;
			break;
		}

		vy = 0;
		x += 0;
		y += 0;
		break;
	}

	case SM_STAND_UP_STAIR:
	{
		if (IsKeyDown(DIK_UP))
		{
			//this->SetNX(1);
			this->SetNX(1 * curNX_BOX);
			this->state = SM_MOVE_UP_STAIR;
			ani = SM_MOVE_UP_STAIR;
			break;
		}
		if (IsKeyDown(DIK_DOWN))
		{
			//this->SetNX(-1);
			this->SetNX((-1)*curNX_BOX);
			this->state = SM_MOVE_DOWN_STAIR;
			ani = SM_MOVE_DOWN_STAIR;
			break;
		}
		vy = 0;
		x += 0;
		y += 0;
		break;
	}

	case SM_JUMP:
	{
		if (isGround)											//sm collision Ground
		{
			this->state = SM_STAND_STATIC;
			ani = SM_STAND_STATIC;
			isGround = true;
			break;
		}

		else if ((IsKeyPress(DIK_Z)) && isAttack == false)		//sm attack RIGHT/LEFT
		{
			Sound::GetInstance()->Play(USING_WHIP);
			this->isAttack = true;
			this->state = SM_STAND_ATTACK;
			ani = SM_STAND_ATTACK;
			break;
		}

		//-------------------------------------------------------------------
		else if ((IsKeyPress(DIK_LCONTROL))) //sm use weapon in stand attack
		{
			this->isAttack = true;
			this->state = SM_THROW_WEAPON;
			vx = 0;
			ani = SM_THROW_WEAPON;
			this->useWeapon(this->typeWeapon);
			break;
		}
		//-------------------------------------------------------------------
		break;
	}

	case SM_MOVE_LEFT:
	{
		if (IsKeyRelease(DIK_RIGHT))		//sm stop move right
		{
			this->state = SM_STAND_STATIC;
			vx = 0;
			ani = SM_STAND_STATIC;
			douKey = false;
			break;
		}
		else if (IsKeyRelease(DIK_LEFT))	//sm stop move left
		{
			this->state = SM_STAND_STATIC;
			vx = 0;
			ani = SM_STAND_STATIC;
			douKey = false;
			break;
		}

		else if (IsKeyPress(DIK_Z) && isAttack == false)				//move - attack - stop move
		{
			Sound::GetInstance()->Play(USING_WHIP);
			vx = 0;
			this->state = SM_STAND_ATTACK;
			ani = SM_STAND_ATTACK;
			isAttack = true;
			break;
		}

		//-------------------------------------------------------------------
		else if ((IsKeyPress(DIK_LCONTROL)))
		{
			this->isAttack = true;
			this->state = SM_THROW_WEAPON;
			vx = 0;
			ani = SM_THROW_WEAPON;
			this->useWeapon(this->typeWeapon);
			break;
		}
		//-------------------------------------------------------------------
		//----------------------------
		else if (isAutoBox == -1)
		{
			this->state = SM_AUTO_DOOR;
			nx = 1;
			vx = SM_WALKING_SPEED;
			ani = SM_MOVE_LEFT;
			break;
		}
		//----------------------------
		else if (IsKeyPress(DIK_X) && isGround == true)
		{
			isGround = false;
			vy = -SM_JUMP_SPEED_Y;
			this->state = SM_JUMP;
			ani = SM_JUMP;
			break;
		}
		break;

	}
	//------------------------
	case SM_AUTO_DOOR:
	{
		if (isAutoBox == 1)
		{
			this->isAutoBox = 0;
			this->state = SM_STAND_STATIC;
			ani = SM_STAND_STATIC;
			break;
		}
		break;
	}
	//------------------------
	case SM_SIT:
	{
		if (IsKeyRelease(DIK_DOWN))			//sm stop sit and stand
		{
			this->state = SM_STAND_STATIC;
			ani = SM_STAND_STATIC;
			break;
		}
		else if (IsKeyDown(DIK_RIGHT))		//sm sit right
		{
			nx = 1;
			break;
		}
		else if (IsKeyDown(DIK_LEFT))		//sm sit left
		{
			nx = -1;
			break;
		}
		else if (IsKeyPress(DIK_Z) && isAttack == false)			//sm sit attack LEFT/RIGHT
		{
			Sound::GetInstance()->Play(USING_WHIP);
			this->state = SM_SIT_ATTACK;
			ani = SM_SIT_ATTACK;
			isAttack = true;
			break;
		}

		//-------------------------------------------------------------------
		else if ((IsKeyPress(DIK_LCONTROL))) //sm use weapon in stand attack
		{
			this->isAttack = true;
			this->state = SM_THROW_WEAPON;
			vx = 0;
			ani = SM_THROW_WEAPON;
			this->useWeapon(this->typeWeapon);
			break;
		}
		//-------------------------------------------------------------------
		break;
	}

	case SM_STAND_ATTACK:
	{

		if (animations[ani]->CheckDoAllFrame())//finish attack
		{
			animations[ani]->SetDoAllFrame(false);
			this->state = SM_STAND_STATIC;
			ani = SM_STAND_STATIC;
			isAttack = false;
			break;
		}
		break;
	}

	case SM_SIT_ATTACK:
	{
		if (animations[ani]->CheckDoAllFrame())		//sm finish sit attack
		{
			animations[ani]->SetDoAllFrame(false);
			this->state = SM_SIT;
			ani = SM_SIT;
			isAttack = false;
			break;
		}
		break;
	}

	case SM_EAT_ITEM:
	{
		vx = 0;
		if (animations[SM_EAT_ITEM]->CheckDoAllFrame())
		{
			animations[SM_EAT_ITEM]->SetDoAllFrame(false);
			this->state = SM_STAND_STATIC;
			isEatItem = false;
			ani = SM_STAND_STATIC;
		}
		break;
	}
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x + 20;
	top = y;
	right = x + SM_BBOX_WIDTH;
	bottom = y + SM_BBOX_HEIGHT;
}

void CMario::KeyEventHandler()
{
	this->SetState(this->state);
}

void CMario::useWeapon(int _typeWeapon)
{
	switch (_typeWeapon)
	{
		case USE_AXE:
		{
			axe->SetCurrentPosSimon(this->x, this->y, this->nx);
			break;
		}
		case USE_KNIFE:
		{
			kni->SetCurrentPosSimon(this->x, this->y, this->nx);
			break;
		}
		case USE_HOLY:
		{
			holy->SetCurrentPosSimon(this->x, this->y, this->nx);
			break;
		}
	}
}




