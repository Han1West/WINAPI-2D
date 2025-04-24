#include "pch.h"
#include "CDispenser.h"

#include "CEngine.h"

#include "CCollider.h"
#include "CTexture.h"
#include "CRigidBody.h"
#include "CAssetMgr.h"
#include "CStateMachine.h"
#include "CState.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CETCObj.h"
#include "CBomb.h"
#include "CShotgun.h"
#include "CPlayer.h"
#include "CMonster.h"


CDispenser::CDispenser()
	: m_Texture(nullptr)
	, m_Collider(nullptr)
	, m_IsShot(false)
	, m_IsRight(true)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Dispenser");
	m_Collider->SetOffset(Vec2(0.f, 0.f));
	m_Collider->SetScale(Vec2(64.f, 64.f));

	m_DetectCollider = AddComponent(new CCollider);
	m_DetectCollider->SetName(L"DetectCollider");
	m_DetectCollider->SetOffset(Vec2(0.f, 0.f));
	m_DetectCollider->SetScale(Vec2(1600.f, 50.f));

}

CDispenser::~CDispenser()
{
}


void CDispenser::Tick()
{
}

void CDispenser::Render()
{
	HDC dc = CEngine::Get()->GetBackDC();

	Vec2 vPos = GetRenderPos();
	UINT width = m_Texture->GetWidth();
	UINT height = m_Texture->GetHeight();

	{
		BLENDFUNCTION blend = {};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(BackDC, (int)vPos.x - width / 2, (int)vPos.y - height / 2
			, width, height, m_Texture->GetDC(), 0, 0, width, height, blend);
	}
}


void CDispenser::ShotArrow()
{
	if (false == m_IsShot)
	{
		CETCObj* pArrow = new CETCObj;
		pArrow->SetName(L"CETCObj");
		pArrow->SetAtt(2.f);
		pArrow->SetPos(GetPos());
		if (m_IsRight)
		{
			pArrow->SetIsRight(true);
			pArrow->SetTexture(CAssetMgr::Get()->LoadTexture(L"ArrowR", L"texture\\arrow.png"));
		}
		else
		{
			pArrow->SetIsRight(false);
			pArrow->SetTexture(CAssetMgr::Get()->LoadTexture(L"ArrowL", L"texture\\arrow_Left.png"));
		}
		pArrow->SetShot(true);
		CreateObject(pArrow, LAYER_TYPE::MONSTER_PROJECTILE);
		CSound* pSound = CAssetMgr::Get()->LoadSound(L"arrowshot", L"sound\\arrowshot.wav");
		pSound->Play();
		m_IsShot = true;
	}
}

void CDispenser::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	// 탐지 범위에 물체가 있고 만약 그 물체의 속도가 0이 아니라면 화살 발사	

	CObj* pObj = _OtherObj;
	CRigidBody* pRigidBody = pObj->GetComponent<CRigidBody>();
	Vec2 pObjPos = pObj->GetPos();
	Vec2 pDispenserPos = GetPos();

	if (_OwnCollider->GetName() == L"DetectCollider")
	{
		Vec2 vCollPos = _OwnCollider->GetFinalPos();
		Vec2 vOtherCollPos = _OtherCollider->GetFinalPos();
		if (pObj && pObj->GetName() == L"Tile Object")
		{
			if (m_IsRight)
			{
				if (vCollPos.x < vOtherCollPos.x)
				{
					float vDistance = _OtherCollider->GetFinalPos().x - _OwnCollider->GetFinalPos().x;
					Vec2 vScale = _OwnCollider->GetScale();
					vScale.x = vDistance + (_OtherCollider->GetScale().x * 2);
					m_DetectCollider->SetScale(vScale);
				}
			}
			else
			{
				if (vCollPos.x > vOtherCollPos.x)
				{
					float vDistance = _OtherCollider->GetFinalPos().x - _OwnCollider->GetFinalPos().x;
					Vec2 vScale = _OwnCollider->GetScale();
					vScale.x = abs(vDistance) + (_OtherCollider->GetScale().x * 2);
					m_DetectCollider->SetScale(vScale);
				}
			}	
		}
		if (pObj && pObj->GetName() != L"Tile Object")
		{
			if (m_IsRight)
			{
				if (pDispenserPos.x < pObjPos.x)
				{
					if (pObj->GetName() != L"Plarform")
					{
						Vec2 pGravityVelo = pRigidBody->GetGravityVeloicity();
						Vec2 pVelo = pRigidBody->GetVelocity();
						Vec2 ZeroVector = Vec2(0.f, 0.f);

						if (pObj->GetName() == L"Player")
						{
							CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
							CStateMachine* pSm = pPlayer->GetComponent<CStateMachine>();
							if (pSm->GetState(PLAYER_STATE::ISMOVING) || ZeroVector != pGravityVelo)
							{
								ShotArrow();
							}
						}
						else if (pObj->GetName() == L"CETCObj")
						{
							CETCObj* pETC = dynamic_cast<CETCObj*>(pObj);
							if (ZeroVector != pVelo || ZeroVector != pGravityVelo)
							{
								ShotArrow();
							}
						}
						else if (pObj->GetName() == L"Rock")
						{
							CETCObj* pETC = dynamic_cast<CETCObj*>(pObj);
							if (ZeroVector != pVelo || ZeroVector != pGravityVelo)
							{
								ShotArrow();
							}
						}
						else if (pObj->GetName() == L"Bomb")
						{
							CBomb* pBomb = dynamic_cast<CBomb*>(pObj);
							if (ZeroVector != pVelo || ZeroVector != pGravityVelo)
							{
								ShotArrow();
							}
						}
						else
						{
							CMonster* pMonster = dynamic_cast<CMonster*>(pObj);
							CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
							CState* pState = pSm->GetCurState();
							if (pState->GetName() != L"IdleState" || ZeroVector != pGravityVelo)
							{
								ShotArrow();
							}
						}
					}
				}
			}
			else
			{
				if (pObjPos.x < pDispenserPos.x)
				{
					if (pObj->GetName() != L"Plarform")
					{
						Vec2 pGravityVelo = pRigidBody->GetGravityVeloicity();
						Vec2 pVelo = pRigidBody->GetVelocity();
						Vec2 ZeroVector = Vec2(0.f, 0.f);

						if (pObj->GetName() == L"Player")
						{
							CPlayer* pPlayer = dynamic_cast<CPlayer*>(pObj);
							CStateMachine* pSm = pPlayer->GetComponent<CStateMachine>();
							if (pSm->GetState(PLAYER_STATE::ISMOVING) || ZeroVector != pGravityVelo)
							{
								ShotArrow();
							}
						}
						else if (pObj->GetName() == L"CETCObj")
						{
							CETCObj* pETC = dynamic_cast<CETCObj*>(pObj);
							if (ZeroVector != pVelo || ZeroVector != pGravityVelo)
							{
								ShotArrow();
							}
						}
						else if (pObj->GetName() == L"Rock")
						{
							CETCObj* pETC = dynamic_cast<CETCObj*>(pObj);
							if (ZeroVector != pVelo || ZeroVector != pGravityVelo)
							{
								ShotArrow();
							}
						}
						else if (pObj->GetName() == L"Bomb")
						{
							CBomb* pBomb = dynamic_cast<CBomb*>(pObj);
							if (ZeroVector != pVelo || ZeroVector != pGravityVelo)
							{
								ShotArrow();
							}
						}
						else if (pObj->GetName() == L"Shotgun")
						{
							CShotgun* pShotgub = dynamic_cast<CShotgun*>(pObj);
							if (ZeroVector != pVelo || ZeroVector != pGravityVelo)
							{
								ShotArrow();
							}
						}
						else
						{
							CMonster* pMonster = dynamic_cast<CMonster*>(pObj);
							CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
							CState* pState = pSm->GetCurState();
							if (pState->GetName() != L"IdleState" || ZeroVector != pGravityVelo)
							{
								ShotArrow();
							}
						}
					}
				}
			}

		}
	}
	
}

