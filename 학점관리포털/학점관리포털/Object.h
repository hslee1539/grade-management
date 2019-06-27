#pragma once
#include <Windows.h>
#include <thread>
#include "Math.h"
#include "Define.h"

/*
�����쿡 �簢���� ����ϱ� ���� �簢�� ��ü�Դϴ�.
�� Ŭ������ �ִϸ��̼� ȿ���� ȿ�������� �����ϱ� ���� ������ �߽��ϴ�.
*/
class Object
{
	//�����Լ��� ������ �� ����Ʈ ���� ��� ���� �� �ڷ�.
public: // ����� ��ü�� ���� ��ġ
	const long originX;
	const long originY;
	const long originW;
	const long originH;
	const INT8 originR;
	const INT8 originG;
	const INT8 originB;
protected: // �ִϸ��̼ǿ� ����
	short dx;
	short dy;
	short dw;
	short dh;
	INT16 dR;// 16��Ʈ�� ������ ������ �� �� �ֱ� ������.
	INT16 dG;
	INT16 dB;
	long targetX;// ���� ������ ���꿡�� ������ �ս��� �ֱ� ������ �������� dx + currentX�� ������ ���⿡ �����ϰ� ���� ���ؼ�
	long targetY;//�� ���� ����
	long targetW;//����
	long targetH;//����
	INT8 targetR;
	INT8 targetG;
	INT8 targetB;
	short frameMax;
	short frameCur;
public:// // �ִϸ��̼ǿ� ���� 2
	bool threadRunning;
public: // ǥ�õ� ��ġ��� ������.
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
	/*�ʱ� �����ߴ� ������ �ǵ����ϴ�.*/
	virtual void back() { this->_back(); }
	/*hdc�� �׸��� �׷��ݴϴ�.*/
	virtual void draw(HDC hdc) { this->_draw(hdc); }

	// x�� y ��ǥ�� �� ��ü ���� �ȿ� �ִ��� üũ�մϴ�.
	bool innerThis(long x, long y) {
		if (this->currentX <= x && x < this->currentX + this->currentW && this->currentY <= y && y < this->currentY + this->currentH)
			return true;
		return false;
	}
	void draw2(HDC hdc) {
		if (this->visible)
			this->draw(hdc);
	}
	/* �ִϸ��̼��� �����մϴ�. �ִϸ��̼� ����� �ñ׸� ������ ������, ���������� dx,dy,dw,dh�� ���� ��ġ�� ��ü�� �����մϴ�.
	startFrame�� ������ ���� ��, ������ ����� �ɶ� ���� ���� �����մϴ�.
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
	// �ñ׸�	m		((m-i)*2 - 1)     =       1 �� ����� �̿�
	//			i = 0	(m * m)
	// �� �ǰų� ���� ��쿡 false�� ��ȯ
	bool animationNext() {
		// frameCur�� ���̳ʽ� �ΰ���, �ִϸ��̼��� ���� �ð����� ����� �׸��� �����.
		if (this->frameCur < 0) {
			this->frameCur++;
			return true;
		}
		// frameCur 0�Ǵ� ���, ��, �ִϸ��̼��� �ؾ� �� ���.
		else if (this->frameCur < this->frameMax) {
			// m * m �� ����
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
		// �������� ���.
		else if (this->frameCur == this->frameMax && this->frameCur != 0) {
			this->animationLast();
		}
		// �������� ����, �� �� ���, �׸��� ������ �ȵ� ��� false
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
	//�� ��ü ��ü������ �ִϸ��̼��� �����մϴ�.
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

