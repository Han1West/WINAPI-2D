#pragma once


class CTaskMgr
{
	SINGLE(CTaskMgr);
private:
	vector<tTask>		m_vecTask;

	vector<CObj*>		m_GC;

public:
	void AddTask(const tTask& _Task) { m_vecTask.push_back(_Task); }

private:
	void ExcuteTask(const tTask& _Task);

public:
	void Tick();
};

