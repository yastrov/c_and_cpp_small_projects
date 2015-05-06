/*
The program illustrates how to work with casts (static_cast, reinterpret_cast)
and work with char array as array of ints. For presentation (lesson, how-to)
for juniors.
C++ language.
Written by Yuri (Yuriy) Astrov
*/
#include <iostream>
#include <iomanip>
#include <string>
#include <locale>
using namespace std;

size_t numofchars(const char* ch) {
	size_t n = 0;
	const char* pointer = ch;
	while (*pointer != '\0'){
		n++;//n=n+1; n+=1;
		pointer++;
	}
	return n;
}

int main(int argc, char**argv)
{
	setlocale(LC_ALL, "RUSSIAN");
	/*Строка может быть строкой, а может массивом char-ов,
	который нам сейчас интереснее и лежит в основе строки.*/
	const char *hworld = "Hello world";
	const size_t size_of_string = numofchars(hworld) + 1;
	cout << setw(15) << "Size of char: " << sizeof(char) << endl
		<< setw(15) << "Size of int: " << sizeof(int) << endl;
	cout << "char" << "\tScan Code клавиши (dec, hex)" << endl;
	for (size_t i = 0; i < size_of_string; i++){
		/*Распечатаем символ.*/
		cout << hworld[i] << "\t"
			/*Распечатаем код клавиши в 10-тичном виде.*/
			<< dec << setw(4) << static_cast<int>(hworld[i])//(int)hworld[i]
			/*Распечатаем код клавиши в 16-ти ричном виде.*/
			<< "\t" << hex << static_cast<int>(hworld[i]) << endl;
	}
	cout << endl;
	/*
	Объявляем указатель на ту же область памяти
	но теперь будем считать, что она состоит из ячеек
	типа int, каждая размером 4 байта.
	*/
	const int *hint = reinterpret_cast<const int*>(hworld);
	//const int *hint = (const int*)hworld;
	const size_t size_of_string_int = 
		//(const size_t)(size_of_string / sizeof(int) + 0.5);
		static_cast<const size_t>(size_of_string / sizeof(int) + 0.5);
	/*
	Объявляем указатель на int и сразу же
	ввыделяем память для 3х элементов:
	3*4 = 12 байт
	*/
	int *new_hint = new int[size_of_string_int];
	/*Можно было бы int new_hint2[3];*/
	cout << "Array of int:" << endl;
	for (size_t i = 0; i < size_of_string_int; i++){
		cout << dec << setw(10) << hint[i] << "\t"
			<< hex << hint[i] << endl;
		/*Для чистоты эксперимента копируем уже как int-ы.*/
		//new_hint[i] = (int)hint[i];
		new_hint[i] = static_cast<int>(hint[i]);
	}
	cout << endl << endl << "Оригинальная строка:" << endl;
	/*Чтобы распечатать строку пользователю нужно представить
	массив числе как массив символов.*/
	//cout << (const char*)new_hint << endl;
	cout << reinterpret_cast<char*>(new_hint) << endl;
	/*Или предварительно сделав строкой.*/
	//cout << string((const char*)new_hint) << endl;
	cout << string(reinterpret_cast<char*>(new_hint)) << endl;
	/*Освободим выделенную ранее память.*/
	delete new_hint;
	return 0;
}