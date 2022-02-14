#pragma once
#include <string>
#include <atlstr.h>
#include <vector>
using namespace std;

struct Node {
private:
	int key;
	string value;
	struct Node *next;
public:
	Node()
	{
		this->next = NULL;
		this->key = NULL;
		this->value = "";
	}

	int GetKey()
	{
		return key;
	}

	string GetValue()
	{
		return value;
	}

	struct Node *GetNext()
	{
		return next;
	}

	void SetKey(int key)
	{
		this->key = key;
	}

	void SetValue(string value)
	{
		this->value = value;
	}

	void SetNext(Node *next)
	{
		this->next = next;
	}

	Node* GetLastNode()
	{
		Node *currentNode = this;
		while (currentNode->GetNext() != NULL)
		{
			currentNode = currentNode->GetNext();
		}
		return currentNode;
	}

	string GetBytes()
	{
		byte* keyBytes = new byte;
		byte* valueBytes = new byte;
		int lenth = value.length();

		memmove(keyBytes, &key, sizeof(int));
		memmove(valueBytes, &lenth, sizeof(int));

		string result;

		for (int i = 0; i < 4; i++)
			result += *(keyBytes + i);

		for (int i = 0; i < 4; i++)
			result += *(valueBytes + i);

		result += value;
		return result;
	}
};
