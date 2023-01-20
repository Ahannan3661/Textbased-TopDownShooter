#include "DEnemy.h"

DEnemy::DEnemy(int x, int y)
{
	Birth();
	this->dodged = false;
	this->x = x;
	this->y = y;
	tx = 0;
	ty = 0;

	wstring ascii_art;
	ascii_art.append(L"   @");
	ascii_art.append(L"@@@ ");
	ascii_art.append(L"@@@ ");
	ascii_art.append(L"   @");
	UpdateArt(ascii_art);

	//4x4 sprite
	SetWidth(4);
	SetHeight(4);
}

void DEnemy::Move()
{
	if (x >= 0)
	{
		//cooldown tracking and reset
		if (dodged) dodge_cooldown--;
		if (dodge_cooldown == 0)
		{
			dodged = false;
			dodge_cooldown = 15;
		}

		x += (int)tx;
		y += (int)ty;
		tx *= friction;
		ty *= friction;

		//keeping in bounds
		if (y < 0) y = 0;
		if (y > 36) y = 36;
	}
	else
		Kill();
}

void DEnemy::Translate(float x, float y)
{
	tx = x;
	//to make sure "dodge" action is completed and not overwritten
	if (y == 0 || ty > 0) return;
	ty = y;
}