#pragma once
#include "MemoryTool.h"
#include "BasicArray.h"
#include <malloc.h>
/*
리스트 자료형 클래스입니다.
구현중입니다.
*/
template < typename Key , typename Value >
class List
{
	template < typename oK, typename oV > friend class List;
	BasicArray* data;
	BasicArray* start;//값을 임의로 바꾸는 것을 금지
public:
	/*이 객체의 모든 데이터 셋들의 바이트를 알려줍니다. 할당된 영역의 바이트는 이 결과물 + sizeof(size_t) * 3 하면 됩니다.*/
	size_t getByte() { return this->start[-1] + this->start[-2] + this->start[-3] * sizeof(BasicArray) * 3; }
	/*이 객체의 모든 키값들의 바이트 수를 알려줍니다.*/
	size_t getKeyByte() { return this->start[-2]; }
	/*이 객체의 모든 값들의 바이트 수를 알려줍니다.*/
	size_t getValueByte() { return this->start[-1]; }
	/*이 객체의 데이터 셋의 수, 즉 인덱스 수를 알려줍니다.*/
	size_t getIndex() { return this->start[-3]; }

private:
	/*해당 정보 만큼 할당을 하고 가장 앞에 list의 수만 표시하고 포인트를 데이터 셋으로 설정 것만 함*/
	void _alloc( size_t allKeyByte , size_t allValueByte, size_t numberOfList) {
		size_t allByte = allKeyByte +  allValueByte + sizeof(BasicArray) * 3 * numberOfList;
		this->data = (BasicArray*)malloc(allByte + sizeof (size_t) * 3 );
		this->data += 3;
		this->start = data;
		this->data[-3].daa = numberOfList;
		this->data[-2].data = allKeyByte;
		this->data[-1].data = allValueByte;
	}
	/*해제 과정은 alloc함수에 의해 포인터가 이동 된 것을 다시 뒤로 간 뒤에 메모리 해제를 함.*/
	void _free() {
		if (this->start != nullptr) {
			this->start -= 3;
			free(this->start);
			this->data = nullptr;
			this->start = nullptr;
		}
	}
protected:
	template < typename keyT, typename valueT > void _set(BasicArray* main, keyT * key, size_t keyByte, valueT * value, size_t valueByte, size_t eoa) {
		main->getValueByte() = valueByte;
		main->getKeyByte() = keyByte;
		main->getEoA() = eoa;
		MemoryTool::copyDeep<char>(main->getKey(), key, keyByte);
		MemoryTool::copyDeep<char>(main->getValue(), value, valueByte);
	}
protected: //초기화 메소드
	template < typename KeyT, typename ValueT > void _init(KeyT * key, size_t keyByte, ValueT * value, size_t valueByte, size_t eoa) {
		this->_alloc(keyByte, valueByte, 1);
		this->_set(this->data, key, keyByte, value, valueByte, eoa);
	}
	template < typename leftKeyT , typename leftValueT , typename rightKeyT , typename rightValueT >void _initAppend(leftKeyT * leftKey, size_t leftKeyByte, leftValueT * leftValue, size_t leftValueByte, size_t leftEoA, rightKeyT * rightKey, size_t rightKeyByte, rightValueT * rightValue, size_t rightValueByte, size_t rightEoA) {
		this->_alloc(leftKeyByte + rightKeyByte, leftValueByte + rightValueByte, 2);
		this->_set(this->data, leftKey, leftKeyByte, leftValue, leftValueByte, leftEoA);
		this->_set(this->data->getNext(), rightKey, rightKeyByte, rightValue, rightValueByte, rightEoA);
	}
	template < typename leftKeyT, typename leftValueT, typename rightKeyT, typename rightValueT > void _initAppend(List<leftKeyT, rightKeyT> &left, rightKeyT * rightKey, size_t rightKeyByte, rightValueT * rightValue, size_t rightValueByte, size_t rightEoA) {
		this->_alloc(left.getKeyByte() + rightKeyByte, left.getValueByte() + rightValueByte, left.getIndex() +1);
		MemoryTool::copyDeep<char>(this->data, left.data, left.getByte());
		this->_set(MemoryTool::movePTR<BasicArray, char>(this->data, left.getByte()) , rightKey, rightKeyByte, rightValue, rightValueByte, rightEoA);
	}
	template < typename leftKeyT, typename leftValueT, typename rightKeyT, typename rightValueT >void _initAppend(leftKeyT * leftKey, size_t leftKeyByte, leftValueT * leftValue, size_t leftValueByte, size_t leftEoA, List<rightKeyT, rightValueT> & right) {
		this->_alloc(leftKeyByte + right.getKeyByte(), leftValueByte + right.getValueByte(), right.getIndex() + 1);
		this->_set(this->data, leftKey, leftKeyByte, leftValue, leftValueByte, leftEoA);
		MemoryTool::copyDeep<char>(this->data->getNext(), right.data, right.getByte());
	}
	template < typename leftKeyT, typename leftValueT, typename rightKeyT, typename rightValueT >void _initAppend(List<leftKeyT, leftValueT > & left, List<rightKeyT, rightValueT> & right) {
		this->_alloc(left.getKeyByte() + right.getKeyByte(), left.getValueByte() + right.getValueByte(), left.getIndex() + right.getIndex());
		MemoryTool::copyDeep<char>(this->data, left.data, left.getByte());
		MemoryTool::copyDeep<char>(this->data->getNext(), right.data, right.getByte());
	}
public:

};

