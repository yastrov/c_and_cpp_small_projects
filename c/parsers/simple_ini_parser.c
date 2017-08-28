/*
 * Simple Configuration (Settings) INI file parser / reader.
 * 
Скомпилировать с помощью GCC (или MinGW):
gcc -std=c99 simple_ini_parser.c -o simple_ini_parser

Для Microsoft Visual Studio Community Edition
запускать сочетанием клавиш: CTRL+F5


Simple INI file test.ini:

[Group1]
key1=value 1
key2=value2

[Group2]
key1=value 1
key2=value2
 
Author: Yuriy Astrov
License: MIT License
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h> /* For ptrdiff_t */

#define MAX_STR_LEN 140

int find_char(const char *s, char ch)
{
	if(s == NULL) return -1;
    int index = 0;
    while(*s != '\0') {
		if(*s == ch)
			return index;
		++s;
		++index;
	}
    return -1;
}

int find_charn(const char *s, char ch, int max)
{
	if(s == NULL) return -1;
    int index = 0;
    while(*s != '\0' && index < max) {
		if(*s == ch)
			return index;
		++s;
		++index;
	}
    return -1;
}

void parse_and_print(FILE *fp)
{
	if(fp == NULL) return;
	rewind(fp);
	char group[MAX_STR_LEN];
	char key[MAX_STR_LEN];
	char value[MAX_STR_LEN];
	char buffer[MAX_STR_LEN];
	char *result = NULL;
	int index = -1;

	/* While not EOF (пока не конец файла). */
	while (!feof(fp)) {
		/* Очищаем buffer, мало ли что там лежало. */
		memset(buffer, 0, MAX_STR_LEN*sizeof(char));
		/* Читаем из файла строку. */
		result = fgets(buffer, MAX_STR_LEN, fp);
		if(result == NULL)
			break;
		/* Пытаемся понять, группа ли это [Group1],
		 * тогда должно начинаться на '[' */
		if(buffer[0] == '[') {
			memset(group, 0, MAX_STR_LEN*sizeof(char));
			index = find_charn(buffer, ']', MAX_STR_LEN);
			if(index != -1) {
				/* Копируем название группы из buffer в group. */
				strncpy(group, buffer+1, index-1);
				printf("GROUP: %s\n", group);
			}
			/* Если да, можно переходить к следующей строке файла. 
			 * Дальнейшие проверки - пустая трата времени. */
			continue;
		}
		/* Если попали сюда, значит явно не группа,
		 * может это пара ключ (название переменной)-значение?
		 * Тогда должен быть символ '=' */
		index = find_charn(buffer, '=', MAX_STR_LEN);
		if(index != -1) {
			memset(key, 0, MAX_STR_LEN*sizeof(char));
			memset(value, 0, MAX_STR_LEN*sizeof(char));
			/* Копируем название ключа из buffer в group. */
			strncpy(key, buffer, index);
			/* Копируем значение из buffer в group. */
			strncpy(value, buffer+index+1, MAX_STR_LEN);
			printf("KEY: %s\n", key);
			printf("VALUE: %s\n", value);
		}
		
	}
}

char *read_value_str(FILE *fp, const char *group_name, const char *key_name, char *data)
{
	if(fp == NULL || group_name == NULL || key_name == NULL || data == NULL) return NULL;
	rewind(fp);
	char group[MAX_STR_LEN];
	char key[MAX_STR_LEN];
	char value[MAX_STR_LEN];
	char buffer[MAX_STR_LEN];
	char *result = NULL;
	int index = -1;
	char group_found_flag = 0;

	while (!feof(fp)) {
		memset(buffer, 0, MAX_STR_LEN*sizeof(char));
		result = fgets(buffer, MAX_STR_LEN, fp);
		if(result == NULL)
			break;
		if(buffer[0] == '[') {
			memset(group, 0, MAX_STR_LEN*sizeof(char));
			index = find_charn(buffer, ']', MAX_STR_LEN);
			if(index != -1) {
				strncpy(group, buffer+1, index-1);
				if(strcmp(group, group_name)==0) {
					group_found_flag = 1;
				} else {
					group_found_flag = 0;
				}
			}
			continue;
		}
		index = find_charn(buffer, '=', MAX_STR_LEN);
		if(index != -1) {
			memset(key, 0, MAX_STR_LEN*sizeof(char));
			memset(value, 0, MAX_STR_LEN*sizeof(char));
			strncpy(key, buffer, index);
			strncpy(value, buffer+index+1, MAX_STR_LEN);
			if(group_found_flag == 1 && strcmp(key, key_name)==0) {
				strncpy(data, value, MAX_STR_LEN);
				return data;
			}
		}
	}
	return NULL;
}

int main(int argc, char **argv)
{
	const char *filename = "test.ini";
	FILE *fp = fopen(filename, "r");
	if(!fp) {
        /* Короткая форма сообщения об ошибке в stderr. */
        perror(filename);
        return EXIT_FAILURE; /* Выход */
    }
    
    parse_and_print(fp);
    char buffer[MAX_STR_LEN];
    char *ptr = read_value_str(fp, "Group1", "key2", buffer); 
    if(ptr != NULL) {
		printf("Find value: %s\n", buffer);
	} else {
		printf("Can't find!\n");
	}
    fclose(fp);
	return 0;
}
