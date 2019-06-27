#pragma once

/*메모리 관련된 툴들을 가지고 있는 네임스페이스입니다. 구별해야 할 용어가 byte랑 index인데, byte는 메모리를 1바이트씩 보겠다는 의미이고, index는 int의 경우 4바이트씩 보겠다는 의미를 가집니다.*/
namespace MemoryTool {
	/*dataT 타입을 moveT으로 변환후 moveIndex만큼 포인터를 이동한 것을 returnT 타입으로 바꿔서 반환합니다.*/
	template < typename returnT, typename moveT, typename dataT > returnT * movePTR(dataT * data, size_t moveIndex) { return (returnT*)((void*)(&(((moveT*)((void*)data))[moveIndex]))); }
	/*mainT에 targetT를 copyT형으로 바꿔 copyIndex번 복사를 합니다. main과 target은 sizeof(processT) * copyIndex 보다 크개 할당 되어 있어야 합니다.*/
	template < typename copyT, typename mainT, typename targetT > void copyDeep(mainT *main, targetT *target, size_t copyIndex) {
		copyT* mainPTR = (copyT*)((void*)main);
		copyT* targetPTR = (copyT*)((void*)target);
		for (int i = 0; i < copyIndex; i++) {
			mainPTR[i] = targetPTR[i];
		}
	}
	/*왼쪽 레퍼런스와 오른쪽 레퍼런스를 교체합니다.*/
	template < typename leftT, typename rightT > void swap(leftT* &left, rightT* &right) {
		leftT * save = left;
		left = right;
		right = save;
	}
	/*두 변수의 바이너리 값이 같은지 검사합니다. 검사는 두 변수중 작은 타입의 범위만큼만 검사합니다. 2017.11.23일 추가 함수*/
	template <typename leftT, typename rightT > bool isSame(leftT &left, rightT right) {
		bool sw = sizeof(leftT) < sizeof(rightT);
		for (int i = 0, max = (sw) * sizeof(leftT) + (!sw) * sizeof(rightT); i < max; i ++ )
			if (*movePTR<char, char>(&left, i) != *movePTR<char, char>(&right, i))
				return false;
		return true;
	}
	/*main과 target를 processT으로 봐서 요소가 틀린 지점을 반환합니다. 모두 같을시 maxIndex를 반환합니다. main과 target은 maxIndex보다 같거나 많은 인덱스를 가지고 있어야 하고 processT는 mainT와 targetT보다 작거나 같은 타입이어야 합니다.*/
	template < typename processT, typename mainT, typename targetT > size_t numberOfSameIndex(mainT * main, targetT * target, size_t maxIndex) {
		processT * mainValue = (processT*)((void*)(main));
		processT * targetValue = (processT*)((void*)(target));
		size_t index;
		for (index = 0; index < maxIndex; index++) {
			mainValue = (processT*)((void*)(&main[index]));
			targetValue = (processT*)((void*)(&target[index]));
			/* 예전 코드
			if ( !(*mainValue == *targetValue) ) // 타입들이 == 연산을 해야함.
				return index;
				*/
			if (!isSame(*mainValue, *targetValue)) //2017.11.23일 수정.
				break;
		}
		return index;
	}
	/*main에서 target과 공통된 부분의 시작 인덱스(main기준)를 넘겨줌 (바이트가 아니라 배열의 인덱스 단위임) 없을시, mainIndex를 반환*/
	template < typename processT, typename mainT, typename targetT > size_t findIndex(mainT * main, size_t mainIndex, targetT * target, size_t targetIndex) {
		size_t index = 0;
		size_t sameIndex = 0;
		size_t maxIndex = mainIndex - targetIndex + 1;
		for (index = 0; index < maxIndex; index++) {
			sameIndex = numberOfSameIndex<processT>(&(main[index]), target, targetIndex);
			if (targetIndex == sameIndex) {
				return index;
			}
		}
		return mainIndex;
	}
	/*left가 클경우 -1 right가 클 경우 +1 같을 경우 0*/
	template < typename processT, typename leftT, typename rightT > int VS(leftT * left, size_t leftIndex, rightT * right, size_t rightIndex) {
		size_t passIndex = 0;
		bool sw = leftIndex < rightIndex;
		size_t maxIndex = sw * leftIndex + (!sw) * rightIndex;
		passIndex = numberOfSameIndex<processT>(left, right, maxIndex);
		processT * leftValue = &left[passIndex];
		processT * rightValue = &right[passIndex];
		return (leftValue[0] < rightValue[0]) - (leftValue[0] > rightValue[0]);
	}
	/*sizeof(T)의 배수이거나 아닌 byte를 sizeof(T)의 배수로 알려줍니다. findAllocByte<int>(5)를 할 경우 32비트 컴퓨터에서는 2를 반환하고 64비트 컴퓨터에서는 1을 반환합니다.*/
	template < typename T > size_t findAllocByte(size_t byte) {
		size_t returnData = byte / sizeof(T);
		returnData += (byte > returnData * sizeof(T));
		return returnData * sizeof(T);
	}
}