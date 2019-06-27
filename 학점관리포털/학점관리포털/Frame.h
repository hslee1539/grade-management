#pragma once
#include "Array1.h"
#include "LinkedList.h"
#include "Object.h"
#include "TextObject.h"
#include <thread>
#include <mutex>
#include "Define.h"
#include "Subject.h";
#include "PortalSystem2.h"

class Frame
{
public: // �ʵ� 1
	//mutex visible_sync;
	volatile bool visible;
	bool enable;
	//���α׷� ��ü �����Դϴ�.
	long width = 900;
	long height = 720;
	volatile bool threadRunning;
	mutex threadOn;
	
	PortalSystem2 *psAdapter;
	Array<Subject> * mainDB;
	

protected: // �ʵ�2
	// Array<> �� ����, �迭 ������ append�Ҷ� ���� �޸� ��ġ�� �ٲ�� append�ص� ��ġ�� ������ �ʴ� ��ũ�� ����Ʈ�� ���
	LinkedList<int, Object> objList;
	LinkedList<int, TextObject> textList;
	// �� �迭 Ŭ������ �ִ� �迭�� ������ ���� �׷����� ǥ��. virtual Ŭ������ �������� �迭�̶�, TextObject�� Object��, virtaul ������ ������ �Լ��� �����ϱ� ������ ���� ��ü�� �Լ��� ������.
	Array<TextObject*> data;
	// ��... 12.14�ϳ� ��� ���� ������ ����ϸ� �̷� �ʿ� ���� Engine��ü�� �ɹ��� �ΰ�, Engine�� �ɹ���� (Frame�� �ڽĵ�) ��Ʈ�� �� �� �־��� �ٵ�,
	// �ڵ� �ۼ� �ҋ��� ���� ������ ����, Engine�� ���⼭ ����� �� �����Ϸ��� ���� �ڵ忡�� ������ ������, ������ ������ ����,
	// ������ ������� Frame��ü�� �����Ҷ� ����, �� �������� �ּ��� ���� ������ �����ϰ� ����...
	// ���� �̻��ѵ� ��������� �ٸ� �ڽ� Ŭ������ ȭ�� ��ȯ�� �Ҷ� �ʿ��մϴ�.
	LinkedList <long, Frame*> *frameList;
	// �ٸ� �������� �����ϱ� ���� �����Դϴ�.
public:// ������.
	Frame( long ID , LinkedList <long, Frame*> *frameList , PortalSystem2 *psAdapter,Array<Subject> * mainDB ) {
		this->frameList = frameList;
		Frame * PTR = this;
		frameList->append(ID, PTR);
		this->psAdapter = psAdapter;
		this->mainDB = mainDB;
		this->threadRunning = false;
	}

public: // �޼ҵ��
	//��縸 �ִ� ������Ʈ�� �߰��մϴ�. add ������ ���� �׷����� ������ ���ϴ� �����ϼ���/
	void add(Object &object) {
		static int constInt = 0; // ���� ��
		//Ű���� ������� ����. 0���� ����
		this->objList.append(constInt, object);
		TextObject *ptr = (TextObject*)&this->objList.getValue();
		this->data += ptr; // �ٷ� �߰����� �ʰ� ������ ����� �߰��� ������ lvalue�� �����͸� �ްԲ� �Ǿ� �ֱ� �����Դϴ�.
	}
	//��� + �ؽ�Ʈ�� �ִ� ������Ʈ�� �߰��մϴ�. add ������ ���� �׷����� ������ ���ϴ� �����ϼ���.
	void add(TextObject &object) {
		static int constInt = 0;
		this->textList.append(constInt,object);
		TextObject *ptr = &this->textList.getValue();
		this->data += ptr;
	}
	// ��� ������ �߿��� �ش� ID�� ���� �������� ��ȯ�մϴ�. ������ nullptr�� ��ȯ�մϴ�.
	Frame* otherFrame(long ID) {
		if (this->frameList->moveKey(ID, false) == true)
			return this->frameList->getValue();
		return nullptr;
	}
	/*���� �迭�� ������ �°� �׸��ϴ�.*/
	void draw(HDC hdc) {
		if (this->visible) {
			DeleteObject(SelectObject(hdc, CreatePen(PS_NULL, 0, 0)));
			for (size_t i = 0, max = this->data.getByte() / sizeof(int*); i < max; i++) {
				TextObject * obj = this->data[i];
				if (obj->ID == 1) {
					obj->draw(hdc);
					//((Object*)this->data[i])->draw(hdc);
				}
				else if (obj->ID == 2) {
					obj->draw(hdc);
					//((TextObject*)this->data[i])->draw(hdc);
				}
			}
		}
	}
	virtual void animationOpenEvent(HWND hWnd) {

	}
	// WM_PAINT�� ȣ���ϱ� ���� hWnd�� ����. �����带 �����ϰ� ������� ���븦 �׸��� ���۸� ��.
	void animationOpen(HWND hWnd) {
		this->threadOn.lock();
		this->visible = true;
		this->threadRunning = true;
		for (size_t i = 0, max = this->data.getByte() / sizeof(int*); i < max; i++) {
			this->data[i]->back();
			short x = this->data[i]->currentW/2;
			//short x = 10;
			short y = this->data[i]->currentH / 8;
			short w = this->data[i]->currentW / 4;
			short h = this->data[i]->currentH / 4;
			short r = this->data[i]->currentR * 3 / 4;
			short g = this->data[i]->currentG * 3 / 4;
			short b = this->data[i]->currentB * 3 / 4;
			this->data[i]->currentX += x;
			this->data[i]->currentY += y;
			this->data[i]->currentR -= r;
			this->data[i]->currentG -= g;
			this->data[i]->currentB -= b;
			this->data[i]->currentW -= w;
			this->data[i]->currentH -= h;
			
			this->data[i]->animationSet(-x, -y, w, h, r,g,b, ANIMATION_FRAME, (-1)*i/2);
		}
		this->animationOpenEvent(hWnd);
		this->threadOn.unlock();
		thread th = thread(&Frame::threadAnimation, this, hWnd ,nullptr , false );
		SendMessage(hWnd, WM_TIMER, 1, 0);
		th.detach();
	}
	void threadAnimation(HWND hWnd , Frame * open, bool bOpen) {
		this->threadOn.lock();
		this->threadRunning = true;
		bool running = false;
		// �� ��ü�鿡�� �����带 ����Ѵٰ� �˷���.
		for (size_t i = 0, max = this->data.getByte() / sizeof(int*); i < max; i++) {
			this->data[i]->threadRunning = true;
		}
		// �ٽ� ������.
		do {
			running = false;
			for (size_t i = 0, max = this->data.getByte() / sizeof(int*); i < max; i++) {
				running |= this->data[i]->animationNext(); // or �� �Ͽ�, �ϳ��� true�� ��� ��� �����ϰ� �� ��.
			}
			Sleep(THREAD_SLEEP);
		} while (running);
		// �� ��ü�鿡�� ������ ����� ���ߴٰ� �˷���.
		for (size_t i = 0, max = this->data.getByte() / sizeof(int*); i < max; i++) {
			this->data[i]->threadRunning = false;
		}
		this->threadRunning = false;
		this->threadOn.unlock();
		//�ٸ� �������� ���ϴ�
		if (bOpen) {
			this->visible = false;
			open->threadRunning = true;
			open->animationOpen(hWnd);
		}
	}

	  void animationClose(HWND hWnd, Frame *open , bool bOpen) {
		this->visible = true;
		this->threadRunning = true;
		for (size_t i = 0, max = this->data.getByte() / sizeof(int*); i < max; i++) {
			this->data[i]->back();
			short x = this->data[i]->currentW/2;
			//short x = 10;
			short y = this->data[i]->currentH / 8;
			short w = this->data[i]->currentW / 4;
			short h = this->data[i]->currentH / 4;
			short r = this->data[i]->currentR *3/ 4;
			short g = this->data[i]->currentG *3/ 4;
			short b = this->data[i]->currentB *3/ 4;
			this->data[i]->animationSet(-x, y, -w , -h, -r, -g, -b, ANIMATION_FRAME, (-1)*i/2);
		}
		thread th = thread(&Frame::threadAnimation, this, hWnd , open , bOpen);
		SendMessage(hWnd, WM_TIMER, 1, 0);
		th.detach();
	}
	// ���õ� ���� �ε����� ��ȯ. 0�� ��� ����̶�, ������ 0�� ��ȯ.
	size_t touchThis(long x, long y) {
		size_t i,max;
		// ��°� �ݴ�� ��. ������ ���߿� ��� �Ǵ� ���� ���� ���� �׷����� ������.
		for ( i = max = this->data.getByte() / sizeof(int*); i > 0;)
			if (this->data[--i]->innerThis(x, y))
				return i;
		return i;
	}
	// buttonIn �� ��ü��.
	void buttonDown(HWND hWnd, long x, long y) {
		if (this->threadOn.try_lock()) {
			size_t index = this->touchThis(x, y);
			if (pIndex) {
				this->data[pIndex]->back();
				pIndex = 0;
			}
			if (index) {
				pIndex = index;
				this->data[index]->back();
				short w = this->data[index]->originW / 8;
				short h = this->data[index]->originH / 8;
				this->data[index]->animationSet(w, h, -w * 2, -h * 2, 0, 0, 0, ANIMATION_FRAME / 2, 0);
				this->data[index]->animationStarter();
				SendMessage(hWnd, WM_TIMER, 1, 0);
				// virtaul�̹Ƿ� ������ �Լ��� ���� �ڽ� �޼ҵ尡 ȣ��.
				this->buttonDownEvent(hWnd, index);
			}
			this->threadOn.unlock();
		}
	}
	// Ŭ���� ��� �ִϸ��̼��� ��.
	void buttonUp(HWND hWnd,long x, long y) {
		if (this->threadOn.try_lock()) {
			size_t index = this->touchThis(x, y);
			if (pIndex) {
				this->data[pIndex]->back();
				pIndex = 0;
			}
			if (index) {
				pIndex = index;
				this->data[index]->back();
				//this->data[index]->animationSet(-10, -10, 20, 20, 0, 0, 0, 3, 0);
				//this->data[index]->animationStarter();
				SendMessage(hWnd, WM_TIMER, 1, 0);
				// virtaul�̹Ƿ� ������ �Լ��� ���� �ڽ� �޼ҵ尡 ȣ��.
				this->buttonUpEvent(hWnd, index);
			}
			this->threadOn.unlock();
		}
	}
	// Ŭ�� �̺�Ʈ�� ����. ��ư�� �������� index�� �ش�Ǵ� ���� ȣ������.
	virtual void buttonDownEvent(HWND hWnd, size_t index) {}
	// Ŭ�� �̺�Ʈ�� ����. ��ư�� �������� index�� �ش�Ǵ� ���� ȣ������.
	virtual void buttonUpEvent(HWND hWnd,size_t index) {}
	// buttonIn�� ����ϴ� �����Դϴ�. ���� ������ �ʿ䰡 ������, ��ü���� �ٸ� ���� ���� �ʿ䰡 �־� static �� �ƴ� ��ü �ɹ��� ���Ǿ����ϴ�.
	size_t pIndex = 0;
	// ���콺�� ��ư ���� ������ �ִϸ��̼� �Դϴ�.
	void buttonIn(HWND hWnd, long x, long y) {
		if (this->threadOn.try_lock()) {
			size_t index = this->touchThis(x, y);
			if (index) {
				if (!this->threadRunning)
					if (!this->data[index]->threadRunning)
						if (pIndex != index) {
							if (pIndex) {
								this->data[pIndex]->back();
							}
							pIndex = index;
							this->data[index]->currentR += 20;
							this->data[index]->currentG += 20;
							this->data[index]->currentB += 40;
							SendMessage(hWnd, WM_TIMER, 1, 0);
						}
			}
			else if (pIndex) {
				this->data[pIndex]->back();
				pIndex = 0;
			}
			this->threadOn.unlock();
		}
	}
	virtual void keyEvent(HWND hWnd, char c[3], bool finish) {

	}
};