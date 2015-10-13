/*
Program for read some binary file format with list, which contain list with readen books.
C++ language.

Compile: g++ -std=c++11 readenbooks.cpp -o rbooks

Written by Yuri (Yuriy) Astrov*/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <locale>
#include <vector>
#include <tchar.h>
#include <algorithm>
using namespace std;
//#define CHECK_COMPARE_WITH_C_CAST
//#define CHAR_MODE
// We don't need to pack here< but I want to remember it.
#ifdef CHAR_MODE
const string mode_info = "CHAR MODE";
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

void readfrom(std::ifstream& stream, fileinfo *out)
{
	//stream.read((char*)&out->number, sizeof(out->number));
	stream.read(reinterpret_cast<char*>(&out->number), sizeof(out->number));
	stream.read(reinterpret_cast<char*>(&out->type), sizeof(out->type));
	stream.read(reinterpret_cast<char*>(&out->sizeP), sizeof(out->sizeP));
	stream.read(reinterpret_cast<char*>(out->path), out->sizeP);
	stream.read(reinterpret_cast<char*>(&out->sizeF), sizeof(out->sizeF));
	stream.read(reinterpret_cast<char*>(out->filename), out->sizeF);
#ifdef CHECK_COMPARE_WITH_C_CAST
	cout << "---Check reinterpret_cast<char*>(out->filename) == reinterpret_cast<char*>(&out->filename): ";
		//I check, it's valid.
		if(reinterpret_cast<char*>(out->filename) == reinterpret_cast<char*>(&out->filename))
			cout << "It's valid" << endl;
		else
			cout << "Warning int cast" << endl;
#endif
}
#else
// String Mode
const string mode_info = "STRING MODE";
struct fileinfo
{
	int number;
	int type;
	int sizeP;	// It for compability with printelement function.
	string path;
	int sizeF;	// It for compability with printelement function.
	string filename;
};

void readfrom(std::ifstream& stream, fileinfo *out)
{
	stream.read(reinterpret_cast<char*>(&out->number), sizeof(out->number));
	stream.read(reinterpret_cast<char*>(&out->type), sizeof(out->type));
	stream.read(reinterpret_cast<char*>(&out->sizeP), sizeof(out->sizeP));
	//Important! First - resize, second - get begin.
	out->path.resize(out->sizeP, ' ');
	char* begin = &*out->path.begin();
	stream.read(begin, out->sizeP);
	stream.read(reinterpret_cast<char*>(&out->sizeF), sizeof(out->sizeF));
	out->filename.resize(out->sizeF, ' ');
	begin = &*out->filename.begin();
	stream.read(begin, out->sizeF);
}
#endif

void printelement(std::ostream& stream, const fileinfo& item)
{
	stream << endl;
	stream << std::setw(11) << "Number:\t" << item.number << endl;
	stream << std::setw(11) << "Type:\t" << item.type << endl;
	stream << std::setw(11) << "File name:\t" << item.filename << endl;
	stream << std::setw(11) << "Path:\t" << item.path << endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "RUSSIAN");
	cout << "Compile struct and functions: " << mode_info << endl;
	std::vector<fileinfo> items;
	int count, i=0;
#ifdef CHECK_COMPARE_WITH_C_CAST
	cout << "---Check reinterpret_cast<char*>(&count) == (char*)&count: ";
	//I check, it's valid.
	if(reinterpret_cast<char*>(&count) == (char*)&count) {
		cout << "It's valid" << endl;
	} else {
		cout << "Warning int cast" << endl;
	}
#endif
	char ending[4];
	std::ifstream input(_T(argv[1]), std::ifstream::binary);
	if (input) {
		input.read(reinterpret_cast<char*>(&count), sizeof(count));
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
			input.read(reinterpret_cast<char*>(ending), sizeof(ending));
		input.close();
	}
	/*
	for_each(items.cbegin(), items.cend(), [](const fileinfo& item){
		printelement(cout, item);
	});
	*/
	for(const auto& item: items)
		printelement(cout, item);
	std::cout << endl;
	return 0;
}

