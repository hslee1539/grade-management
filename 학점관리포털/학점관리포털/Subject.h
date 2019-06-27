#pragma once
#include <string>
#include "MemoryTool.h"
#define ���� 0
#define ���� 1
#define �ʼ� 0
#define ���� 2
#define ���� ���� | �ʼ�
#define ���� ���� | ����
#define ���� ���� | �ʼ�
#define ���� ���� | ����

//���� 27
//���� 15
//���� 36
//���� 45
//��	123
//
//�ϼ� 7

class Subject
{
public:
	char name[10] = { 0,0,0,0,0,0,0,0,0,0 };
	char null = 0;// name�� �ΰ��� ���� ���� ���� ���ڿ��� ���� ��, ���� ��� �Լ��� ��ü������ strlen�� �Ͽ� ����ϴ� ���, memoryArea ������ ���� �ϱ� ���� ����.
public://������ ������ ���� ����
	char credit = 0;
	char score[3] = {0,0,0};
	// 8��Ʈ ������ �̷��� ���.
	// 0��Ʈ �κ� : ����(0) or ����(1)
	// 1��Ʈ �κ� : �ʼ�(0) or ����(1)
	// 2��Ʈ �κ� : 0
	// 3��Ʈ �κ� : 0
	char code = 0;
public://�޼ҵ��
	void reName(char * name , size_t byte) {MemoryTool::copyDeep<char>(this->name, name, byte);}
public://������ �Լ���
	explicit operator char*() {return this->name;}
	//1����Ʈ ������ �޸� ��ü�� ���մϴ�.
	bool operator == (Subject& right) { return MemoryTool::numberOfSameIndex<char>(((char*)((void*)this)), (char*)((void*)(&right)), sizeof(Subject)) == sizeof(Subject);}
};

