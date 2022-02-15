#include "stdafx.h"
#include "HashTable.h"
#include <clocale>
#include <iostream>
#include <exception>
#include <stdio.h> 
using namespace std;

// �����:
/*
������� ������� GetProcAddress, ����� �������� ��������� ��� ������ �������������� �������, ���������� �����������. 
��������� ���������� �������� ������� ��������� DLL � ������� ���������, ���������� �� ������� ������� ������, 
������� ��� ������������� ��������� ���������� � ����������� �������.
*/
HashTable* Expl(int size)
{
	HINSTANCE hinstLib;
	hinstLib = LoadLibrary(TEXT("../Debug/DLL.dll"));					// ����� ��������� DLL

	/*
	����� ���� ��� ���������� ���������, 
	����� ����� �� ������������ � ��� ������� ����� �������� � ������� GetProcAddress, 
	������� ���������� �������� ���������� ���������� � ��� �������. 
	����� ������� �� DLL ����� ��������, ��� ������. 
	*/
	void *(_stdcall *vHashTable)(int);
	(FARPROC &)vHashTable = GetProcAddress(hinstLib, "NewHashTable");	// ����� �������� ��������� �� ��������� ������� (��� ����������)
	HashTable* hashTable = (HashTable*)vHashTable(size);

	FreeLibrary(hinstLib);												// ����� ��������� ���������� � ���������� ���������� �� �������
	return hashTable;
}

/*
��� ������� ����������� (implicit linking) ������� ���������� ���������� ������� (������ ����� ���������� lib), 
���������� ������ ���������� � ������� DLL, ������� ����� ������������ ����������. 
���������, ��� ��������� ���������� ���� �� � ����� �� ���, ������ ��������� � ������� exe-���� ������� �������.

�����, ����� exe-���� ����� �������, ��������� ���������� ��� DLL, ������������� � ������� �������, 
�� �������� ������������ ��������; � ������ ������� ���� ������� ���������� �����������.
*/
HashTable* Impl(int size)
{
	HashTable *hashTable;
	hashTable = new HashTable(size);

	return hashTable;
}

void ShowContentOfHashTable(HashTable* hashTable)
{
Node* buffer;
	cout << "��� ����� ���-�������: " << endl;
	for (int i = 0; i < hashTable->Length(); i++)
	{
		buffer = hashTable->GetArray()[i].GetNext();
		cout << i << " : ";
		while (buffer != NULL)
		{
			cout << buffer->GetKey();
			buffer = buffer->GetNext();
		}
		cout << endl;
	}
}

int main()
{
	setlocale(LC_ALL, "Russian");

	cout << "������������ ������ �7.\n"
		<< "��������: �������� �. �., ������ 6213.\n" << endl;

	setlocale(LC_ALL, "Russian");

	HashTable* hashTable = NULL;
	try
	{
		cout << "��� ����������:\n"
			<< "1. �����\n"
			<< "2. �������\n";
		cout << "��� ����� >>> ";
		int v = 0;
		string str;
		cin >> str;
		v = atoi(str.c_str());
		switch (v)
		{
		case 1:
		{
			while (true)
			{
				try
				{
					cout << "������� ������ ���-������� >>> ";
					int size = 0;
					string str;
					cin >> str;
					size = atoi(str.c_str());
					if (size != 0)
						hashTable = Expl(size);
					else { throw "������: ������ ������� �� ����� ���� �������!"; }
					break;
				}
				catch (char* msg)
				{
					cout << msg << endl;
				}
			}
			cout << "���-������� ������� �������" << endl;
			break;
		}
		case 2:
		{
			while (true)
			{
				try
				{
					cout << "������� ������ ���-������� >>> ";
					int size = 0;
					string str;
					cin >> str;
					size = atoi(str.c_str());
					if (size != 0)
						hashTable = Impl(size);
					else { throw "������: ������ ������� �� ����� ���� �������"; }
					break;
				}
				catch (char* msg)
				{
					cout << msg << endl;
				}
			}
			cout << "���-������� ������� �������" << endl;
			break;
		}
		default:
		{
			throw "���������� ��������� ����� ����";
		}
		}
	}
	catch (char* msg)
	{
		cout << msg << endl;
	}

	/*
	������� � ������ ����, ������������ ��� �������� ������������� ������� ������� ������ ��� ���������� ���������� � ������������ �������. 
	*/
	
	string buf;
	string value;
	int key = 0;
	while (true)
	{
		cout << "\n1. �������� �������\n"
			<< "2. �������� �������\n"
			<< "3. ������� �������\n"
			<< "4. ������� �������� ��������\n"
			<< "5. ������� ���-�������\n"
			<< "6. �������� � ����\n"
			<< "7. ��������� �� �����\n";
		cout << "��� ����� >>> ";
		int choice = 0;
		cin >> choice;
		cout << endl;
		try {
			switch (choice) {
			case 1:
			{
				cout << "������� ����, �� �������� ������ �������� �������� � ���-������� >>> ";
				cin >> buf;
				key = atoi(buf.c_str());

				if (hashTable->IsExists(key))
					throw "������: ������� � ��������� ������ ��� ���� � �������";
				cout << "������� �������, ������� ������ �������� � ������� >>> ";
				cin >> value;
				hashTable->Add(value, key);
				cout << "������� ������� �������� " << "(" << hashTable->GetValueByKey(key) << "," << key << ")" << endl;
				ShowContentOfHashTable(hashTable);
				break;
			}
			case 2:
			{
				cout << "������� ���� >>> ";
				cin >> buf;
				key = atoi(buf.c_str());
				if (!hashTable->IsExists(key))
					throw  "������: ������� � ��������� ������ ����������� � �������";
				cout << "(" << hashTable->GetValueByKey(key) << "," << key << ")" << endl;

				cout << "������� ����� �������� >>> ";
				cin >> value;

				hashTable->Edit(key, value);
				cout << "������� ������� ������ " << "(" << hashTable->GetValueByKey(key) << "," << key << ")" << endl;
				break;
			}
			case 3:
			{
				cout << "������� ���� >>> ";
				cin >> buf;
				key = atoi(buf.c_str());
				value = hashTable->GetValueByKey(key);
				if (hashTable->DeleteByKey(key))
				{
					cout << "������� ������� ����� " << "(" << value << "," << key << ")" << endl;
				}
				else {
					throw "������: ������� � �������� ������ ����������� � �������";
				}
				break;
			}
			case 4:
			{
				cout << "������� ���� >>> ";
				cin >> buf;
				key = atoi(buf.c_str());
				if (!hashTable->IsExists(key))
					throw  "������: ������� � ��������� ������ ����������� � �������";
				cout << "(" << hashTable->GetValueByKey(key) << "," << key << ")" << endl;

				break;
			}
			case 5:
			{
				ShowContentOfHashTable(hashTable);
				break;
			}
			case 6:
			{
				cout << "������� �������� �����: \n> ";
				cin >> buf;
				hashTable->Save(buf);
				cout << "���� � ������ \"" << buf << "\" ������� ������" << endl;
				break;
			}
			case 7:
			{
				cout << "������� �������� �����: \n> ";
				cin >> buf;
				hashTable->Load(buf);
				cout << "������� ������� ���������" << endl;
				break;
			}
			default:
			{
				cout << "������� ������������ �������� ������ ����" << endl;
				break;
			}
			}
		}
		catch (char* msg)
		{
			cout << msg << endl;
		}
	}

	system("pause");
	return 0;
}
