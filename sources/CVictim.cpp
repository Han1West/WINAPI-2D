#include "pch.h"
#include "CVictim.h"

#include "CMonster.h"
#include "CEngine.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CStateMachine.h"
#include "CIdleState.h"
#include "CStunState.h"
#include "CHoldState.h"
#include "CPatrolState.h"
#include "CDeadState.h"
#include "CExitState.h"

#include "CTile.h"
#include "CMissile.h"
#include "CWhip.h"
#include "CPlayer.h"
#include "CExit.h"

CVictim::CVictim()
	: m_Body(nullptr)
	, m_Head(nullptr)
	, m_Animator(nullptr)
	, m_RigidBody(nullptr)
	, m_Sm(nullptr)
	, m_Feet(nullptr)
	, m_AccTime(0.f)
	, m_ForceTime(0.f)
	, m_StunTime(0.f)
	, m_Dir{}
	, m_ForceOn(false)
	, m_IsHold(false)
	, m_IsThrown(false)
	, m_CanHold(false)
	, m_InitDir(false)
	, m_Init(true)
	, m_Delay(0.f)
{
	// 몬스터 정보
	tMonsterInfo pInfo = {};
	pInfo.MaxHP = 5;
	pInfo.HP = 5;
	pInfo.Speed = 400.f;
	pInfo.AttackRange = 0.f;
	pInfo.DetectRange = 1000.f;

	SetMonsterInfo(pInfo);

	// 몸 충돌체
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"VictimBody");
	m_Body->SetOffset(Vec2(0.f, 15.f));
	m_Body->SetScale(Vec2(40.f, 30.f));

	// 발 충돌체
	m_Feet = AddComponent(new CCollider);
	m_Feet->SetName(L"VictimFeet");
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
	m_Animator->LoadAnimation(L"animation\\NPC_DIE_R.anim");
	m_Animator->LoadAnimation(L"animation\\NPC_DIE_L.anim");

	m_Animator->LoadAnimation(L"animation\\NPC_HIT_R.anim");
	m_Animator->LoadAnimation(L"animation\\NPC_HIT_L.anim");

	m_Animator->LoadAnimation(L"animation\\NPC_MOVE_R.anim");
	m_Animator->LoadAnimation(L"animation\\NPC_MOVE_L.anim");

	m_Animator->LoadAnimation(L"animation\\NPC_IDLE_R.anim");
	m_Animator->LoadAnimation(L"animation\\NPC_IDLE_L.anim");

	m_Animator->LoadAnimation(L"animation\\NPC_ENTER.anim");

	// StateMachine
	m_Sm = AddComponent(new CStateMachine);
	m_Sm->AddState(L"IdleState", new CIdleState);
	m_Sm->AddState(L"PatrolState", new CPatrolState);
	m_Sm->AddState(L"HoldState", new CHoldState);
	m_Sm->AddState(L"StunState", new CStunState);
	m_Sm->AddState(L"DeadState", new CDeadState);
	m_Sm->AddState(L"ExitState", new CExitState);

	m_Sm->ChangeState(L"IdleState");

}

CVictim::~CVictim()
{
}

void CVictim::Tick()
{
	if (m_Init)
	{
		if (m_InitDir)
		{
			m_Animator->Play(L"npc_idle_r", false);
			m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
		}
		else
		{
			m_Animator->Play(L"npc_idle_l", false);
			m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
		}
		m_Init = false;
	}
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	CStateMachine* pStateMachine = pPlayer->GetComponent<CStateMachine>();
	Vec2 vPPos = pPlayer->GetPos();
	Vec2 vPScale = pPlayer->GetScale();
	bool pRight = pStateMachine->GetState(PLAYER_STATE::ISRIGHT);
	bool pUp = pStateMachine->GetState(PLAYER_STATE::ISUP);
	bool pDown = pStateMachine->GetState(PLAYER_STATE::ISDOWN);

	if (m_Sm->GetCurState()->GetName() == L"IdleState")
	{
		m_Delay += DT;

		if (10.f < m_Delay)
		{
			CSound* pSound = CAssetMgr::Get()->LoadSound(L"damsel_female", L"sound\\damsel_female.wav");
			pSound->Play();
			m_Delay = 0.f;
		}
	}

	if (m_IsHold)
	{
		if (pRight)
			SetPos(vPPos.x + 20.f / 2.f, vPPos.y);
		else
			SetPos(vPPos.x - 20.f / 2.f, vPPos.y);
	}

	else if (m_IsThrown)
	{
		m_Sm->ChangeState(L"StunState");

		m_AccTime += DT;
		m_ForceTime += DT;

		if (m_ForceOn == true)
		{
			m_RigidBody->AddForce(Vec2(1800.f * m_Dir.x, -2000.f * m_Dir.y));
		}

		if (0.6f < m_ForceTime)
		{
			m_ForceOn = false;
			m_RigidBody->AddForce(Vec2(-100.f * m_Dir.x, 1000.f * m_Dir.y));
			if (m_RigidBody->IsGround())
			{
				m_RigidBody->SetForce(Vec2(0.f, 0.f));
			}
			m_ForceTime = 0.f;
		}

		if (0.6f < m_AccTime)
		{
			m_IsThrown = false;
			m_ForceOn = true;
			m_AccTime = 0.f;
		}
	}
}

void CVictim::Render()
{
	m_Animator->Render();
}

void CVictim::SetGravityUse(bool _Use)
{
	m_RigidBody->UseGravity(_Use);
}

void CVictim::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CWhip* pWhip = dynamic_cast<CWhip*>(_OtherObj);
	CMissile* pMissile = dynamic_cast<CMissile*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);
	CExit* pExit = dynamic_cast<CExit*>(_OtherObj);

	if (pPlayer)
	{
		m_CanHold = true;
	}

	if (pTile)
	{
		if (_OwnCollider->GetName() == L"VictimBody"
			&& m_Sm->GetCurState()->GetName() != L"HoldState")
		{
			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
				m_Animator->Play(L"npc_move_l", true);
			}
			else
			{
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
				m_Animator->Play(L"npc_move_r", true);
			}
		}

		if (_OwnCollider->GetName() == L"VictimFeet" 
			&& m_Sm->GetCurState()->GetName() != L"HoldState")
		{
			Vec2 vPos = GetPos();
			vPos.y = _OtherCollider->GetFinalPos().y - 70.f;
			SetPos(vPos);
			m_RigidBody->SetGround(true);
		}

		if (m_Sm->GetCurState()->GetName() == L"StunState")
		{
			Vec2 vPos = GetPos();
			vPos.y = _OtherCollider->GetFinalPos().y - 70.f;
			SetPos(vPos);
			m_RigidBody->SetGround(true);
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
			m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
			m_Sound->Play();
			m_Sm->ChangeState(L"StunState");
		}
		else
		{
			m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
			m_Sound->Play();
			m_Sm->ChangeState(L"DeadState");
		}
	}

	if (pMissile && pCurState != L"DeadState")
	{
		m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
		m_Sound->Play();
		m_Sm->ChangeState(L"DeadState");
	}

	if (pExit)
	{
		CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
		pPlayer->WithNPC(true);
		pPlayer->NulltoNPC();
		CStateMachine* pSm = pPlayer->GetComponent<CStateMachine>();
		pSm->SetState(PLAYER_STATE::ISHOLDING, false);

		Vec2 vExitPos = pExit->GetPos();
		vExitPos.x += 40.f;
		vExitPos.y += 20.f;

		SetPos(vExitPos);

		m_IsHold = false;
		m_Sound = CAssetMgr::Get()->LoadSound(L"intodoor", L"sound\\intodoor.wav");
		m_Sound->Play();
		m_Sound = CAssetMgr::Get()->LoadSound(L"deposit", L"sound\\deposit.wav");
		m_Sound->Play();
		m_Sm->ChangeState(L"ExitState");
	}
}

void CVictim::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	
	if (pPlayer)
	{
		m_CanHold = false;
	}


}


