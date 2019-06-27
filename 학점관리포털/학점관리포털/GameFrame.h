#pragma once
#include "Frame.h"

class GamePlay {

};

class GameFrame :
	public Frame
{
private:
	// 6���� ī�尡 �������� Ÿ���� ������ �ϴ� �����Դϴ�. �б�� ��� ������ ���� ������ ��, �ݵ��� �Ʒ� ���ý��� ��� �� ��.
	volatile int gameTimer[6];
	 
	// gameTimer�� ���� ������ �� ����ȭ ��ü�Դϴ�.
	mutex gameTimerSync;
	volatile bool gameRunning = false;
	// ���� ���ǵ� �Դϴ�.
	int gameMS = 25;


	// ���� �� �޼ҵ忡�� ���Ǵ� �ε����� ������ 0~5�ε�, �� ���ڸ� ���ؼ� this->data[]�� �ε����� ����ϸ� ���� ȭ�鿡 �������� Object Ŭ���� ��ü�� ��ȣ�� ��. ������ ���� ���� ������ ������ ���ؼ� ����ϱ� ����.
	const int cardStartIndex;

	// Ŭ���� ���� �����ִ� �ð��Դϴ�. �� gameMS���� �� ���� ���� �� ��ŭ ȭ�鿡 �������� �˴ϴ�.
	const int showTime = 40;

	// 6���� ī�� ���� �迭�Դϴ�. ���� �ε����� ��ȣ�� ��� �ֽ��ϴ�.
	int gameMap[6];
	// 6���� ī�� ���� �迭��, �ΰ��� ī�� ã�⸦ ������ ���, �� ���� true�� �մϴ�.
	bool finsihMap[6];
protected:

	void gameINIT() {
		// ������ �۵� ���̸� ���� ���� �� ��.
		this->gameRunning = false;
		// ���ӿ��� ���� ���� ��񿡼� �������� ���õ� ���� ��� �ε����� ������ �����ϴ� �迭�Դϴ�.
		int gameIndex[3];
		size_t dbIndex = this->mainDB->getIndex();
		time_t currentTime;
		time(&currentTime);
		//��� �ּ� 3���� �ڷᰡ �־�� ��.
		if (dbIndex > 2) {
			char checkBit = 0;
			int finish_index = 0;
			int i = 0;
			// �����ϰ� ����.
			srand(currentTime);
			gameIndex[0] = rand() % dbIndex;
			gameIndex[1] = rand() % dbIndex;
			gameIndex[2] = rand() % dbIndex;
			{// 2017-12-20�� �߰��ڵ��, ���� �� ���� �ִ� �����̸� �ٽ� �ٸ��� ���� �ݺ���.
				while (gameIndex[0] == gameIndex[1]) {
					gameIndex[1] = rand() &dbIndex;
				}
				while (gameIndex[0] == gameIndex[2] || gameIndex[1] == gameIndex[2]) {
					gameIndex[2] = rand() & dbIndex;
				}
			}
			// �ñ׸� 2�� n���� n�� 0���� 5���� ���� �ϴ� ���� ������?
			while (checkBit != 63) {
				i = rand() % 6;
				// 2�� i�� ���ڰ� checkBit�� ������ �Ǿ� ���� ������, ����
				// �� 6���� ������ ������ �Ʒ� ������ ����.
				if (!(checkBit & (1 << i))) {
					// �׷��� �� ���� ���� ��Ŵ.
					checkBit |= (1 << i);
					// 2�� i���� �����Ƿ�, i�ε����� gameIndex �� finish_index�� �༭ ����.
					// gameIndex�� ������ ��񿡼� �������� ���õ� 3���� ����Ÿ �ε��� ��ȣ�� ����Ǿ� ����.
					// i�� finish_index�� �������� ����. (�ϳ��� �������� ���� �ް�, �ϳ��� +1�� ����)
					this->gameMap[i] = gameIndex[finish_index / 2];
					// �ϼ� ������ �ʱ�ȭ. (finsih_index)
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
		// true�̸� �ش� �ε��� ī���� 
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
		cardStartIndex ( 6) // text ������Ʈ�� ���� ���� ���� ��.
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
		this->add(TextObject(this->width / 2, 0, this->width / 6, this->height / 6, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "���� ���", strlen("���� ���"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//5�� : ���� �޴� ������Ʈ
		this->add(TextObject(this->width * 2 / 3, 0, this->width / 6, this->height / 6, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_RG, COL_BUTTONSELLECTED_B, "����", strlen("����"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));

		//6�� : text1 ������Ʈ
		this->add(TextObject(this->width * 4 / 16, this->height *2/ 9, this->width * 2 / 16, this->height * 4 / 16, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "NoData", strlen("NoData"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//7�� : text1 ������Ʈ
		this->add(TextObject(this->width * 7 / 16, this->height *2/ 9, this->width * 2 / 16, this->height * 4 / 16, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "NoData", strlen("NoData"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//8�� : text1 ������Ʈ
		this->add(TextObject(this->width * 10 / 16, this->height *2/ 9, this->width * 2 / 16, this->height * 4 / 16, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "NoData", strlen("NoData"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//9�� : text1 ������Ʈ
		this->add(TextObject(this->width * 4 / 16, this->height *2/ 3, this->width * 2 / 16, this->height * 4 / 16, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "NoData", strlen("NoData"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//10�� : text1 ������Ʈ
		this->add(TextObject(this->width * 7 / 16, this->height *2/ 3, this->width * 2 / 16, this->height * 4 / 16, COL_BUTTON_RGB, COL_BUTTON_RGB, COL_BUTTON_RGB, "NoData", strlen("NoData"), COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, COL_TEXTNOMAL_RGB, SIZE_TEXT_W, SIZE_TEXT_H, true));
		//11�� : text1 ������Ʈ
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
		// ī�� ������Ʈ ó��
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
				if (index - this->cardStartIndex != pCard) { // 2017-12-20�� ���� �Ƚ� : ���� ī�� ���ý� ���ǿ� ���� �ϴ� ��츦 ������.
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

