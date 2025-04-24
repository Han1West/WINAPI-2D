#include "pch.h"
#include "CAttackState.h"

#include "CMonster.h"
#include "CPlayer.h"
#include "CStateMachine.h"
#include "CTimeMgr.h"

CAttackState::CAttackState()
	: m_PlayerPos{}
	, m_IsDirRight(false)
	, m_StartPos{}
	, m_CanAttack(false)
	, m_AccTime(0.f)
	, m_AttDelay(0.f)
{
}

CAttackState::~CAttackState()
{
}

void CAttackState::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
	pSm->SetMonsterState((int)MONSTER_STATE::ISATTACK, true);

	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	m_PlayerPos = pPlayer->GetPos();
	m_StartPos = pMonster->GetPos();

	// 추적할 방향을 결정 한다.
	if (m_PlayerPos.x < m_StartPos.x)
		m_IsDirRight = false;
	else
		m_IsDirRight = true;

}

void CAttackState::FinalTick()
{
	CMonster* pMonster = GetOwner<CMonster>();
	Vec2 vPos = pMonster->GetPos();

	if(pMonster->GetName() == L"Snake")
		pMonster->SetPos(vPos);

	else if (pMonster->GetName() == L"Olmec")
	{
		Vec2 vScale = pMonster->GetScale();
		Vec2 vDir = Vec2(1.f, 0.f);
		if (true == m_IsDirRight)
			vDir = Vec2(1.f, 0.f);
		else
			vDir = Vec2(-1.f, 0.f);

		if (m_StartPos.y - (vScale.y) > vPos.y)
		{
			m_CanAttack = true;
		}
		if (m_CanAttack)
		{
			m_AttDelay += DT;
			if (1.5f < m_AttDelay)
			{
				vPos.y += 1000.f * DT;
				pMonster->SetPos(vPos);
			}
			else
			{
				pMonster->SetPos(vPos);
			}
		}
		else
		{
			if (vPos.x - vScale.x / 4.f < m_PlayerPos.x && m_PlayerPos.x < vPos.x + vScale.x / 4.f)
			{
				vPos.y -= 100.f * DT;
				pMonster->SetPos(vPos);
			}
			else
			{
				m_AccTime += DT;
				if (m_AccTime < 0.6f)
				{
					vPos.y -= 400.f * DT;
					pMonster->SetPos(vPos);
				}
				else if (0.6f < m_AccTime)
				{
					//vPos.y -= 200.f * DT;
					vPos.x += 400.f * vDir.x * DT;
					pMonster->SetPos(vPos);
				}
			}
		}
	}
}

void CAttackState::Exit()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
	pSm->SetMonsterState((int)MONSTER_STATE::ISATTACK, false);
	m_CanAttack = false;
	m_AccTime = 0.f;
	m_AttDelay = 0.f;
	if (pMonster->GetName() == L"Olmec")
	{
		Vec2 vPos = pMonster->GetPos();
		vPos.y += 64.f;
		//pMonster->SetPos(vPos);
	}

}

