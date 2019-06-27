#pragma once
#include "Frame.h"
class ReportFrame :
	public Frame
{
public:
	// 키 입력을 받을 스위치 입니다.
	short sw = 6;

	ReportFrame(LinkedList <long, Frame*>* frameList, PortalSystem2 *psAdapter, Array<Subject> * mainDB) : Frame(FRAME_REPORT, frameList, psAdapter, mainDB)
	{
		//0번 : 배경 오브젝트
		this->add(Object(0, 0, this->width, this->height, COL_MENUBG_RGB, COL_MENUBG_RGB, COL_MENUBG_RGB, true));
		//1번 : 메인 메뉴 오브젝트
		this->add(TextObject(0, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "메인 메뉴", strlen("메인 메뉴"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//2번 : 추가 메뉴 오브젝트
		this->add(TextObject(this->width / 6, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "과목 추가", strlen("과목 추가"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//3번 : 삭제 메뉴 오브젝트
		this->add(TextObject(this->width / 3, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "과목 삭제", strlen("과목 삭제"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//4번 : 보고서 출력 오브젝트
		this->add(TextObject(this->width / 2, 0, this->width / 6, this->height / 6, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_B, "보고서 출력", strlen("보고서 출력"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//5번 : 게임 메뉴 오브젝트
		this->add(TextObject(this->width * 2 / 3, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "게임", strlen("게임"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));

		//6번 : 파일 이름 오브젝트
		this->add(TextObject(this->width / 4, this->height / 3, this->width * 3 / 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "파일 이름을 입력하세요.", strlen("파일 이름을 입력하세요."), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//7번 : 파일 저장 오브젝트
		this->add(TextObject(this->width * 10 / 16, this->height * 4 / 12, this->width / 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "저장", strlen("메인"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
	}

	void buttonUpEvent(HWND hWnd, size_t index) {
		switch (index) {
		case 1:
		{
			this->animationClose(hWnd, this->otherFrame(FRAME_MAIN), true);
		}
		break;
		case 2:
		{
			this->animationClose(hWnd, this->otherFrame(FRAME_ADD), true);
		}
		break;
		case 3:
		{
			this->animationClose(hWnd, this->otherFrame(FRAME_DEL), true);
		}
		break;
		case 4:
		{
			this->animationClose(hWnd, this->otherFrame(FRAME_REPORT), true);
		}
		break;
		case 5:
		{
			this->animationClose(hWnd, this->otherFrame(FRAME_GAME), true);
		}
		break;
		case 6://이름 상자
		{
			pLen = 0;
			for (size_t i = 0, max = sizeof(this->data[index]->text); i < max; i++) {
				this->data[index]->text[i] = 0;
			}
		}
		break;
		case 7://저장 상자
		{
			pLen = 0;
			size_t strLen = strlen(this->data[sw]->text);
			size_t strSize = sizeof(this->data[sw]->text);
			bool sizeSW = strLen < strSize;// 널값이 있는 경우 strLen은 항상 strSize보다 작을 수 밖에 없음.
			Array<char> fileName(this->data[sw]->text, (sizeSW) * strLen + (!sizeSW)* strSize, 0);
			fileName += ".txt";
			((PortalSystem2*)this->psAdapter)->createReport(fileName.getData(), *this->mainDB);
		}
		break;
		}
	}
	/*키보드 입력시, 이전 문자열의 길이입니다.*/
	short pLen = 0;
	virtual void keyEvent(HWND hWnd, char c[3], bool finish) {
		this->data[sw]->currentLen -= pLen;
		pLen = strlen(c);
		this->data[sw]->text[this->data[sw]->currentLen] = c[0];
		this->data[sw]->text[this->data[sw]->currentLen + 1] = c[1];
		this->data[sw]->text[this->data[sw]->currentLen + 2] = c[2];
		this->data[sw]->currentLen = strlen(this->data[sw]->text);
		if (finish) {
			switch (c[0]) {
			case VK_BACK:
				if (this->data[sw]->currentLen > 1) {
					if (this->data[sw]->text[this->data[sw]->currentLen - 2] & 128) {
						this->data[sw]->currentLen -= 1;
					}
					this->data[sw]->currentLen -= 2;
				}
				else {
					this->data[sw]->currentLen -= 1;
				}
				break;
			case VK_RETURN:
			{
				this->data[sw]->currentLen -= 1;
				this->data[sw]->text[this->data[sw]->currentLen] = 0;
				this->buttonUpEvent(hWnd, 7);
			}
				break;
			case VK_TAB: 
			{
				this->data[sw]->currentLen -= 1;
				this->data[sw]->text[this->data[sw]->currentLen] = 0;
			}
				break;
			}
			pLen = 0;
		}
		SendMessage(hWnd, WM_TIMER, 1, 0);
	}

};

