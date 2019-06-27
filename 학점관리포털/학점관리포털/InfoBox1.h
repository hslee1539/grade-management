#pragma once
#include "PortalUI1.h"
class InfoBox1 :
	public PortalUI1
{
	Array<Subject> *data;
	size_t allCredit = 0;
public:
	InfoBox1(Array<Subject> &db) { this->syn(db); }

	void syn( Array<Subject> &db) {
		this->allCredit = 0;
		for (size_t i = 0, max = db.getByte() / sizeof(Subject); i < max; i ++) 
			this->allCredit += db.getData()[i].credit - '0';
	}
	virtual void draw() {
		cout << "ÃÑ ÇÐÁ¡ :" <<allCredit<<endl;
	}
};

