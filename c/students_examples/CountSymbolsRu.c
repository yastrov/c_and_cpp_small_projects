/*
Кодировка файла Windows-1251, окончание строк: CRLF.

Задача: дана строка, содержащая маленькие латинские буквы (вводится пользователем с клавиатуры).
Требуется посчитать количество повторений каждой буквы.

Поскольку могут быть только маленькие латинские буквы, значит может быть всего лишь 26 букв.
Причём код  символа 'a' - 0x61, а каждый следующий больше предыдущего на 1. (См. таблицу ASCII.)
Значит мы можем использовать для хранения количества повторений использовать массив
(можно int - для короткой строки, можно size_t),
в котором индекс элемента - это код символа минус 0x61,
а сам элемент - количество повторений символа с таким кодом.
C учётом символа конца строки в языке Си ('\0'), длина массива будет 27.

Скомпилировать с помощью GCC (или MinGW):
gcc -std=c99 CountSymbolsRu.c -o CountSymbols

Для Microsoft Visual Studio Community Edition
запускать сочетанием клавиш: CTRL+F5

Author: Yuriy Astrov
License: MIT License
*/
#include <stdio.h>
#include <stdlib.h>

/* Объявляем константу с помощью препроцессора.
Использовать
static const size_t CHAR_IN_LATIN = 27;
было бы лучше, но одно но: тогда объявить 
int array[CHAR_IN_LATIN] = {0};
в main уже не получится:(. */
#define CHAR_IN_LATIN 27
#define MAXIMUM_LENGTH 257

/* Функция, которая подсчитывает количество конкретных символов в строке str
и записывает эту информацию в массив table.
Индекс в table - код символа минус 0x61 (код символа 'a'),
значение - количество встретившихся (этих, с таким кодом) символов.
const - потому что не будем изменять.*/
void str_to_table(int table[], const char str[], size_t max_str_len) {
    /* Подсчитаем количество повторений для каждого символа в строке. */
    for(size_t i=0; i < max_str_len && str[i] != '\0'; ++i) {
        /* Превратим код символа в индекс массива:*/
        size_t new_index = str[i] - 0x61;
        /* То же самое:
        size_t new_index = str[i] - 'a'; */
        /* Увеличим значение в массиве на 1 */
        table[new_index] = table[new_index] + 1;
    }
}

/* Функция, которая выводит информацию о встреченных символах из table на экран. */
void print_table(const int table[], size_t table_size) {
    /* Распечатаем таблицу результатов:*/
    for(size_t i=0; i < table_size; ++i) {
        /* Восстановим код символа */
        char chr = i + 0x61;
        /* То же самое:
        char chr = i + 'a'; */
        int count = table[i];
        /* Пропустим и не будем печатать, если символ так и не встретился. */
        if(count < 1) continue;
        /* одиночный символ, потом - количество повторений */
        printf("%c - %d\n", chr, count);
    }
}

int main()
{
    int array[CHAR_IN_LATIN] = {0}; /* {0} - присвоение каждой ячейке массива 0 (нуля). С99. */
    /* Для простоты предположим, что MAXIMUM_LENGTH - максимальная длина введённой строки. */
    char input[MAXIMUM_LENGTH] = {0};
    printf("Enter string without spaces (maximum length %d symbols):\n", MAXIMUM_LENGTH);
    /*К сожалению, с помощью формата %s можно считать строку символов без пробелов.*/
    scanf("%s", input);
    str_to_table(array, input, MAXIMUM_LENGTH);
    printf("Result (char and count number):\n");
    print_table(array, CHAR_IN_LATIN);
    return 0;
}
