#pragma once
#include<iostream>
#include <stdlib.h> 
#include "GameObject.h"

using namespace std;

class PowerUp : public GameObject
{
public:

	PowerUp(int x, int y)
	{
		Birth();
		this->x = x;
		this->y = y;

		wstring ascii_art;
		ascii_art.append(L"|^^|");
		ascii_art.append(L"|^^|");
		ascii_art.append(L"|^^|");
		ascii_art.append(L"|^^|");
		UpdateArt(ascii_art);

		//4x4 sprite
		SetWidth(4);
		SetHeight(4);
	}

	void Move() 
	{
		if (y <= 36)
			y++;
		else
			Kill();
	}
};
