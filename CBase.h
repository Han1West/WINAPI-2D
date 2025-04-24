#pragma once

// 모든 클래스들의 최상위 부모클래스
// 추상클래스

class CBase
{
private:
	static UINT g_ID;

private:
	wstring		m_Name; // wchar_t 타입 동적 배열
	const UINT	m_ID;	// Base 객체마다 고유한 값을 가짐

public:
	// 클래스 헤더에 구현한 함수 ==> 인라인 함수
	// 함수가 호출되더라도 실제 호출비용 없이 코드가 복사되어서 구현됨
	// 함수 코드가 짧은 경우, 자주 호출되는 경우
	void SetName(const wstring& _Name)  { m_Name = _Name;  }
	const wstring& GetName()  { return m_Name;  }
	UINT GetID() { return m_ID; }

public:
	virtual CBase* Clone() = 0;

public:
	CBase();
	CBase(const CBase& _Other);
	virtual ~CBase();
};

