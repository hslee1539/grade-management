#pragma once
//이 포털의 모든 배열을 관리하는 클래스를 추가합니다.
#include "Array1.h"
//이 포털의 모든 파일 입출력을 관리하는 클래스를 추가합니다.
#include "File.h"
#include <iostream>
//이 포털의 데이터 레코드 하나하나를 관리하는 클래스를 추가합니다.
#include "Subject.h"

/*
과목 데이터를 관리를 해주는 클래스입니다.
화면 출력은 PortalUI가 관리합니다.
내부 함수들이 구현이 됬지만, 가상 함수로 선언하여, 어뎁터 인터페이스로도 사용이 가능하게 구현했습니다.
필드 변수와 생성자가 없습니다.
*/
class PortalSystem1
{
protected:

public://파일 동기화 관련
	/*해당 파일 이름 파라메터에 데이터 파라메터 정보로 저장합니다.*/
	virtual void synchronizedFILE(  char* fileName , Array<Subject> & data) {
		File<Subject> file( new ofstream (fileName , ofstream::binary ));
		file.write(data, false);
	}
	/*해당 파일 이름에서 정보를 가지고 옵니다.*/
	virtual Array<Subject> synchronizedMemory(char * fileName) {
		File<Subject> file(new ifstream(fileName, ifstream::binary));
		return file.read();
	}
public://추가 삭제 관련
	/*해당 데이터를 메모리에 저장합니다.*/
	virtual bool add(Array<Subject> &target, Subject &data) { target = target + data; return true; }
	//해당 과목을 타겟에서 제거합니다. 가장 먼저 발견이 되는 곳을 1번만 합니다.
	virtual bool remove(Array<Subject> &target, Subject &subject) {
		target -= subject; return true;
	}

public://타겟의 인덱스 데이터를 limit만큼 가지고 와 반환합니다.
	virtual Array<Subject> makeList(Array<Subject> &target, size_t index, size_t limit) {
		size_t targetIndex = target.getByte() / sizeof(Subject);
		if (targetIndex < index + limit) {
			if (targetIndex < index)
				//표시 할 수 없어 만들지 않음
				limit = 0;
			else 
				//리미트를 전부 표시 할 수 없어 남은 부분만 리스트로 만듬
				limit = targetIndex - index;
		}
		Array<Subject> list;
		for (int i = 0; i < limit; i++) {
			list = list + target[i + index];
		}
		return list;
	}
	/*해당 데이터를 디비에서 모두 찾습니다.*/
	virtual Array<Subject> searchName(Array<Subject> &db, Array<char> &data) {
		Array<Subject> list;
		size_t searchByte = data.getByte() - data.getEoA();
		size_t maxIndex = db.getByte() / sizeof(Subject);
		size_t same;
		for (size_t i = 0; i < maxIndex; i++) {
			same = MemoryTool::numberOfSameIndex<char>((char*)db[i], data.getData(), searchByte);
			if (same == searchByte)
				list += db[i];
		}
		return list;
	}
	// 해당 요소로 subject를 만듭니다. 못만들 경우, 모든 요소가 0인 subject를 반환합니다.
	virtual Subject makeSubject(char * name, char credit, char *score) {
		Subject subject;// 자동으로 모든 요소가 0으로 초기화
		for (int i = 0; i < 10; i++) {
			if (name[i] == 0) {
				if ('0' - 1 < credit && credit < '9' + 1) {
					for (int ii = 0; ii < 3; ii++) {
						if (score[ii] == 0) {
							subject.reName(name, i);
							subject.credit = credit;
							subject.score[0] = score[0];
							subject.score[1] = score[1];
							subject.score[2] = score[2];
							break;
						}
					}
					break;
				}
			}
		}
		return subject;
	}
};

