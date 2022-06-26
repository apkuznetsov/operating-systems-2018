#include "stdafx.h"
#include "HashTable.h"
#include <clocale>
#include <iostream>
#include <exception>
#include <stdio.h> 
using namespace std;

// явное:
/*
Вызвать функцию GetProcAddress, чтобы получить указатель для каждой экспортируемой функции, вызываемой приложением. 
Поскольку приложения вызывают функции библиотек DLL с помощью указателя, компилятор не создает внешних ссылок, 
поэтому нет необходимости выполнять связывание с библиотекой импорта.
*/
HashTable* Expl(int size)
{
	HINSTANCE hinstLib;
	hinstLib = LoadLibrary(TEXT("../Debug/DLL.dll"));					// чтобы загрузить DLL

	/*
	После того как библиотека загружена, 
	адрес любой из содержащихся в ней функций можно получить с помощью GetProcAddress, 
	которой необходимо передать дескриптор библиотеки и имя функции. 
	Затем функцию из DLL можно вызывать, как обычно. 
	*/
	void *(_stdcall *vHashTable)(int);
	(FARPROC &)vHashTable = GetProcAddress(hinstLib, "NewHashTable");	// чтобы получить указатели на требуемые функции (или переменные)
	HashTable* hashTable = (HashTable*)vHashTable(size);

	FreeLibrary(hinstLib);												// чтобы выгрузить библиотеку и освободить занимаемые ею ресурсы
	return hashTable;
}

/*
При неявном подключении (implicit linking) линкеру передается библиотека импорта (обычно имеет расширение lib), 
содержащая список переменных и функций DLL, которые могут использовать приложения. 
Обнаружив, что программа обращается хотя бы к одной из них, линкер добавляет в целевой exe-файл таблицу импорта.

Позже, когда exe-файл будет запущен, загрузчик проецирует все DLL, перечисленные в таблице импорта, 
на адресное пространство процесса; в случае неудачи весь процесс немедленно завершается.
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
	cout << "все ключи хэш-таблицы: " << endl;
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

	cout << "Лабораторная работа №7.\n"
		<< "Выполнил: Кузнецов А. П., группа 6213.\n" << endl;

	setlocale(LC_ALL, "Russian");

	HashTable* hashTable = NULL;
	try
	{
		cout << "тип связывания:\n"
			<< "1. явный\n"
			<< "2. неявный\n";
		cout << "ваш выбор >>> ";
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
					cout << "задайте размер хэш-таблицы >>> ";
					int size = 0;
					string str;
					cin >> str;
					size = atoi(str.c_str());
					if (size != 0)
						hashTable = Expl(size);
					else { throw "ошибка: размер таблицы не может быть нулевым!"; }
					break;
				}
				catch (char* msg)
				{
					cout << msg << endl;
				}
			}
			cout << "хэш-таблица успешно создана" << endl;
			break;
		}
		case 2:
		{
			while (true)
			{
				try
				{
					cout << "задайте размер хэш-таблицы >>> ";
					int size = 0;
					string str;
					cin >> str;
					size = atoi(str.c_str());
					if (size != 0)
						hashTable = Impl(size);
					else { throw "ошибка: размер таблицы не может быть нулевым"; }
					break;
				}
				catch (char* msg)
				{
					cout << msg << endl;
				}
			}
			cout << "хэш-таблица успешно создана" << endl;
			break;
		}
		default:
		{
			throw "Отсутсвует выбранный пункт меню";
		}
		}
	}
	catch (char* msg)
	{
		cout << msg << endl;
	}

	/*
	Мьютекс – объект ядра, используемый как средство синхронизации доступа потоков одного или нескольких приложений к критическому участку. 
	*/
	
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
			switch (choice) {
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
				ShowContentOfHashTable(hashTable);
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
				ShowContentOfHashTable(hashTable);
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
