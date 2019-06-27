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
private://�ƴ��� �κ��Դϴ�. ���߿� �ν��Ͻ��� Ŭ���� �̸��� �ٲٸ� �˴ϴ�.
	PortalSystem1 psAdapter = PortalSystem1();

private://���� ���۵�
	void load(){
		cout << "�ε���...";
		this->mainDB = this->psAdapter.synchronizedMemory(this->fileName);
		cout << ".ok" << endl;
	}
	void save() {
		cout << "������...";
		this->psAdapter.synchronizedFILE(this->fileName, this->mainDB);
		cout << ".ok" << endl;
	}
	void help() {
		static TextBar bar1 = TextBar();
		cout << endl << "save : ���Ͽ� �����մϴ�." <<
			endl << "next : ����Ʈ�� �Ʒ��� �����ϴ�." <<
			endl << "back : ����Ʈ�� ���� �ø��ϴ�." <<
			endl << "new  : �����͸� �߰��մϴ�." <<
			endl << "delete  : �����͸� �����մϴ�." <<
			endl << "EXIT : �����մϴ�." <<
			endl << "�ѹ��� ���͸� ������ �ǵ��� ���ϴ�." << endl;
		bar1.draw();
	}
	void version() {
		cout << "������ �ʿ��ϸ� help�� �Է��ϼ���" << endl;
	}
	void inputSubject( Subject &data) {
		static TextBar bar1 = TextBar();
		do {
			cout << "������� �Է��ϼ���. " << endl;
			bar1.draw();
			cout << bar1.text.getByte() << "byte / 10Byte" << endl << endl;
		} while (bar1.text.getByte() > 10);//�ݺ� ��ų ���ǹ��� ���⿡
		if (bar1.text.getByte() == 0)
			return ;
		data.reName(bar1.text.getData(), bar1.text.getByte());
		do {
			cout << "������ �Է��ϼ���." << endl;
			bar1.draw();
			cout << bar1.text.getByte() << "byte / 2Byte" << endl << endl;
		} while (bar1.text.getByte() > 2);//�ݺ� ��ų ���ǹ��� ���⿡
		if (bar1.text.getByte() == 0)
			return;
		data.credit = bar1.text.getData()[0];
		do {
			cout << "������ �Է��ϼ���" << endl;
			bar1.draw();
			cout << bar1.text.getByte() << "byte / 3Byte" << endl << endl;
		} while (bar1.text.getByte() > 3);//�ݺ� ��ų ���ǹ��� ���⿡
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
		cout << "�˻�� �Է��ϼ���" << endl;
		bar1.draw();
		Array<Subject> list = this->psAdapter.searchName(this->mainDB, bar1.text);
		ListBox1 box1 = ListBox1( list , list.getByte()/sizeof(Subject) , false);
		box1.draw();
		cout << "��ġ���� ���͸� ��������" << endl;
		bar1.draw();
	}
	bool mainFrame( size_t numOfList) {
		//��ɾ��
		static Array<char> SAVE("save", 5, 1);
		static Array<char> NEXT("next", 5, 1);
		static Array<char> BACK("back", 5, 1);
		static Array<char> NEW("new", 4, 1);
		static Array<char> DEL("delete", 7, 1);
		static Array<char> EXIT("EXIT", 5, 1);
		static Array<char> HELP("help", 5, 1);
		static Array<char> TEST("test", 5, 1);
		static Array<char> SEARCH("search", 7, 1);
		//������
		//ȭ�鿡 �����͸� ǥ�� �ϱ� ���� ����Ʈ 
		static ListBox1 box1 = ListBox1(this->mainDB, numOfList , true);
		static TextBar bar1 = TextBar();
		static InfoBox1 info1 = InfoBox1( this->mainDB);

		//�׷��� ���
		box1.draw();//����Ʈ�� �׸�
		info1.draw();
		cout << endl << "��ɾ �Է��ϼ���. " << endl;
		bar1.draw();//>>�� ǥ���ϰ� �Է�

		// �Է��� ��ɾ� ó��
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
		//���ϳ����� �״�� �޸𸮿� ī���մϴ�.
		this->load();
		//���� & ���� ����մϴ�.
		this->version();
		Sleep(750);
		//���� ȭ���� ����ϰ� ���� ȣ���� ������ ���� �ݺ��մϴ�.
		while (this->mainFrame(5))
			system("cls");
	}
};

