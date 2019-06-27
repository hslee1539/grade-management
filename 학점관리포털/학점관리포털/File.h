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
	//������ �н��ϴ�. ������ ��� Array<T>()�� ��ȯ�մϴ�.
	Array<T> read();
	// ������ �����մϴ�.
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
	//new �� delete�� ��� �ؼ� 
	if (this->in != nullptr) {
		if (this->in->is_open()) {
			streamoff streamSize;
			streamoff current = this->in->tellg();
			this->in->seekg(0, this->in->end);
			streamSize = this->in->tellg();
			this->in->seekg(current, this->in->beg);
			//1byte�� �о�� streamSize�� sizeof(T) ����Ʈ�� ũ�⸦ ������ �迭 �ε����� ��ȯ
			int maxINDEX = streamSize / sizeof(T);
			maxINDEX += streamSize > maxINDEX * sizeof(T);//���� ����� ���� ���� �ʴ� ��쿡 +1�� ��
			if (maxINDEX > 0) {
				Array<T> data(maxINDEX * sizeof(T), 0);//�迭�� �� ������ ũ�� ��ŭ 
				this->in->read((char*)((void*)data.getData()), data.getByte());//�� maxINDEX
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

