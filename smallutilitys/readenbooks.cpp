/*
Program for read some binary file format with list, which contain list with readen books.
C++ language.
Written by Yuri (Yuriy) Astrov*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <locale>
#include <vector>
#include <tchar.h>
#include <algorithm>
using namespace std;

// We don't need to pack here< but I want to remember it.
//#pragma pack(push, r1, 1)
struct fileinfo
{
	int number;
	int type;
	int sizeP; //Size of path string
	char path[256]; // We really don't want to have a problem with array and pointers in this case!
	int sizeF; //Size of filename string
	char filename[256];
};
//#pragma pack(pop, r1)

void readfrom(ifstream& stream, fileinfo *out)
{
	stream.read((char*)&out->number, sizeof(out->number));
	stream.read((char*)&out->type, sizeof(out->type));
	stream.read((char*)&out->sizeP, sizeof(out->sizeP));
	stream.read((char*)&out->path, out->sizeP);
	stream.read((char*)&out->sizeF, sizeof(out->sizeF));
	stream.read((char*)&out->filename, out->sizeF);
}

void printelement(fileinfo item)
{
	cout << endl;
	cout << std::setw(11) << "Number:\t" << item.number << endl;
	cout << std::setw(11) << "Type:\t" << item.type << endl;
	cout << std::setw(11) << "File name:\t" << item.filename << endl;
	cout << std::setw(11) << "Path:\t" << item.path << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "RUSSIAN");
	fileinfo item;
	std::vector<fileinfo> items;
	int count, i=0;
	char ending[4];
	std::ifstream input(_T(argv[0]), std::ifstream::binary);
	if (input) {
		input.read((char*)&count, sizeof(count));
		cout << "Количество записей: " << count << endl;
		cout << "Файлы:" << endl;
		items.resize(count);
		while (i < count && !input.eof()){
			/* Only for example:
			input.read((char*)&item.number, sizeof(item.number));
			input.read((char*)&item.type, sizeof(item.type));
			input.read((char*)&item.sizeP, sizeof(item.sizeP));
			input.read((char*)&item.path, item.sizeP);
			input.read((char*)&item.sizeF, sizeof(item.sizeF));
			input.read((char*)&item.filename, item.sizeF);
			*/
			//readfrom(input, &item);
			readfrom(input, &items[i]);
			++i;
		}
		if (!input.eof())
			input.read((char*)&ending, sizeof(ending));
		input.close();
		cout << "Подробности: " << endl;
		for_each(items.begin(), items.end(), printelement);
		std::cout << endl;
	}
	return 0;
}

