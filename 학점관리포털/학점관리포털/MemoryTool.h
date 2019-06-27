#pragma once

/*�޸� ���õ� ������ ������ �ִ� ���ӽ����̽��Դϴ�. �����ؾ� �� �� byte�� index�ε�, byte�� �޸𸮸� 1����Ʈ�� ���ڴٴ� �ǹ��̰�, index�� int�� ��� 4����Ʈ�� ���ڴٴ� �ǹ̸� �����ϴ�.*/
namespace MemoryTool {
	/*dataT Ÿ���� moveT���� ��ȯ�� moveIndex��ŭ �����͸� �̵��� ���� returnT Ÿ������ �ٲ㼭 ��ȯ�մϴ�.*/
	template < typename returnT, typename moveT, typename dataT > returnT * movePTR(dataT * data, size_t moveIndex) { return (returnT*)((void*)(&(((moveT*)((void*)data))[moveIndex]))); }
	/*mainT�� targetT�� copyT������ �ٲ� copyIndex�� ���縦 �մϴ�. main�� target�� sizeof(processT) * copyIndex ���� ũ�� �Ҵ� �Ǿ� �־�� �մϴ�.*/
	template < typename copyT, typename mainT, typename targetT > void copyDeep(mainT *main, targetT *target, size_t copyIndex) {
		copyT* mainPTR = (copyT*)((void*)main);
		copyT* targetPTR = (copyT*)((void*)target);
		for (int i = 0; i < copyIndex; i++) {
			mainPTR[i] = targetPTR[i];
		}
	}
	/*���� ���۷����� ������ ���۷����� ��ü�մϴ�.*/
	template < typename leftT, typename rightT > void swap(leftT* &left, rightT* &right) {
		leftT * save = left;
		left = right;
		right = save;
	}
	/*�� ������ ���̳ʸ� ���� ������ �˻��մϴ�. �˻�� �� ������ ���� Ÿ���� ������ŭ�� �˻��մϴ�. 2017.11.23�� �߰� �Լ�*/
	template <typename leftT, typename rightT > bool isSame(leftT &left, rightT right) {
		bool sw = sizeof(leftT) < sizeof(rightT);
		for (int i = 0, max = (sw) * sizeof(leftT) + (!sw) * sizeof(rightT); i < max; i ++ )
			if (*movePTR<char, char>(&left, i) != *movePTR<char, char>(&right, i))
				return false;
		return true;
	}
	/*main�� target�� processT���� ���� ��Ұ� Ʋ�� ������ ��ȯ�մϴ�. ��� ������ maxIndex�� ��ȯ�մϴ�. main�� target�� maxIndex���� ���ų� ���� �ε����� ������ �־�� �ϰ� processT�� mainT�� targetT���� �۰ų� ���� Ÿ���̾�� �մϴ�.*/
	template < typename processT, typename mainT, typename targetT > size_t numberOfSameIndex(mainT * main, targetT * target, size_t maxIndex) {
		processT * mainValue = (processT*)((void*)(main));
		processT * targetValue = (processT*)((void*)(target));
		size_t index;
		for (index = 0; index < maxIndex; index++) {
			mainValue = (processT*)((void*)(&main[index]));
			targetValue = (processT*)((void*)(&target[index]));
			/* ���� �ڵ�
			if ( !(*mainValue == *targetValue) ) // Ÿ�Ե��� == ������ �ؾ���.
				return index;
				*/
			if (!isSame(*mainValue, *targetValue)) //2017.11.23�� ����.
				break;
		}
		return index;
	}
	/*main���� target�� ����� �κ��� ���� �ε���(main����)�� �Ѱ��� (����Ʈ�� �ƴ϶� �迭�� �ε��� ������) ������, mainIndex�� ��ȯ*/
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
	/*left�� Ŭ��� -1 right�� Ŭ ��� +1 ���� ��� 0*/
	template < typename processT, typename leftT, typename rightT > int VS(leftT * left, size_t leftIndex, rightT * right, size_t rightIndex) {
		size_t passIndex = 0;
		bool sw = leftIndex < rightIndex;
		size_t maxIndex = sw * leftIndex + (!sw) * rightIndex;
		passIndex = numberOfSameIndex<processT>(left, right, maxIndex);
		processT * leftValue = &left[passIndex];
		processT * rightValue = &right[passIndex];
		return (leftValue[0] < rightValue[0]) - (leftValue[0] > rightValue[0]);
	}
	/*sizeof(T)�� ����̰ų� �ƴ� byte�� sizeof(T)�� ����� �˷��ݴϴ�. findAllocByte<int>(5)�� �� ��� 32��Ʈ ��ǻ�Ϳ����� 2�� ��ȯ�ϰ� 64��Ʈ ��ǻ�Ϳ����� 1�� ��ȯ�մϴ�.*/
	template < typename T > size_t findAllocByte(size_t byte) {
		size_t returnData = byte / sizeof(T);
		returnData += (byte > returnData * sizeof(T));
		return returnData * sizeof(T);
	}
}