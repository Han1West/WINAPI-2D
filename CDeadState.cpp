#include "pch.h"
#include "CDeadState.h"

#include "CShopkeeper.h"
#include "CShotgun.h"
#include "CPassiveItem.h"
#include "CMonster.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CStateMachine.h"
#include "CTimeMgr.h"

CDeadState::CDeadState()
{
}

CDeadState::~CDeadState()
{
}

void CDeadState::Enter()
{
	CMonster* pMonster = GetOwner<CMonster>();
	CAnimator* pAnimator = pMonster->GetComponent<CAnimator>();
	CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();

	if (pMonster->GetName() == L"Shopkeeper")
	{
		CShopkeeper* pShopKeeper = dynamic_cast<CShopkeeper*>(pMonster);

		if (!pShopKeeper->IsDisplayChange())
		{
			vector<CObj*> pShopItem = pShopKeeper->GetShopItem();

			for (int i = 0; i < pShopItem.size(); ++i)
			{
				if (CShotgun* pShotgun = dynamic_cast<CShotgun*>(pShopItem[i]))
					pShotgun->SetDisplay(false);

				if (CPassiveItem* pPassiveItem = dynamic_cast<CPassiveItem*>(pShopItem[i]))
					pPassiveItem->SetDisplay(false);
			}
			pShopKeeper->SetDisplayChange(true);
		}

		CCollider* pCollider = dynamic_cast<CCollider*>(pMonster->GetComponentByName(L"ShopkeeperBody"));
		pCollider->Deactivate();
		pCollider = dynamic_cast<CCollider*>(pMonster->GetComponentByName(L"ShopkeeperHead"));
		pCollider->Deactivate();
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"shopkeeper_die_r", false);
		}
		else
		{
			pAnimator->Play(L"shopkeeper_die_l", false);
		}
	}

	if (pMonster->GetName() == L"Caveman")
	{
		CCollider* pCollider = dynamic_cast<CCollider*>(pMonster->GetComponentByName(L"CavemanBody"));
		pCollider->Deactivate();
		pCollider = dynamic_cast<CCollider*>(pMonster->GetComponentByName(L"CavemanHead"));
		pCollider->Deactivate();
		if (pSm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			pAnimator->Play(L"caveman_die_r", false);
		}
		else
		{
			pAnimator->Play(L"caveman_die_l", false);
		}
	}

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

void CDeadState::FinalTick()
{
}

void CDeadState::Exit()
{
}



