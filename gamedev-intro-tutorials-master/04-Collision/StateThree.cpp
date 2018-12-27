#include "StateThree.h"
#include "tinyxml.h"

StateThree::StateThree()
{

}

StateThree::~StateThree()
{

}

void StateThree::InitState(LPDIRECT3DDEVICE9 dv, LPDIRECT3DSURFACE9 bb)
{

}

void StateThree::LoadState(int AS)
{
	CGameState::LoadState(AS);

	isInitSimon = true;

	//r1
	r1.left = 0;
	r1.top = 200;
	r1.right = 160;
	r1.bottom = 400;

	//r2
	r2.left = 254;
	r2.top = 200;
	r2.right = 420;
	r2.bottom = 400;

	//r2
	r3.left = 698;
	r3.top = 200;
	r3.right = 891;
	r3.bottom = 400;

	pointdoor.clear();
	//D3DXVECTOR2 pos(100.0f, 250.0f);
	D3DXVECTOR2 pos(106.0f, 100.0f);
	D3DXVECTOR2 pos1(764.0f, 100.0f);
	pointdoor.push_back(pos);
	pointdoor.push_back(pos1);

	if (AS >= 0)
	{
		simon->SetPosition(pointdoor[AS].x, pointdoor[AS].y - 80);
	}


	//Box Map
	BoxMap *map1 = new BoxMap();
	map1->LoadResource();
	map1->SetPosition(pointdoor[0].x, pointdoor[0].y);
	map1->SetASDoor(AS1);
	map1->SetAniBoxMap(BOX_CHANGE_MAP1);
	objecttsStatic.push_back(map1);

	BoxMap *map2 = new BoxMap();
	map2->LoadResource();
	map2->SetPosition(pointdoor[1].x, pointdoor[1].y);
	map2->SetASDoor(AS2);
	map2->SetAniBoxMap(BOX_CHANGE_MAP1);
	objecttsStatic.push_back(map2);

	//set position simon undoor
	//-----------------------------------------------------
	//init XML
	TiXmlDocument doc("ResourceXML\\map1B.xml");
	if (!doc.LoadFile())
	{
		printf("%s", doc.ErrorDesc());
	}
	int id;
	int id_obj;
	int posX, posY;

	//load XML ground
	TiXmlElement* rootG = doc.RootElement();
	TiXmlElement* ground = nullptr;
	float multiplier;
	ground = rootG->FirstChildElement("ground");
	for (TiXmlElement* obj = ground->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryFloatAttribute("multiplier", &multiplier);

		//init ground
		CLimit *limG = new CLimit();
		limG->SetwidthBBox(multiplier);
		limG->SetPosition(posX, posY);
		limG->SetState(LIMIT_GROUND);
		objects.push_back(limG);
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
		lim->SetState(LIMIT_CAMERA);
		objects.push_back(lim);
	}

	//load XML limitjump
	TiXmlElement* rootjump = doc.RootElement();
	TiXmlElement* jump = nullptr;
	float mul;
	jump = rootjump->FirstChildElement("limjump");
	for (TiXmlElement* obj = jump->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryFloatAttribute("multiplier", &mul);

		//init jump
		CLimit *limJ = new CLimit();
		limJ->SetwidthBBox(mul);
		limJ->SetPosition(posX, posY);
		limJ->SetState(LIMIT_GROUND);
		objecttsStatic.push_back(limJ);
	}

	//load XML candle
	TiXmlElement* rootcan = doc.RootElement();
	TiXmlElement* candle = nullptr;
	candle = rootcan->FirstChildElement("candlemi");
	for (TiXmlElement* obj = candle->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);

		//init candle
		CandleSmall *can = new CandleSmall();
		can->LoadResource();
		can->SetPosition(posX, posY);
		can->SetState(CANDLESMALL_STATE_NO);
		objects.push_back(can->GetItem());
		objecttsStatic.push_back(can);
	}

	//load XML merman
	TiXmlElement* rootmer = doc.RootElement();
	TiXmlElement* man = nullptr;
	int rectlim;
	man = rootmer->FirstChildElement("merman");
	for (TiXmlElement* obj = man->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryIntAttribute("rect", &rectlim);

		//init candle
		Merman *mer2 = new Merman();
		mer2->LoadResource();
		mer2->SetPosition(posX, posY);
		mer2->SetState(MERMAN_JUMP);
		switch (rectlim)
		{
			case 1:
			{
				mer2->Set_Rlim(r1);
				break;
			}
			case 2:
			{
				mer2->Set_Rlim(r2);
				break;
			}
			case 3:
			{
				mer2->Set_Rlim(r3);
				break;
			}
		}
		objectsEnemy.push_back(mer2->GetFireball());
		objectsEnemy.push_back(mer2);
	}

	//load XML stair
	TiXmlElement* rootstair = doc.RootElement();
	TiXmlElement* stair = nullptr;
	int isLeft, boxState;
	stair = rootstair->FirstChildElement("boxstair");
	for (TiXmlElement* obj = stair->FirstChildElement(); obj != NULL; obj = obj->NextSiblingElement())
	{
		obj->QueryIntAttribute("id", &id_obj);
		obj->QueryIntAttribute("x", &posX);
		obj->QueryIntAttribute("y", &posY);
		obj->QueryIntAttribute("left", &isLeft);
		obj->QueryIntAttribute("state", &boxState);

		//init boss
		BOXStair *box11 = new BOXStair();
		box11->LoadResource();
		box11->SetcurrentBox(isLeft);
		box11->SetState(boxState);
		box11->SetPosition(posX, posY);
		objecttsStatic.push_back(box11);
	}
	//-----------------------------------------------------
}

void StateThree::UpdateState(DWORD dt)
{
	//this->createEnemy();
	CGameState::UpdateState(dt);
}

void StateThree::RenderState(LPDIRECT3DDEVICE9 d3ddv, LPDIRECT3DSURFACE9 bb, LPD3DXSPRITE spriteHandler)
{
	//Update viewport
	tilemap->UpdateScrollPosition();
	CGameState::RenderState(d3ddv, bb, spriteHandler);
}

void StateThree::createEnemy()
{
	//simon thuoc RECT?
	RECT temp = r1;
	if (simon->x >= r2.left)
	{
		temp = r2;
	}

	//so luong merman con tren map
	//if (merA->isDead || merB->isDead)
	//{
	//	numMerman--;
	//}


	if (numMerman < 2)
	{
		//create merman khung thich hop
		//if (merA->isDead)
		{
			merA = new Merman();
			merA->LoadResource();
			merA->SetPosition(temp.left + 100.0f, 400.0f);
			merA->SetState(MERMAN_JUMP);
			merA->Set_Rlim(temp);
			objectsEnemy.push_back(merA->GetFireball());
			objectsEnemy.push_back(merA);
			numMerman++;
		}

		//if (merB->isDead)
		{
			merB = new Merman();
			merB->LoadResource();
			merB->SetPosition(temp.left + 150.0f, 400.0f);
			merB->SetState(MERMAN_JUMP);
			merB->Set_Rlim(temp);
			objectsEnemy.push_back(merB->GetFireball());
			objectsEnemy.push_back(merB);
			numMerman++;
		}

	}
	
}
