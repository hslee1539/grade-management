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
		//0번 : 배경 오브젝트
		this->add(Object(0, 0, this->width, this->height , COL_MENUBG_RGB, COL_MENUBG_RGB, COL_MENUBG_RGB, true));
		//1번 : 메인 메뉴 오브젝트
		this->add(TextObject(0, 0, this->width / 6, this->height / 6, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_B, "메인 메뉴", strlen("메인 메뉴"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//2번 : 추가 메뉴 오브젝트
		this->add(TextObject(this->width / 6, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "과목 추가", strlen("과목 추가"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//3번 : 삭제 메뉴 오브젝트
		this->add(TextObject(this->width / 3, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "과목 삭제", strlen("과목 삭제"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//4번 : 보고서 출력 오브젝트
		this->add(TextObject(this->width / 2, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "보고서 출력", strlen("보고서 출력"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//5번 : 게임 메뉴 오브젝트
		this->add(TextObject(this->width *2/ 3, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "게임", strlen("게임"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));

		//6번 : text1_name 오브젝트
		this->add(TextObject(this->width / 4, this->height / 3, this->width *3 / 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//7번 : text1_grade 오브젝트
		this->add(TextObject(this->width *5/ 8, this->height / 3, this->width  / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//8번 : text1_score 오브젝트
		this->add(TextObject(this->width *11/ 16, this->height / 3, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//9번 : text2 오브젝트
		this->add(TextObject(this->width / 4, this->height * 5 / 12, this->width *3/ 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//10번 : text2 오브젝트
		this->add(TextObject(this->width *5/ 8, this->height * 5 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//11번 : text2 오브젝트
		this->add(TextObject(this->width *11/ 16, this->height * 5 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//12번 : text3 오브젝트
		this->add(TextObject(this->width / 4, this->height * 6 / 12, this->width *3/ 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//13번 : text3 오브젝트
		this->add(TextObject(this->width *5/ 8, this->height * 6 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//14번 : text3 오브젝트
		this->add(TextObject(this->width *11/ 16, this->height * 6 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//15번 : text4 오브젝트
		this->add(TextObject(this->width / 4, this->height * 7 / 12, this->width *3/ 8, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//16번 : text4 오브젝트
		this->add(TextObject(this->width *5/ 8, this->height * 7 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//17번 : text4 오브젝트
		this->add(TextObject(this->width *11/ 16, this->height * 7 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//18번 : 이전 메뉴 오브젝트
		this->add(TextObject(this->width *3/ 4, this->height / 3, this->width / 16, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "이전", strlen("메인"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//19번 : 다음 메뉴 오브젝트
		this->add(TextObject(this->width * 3 / 4, this->height / 2, this->width / 16, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "다음", strlen("메인"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));

		//20번 : 전필 오브젝트
		this->add(TextObject(this->width * 4 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "전필", strlen("메인"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//21번 : 전필_ 오브젝트
		this->add(TextObject(this->width * 5 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//22번 : 전선 오브젝트
		this->add(TextObject(this->width * 6 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "전선", strlen("메인"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//23번 : 전선_ 오브젝트
		this->add(TextObject(this->width * 7 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//24번 : 교필 오브젝트
		this->add(TextObject(this->width * 8 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "교필", strlen("메인"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//25번 : 교필_ 오브젝트
		this->add(TextObject(this->width * 9 / 16, this->height * 9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "", 1, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//26번 : 교필 오브젝트
		this->add(TextObject(this->width * 10 / 16, this->height *9 / 12, this->width / 16, this->height / 12, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "교선", strlen("메인"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//27번 : 교필_ 오브젝트
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
		sync.lock(); // 이녀석은 아래의 스래드가 종료되면 락을 해제함.
		
		thread t1(&MainFrame::synchronized_, this);
		t1.detach();
		sync.unlock();
	}
	void synchronized_() {
		sync.lock();
		static size_t preLen = 0;
		this->list = this->psAdapter->makeList(*this->mainDB, this->list_index, 4);
		RECT credit = {0,0,0,0};
		// 만약 mainDB 배열에 길이가 변하면 갱신을 함. (학점 계산 하는 부분)
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
		while (ob_i < ob_max) {//남은 영역은 길이를 0으로 함. (즉, TextOut의 출력 길이를 0으로 하므로 글자 출력 안함.)
			this->data[ob_i++]->currentLen = 0;
			this->data[ob_i++]->currentLen = 0;
			this->data[ob_i++]->currentLen = 0;
		}
		sync.unlock();
	}
};

