#include "pch.h"
#include "CBigSnake.h"

#include "CEngine.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
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
#include "CSplit.h"
#include "CSound.h"

CBigSnake::CBigSnake()
	: m_Body(nullptr)
	, m_Head(nullptr)
	, m_Animator(nullptr)
	, m_RigidBody(nullptr)
	, m_Sm(nullptr)
	, m_AccTime(0.f)
{
	// 몸 충돌체
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"BigSnakeBody");
	m_Body->SetOffset(Vec2(0.f, 20.f));
	m_Body->SetScale(Vec2(40.f, 40.f));

	// 머리 충돌체
	m_Head = AddComponent(new CCollider);
	m_Head->SetName(L"BigSnakeHead");
	m_Head->SetOffset(Vec2(0.f, 5.f));
	m_Head->SetScale(Vec2(40.f, 10.f));
	
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
	m_Animator->LoadAnimation(L"animation\\BIGSNAKE_MOVE_L.anim");
	m_Animator->LoadAnimation(L"animation\\BIGSNAKE_MOVE_R.anim");

	m_Animator->LoadAnimation(L"animation\\BIGSNAKE_ATTACK_L.anim");
	m_Animator->LoadAnimation(L"animation\\BIGSNAKE_ATTACK_R.anim");

	m_Animator->Play(L"bigsnake_move_r", true);

	// StateMachine
	m_Sm = AddComponent(new CStateMachine);
	m_Sm->AddState(L"PatrolState", new CPatrolState);
	m_Sm->AddState(L"AttackState", new CAttackState);

	m_Sm->ChangeState(L"PatrolState");
	m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
}

CBigSnake::~CBigSnake()
{
}


void CBigSnake::Tick()
{

	m_AccTime += DT;

	if (1.f < m_AccTime)
	{
		if (m_Sm->GetCurState()->GetName() == L"PatrolState")
		{
			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Sm->ChangeState(L"AttackState");
				m_Animator->Play(L"bigsnake_attack_r", false);
				CSplit* pSplit = new CSplit;
				pSplit->SetName(L"Split");
				pSplit->SetScale(10.f, 10.f);
				pSplit->SetPos(GetPos().x + 10.f, GetPos().y + 20.f);
				pSplit->SetVelocity(Vec2(1.f, -0.3f));
				pSplit->SetRight(true);
				pSplit->SetTexture(CAssetMgr::Get()->LoadTexture(L"SplitR", L"texture\\split.png"));
				pSplit->SetSpeed(600.f);
				CSound *pSound = CAssetMgr::Get()->LoadSound(L"sizzle", L"sound\\vsnake_sizzle.wav");
				pSound->Play();
				CreateObject(pSplit, LAYER_TYPE::MONSTER_PROJECTILE);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
			}
			else
			{
				m_Sm->ChangeState(L"AttackState");
				m_Animator->Play(L"bigsnake_attack_l", false);
				CSplit* pSplit = new CSplit;
				pSplit->SetName(L"Split");
				pSplit->SetScale(10.f, 10.f);
				pSplit->SetPos(GetPos().x - 10.f, GetPos().y + 20.f);
				pSplit->SetVelocity(Vec2(-1.f, -0.5f));
				pSplit->SetRight(false);
				pSplit->SetSpeed(600.f);
				pSplit->SetTexture(CAssetMgr::Get()->LoadTexture(L"SplitL", L"texture\\split_left.png"));
				CSound* pSound = CAssetMgr::Get()->LoadSound(L"sizzle", L"sound\\vsnake_sizzle.wav");
				pSound->Play();
				CreateObject(pSplit, LAYER_TYPE::MONSTER_PROJECTILE);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
			}
		}

		else if (m_Sm->GetCurState()->GetName() == L"AttackState")
		{
			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Sm->ChangeState(L"PatrolState");
				m_Animator->Play(L"bigsnake_move_r", true);
			}
			else
			{
				m_Sm->ChangeState(L"PatrolState");
				m_Animator->Play(L"bigsnake_move_l", true);
			}
		}
		m_AccTime = 0.f;
	}


}

void CBigSnake::Render()
{
	m_Animator->Render();
}

void CBigSnake::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CWhip* pWhip = dynamic_cast<CWhip*>(_OtherObj);
	CMissile* pMissile = dynamic_cast<CMissile*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (pTile)
	{
		if (_OwnCollider->GetName() == L"BigSnakeHead")
		{
			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Animator->Play(L"bigsnake_move_l", true);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
			}
			else
			{
				m_Animator->Play(L"bigsnake_move_r", true);
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

	if (_OwnCollider->GetName() == L"BigSnakeHead")
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
				m_Animator->Play(L"bigsnake_attack_r", false);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"snakebite", L"sound\\snakebite.wav");
				m_Sound->Play();
			}
			else
			{
				m_Sm->ChangeState(L"AttackState");
				m_Animator->Play(L"bigsnake_attack_l", false);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
				m_Sound = CAssetMgr::Get()->LoadSound(L"snakebite", L"sound\\snakebite.wav");
				m_Sound->Play();
			}
		}
	}
}

void CBigSnake::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (_OwnCollider->GetName() == L"BigSnakeBody")
	{
		if (pPlayer)
		{
			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Sm->ChangeState(L"PatrolState");
				m_Animator->Play(L"bigsnake_move_r", true);
			}
			else if (!m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Sm->ChangeState(L"PatrolState");
				m_Animator->Play(L"bigsnake_move_l", true);
			}
		}
	}


	if (pTile)
	{
		if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			m_Animator->Play(L"bigsnake_move_l", true);
			m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
		}
		else
		{
			m_Animator->Play(L"bigsnake_move_r", true);
			m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
		}
	}
}

