#pragma once
#include "PortalUI1.h"
#include "Array1.h"
#include "Subject.h"
class TextBar :
	public PortalUI1
{
public:
	Array<char> text;

	virtual void draw() {
		cout << "<<";
		this->text = this->getline(4);
	}
};

