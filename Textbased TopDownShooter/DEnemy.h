#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"

using namespace std;

// dodging enemy
class DEnemy : public GameObject
{
private:
	const float friction = 0.9;
	float tx;
	float ty;
	int dodge_cooldown = 15;
public:
	bool dodged;
	DEnemy(int x, int y);
	void Move();
	void Translate(float x, float y);
};
