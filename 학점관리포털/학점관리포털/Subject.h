#pragma once
#include <string>
#include "MemoryTool.h"
#define 전공 0
#define 교양 1
#define 필수 0
#define 선택 2
#define 전필 전공 | 필수
#define 전선 전공 | 선택
#define 교필 교양 | 필수
#define 교선 교양 | 선택

//교필 27
//교선 15
//전필 36
//전선 45
//합	123
//
//일선 7

class Subject
{
public:
	char name[10] = { 0,0,0,0,0,0,0,0,0,0 };
	char null = 0;// name에 널값을 포함 하지 않은 문자열을 넣을 떄, 문자 출력 함수가 자체적으로 strlen을 하여 출력하는 경우, memoryArea 오류를 방지 하기 위해 만듬.
public://학점과 점수와 과목 종류
	char credit = 0;
	char score[3] = {0,0,0};
	// 8비트 공간을 이렇게 사용.
	// 0비트 부분 : 전공(0) or 교양(1)
	// 1비트 부분 : 필수(0) or 선택(1)
	// 2비트 부분 : 0
	// 3비트 부분 : 0
	char code = 0;
public://메소드들
	void reName(char * name , size_t byte) {MemoryTool::copyDeep<char>(this->name, name, byte);}
public://연산자 함수들
	explicit operator char*() {return this->name;}
	//1바이트 단위로 메모리 전체를 비교합니다.
	bool operator == (Subject& right) { return MemoryTool::numberOfSameIndex<char>(((char*)((void*)this)), (char*)((void*)(&right)), sizeof(Subject)) == sizeof(Subject);}
};

