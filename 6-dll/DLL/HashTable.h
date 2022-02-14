#pragma once
#include "Node.h"
#ifdef MATHFUNCSDLL_EXPORTS
#define MATHFUNCSDLL_API __declspec(dllexport) 
#else
#define MATHFUNCSDLL_API __declspec(dllimport) 
#endif

/*
����� ������ MATHFUNCSDLL_EXPORTS ���������, 
������ MATHFUNCSDLL_API ��������� ����������� __declspec(dllexport) � ����������� �������-������ � ���� ����. 
���� ����������� ��������� ������� ������� ����������� DLL ��� ������������� �� ������� ������������.
���� ������ MATHFUNCSDLL_EXPORTS �� ���������, ��������, ����� ���� ��������� ������� �����������, 
������ MATHFUNCSDLL_API ���������� ����������� __declspec(dllimport) � ����������� �������-������.
*/

class MATHFUNCSDLL_API HashTable {
private:
	int sizeOfTable;
	Node *hashArr;

public:
	HashTable(int size);
	int Length();
	Node* GetArray();
	int GetHash(int key);

	void Add(string value, int key);
	bool Edit(int key, string value);
	bool DeleteByKey(int key);

	string GetValueByKey(int key);
	Node *GetNodeByKey(int key);

	bool IsExists(int key);

	string GetSizeOfTableInBytes();
	void Save(string fileName);
	void Load(string fileName);

	HashTable* NewHashTable(int size);
};
