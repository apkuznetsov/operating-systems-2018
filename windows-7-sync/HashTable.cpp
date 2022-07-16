#include "stdafx.h"
#include "HashTable.h"
#include <windows.h>
#include <vector>
using namespace std;

// аргументы
struct ADDFUNC_ARGS
{
	HANDLE* mutex;
	int key;
	string value;
	HashTable* hashTable;
};
struct SAVEFUNC_ARGS
{
	HANDLE* mutex;
	string fileName;
	HashTable* hashTable;
};

// функция потока:
DWORD WINAPI AddThread(LPVOID vArgs)			// LPVOID - это указатель.При этом это указатель(P от Pointer) на что угодно (VOID) и дальний(L - от Long).
{
	ADDFUNC_ARGS* args = (ADDFUNC_ARGS*)vArgs;	// явно преобразуем аргумент функции к структуре

												// присваивание полученных аргументов:
	HANDLE* mutex = args->mutex;
	int key = args->key;
	string value = args->value;
	HashTable* hashTable = args->hashTable;

	WaitForSingleObject(*mutex, INFINITE);

	Node* tempNode = new Node();
	tempNode->SetKey(key);
	tempNode->SetValue(&value[0]);
	tempNode->SetNext(NULL);
	/*
	1) доступ к элементу (узлу) по индексу
	2) индекс -- хэш-значение, сформированное из ключа узла
	3) узел расположенный по индексу -- ссылается на новый элемент
	*/
	hashTable->GetArray()[hashTable->GetHash(tempNode->GetKey())].GetLastNode()->SetNext(tempNode);

	ReleaseMutex(mutex);
	ExitThread(0);
}

DWORD WINAPI SaveThread(LPVOID vArgs)
{
	SAVEFUNC_ARGS* args = (SAVEFUNC_ARGS*)vArgs;

	HANDLE* mutex = args->mutex;
	HashTable* hashTable = args->hashTable;
	string fileName = args->fileName;

	WaitForSingleObject(*mutex, INFINITE);

	hashTable->Save(fileName);

	ReleaseMutex(mutex);
	ExitThread(0);
}

void HashTable::Add(string value, int key)
{
	HANDLE mutex = CreateMutex(NULL, FALSE, NULL);

	ADDFUNC_ARGS addArgs = { &mutex, key, value , this };
	SAVEFUNC_ARGS saveArgs = { &mutex, "file.txt", this };

	HANDLE addThread = CreateThread(NULL, 0, &AddThread, (LPVOID)&addArgs, 0, NULL);	// вызов AddThread()
	WaitForSingleObject(addThread, INFINITE);
	HANDLE saveThread = CreateThread(NULL, 0, &SaveThread, (LPVOID)&saveArgs, 0, NULL);	// вызов SaveThread()
	HANDLE* threads = new HANDLE[2]{ addThread, saveThread };

	WaitForMultipleObjects(2, threads, TRUE, INFINITE);
}

void HashTable::Save(string fileName)
{
	DeleteFile(wstring(fileName.begin(), fileName.end()).c_str());

	HANDLE hFile = CreateFile(
		wstring(fileName.begin(), fileName.end()).c_str(),	// имя файла, преобразуемое к типу char*
		GENERIC_WRITE,										// доступ для чтения и записи
		0,													// файл не может быть разделяемым
		NULL,												// дескриптор файла не наследуется
		CREATE_NEW,											// создать новый, если не существует
		FILE_ATTRIBUTE_NORMAL,								// файл не имеет особых атрибутов
		NULL												// всегда NULL для Windows
	);
	if (hFile != INVALID_HANDLE_VALUE)			// если файл создан
	{
		DWORD temp;

		WriteFile(
			hFile,								// дескриптор файла
			GetSizeOfTableInBytes().c_str(),	// буфер данных (строка конвертированная в массив символов)
			GetSizeOfTableInBytes().length(),	// число байтов для записи
			&temp,								// число записанных байтов
			NULL);								// асинхронный буфер (отсутствует)

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
	else	// файл не создан
	{
		CloseHandle(hFile);
		throw "ошибка: файл не создан";
	}
}

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

							/* функция memmove копирует n байт из массива (области памяти),
							на который указывает аргумент source,
							в массив (область памяти),
							на который указывает аргумент destination. */
	memmove(
		bytes,				// указатель на массив в который будут скопированы данные.
		&sizeOfTable,		// указатель на массив источник копируемых данных.
		sizeof(int));		// количество байт для копирования
	for (int i = 0; i < 4; i++)
		result += *(bytes + i);
	return result;
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
		// считываем из файла в
		if (ReadFile(
			hFile,
			&arrSize,
			sizeof(int),
			&temp,
			NULL) == 0)
		{
			throw "ошибка при чтении файла";
		};

		hashArr = new Node[arrSize];
		sizeOfTable = arrSize;

		while (temp != 0)	// пока считывание происходит
		{
			int key = 0;
			if (ReadFile(
				hFile,
				&key,
				sizeof(int),
				&temp,
				NULL) == 0)
			{
				throw "При чтении файла возникла ошибка! Ключ";
			};

			int length = 0;
			if (ReadFile(
				hFile,
				&length,
				sizeof(int),
				&temp,
				NULL) == 0)
			{
				throw "При чтении файла возникла ошибка! Длина";
			};

			char* valueChars = new char[length] {0};
			if (ReadFile(
				hFile,
				valueChars,
				sizeof(byte) * length,
				&temp,
				NULL) == 0)
			{
				throw "При чтении файла возникла ошибка! Строка";
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
		throw "ошибка: файл невозможно открыть";
		return;
	}
}

HashTable* HashTable::NewHashTable(int size)
{
	return new HashTable(size);
}
