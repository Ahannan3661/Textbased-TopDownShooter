#pragma once
#include <iostream>
#include <stdlib.h> 
#include "Node.h"

using namespace std;

class LinkedList
{
private:
	Node* head;
	Node* tail;
	int length;
public:

	LinkedList();
	void Insert(GameObject* data);
	int Size();

	//returns cloud pointer in the Node at index position of the list
	GameObject* At(int index);

	//remove the node in list at index position
	void Erase(int index);

	Node* Begin();
};