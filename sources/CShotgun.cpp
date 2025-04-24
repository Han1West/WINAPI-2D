#include "pch.h"
#include "CShotgun.h"

#include "CEngine.h"

#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"

#include "CAnimator.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CStateMachine.h"
#include "CTexture.h"

#include "CTimeMgr.h"
#include "CPlayer.h"

#include "CLogMgr.h"

CShotgun::CShotgun()
	: m_Collider(nullptr)
	, m_Animator(nullptr)
	, m_RigidBody(nullptr)
	, m_Dir(Vec2(1.f, 1.f))
	, m_IsThrown(false)
	, m_Price(0)
	, m_IsDisplay(false)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetOffset(Vec2(0.f, 10.f));
	m_Collider->SetScale(Vec2(40.f, 20.f));

	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(5.f);
	m_RigidBody->SetMaxSpeed(1000.f);
	m_RigidBody->SetFriction(2400.f);
	m_RigidBody->SetJumpSpeed(300.f);
	m_RigidBody->SetMaxGravitySpeed(1500.f);

	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\SHOTGUN_R.anim");
	m_Animator->LoadAnimation(L"animation\\SHOTGUN_L.anim");

	m_Animator->Play(L"shotgun_r", false);
}

CShotgun::~CShotgun()
{
}

void CShotgun::Tick()
{
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	if (nullptr == pPlayer)
		return;
	CStateMachine* pStateMachine = pPlayer->GetComponent<CStateMachine>();
	Vec2 vPPos = pPlayer->GetPos();
	Vec2 vPScale = pPlayer->GetScale();
	bool pRight = pStateMachine->GetState(PLAYER_STATE::ISRIGHT);


	if (m_IsHold && !m_IsNPC)
	{
		if (pRight)
		{
			SetPos(vPPos.x + 40.f / 2.f, vPPos.y + (vPScale.y / 2.f) - 15.f);
			m_Animator->Play(L"shotgun_r", false);
		}
		else
		{
			SetPos(vPPos.x - 40.f / 2.f, vPPos.y + (vPScale.y / 2.f) - 15.f);
			m_Animator->Play(L"shotgun_l", false);

		}		
	}
	else if (m_IsNPC)
	{
		if (m_NPCDirRight)
		{
			m_Animator->Play(L"shotgun_r", false);
		}
		else
		{
			m_Animator->Play(L"shotgun_l", false);
		}
	}

	else if (m_IsThrown)
	{
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

}

void CShotgun::Render()
{
	m_Animator->Render();
}

void CShotgun::SetGravityUse(bool _Use)
{
	m_RigidBody->UseGravity(_Use);
}

void CShotgun::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);

	if (pPlayer)
	{
		m_CanHold = true;
	}
}

void CShotgun::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);

	if (pPlayer)
	{
		m_CanHold = false;
	}
}

