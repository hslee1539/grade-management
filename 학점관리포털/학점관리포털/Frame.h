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
public: // 필드 1
	//mutex visible_sync;
	volatile bool visible;
	bool enable;
	//프로그램 전체 길이입니다.
	long width = 900;
	long height = 720;
	volatile bool threadRunning;
	mutex threadOn;
	
	PortalSystem2 *psAdapter;
	Array<Subject> * mainDB;
	

protected: // 필드2
	// Array<> 를 쓰면, 배열 구조라 append할때 마다 메모리 위치가 바뀌어 append해도 위치가 변하지 않는 링크드 리스트를 사용
	LinkedList<int, Object> objList;
	LinkedList<int, TextObject> textList;
	// 이 배열 클래스에 있는 배열을 순서에 따라서 그래픽을 표시. virtual 클래스의 포인터형 배열이라, TextObject든 Object든, virtaul 포인터 구역의 함수를 실행하기 때문에 원본 객체의 함수를 실행함.
	Array<TextObject*> data;
	// 아... 12.14일날 배운 전방 참조를 사용하면 이럴 필요 없이 Engine자체를 맴버로 두고, Engine의 맴버들로 (Frame의 자식들) 컨트롤 할 수 있었을 텐데,
	// 코드 작성 할떄는 전방 참조를 몰라, Engine을 여기서 사용할 시 컴파일러가 꼬여 코드에는 오류가 없으나, 컴파일 오류가 나서,
	// 최후의 방법으로 Frame객체가 선언할때 마다, 이 포인터의 주소의 값에 정보를 저장하게 만듬...
	// 말이 이상한데 결론적으로 다른 자식 클래스로 화면 전환을 할때 필요합니다.
	LinkedList <long, Frame*> *frameList;
	// 다른 프래임을 관리하기 위한 변수입니다.
public:// 생성자.
	Frame( long ID , LinkedList <long, Frame*> *frameList , PortalSystem2 *psAdapter,Array<Subject> * mainDB ) {
		this->frameList = frameList;
		Frame * PTR = this;
		frameList->append(ID, PTR);
		this->psAdapter = psAdapter;
		this->mainDB = mainDB;
		this->threadRunning = false;
	}

public: // 메소드들
	//모양만 있는 오브젝트를 추가합니다. add 순서에 따라서 그려지는 순위가 변하니 참고하세요/
	void add(Object &object) {
		static int constInt = 0; // 고정 값
		//키값은 사용하지 않음. 0으로 고정
		this->objList.append(constInt, object);
		TextObject *ptr = (TextObject*)&this->objList.getValue();
		this->data += ptr; // 바로 추가하지 않고 변수를 만들어 추가한 이유는 lvalue의 데이터를 받게끔 되어 있기 때문입니다.
	}
	//모양 + 텍스트가 있는 오브젝트를 추가합니다. add 순서에 따라서 그려지는 순위가 변하니 참고하세요.
	void add(TextObject &object) {
		static int constInt = 0;
		this->textList.append(constInt,object);
		TextObject *ptr = &this->textList.getValue();
		this->data += ptr;
	}
	// 모든 프레임 중에서 해당 ID를 가진 프래임을 반환합니다. 없으면 nullptr를 반환합니다.
	Frame* otherFrame(long ID) {
		if (this->frameList->moveKey(ID, false) == true)
			return this->frameList->getValue();
		return nullptr;
	}
	/*내부 배열을 순서에 맞게 그립니다.*/
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
	// WM_PAINT를 호출하기 위해 hWnd를 받음. 스레드를 실행하고 스레드는 뼈대를 그리는 동작만 함.
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
		// 각 객체들에게 스레드를 사용한다고 알려줌.
		for (size_t i = 0, max = this->data.getByte() / sizeof(int*); i < max; i++) {
			this->data[i]->threadRunning = true;
		}
		// 핵심 스레드.
		do {
			running = false;
			for (size_t i = 0, max = this->data.getByte() / sizeof(int*); i < max; i++) {
				running |= this->data[i]->animationNext(); // or 을 하여, 하나라도 true일 경우 계속 실행하게 끔 함.
			}
			Sleep(THREAD_SLEEP);
		} while (running);
		// 각 객체들에게 스레드 사용을 다했다고 알려줌.
		for (size_t i = 0, max = this->data.getByte() / sizeof(int*); i < max; i++) {
			this->data[i]->threadRunning = false;
		}
		this->threadRunning = false;
		this->threadOn.unlock();
		//다른 프레임을 엽니다
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
	// 선택된 놈의 인덱스를 반환. 0은 배경 취급이라, 없을시 0을 반환.
	size_t touchThis(long x, long y) {
		size_t i,max;
		// 출력과 반대로 함. 이유는 나중에 출력 되는 것이 가장 위에 그려지기 때문에.
		for ( i = max = this->data.getByte() / sizeof(int*); i > 0;)
			if (this->data[--i]->innerThis(x, y))
				return i;
		return i;
	}
	// buttonIn 이 대체함.
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
				// virtaul이므로 포인터 함수에 의해 자식 메소드가 호출.
				this->buttonDownEvent(hWnd, index);
			}
			this->threadOn.unlock();
		}
	}
	// 클릭한 경우 애니메이션을 함.
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
				// virtaul이므로 포인터 함수에 의해 자식 메소드가 호출.
				this->buttonUpEvent(hWnd, index);
			}
			this->threadOn.unlock();
		}
	}
	// 클릭 이벤트를 정의. 버튼이 눌러지면 index에 해당되는 놈을 호출해줌.
	virtual void buttonDownEvent(HWND hWnd, size_t index) {}
	// 클릭 이벤트를 정의. 버튼이 눌러지면 index에 해당되는 놈을 호출해줌.
	virtual void buttonUpEvent(HWND hWnd,size_t index) {}
	// buttonIn이 사용하는 변수입니다. 값을 유지할 필요가 있으며, 객체마다 다른 값을 가질 필요가 있어 static 이 아닌 객체 맴버로 사용되었습니다.
	size_t pIndex = 0;
	// 마우스가 버튼 위에 있을때 애니메이션 입니다.
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