#pragma once
#include "Array1.h"
#include "Subject.h"
#include "PortalSystem1.h"
#include "PortalUI1.h"
#include "TextBar.h"
#include "ListBox1.h"
#include "InfoBox1.h"
#include <Windows.h>

class PortalEngin:public PortalUI1
{
	Array<Subject> mainDB;
	char * fileName = "myDB.txt";
private://아답터 부분입니다. 나중에 인스턴스할 클래스 이름만 바꾸면 됩니다.
	PortalSystem1 psAdapter = PortalSystem1();

private://각각 동작들
	void load(){
		cout << "로딩중...";
		this->mainDB = this->psAdapter.synchronizedMemory(this->fileName);
		cout << ".ok" << endl;
	}
	void save() {
		cout << "저장중...";
		this->psAdapter.synchronizedFILE(this->fileName, this->mainDB);
		cout << ".ok" << endl;
	}
	void help() {
		static TextBar bar1 = TextBar();
		cout << endl << "save : 파일에 저장합니다." <<
			endl << "next : 리스트를 아래로 내립니다." <<
			endl << "back : 리스트를 위로 올립니다." <<
			endl << "new  : 데이터를 추가합니다." <<
			endl << "delete  : 데이터를 삭제합니다." <<
			endl << "EXIT : 종료합니다." <<
			endl << "한번더 엔터를 누르면 되돌아 갑니다." << endl;
		bar1.draw();
	}
	void version() {
		cout << "도움이 필요하면 help를 입력하세요" << endl;
	}
	void inputSubject( Subject &data) {
		static TextBar bar1 = TextBar();
		do {
			cout << "과목명을 입력하세요. " << endl;
			bar1.draw();
			cout << bar1.text.getByte() << "byte / 10Byte" << endl << endl;
		} while (bar1.text.getByte() > 10);//반복 시킬 조건문을 여기에
		if (bar1.text.getByte() == 0)
			return ;
		data.reName(bar1.text.getData(), bar1.text.getByte());
		do {
			cout << "학점을 입력하세요." << endl;
			bar1.draw();
			cout << bar1.text.getByte() << "byte / 2Byte" << endl << endl;
		} while (bar1.text.getByte() > 2);//반복 시킬 조건문을 여기에
		if (bar1.text.getByte() == 0)
			return;
		data.credit = bar1.text.getData()[0];
		do {
			cout << "점수를 입력하세요" << endl;
			bar1.draw();
			cout << bar1.text.getByte() << "byte / 3Byte" << endl << endl;
		} while (bar1.text.getByte() > 3);//반복 시킬 조건문을 여기에
		if (bar1.text.getByte() == 0)
			return;
		MemoryTool::copyDeep<char>(data.score, bar1.text.getData(), bar1.text.getByte());
	}
	void addFrame() {
		Subject data;
		this->inputSubject(data);
		this->psAdapter.add(this->mainDB, data);
	}
	void deleteFrame() {
		Subject data;
		this->inputSubject(data);
		this->psAdapter.remove(this->mainDB, data);
	}
	void searchFrame() {
		TextBar bar1 = TextBar();
		cout << "검색어를 입력하세요" << endl;
		bar1.draw();
		Array<Subject> list = this->psAdapter.searchName(this->mainDB, bar1.text);
		ListBox1 box1 = ListBox1( list , list.getByte()/sizeof(Subject) , false);
		box1.draw();
		cout << "마치려면 엔터를 누르세요" << endl;
		bar1.draw();
	}
	bool mainFrame( size_t numOfList) {
		//명령어들
		static Array<char> SAVE("save", 5, 1);
		static Array<char> NEXT("next", 5, 1);
		static Array<char> BACK("back", 5, 1);
		static Array<char> NEW("new", 4, 1);
		static Array<char> DEL("delete", 7, 1);
		static Array<char> EXIT("EXIT", 5, 1);
		static Array<char> HELP("help", 5, 1);
		static Array<char> TEST("test", 5, 1);
		static Array<char> SEARCH("search", 7, 1);
		//위젯들
		//화면에 데이터를 표시 하기 위한 리스트 
		static ListBox1 box1 = ListBox1(this->mainDB, numOfList , true);
		static TextBar bar1 = TextBar();
		static InfoBox1 info1 = InfoBox1( this->mainDB);

		//그래픽 출력
		box1.draw();//리스트를 그림
		info1.draw();
		cout << endl << "명령어를 입력하세요. " << endl;
		bar1.draw();//>>를 표시하고 입력

		// 입력한 명령어 처리
		if (bar1.text == SAVE)
			this->save();
		else if (bar1.text == NEXT)
			box1.next();
		else if (bar1.text == BACK)
			box1.back();
		else if (bar1.text == NEW) {
			this->addFrame();
			box1.syn();
			info1.syn(this->mainDB);
		}
		else if (bar1.text == DEL) {
			this->deleteFrame();
			box1.syn();
			info1.syn(this->mainDB);
		}
		else if (bar1.text == SEARCH)
			this->searchFrame();
		else if (bar1.text == HELP)
			help();
		else if (bar1.text == TEST)
			this->mainDB = this->mainDB + this->mainDB;
		else if (bar1.text == EXIT)
			return false;
		return true;
	}
public:
	void draw() {
		//파일내용을 그대로 메모리에 카피합니다.
		this->load();
		//버전 & 팁을 출력합니다.
		this->version();
		Sleep(750);
		//메인 화면을 출력하고 종료 호출이 있을때 까지 반복합니다.
		while (this->mainFrame(5))
			system("cls");
	}
};

