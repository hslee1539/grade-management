#pragma once
struct BasicArray
{
	size_t data;
	size_t& getValueByte() { return this[0].data; }
	size_t& getEoA() { return this[1].data; }
	size_t& getKeyByte() { return this[2].data; }
	/*���� Ű ����Ʈ ���� 0�̸� ���� ������ ��*/
	char* getKey() { return (char*)(void*)&this[3].data; }
	/*�ݵ��� key������ ������ ��� �� ��.*/
	char* getValue() { return (char*)(void*)&this[3 + this->getKeyByte()].data; }
	/*�������� ���� ���� �� �����Ͱ� �� �ϼ��� �Ǿ����� üũ �� ��.*/
	void* getNext() {
		char * PTR = (char*)this->getValue();
		PTR += this->getValueByte();
		return PTR;
	}
};