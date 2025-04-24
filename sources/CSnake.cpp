#include "pch.h"
#include "CSnake.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CStateMachine.h"
#include "CPatrolState.h"
#include "CAttackState.h"

#include "CPlatform.h"
#include "CMissile.h"
#include "CWhip.h"
#include "CPlayer.h"
#include "CTile.h"
#include "CEffect.h"

CSnake::CSnake()
	: m_Body(nullptr)
	, m_Head(nullptr)
	, m_Animator(nullptr)
	, m_RigidBody(nullptr)
	, m_Sm(nullptr)
{
	// 몸 충돌체
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"SnakeBody");
	m_Body->SetOffset(Vec2(0.f, 30.f));
	m_Body->SetScale(Vec2(30.f, 30.f));

	// 머리 충돌체
	m_Head = AddComponent(new CCollider);
	m_Head->SetName(L"SnakeHead");
	m_Head->SetOffset(Vec2(0.f, 5.f));
	m_Head->SetScale(Vec2(30.f, 10.f));

	//// 발 충돌체
	//m_Feet = AddComponent(new CCollider);
	//m_Feet->SetName(L"SnakeBottom");
	//m_Feet->SetOffset(Vec2(0.f, 40.f));
	//m_Feet->SetScale(Vec2(10.f, 10.f));

	// 물리
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(300.f);
	m_RigidBody->SetFriction(1000.f);
	m_RigidBody->SetMaxGravitySpeed(1500.f);
	m_RigidBody->SetJumpSpeed(600.f);
	m_RigidBody->SetGround(false);

	// 애니메이터 
	m_Animator = AddComponent(new CAnimator);
	m_Animator->LoadAnimation(L"animation\\SNAKE_MOVE_L.anim");
	m_Animator->LoadAnimation(L"animation\\SNAKE_MOVE_R.anim");

	m_Animator->LoadAnimation(L"animation\\SNAKE_ATTACK_L.anim");
	m_Animator->LoadAnimation(L"animation\\SNAKE_ATTACK_R.anim");

	m_Animator->Play(L"snake_move_r", true);

	// StateMachine
	m_Sm = AddComponent(new CStateMachine);
	m_Sm->AddState(L"PatrolState", new CPatrolState);
	m_Sm->AddState(L"AttackState", new CAttackState);

	m_Sm->ChangeState(L"PatrolState");
	m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
}

CSnake::~CSnake()
{
}

void CSnake::Tick()
{
}

void CSnake::Render()
{
	m_Animator->Render();
}

void CSnake::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CWhip* pWhip = dynamic_cast<CWhip*>(_OtherObj);
	CMissile* pMissile = dynamic_cast<CMissile*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (pTile)
	{
		if (_OwnCollider->GetName() == L"SnakeHead")
		{
			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Animator->Play(L"snake_move_l", true);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
			}
			else
			{
				m_Animator->Play(L"snake_move_r", true);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
			}
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

	if (_OwnCollider->GetName() == L"SnakeHead")
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

	if (_OwnCollider->GetName() == L"SnakeBody")
	{
		if (pPlayer)
		{
			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Sm->ChangeState(L"AttackState");
				m_Animator->Play(L"snake_attack_r", false);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"snakebite", L"sound\\snakebite.wav");
				m_Sound->Play();
			}
			else
			{
				m_Sm->ChangeState(L"AttackState");
				m_Animator->Play(L"snake_attack_l", false);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
				m_Sound = CAssetMgr::Get()->LoadSound(L"snakebite", L"sound\\snakebite.wav");
				m_Sound->Play();
			}
		}
	}
}

void CSnake::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CPlatform* pPlatform = dynamic_cast<CPlatform*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (_OwnCollider->GetName() == L"SnakeBody")
	{
		if (pPlayer)
		{
			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Sm->ChangeState(L"PatrolState");
				m_Animator->Play(L"snake_move_r", true);
			}
			else if(!m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Sm->ChangeState(L"PatrolState");
				m_Animator->Play(L"snake_move_l", true);
			}
		}
	}


	if (pTile)
	{
		if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			m_Animator->Play(L"snake_move_l", true);
			m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
		}
		else
		{
			m_Animator->Play(L"snake_move_r", true);
			m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
		}
	}
}

