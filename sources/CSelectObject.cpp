#include "pch.h"
#include "CSelectObject.h"

#include "CEngine.h"

CSelectObject::CSelectObject(HDC _DC, PEN_TYPE _Pen)
	: m_DC(_DC)	
	, m_PrevGDI(nullptr)
{	
	m_PrevGDI = SelectObject(m_DC, CEngine::Get()->GetPen(_Pen));
}

CSelectObject::CSelectObject(HDC _DC, BRUSH_TYPE _BRUSH)
	: m_DC(_DC)
	, m_PrevGDI(nullptr)
{
	m_PrevGDI = SelectObject(m_DC, CEngine::Get()->GetBrush(_BRUSH));
}

CSelectObject::~CSelectObject()
{
	SelectObject(m_DC, m_PrevGDI);
}
