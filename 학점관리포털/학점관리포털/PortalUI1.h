#pragma once
#include "Array1.h"
#include "Subject.h"
#include <iostream>

/*
ȭ�� ����� �������̽��Դϴ�. �� �������̽��� ����Ͽ� ������ �����߽��ϴ�. (ListBox , TextBar)
���� ������ PotalSystem�� �մϴ�.
�ʵ� ������ �����ڰ� �����ϴ�.
*/
class PortalUI1
{
protected://�Է�
	/*��Ƽ ����Ʈ�� cmd���� ���� �Է� ������ �޽��ϴ�.*/
	virtual Array<char> getline( size_t buffer ) {
		//���� �����Դϴ�.
		char * PTR = new char[buffer];
		//console input������ ������ ��Ʈ���� �ƴ��� ��Ÿ���ϴ�.
		bool end_of_stream = true;
		//�� �迭 ��ü�� PTR�� ��� append�ϸ鼭 ������� ����ϴ�.
		Array<char> arr;
		//PTR���� ���Ⱑ �ִ� ���� �˷� �ݴϴ�. ������ buffer���� �����ϴ�.
		size_t byte;
		do {
			for (byte = 0; byte < buffer; byte++) {
				PTR[byte] = cin.get();
				if (PTR[byte] == '\n') {
					PTR[byte] = 0;//���� �κ��� �ι��ڷ� ��ȯ.
					end_of_stream = false;
					byte++;
					break;
				}
			}
			arr.append(PTR, byte, !end_of_stream);
		} while (end_of_stream );
		delete[] PTR;
		return arr;
	}
protected://ȭ�� ���
	/*[data1][����][data2][����][data3]�������� ����� �մϴ�.
	data1�� ���� ���ڰ� ���� �� ���� ����� �ϸ�, maxByte���� ���� ��, ����� �Ųߴϴ�.*/
	void drawFormat1(char * data, size_t maxByte, size_t nullByte, char data2, char * data3 ) {
		size_t strlenData = strlen(data);
		bool trust = strlenData < maxByte;
		//strlen�� �ŷ��� �� ������ ����ϰ� ������ maxByte��
		size_t max = trust * strlenData + (!trust)* maxByte;
		for (size_t i = 0; i < max; i++)
			cout << data[i];
		for (size_t i = max; i < maxByte - nullByte; i++)
			cout << " ";
		cout << "   " << data2 << "   " << data3 << endl;
	}
	void drawFormat1_table1(char *table1, char *table2 , char *table3) {
		cout <<  table1 << "        " <<  table2 << "   " << table3 << "   " << endl;
	}
	void drawFormat1_table2(char *table1, int table2) {
		cout << table1 << "           " << table2 << endl;
	}
public:
	/*��� �κ�*/
	virtual void draw() = 0;
};

