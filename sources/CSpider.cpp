#include "pch.h"
#include "CSpider.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CStateMachine.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CStunState.h"
#include "CJumpState.h"

#include "CPlatform.h"
#include "CMissile.h"
#include "CWhip.h"
#include "CPlayer.h"
#include "CTile.h"

CSpider::CSpider()
	: m_Body(nullptr)
	, m_Head(nullptr)
	, m_Animator(nullptr)
	, m_RigidBody(nullptr)
	, m_Sm(nullptr)
	, m_AccTime(0.f)
{
	// 몬스터 정보
	tMonsterInfo pInfo = {};
	pInfo.MaxHP = 5;
	pInfo.HP = 5;
	pInfo.Speed = 300.f;
	pInfo.AttackRange = 0.f;
	pInfo.DetectRange = 800.f;

	SetMonsterInfo(pInfo);

	// 몸 충돌체
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"SpiderBody");
	m_Body->SetOffset(Vec2(0.f, 25.f));
	m_Body->SetScale(Vec2(30.f, 20.f));

	// 머리 충돌체
	m_Head = AddComponent(new CCollider);
	m_Head->SetName(L"SpiderHead");
	m_Head->SetOffset(Vec2(0.f, 5.f));
	m_Head->SetScale(Vec2(20.f, 10.f));

	// 물리
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(300.f);
	m_RigidBody->SetFriction(1000.f);
	m_RigidBody->SetMaxGravitySpeed(1500.f);
	m_RigidBody->SetJumpSpeed(1000.f);
	m_RigidBody->UseGravity(false);


	// 애니메이터
	m_Animator = AddComponent(new CAnimator);
	m_Animator->LoadAnimation(L"animation\\SPIDER_DROP.anim");
	m_Animator->LoadAnimation(L"animation\\SPIDER_GROUND_IDLE.anim");
	m_Animator->LoadAnimation(L"animation\\SPIDER_IDLE.anim");
	m_Animator->LoadAnimation(L"animation\\SPIDER_JUMP.anim");

	m_Animator->Play(L"spider_idle", false);

	// StateMachine
	m_Sm = AddComponent(new CStateMachine);
	m_Sm->AddState(L"IdleState", new CIdleState);
	m_Sm->AddState(L"TraceState", new CTraceState);
	m_Sm->AddState(L"JumpState", new CJumpState);
	m_Sm->AddState(L"StunState", new CStunState);

	m_Sm->ChangeState(L"IdleState");
}

CSpider::~CSpider()
{
}

void CSpider::Tick()
{
	if (m_Sm->GetCurState()->GetName() == L"IdleState" && m_RigidBody->IsGround())
	{
		m_AccTime += DT;
	}
	if (2.f < m_AccTime)
	{
		m_Sm->ChangeState(L"JumpState");
		m_Sound = CAssetMgr::Get()->LoadSound(L"spiderjump", L"sound\\spider_jump.wav");
		m_Sound->Play();
		m_AccTime = 0.f;
	}
}

void CSpider::Render()
{
	m_Animator->Render();
}


void CSpider::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlatform* pPlatform = dynamic_cast<CPlatform*>(_OtherObj);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CWhip* pWhip = dynamic_cast<CWhip*>(_OtherObj);
	CMissile* pMissile = dynamic_cast<CMissile*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (pTile)
	{
		Vec2 vPos = GetPos();
		if (_OwnCollider->GetName() == L"SpiderBody")
		{
			Vec2 vCollPos = _OtherCollider->GetFinalPos();
			Vec2 vCollScale = _OtherCollider->GetScale();
			if (vPos.x < vCollPos.x && vCollPos.y - vCollScale.y /2.f < vPos.y)
			{
				vPos.x = vCollPos.x - vCollScale.x / 2.f - 50.f;
				SetPos(vPos);
			}
			else if (vCollPos.x < vPos.x && vCollPos.y - vCollScale.y /2.f < vPos.y)
			{
				vPos.x = vCollPos.x + vCollScale.x / 2.f + 50.f;
				SetPos(vPos);
			}
			else
			{
				m_RigidBody->SetGround(true);
			}
		}

		if (_OwnCollider->GetName() == L"SpiderHead")
		{
			//if (m_Sm->GetCurState()->GetName() == L"JumpState")
			//{
			//	m_Sm->ChangeState(L"IdleState");
			//	m_Animator->Play(L"spider_ground_idle", true);
			//}
			Vec2 vPos = GetPos();
			vPos.y = _OtherCollider->GetFinalPos().y + 50.f;
			SetPos(vPos);
			//m_RigidBody->AddVelociy(Vec2(0.f, 1000000.f));
		}

		if (m_Sm->GetCurState()->GetName() == L"JumpState")
		{
			m_Sm->ChangeState(L"IdleState");
			m_Animator->Play(L"spider_ground_idle", true);
		}
	}

	if (pWhip)
	{
		MakeEffect();
		m_Sound = CAssetMgr::Get()->LoadSound(L"splat", L"sound\\splat.wav");
		m_Sound->Play();
		DeleteObject(this);
	}


	if (pMissile)
	{
		MakeEffect();
		m_Sound = CAssetMgr::Get()->LoadSound(L"splat", L"sound\\splat.wav");
		m_Sound->Play();
		DeleteObject(this);
	}

	if (_OwnCollider->GetName() == L"SpiderHead")
	{
		if (pPlayer)
		{
			if (_OtherCollider->GetName() == L"Feet Collider")
			{
				MakeEffect();
				m_Sound = CAssetMgr::Get()->LoadSound(L"squish", L"sound\\squish.wav");
				m_Sound->Play();
				DeleteObject(this);
			}
		}
	}
}

void CSpider::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	//CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	//if (pTile)
	//{
	//	if (_OwnCollider->GetName() == L"SpiderBody")
	//	{
	//		Vec2 vPos = GetPos();
	//		Vec2 vCollPos = _OtherCollider->GetFinalPos();
	//		Vec2 vCollScale = _OtherCollider->GetScale();
	//		if (vPos.x < vCollPos.x && vCollPos.y - vCollScale.y / 2.f < vPos.y)
	//		{
	//			vPos.x -= GetMonsterInfo().Speed *DT;
	//			SetPos(vPos);
	//		}
	//		else if (vCollPos.x < vPos.x && vCollPos.y - vCollScale.y / 2.f < vPos.y)
	//		{
	//			vPos.x += GetMonsterInfo().Speed * DT;
	//			SetPos(vPos);
	//		}
	//	}
	//}
}

void CSpider::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (pTile)
	{
		if (_OwnCollider->GetName() == L"SpiderBody")
		{
			m_RigidBody->SetGround(false);
		}
	}

}


