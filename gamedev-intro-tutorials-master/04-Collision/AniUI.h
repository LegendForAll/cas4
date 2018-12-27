#pragma once
#include "GameObject.h"

class AniUI : public CGameObject
{
	bool isUpdate = false;
	int ani = 0;

public:

	AniUI();
	~AniUI();

	virtual void LoadResource();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects, vector<LPGAMEOBJECT> *coObjectStatic, vector<LPGAMEOBJECT> *coObjectEnemy);
	virtual void Render(float xViewport, float yViewport);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
};
