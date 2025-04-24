#include "pch.h"
#include "CSkeleton.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CStateMachine.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CWakeupState.h"

#include "CPlatform.h"
#include "CMissile.h"
#include "CWhip.h"
#include "CPlayer.h"
#include "CTile.h"

CSkeleton::CSkeleton()
	: m_Body(nullptr)
	, m_Head(nullptr)
	, m_Feet(nullptr)
	, m_Animator(nullptr)
	, m_Sm(nullptr)
{
	// 몬스터 정보
	tMonsterInfo pInfo = {};
	pInfo.MaxHP = 1;
	pInfo.HP = 1;
	pInfo.Speed = 200.f;
	pInfo.DetectRange = 200.f;
	pInfo.AttackRange = 800.f;

	SetMonsterInfo(pInfo);

	// 몸 충돌체
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"SkeletonBody");
	m_Body->SetOffset(Vec2(0.f, 15.f));
	m_Body->SetScale(Vec2(40.f, 40.f));

	// 머리 충돌체
	m_Head = AddComponent(new CCollider);
	m_Head->SetName(L"SkeletonHead");
	m_Head->SetOffset(Vec2(0.f, -15.f));
	m_Head->SetScale(Vec2(20.f, 10.f));

	// 발 충돌체
	m_Feet = AddComponent(new CCollider);
	m_Feet->SetName(L"SkeletonFeet");
	m_Feet->SetOffset(Vec2(0.f, 40.f));
	m_Feet->SetScale(Vec2(10.f, 10.f));

	// 물리
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(300.f);
	m_RigidBody->SetFriction(1000.f);
	m_RigidBody->SetMaxGravitySpeed(1500.f);
	m_RigidBody->SetJumpSpeed(600.f);

	// 애니메이터
	m_Animator = AddComponent(new CAnimator);
	m_Animator->LoadAnimation(L"animation\\SKELETON_IDLE.anim");

	m_Animator->LoadAnimation(L"animation\\SKELETON_MOVE_L.anim");
	m_Animator->LoadAnimation(L"animation\\SKELETON_MOVE_R.anim");

	m_Animator->LoadAnimation(L"animation\\SKELETON_WAKEUP.anim");

	m_Animator->Play(L"skeleton_idle", false);

	// StateMachine
	m_Sm = AddComponent(new CStateMachine);
	m_Sm->AddState(L"IdleState", new CIdleState);
	m_Sm->AddState(L"TraceState", new CTraceState);
	m_Sm->AddState(L"WakeupState", new CWakeupState);

	m_Sm->ChangeState(L"IdleState");
	m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);

}

CSkeleton::~CSkeleton()
{
}


void CSkeleton::Tick()
{
}

void CSkeleton::Render()
{
	m_Animator->Render();
}

void CSkeleton::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlatform* pPlatform = dynamic_cast<CPlatform*>(_OtherObj);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CWhip* pWhip = dynamic_cast<CWhip*>(_OtherObj);
	CMissile* pMissile = dynamic_cast<CMissile*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (pTile)
	{
		if (_OwnCollider->GetName() == L"SkeletonBody")
		{
			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Animator->Play(L"skeleton_move_l", true);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
			}
			else
			{
				m_Animator->Play(L"skeleton_move_r", true);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
			}
		}

		if (_OwnCollider->GetName() == L"SkeletonFeet")
		{
			m_RigidBody->SetGround(true);
		}
		if (_OwnCollider->GetName() == L"SkeletonHead")
		{
			Vec2 vPos = GetPos();
			vPos.y = _OtherCollider->GetFinalPos().y + 50.f;
			SetPos(vPos);
			m_RigidBody->AddVelociy(Vec2(0.f, 1000000.f));
		}
	}

	if (pWhip)
	{
		m_Sound = CAssetMgr::Get()->LoadSound(L"splat", L"sound\\splat.wav");
		m_Sound->Play();
		DeleteObject(this);
	}
	

	if (pMissile)
	{
		m_Sound = CAssetMgr::Get()->LoadSound(L"splat", L"sound\\splat.wav");
		m_Sound->Play();
		DeleteObject(this);
	}

	if (_OwnCollider->GetName() == L"SkeletonHead")
	{
		//if (pPlatform)
		//{
		//	CRigidBody* pRigidBody = pPlatform->GetComponent<CRigidBody>();
		//	if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		//	{
		//		m_Animator->Play(L"skeleton_move_l", true);
		//		m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
		//	}
		//	else
		//	{
		//		m_Animator->Play(L"skeleton_move_r", true);
		//		m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
		//	}
		//}
		if (pPlayer)
		{
			if (_OtherCollider->GetName() == L"Feet Collider")
			{
				m_Sound = CAssetMgr::Get()->LoadSound(L"squish", L"sound\\squish.wav");
				m_Sound->Play();
				DeleteObject(this);
			}
		}
	}
}


