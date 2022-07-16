/* Число простое, если оно имеет два различных делителя: единицу и саму себя.
 * Число, имеющие большее число делителей, называются составными.
 * Таким образом, если мы умеем раскладывать числа на множители, то мы умеем проверять числа на простоту. */

#include <clocale>
#include <windows.h>

bool is_prime(int);

int main() {
    setlocale(LC_ALL, "RUS");

    puts("Напечатать все простые числа, не превосходящие заданное число M.");
    puts("");

    printf("введите натуральное число, M = ");
    int m;
    scanf("%d", &m);
    puts("");

    printf("список всех простых чисел,\nне превосходящие число M = %d:\n", m);
    for (int i = 1; i <= m; ++i) {
        if (is_prime(i))
            printf("%d\n", i);
    }

    puts("");
    system("PAUSE");

    return 1;
}

bool is_prime(int number) {
    if (number == 1)
        return false;

    for (int d = 2; d * d <= number; d++) {
        if (number % d == 0)
            return false;
    }

    return true;
}
