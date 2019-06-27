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
	//보고 있는 페이지 수 입니다. (사실 가장 위에 올 인덱스 수...)
	size_t page;
	//보여지는 리스트의 수를 결정합니다.
	size_t view_count;
	//현제 보고있는 페이지 위치를 알려주는 그래프를 추가할지 결정합니다.
	bool barVisible;

	void show() {
		//현 리스트에 들어있는 요소들을 구합니다.
		size_t max = this->list.getByte() / sizeof(Subject);
		this->drawFormat1_table1("과목", "학점", "점수");
		for (int i = 0; i < max; i++)
			this->drawFormat1((char*)(list[i]), 10, 0, list[i].credit, list[i].score);
		/*비어 있는 곳을 띄어쓰기로 매꿉니다.*/
		for (size_t i = max; i < view_count; i++) {
			cout << endl;
		}
		//현 위치를 알려주는 바를 그려줍니다.
		if (this->barVisible) {
			size_t dataIndex = this->data.getByte() / sizeof(Subject);
			size_t numOfBlock = 10;
			size_t usedBlock = 0;
			if (dataIndex > 0) {
				usedBlock = (page)* numOfBlock / (dataIndex);
				for (int i = 0; i < usedBlock; i++)
					cout << "■";
			}
			for (size_t i = usedBlock; i < numOfBlock; i++)
				cout << "□";
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

