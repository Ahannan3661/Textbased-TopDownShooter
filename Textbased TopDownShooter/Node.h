#pragma once
#include <iostream>
#include <stdlib.h> 
#include "GameObject.h"

using namespace std;

class Node
{
public:
	Node* next;
	Node* previous;
	GameObject* data;

	Node(GameObject* data)
	{
		next = nullptr;
		previous = nullptr;
		this->data = data;
	}
};