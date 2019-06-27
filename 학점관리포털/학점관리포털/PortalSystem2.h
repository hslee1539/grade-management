#pragma once
#include "PortalSystem1.h"
#include <shellapi.h>
#include <windows.h>
#include "Math.h"
// windows api�� ���˴ϴ�.
class PortalSystem2 :
	public PortalSystem1
{
public:
	bool add(Array<Subject> &target, Subject &data) { 
		if (data.name[0] != 0) {
			if ('0' - 1 < data.credit < '9' + 1) {
				if (data.score[0] != 0) {
					target = target + data;
					return true;
				}
			}
		}
		return false;
	}
	// subject���� ����,����,����,������ ������ rect�� ���ؼ� �˷��ݴϴ�.
	virtual bool makeCredit(Subject &subject, RECT * rect ) {
		if (subject.credit > '0' - 1 && subject.credit < '9' + 1) {
			switch (subject.code) {
			case ����:
				rect->top += subject.credit - '0';
				break;
			case ����:
				rect->left += subject.credit - '0';
				break;
			case ����:
				rect->right += subject.credit - '0';
				break;
			case ����:
				rect->bottom += subject.credit - '0';
				break;
			}
			return true;
		}
		return false;
	}
private:
	// subjectScore���� subject::score�� ���ּ���.
	// rectMember���� score���� RECT ������ �ʵ带 �ϳ��� ���ּ���/
	// �׷��� ������ �����ݴϴ�. (0.5 �� �Ҽ��� �־ * 10�� �ؼ� ����.)
	void _score(long &rectMember, char *subjectScore) {
		if (subjectScore[0] == 'A' || subjectScore[0] == 'a') {
			rectMember += 40;
		}
		else if (subjectScore[0] == 'B' || subjectScore[0] == 'b') {
			rectMember += 30;
		}
		else if (subjectScore[0] == 'C' || subjectScore[0] == 'c') {
			rectMember += 20;
		}
		if (subjectScore[1] == '+') {
			rectMember += 5;
		}
	}
public:
	virtual bool makeScore(Subject & subject, RECT * rect, RECT * count) {
		if (subject.credit > '0' - 1 && subject.credit < '9' + 1) {
			switch (subject.code) {
			case ����:
				this->_score(rect->top, subject.score);
				count->top++;
				break;
			case ����:
				this->_score(rect->left, subject.score);
				count->left++;
				break;
			case ����:
				this->_score(rect->right, subject.score);
				count->right++;
				break;
			case ����:
				this->_score(rect->bottom, subject.score);
				count->bottom++;
				break;
			}
			return true;
		}
		return false;
	}
	
	// subjectList���� ���� ���� ���� ���� ������ rect�� ���ؼ� �˷��ݴϴ�. ���� subject�� ���� ���ڵ忡 ���ڹ��ڰ� �ƴ� ��� �ߴ��ϰ� fasle�� ��ȯ�մϴ�.
	virtual bool makeCredit(Array<Subject > & subjectArray, RECT * rect) {
		for (size_t i = 0, max = subjectArray.getIndex(); i < max; i++) {
			if (!this->makeCredit(subjectArray[i], rect)) {
				return false;
			}
		}
		return true;
	}
	// �� �Լ��� ��Ƽ����Ʈ �������� ó���մϴ�.
	virtual bool createReport(char * fileName, Array<Subject> &subjectArray) {
		File<char> file(new ofstream(fileName));// �ؽ�Ʈ ���� ����
		Array<char> buffer;// ���Ͽ� ����� �޸� �����Դϴ�. ��� ���� IO�� ���� �ʰ� �޸𸮿��� ������ ó���ϰ� �ѹ��� �ϱ� ���ؼ��Դϴ�. (������ �ϱ� ���ؼ�)
		//short credit[4] = { 0,0,0,0 };// code�� ����, ������ �������� ����.
		RECT credit = {0,0,0,0};
		RECT score_count = { 0,0,0,0 };
		RECT score = { 0,0,0,0 };
		char enter = '\n';
		//�̰��� ������ �պκ��� ������ �����մϴ�.
		{
			Array<char> front("���� ���� ��Ȳ�Դϴ�.");
			front += enter;
			buffer+= front + enter;
		}
		// �̰��� subjectArr�� ���ڵ带 ���Ŀ� �°� �����մϴ�.
		for (size_t i = 0, max = subjectArray.getIndex(); i < max; i++) {
			size_t processLen = strlen(subjectArray[i].name) + 1;
			size_t sizeOf = sizeof(subjectArray[i].name);
			processLen *=  processLen<sizeOf;// ���� Subject::name�� ũ�⺸�� ũ�� ���ǹ��� ���� 0�� �ǰ� �̰��� �������� 0�� ��.
			processLen += (processLen == 0) * sizeOf;// ���� ���� 0�̶��, Subject::name�� ũ�⸦ ����.
			Array<char> subjectName(subjectArray[i].name , processLen , processLen != sizeOf);
			Array<char> spaceBarName("                                                                       ", sizeOf-processLen, 0);// ��, ������� �����̽��ٸ� �ִ´�.
			Array<char> subjectCredit(subjectArray[i].credit);
			makeCredit(subjectArray[i], &credit);
			makeScore(subjectArray[i], &score, &score_count);
			//credit[subjectArray[i].code & 3] += subjectArray[i].credit - '0';// code�� 3�� &�� ���� ��, 2����Ʈ ������ ���ؼ� ������ �з��ؼ� ����.
			Array<char> spaceBarCredit("          ");
			char c[2] = { subjectArray[i].score[0] , subjectArray[i].score[1] };
			Array<char> subjectScore(c);//
			Array<char> spaceBarScore("      ");
			switch (subjectArray[i].code & 3) {
			case ����:
				spaceBarScore += Array<char>("����");
				break;
			case ����:
				spaceBarScore += Array<char>("����");
				break;
			case ����:
				spaceBarScore += Array<char>("����");
				break;
			case ����:
				spaceBarScore += Array<char>("����");
				break;
			}
			buffer += subjectName + spaceBarName + subjectCredit + spaceBarCredit + subjectScore + spaceBarScore + enter;
		}
		buffer += enter;
		//������ ������ �κ��Դϴ�.

		//�� ���� ���
		{
			Array<char> arr[4]; 
			for (int i = 0; i < 4; i++) {
				switch (i & 3) {
				case ����:
					arr[i] += Array<char>("����") + Math::toString (credit.top,10) + "   ";
					break;
				case ����:
					arr[i] += Array<char>("����") + Math::toString ( credit.left , 10 ) + "   ";
					break;
				case ����:
					arr[i] += Array<char>("����") + Math::toString ( credit.bottom , 10 ) + "   ";
					break;
				case ����:
					arr[i] += Array<char>("����") + Math::toString ( credit.right , 10 ) + "   ";
					break;
				}
			} 
			buffer += arr[0] + arr[1] + arr[2] + arr[3] + enter;
		}
		// ���� ���� ���
		{
			Array<char> arr[4];
			RECT studentID2014 = { 36, 45,27,15 };
			for (int i = 0; i < 4; i++) {
				switch (i & 3) {
				case ����:
					arr[i] += Array<char>("����") + Math::toString(studentID2014.top - credit.top, 10) + "   ";
					break;
				case ����:
					arr[i] += Array<char>("����") + Math::toString(studentID2014.left - credit.left, 10) + "   ";
					break;
				case ����:
					arr[i] += Array<char>("����") + Math::toString(studentID2014.bottom - credit.bottom, 10) + "   ";
					break;
				case ����:
					arr[i] += Array<char>("����") + Math::toString(studentID2014.right - credit.right, 10) + "   ";
					break;
				}
			}
			buffer += enter;
			buffer += "2014�й��� ������ �̸�ŭ �� ���� �մϴ�.";
			buffer += enter;
			buffer += arr[0] + arr[1] + arr[2] + arr[3] + enter + enter;
		}
		// ��� ���� ���.
		{
			Array<char> arr[4];
			for (int i = 0; i < 4; i++) {
				switch (i & 3) {
				case ����:
					if (!score_count.top) score_count.top = 1;
					arr[i] += Array<char>("����") + Math::toString(score.top / (score_count.top), 10) + "   ";
					break;
				case ����:
					if (!score_count.left) score_count.left = 1;
					arr[i] += Array<char>("����") + Math::toString(score.left / (score_count.left), 10) + "   ";
					break;
				case ����:
					if (!score_count.bottom) score_count.bottom = 1;
					arr[i] += Array<char>("����") + Math::toString(score.bottom / (score_count.bottom), 10) + "   ";
					break;
				case ����:
					if (!score_count.right) score_count.right = 1;
					arr[i] += Array<char>("����") + Math::toString(score.right / (score_count.right), 10) + "   ";
					break;
				}
			}
			buffer += "��������� (10�� ����� ��)";
			buffer += enter;
			buffer += arr[0] + arr[1] + arr[2] + arr[3] + enter;
		}


		// �ѹ��� ���Ͽ� ����.
		// append ��ҿ� false�� �����Ƿ�, ���� ��Ʈ���� ������ ������ �̵��� ���縦 ��. (��, ����Ⱑ �ƴ��� ����!)
		file.write(buffer, false);
		ShellExecuteA(NULL, "open", "notepad", fileName, NULL, SW_SHOW);
		return true;
	}
	virtual Subject makeSubject(char * name, char credit, char *score , char code) {
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
							subject.code = code;
							subject.null = 0;
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


