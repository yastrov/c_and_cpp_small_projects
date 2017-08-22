/*
Кодировка файла Windows-1251, окончание строк: CRLF.
Программа для чтения ID3 тегов версии 1,
которые часто встречаются в MP3 аудио файлах.
Подробнее о тегах:
https://ru.wikipedia.org/wiki/ID3_(метаданные).

Скомпилировать с помощью GCC (или MinGW):
gcc -std=c99 ID3v1TagReaderRu.c -o ID3v1TagReader

Для Microsoft Visual Studio Community Edition
запускать сочетанием клавиш: CTRL+F5

Author: Yuriy Astrov
License: MIT License
*/
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Структура, описывающая ID3 тег версии 1.
Размер всех строк увеличен на 1, поскольку
 В Си всегда последний символ строки должен быть '\0', или 0.*/
struct ID3v1Tag {
    char header[4];
    char title[31];
    char artist[31];
    char album[31];
    char year[5];
    char comment[31];
    char null_byte; /* "Флаг". Если 0 - значит есть номер трека. */
    //uint8_t null_byte;
    char track_number; /* Можно хранить 1 байт в char. */
    //uint8_t track_number;
    uint8_t genre; /* Можно хранить беззнаковое (всегда положительное) число размером в 1 байт в uint8_t. */
};

/* Чтобы каждый раз не писать что-то похожее на:
struct ID3v1Tag data;
Создадим псевдоним, и получится :
ID3v1TagData data;
Объявить псевдоним для структуры можно несколькими способами. */
typedef struct ID3v1Tag ID3v1TagData;
/* Или вместо всего выше можно было написать:
typedef struct {
    char header[4];
    char title[31];
    ...
} ID3v1TagData;
*/

/* Размер тега ID3v1 из документации в байтах. */
const int ID3v1Size = 128;

/* Функция для считывание тега из файла fp в data.
Если считывание удалось, возвращаем 0,
в случае неудачи (ошибки) возвращаем -1.*/
int readTagData(FILE *fp, ID3v1TagData *data)
{
    /* Очистим память, гда хранится структура. */
    memset(data, 0, sizeof(ID3v1TagData));
    /* Переместим курсор на -ID3v1Size (т.е. -128 байт) от конца (SEEK_END) файла. */
    const int result = fseek(fp, -ID3v1Size, SEEK_END);
    if(result != 0) {
        perror("Невозможно переместить курсор в конец файла!");
        return -1;
    }
    /* Прочитаем из файла заголовок, размером 3 байта и количеством 1 штука. */
    /* Если у нас есть указатель на структуру, то доступ к её полю (например header) будет через ->. */
    fread(data->header, 3, 1, fp);
    /* Проверим, что это действительно тег ID3v1,
    т.е. начинается с TAG, как и описано в документации. */
    if(strcmp(data->header, "TAG") != 0) {
        return -1;
    }
    /* Прочитаем из файла название, 30 байта и количеством 1 шт. */
    fread(data->title, 30, 1, fp);
    fread(data->artist, 30, 1, fp);
    fread(data->album, 30, 1, fp);
    fread(data->year, 4, 1, fp);
    /* Комментарий */
    fread(data->comment, 30, 1, fp);
    /* Проверим, "нулевой" байт, который показывает, указан ли номер трека.
    Он - предпоследний байт в комментарии, т.е. 28.
    Последний байт будет с индексом 29. (Всегда на 1 меньше размера массива.)*/
    data->null_byte = data->comment[28];
    if(data->null_byte == 0) {
        data->track_number = data->comment[29];
    }
    /* Жанр. Его загружаем просто в переменную,
    а не массивы, как раньше, значит нужно передать её адрес с помощью &: */
    fread(&data->genre, 1, 1, fp);
    return 0;
}

/* Функция для вывода информации из data на экран.
data не изменяется, потому const. */
void printTagData(const ID3v1TagData *data)
{
    /* Разделителем будет знак табуляции \t.
    %s - параметр формата для строки (массива char) */
    printf("Название:\t%s\n", data->title);
    printf("Исполнитель:\t%s\n", data->artist);
    printf("Альбом:\t%s\n", data->album);
    printf("Год:\t%s\n", data->year);
    printf("Комментарий:\t%s\n", data->comment);
    printf("Значение нулевого байта:\t%x\n", data->null_byte);
    if(data->null_byte == 0) {
        printf("Номер трека:\t%d", data->track_number);
        /* В скобках выведем значение в hex виде. */
        const unsigned char ch = data->track_number;
        printf(" (в hex: ox%x)\n", ch);
    }
    printf("Жанр:\t%d\n", data->genre);
}

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");
    printf("ОТЛАДОЧНОЕ: Размер char в байтах: %d\n", sizeof(char));
    /* Если параметров запуска меньше 2, значит имя файла нам точно не передали.
    Первый параметр - всегда имя программы. */
    if(argc < 2) {
        /* Вывод ошибки в stderr (файл, куда принято записывать информацию о ошибках в программе). */
        fprintf(stderr, "Ошибка, требуется файл!\n");
        printf("Использование:\n\n");
        printf("%s FILE.MP3\n\n", argv[0]);
        return EXIT_FAILURE; /* Выход */
    }
    /* Для наглядности создадим указатель на параметр запуска, содержащий имя файла.
    Изменять не будем, значит const. */
    const char *filename = argv[1];
    /* Для отладки можно было бы: const char *filename = "C:\\1.mp3"; */
    printf("Идёт открытие файла.\n\n");
    /* Откроем файл. */
    FILE* fp = fopen(filename, "rb");
    /* fp - указатель на память, где размещается FILE,
    "r" - открываем файл на чтение (read).
    То же самое по сути:
    FILE* fp = fopen(argv[1], "r"); */
    if(!fp) {
        /* Короткая форма сообщения об ошибке в stderr. */
        perror(filename);
        perror("Невозможно открыть файл!");
        return EXIT_FAILURE; /* Выход */
    }
    /* Объявим структуру, в которой будет храниться информация из тега. */
    ID3v1TagData data;
    /* Вызовем нашу функцию для считывания информации о теге из файла. */
    const int tagReadResult = readTagData(fp, &data);
    if(tagReadResult != 0) {
        printf("Ошибка: В файле нет ID3v1 тега!\n");
        fclose(fp);
        return EXIT_FAILURE; /* Выход */
    }
    /* Закрываем файл. */
    fclose(fp);
    /* Вывод информации на экран. */
    printTagData(&data);
    return 0;
}
