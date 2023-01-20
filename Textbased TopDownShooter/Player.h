#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"

using namespace std;

class Player : public GameObject
{
private:
	float tx, ty;
	const float friction = 0.9;
	bool power_up;
	int shooting_coolDown = 15;
	int power_up_cooldown = 100;
public:
	
	Player(int x, int y);
	void Move();
	void Translate(float tx, float ty);
	bool OnCoolDown();
	GameObject* Shoot();
	void PowerUp();
	bool PowerUpStatus();
	void IncreasePowerUpCoolDown();
};
