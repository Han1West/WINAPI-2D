#include "pch.h"
#include "CCaveman.h"

#include "CMonster.h"
#include "CEngine.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CStateMachine.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CStunState.h"
#include "CDeadState.h"

#include "CPlatform.h"
#include "CMissile.h"
#include "CWhip.h"
#include "CPlayer.h"
#include "CTile.h"


CCaveman::CCaveman()
	: m_Body(nullptr)
	, m_Head(nullptr)
	, m_Animator(nullptr)
	, m_RigidBody(nullptr)
	, m_Sm(nullptr)
	, m_Feet(nullptr)
	, m_Init(true)
	, m_InitDir(false)
{
	// 몬스터 정보
	tMonsterInfo pInfo = {};
	pInfo.MaxHP = 5;
	pInfo.HP = 5;
	pInfo.Speed = 400.f;
	pInfo.AttackRange = 0.f;
	pInfo.DetectRange = 800.f;

	SetMonsterInfo(pInfo);

	// 몸 충돌체
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"CavemanBody");
	m_Body->SetOffset(Vec2(0.f, 15.f));
	m_Body->SetScale(Vec2(40.f, 35.f));

	// 머리 충돌체
	m_Head = AddComponent(new CCollider);
	m_Head->SetName(L"CavemanHead");
	m_Head->SetOffset(Vec2(0.f, -10.f));
	m_Head->SetScale(Vec2(20.f, 10.f));

	// 발 충돌체
	m_Feet = AddComponent(new CCollider);
	m_Feet->SetName(L"CavemanFeet");
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
	m_Animator->LoadAnimation(L"animation\\CAVEMAN_DIE_L.anim");
	m_Animator->LoadAnimation(L"animation\\CAVEMAN_DIE_R.anim");

	m_Animator->LoadAnimation(L"animation\\CAVEMAN_HIT_L.anim");
	m_Animator->LoadAnimation(L"animation\\CAVEMAN_HIT_R.anim");

	m_Animator->LoadAnimation(L"animation\\CAVEMAN_IDLE_L.anim");
	m_Animator->LoadAnimation(L"animation\\CAVEMAN_IDLE_R.anim");

	m_Animator->LoadAnimation(L"animation\\CAVEMAN_MOVE_L.anim");
	m_Animator->LoadAnimation(L"animation\\CAVEMAN_MOVE_R.anim");

	// StateMachine
	m_Sm = AddComponent(new CStateMachine);
	m_Sm->AddState(L"IdleState", new CIdleState);
	m_Sm->AddState(L"TraceState", new CTraceState);
	m_Sm->AddState(L"StunState", new CStunState);
	m_Sm->AddState(L"DeadState", new CDeadState);

	m_Sm->ChangeState(L"IdleState");
}

CCaveman::~CCaveman()
{
}

void CCaveman::Tick()
{
	if (m_Init)
	{
		if (m_InitDir)
		{
			m_Animator->Play(L"caveman_idle_r", false);
			m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
		}
		else
		{
			m_Animator->Play(L"caveman_idle_l", false);
			m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
		}
		m_Init = false;
	}
	if (m_Sm->GetCurState()->GetName() == L"IdleState")
	{
		m_Head->Activate();
	}
}

void CCaveman::Render()
{
	m_Animator->Render();
}

void CCaveman::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{

	CPlatform* pPlatform = dynamic_cast<CPlatform*>(_OtherObj);
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CWhip* pWhip = dynamic_cast<CWhip*>(_OtherObj);
	CMissile* pMissile = dynamic_cast<CMissile*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (pTile)
	{
		if (_OwnCollider->GetName() == L"CavemanBody")
		{
			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
				m_Animator->Play(L"caveman_move_l", true);
			}
			else
			{
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
				m_Animator->Play(L"caveman_move_r", true);
			}
		}
		if (_OwnCollider->GetName() == L"CavemanFeet")
		{
			m_RigidBody->SetGround(true);
			Vec2 vPos = GetPos();
			vPos.y = _OtherCollider->GetFinalPos().y - 70.f;
			SetPos(vPos);
		}
	}

	wstring pCurState = m_Sm->GetCurState()->GetName();

	if (pWhip && pCurState != L"DeadState")
	{
		tMonsterInfo pInfo = GetMonsterInfo();
		pInfo.HP -= 1;
		SetMonsterInfo(pInfo);
		if (0 != pInfo.HP)
		{
			m_Sm->ChangeState(L"StunState");
			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
				m_Sound->Play();
				m_Head->Deactivate();
			}
			else
			{
				m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
				m_Sound->Play();
				m_Head->Deactivate();
			}
		}
		else
		{
			m_Sound = CAssetMgr::Get()->LoadSound(L"splat", L"sound\\hit.wav");
			m_Sound->Play();
			m_Sm->ChangeState(L"DeadState");
		}
	}

	if (pMissile && pCurState != L"DeadState")
	{
		m_Sound = CAssetMgr::Get()->LoadSound(L"splat", L"sound\\splat.wav");
		m_Sound->Play();
		m_Sm->ChangeState(L"DeadState");
	}

	if (_OwnCollider->GetName() == L"CavemanHead")
	{
		if (pPlayer)
		{
			if (_OtherCollider->GetName() == L"Feet Collider")
			{
				tMonsterInfo pInfo = GetMonsterInfo();
				int pDamage = pPlayer->GetFeetDamage();
				pInfo.HP -= pDamage;
				SetMonsterInfo(pInfo);
				if (0 != pInfo.HP)
				{
					m_Sm->ChangeState(L"StunState");
					if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
					{
						m_Sound = CAssetMgr::Get()->LoadSound(L"squish", L"sound\\squish.wav");
						m_Sound->Play();
						m_Head->Deactivate();
					}
					else
					{
						m_Sound = CAssetMgr::Get()->LoadSound(L"squish", L"sound\\squish.wav");
						m_Sound->Play();
						m_Head->Deactivate();
					}
				}
				else
				{
					m_Sound = CAssetMgr::Get()->LoadSound(L"squish", L"sound\\squish.wav");
					m_Sound->Play();
					m_Sm->ChangeState(L"DeadState");
				}
			}
		}
	}
}

void CCaveman::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (pTile)
	{
		if (_OwnCollider->GetName() == L"CavemanFeet")
		{
			m_RigidBody->SetGround(false);
		}
	}
}

