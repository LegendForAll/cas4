#pragma once
#include "GameObject.h"

class Door : public CGameObject
{

	int ani = DOOR_CLOSE;
	bool isAction = false;

public:

	Door();
	~Door();

	virtual void LoadResource();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects, vector<LPGAMEOBJECT> *coObjectStatic, vector<LPGAMEOBJECT> *coObjectEnemy);
	virtual void Render(float xViewport, float yViewport);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);

	void Action(float xViewport, float yViewport);
	void SetState(int _state);
};