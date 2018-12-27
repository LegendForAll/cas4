#include "Door.h"
#include "tinyxml.h"

Door::Door()
{

}

Door::~Door()
{

}

void Door::LoadResource()
{
	//load resource zombie
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_DOOR, L"Resource\\sprites\\Ground\\Door.png", D3DCOLOR_XRGB(255, 255, 255));

	//sprite
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texdoor = textures->Get(ID_TEX_DOOR);

	//animations
	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	//init XML
	TiXmlDocument doc("ResourceXML\\door.xml");
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
			sprites->Add(idFrame, left, top, right, bottom, texdoor);
			ani->Add(idFrame);
		}

		//load 1 frame finish
		animations->Add(id, ani);
		this->AddAnimation(id);
	}
}

void Door::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT>* coObjectEnemy)
{
	CGameObject::Update(dt);

}

void Door::Render(float xViewport, float yViewport)
{
	//Update action
	this->Action(xViewport, yViewport);

	this->SetState(this->state);

	animations[ani]->Render(x - xViewport, y - yViewport);
}

void Door::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{

}

void Door::Action(float xViewport, float yViewport)
{
	if ((this->x - (xViewport + SCREEN_WIDTH / 2)) <= DIS_PANTHER_ATTACK)
	{
		this->isAction = true;
	}
}

void Door::SetState(int _state)
{
	CGameObject::SetState(_state);
	
	switch (this->state)
	{
		case DOOR_CLOSE:
		{
			this->state = DOOR_CLOSE;
			ani = DOOR_CLOSE;
			if (isAction)
			{
				this->state = DOOR_OPEN;
				ani = DOOR_OPEN;
			}
			break;
		}

		case DOOR_OPEN:
		{
			if (animations[DOOR_OPEN]->CheckDoAllFrame())
			{
				this->state = DOOR_STATIC;
				ani = DOOR_STATIC;
				//animations[ani]->SetDoAllFrame(false);
			}
			break;
		}

		case DOOR_STATIC:
		{
			break;
		}
	}
}
