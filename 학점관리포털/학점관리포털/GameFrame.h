#pragma once
#include "Frame.h"

class GamePlay {

};

class GameFrame :
	public Frame
{
private:
	// 6개의 카드가 독립적인 타임을 가지게 하는 변수입니다. 읽기는 상관 없지만 값을 수정할 때, 반듯이 아래 뮤택스를 사용 할 것.
	volatile int gameTimer[6];
	 
	// gameTimer의 값을 수정할 때 동기화 객체입니다.
	mutex gameTimerSync;
	volatile bool gameRunning = false;
	// 게임 스피드 입니다.
	int gameMS = 25;


	// 게임 쪽 메소드에서 사용되는 인덱스의 범위가 0~5인데, 이 숫자를 더해서 this->data[]의 인덱스로 사용하면 실제 화면에 보여지는 Object 클래스 객체의 번호가 됨. 변수로 따로 만든 이유는 간단히 더해서 사용하기 위해.
	const int cardStartIndex;

	// 클릭한 것을 보여주는 시간입니다. 즉 gameMS값과 이 값을 곱한 값 만큼 화면에 보여지게 됩니다.
	const int showTime = 40;

	// 6개의 카드 정보 배열입니다. 게임 인덱스의 번호가 들어 있습니다.
	int gameMap[6];
	// 6개의 카드 정보 배열로, 두개의 카드 찾기를 성공한 경우, 이 값을 true로 합니다.
	bool finsihMap[6];
protected:

	void gameINIT() {
		// 게임이 작동 중이면 먼저 종료 할 것.
		this->gameRunning = false;
		// 게임에서 사용될 메인 디비에서 랜덤으로 선택된 메인 디비 인덱스의 정보를 저장하는 배열입니다.
		int gameIndex[3];
		size_t dbIndex = this->mainDB->getIndex();
		time_t currentTime;
		time(&currentTime);
		//디비에 최소 3개의 자료가 있어야 함.
		if (dbIndex > 2) {
			char checkBit = 0;
			int finish_index = 0;
			int i = 0;
			// 랜덤하게 선택.
			srand(currentTime);
			gameIndex[0] = rand() % dbIndex;
			gameIndex[1] = rand() % dbIndex;
			gameIndex[2] = rand() % dbIndex;
			{// 2017-12-20일 추가코드로, 랜덤 한 것이 있는 숫자이면 다시 다를때 까지 반복함.
				while (gameIndex[0] == gameIndex[1]) {
					gameIndex[1] = rand() &dbIndex;
				}
				while (gameIndex[0] == gameIndex[2] || gameIndex[1] == gameIndex[2]) {
					gameIndex[2] = rand() & dbIndex;
				}
			}
			// 시그마 2의 n승이 n이 0부터 5이하 까지 하는 값과 같은지?
			while (checkBit != 63) {
				i = rand() % 6;
				// 2의 i승 숫자가 checkBit에 포함이 되어 있지 않으면, 실행
				// 총 6번만 참값을 가지고 아래 내용을 실행.
				if (!(checkBit & (1 << i))) {
					// 그러면 그 수를 포함 시킴.
					checkBit |= (1 << i);
					// 2의 i승이 없으므로, i인덱스에 gameIndex 에 finish_index를 줘서 저장.
					// gameIndex는 이전에 디비에서 랜덤으로 선택된 3개의 데이타 인덱스 번호가 저장되어 있음.
					// i와 finish_index는 독립적인 수임. (하나는 랜덤으로 값을 받고, 하나는 +1씩 증가)
					this->gameMap[i] = gameIndex[finish_index / 2];
					// 완성 정보를 초기화. (finsih_index)
					this->finsihMap[finish_index] = 0;
					finish_index++;
				}
			}
			thread t1(&GameFrame::gameThread, this);
			t1.detach();
		}
	}
	void gameThread() {
		size_t index;
		// true이면 해당 인덱스 카드의 
		Sleep(this->gameMS);
		bool textSW[6] = { this->gameTimer[0] > 0,this->gameTimer[1] > 0,this->gameTimer[2] > 0,this->gameTimer[3] > 0,this->gameTimer[4] > 0,this->gameTimer[5] > 0 };
		this->gameRunning = true;
		while (this->gameRunning ) {
			for (index = 0; index < 6; index++) {
				if (this->gameTimer[index] > 0) {
					if (textSW[index]) {
						MemoryTool::copyDeep<char>(this->data[this->cardStartIndex + index]->text, this->mainDB[0][gameMap[index]].name,sizeof(Subject::name));
						this->data[this->cardStartIndex + index]->currentLen = strlen (this->data[this->cardStartIndex + index]->text);
						textSW[index] = false;
					}
					this->gameTimerSync.lock();
					this->gameTimer[index] --;
					this->gameTimerSync.unlock();
				}
				else {
					if (!textSW[index] && !this->finsihMap[index]) {
						size_t strLen = strlen("hidden");
						MemoryTool::copyDeep<char>(this->data[ this->cardStartIndex + index]->text, "hidden" , strLen);
						this->data[this->cardStartIndex + index]->currentLen = strLen;
						textSW[index] = true;
					}
				}
			}
			this->gameRunning = this->visible;
			Sleep(this->gameMS);
		}
	}
	void gameInputThread() {
		
	}
public:

	GameFrame(LinkedList <long, Frame*>* frameList, PortalSystem2 *psAdapter, Array<Subject> * mainDB) : Frame(FRAME_GAME, frameList, psAdapter, mainDB),
		cardStartIndex ( 6) // text 오브젝트의 시작 값을 넣줄 것.
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
		this->add(TextObject(this->width / 2, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "보고서 출력", strlen("보고서 출력"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//5번 : 게임 메뉴 오브젝트
		this->add(TextObject(this->width * 2 / 3, 0, this->width / 6, this->height / 6, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_B, "게임", strlen("게임"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));

		//6번 : text1 오브젝트
		this->add(TextObject(this->width * 4 / 16, this->height *2/ 9, this->width * 2 / 16, this->height * 4 / 16, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "NoData", strlen("NoData"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//7번 : text1 오브젝트
		this->add(TextObject(this->width * 7 / 16, this->height *2/ 9, this->width * 2 / 16, this->height * 4 / 16, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "NoData", strlen("NoData"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//8번 : text1 오브젝트
		this->add(TextObject(this->width * 10 / 16, this->height *2/ 9, this->width * 2 / 16, this->height * 4 / 16, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "NoData", strlen("NoData"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//9번 : text1 오브젝트
		this->add(TextObject(this->width * 4 / 16, this->height *2/ 3, this->width * 2 / 16, this->height * 4 / 16, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "NoData", strlen("NoData"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//10번 : text1 오브젝트
		this->add(TextObject(this->width * 7 / 16, this->height *2/ 3, this->width * 2 / 16, this->height * 4 / 16, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "NoData", strlen("NoData"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//11번 : text1 오브젝트
		this->add(TextObject(this->width * 10 / 16, this->height *2/ 3, this->width * 2 / 16, this->height * 4 / 16, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "NoData", strlen("NoData"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));

	}
	void animationOpenEvent(HWND hWnd) {
		this->gameINIT();
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
			this->animationClose(hWnd, this, true);
		}
		break;
		}
		// 카드 오브젝트 처리
		if (this->cardStartIndex - 1 < index && index < this->cardStartIndex + 6) {
			long numberOfVisibleCard = 0;
			static int pCard = -1;
			this->gameTimerSync.lock();

			for (size_t i = 0; i < 6; i++) {
				numberOfVisibleCard += this->gameTimer[i] > 0;
			}
			if (numberOfVisibleCard > 1) {
				for (size_t i = 0, choose = 0; i < 6; i++) {
					this->gameTimer[i] = 0;
				}
			}
			else if (numberOfVisibleCard == 1) {
				if (index - this->cardStartIndex != pCard) { // 2017-12-20일 버그 픽스 : 같은 카드 선택시 조건에 만족 하는 경우를 배제함.
					if (this->gameMap[pCard] == this->gameMap[index - this->cardStartIndex]) {
						this->finsihMap[pCard] = true;
						this->finsihMap[index - this->cardStartIndex] = true;
					}
				}
			}
			this->gameTimer[index - this->cardStartIndex] += this->showTime;
			pCard = index - this->cardStartIndex;
			this->gameTimerSync.unlock();
		}
	}
};

