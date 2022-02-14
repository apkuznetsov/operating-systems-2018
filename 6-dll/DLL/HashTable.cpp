#include "stdafx.h"
#include "HashTable.h"
#include <windows.h>
#include <vector>
using namespace std;

HashTable::HashTable(int size)
{
	sizeOfTable = size;
	hashArr = new Node[sizeOfTable];
}
int HashTable::Length()
{
	return sizeOfTable;
}
Node* HashTable::GetArray()
{
	return hashArr;
}
int HashTable::GetHash(int key)
{
	int hash = key % sizeOfTable;
	return hash;
}

void HashTable::Add(string value, int key)
{
	Node* newNode = new Node();
	newNode->SetKey(key);
	newNode->SetValue(&value[0]);
	newNode->SetNext(NULL);
	hashArr[GetHash(newNode->GetKey())].GetLastNode()->SetNext(newNode);
}
bool HashTable::Edit(int key, string value)
{
	Node* tempNode = GetNodeByKey(key);
	if (tempNode == NULL)
		return false;
	else
	{
		tempNode->SetValue(&value[0]);
		return true;
	}
}
bool HashTable::DeleteByKey(int key)
{
	Node *currentNode = &hashArr[GetHash(key)];
	if (currentNode->GetKey() == key)
	{
		delete(&hashArr[GetHash(key)]);
		hashArr[GetHash(key)] = *currentNode->GetNext();
		return true;
	}
	else {
		do
		{
			if (currentNode->GetNext()->GetKey() == key)
			{
				Node *bufferNode = currentNode->GetNext();
				currentNode->SetNext(currentNode->GetNext()->GetNext());
				delete(bufferNode);
				return true;
			}
			currentNode = currentNode->GetNext();
		} while (currentNode != NULL);
	}
	return false;
}

string HashTable::GetValueByKey(int key)
{
	Node* tempNode = GetNodeByKey(key);
	return (tempNode != NULL ? tempNode->GetValue() : "");
}
Node* HashTable::GetNodeByKey(int key)
{
	Node *currentNode = &hashArr[GetHash(key)];
	while (currentNode->GetNext() != NULL)
	{
		currentNode = currentNode->GetNext();
		if (currentNode->GetKey() == key) {
			return currentNode;
		}
	}
	return NULL;
}

bool HashTable::IsExists(int key)
{
	if (GetNodeByKey(key) == NULL) return false;
	else return true;
}

string HashTable::GetSizeOfTableInBytes()
{
	string result;
	byte* bytes = new byte;	// unsigned char

							/* ������� memmove �������� n ���� �� ������� (������� ������),
							�� ������� ��������� �������� source,
							� ������ (������� ������),
							�� ������� ��������� �������� destination. */
	memmove(
		bytes,				// ��������� �� ������ � ������� ����� ����������� ������.
		&sizeOfTable,		// ��������� �� ������ �������� ���������� ������.
		sizeof(int));		// ���������� ���� ��� �����������
	for (int i = 0; i < 4; i++)
		result += *(bytes + i);
	return result;
}
void HashTable::Save(string fileName)
{
	HANDLE hFile = CreateFile(
		wstring(fileName.begin(), fileName.end()).c_str(),	// ��� �����, ������������� � ���� char*
		GENERIC_WRITE,										// ������ ��� ������ � ������
		0,													// ���� �� ����� ���� �����������
		NULL,												// ���������� ����� �� �����������
		CREATE_NEW,											// ������� �����, ���� �� ����������
		FILE_ATTRIBUTE_NORMAL,								// ���� �� ����� ������ ���������
		NULL												// ������ NULL ��� Windows
	);
	if (hFile != INVALID_HANDLE_VALUE)			// ���� ���� ������
	{
		DWORD temp;

		WriteFile(
			hFile,								// ���������� �����
			GetSizeOfTableInBytes().c_str(),	// ����� ������ (������ ���������������� � ������ ��������)
			GetSizeOfTableInBytes().length(),	// ����� ������ ��� ������
			&temp,								// ����� ���������� ������
			NULL);								// ����������� ����� (�����������)

		for (int i = 0; i < sizeOfTable; i++)
		{
			Node tempNode = hashArr[i];
			while (tempNode.GetNext() != NULL)
			{
				tempNode = *tempNode.GetNext();
				WriteFile(
					hFile,
					tempNode.GetBytes().c_str(),
					tempNode.GetBytes().length(),
					&temp,
					NULL);
			}
		}
		CloseHandle(hFile);
	}
	else	// ���� �� ������
	{
		CloseHandle(hFile);
		throw "������: ���� �� ������";
	}
}
void HashTable::Load(string fileName)
{
	HANDLE hFile = CreateFile(
		wstring(fileName.begin(), fileName.end()).c_str(),
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD temp;

		int arrSize = 0;
		// ��������� �� ����� �
		if (ReadFile(
			hFile,
			&arrSize,
			sizeof(int),
			&temp,
			NULL) == 0)
		{
			throw "������ ��� ������ �����";
		};

		hashArr = new Node[arrSize];
		sizeOfTable = arrSize;

		while (temp != 0)	// ���� ���������� ����������
		{
			int key = 0;
			if (ReadFile(
				hFile,
				&key,
				sizeof(int),
				&temp,
				NULL) == 0)
			{
				throw "��� ������ ����� �������� ������! ����";
			};

			int length = 0;
			if (ReadFile(
				hFile,
				&length,
				sizeof(int),
				&temp,
				NULL) == 0)
			{
				throw "��� ������ ����� �������� ������! �����";
			};

			char* valueChars = new char[length] {0};
			if (ReadFile(
				hFile,
				valueChars,
				sizeof(byte) * length,
				&temp,
				NULL) == 0)
			{
				throw "��� ������ ����� �������� ������! ������";
			};
			string value = valueChars;
			value = value.substr(0, length);

			if (temp != 0) Add(value, key);
		}

		CloseHandle(hFile);
	}
	else
	{
		CloseHandle(hFile);
		throw "������: ���� ���������� �������";
		return;
	}
}

HashTable* HashTable::NewHashTable(int size)
{
	return new HashTable(size);
}
