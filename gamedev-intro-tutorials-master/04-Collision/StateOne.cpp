#include "StateOne.h"
#include "tinyxml.h"

StateOne::StateOne()
{

}

StateOne::~StateOne()
{

}

void StateOne::InitState(LPDIRECT3DDEVICE9 dv, LPDIRECT3DSURFACE9 bb)
{

}

void StateOne::LoadState(int AS)
{
	CGameState::LoadState(AS);

	//Simon
	//simon = new CMario();
	//simon->LoadResource();
	//simon->SetPosition(50.0f, 0);
	//simon->SetState(SM_STAND_STATIC);
	//objects.push_back(simon);
	isInitSimon = true;

	//door
	Door *dr = new Door();
	dr->LoadResource();
	dr->SetPosition(800, 280);
	dr->SetState(DOOR_CLOSE);
	//objecttsStatic.push_back(dr);

	Zombie *z = new Zombie();
	z->LoadResource();
	z->SetPosition(600, 200);
	//objectsEnemy.push_back(z);

	//item rosary
	Items *ite = new Items();
	ite->LoadResource();
	ite->SetPosition(600, 200);
	ite->SetState(I_ROSARY);
	//objects.push_back(ite);

	Items *ite1 = new Items();
	ite1->LoadResource();
	ite1->SetPosition(200, 200);
	ite1->SetState(I_STOPWATCH);
	objects.push_back(ite1);

	Items *ite2 = new Items();
	ite2->LoadResource();
	ite2->SetPosition(400, 200);
	ite2->SetState(I_KNIFE);
	//objects.push_back(ite2);

	//effect
	ef = new CEffect();
	ef->LoadResource();
	ef->SetPosition(600, 200);
	ef->SetState(EFFECT_THOR);
	ef->SetDead(true);
	objecttsStatic.push_back(ef);

	//UI
	uigame = new UI();
	uigame->LoadResource();
	uigame->SetPosition(0.0f, 20.0f);
	objects.push_back(uigame);

	//Groud
	BOXStair *box_dl = new BOXStair();
	box_dl->LoadResource();
	box_dl->SetPosition(1300.0f, 380.0f);
	box_dl->SetState(BOX_LEFT);
	//objecttsStatic.push_back(box_dl);

	//----------------------------------------------------------
	//init XML
	TiXmlDocument doc("ResourceXML\\mapR.xml");
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
	}
	int id;
	int id_obj;
	int posX, posY;
	

	//load XML candle
	TiXmlElement* rootcan = doc.RootElement();
	TiXmlElement* candle = nullptr;
	candle = rootcan->FirstChildElement("candle");
	for (TiXmlElement* obj = candle->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);

		//init candle
		Candle *can = new Candle();
		can->LoadResource();
		can->SetPosition(posX, posY);
		objects.push_back(can->GetItem());
		objecttsStatic.push_back(can);
	}

	//load XML limit
	TiXmlElement* rootlim = doc.RootElement();
	TiXmlElement* limit = nullptr;
	limit = rootlim->FirstChildElement("limit");
	for (TiXmlElement* obj = limit->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);

		//init limit
		CLimit *lim = new CLimit();
		lim->SetPosition(posX, posY);
		objects.push_back(lim);
	}

	//load XML ground
	TiXmlElement* rootG = doc.RootElement();
	TiXmlElement* ground = nullptr;
	float multiplier;
	int typeG;
	ground = rootG->FirstChildElement("ground");
	for (TiXmlElement* obj = ground->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryFloatAttribute("multiplier", &multiplier);
		obj->QueryIntAttribute("state", &typeG);


		//init ground
		CLimit *limG = new CLimit();
		limG->SetwidthBBox(multiplier);
		limG->SetPosition(posX, posY);
		limG->SetState(typeG);
		objects.push_back(limG);
	}

	//load XML boxmap
	TiXmlElement* rootMap = doc.RootElement();
	TiXmlElement* change = nullptr;
	int Cmap;
	change = rootMap->FirstChildElement("changemap");
	for (TiXmlElement* obj = change->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryIntAttribute("change", &Cmap);


		//init box map
		BoxMap *map = new BoxMap();
		map->LoadResource();
		map->SetPosition(posX, posY);
		map->SetAniBoxMap(Cmap);
		objecttsStatic.push_back(map);
	}
	//----------------------------------------------------------
}

void StateOne::UpdateState(DWORD dt)
{
	CGameState::UpdateState(dt);
	//effect map
	if (simon->GetStateItemSimon() == I_ROSARY)
	{
		ef->SetDead(false);
	}

	//--------------------------------------------------------------------
	if (simon->GetStateItemSimon() == I_STOPWATCH)
	{
		ef->SetDead(false);
	}
	//---------------------------------------------------------------------
	this->uigame->UpdateItem(simon);
}

void StateOne::RenderState(LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 bb, LPD3DXSPRITE spriteHandler)
{
	//Update viewport


	tilemap->UpdateScrollPosition();
	CGameState::RenderState(d3ddv, bb, spriteHandler);

}
