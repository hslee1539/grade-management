#pragma once
#include "Frame.h"
//#include "Engine.h"
#include "Define.h"

class MainFrame :
	public Frame

{
public:
	size_t list_index;
	Array<Subject> list;
	MainFrame(LinkedList <long, Frame*>* frameList, PortalSystem2 *psAdapter, Array<Subject> * mainDB) : Frame(FRAME_MAIN,frameList,psAdapter,mainDB)
	{
		list_index = 0;
		//0�� : ��� ������Ʈ
		this->add(Object(0, 0, this->width, this->height , COL_MENUBG_RGB, COL_MENUBG_RGB, COL_MENUBG_RGB, true));
		//1�� : ���� �޴� ������Ʈ
		this->add(TextObject(0, 0, this->width / 6, this->height / 6, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_B, "���� �޴�", strlen("���� �޴�"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//2�� : �߰� �޴� ������Ʈ
		this->add(TextObject(this->width / 6, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "���� �߰�", strlen("���� �߰�"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//3�� : ���� �޴� ������Ʈ
		this->add(TextObject(this->width / 3, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "���� ����", strlen("���� ����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//4�� : ���� ��� ������Ʈ
		this->add(TextObject(this->width / 2, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "���� ���", strlen("���� ���"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//5�� : ���� �޴� ������Ʈ
		this->add(TextObject(this->width *2/ 3, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "����", strlen("����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));

		//6�� : text1_name ������Ʈ
		this->add(TextObject(this->width / 4, this->height / 3, this->width *3 / 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//7�� : text1_grade ������Ʈ
		this->add(TextObject(this->width *5/ 8, this->height / 3, this->width  / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//8�� : text1_score ������Ʈ
		this->add(TextObject(this->width *11/ 16, this->height / 3, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//9�� : text2 ������Ʈ
		this->add(TextObject(this->width / 4, this->height * 5 / 12, this->width *3/ 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//10�� : text2 ������Ʈ
		this->add(TextObject(this->width *5/ 8, this->height * 5 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//11�� : text2 ������Ʈ
		this->add(TextObject(this->width *11/ 16, this->height * 5 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//12�� : text3 ������Ʈ
		this->add(TextObject(this->width / 4, this->height * 6 / 12, this->width *3/ 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//13�� : text3 ������Ʈ
		this->add(TextObject(this->width *5/ 8, this->height * 6 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//14�� : text3 ������Ʈ
		this->add(TextObject(this->width *11/ 16, this->height * 6 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//15�� : text4 ������Ʈ
		this->add(TextObject(this->width / 4, this->height * 7 / 12, this->width *3/ 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//16�� : text4 ������Ʈ
		this->add(TextObject(this->width *5/ 8, this->height * 7 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//17�� : text4 ������Ʈ
		this->add(TextObject(this->width *11/ 16, this->height * 7 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//18�� : ���� �޴� ������Ʈ
		this->add(TextObject(this->width *3/ 4, this->height / 3, this->width / 16, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "����", strlen("����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//19�� : ���� �޴� ������Ʈ
		this->add(TextObject(this->width * 3 / 4, this->height / 2, this->width / 16, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "����", strlen("����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));

		//20�� : ���� ������Ʈ
		this->add(TextObject(this->width * 4 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "����", strlen("����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//21�� : ����_ ������Ʈ
		this->add(TextObject(this->width * 5 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//22�� : ���� ������Ʈ
		this->add(TextObject(this->width * 6 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "����", strlen("����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//23�� : ����_ ������Ʈ
		this->add(TextObject(this->width * 7 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//24�� : ���� ������Ʈ
		this->add(TextObject(this->width * 8 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "����", strlen("����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//25�� : ����_ ������Ʈ
		this->add(TextObject(this->width * 9 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//26�� : ���� ������Ʈ
		this->add(TextObject(this->width * 10 / 16, this->height *9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "����", strlen("����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//27�� : ����_ ������Ʈ
		this->add(TextObject(this->width * 11 / 16, this->height *9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));


	}
	void buttonUpEvent(HWND hWnd,size_t index) {
		switch (index) {
		case 1:
		{
			this->animationClose(hWnd, this, true);
		}
		break;
		case 2:
		{
			this->animationClose(hWnd, this->otherFrame(FRAME_ADD),true);
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
		case 18:
		{
			if (this->list_index) {
				this->list_index--;
				this->synchronized();
			}
		}
		break;
		case 19:
			if (this->list_index+4 < this->mainDB->getByte()/sizeof(Subject)) {
				this->list_index++;
				this->synchronized();
			}
		}
	}
	void animationOpenEvent(HWND hWnd) {
		this->synchronized();
	}
	mutex sync;
	void synchronized() {
		sync.lock(); // �̳༮�� �Ʒ��� �����尡 ����Ǹ� ���� ������.
		
		thread t1(&MainFrame::synchronized_, this);
		t1.detach();
		sync.unlock();
	}
	void synchronized_() {
		sync.lock();
		static size_t preLen = 0;
		this->list = this->psAdapter->makeList(*this->mainDB, this->list_index, 4);
		RECT credit = {0,0,0,0};
		// ���� mainDB �迭�� ���̰� ���ϸ� ������ ��. (���� ��� �ϴ� �κ�)
		if (preLen != this->mainDB->getByte()) {
			this->psAdapter->makeCredit(*this->mainDB, &credit);
			Array<char> tmp = Math::toString(credit.left, 10);
			MemoryTool::copyDeep<char>(this->data[21]->text, tmp.getData(), tmp.getByte());
			tmp = Math::toString(credit.top, 10);
			MemoryTool::copyDeep<char>(this->data[23]->text, tmp.getData(), tmp.getByte());
			tmp = Math::toString(credit.right, 10);
			MemoryTool::copyDeep<char>(this->data[25]->text, tmp.getData(), tmp.getByte());
			tmp = Math::toString(credit.bottom, 10);
			MemoryTool::copyDeep<char>(this->data[27]->text, tmp.getData(), tmp.getByte());
			preLen = this->mainDB->getByte();
		}


		int ob_i = 6;
		int ob_max = 17;
		if ( this->list.getData() != nullptr )
			for (int i = 0, max = this->list.getIndex(); i < max; i++) {
				//while (ob_i < ob_max) {
					MemoryTool::copyDeep<char>(this->data[ob_i]->text, this->list[i].name, sizeof(this->list[i].name));
					this->data[ob_i]->currentLen = strlen(this->data[ob_i]->text);
					this->data[++ob_i]->text[0] = this->list[i].credit;
					this->data[ob_i]->text[1] = 0;
					this->data[ob_i]->currentLen = 1;
					MemoryTool::copyDeep<char>(this->data[++ob_i]->text, this->list[i].score, 3);
					this->data[ob_i]->currentLen = strlen(this->data[ob_i]->text);
					ob_i++;
				//}
			}
		while (ob_i < ob_max) {//���� ������ ���̸� 0���� ��. (��, TextOut�� ��� ���̸� 0���� �ϹǷ� ���� ��� ����.)
			this->data[ob_i++]->currentLen = 0;
			this->data[ob_i++]->currentLen = 0;
			this->data[ob_i++]->currentLen = 0;
		}
		sync.unlock();
	}
};

