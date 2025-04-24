#include "pch.h"
#include "CTile.h"

#include "CTileMap.h"

#include "CAssetMgr.h"
#include "CCollider.h"
#include "CStateMachine.h"
#include "CLogMgr.h"

#include "CPlayer.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CETCObj.h"
#include "CShotgun.h"
#include "CBigSnake.h"
#include "CCaveman.h"
#include "CSkeleton.h"
#include "CSnake.h"
#include "CSpider.h"
#include "CDispenser.h"
#include "CSpike.h"
#include "CVictim.h"
#include "CShopkeeper.h"
#include "CBox.h"

#include "CPassiveItem.h"

CTile::CTile()
	: m_TileMap(nullptr)
	, m_Collider(nullptr)
	, m_vecCollider{}
	, m_FirstCall(true)
{
	m_TileMap = AddComponent(new CTileMap);
}

CTile::CTile(const CTile& _Other)
	: CObj(_Other)
	, m_TileMap(nullptr)
	, m_Collider(nullptr)
{
	m_TileMap = GetComponent<CTileMap>();
}


CTile::~CTile()
{
}


void CTile::Tick()
{
	if (m_FirstCall)
	{
		//int pRow = m_TileMap->GetRow();
		//int pCol = m_TileMap->GetCol();

		//ClearCollider();
		//LOG(LOG_LEVEL::LOG, L"CALLED CHNAGE");
		//Vec2 pTileSize = m_TileMap->GetTileSize();
		//for (int i = 0; i < pRow; ++i)
		//{
		//	for (int j = 0; j < pCol; ++j)
		//	{
		//		m_Collider = AddComponent(new CCollider);
		//		m_Collider->SetOffset(Vec2((pTileSize.x / 2.f) + (j * pTileSize.x),
		//			(pTileSize.y / 2.f) + (i * pTileSize.y)));
		//		m_Collider->SetScale(pTileSize);
		//		tTileInfo* pTileInfo = m_TileMap->GetTileInfo(j, i);
		//		if (pTileInfo->ImgIdx == -1)
		//		{
		//			m_Collider->ImDeactivate();
		//		}
		//		else
		//		{
		//			m_Collider->Activate();
		//		}
		//		m_vecCollider.push_back(m_Collider);
		//	}
		//}
		CombineCollider();
		m_FirstCall = false;
	}

	//if (m_TileMap->IsChange())
	//{
	//	int pRow = m_TileMap->GetRow();
	//	int pCol = m_TileMap->GetCol();
	//	for (int i = 0; i < pRow; ++i)
	//	{
	//		for (int j = 0; j < pCol; ++j)
	//		{
	//			tTileInfo* pTileInfo = m_TileMap->GetTileInfo(j, i);
	//			if (pTileInfo->ImgIdx == -1)
	//			{
	//				m_vecCollider[j + i * pCol]->Deactivate();
	//			}
	//		}
	//	}
	//	m_TileMap->SetChange(false);
	//}
}

void CTile::Render()
{
	m_TileMap->Render();
}


void CTile::CombineCollider()
{
	CCollider* pCollider = AddComponent(new CCollider);
	
	int pRow = m_TileMap->GetRow();
	int pCol = m_TileMap->GetCol();
	
	Vec2 vStartPos = {};
	Vec2 VEndPos = {};

	int StartIdx = 0;

	Vec2 pTileSize = m_TileMap->GetTileSize();

	// 시작 좌표를 구한다

	bool bMake = false;

	for (int i = 0; i < pRow; ++i)
	{
		for (int j = 0; j < pCol; ++j)
		{
			tTileInfo* pTileInfo = m_TileMap->GetTileInfo(j, i);
			
			// 시작할 좌표를 구한다
			// 만약 Collider를 만드는 중이라면 기록하지 않음
			if (pTileInfo->ImgIdx != -1 && !bMake)
			{
				StartIdx = j;
				vStartPos = Vec2((pTileSize.x / 2.f) + (StartIdx * pTileSize.x),
					(pTileSize.y / 2.f) + (i * pTileSize.y));
				bMake = true;
			}

			// 마지막 좌표를 구하고 Collider를 만든다.
			if (pTileInfo->ImgIdx == -1 && bMake)
			{
				VEndPos = Vec2((pTileSize.x / 2.f) + ((j - 1)* pTileSize.x),
					(pTileSize.y / 2.f) + (i * pTileSize.y));

				m_Collider = AddComponent(new CCollider);
				m_Collider->SetOffset(Vec2((vStartPos.x + VEndPos.x) / 2.f, 
									(pTileSize.y / 2.f) + (i * pTileSize.y)));
				m_Collider->SetScale(Vec2(pTileSize.x * (j-StartIdx), pTileSize.y));
				m_vecCollider.push_back(m_Collider);

				bMake = false;
			}

			// 마지막 좌표가 타일의 끝인 경우
			if (j == pCol-1 && bMake)
			{
				VEndPos = Vec2((pTileSize.x / 2.f) + (j * pTileSize.x),
					(pTileSize.y / 2.f) + (i * pTileSize.y));

				m_Collider = AddComponent(new CCollider);
				m_Collider->SetOffset(Vec2((vStartPos.x + VEndPos.x) / 2.f,
					(pTileSize.y / 2.f) + (i * pTileSize.y)));
				m_Collider->SetScale(Vec2(pTileSize.x * (j- StartIdx + 1) , pTileSize.y));
				m_vecCollider.push_back(m_Collider);

				bMake = false;
			}
		}
	}
}

void CTile::ChangeCombineCollider(CCollider* _Collider)
{
	Vec2 vCollPos = _Collider->GetFinalPos();
	Vec2 vCollSize = _Collider->GetScale();
	bool vMake = false;

	// 충돌한 Collider의 Y좌표를 (Row)값을 구한다.
	int CurRow = (vCollPos.y - (vCollSize.y/2.f)) / vCollSize.y;
	int pCol = m_TileMap->GetCol();
	int StartIdx = 0;

	// 다시 그릴 줄들의 Collider는 삭제해준다.
	for (int i = 0; i < m_vecCollider.size(); ++i)
	{
		if (m_vecCollider[i]->GetFinalPos().y == _Collider->GetFinalPos().y)
			m_vecCollider[i]->Deactivate();
	}

	Vec2 vStartPos = {};
	Vec2 VEndPos = {};
	Vec2 pTileSize = m_TileMap->GetTileSize();
	
	// 그 줄에 대해서 Collider를 다시 그린다.
	for (int j = 0; j < pCol; ++j)
	{
		tTileInfo* pTileInfo = m_TileMap->GetTileInfo(j, CurRow);

		// 시작할 좌표를 구한다
		// 만약 Collider를 만드는 중이라면 기록하지 않음
		if (pTileInfo->ImgIdx != -1 && !vMake)
		{
			StartIdx = j;
			vStartPos = Vec2((pTileSize.x / 2.f) + (StartIdx * pTileSize.x),
				(pTileSize.y / 2.f) + (CurRow * pTileSize.y));
			vMake = true;
		}

		// 마지막 좌표를 구하고 Collider를 만든다.
		if (pTileInfo->ImgIdx == -1 && vMake)
		{
			VEndPos = Vec2((pTileSize.x / 2.f) + ((j - 1)* pTileSize.x),
				(pTileSize.y / 2.f) + (CurRow * pTileSize.y));

			m_Collider = AddComponent(new CCollider);
			m_Collider->SetOffset(Vec2((vStartPos.x + VEndPos.x) / 2.f, 
								(pTileSize.y / 2.f) + (CurRow * pTileSize.y)));
			m_Collider->SetScale(Vec2(pTileSize.x * (j-StartIdx), pTileSize.y));
			m_vecCollider.push_back(m_Collider);

			vMake = false;
		}

		// 마지막 좌표가 타일의 끝인 경우
		if (j == pCol-1 && vMake)
		{
			VEndPos = Vec2((pTileSize.x / 2.f) + (j * pTileSize.x),
				(pTileSize.y / 2.f) + (CurRow * pTileSize.y));

			m_Collider = AddComponent(new CCollider);
			m_Collider->SetOffset(Vec2((vStartPos.x + VEndPos.x) / 2.f,
				(pTileSize.y / 2.f) + (CurRow * pTileSize.y)));
			m_Collider->SetScale(Vec2(pTileSize.x * (j- StartIdx + 1) , pTileSize.y));
			m_vecCollider.push_back(m_Collider);

			vMake = false;
		}
	}

	// 원래 Collider는 삭제한다.
	//_Collider->Deactivate();
	//DeleteObject(_Collider);

}



void CTile::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CETCObj* pCETCObj = dynamic_cast<CETCObj*>(_OtherObj);
	CShotgun* pShotgun = dynamic_cast<CShotgun*>(_OtherObj);
	CSnake* pSnake = dynamic_cast<CSnake*>(_OtherObj);
	CBigSnake* pBigSnake = dynamic_cast<CBigSnake*>(_OtherObj);
	CCaveman* pCaveman = dynamic_cast<CCaveman*>(_OtherObj);
	CSkeleton* pSkeleton = dynamic_cast<CSkeleton*>(_OtherObj);
	CSpider* pSpider = dynamic_cast<CSpider*>(_OtherObj);
	CVictim* pVictim = dynamic_cast<CVictim*>(_OtherObj);
	CShopkeeper* pShopkeeper = dynamic_cast<CShopkeeper*>(_OtherObj);
	CBox* pBox = dynamic_cast<CBox*>(_OtherObj);

	CPassiveItem* pPassiveItem = dynamic_cast<CPassiveItem*>(_OtherObj);

	if (pPlayer)
	{
		if (_OtherCollider->GetName() == L"Feet Collider")
		{
			CRigidBody* pRigidBody = pPlayer->GetComponent<CRigidBody>();
			pRigidBody->SetGround(true);
		}
	}
	if (pShopkeeper)
	{
		if (_OtherCollider->GetName() == L"ShopkeeperFeet")
		{
			CRigidBody* pRigidBody = pShopkeeper->GetComponent<CRigidBody>();
			Vec2 vTilePos = _OwnCollider->GetFinalPos();
			Vec2 vMonPos = pShopkeeper->GetPos();
			vMonPos.y = vTilePos.y - 70.f;
			pShopkeeper->SetPos(vMonPos);
			pRigidBody->SetGround(true);
		}
	}
	if (pCETCObj)
	{
		CRigidBody* pRigidBody = pCETCObj->GetComponent<CRigidBody>();
		//pRigidBody->SetGround(true);
	}
	if (pShotgun)
	{
		CRigidBody* pRigidBody = pShotgun->GetComponent<CRigidBody>();
		pRigidBody->SetGround(true);
	}
	if (pSnake)
	{
		CRigidBody* pRigidBody = pSnake->GetComponent<CRigidBody>();
		pRigidBody->SetGround(true);
	}
	if (pBigSnake)
	{
		CRigidBody* pRigidBody = pBigSnake->GetComponent<CRigidBody>();
		pRigidBody->SetGround(true);
	}
	if (pCaveman)
	{

		if (_OtherCollider->GetName() == L"CavemanFeet")
		{
			CRigidBody* pRigidBody = pCaveman->GetComponent<CRigidBody>();
			/*Vec2 vTilePos = _OwnCollider->GetFinalPos();
			Vec2 vMonPos = pCaveman->GetPos();
			vMonPos.y = vTilePos.y - 70.f;
			pCaveman->SetPos(vMonPos);*/
			pRigidBody->SetGround(true);
		}
	}
	if (pSkeleton)
	{
		if (_OtherCollider->GetName() == L"SkeletonFeet")
		{
			CRigidBody* pRigidBody = pSkeleton->GetComponent<CRigidBody>();
			//Vec2 vTilePos = _OwnCollider->GetFinalPos();
			//Vec2 vMonPos = pSkeleton->GetPos();
			//vMonPos.y = vTilePos.y - 70.f;
			//pSkeleton->SetPos(vMonPos);
			pRigidBody->SetGround(true);
		}
	}
	if (pSpider)
	{
		CRigidBody* pRigidBody = pSpider->GetComponent<CRigidBody>();
		//pRigidBody->SetGround(true);
	}

	if (pPassiveItem)
	{
		CRigidBody* pRigidBody = pPassiveItem->GetComponent<CRigidBody>();
		pRigidBody->SetGround(true);
	}

	if (pBox)
	{
		CRigidBody* pRigidBody = pBox->GetComponent<CRigidBody>();
		pRigidBody->SetGround(true);
	}
}


void CTile::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CETCObj* pCETCObj = dynamic_cast<CETCObj*>(_OtherObj);
	CShotgun* pShotgun = dynamic_cast<CShotgun*>(_OtherObj);
	CCaveman* pCaveman = dynamic_cast<CCaveman*>(_OtherObj);
	CSkeleton* pSkeleton = dynamic_cast<CSkeleton*>(_OtherObj);
	CSpider* pSpider = dynamic_cast<CSpider*>(_OtherObj);
	CVictim* pVictim = dynamic_cast<CVictim*>(_OtherObj);
	CShopkeeper* pShopkeeper = dynamic_cast<CShopkeeper*>(_OtherObj);

	if (pShopkeeper && _OtherCollider->GetName() == L"ShopkeeperFeet")
	{
		CRigidBody* pRigidBody = pShopkeeper->GetComponent<CRigidBody>();
		pRigidBody->SetGround(false);
	}

	if (pVictim && _OtherCollider->GetName() == L"VictimFeet")
	{
		CRigidBody* pRigidBody = pVictim->GetComponent<CRigidBody>();
		pRigidBody->SetGround(false);
	}
	if (pCETCObj)
	{
		CRigidBody* pRigidBody = pCETCObj->GetComponent<CRigidBody>();
		pRigidBody->SetGround(false);
	}
	if (pShotgun)
	{
		CRigidBody* pRigidBody = pShotgun->GetComponent<CRigidBody>();
		pRigidBody->SetGround(false);
	}
}
