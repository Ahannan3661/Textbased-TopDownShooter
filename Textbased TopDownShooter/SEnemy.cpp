#include "SEnemy.h"
#include "Bullet.h"

SEnemy::SEnemy(int x, int y)
{
	Birth();
	this->x = x;
	this->y = y;
	tx = 0;
	ty = 0;

	wstring ascii_art;
	ascii_art.append(L"   <");
	ascii_art.append(L"<<< ");
	ascii_art.append(L"<<< ");
	ascii_art.append(L"   <");
	UpdateArt(ascii_art);

	//4x4 sprite
	SetWidth(4);
	SetHeight(4);
}

void SEnemy::Move()
{
	//cooldown tracking
	if (shooting_cooldown != 0) shooting_cooldown--;
	if (x >= 0)
	{
		x += (int)tx;
		tx *= friction;
	}
	else
		Kill();
}

GameObject* SEnemy::Shoot()
{
	//cooldown reset
	shooting_cooldown = 25;

	//hostle normal bullet
	return new Bullet(x - 2, y + 2, false, false, true);
}

bool SEnemy::OnCoolDown()
{
	return shooting_cooldown != 0;
}

void SEnemy::Translate(float x, float y)
{
	tx = x;
	ty = y;
}