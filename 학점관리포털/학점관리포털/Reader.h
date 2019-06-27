#pragma once
//#include "Array.h"
#include "File.h"
#include "MyDBObject.h"
#include <fstream>
using namespace std;
typedef Array<MyDBObject> KeyList;
/*
*/
template <typename T>
class Reader {
	Array<char> getValue(char * keyName) {
		File<char> stream(new ifstream(keyName, ifstream::binary));
		return stream.read()
	}
	KeyList getKey(char * fileName) {
		File<MyDBObject> stream(new ifstream(fileName, ifstream::binary));
		return stream.read();
	}
public:
	Array<T> get(char * fileName) {
		KeyList list = this->getKey(fileName);

	}
};