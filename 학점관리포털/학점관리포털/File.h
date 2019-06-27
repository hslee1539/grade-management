#pragma once
#include "Array1.h"
#include <fstream>
using namespace std;
template < typename T >
class File
{
	ifstream * in = nullptr;
	ofstream * out = nullptr;
public:

	File(ifstream * input );
	File(ofstream * output);
	~File();
	//파일을 읽습니다. 실패할 경우 Array<T>()를 반환합니다.
	Array<T> read();
	// 파일을 저장합니다.
	void write(Array<T> &target, bool append = true);

};

template<typename T>
inline File<T>::File(ifstream * input)
{
	this->in = input;
}

template<typename T>
inline File<T>::File(ofstream * output)
{
	this->out = output;
}

template<typename T>
inline File<T>::~File()
{
	if (this->in != nullptr) {
		this->in->close();
		delete this->in;
	}
	if (this->out != nullptr) {
		this->out->close();
		delete this->out;
	}
	this->in = nullptr;
	this->out = nullptr;
}

template<typename T>
inline Array<T> File<T>::read()
{
	//new 와 delete를 계속 해서 
	if (this->in != nullptr) {
		if (this->in->is_open()) {
			streamoff streamSize;
			streamoff current = this->in->tellg();
			this->in->seekg(0, this->in->end);
			streamSize = this->in->tellg();
			this->in->seekg(current, this->in->beg);
			//1byte씩 읽어온 streamSize를 sizeof(T) 바이트의 크기를 가지는 배열 인덱스로 변환
			int maxINDEX = streamSize / sizeof(T);
			maxINDEX += streamSize > maxINDEX * sizeof(T);//만약 사이즈가 서로 맞지 않는 경우에 +1이 됨
			if (maxINDEX > 0) {
				Array<T> data(maxINDEX * sizeof(T), 0);//배열을 현 파일의 크기 만큼 
				this->in->read((char*)((void*)data.getData()), data.getByte());//즉 maxINDEX
				return data;
			}
		}
	}
	return Array<T>();
}

template<typename T>
inline void File<T>::write(Array<T>& target, bool append)
{
	if (this->out != nullptr) {
		if (this->out->is_open()) {
			if (append)
				this->out->seekp(0, this->out->out);
			else
				this->out->seekp(0, this->out->beg);
			this->out->write((char*)((void*)target.getData()), target.getByte());
		}
	}
}

