#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"

using namespace std;

class Bullet : public GameObject
{
private:
	
	bool direction; //left to right or right to left
	bool home; //should home or not
	bool hostile; //is shot by enemy or not

	GameObject* home_target = nullptr; //target enemy
public:
	Bullet(int x, int y, bool direction, bool home, bool hostile);
	void Move();
	bool Home();
	bool IsHostile();
	void SetTarget(GameObject*);
	GameObject* GetTarget();
};
