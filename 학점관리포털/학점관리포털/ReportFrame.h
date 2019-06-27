#pragma once
#include "Frame.h"
class ReportFrame :
	public Frame
{
public:
	// Ű �Է��� ���� ����ġ �Դϴ�.
	short sw = 6;

	ReportFrame(LinkedList <long, Frame*>* frameList, PortalSystem2 *psAdapter, Array<Subject> * mainDB) : Frame(FRAME_REPORT, frameList, psAdapter, mainDB)
	{
		//0�� : ��� ������Ʈ
		this->add(Object(0, 0, this->width, this->height, COL_MENUBG_RGB, COL_MENUBG_RGB, COL_MENUBG_RGB, true));
		//1�� : ���� �޴� ������Ʈ
		this->add(TextObject(0, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "���� �޴�", strlen("���� �޴�"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//2�� : �߰� �޴� ������Ʈ
		this->add(TextObject(this->width / 6, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "���� �߰�", strlen("���� �߰�"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//3�� : ���� �޴� ������Ʈ
		this->add(TextObject(this->width / 3, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "���� ����", strlen("���� ����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//4�� : ���� ��� ������Ʈ
		this->add(TextObject(this->width / 2, 0, this->width / 6, this->height / 6, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_B, "���� ���", strlen("���� ���"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//5�� : ���� �޴� ������Ʈ
		this->add(TextObject(this->width * 2 / 3, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "����", strlen("����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));

		//6�� : ���� �̸� ������Ʈ
		this->add(TextObject(this->width / 4, this->height / 3, this->width * 3 / 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "���� �̸��� �Է��ϼ���.", strlen("���� �̸��� �Է��ϼ���."), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//7�� : ���� ���� ������Ʈ
		this->add(TextObject(this->width * 10 / 16, this->height * 4 / 12, this->width / 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "����", strlen("����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
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
		case 6://�̸� ����
		{
			pLen = 0;
			for (size_t i = 0, max = sizeof(this->data[index]->text); i < max; i++) {
				this->data[index]->text[i] = 0;
			}
		}
		break;
		case 7://���� ����
		{
			pLen = 0;
			size_t strLen = strlen(this->data[sw]->text);
			size_t strSize = sizeof(this->data[sw]->text);
			bool sizeSW = strLen < strSize;// �ΰ��� �ִ� ��� strLen�� �׻� strSize���� ���� �� �ۿ� ����.
			Array<char> fileName(this->data[sw]->text, (sizeSW) * strLen + (!sizeSW)* strSize, 0);
			fileName += ".txt";
			((PortalSystem2*)this->psAdapter)->createReport(fileName.getData(), *this->mainDB);
		}
		break;
		}
	}
	/*Ű���� �Է½�, ���� ���ڿ��� �����Դϴ�.*/
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

