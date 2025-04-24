#include "pch.h"
#include "CBomb.h"

#include "CRigidBody.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CTimeMgr.h"
#include "CForce.h"
#include "CPlatform.h"
#include "CTile.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CLogMgr.h"



CBomb::CBomb()
	: m_RigidBody(nullptr)
	, m_Collider(nullptr)
	, m_Animator(nullptr)
	, m_AccTime(0.f)
	, m_AddTime(0.f)
	, m_Force(nullptr)
	, m_Dir(Vec2(1.f, 1.f))
	, m_IsBounce(false)
	, m_IsThrow(true)
	, m_IsCheck(false)
	, m_Init(true)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetOffset(Vec2(0.f, 17.f));
	m_Collider->SetScale(Vec2(30.f, 30.f));


	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(1800.f);
	m_RigidBody->SetFriction(2400.f);
	m_RigidBody->SetJumpSpeed(300.f);
	m_RigidBody->SetMaxGravitySpeed(1500.f);


	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\BOMB.anim");
	m_Animator->Play(L"bomb", false);
}

CBomb::CBomb(const CBomb& _Other)
	: CItem(_Other)
	, m_RigidBody(nullptr)
	, m_Collider(nullptr)
	, m_Animator(nullptr)
	, m_AccTime(0.f)
	, m_AddTime(0.f)
	, m_Force(_Other.m_Force)
	, m_Dir(Vec2(1.f, 1.f))
	, m_Init(true)
{
	m_Animator = GetComponent<CAnimator>();
	m_Collider = GetComponent<CCollider>();
	m_RigidBody = GetComponent<CRigidBody>();
}

CBomb::~CBomb()
{

}

void CBomb::Tick()
{	
	if (m_Init)
	{
		m_AddTime += DT;

		if (1.5f < m_AddTime)
		{
			CSound* pSound = CAssetMgr::Get()->LoadSound(L"bomb_timer", L"sound\\bomb_timer.wav");
			pSound->Play();
			m_Init = false;
		}
	}
	if (m_IsThrow)
	{
		Throw(m_RigidBody, m_Dir);
	}
	

	if (m_IsBounce)
	{
		Bounce(m_RigidBody, m_Dir, m_BouceForce);
	}

	m_AccTime += DT;

	if (m_Duration < m_AccTime)
	{
		Vec2 vPos = GetPos();
		m_Force = new CForce;
		m_Force->SetName(L"BombForce");
		m_Force->SetPos(GetPos()+ Vec2(1.f,1.f));
		m_Force->SetScale(300.f, 300.f);
		m_Force->SetForce(-500.f, 200.f);
		m_Force->SetLifeTime(0.2f);
		CCamera::Get()->SetCamShake(100.f, 10.f, 0.1f);
		CreateObject(m_Force, LAYER_TYPE::EFFECT);

		CSound* pSound = CAssetMgr::Get()->LoadSound(L"kaboom", L"sound\\kaboom.wav");
		pSound->Play();

		DeleteObject(this);
		m_AccTime = 0.f;
	}

	if (!m_Collider->IsCollision())
	{
		m_RigidBody->SetGround(false);
	}
	m_PrevPos = GetPos();
}

void CBomb::Render()
{
	m_Animator->Render();
}



void CBomb::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlatform* pOtherObj = dynamic_cast<CPlatform*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (pTile)
	{
		Vec2 vPos = GetPos();
		Vec2 vCollPos = _OtherCollider->GetFinalPos();
		Vec2 vCollScale = _OtherCollider->GetScale();
		if (vPos.x < vCollPos.x && vCollPos.y - vCollScale.y / 2.f < vPos.y)
		{
			Vec2 vVelo = m_RigidBody->GetVelocity();
			m_RigidBody->SetVelocity(Vec2(-vVelo.x, 0.f));
			SetPos(vPos);
		}
		else if (vCollPos.x < vPos.x && vCollPos.y - vCollScale.y / 2.f < vPos.y)
		{
			Vec2 vVelo = m_RigidBody->GetVelocity();
			m_RigidBody->SetVelocity(Vec2(-vVelo.x, 0.f));
			SetPos(vPos);
		}
		else
		{
			Vec2 vVelo = m_RigidBody->GetVelocity();
			m_RigidBody->SetGround(true);
			m_IsThrow = false;
			m_RigidBody->SetVelocity(Vec2(vVelo.x, 0.f));
			SetForceOn(true);
		}
	}
}


