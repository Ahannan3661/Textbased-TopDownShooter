#include "Player.h"
#include "Bullet.h"

Player::Player(int x, int y)
{
	Birth();
	this->x = x;
	this->y = y;
	tx = 0;
	ty = 0;
	power_up = true;

	wstring ascii_art;
	ascii_art.append(L"@@  ");
	ascii_art.append(L"@@@@");
	ascii_art.append(L"@@@@");
	ascii_art.append(L"@@  ");
	UpdateArt(ascii_art);

	//4x4 sprite
	SetWidth(4);
	SetHeight(4);

}

void Player::Move()
{
	//cooldown tracking and reset
	if (power_up_cooldown != 0 && power_up)
		power_up_cooldown--;
	else
	{
		power_up = false;
		power_up_cooldown = 100;
	}

	if(shooting_coolDown !=0)
		shooting_coolDown--;

	//keeping in bounds
	if (x >= 0 && x<=116 && y >= 0 && y <= 36)
	{
		x += (int)tx;
		y += (int)ty;
		tx *= friction;
		ty *= friction;
	}
	else
	{
		if (x < 0) x = 0;
		if (x > 116) x = 116;
		if (y < 0) y = 0;
		if (y > 36) y = 36;
		tx = 0;
		ty = 0;
	}
}

void Player::Translate(float tx, float ty)
{
	this->tx = tx;
	this->ty = ty;
}

GameObject* Player::Shoot()
{
	GameObject* bullet;
	if (power_up)
	{
		//homing bullet
		bullet = new Bullet(x + 5, y + 2, true, true, false);
	}
	else
	{
		//normal bullet
		bullet = new Bullet(x + 5, y + 2, true, false, false);
	}
	shooting_coolDown = 15;
	return bullet;
}

bool Player::OnCoolDown()
{
	return shooting_coolDown != 0;
}

void Player::PowerUp()
{
	power_up = true;
}

//used to stack powerup duration
void Player::IncreasePowerUpCoolDown()
{
	power_up_cooldown += 100;
}

bool Player::PowerUpStatus()
{
	return power_up;
}