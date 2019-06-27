#pragma once
#include "Array1.h"
namespace Math {

	template <typename rT,typename nT , typename iT>
	rT pow(nT num, iT index) {
		rT returnData = 1;
		for (iT i = 0; i < index; i++)
			returnData *= num;
		return returnData;
	}
	// 로그를 정수로 구합니다. x는 아랫부분 y는 윗부분... 수학 용어를 잊음..
	size_t mylog(size_t x, size_t y) {
		size_t i = 0;
		for (; x <= y; y /= x, i++) {}
		return i;
	}
	// 양수값을 number 진수에 맞게 글자로 만듭니다.
	Array<char> toString(size_t integer , char number) {
		char num = '0';
		size_t max = mylog(number, integer)+1;
		Array<char> returnData(max + 1, 1);
		returnData[max] = 0;
		for (size_t i = max; i-- > 0;) {
			returnData[i] = integer % number + '0';
			integer /= number;
		}
		return returnData;
	}
}