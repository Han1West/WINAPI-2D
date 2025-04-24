#include "pch.h"
#include "CPlatform.h"

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

#include "CPassiveItem.h"

CPlatform::CPlatform()
	: m_Collider(nullptr)
{
	m_Collider = AddComponent(new CCollider);
}

CPlatform::CPlatform(const CPlatform& _Other)
	: CObj(_Other)
	, m_Collider(nullptr)
{
	m_Collider = GetComponent<CCollider>();
}

CPlatform::~CPlatform()
{

}

void CPlatform::SetScale(Vec2 _Scale)
{
	CObj::SetScale(_Scale);

	if (m_Collider)
	{
		m_Collider->SetScale(_Scale);
	}
}

void CPlatform::SetScale(float _Width, float _Height)
{
	CObj::SetScale(_Width, _Height);

	if (m_Collider)
	{
		m_Collider->SetScale(Vec2(_Width, _Height));
	}
}

void CPlatform::Tick()
{

}

void CPlatform::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CETCObj* pCETCObj = dynamic_cast<CETCObj*>(_OtherObj);
	CShotgun* pShotgun = dynamic_cast<CShotgun*>(_OtherObj);
	CSnake* pSnake = dynamic_cast<CSnake*>(_OtherObj);
	CCaveman* pCaveman = dynamic_cast<CCaveman*>(_OtherObj);
	CSkeleton* pSkeleton = dynamic_cast<CSkeleton*>(_OtherObj);
	CSpider* pSpider = dynamic_cast<CSpider*>(_OtherObj);

	CPassiveItem* pPassiveItem = dynamic_cast<CPassiveItem*>(_OtherObj);

	if (pPlayer)
	{
		CRigidBody* pRigidBody = pPlayer->GetComponent<CRigidBody>();
		pRigidBody->SetGround(true);
	}
	if (pCETCObj)
	{
		CRigidBody* pRigidBody = pCETCObj->GetComponent<CRigidBody>();
		pRigidBody->SetGround(true);
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
	if (pCaveman)
	{
		CRigidBody* pRigidBody = pCaveman->GetComponent<CRigidBody>();
		pRigidBody->SetGround(true);
	}
	if (pSkeleton)
	{
		CRigidBody* pRigidBody = pSkeleton->GetComponent<CRigidBody>();
		pRigidBody->SetGround(true);
	}
	if (pSpider)
	{
		CRigidBody* pRigidBody = pSpider->GetComponent<CRigidBody>();
		pRigidBody->SetGround(true);
	}

	if (pPassiveItem)
	{
		CRigidBody* pRigidBody = pPassiveItem->GetComponent<CRigidBody>();
		pRigidBody->SetGround(true);
	}
}

void CPlatform::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CETCObj* pCETCObj = dynamic_cast<CETCObj*>(_OtherObj);
	CShotgun* pShotgun = dynamic_cast<CShotgun*>(_OtherObj);
	CCaveman* pCaveman = dynamic_cast<CCaveman*>(_OtherObj);
	CSkeleton* pSkeleton = dynamic_cast<CSkeleton*>(_OtherObj);
	CSpider* pSpider = dynamic_cast<CSpider*>(_OtherObj);

	if (pPlayer)
	{
		CRigidBody* pRigidBody = pPlayer->GetComponent<CRigidBody>();
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
	if (pCaveman)
	{
		CRigidBody* pRigidBody = pCaveman->GetComponent<CRigidBody>();
		pRigidBody->SetGround(false);
	}
	if (pSkeleton)
	{
		CRigidBody* pRigidBody = pSkeleton->GetComponent<CRigidBody>();
		pRigidBody->SetGround(false);
	}
	if (pSpider)
	{
		CRigidBody* pRigidBody = pSpider->GetComponent<CRigidBody>();
		pRigidBody->SetGround(false);
	}
}
