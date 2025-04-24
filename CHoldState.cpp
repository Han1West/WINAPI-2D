#include "pch.h"
#include "CHoldState.h"

#include "CMonster.h"
#include "CAnimator.h"
#include "CStateMachine.h"
#include "CEffect.h"
#include "CAnimator.h"

CHoldState::CHoldState()
{
}

CHoldState::~CHoldState()
{
}

void CHoldState::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* pAnimator = pMonster->GetComponent<CAnimator>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();

	Vec2 vPos = pMonster->GetPos();

	m_Effect = new CEffect;
	m_Effect->SetName(L"DizzyBird");
	m_Effect->SetPos(Vec2(vPos.x, vPos.y - 30.f));
	m_Effect->SetLayerType((int)LAYER_TYPE::EFFECT);
	CreateObject(m_Effect, LAYER_TYPE::EFFECT);
	CAnimator* pEfeectAnimator = m_Effect->GetComponent<CAnimator>();
	pEfeectAnimator->Play(L"dizzy_birds", true);

	if (pMonster->GetName() == L"Victim")
	{
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"npc_die_r", false);
		}
		else
		{
			pAnimator->Play(L"npc_die_l", false);
		}
	}
}

void CHoldState::FinalTick()
{
	CMonster* pMonster = GetOwner<CMonster>();
	Vec2 vPos = pMonster->GetPos();
	m_Effect->SetPos(Vec2(vPos.x, vPos.y - 30.f));
}

void CHoldState::Exit()
{
	DeleteObject(m_Effect);
}



