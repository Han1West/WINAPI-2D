#include "pch.h"
#include "CPlayerStunState.h"

#include "CPlayer.h"
#include "CAnimator.h"
#include "CStateMachine.h"
#include "CTimeMgr.h"
#include "CEffect.h"

CPlayerStunState::CPlayerStunState()
	: m_AccTime(0.f)
{
}

CPlayerStunState::~CPlayerStunState()
{
}

void CPlayerStunState::Enter()
{
	CPlayer* pPlayer = GetOwner<CPlayer>();
	CAnimator* pAnimator = pPlayer->GetComponent<CAnimator>();
	CStateMachine* pSm = pPlayer->GetComponent<CStateMachine>();

	Vec2 vPos = pPlayer->GetPos();

	m_Effect = new CEffect;
	m_Effect->SetName(L"DizzyBird");
	m_Effect->SetPos(Vec2(vPos.x, vPos.y - 30.f));
	m_Effect->SetLayerType((int)LAYER_TYPE::EFFECT);
	CreateObject(m_Effect, LAYER_TYPE::EFFECT);
	CAnimator* pEfeectAnimator = m_Effect->GetComponent<CAnimator>();
	pEfeectAnimator->Play(L"dizzy_birds", true);

	if (pSm->GetState(PLAYER_STATE::ISRIGHT))
	{
		pAnimator->Play(L"player_hit_r", false);
	}
	else
	{
		pAnimator->Play(L"player_hit_l", false);
	}
}

void CPlayerStunState::FinalTick()
{
	m_AccTime += DT;

	CPlayer* pPlayer = GetOwner<CPlayer>();
	CAnimator* pAnimator = pPlayer->GetComponent<CAnimator>();
	CStateMachine* pSm = pPlayer->GetComponent<CStateMachine>();

	if (3.f < m_AccTime)
	{
		if (pSm->GetState(PLAYER_STATE::ISRIGHT))
		{
			pSm->SetState(PLAYER_STATE::ISRIGHT, false);
			pSm->SetState(PLAYER_STATE::ISSTUN, false);
			pSm->ChangeState(L"PlayerIdleState");
		}
		else
		{
			pSm->SetState(PLAYER_STATE::ISRIGHT, false);
			pSm->SetState(PLAYER_STATE::ISSTUN, false);
			pSm->ChangeState(L"PlayerIdleState");
		}
		m_AccTime = 0.f;
	}
}

void CPlayerStunState::Exit()
{
	DeleteObject(m_Effect);
}

