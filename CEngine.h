#pragma once

class CLevel;
class CTexture;

class CEngine
{
	SINGLE(CEngine);
private:
	HWND		m_hMainWnd;
	HDC			m_DC;

	CTexture*	m_BackBuffer;

	Vec2		m_Resolution;

	HPEN		m_Pen[(int)PEN_TYPE::END];
	HBRUSH		m_Brush[(int)BRUSH_TYPE::END];


public:
	HWND GetMainHwnd() { return m_hMainWnd; }
	HDC GetMainDC() { return m_DC; }
	HDC GetBackDC();

	HPEN GetPen(PEN_TYPE _Type) { return m_Pen[(UINT)_Type]; }
	HBRUSH GetBrush(BRUSH_TYPE _Type) { return m_Brush[(UINT)_Type]; }

	Vec2 GetResolution() { return m_Resolution; }

	void ChangeWindowResolution(UINT _Width, UINT _Height);

public:
	void Init(HWND _MainHwnd, UINT _Width, UINT _Height);
	void Progress();

private:
	void Render();
	void CreateGDIObject();
};

