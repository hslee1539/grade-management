#pragma once
#include <Windows.h>
#include <thread>
#include "Math.h"
#include "Define.h"

/*
윈도우에 사각형을 출력하기 위한 사각형 객체입니다.
이 클래스는 애니메이션 효과를 효율적으로 수행하기 위해 디자인 했습니다.
*/
class Object
{
	//가상함수가 포인터 형 바이트 수를 잡고 나서 그 뒤로.
public: // 사용할 객체의 원래 위치
	const long originX;
	const long originY;
	const long originW;
	const long originH;
	const INT8 originR;
	const INT8 originG;
	const INT8 originB;
protected: // 애니메이션용 변수
	short dx;
	short dy;
	short dw;
	short dh;
	INT16 dR;// 16비트인 이유는 음수가 올 수 있기 때문에.
	INT16 dG;
	INT16 dB;
	long targetX;// 정수 나눗셈 연산에서 데이터 손실이 있기 때문에 마지막은 dx + currentX의 정보를 여기에 저장하고 쓰기 위해서
	long targetY;//위 설명 참고
	long targetW;//동일
	long targetH;//동일
	INT8 targetR;
	INT8 targetG;
	INT8 targetB;
	short frameMax;
	short frameCur;
public:// // 애니메이션용 변수 2
	bool threadRunning;
public: // 표시될 위치들과 정보들.
	long currentX;
	long currentY;
	long currentW;
	long currentH;
	INT8 currentR;
	INT8 currentG;
	INT8 currentB;
	bool visible;
	short ID;

public:
	Object(long x, long y, long w, long h, INT8 R, INT8 G, INT8 B, bool visible) : originX(x), originY(y), originW(w), originH(h), originR (R), originG(G),originB(B), visible(visible), ID(1), frameCur(0),frameMax(0),threadRunning(false){}
protected:
	void _back() {
		this->threadRunning = false;
		this->currentX = this->originX;
		this->currentY = this->originY;
		this->currentW = this->originW;
		this->currentH = this->originH;
		this->currentR = this->originR;
		this->currentG = this->originG;
		this->currentB = this->originB;
		this->targetX = this->originX;
		this->targetY = this->originY;
		this->targetW = this->originW;
		this->targetH = this->originH;
		this->targetR = this->originR;
		this->targetG = this->originG;
		this->targetB = this->originB;
	}
	void _draw(HDC hdc) {
		DeleteObject(SelectObject(hdc, CreateSolidBrush(RGB(this->currentR, this->currentG, this->currentB))));
		Rectangle(hdc, this->currentX, this->currentY, this->currentX + this->currentW, this->currentY + this->currentH);
	}
public:
	/*초기 설정했던 값으로 되돌립니다.*/
	virtual void back() { this->_back(); }
	/*hdc에 그림을 그려줍니다.*/
	virtual void draw(HDC hdc) { this->_draw(hdc); }

	// x와 y 좌표가 이 객체 공간 안에 있는지 체크합니다.
	bool innerThis(long x, long y) {
		if (this->currentX <= x && x < this->currentX + this->currentW && this->currentY <= y && y < this->currentY + this->currentH)
			return true;
		return false;
	}
	void draw2(HDC hdc) {
		if (this->visible)
			this->draw(hdc);
	}
	/* 애니메이션을 정의합니다. 애니메이션 모션은 시그마 공식을 따르고, 최종적으로 dx,dy,dw,dh를 더한 위치에 객체가 존재합니다.
	startFrame은 음수를 넣을 시, 음수가 양수가 될때 까지 쉬고 시작합니다.
	*/
	void animationSet(short dx, short dy, short dw,short dh, INT16 dR, INT16 dG, INT16 dB ,short maxFrame , short startFrame = 0) {
		this->dx = dx;
		this->dy = dy;
		this->dw = dw;
		this->dh = dh;
		this->dR = dR;
		this->dG = dG;
		this->dB = dB;
		this->targetX = dx + currentX;
		this->targetY = dy + currentY;
		this->targetW = dw + currentW;
		this->targetH = dh + currentH;
		this->targetR = dR + currentR;
		this->targetG = dG + currentG;
		this->targetB = dB + currentB;
		this->frameMax = maxFrame;
		this->frameCur = startFrame;
		this->animationSetExtra();
	}
	virtual void animationSetExtra() {

	}
	// 시그마	m		((m-i)*2 - 1)     =       1 인 사실을 이용
	//			i = 0	(m * m)
	// 다 되거나 끝난 경우에 false를 반환
	bool animationNext() {
		// frameCur가 마이너스 인경우로, 애니메이션을 일정 시간동안 대기후 그리는 경우임.
		if (this->frameCur < 0) {
			this->frameCur++;
			return true;
		}
		// frameCur 0또는 양수, 즉, 애니메이션을 해야 할 경우.
		else if (this->frameCur < this->frameMax) {
			// m * m 의 약자
			long mmm = Math::pow<long>(this->frameMax ,2);
			this->currentX += (this->dx * ((this->frameMax - this->frameCur) * 2 - 1)) / mmm;
			this->currentY += (this->dy * ((this->frameMax - this->frameCur) * 2 - 1)) / mmm;
			this->currentW += (this->dw * ((this->frameMax - this->frameCur) * 2 - 1)) / mmm;
			this->currentH += (this->dh * ((this->frameMax - this->frameCur) * 2 - 1)) / mmm;
			this->currentR += (this->dR * ((this->frameMax - this->frameCur) * 2 - 1)) / mmm;
			this->currentG += (this->dG * ((this->frameMax - this->frameCur) * 2 - 1)) / mmm;
			this->currentB += (this->dB * ((this->frameMax - this->frameCur) * 2 - 1)) / mmm;
			this->animationNextExtra_running();
			this->frameCur++;
			return true;
		}
		// 마지막인 경우.
		else if (this->frameCur == this->frameMax && this->frameCur != 0) {
			this->animationLast();
		}
		// 마지막인 경우와, 다 된 경우, 그리고 설정이 안된 경우 false
		return false;
	}
	void animationLast() {
		this->currentX = this->targetX;
		this->currentY = this->targetY;
		this->currentW = this->targetW;
		this->currentH = this->targetH;
		this->currentR = this->targetR;
		this->currentG = this->targetG;
		this->currentB = this->targetB;
		this->animationNextExtra_end();
	}
	virtual void animationNextExtra_running() {

	}
	virtual void animationNextExtra_end() {

	}
	//이 객체 자체적으로 애니메이션을 시작합니다.
	void animationStarter() {
		if (this->frameCur < 1 && this->frameMax > 0 || !this->threadRunning) {
			this->threadRunning = true;
			thread t(&Object::_animation, this);
			t.detach();
		}
	}
	void _animation() {
		while (this->animationNext() || this->threadRunning) {
			Sleep(THREAD_SLEEP);
		}
		this->animationLast();
		this->threadRunning = false;
	}
};

