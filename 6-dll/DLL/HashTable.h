#pragma once
#include "Node.h"
#ifdef MATHFUNCSDLL_EXPORTS
#define MATHFUNCSDLL_API __declspec(dllexport) 
#else
#define MATHFUNCSDLL_API __declspec(dllimport) 
#endif

/*
Когда символ MATHFUNCSDLL_EXPORTS определен, 
символ MATHFUNCSDLL_API установит модификатор __declspec(dllexport) в объявлениях функций-членов в этом коде. 
Этот модификатор разрешает экспорт функции библиотекой DLL для использования ее другими приложениями.
Если символ MATHFUNCSDLL_EXPORTS не определен, например, когда файл заголовка включен приложением, 
символ MATHFUNCSDLL_API определяет модификатор __declspec(dllimport) в объявлениях функций-членов.
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
