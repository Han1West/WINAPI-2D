#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"


CTimeMgr::CTimeMgr()
	: m_Frequency{}
	, m_CurrentCount{}
	, m_PrevCount{}
	, m_DT(0.)
	, m_fDT(0.f)
	, m_Time(0.)
	, m_FPS(0)
{

}

CTimeMgr::~CTimeMgr()
{

}

void CTimeMgr::Init()
{
	QueryPerformanceFrequency(&m_Frequency);
	QueryPerformanceCounter(&m_PrevCount);
}

void CTimeMgr::Tick()
{
	QueryPerformanceCounter(&m_CurrentCount);

	m_DT = (double)(m_CurrentCount.QuadPart - m_PrevCount.QuadPart) / (double)m_Frequency.QuadPart;

	// DT 보정, 60 분의 1 보다 크지 못하게 보정
	if ((1.f / 60.f) < m_DT)
		m_DT = (1.f / 60.f);

	m_fDT = (float)m_DT;
	
	// 시간 누적
	m_Time += m_DT;

	// 시간 누적
	m_Second += m_DT;

	// 함수 호출횟수
	++m_FPS;

	if (1. < m_Second)
	{
		m_Second -= 1.;

		// 윈도우 타이틀에 FPS 랑 DeltaTime 표시
		wchar_t buff[256] = {};
		swprintf_s(buff, 256, L"FPS : %d, DeltaTime : %f", m_FPS, m_fDT);
		SetWindowText(CEngine::Get()->GetMainHwnd(), buff);

		m_FPS = 0;
	}

	m_PrevCount = m_CurrentCount;
}