#pragma once
#include "Node.h"
using namespace std;

class HashTable {
private:
	int sizeOfTable;
	Node* hashArr;

	int GetHash(int key);
public:
	HashTable(int size);
	int Length();
	Node* GetArray();

	void Add(string value, int key);
	bool Edit(int key, string value);

	string GetValueByKey(int key);
	Node* GetNodeByKey(int key);
	bool DeleteByKey(int key);

	bool IsExists(int key);

	string GetSizeOfTableInBytes();
	void Save(string fileName);
	void Load(string fileName);
};
