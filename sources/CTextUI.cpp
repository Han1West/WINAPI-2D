#include "pch.h"
#include "CTextUI.h"

#include "CEngine.h"

CTextUI::CTextUI()
	: m_Str {}
	, m_Scale(10)
{
}


CTextUI::CTextUI(const CTextUI& _Other)
	: CUI(_Other)
	, m_Str {}
	, m_Scale(0)
{
}


CTextUI::~CTextUI()
{
}


void CTextUI::Tick_UI()
{
}

void CTextUI::Render_UI()
{
	Vec2 vPos = GetFinalPos();

	HFONT hFont = CreateFont(m_Scale, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, VARIABLE_PITCH | FF_ROMAN, TEXT("±Ã¼­"));
	SetTextColor(BackDC, RGB(255, 255, 255));
	SelectObject(BackDC, hFont);
	SetBkMode(BackDC, TRANSPARENT);
	TextOut(BackDC, vPos.x, vPos.y, m_Str.c_str(), m_Str.length());
	DeleteObject(hFont);
}

