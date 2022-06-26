#include "stdafx.h"
#include "HashTable.h"
#include <clocale>
#include <iostream>
#include <exception>
using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");

	cout << "Лабораторная работа №4.\n"
		<< "Выполнил: Кузнецов А. П., группа 6213.\n" << endl;

	HashTable* hashTable;

	while (true)
	{
		try
		{
			cout << "введите размер хэш-таблицы >>> ";
			int size = 0;
			string str;
			cin >> str;
			size = atoi(str.c_str());
			if (size > 0)
				hashTable = new HashTable(size);
			else { throw "ошибка: размер таблицы должен быть больше нуля"; }
			break;
		}
		catch (char* msg)
		{
			cout << msg << endl;
		}
	}
	cout << "хэш-таблица успешно создана" << endl;

	string buf;
	string value;
	int key = 0;
	while (true)
	{
		cout << "\n1. добавить элемент\n"
			<< "2. изменить элемент\n"
			<< "3. удалить элемент\n"
			<< "4. вывести значение элемента\n"
			<< "5. вывести хэш-таблицу\n"
			<< "6. записать в файл\n"
			<< "7. загрузить из файла\n";
		cout << "ваш выбор >>> ";
		int choice = 0;
		cin >> choice;
		cout << endl;
		try {
			switch (choice)
			{
			case 1:
			{
				cout << "введите ключ, по которому хотите вставить значение в хэш-таблицу >>> ";
				cin >> buf;
				key = atoi(buf.c_str());

				if (hashTable->IsExists(key))
					throw "ошибка: элемент с введенным ключём уже есть в таблице";
				cout << "введите элемент, который хотите вставить в таблицу >>> ";
				cin >> value;
				hashTable->Add(value, key);
				cout << "элемент успешно добавлен " << "(" << hashTable->GetValueByKey(key) << "," << key << ")" << endl;
				break;
			}
			case 2:
			{
				cout << "введите ключ >>> ";
				cin >> buf;
				key = atoi(buf.c_str());
				if (!hashTable->IsExists(key))
					throw  "ошибка: элемент с введенным ключём отсутствует в таблице";
				cout << "(" << hashTable->GetValueByKey(key) << "," << key << ")" << endl;

				cout << "введите новое значение >>> ";
				cin >> value;

				hashTable->Edit(key, value);
				cout << "элемент успешно изменён " << "(" << hashTable->GetValueByKey(key) << "," << key << ")" << endl;
				break;
			}
			case 3:
			{
				cout << "введите ключ >>> ";
				cin >> buf;
				key = atoi(buf.c_str());
				value = hashTable->GetValueByKey(key);
				if (hashTable->DeleteByKey(key))
				{
					cout << "элемент успешно удалён " << "(" << value << "," << key << ")" << endl;
				}
				else {
					throw "ошибка: элемент с введённым ключём отсутствует в таблице";
				}
				break;
			}
			case 4:
			{
				cout << "введите ключ >>> ";
				cin >> buf;
				key = atoi(buf.c_str());
				if (!hashTable->IsExists(key))
					throw  "ошибка: элемент с введенным ключем отсутствует в таблице";
				cout << "(" << hashTable->GetValueByKey(key) << "," << key << ")" << endl;

				break;
			}
			case 5:
			{
				Node* buffer;
				cout << "все ключи хэш-таблицы: " << endl;
				for (int i = 0; i < hashTable->Length(); i++)
				{
					buffer = hashTable->GetArray()[i].GetNext();
					cout << i << " : ";
					while (buffer != NULL)
					{
						cout << "" << buffer->GetKey() << "";
						buffer = buffer->GetNext();
					}
					cout << endl;
				}
				break;
			}
			case 6:
			{
				cout << "введите название файла: \n> ";
				cin >> buf;
				hashTable->Save(buf);
				cout << "файл с именем \"" << buf << "\" успешно создан" << endl;
				break;
			}
			case 7:
			{
				cout << "введите название файла: \n> ";
				cin >> buf;
				hashTable->Load(buf);
				cout << "таблица успешно загружена" << endl;
				break;
			}
			default:
			{
				cout << "введено некорректное значение пункта меню" << endl;
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
