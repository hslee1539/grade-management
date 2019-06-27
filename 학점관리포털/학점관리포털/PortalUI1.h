#pragma once
#include "Array1.h"
#include "Subject.h"
#include <iostream>

/*
화면 입출력 인터페이스입니다. 이 인터페이스를 상속하여 위젯을 구현했습니다. (ListBox , TextBar)
파일 관련은 PotalSystem이 합니다.
필드 변수와 생성자가 없습니다.
*/
class PortalUI1
{
protected://입력
	/*멀티 바이트로 cmd에서 엔터 입력 전까지 받습니다.*/
	virtual Array<char> getline( size_t buffer ) {
		//버퍼 공간입니다.
		char * PTR = new char[buffer];
		//console input에서의 마지막 스트림이 아닌지 나타냅니다.
		bool end_of_stream = true;
		//이 배열 객체로 PTR을 계속 append하면서 결과물을 만듭니다.
		Array<char> arr;
		//PTR에서 띄어쓰기가 있는 곳을 알려 줍니다. 없을시 buffer값과 같습니다.
		size_t byte;
		do {
			for (byte = 0; byte < buffer; byte++) {
				PTR[byte] = cin.get();
				if (PTR[byte] == '\n') {
					PTR[byte] = 0;//엔터 부분을 널문자로 변환.
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
protected://화면 출력
	/*[data1][공백][data2][공백][data3]형식으로 출력을 합니다.
	data1은 공백 문자가 나올 때 까지 출력을 하며, maxByte보다 작을 시, 띄어쓰기로 매꿉니다.*/
	void drawFormat1(char * data, size_t maxByte, size_t nullByte, char data2, char * data3 ) {
		size_t strlenData = strlen(data);
		bool trust = strlenData < maxByte;
		//strlen이 신뢰할 수 있으면 사용하고 없으면 maxByte로
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
	/*어뎁터 부분*/
	virtual void draw() = 0;
};

