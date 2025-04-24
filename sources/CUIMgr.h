#pragma once

class CUI;

class CUIMgr
{
	SINGLE(CUIMgr);
private:
	int		m_PreventFrame;

public:
	void Tick();

public:
	void SetPreventFrame(int _FrameCount)
	{
		m_PreventFrame = _FrameCount;
	}

private:
	CUI* GetPriorityUI(CUI* _ParentUI);
	void StatusCheck(CUI* _ParentUI);
};

