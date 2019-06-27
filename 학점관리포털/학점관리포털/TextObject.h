#pragma once
#include "Object.h"
#include <mutex>
#define MAXTEXT 32
class TextObject : public Object
{
protected:
	const INT8 originTextR;
	const INT8 originTextG;
	const INT8 originTextB;
	const short originFontW;
	const short originFontH;
protected:
	INT16 dTextR;
	INT16 dTextG;
	INT16 dTextB;
public:
	INT8 currentTextR;
	INT8 currentTextG;
	INT8 currentTextB;
	short currentLen;
	short currentFontW;
	short currentFontH;
	HFONT font=0;
	char text[MAXTEXT];
	std::mutex mx;
public:
	TextObject(const TextObject &obj) : Object(obj.originX, obj.originY, obj.originW, obj.originH, obj.originR, obj.originG, obj.originB, obj.visible), currentLen(obj.currentLen), originTextR(obj.originTextR), originTextG(obj.originTextG), originTextB(obj.originTextB), originFontW(obj.originFontW), originFontH(obj.originFontH) {
		strcpy_s(this->text, MAXTEXT, obj.text);
	}
	TextObject (long x, long y, long w, long h, INT8 R, INT8 G , INT8 B, char *text, short textLen, INT8 textR , INT8 textG , INT8 textB , short fontW, short fontH, bool visible) : Object (x,y,w,h,R,G,B,visible),  originTextR(textR), originTextG(textG), originTextB(textB), originFontW ( fontW ), originFontH ( fontH), currentLen(textLen){
		strcpy_s(this->text, MAXTEXT, text);
		this->ID = 2;
	}
protected:
	void _drawText(HDC hdc) {
		RECT rect = { this->currentX , this->currentY , this->currentX + this->currentW , this->currentY + this->currentH };
		SelectObject(hdc, this->font);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(this->currentTextR , this->currentTextG , this->currentTextB));
		DrawTextA(hdc, this->text, this->currentLen, &rect,  DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	void _backText() {
		this->currentTextR = this->originTextR;
		this->currentTextG = this->originTextG;
		this->currentTextB = this->originTextB;
		this->currentFontW = this->originFontW;
		this->currentFontH = this->originFontH;
		this->initFont();
	}
public:
	void initFont() {
		if (mx.try_lock()) {
			if (this->font != 0)
				DeleteObject(this->font);
			this->font = CreateFontA(this->currentFontH, this->currentFontW, 0, 0, FW_NORMAL, 0, 0, 0, HANGEUL_CHARSET, 3, 2, CLEARTYPE_NATURAL_QUALITY, FIXED_PITCH | FF_ROMAN, "¸¼Àº °íµñ");
			mx.unlock();
		}
	}
	virtual void back() {
		this->_back();
		this->_backText();
	}
	virtual void draw(HDC hdc) {
		this->_draw(hdc);
		this->_drawText(hdc);
	}
	virtual void animationSetExtra() {
		if (this->dR > 0) {
			this->dTextR = this->originTextR - this->currentR;
			this->currentTextR = this->currentR;
		}
		else {
			this->dTextR = this->originTextR - this->targetR;
			//this->currentTextR = this->targetR;
		}
		if (this->dG > 0) {
			this->dTextG = this->originTextG - this->currentG;
			this->currentTextG = this->currentG;
		}
		else {
			this->dTextG = this->originTextG - this->targetG;
			//this->currentTextG = this->targetG;
		}
		if (this->dB > 0) {
			this->dTextB = this->originTextB - this->currentB;
			this->currentTextB = this->currentB;
		}
		else {
			this->dTextB = this->originTextB - this->targetB;
			//this->currentTextB = this->targetB;
		}

		this->initFont();
		
	}
	void animationNextExtra_running() {
		long mmm = Math::pow<long>(this->frameMax, 2);
		this->currentTextR += (this->dTextR * ((this->frameMax - this->frameCur) * 2 - 1)) / mmm;
		this->currentTextG += (this->dTextG * ((this->frameMax - this->frameCur) * 2 - 1)) / mmm;
		this->currentTextB += (this->dTextB * ((this->frameMax - this->frameCur) * 2 - 1)) / mmm;
		this->initFont();
	}
	void animationNextExtra_end() {
		//this->initFont();
	}
};

