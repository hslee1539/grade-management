#pragma once
#include "MemoryTool.h"
#include "BasicArray.h"
#include <malloc.h>
using namespace std;


/*배열을 관리해주는 클래스입니다.*/
template < typename T >
class Array {
	template < typename oT > friend class Array;
	char * data;
	BasicArray* _get(){ return ((BasicArray*)((void*)this->data)); }
	void * _getData() { return ((BasicArray*)((void*)this->data))->getValue(); }

private: //가장 기초적인 메소드들
	void _create( size_t allocByte , size_t saveByte , size_t eoa) {
		this->data = (char *)malloc(allocByte + sizeof(BasicArray) * 3);
		//this->data = new char[allocByte + sizeof(BasicArray) * 3];
		this->_get()->getEoA() = eoa;
		this->_get()->getValueByte() = saveByte;
		this->_get()->getKeyByte() = 0;
	}
	void _delete() {
		if (this->data != nullptr) {
			free(this->data);
			//delete[] data;
			this->data = nullptr;
		}
	}
protected://초기화 메소드로 4가지의 경우가 있습니다.
	// 1. 배열 하나와 배열 사이즈가 있는 경우, (사이즈 정보가 반듯이 필요)
	// 2. 배열 두개를 합치는 경우 (두 배열의 사이즈 정보가 반듯이 필요)
	// 3. 배열1에 포함된 배열2를 배열3으로 대체하여 초기화 하는 경우 (3개의 사이즈 정보가 반듯이 필요)
	// 4. 배열1에 배열2의 가 포함 되어 있으면 요소를 제거하여 초기화 하는 경우 (두 배열의 사이즈 정보가 반듯이 필요)
	// 
	/*포인터 타입의 배열을 사이즈와 eoa(배열의 마지막)을 알려줘서 새롭게 객체를 생성합니다.*/
	template < typename dataT > void init(dataT * data, size_t dataByte, size_t eoa) {
		int byte = MemoryTool::findAllocByte<T>(dataByte);
		this->_create(byte, dataByte, eoa);
		if (data != nullptr)
			MemoryTool::copyDeep<char>(this->_getData(), data, dataByte);
	}
	/*왼쪽 포인터에 왼쪽 바이트 까지 앞에 복사하고 뒤에 오른쪽 포인터부분의 오른쪽 바이트 수 만큼 복사하고 eoa(배열의 마지막)을 설정하여 초기화 합니다. 이 배열은 leftByte + rightByte의 크기를 가집니다.*/
	template < typename leftT , typename rightT > void initAppend ( leftT* left , size_t leftByte , rightT* right , size_t rightByte , size_t eoa ){
		size_t byte = MemoryTool::findAllocByte<T>(leftByte + rightByte);
		this->_create(byte, leftByte + rightByte, eoa);
		MemoryTool::copyDeep<char>(this->_getData(), left, leftByte);
		MemoryTool::copyDeep<char>(MemoryTool::movePTR<char, char>(this->_getData(), leftByte), right, rightByte);
	}
	/*메인에 타깃에 해당되는 부분을 리플레이스 포인터 정보로 대체하여 초기화 합니다.*/
	template < typename mainT, typename targetT, typename replaceT > void initReplace(mainT * main, size_t mainByte, targetT * target, size_t targetByte, replaceT * replace, size_t replaceByte, size_t eoa) {
		size_t startIndex = MemoryTool::findIndex<char>(main, mainByte / sizeof(mainByte), target, targetByte / sizeof(targetByte));
		bool sw = (startIndex * sizeof(mainT)) != mainByte;//공통 된 부분이 있는지 검사
		size_t byte = MemoryTool::findAllocByte<T> (mainByte + sw* (replaceByte - targetByte));
		this->_create(byte, mainByte + sw*(replaceByte - targetByte), eoa);
		MemoryTool::copyDeep<char>(this->_getData(), main, startIndex * sizeof(mainT));
		MemoryTool::copyDeep<char>(MemoryTool::movePTR<char, mainT>(this->_getData(), startIndex), replace, sw * replaceByte );
		MemoryTool::copyDeep<char>(MemoryTool::movePTR<char, char>(this->_getData(), startIndex * sizeof(mainT) + replaceByte), MemoryTool::movePTR<char, char>(main, startIndex * sizeof(mainT) + targetByte), mainByte - startIndex * sizeof(mainT) -  sw*targetByte);
	}
	/*메인에 타깃 정보가 있으면 제외하고 초기화 합니다. mainByte_exceptEoA과 targetByte는 eoa 공간 수를 제외하고 입력을 하고 mainByte_exceptEoA는 반듯이 eoa 공간을 제외 한 값으로 줘야 합니다.*/
	template < typename mainT, typename targetT > void initExcept(mainT * main, size_t mainByte_exceptEoA, size_t mainEoA ,targetT *target, size_t targetByte) {
		//공통된 부분의 시작 위치 인덱스입니다.
		size_t startIndex;
		if ( sizeof(mainT) > sizeof(targetT))
			//	findIndex가 공통된 부분의 시작 점을 알려줍니다. 없을 경우 2번째 파라메터 mainByte_exceptEoA/sizeof(mainT)를 돌려 줍니다.
			startIndex = MemoryTool::findIndex<mainT>(main, mainByte_exceptEoA / sizeof(mainT), target, targetByte / sizeof(targetT));
		else
			startIndex = MemoryTool::findIndex<targetT>(main, mainByte_exceptEoA / sizeof(mainT), target, targetByte / sizeof(targetT));
		//	공통된 부분이 있는 지 검사합니다.
		bool sw = (startIndex * sizeof(mainT)) != mainByte_exceptEoA;
		//	만약 공통된 부분이 있으면 메모리는 총, 타겟을 제외하여 mainByte_exceptEoA + mainEoA - targetByte가 됩니다. 
		//	하지만 공통된 부분이 없을때, 특히 target쪽이 메모리가 더 큰 경우 mainByte_exceptEoA + mainEoA - targetByte하면 오버 플로우가 일어나므로,  
		//	공통된 부분이 있으면 빼고 없으면 main 포인터만 복사 하기 위해서 sw를 targetByte에 곱해줘 안정성을 높였습니다.
		size_t byte = MemoryTool::findAllocByte<T>(mainByte_exceptEoA + mainEoA - sw * targetByte);
		this->_create(byte, mainByte_exceptEoA - sw*targetByte, mainEoA);
		MemoryTool::copyDeep<char>(this->_getData(), main, startIndex * sizeof(mainT));
		MemoryTool::copyDeep<char>(MemoryTool::movePTR<char, mainT>(this->_getData(), startIndex), MemoryTool::movePTR<char, char>(main, startIndex * sizeof(mainT) + sw*targetByte), sw*(mainByte_exceptEoA + mainEoA - (startIndex * sizeof(mainT) + targetByte)));
		//MemoryTool::copyDeep<char>(MemoryTool::movePTR<char, mainT>(this->_getData(), startIndex * sizeof(mainT)), MemoryTool::movePTR<char, mainT>(main, startIndex * sizeof(mainT) + targetByte), mainByte_exceptEoA - startIndex * sizeof(mainT) - targetByte + mainEoA);
		//MemoryTool::copyDeep<char>(MemoryTool::movePTR<char, mainT>(this->_getData(), startIndex ), MemoryTool::movePTR<char, mainT>(main, startIndex ), mainByte_exceptEoA + mainEoA - startIndex * sizeof(mainT) - targetByte);
	}
public://생성자들
	Array() : data(nullptr) {}
	Array(size_t byte , size_t eoa):data(nullptr) {this->init( this->data, byte, eoa);}
	template < typename dataT > Array(dataT *data, size_t dataByte, size_t eoa) {this->init(data, dataByte, eoa);}
	template < typename dataT > Array(dataT &atom) { this->init(&atom, sizeof(dataT), 0); }
	template < typename leftT, typename rightT > Array(leftT *left, size_t leftByte, size_t leftEoA, rightT *right, size_t rightByte, size_t rightEoA , bool appendOrException) {
		if (appendOrException)
			this->initAppend(left, leftByte - leftEoA, right, rightByte, rightEoA);
		else
			this->initExcept(left, leftByte - leftEoA, leftEoA, right, rightByte - rightEoA);
	}
	template < typename dataT > Array(Array<dataT> &arr) { this->init(arr.getData(), arr.getByte(), arr.getEoA()); }
	// append의 경우, 왼쪽 배열의 eoa가 1 이상인 경우 해당 바이트를 제외하고 서 오른쪽 배열을 추가합니다. 오른쪽 배열이 마지막에 오므로 이 객체의 eoa는 오른쪽 객체의 eoa를 상속 받습니다..
	// except의 경우, 오른쪽 배열에서 eoa를 제외한 부분이 왼쪽 배열에서 eoa를 제외한 부분과 같은 내용이 있으면 제외하고 초기화 합니다. 이 객체는 왼쪽 배열의 정보를 받으므로, 왼쪽 eoa를 상속 받습니다.
	/*appendOrExcept가 true 이면 두 배열을 append하여 초기화 하고, false 이면 왼쪽 배열에 오른쪽 배열의 요소가 있으면 제외 하여 초기화 합니다.*/
	template < typename leftT, typename rightT > Array(Array<leftT> &left, Array<rightT> &right , bool appendOrExcept = true) {
		if (appendOrExcept)
			this->initAppend(left.getData(), left.getByte() - left.getEoA(), right.getData(), right.getByte(), right.getEoA());
		else
			this->initExcept(left.getData(), left.getByte() - left.getEoA(), left.getEoA(), right.getData(), right.getByte() - right.getEoA());
	}
	template < typename dataT > Array(Array<dataT> &&rValue) {
		this->data = nullptr;
		MemoryTool::swap(this->data, rValue.data);
	}
public://소멸자
	~Array() {
		this->_delete();
	}
public:// 타입 변환
	/*operator T* () {
		if ( this->data != nullptr)
			return (T*)this->_getData();
		return nullptr;
	}*/
	T& operator [] ( int index){
		return ((T*)this->_getData())[index];
	}
public:// 대입 연산자
	template < typename dataT > Array<T>& operator = (Array<dataT> &lValue) {
		this->_delete();
		this->init(lValue._getData() , lValue._getByte() , lValue._getEoA());
		return this*
	}
	template < typename dataT > Array<T>& operator = (Array<dataT> &&rValue) {
		MemoryTool::swap(this->data , rValue.data);
		return *this;
	}

public:// 증감 연산자
	template < typename rightT> Array<T> operator + (rightT & right) {return Array <T>(this->getData(), this->getByte() ,this->getEoA(), &right, sizeof(rightT) , 0 , true);}
	/*새로운 객체를 만들어 그곳에다 이 객체와 오른쪽 객체를 append하여 반환합니다.*/
	template < typename rightT> Array<T> operator + (Array<rightT> &right) {return Array<T>(*this, right , true);}
	/*이 객체에 오른쪽 객체와 append하여 초기화 합니다.*/
	template < typename rightT> Array<T>& operator += ( rightT & right){ return *this = Array<T>(this->getData(), this->getByte(), this->getEoA(), &right, sizeof(rightT), 0, true);}
	/*이 객체에 오른쪽 객체와 append하여 초기화 합니다.*/
	template < typename rightT > Array<T>& operator += (Array<rightT> &right) {return this[0] = Array< T> (*this, right, true);}
	/*새로운 객체를 만들어 오른쪽 대상과 공통된 부분을 제외합니다.*/
	template < typename rightT> Array<T> operator - (Array<rightT> &right) {return Array<T>(*this, right, false);}
	/*이 객체에 오른쪽 대상을 제거합니다.*/
	template < typename rightT> Array<T> operator - (rightT &right) { return Array<T>(this->getData(), this->getByte(), this->getEoA(), &right, sizeof(rightT), 0, false); }
	/*이 객체에 오른쪽 대상을 제거하여 초기화 합니다.*/
	template < typename rightT> Array<T>& operator -= ( rightT &right){ return this[0] = Array<T>(this->getData(), this->getByte(), this->getEoA(), &right, sizeof(rightT), 0, false);}
	/*이 객체에 오른쪽 객체와 공통된 부분을 제외하여 초기화 합니다*/
	template <typename dataT> Array<T>& operator -= (Array<dataT> & right) {return this[0] = Array<T>(*this, right, false);}
public:// 비교 연산자
	template < typename rightT> bool operator == (rightT &right) {return MemoryTool::VS<char>(this->getData(), (this->getByte() - this->getEoA()) / sizeof(T), &right, 1) == 0;}
	template < typename rightT> bool operator == (Array<rightT> &right) {
		return MemoryTool::VS<char>(this->getData(), (this->getByte() - this->getEoA())/ sizeof(T), right.getData(), (right.getByte() - right.getEoA() )/ sizeof(rightT)) == 0;
	}

public://메소드들
	T* getData() {
		if (this->data != nullptr)
			return (T*)this->_getData();
		return nullptr;
	}
	size_t getByte() {
		if (this->data != nullptr)
			return this->_get()->getValueByte();
		return 0;
	}
	size_t getEoA() {
		if (this->data != nullptr)
			return this->_get()->getEoA();
		return 0;
	}
	size_t getIndex() {
		if (this->data != nullptr)
			return this->_get()->getValueByte() / sizeof(T);
		return 0;
	}
	template < typename dataT > void append(dataT * data, size_t dataByte, size_t eoa) {*this = Array<T>(this->getData(), this->getByte(), this->getEoA(), data, dataByte, eoa , true);}
};