/*
��������� ����� Windows-1251, ��������� �����: CRLF.
��������� ��� ������ ID3 ����� ������ 1,
������� ����� ����������� � MP3 ����� ������.
��������� � �����:
https://ru.wikipedia.org/wiki/ID3_(����������).

�������������� � ������� GCC (��� MinGW):
gcc -std=c99 ID3v1TagReaderRu.c -o ID3v1TagReader

��� Microsoft Visual Studio Community Edition
��������� ���������� ������: CTRL+F5

Author: Yuriy Astrov
License: MIT License
*/
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* ���������, ����������� ID3 ��� ������ 1.
������ ���� ����� �������� �� 1, ���������
 � �� ������ ��������� ������ ������ ������ ���� '\0', ��� 0.*/
struct ID3v1Tag {
    char header[4];
    char title[31];
    char artist[31];
    char album[31];
    char year[5];
    char comment[31];
    char null_byte; /* "����". ���� 0 - ������ ���� ����� �����. */
    //uint8_t null_byte;
    char track_number; /* ����� ������� 1 ���� � char. */
    //uint8_t track_number;
    uint8_t genre; /* ����� ������� ����������� (������ �������������) ����� �������� � 1 ���� � uint8_t. */
};

/* ����� ������ ��� �� ������ ���-�� ������� ��:
struct ID3v1Tag data;
�������� ���������, � ��������� :
ID3v1TagData data;
�������� ��������� ��� ��������� ����� ����������� ���������. */
typedef struct ID3v1Tag ID3v1TagData;
/* ��� ������ ����� ���� ����� ���� ��������:
typedef struct {
    char header[4];
    char title[31];
    ...
} ID3v1TagData;
*/

/* ������ ���� ID3v1 �� ������������ � ������. */
const int ID3v1Size = 128;

/* ������� ��� ���������� ���� �� ����� fp � data.
���� ���������� �������, ���������� 0,
� ������ ������� (������) ���������� -1.*/
int readTagData(FILE *fp, ID3v1TagData *data)
{
    /* ������� ������, ��� �������� ���������. */
    memset(data, 0, sizeof(ID3v1TagData));
    /* ���������� ������ �� -ID3v1Size (�.�. -128 ����) �� ����� (SEEK_END) �����. */
    const int result = fseek(fp, -ID3v1Size, SEEK_END);
    if(result != 0) {
        perror("���������� ����������� ������ � ����� �����!");
        return -1;
    }
    /* ��������� �� ����� ���������, �������� 3 ����� � ����������� 1 �����. */
    /* ���� � ��� ���� ��������� �� ���������, �� ������ � � ���� (�������� header) ����� ����� ->. */
    fread(data->header, 3, 1, fp);
    /* ��������, ��� ��� ������������� ��� ID3v1,
    �.�. ���������� � TAG, ��� � ������� � ������������. */
    if(strcmp(data->header, "TAG") != 0) {
        return -1;
    }
    /* ��������� �� ����� ��������, 30 ����� � ����������� 1 ��. */
    fread(data->title, 30, 1, fp);
    fread(data->artist, 30, 1, fp);
    fread(data->album, 30, 1, fp);
    fread(data->year, 4, 1, fp);
    /* ����������� */
    fread(data->comment, 30, 1, fp);
    /* ��������, "�������" ����, ������� ����������, ������ �� ����� �����.
    �� - ������������� ���� � �����������, �.�. 28.
    ��������� ���� ����� � �������� 29. (������ �� 1 ������ ������� �������.)*/
    data->null_byte = data->comment[28];
    if(data->null_byte == 0) {
        data->track_number = data->comment[29];
    }
    /* ����. ��� ��������� ������ � ����������,
    � �� �������, ��� ������, ������ ����� �������� � ����� � ������� &: */
    fread(&data->genre, 1, 1, fp);
    return 0;
}

/* ������� ��� ������ ���������� �� data �� �����.
data �� ����������, ������ const. */
void printTagData(const ID3v1TagData *data)
{
    /* ������������ ����� ���� ��������� \t.
    %s - �������� ������� ��� ������ (������� char) */
    printf("��������:\t%s\n", data->title);
    printf("�����������:\t%s\n", data->artist);
    printf("������:\t%s\n", data->album);
    printf("���:\t%s\n", data->year);
    printf("�����������:\t%s\n", data->comment);
    printf("�������� �������� �����:\t%x\n", data->null_byte);
    if(data->null_byte == 0) {
        printf("����� �����:\t%d", data->track_number);
        /* � ������� ������� �������� � hex ����. */
        const unsigned char ch = data->track_number;
        printf(" (� hex: ox%x)\n", ch);
    }
    printf("����:\t%d\n", data->genre);
}

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");
    printf("����������: ������ char � ������: %d\n", sizeof(char));
    /* ���� ���������� ������� ������ 2, ������ ��� ����� ��� ����� �� ��������.
    ������ �������� - ������ ��� ���������. */
    if(argc < 2) {
        /* ����� ������ � stderr (����, ���� ������� ���������� ���������� � ������� � ���������). */
        fprintf(stderr, "������, ��������� ����!\n");
        printf("�������������:\n\n");
        printf("%s FILE.MP3\n\n", argv[0]);
        return EXIT_FAILURE; /* ����� */
    }
    /* ��� ����������� �������� ��������� �� �������� �������, ���������� ��� �����.
    �������� �� �����, ������ const. */
    const char *filename = argv[1];
    /* ��� ������� ����� ���� ��: const char *filename = "C:\\1.mp3"; */
    printf("��� �������� �����.\n\n");
    /* ������� ����. */
    FILE* fp = fopen(filename, "rb");
    /* fp - ��������� �� ������, ��� ����������� FILE,
    "r" - ��������� ���� �� ������ (read).
    �� �� ����� �� ����:
    FILE* fp = fopen(argv[1], "r"); */
    if(!fp) {
        /* �������� ����� ��������� �� ������ � stderr. */
        perror(filename);
        perror("���������� ������� ����!");
        return EXIT_FAILURE; /* ����� */
    }
    /* ������� ���������, � ������� ����� ��������� ���������� �� ����. */
    ID3v1TagData data;
    /* ������� ���� ������� ��� ���������� ���������� � ���� �� �����. */
    const int tagReadResult = readTagData(fp, &data);
    if(tagReadResult != 0) {
        printf("������: � ����� ��� ID3v1 ����!\n");
        fclose(fp);
        return EXIT_FAILURE; /* ����� */
    }
    /* ��������� ����. */
    fclose(fp);
    /* ����� ���������� �� �����. */
    printTagData(&data);
    return 0;
}
