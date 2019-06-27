#pragma once
//�� ������ ��� �迭�� �����ϴ� Ŭ������ �߰��մϴ�.
#include "Array1.h"
//�� ������ ��� ���� ������� �����ϴ� Ŭ������ �߰��մϴ�.
#include "File.h"
#include <iostream>
//�� ������ ������ ���ڵ� �ϳ��ϳ��� �����ϴ� Ŭ������ �߰��մϴ�.
#include "Subject.h"

/*
���� �����͸� ������ ���ִ� Ŭ�����Դϴ�.
ȭ�� ����� PortalUI�� �����մϴ�.
���� �Լ����� ������ ������, ���� �Լ��� �����Ͽ�, ��� �������̽��ε� ����� �����ϰ� �����߽��ϴ�.
�ʵ� ������ �����ڰ� �����ϴ�.
*/
class PortalSystem1
{
protected:

public://���� ����ȭ ����
	/*�ش� ���� �̸� �Ķ���Ϳ� ������ �Ķ���� ������ �����մϴ�.*/
	virtual void synchronizedFILE(  char* fileName , Array<Subject> & data) {
		File<Subject> file( new ofstream (fileName , ofstream::binary ));
		file.write(data, false);
	}
	/*�ش� ���� �̸����� ������ ������ �ɴϴ�.*/
	virtual Array<Subject> synchronizedMemory(char * fileName) {
		File<Subject> file(new ifstream(fileName, ifstream::binary));
		return file.read();
	}
public://�߰� ���� ����
	/*�ش� �����͸� �޸𸮿� �����մϴ�.*/
	virtual bool add(Array<Subject> &target, Subject &data) { target = target + data; return true; }
	//�ش� ������ Ÿ�ٿ��� �����մϴ�. ���� ���� �߰��� �Ǵ� ���� 1���� �մϴ�.
	virtual bool remove(Array<Subject> &target, Subject &subject) {
		target -= subject; return true;
	}

public://Ÿ���� �ε��� �����͸� limit��ŭ ������ �� ��ȯ�մϴ�.
	virtual Array<Subject> makeList(Array<Subject> &target, size_t index, size_t limit) {
		size_t targetIndex = target.getByte() / sizeof(Subject);
		if (targetIndex < index + limit) {
			if (targetIndex < index)
				//ǥ�� �� �� ���� ������ ����
				limit = 0;
			else 
				//����Ʈ�� ���� ǥ�� �� �� ���� ���� �κи� ����Ʈ�� ����
				limit = targetIndex - index;
		}
		Array<Subject> list;
		for (int i = 0; i < limit; i++) {
			list = list + target[i + index];
		}
		return list;
	}
	/*�ش� �����͸� ��񿡼� ��� ã���ϴ�.*/
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
	// �ش� ��ҷ� subject�� ����ϴ�. ������ ���, ��� ��Ұ� 0�� subject�� ��ȯ�մϴ�.
	virtual Subject makeSubject(char * name, char credit, char *score) {
		Subject subject;// �ڵ����� ��� ��Ұ� 0���� �ʱ�ȭ
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

