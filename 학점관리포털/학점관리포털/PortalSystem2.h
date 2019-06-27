#pragma once
#include "PortalSystem1.h"
#include <shellapi.h>
#include <windows.h>
#include "Math.h"
// windows api가 사용됩니다.
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
	// subject에서 전필,전선,교필,교선의 학점를 rect에 더해서 알려줍니다.
	virtual bool makeCredit(Subject &subject, RECT * rect ) {
		if (subject.credit > '0' - 1 && subject.credit < '9' + 1) {
			switch (subject.code) {
			case 전선:
				rect->top += subject.credit - '0';
				break;
			case 전필:
				rect->left += subject.credit - '0';
				break;
			case 교선:
				rect->right += subject.credit - '0';
				break;
			case 교필:
				rect->bottom += subject.credit - '0';
				break;
			}
			return true;
		}
		return false;
	}
private:
	// subjectScore에는 subject::score를 넣주세요.
	// rectMember에는 score관련 RECT 변수의 필드를 하나를 넣주세요/
	// 그러면 학점을 더해줍니다. (0.5 즉 소수가 있어서 * 10을 해서 더함.)
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
			case 전선:
				this->_score(rect->top, subject.score);
				count->top++;
				break;
			case 전필:
				this->_score(rect->left, subject.score);
				count->left++;
				break;
			case 교선:
				this->_score(rect->right, subject.score);
				count->right++;
				break;
			case 교필:
				this->_score(rect->bottom, subject.score);
				count->bottom++;
				break;
			}
			return true;
		}
		return false;
	}
	
	// subjectList에서 전필 전선 교필 교선 순으로 rect에 더해서 알려줍니다. 만약 subject에 학점 레코드에 숫자문자가 아닐 경우 중단하고 fasle를 반환합니다.
	virtual bool makeCredit(Array<Subject > & subjectArray, RECT * rect) {
		for (size_t i = 0, max = subjectArray.getIndex(); i < max; i++) {
			if (!this->makeCredit(subjectArray[i], rect)) {
				return false;
			}
		}
		return true;
	}
	// 이 함수는 멀티바이트 기준으로 처리합니다.
	virtual bool createReport(char * fileName, Array<Subject> &subjectArray) {
		File<char> file(new ofstream(fileName));// 텍스트 모드로 열기
		Array<char> buffer;// 파일에 출력할 메모리 버퍼입니다. 계속 파일 IO를 하지 않고 메모리에서 모든것을 처리하고 한번에 하기 위해서입니다. (빠르게 하기 위해서)
		//short credit[4] = { 0,0,0,0 };// code에 따라서, 각각의 점수들을 저장.
		RECT credit = {0,0,0,0};
		RECT score_count = { 0,0,0,0 };
		RECT score = { 0,0,0,0 };
		char enter = '\n';
		//이곳은 파일의 앞부분을 내용을 정의합니다.
		{
			Array<char> front("현재 과목 현황입니다.");
			front += enter;
			buffer+= front + enter;
		}
		// 이곳은 subjectArr의 레코드를 형식에 맞게 정의합니다.
		for (size_t i = 0, max = subjectArray.getIndex(); i < max; i++) {
			size_t processLen = strlen(subjectArray[i].name) + 1;
			size_t sizeOf = sizeof(subjectArray[i].name);
			processLen *=  processLen<sizeOf;// 만약 Subject::name의 크기보다 크면 조건문에 의해 0이 되고 이것을 곱해지면 0이 됨.
			processLen += (processLen == 0) * sizeOf;// 위에 의해 0이라면, Subject::name의 크기를 가짐.
			Array<char> subjectName(subjectArray[i].name , processLen , processLen != sizeOf);
			Array<char> spaceBarName("                                                                       ", sizeOf-processLen, 0);// 즉, 빈공간에 스페이스바를 넣는다.
			Array<char> subjectCredit(subjectArray[i].credit);
			makeCredit(subjectArray[i], &credit);
			makeScore(subjectArray[i], &score, &score_count);
			//credit[subjectArray[i].code & 3] += subjectArray[i].credit - '0';// code는 3과 &한 영역 즉, 2바이트 영역에 대해서 학점을 분류해서 더함.
			Array<char> spaceBarCredit("          ");
			char c[2] = { subjectArray[i].score[0] , subjectArray[i].score[1] };
			Array<char> subjectScore(c);//
			Array<char> spaceBarScore("      ");
			switch (subjectArray[i].code & 3) {
			case 전선:
				spaceBarScore += Array<char>("전선");
				break;
			case 전필:
				spaceBarScore += Array<char>("전필");
				break;
			case 교선:
				spaceBarScore += Array<char>("교선");
				break;
			case 교필:
				spaceBarScore += Array<char>("교필");
				break;
			}
			buffer += subjectName + spaceBarName + subjectCredit + spaceBarCredit + subjectScore + spaceBarScore + enter;
		}
		buffer += enter;
		//파일의 마지막 부분입니다.

		//총 학점 출력
		{
			Array<char> arr[4]; 
			for (int i = 0; i < 4; i++) {
				switch (i & 3) {
				case 전선:
					arr[i] += Array<char>("전선") + Math::toString (credit.top,10) + "   ";
					break;
				case 전필:
					arr[i] += Array<char>("전필") + Math::toString ( credit.left , 10 ) + "   ";
					break;
				case 교선:
					arr[i] += Array<char>("교선") + Math::toString ( credit.bottom , 10 ) + "   ";
					break;
				case 교필:
					arr[i] += Array<char>("교필") + Math::toString ( credit.right , 10 ) + "   ";
					break;
				}
			} 
			buffer += arr[0] + arr[1] + arr[2] + arr[3] + enter;
		}
		// 남은 학점 출력
		{
			Array<char> arr[4];
			RECT studentID2014 = { 36, 45,27,15 };
			for (int i = 0; i < 4; i++) {
				switch (i & 3) {
				case 전선:
					arr[i] += Array<char>("전선") + Math::toString(studentID2014.top - credit.top, 10) + "   ";
					break;
				case 전필:
					arr[i] += Array<char>("전필") + Math::toString(studentID2014.left - credit.left, 10) + "   ";
					break;
				case 교선:
					arr[i] += Array<char>("교선") + Math::toString(studentID2014.bottom - credit.bottom, 10) + "   ";
					break;
				case 교필:
					arr[i] += Array<char>("교필") + Math::toString(studentID2014.right - credit.right, 10) + "   ";
					break;
				}
			}
			buffer += enter;
			buffer += "2014학번은 앞으로 이만큼 더 들어야 합니다.";
			buffer += enter;
			buffer += arr[0] + arr[1] + arr[2] + arr[3] + enter + enter;
		}
		// 평균 점수 출력.
		{
			Array<char> arr[4];
			for (int i = 0; i < 4; i++) {
				switch (i & 3) {
				case 전선:
					if (!score_count.top) score_count.top = 1;
					arr[i] += Array<char>("전선") + Math::toString(score.top / (score_count.top), 10) + "   ";
					break;
				case 전필:
					if (!score_count.left) score_count.left = 1;
					arr[i] += Array<char>("전필") + Math::toString(score.left / (score_count.left), 10) + "   ";
					break;
				case 교선:
					if (!score_count.bottom) score_count.bottom = 1;
					arr[i] += Array<char>("교선") + Math::toString(score.bottom / (score_count.bottom), 10) + "   ";
					break;
				case 교필:
					if (!score_count.right) score_count.right = 1;
					arr[i] += Array<char>("교필") + Math::toString(score.right / (score_count.right), 10) + "   ";
					break;
				}
			}
			buffer += "평균점수는 (10배 적용된 값)";
			buffer += enter;
			buffer += arr[0] + arr[1] + arr[2] + arr[3] + enter;
		}


		// 한번에 파일에 쓰기.
		// append 요소에 false를 줬으므로, 파일 스트림을 파일의 앞으로 이동후 복사를 함. (단, 지우기가 아님을 주의!)
		file.write(buffer, false);
		ShellExecuteA(NULL, "open", "notepad", fileName, NULL, SW_SHOW);
		return true;
	}
	virtual Subject makeSubject(char * name, char credit, char *score , char code) {
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


