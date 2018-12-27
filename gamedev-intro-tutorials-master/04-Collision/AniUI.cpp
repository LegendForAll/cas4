#include "AniUI.h"
#include "tinyxml.h"

AniUI::AniUI()
{

}

AniUI::~AniUI()
{

}

void AniUI::LoadResource()
{
	//load resource zombie
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_MENU, L"Resource\\sprites\\Effect\\menu.png", D3DCOLOR_XRGB(255, 0, 255));

	//sprites
	CSprites * sprites = CSprites::GetInstance();
	LPDIRECT3DTEXTURE9 texMe = textures->Get(ID_TEX_MENU);

	//animations
	LPANIMATION ani;
	CAnimations * animations = CAnimations::GetInstance();

	//init XML
	TiXmlDocument doc("ResourceXML\\animeintro.xml");
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
			sprites->Add(idFrame, left, top, right, bottom, texMe);
			ani->Add(idFrame);
		}

		//load 1 frame finish
		animations->Add(id, ani);
		this->AddAnimation(id);
	}
}

void AniUI::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects, vector<LPGAMEOBJECT>* coObjectStatic, vector<LPGAMEOBJECT>* coObjectEnemy)
{

}

void AniUI::Render(float xViewport, float yViewport)
{
	CGameObject::SetState(this->state);
	switch (this->state)
	{
		case MI_BAT_MENU:
		{
			this->ani = MI_BAT_MENU;
			break;
		}

		case MI_BAT_INTRO:
		{
			this->ani = MI_BAT_INTRO;
			break;
		}

		case MI_SKY_INTRO:
		{
			this->ani = MI_SKY_INTRO;
			break;
		}

		animations[ani]->Render(x, y);
	}
	animations[ani]->Render(x, y);
}

void AniUI::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{

}
