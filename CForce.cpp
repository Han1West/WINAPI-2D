#include "pch.h"
#include "CForce.h"

#include "CDebugMgr.h"
#include "CTimeMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CObj.h"
#include "CRigidBody.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CTile.h"
#include "CTileMap.h"
#include "CPlayer.h"
#include "CShopkeeper.h"
#include "CCaveman.h"
#include "CVictim.h"
#include "CPassiveItem.h"

#include "CStateMachine.h"

CForce::CForce()
	: m_Force(0.f)
	, m_Range(0.f)
	, m_Duration(0.f)
	, m_AccTime(0.f)
	, m_Animator(nullptr)
	, m_Att(10.f)
	, m_Collider(nullptr)
{
	m_Animator = AddComponent(new CAnimator);
	m_Animator->LoadAnimation(L"animation\\BLAST.anim");
	m_Animator->Play(L"blast", false);

	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Force");
	m_Collider->SetOffset(Vec2(0.f, 00.f));
	m_Collider->SetScale(Vec2(200.f, 130.f));

}

CForce::CForce(const CForce& _Other)
	: CObj(_Other)
	, m_Force(_Other.m_Force)
	, m_Range(_Other.m_Range)
	, m_Duration(_Other.m_Duration)
	, m_AccTime(0.f)
{
}

CForce::~CForce()
{
}

void CForce::Tick()
{
	CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
	CTileMap* pTileMap = pLevel->GetTileMap();
	m_AccTime += DT;
	if (m_AccTime < m_Duration)
	{
		for (int i = 0; i < pTileMap->GetRow(); ++i)
		{
			for (int j = 0; j < pTileMap->GetCol(); ++j)
			{
				//if(pTileMap->)
				//tTileInfo* pTileInfo =  pTileMap->GetTileInfo(i,j);
				//pTileInfo->ImgIdx
			}
		}


		for (int i = 0; i < 32; ++i)
		{
			if (i == 1)
			{
				continue;
			}
			if (i == (int)LAYER_TYPE::UI)
			{
				continue;
			}
			const vector<CObj*>& vecObjects = pLevel->GetLayer((LAYER_TYPE)i);

			for (size_t j = 0; j < vecObjects.size(); ++j)
			{
				float fDist = GetPos().Distance(vecObjects[j]->GetPos());

				if (m_Range < fDist)
					continue;

				CRigidBody* pRigidBody = vecObjects[j]->GetComponent<CRigidBody>();
				CPassiveItem* pPassiveItem = dynamic_cast<CPassiveItem*>(vecObjects[j]);
				if (nullptr == pPassiveItem)
				{
					if (vecObjects[j]->GetName() == L"Player")
					{
						CPlayer* pPlayer = dynamic_cast<CPlayer*>(vecObjects[j]);
						int pHP = pPlayer->GetCurrentHP();
						pHP -= m_Att;
						pPlayer->SetHP(pHP);
					}
					else if (vecObjects[j]->GetName() == L"Caveman")
					{
						CCaveman* pObj = dynamic_cast<CCaveman*>(vecObjects[j]);
						CStateMachine* pSm = pObj->GetComponent<CStateMachine>();
						pSm->ChangeState(L"DeadState");
					}
					else if (vecObjects[j]->GetName() == L"Shopkeeper")
					{
						CShopkeeper* pObj = dynamic_cast<CShopkeeper*>(vecObjects[j]);
						CStateMachine* pSm = pObj->GetComponent<CStateMachine>();
						pSm->ChangeState(L"DeadState");
					}
					else if (vecObjects[j]->GetName() == L"Victim")
					{
						CVictim* pObj = dynamic_cast<CVictim*>(vecObjects[j]);
						CStateMachine* pSm = pObj->GetComponent<CStateMachine>();
						pSm->ChangeState(L"DeadState");
					}
					else if (vecObjects[j]->GetName() == L"Olmec")
					{
						continue;
					}
					else if (vecObjects[j]->GetName() == L"RopeOn")
					{
						continue;
					}
					else if (vecObjects[j]->GetName() != L"BackGround")
					{
						DeleteObject(vecObjects[j]);
					}
				}


				if (nullptr == pRigidBody)
					continue;

				// 힘이 적용될 방향을 구한다.
				Vec2 ForceDir = vecObjects[j]->GetPos() - GetPos();
				ForceDir.Normalize();

				// 거리에 따라서 힘이 감소한다.
				float ForceRatio = 1.f - (fDist / m_Range);
				if (ForceRatio < 0.f)
					ForceRatio = 0.f;

				Vec2 vForce = ForceDir * m_Force * ForceRatio;

				// 해당 방향으로 Force 의 크기만큼 키워서 상대방 RigidBody 에 적용시킨다.
				pRigidBody->AddForce(vForce);
			}
		}
	}

	//DrawDebugShape(DEBUG_SHAPE::CIRCLE, PEN_TYPE::BLUE, BRUSH_TYPE::HOLLOW, GetPos(), Vec2(m_Range * 2.f, m_Range * 2.f));

	

	if (3.f < m_AccTime)
		DeleteObject(this);
}

void CForce::Render()
{
	m_Animator->Render();
}

void CForce::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (pTile)
	{
		Vec2 vCollPos = _OtherCollider->GetFinalPos();
		Vec2 vForcePos = _OwnCollider->GetFinalPos();
		int Row = vForcePos.x / 64.f;
		int NextRow = vForcePos.x / 64.f + 1;
		int PrevRow = vForcePos.x / 64.f - 1;
		int Col = (vCollPos.y - 32.f) / 64.f;

		CTileMap* pTileMap = pTile->GetComponent<CTileMap>();
		tTileInfo* pTileInfo = pTileMap->GetTileInfo(Row, Col);
		if (pTileMap != nullptr)
		{
			pTileInfo->ImgIdx = -1;
			pTileMap->SetChange(true);
		}
			
		pTileInfo = pTileMap->GetTileInfo(NextRow, Col);
		if (pTileMap != nullptr)
		{
			pTileInfo->ImgIdx = -1;
			pTileMap->SetChange(true);
		}

		pTileInfo = pTileMap->GetTileInfo(PrevRow, Col);
		if (pTileMap != nullptr)
		{
			pTileInfo->ImgIdx = -1;
			pTileMap->SetChange(true);
		}

		pTile->ChangeCombineCollider(_OtherCollider);
	}

}

void CForce::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
}
