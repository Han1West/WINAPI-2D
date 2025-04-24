#pragma once


class CSelectObject
{
private:
	HDC			m_DC;
	HGDIOBJ		m_PrevGDI;

public:
	CSelectObject(HDC _DC, PEN_TYPE _Pen);
	CSelectObject(HDC _DC, BRUSH_TYPE _BRUSH);
	~CSelectObject();
};

