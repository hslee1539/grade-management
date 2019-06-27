#pragma once
#include "PortalUI1.h"
#include "PortalSystem1.h"
#include "Subject.h"
#include "Array1.h"
class ListBox1 :
	public PortalUI1
{
	PortalSystem1 adapter;
	Array<Subject> &data;
	Array<Subject> list;
	//���� �ִ� ������ �� �Դϴ�. (��� ���� ���� �� �ε��� ��...)
	size_t page;
	//�������� ����Ʈ�� ���� �����մϴ�.
	size_t view_count;
	//���� �����ִ� ������ ��ġ�� �˷��ִ� �׷����� �߰����� �����մϴ�.
	bool barVisible;

	void show() {
		//�� ����Ʈ�� ����ִ� ��ҵ��� ���մϴ�.
		size_t max = this->list.getByte() / sizeof(Subject);
		this->drawFormat1_table1("����", "����", "����");
		for (int i = 0; i < max; i++)
			this->drawFormat1((char*)(list[i]), 10, 0, list[i].credit, list[i].score);
		/*��� �ִ� ���� ����� �Ųߴϴ�.*/
		for (size_t i = max; i < view_count; i++) {
			cout << endl;
		}
		//�� ��ġ�� �˷��ִ� �ٸ� �׷��ݴϴ�.
		if (this->barVisible) {
			size_t dataIndex = this->data.getByte() / sizeof(Subject);
			size_t numOfBlock = 10;
			size_t usedBlock = 0;
			if (dataIndex > 0) {
				usedBlock = (page)* numOfBlock / (dataIndex);
				for (int i = 0; i < usedBlock; i++)
					cout << "��";
			}
			for (size_t i = usedBlock; i < numOfBlock; i++)
				cout << "��";
		}
		cout << endl;
	}
public:
	ListBox1( Array<Subject> &data , size_t view_count , bool barVisible) : data(data){
		this->page = 0;
		this->view_count = view_count;
		list = this->adapter.makeList(data, page, view_count);
		this->barVisible = barVisible;
	}

public:
	virtual void draw() { this->show(); }
	void next() {
		if (this->page < this->data.getByte()/sizeof(Subject))
			list = this->adapter.makeList(data, ++this->page, this->view_count);
	}
	void back() {
		if (this->page > 0)
			list = this->adapter.makeList(data, --this->page, this->view_count);
	}
	void syn() { list = this->adapter.makeList(data, this->page, this->view_count); }
};

