#include "LinkedList.h"
#include "Node.h"

LinkedList::LinkedList()
{
	head = nullptr;
	tail = nullptr;
	length = 0;
}

void LinkedList::Insert(GameObject* data)
{
	Node* node = new Node(data);
	//list is empty
	if (head == nullptr)
	{
		head = node;
		tail = node;
	}
	//insertion at the end of the list
	else
	{
		tail->next = node;
		node->previous = tail;
		tail = node;
	}
	length++;
}

int LinkedList::Size()
{
	return length;
}

//returns cloud pointer in the Node at index position of the list
GameObject* LinkedList::At(int index)
{
	Node* temp = head;
	int i = 0;
	while (temp != nullptr)
	{
		if (index == i) return temp->data;
		i++;
		temp = temp->next;
	}
	return NULL;
}

//remove the node in list at index position
void LinkedList::Erase(int index)
{
	Node* temp = head;
	int i = 0;
	while (temp != nullptr)
	{
		if (index == i)
		{
			if (temp->previous == nullptr && temp->next == nullptr)
			{
				head = nullptr;
				tail = nullptr;
			}
			else if (temp->previous == nullptr)
			{
				head = temp->next;
				head->previous = nullptr;
			}
			else if (temp->next == nullptr)
			{
				tail = temp->previous;
				tail->next = nullptr;
			}
			else
			{
				temp->previous->next = temp->next;
				temp->next->previous = temp->previous;
			}
			delete(temp->data);
			delete(temp);
			temp = nullptr;
			length--;
			return;
		}
		i++;
		temp = temp->next;
	}
}

Node* LinkedList::Begin()
{
	return head;
}