#pragma once
struct BasicArray
{
	size_t data;
	size_t& getValueByte() { return this[0].data; }
	size_t& getEoA() { return this[1].data; }
	size_t& getKeyByte() { return this[2].data; }
	/*만약 키 바이트 수가 0이면 값을 가지고 옴*/
	char* getKey() { return (char*)(void*)&this[3].data; }
	/*반듯이 key설정이 끝나고서 사용 할 것.*/
	char* getValue() { return (char*)(void*)&this[3 + this->getKeyByte()].data; }
	/*다음으로 가기 전에 현 데이터가 다 완성이 되었는지 체크 할 것.*/
	void* getNext() {
		char * PTR = (char*)this->getValue();
		PTR += this->getValueByte();
		return PTR;
	}
};