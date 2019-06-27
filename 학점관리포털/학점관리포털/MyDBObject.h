#pragma once
class MyDBObject
{
	char fileName[10] = { 0,0,0,0,0,0,0,0,0,0 };
public:
	void set(char * name) {
		int i = 0;
		while ( i < 10 && name[i] != 0) {
			this->fileName[i] = name[i];
			i++;
		}
		while(i < 10) {
			this->fileName[i++] = 0;
		}
	}
};

