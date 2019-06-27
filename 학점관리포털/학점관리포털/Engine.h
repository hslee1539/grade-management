#pragma once	
#include <Windows.h>
#include "MainFrame.h"
#include "InputFrame.h"
#include "Frame.h"
#include "PortalSystem1.h"
#include "PortalSystem2.h"
#include "Subject.h"
#include "DeleteFrame.h"
#include "ReportFrame.h"
#include "GameFrame.h"
class Engine
{
	
public:
	Array<Subject> mainDB;
	char * file = "myDB.txt";
	LinkedList <long, Frame*> frameList;
	PortalSystem1 *psAdapter = new PortalSystem2();
	MainFrame mainFrame;// (&frameList, ((PortalSystem2*)psAdapter), &mainDB);
	InputFrame inputFrame;
	DeleteFrame deleteFrame;
	ReportFrame reportFrame;
	GameFrame gameFrame;//= GameFrame(&frameList, ((PortalSystem2*)psAdapter), &mainDB);


	Engine() : mainFrame(&frameList, ((PortalSystem2*)psAdapter), &mainDB) , gameFrame(&frameList, ((PortalSystem2*)psAdapter), &mainDB), inputFrame(&frameList, ((PortalSystem2*)psAdapter), &mainDB),deleteFrame(&frameList, ((PortalSystem2*)psAdapter), &mainDB), reportFrame(&frameList, ((PortalSystem2*)psAdapter), &mainDB) {
		mainDB = this->psAdapter->synchronizedMemory(this->file);
		//mainFrame.synchronized();
	}
	~Engine() {
		this->psAdapter->synchronizedFILE(this->file, this->mainDB);
	}

	void start(HWND hWnd) {
		mainFrame.animationOpen(hWnd);
	}
	void mouseMove(HWND hWnd, long x, long y) {
		if (mainFrame.visible)
			mainFrame.buttonIn(hWnd, x, y);
		else if (inputFrame.visible)
			inputFrame.buttonIn(hWnd, x, y);
		else if (deleteFrame.visible)
			deleteFrame.buttonIn(hWnd, x, y);
		else if (reportFrame.visible) {
			reportFrame.buttonIn(hWnd, x, y);
		}
		else if (gameFrame.visible) {
			gameFrame.buttonIn(hWnd, x, y);
		}
	}
	void mouseLUp(HWND hWnd, long x, long y) {
		if (mainFrame.visible) {
			mainFrame.buttonUp(hWnd, x, y);
		}
		else if (inputFrame.visible) {
			inputFrame.buttonUp(hWnd, x, y);
		}
		else if (deleteFrame.visible) {
			deleteFrame.buttonUp(hWnd, x, y);
		}
		else if (reportFrame.visible) {
			reportFrame.buttonUp(hWnd, x, y);
		}
		else if (gameFrame.visible) {
			gameFrame.buttonUp(hWnd, x, y);
		}
	}
	void mouseLDown(HWND hWnd, long x, long y) {
		if (mainFrame.visible) {
			mainFrame.buttonDown(hWnd, x, y);
		}
		else if (inputFrame.visible) {
			inputFrame.buttonDown(hWnd, x, y);
		}
		else if (deleteFrame.visible) {
			deleteFrame.buttonDown(hWnd, x, y);
		}
		else if (reportFrame.visible) {
			reportFrame.buttonDown(hWnd, x, y);
		}
		else if (gameFrame.visible) {
			gameFrame.buttonDown(hWnd, x, y);
		}
	}
	void draw(HDC hdc) {
		if (mainFrame.visible) {
			mainFrame.draw(hdc);
		}
		else if (inputFrame.visible) {
			inputFrame.draw(hdc);
		}
		else if (deleteFrame.visible) {
			deleteFrame.draw(hdc);
		}
		else if (reportFrame.visible) {
			reportFrame.draw(hdc);
		}
		else if (gameFrame.visible) {
			gameFrame.draw(hdc);
		}
	}
	void keyEvent(HWND hWnd, char c[3], bool finish) {
		if (mainFrame.visible) {
			mainFrame.keyEvent(hWnd, c, finish);
		}
		else if (inputFrame.visible) {
			inputFrame.keyEvent(hWnd, c, finish);
		}
		else if (deleteFrame.visible) {
			deleteFrame.keyEvent(hWnd, c, finish);
		}
		else if (reportFrame.visible) {
			reportFrame.keyEvent(hWnd, c,finish);
		}
		else if (gameFrame.visible) {
			gameFrame.keyEvent(hWnd, c, finish);
		}
	}
};

