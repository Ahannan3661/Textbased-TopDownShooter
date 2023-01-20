#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"

using namespace std;

// Shooting enemy
class SEnemy : public GameObject
{
private:
	const float friction = 0.9;
	float tx, ty;
	int shooting_cooldown = 25;
public:
	SEnemy(int x, int y);
	void Move();
	GameObject* Shoot();
	bool OnCoolDown();
	void Translate(float x, float y);
};
