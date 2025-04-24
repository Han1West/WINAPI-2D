#pragma once
class CDebugMgr
{
	SINGLE(CDebugMgr);
private:
	list<tDebugShapeInfo>	m_DbgList;

	bool					m_DbgOnOff;

public:
	void AddDebugShapeInfo(const tDebugShapeInfo& _info) { m_DbgList.push_back(_info); }

public:	
	void Tick();
};

