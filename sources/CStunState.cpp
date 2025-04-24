#include "pch.h"
#include "CStunState.h"


#include "CShopkeeper.h"
#include "CShotgun.h"
#include "CPassiveItem.h"
#include "CMonster.h"
#include "CAnimator.h"
#include "CStateMachine.h"
#include "CCollider.h"
#include "CTimeMgr.h"
#include "CEffect.h"


CStunState::CStunState()
	: m_AccTime(0.f)
	, m_Effect(nullptr)
	, m_DisplayChange(false)
{
}

CStunState::~CStunState()
{
}

void CStunState::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* pAnimator = pMonster->GetComponent<CAnimator>();
	CCollider* pCollider = pMonster->GetComponent<CCollider>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();

	Vec2 vPos = pMonster->GetPos();

	m_Effect = new CEffect;
	m_Effect->SetName(L"DizzyBird");
	m_Effect->SetPos(Vec2(vPos.x, vPos.y - 30.f));
	m_Effect->SetLayerType((int)LAYER_TYPE::EFFECT);
	CreateObject(m_Effect, LAYER_TYPE::EFFECT);
	CAnimator* pEfeectAnimator = m_Effect->GetComponent<CAnimator>();
	pEfeectAnimator->Play(L"dizzy_birds", true);

	if (pMonster->GetName() == L"Shopkeeper")
	{
		CShopkeeper* pShopKeeper = dynamic_cast<CShopkeeper*>(pMonster);

		if (!pShopKeeper->IsDisplayChange())
		{
			vector<CObj*> pShopItem = pShopKeeper->GetShopItem();

			for (int i = 0; i < pShopItem.size(); ++i)
			{
				if (pShopItem[i]->IsDead())
					continue;

				if (CShotgun* pShotgun = dynamic_cast<CShotgun*>(pShopItem[i]))
					pShotgun->SetDisplay(false);

				if (CPassiveItem* pPassiveItem = dynamic_cast<CPassiveItem*>(pShopItem[i]))
					pPassiveItem->SetDisplay(false);
			}
			pShopKeeper->SetDisplayChange(true);
		}
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"shopkeeper_hit_r", false);
		}
		else
		{
			pAnimator->Play(L"shopkeeper_hit_l", false);
		}
	}
	
	if (pMonster->GetName() == L"Caveman")
	{
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"caveman_hit_r", false);
		}
		else
		{
			pAnimator->Play(L"caveman_hit_l", false);
		}
	}

	if (pMonster->GetName() == L"Victim")
	{
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"npc_hit_r", false);
		}
		else
		{
			pAnimator->Play(L"npc_hit_l", false);
		}
	}

	pCollider->Deactivate();
}

void CStunState::FinalTick()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* pAnimator = pMonster->GetComponent<CAnimator>();
	CCollider* pCollider = pMonster->GetComponent<CCollider>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();

	m_AccTime += DT;
	
	if (1.f < m_AccTime)
	{
		pCollider->Activate();
	}

	if (3.f < m_AccTime)
	{
		if (pMonster->GetName() == L"Shopkeeper")
		{
			GetStateMachine()->ChangeState(L"TraceState");
		}
		else if (pMonster->GetName() == L"Victim")
		{
			GetStateMachine()->ChangeState(L"PatrolState");
		}
		else
		{
			if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				pSm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
				GetStateMachine()->ChangeState(L"IdleState");
			}
			else
			{
				pSm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
				GetStateMachine()->ChangeState(L"IdleState");
			}
		}
		m_AccTime = 0.f;
	}
}


void CStunState::Exit()
{
	DeleteObject(m_Effect);
}