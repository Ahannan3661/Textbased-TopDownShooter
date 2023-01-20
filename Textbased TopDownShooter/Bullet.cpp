#include "Bullet.h"

Bullet::Bullet(int x, int y, bool direction, bool home, bool hostile)
{
	Birth();
	this->hostile = hostile;
	this->x = x;
	this->y = y;
	this->direction = direction;
	this->home = home;
	wstring ascii_art;
	if (home)
	{
		ascii_art.append(L">");
	}
	else
	{
		ascii_art.append(L"*");
	}
	UpdateArt(ascii_art);

	//1x1 sprite
	SetWidth(1);
	SetHeight(1);
}

void Bullet::Move()
{
	//homing bullet
	if (home && x > 0 && x < 120 && home_target != nullptr)
	{
		//home towards target direction using unit vector
		float magnitude = sqrt(((home_target->x + 2 - this->x) * (home_target->x + 2 - this->x)) + ((home_target->y + 2 - this->y) * (home_target->y + 2 - this->y)));
		this->x += (((home_target->x + 2 - this->x) / magnitude) * 3);
		this->y += (((home_target->y + 2 - this->y) / magnitude) * 3);

		//to aproximate float values
		//if (this->x - home_target->x + 2 == 1) this->x -= 1;
		//if (this->x - home_target->x + 2 == -1) this->x += 1;
		//if (this->y - home_target->y + 2 == 1) this->y -= 1;
		//if (this->y - home_target->y + 2 == -1) this->y += 1;

		if (y < 0) y = 0;
		if (y > 39) y = 39;
		if (x < 0) x = 0;
		if (x > 119) x = 119;
	}
	//normal bullets
	else if (x > 0 && x<120)
	{
		if (direction)
			x+=3;
		else
			x-=3;
	}
	else
		Kill();
}

bool Bullet::IsHostile()
{
	return hostile;
}

bool Bullet::Home()
{
	return home;
}

void Bullet::SetTarget(GameObject* enemy)
{
	home_target = enemy;
}
GameObject* Bullet::GetTarget()
{
	return home_target;
}