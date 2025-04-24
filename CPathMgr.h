#pragma once


class CPathMgr
{
	SINGLE(CPathMgr);
private:
	wstring		m_ContentPath;

public:
	const wstring& GetContentPath() { return m_ContentPath; }

public:
	void Init();
};

