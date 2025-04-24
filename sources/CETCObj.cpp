#include "pch.h"
#include "CETCObj.h"

#include "CEngine.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CCollider.h"
#include "CRigidBody.h"
#include "CStateMachine.h"
#include "CTexture.h"
#include "CAssetMgr.h"

#include "CTimeMgr.h"
#include "CObj.h"
#include "CMonster.h"
#include "CPlayer.h"
#include "CTile.h"

#include "CLogMgr.h"


CETCObj::CETCObj()
	: m_Collider(nullptr)
	, m_Texture(nullptr)
	, m_RigidBody(nullptr)
	, m_Dir(Vec2(1.f,1.f))
	, m_IsThrown(false)
	, m_Att(1)
	, m_InitThrow(true)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetScale(Vec2(20.f, 20.f));

	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(2000.f);
	m_RigidBody->SetFriction(2400.f);
	m_RigidBody->SetJumpSpeed(300.f);
	m_RigidBody->SetMaxGravitySpeed(1500.f);
}


CETCObj::CETCObj(const CETCObj& _Other)
	: CItem(_Other)
	, m_Collider(nullptr)
	, m_Texture(_Other.m_Texture)
	, m_RigidBody(nullptr)
	, m_Dir(Vec2(1.f, 1.f))
	, m_IsThrown(false)
{
	m_Collider = GetComponent<CCollider>();
	m_RigidBody = GetComponent<CRigidBody>();
}


CETCObj::~CETCObj()
{

}

void CETCObj::Tick()
{
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	CStateMachine* pStateMachine = pPlayer->GetComponent<CStateMachine>();
	Vec2 vPPos = pPlayer->GetPos();
	Vec2 vPScale = pPlayer->GetScale();
	bool pRight = pStateMachine->GetState(PLAYER_STATE::ISRIGHT);
	bool pUp = pStateMachine->GetState(PLAYER_STATE::ISUP);
	bool pDown = pStateMachine->GetState(PLAYER_STATE::ISDOWN);

	if (m_IsHold)
	{
		if(pRight)
			SetPos(vPPos.x + 20.f / 2.f, vPPos.y + (vPScale.y / 2.f));
		else
			SetPos(vPPos.x - 20.f / 2.f, vPPos.y + (vPScale.y / 2.f));
		
	}
	
	else if(m_IsThrown)
	{		
		if (m_InitThrow)
		{
			SetForceOn(true);
			m_InitThrow = false;
		}
			
		Throw(m_RigidBody, m_Dir);
		m_AccTime += DT;
	}


	if (0.6f < m_AccTime)
	{
		m_IsThrown = false;
		ResetTime();
		SetForceOn(true);
		m_AccTime = 0.f;
	}

	if (m_IsShot)
	{
		Shot(m_IsRight);
	}
	
}

void CETCObj::Render()
{
	Vec2 vPos = GetRenderPos();
	UINT width = m_Texture->GetWidth();
	UINT height = m_Texture->GetHeight();

	// AlphaBlending
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

void CETCObj::SetGravityUse(bool _Use)
{
	m_RigidBody->UseGravity(_Use);
}

void CETCObj::Shot(bool _IsRight)
{
	m_RigidBody->UseGravity(false);
	if (_IsRight)
	{
		m_RigidBody->AddForce(Vec2(1000000.f, 0));
	}
	else
	{
		m_RigidBody->AddForce(Vec2(-1000000.f, 0));
	}
}

void CETCObj::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CMonster* pMonster = dynamic_cast<CMonster*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);


	if (pPlayer)
	{
		m_CanHold = true;
		if (250.f < m_RigidBody->GetVelocity().Length())
		{
			if (_OtherCollider->GetName() == L"Body Collider")
			{
				CStateMachine* pSm = pPlayer->GetComponent<CStateMachine>();
				int pHP = pPlayer->GetCurrentHP();
				pHP -= m_Att;
				pPlayer->SetHP(pHP);
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				pSm->SetState(PLAYER_STATE::ISSTUN, true);
				pSm->SetState(PLAYER_STATE::ISHIT, true);

				CETCObj* pBrokenArrow = new CETCObj;
				pBrokenArrow->SetName(L"CETCObj");
				pBrokenArrow->SetAtt(0.f);
				pBrokenArrow->SetPos(GetPos());
				pBrokenArrow->SetTexture(CAssetMgr::Get()->LoadTexture(L"BrokenArrow", L"texture\\arrow_used.png"));
				DeleteObject(this);
				CreateObject(pBrokenArrow, LAYER_TYPE::ACTIVEITEM);
			}
		}
	}

	if (pTile)
	{
		if (m_IsShot)
		{
			m_RigidBody->SetVelocity(Vec2(0.f, 0.f));
			m_IsShot = false;
			m_RigidBody->UseGravity(true);
		}
		else
		{
			Vec2 vPos = GetPos();
			Vec2 vCollPos = _OtherCollider->GetFinalPos();
			Vec2 vCollScale = _OtherCollider->GetScale();
			Vec2 vOwnPos = _OwnCollider->GetFinalPos();
			if (vOwnPos.x < vCollPos.x && vCollPos.y - vCollScale.y / 2.f < vOwnPos.y)
			{
				Vec2 vVelo = m_RigidBody->GetVelocity();
				m_RigidBody->SetVelocity(Vec2(-vVelo.x, 0.f));
				//SetPos(vPos);
			}
			else if (vCollPos.x < vOwnPos.x && vCollPos.y - vCollScale.y / 2.f < vOwnPos.y)
			{
				Vec2 vVelo = m_RigidBody->GetVelocity();
				m_RigidBody->SetVelocity(Vec2(-vVelo.x, 0.f));
				//SetPos(vPos);
			}
			else
			{
				Vec2 vVelo = m_RigidBody->GetVelocity();
				vPos.y -= 5.f;
				SetPos(vPos);
				m_RigidBody->SetGround(true);
				m_IsThrown = false;
				m_RigidBody->SetVelocity(Vec2(vVelo.x, 0.f));
				SetForceOn(true);
			}
		}
	}

	if (pMonster)
	{
		if (250.f < m_RigidBody->GetVelocity().Length())
		{
			if (pMonster)
			{
				CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
				if (pMonster->GetName() == L"Caveman")
				{
					tMonsterInfo pInfo = pMonster->GetMonsterInfo();
					pInfo.HP -= m_Att;
					pMonster->SetMonsterInfo(pInfo);
					if (pInfo.HP == 0)
						pSm->ChangeState(L"DeadState");
					else
						pSm->ChangeState(L"StunState");
				}
				else if (pMonster->GetName() == L"Shopkeeper")
				{
					tMonsterInfo pInfo = pMonster->GetMonsterInfo();
					pInfo.HP -= m_Att;
					pMonster->SetMonsterInfo(pInfo);
					if (pInfo.HP == 0)
						pSm->ChangeState(L"DeadState");
					else
						pSm->ChangeState(L"StunState");
				}
				else if (pMonster->GetName() == L"Victim")
				{
				tMonsterInfo pInfo = pMonster->GetMonsterInfo();
				pInfo.HP -= m_Att;
				pMonster->SetMonsterInfo(pInfo);
				if (pInfo.HP == 0)
					pSm->ChangeState(L"DeadState");
				else
					pSm->ChangeState(L"StunState");
				}
				else
				{
					MakeEffect();
					DeleteObject(pMonster);
				}
			}
		}
	}
}

void CETCObj::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);

	if (pPlayer)
	{
		m_CanHold = false;
	}
}
