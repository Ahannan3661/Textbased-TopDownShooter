#pragma once
#include<iostream>
#include <stdlib.h> 

using namespace std;

class GameObject
{
private:
	wstring ascii_art;
	bool alive = true;
	int width = 0;
	int height = 0;
public:

	int x = 0;
	int y = 0;

	virtual void Move() = 0;

	virtual wstring Art()
	{
		return ascii_art;
	}

	void UpdateArt(wstring ascii_art)
	{
		this->ascii_art = ascii_art;
	}

	bool Alive()
	{
		return alive;
	}

	void Kill()
	{
		alive = false;
	}

	void Birth()
	{
		alive = true;
	}
	int Width()
	{
		return width;
	}
	int Height()
	{
		return height;
	}
	void SetWidth(int w)
	{
		width = w;
	}
	void SetHeight(int h)
	{
		height = h;
	}
};

