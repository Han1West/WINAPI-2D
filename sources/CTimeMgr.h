#pragma once


class CTimeMgr
{
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_Frequency;	// 1초당 카운팅 가능 수
	LARGE_INTEGER	m_CurrentCount;	// 현재 카운팅
	LARGE_INTEGER	m_PrevCount;	// 이전 프레임 카운팅

	double			m_DT;		// DeltaTime : 1프레임에 걸린 시간
	float			m_fDT;		// DeltaTime : 1프레임에 걸린 시간
	double			m_Time;		// 누적 시간

	double			m_Second;	// 1초 체크 용도 누적시간

	UINT			m_FPS;


public:
	void Init();
	void Tick();

public:
	double GetDeltaTime_D() { return m_DT; }
	float GetDeltaTime() { return m_fDT; }
};

