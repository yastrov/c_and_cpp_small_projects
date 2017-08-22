/*
��������� ����� Windows-1251, ��������� �����: CRLF.

������: ���� ������, ���������� ��������� ��������� ����� (�������� ������������� � ����������).
��������� ��������� ���������� ���������� ������ �����.

��������� ����� ���� ������ ��������� ��������� �����, ������ ����� ���� ����� ���� 26 ����.
������ ���  ������� 'a' - 0x61, � ������ ��������� ������ ����������� �� 1. (��. ������� ASCII.)
������ �� ����� ������������ ��� �������� ���������� ���������� ������������ ������
(����� int - ��� �������� ������, ����� size_t),
� ������� ������ �������� - ��� ��� ������� ����� 0x61,
� ��� ������� - ���������� ���������� ������� � ����� �����.
C ������ ������� ����� ������ � ����� �� ('\0'), ����� ������� ����� 27.

�������������� � ������� GCC (��� MinGW):
gcc -std=c99 CountSymbolsRu.c -o CountSymbols

��� Microsoft Visual Studio Community Edition
��������� ���������� ������: CTRL+F5

Author: Yuriy Astrov
License: MIT License
*/
#include <stdio.h>
#include <stdlib.h>

/* ��������� ��������� � ������� �������������.
������������
static const size_t CHAR_IN_LATIN = 27;
���� �� �����, �� ���� ��: ����� �������� 
int array[CHAR_IN_LATIN] = {0};
� main ��� �� ���������:(. */
#define CHAR_IN_LATIN 27
#define MAXIMUM_LENGTH 257

/* �������, ������� ������������ ���������� ���������� �������� � ������ str
� ���������� ��� ���������� � ������ table.
������ � table - ��� ������� ����� 0x61 (��� ������� 'a'),
�������� - ���������� ������������� (����, � ����� �����) ��������.
const - ������ ��� �� ����� ��������.*/
void str_to_table(int table[], const char str[], size_t max_str_len) {
    /* ���������� ���������� ���������� ��� ������� ������� � ������. */
    for(size_t i=0; i < max_str_len && str[i] != '\0'; ++i) {
        /* ��������� ��� ������� � ������ �������:*/
        size_t new_index = str[i] - 0x61;
        /* �� �� �����:
        size_t new_index = str[i] - 'a'; */
        /* �������� �������� � ������� �� 1 */
        table[new_index] = table[new_index] + 1;
    }
}

/* �������, ������� ������� ���������� � ����������� �������� �� table �� �����. */
void print_table(const int table[], size_t table_size) {
    /* ����������� ������� �����������:*/
    for(size_t i=0; i < table_size; ++i) {
        /* ����������� ��� ������� */
        char chr = i + 0x61;
        /* �� �� �����:
        char chr = i + 'a'; */
        int count = table[i];
        /* ��������� � �� ����� ��������, ���� ������ ��� � �� ����������. */
        if(count < 1) continue;
        /* ��������� ������, ����� - ���������� ���������� */
        printf("%c - %d\n", chr, count);
    }
}

int main()
{
    int array[CHAR_IN_LATIN] = {0}; /* {0} - ���������� ������ ������ ������� 0 (����). �99. */
    /* ��� �������� �����������, ��� MAXIMUM_LENGTH - ������������ ����� �������� ������. */
    char input[MAXIMUM_LENGTH] = {0};
    printf("Enter string without spaces (maximum length %d symbols):\n", MAXIMUM_LENGTH);
    /*� ���������, � ������� ������� %s ����� ������� ������ �������� ��� ��������.*/
    scanf("%s", input);
    str_to_table(array, input, MAXIMUM_LENGTH);
    printf("Result (char and count number):\n");
    print_table(array, CHAR_IN_LATIN);
    return 0;
}
