#include "stdafx.h"
#include <stdio.h>
#include <locale.h>  
#include <ctime>		// содержится time()
#include <cmath>		// содержится pow() – возведение в степень
#include <windows.h>

#define MIN			1	// нижняя граница для ГПСЧ
#define MAX			100	// верхняя
#define PRECISION	2	// точность

void main()
{
	setlocale(LC_ALL, "RUS");

	puts("Задание:");
	puts("По динамическому массиву из \"n\" вещественных чисел необходимо рассчитать:");
	puts("1) выборочное среднее,");
	puts("2) выборочную дисперсию.");
	puts("");

	printf("введите размер массива ... ");
	int n;
	scanf_s("%d", &n);
	puts("");

	printf("массив размера %i случайных вещественных чисел со значениями элементов в отрезке [%i, %i]:\n"
		, n, MIN, MAX);
	puts("");

	srand(time(NULL));	/* генерация случайных чисел: значение, возвращённое функцией time отличается каждую секунду,
						что даёт возможность получать совершенно случайные последовательности чисел,
						при каждом новом вызове функции rand;
						time(NULL) -- возвращает текущую дату */
	const int range = MAX - MIN + 1;				// диапазон; переменная нужна, чтобы не делать постоянно далее лишних вычислений
	const double precision = pow(0.1, PRECISION);	// диапазон; переменная нужна, чтобы не делать постоянно далее лишних вычислений

	double* arr1, * arr2;
	double sampleMean1 = 0, sampleMean2 = 0;			// сумма для выборочного среднего
	double sampleVariance1 = 0, sampleVariance2 = 0;	// сумма для выборочной дисперсии

	arr1 = (double*)malloc(n * sizeof(double));	// выделение памяти для массива вещественных чисел

	HANDLE hHeap;						// указатель на кучу
	hHeap = HeapCreate(0, 0x1000, 0);	// создание кучи
	if (hHeap != NULL)					// если создана удачно
	{
		arr2 = (double*)HeapAlloc(hHeap, 0, n * sizeof(double));
		puts("выделение памяти с помощью HeapAlloc:");
		puts("-------------------------------------");

		for (int i = 0; i < n; i++) {	// заполнение массива
			arr1[i] = (double)(rand() % (range)+MIN) + (double)(rand() % (range)+MIN) * precision;
			arr2[i] = (double)(rand() % (range)+MIN) + (double)(rand() % (range)+MIN) * precision;

			sampleMean1 += arr1[i];
			sampleVariance1 += pow(arr1[i], 2);

			sampleMean2 += arr2[i];
			sampleVariance2 += pow(arr2[i], 2);

			printf("%.2f\t", arr2[i]);
		}
		puts("-------------------------------------");
	}
	else
	{
		puts("ошибка cоздания кучи");
		puts("");
		system("PAUSE");
		return;
	}

	sampleMean1 = 1 / (double)n * sampleMean1;									// вычисление выборочного среднего
	sampleVariance1 = 1 / (double)n * sampleVariance1 - pow(sampleMean1, 2);	// вычисление выборочной дисперсии
	sampleMean2 = 1 / (double)n * sampleMean2;									// вычисление выборочного среднего
	sampleVariance2 = 1 / (double)n * sampleVariance2 - pow(sampleMean2, 2);	// вычисление выборочной дисперсии

	printf("выборочное среднее   = %.2f\n", sampleMean2);
	printf("выборочная дисперсия = %.2f\n", sampleVariance2);
	puts("");

	puts("выделение памяти с помощью malloc:");
	puts("-------------------------------------");
	for (int i = 0; i < n; i++) 	// заполнение массива
		printf("%.2f\t", arr1[i]);
	puts("-------------------------------------");

	printf("выборочное среднее   = %.2f\n", sampleMean1);
	printf("выборочная дисперсия = %.2f\n", sampleVariance1);
	puts("");

	HeapFree(hHeap, 0, arr2);		// освобождение памяти
	if (HeapDestroy(hHeap) == 0)
		puts("ошибка уничтожения кучи");
	else
		puts("куча разрушена");
	free(arr1);						// работа с массивом окончена: освобождаем память

	puts("");
	system("PAUSE");
}
