#include "pch.h"
#include "CPlayer.h"

#include "CDebugMgr.h"

#include "CTimeMgr.h"
#include "CLevelMgr.h"
#include "CLevel.h"

#include "CShotgun.h"
#include "CETCObj.h"
#include "CBomb.h"
#include "CRope.h"
#include "CRopeOn.h"
#include "CForce.h"
#include "CWhip.h"
#include "CMissile.h"
#include "CGuidedMissile.h"

#include "CKeyMgr.h"
#include "CTaskMgr.h"
#include "CEngine.h"
#include "CPathMgr.h"

#include "CAssetMgr.h"
#include "CTexture.h"
#include "CPlatform.h"
#include "CSound.h"

#include "CCollider.h"
#include "CAnimator.h"
#include "CAnim.h"
#include "CRigidBody.h"
#include "CCollisionMgr.h"

#include "CTile.h"
#include "CTileMap.h"

#include "CStateMachine.h"
#include "CPlayerIdleState.h"
#include "CPlayerHitState.h"
#include "CPlayerStunState.h"
#include "CPlayerJumpState.h"
#include "CCamera.h"

#include "CLogMgr.h"

#include "CBigSnake.h"
#include "CCaveman.h"
#include "CSkeleton.h"
#include "CSnake.h"
#include "CSpider.h"
#include "CDispenser.h"
#include "CSpike.h"
#include "CExit.h"
#include "CVictim.h"
#include "COlmec.h"
#include "CIdol.h"
#include "CSplit.h"

#include "CTextUI.h"
#include "CTextureUI.h"

#include "CPassiveItem.h"
#include "CBorder.h"


CPlayer::CPlayer()
	: m_HP(4)
	, m_Texture(nullptr)
	, m_Body(nullptr)
	, m_Feet(nullptr)
	, m_Animator(nullptr)
	, m_RigidBody(nullptr)
	, m_Rope(nullptr)
	, m_Shotgun(nullptr)
	, m_AttDelay(0.f)
	, m_AccTime(0.f)
	, m_HitDelay(0.f)
	, m_FeetDamage(1)
	, m_CurrentGold(2500)
	, m_CurrentBomb(50)
	, m_CurrentRope(4)
	, m_CurrentItem{}
	, m_HasGlove(false)
	, m_WithNPC(false)
	, m_CheckCollTime(0.f)
	, m_CheckOn(false)
	, m_TileCheckCol(nullptr)
{
	// 컴포넌트 설정
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"Body Collider");
	m_Body->SetOffset(Vec2(0.f, 15.f));
	m_Body->SetScale(Vec2(40.f, 40.f));

	m_Feet = AddComponent(new CCollider);
	m_Feet->SetName(L"Feet Collider");
	m_Feet->SetOffset(Vec2(0.f, 45.f));
	m_Feet->SetScale(Vec2(15.f, 10.f));
	
	m_Head = AddComponent(new CCollider);
	m_Head->SetName(L"Head Collider");
	m_Head->SetOffset(Vec2(0.f, -10.f));
	m_Head->SetScale(Vec2(10.f, 5.f));

	m_Check = AddComponent(new CCollider);
	m_Check->SetName(L"Check Collider");
	m_Check->SetOffset(Vec2(0.f, 15.f));
	m_Check->SetScale(Vec2(40.f, 40.f));
	m_Check->Deactivate();

	m_TileCheckCol = AddComponent(new CCollider);
	m_TileCheckCol->SetName(L"TileCheck Collider");
	m_TileCheckCol->SetOffset(Vec2(0.f, 15.f));
	m_TileCheckCol->SetScale(Vec2(40.f, 40.f));
	


	// RigidBody
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(300.f);
	m_RigidBody->SetFriction(1000.f);
	m_RigidBody->SetMaxGravitySpeed(900.f);
	m_RigidBody->SetJumpSpeed(580.f);

	// Player 에 StateMachine 컴포넌트 추가
	m_Sm = AddComponent(new CStateMachine);
	m_Sm->AddState(L"PlayerIdleState", new CPlayerIdleState);
	m_Sm->AddState(L"PlayerHitState", new CPlayerHitState);
	m_Sm->AddState(L"PlayerStunState", new CPlayerStunState);
	m_Sm->AddState(L"PlayerJumpState", new CPlayerJumpState);

	m_Sm->SetState(PLAYER_STATE::CANRIGHT, true);
	m_Sm->SetState(PLAYER_STATE::CANLEFT, true);

	// Player 에 Animator 컴포넌트 추가
	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\PLAYER_ATTACK_L.anim");
	m_Animator->LoadAnimation(L"animation\\PLAYER_ATTACK_R.anim");

	m_Animator->LoadAnimation(L"animation\\PLAYER_CLIMB_UP.anim");

	m_Animator->LoadAnimation(L"animation\\PLAYER_DEAD.anim");

	m_Animator->LoadAnimation(L"animation\\PLAYER_DOWNMOVE_L.anim");
	m_Animator->LoadAnimation(L"animation\\PLAYER_DOWNMOVE_R.anim");

	m_Animator->LoadAnimation(L"animation\\PLAYER_DOWNUP_L.anim");
	m_Animator->LoadAnimation(L"animation\\PLAYER_DOWNUP_R.anim");

	m_Animator->LoadAnimation(L"animation\\PLAYER_HANG.anim");

	m_Animator->LoadAnimation(L"animation\\PLAYER_HEAD_UP.anim");
	m_Animator->LoadAnimation(L"animation\\PLAYER_HEAD_UPL.anim");

	m_Animator->LoadAnimation(L"animation\\PLAYER_HIT_L.anim");
	m_Animator->LoadAnimation(L"animation\\PLAYER_HIT_RIGHT.anim");

	m_Animator->LoadAnimation(L"animation\\PLAYER_IDLE_L.anim");
	m_Animator->LoadAnimation(L"animation\\PLAYER_IDLE_R.anim");

	m_Animator->LoadAnimation(L"animation\\PLAYER_JUMP.anim");
	m_Animator->LoadAnimation(L"animation\\PLAYER_JUMP_L.anim");
	
	m_Animator->LoadAnimation(L"animation\\PLAYER_KNEEL_DOWNL.anim");
	m_Animator->LoadAnimation(L"animation\\PLAYER_KNEEL_DOWNR.anim");

	m_Animator->LoadAnimation(L"animation\\PLAYER_WALK_L.anim");
	m_Animator->LoadAnimation(L"animation\\PLAYER_WALK_RIGHT.anim");

	m_Animator->LoadAnimation(L"animation\\PLAYER_EDGE_L.anim");
	m_Animator->LoadAnimation(L"animation\\PLAYER_EDGE_R.anim");

	// Animation 플레이
	m_Animator->Play(L"player_idle_l", true);
}

CPlayer::CPlayer(const CPlayer& _Other)
	: CObj(_Other)
	, m_Body(nullptr)
	, m_Feet(nullptr)
	, m_Animator(nullptr)
	, m_RigidBody(nullptr)
	, m_AccTime(0.f)
	, m_AttDelay(0.f)
	, m_Rope(nullptr)
{
	m_Body = (CCollider*)GetComponentByName(L"Body Collider");
	m_Feet = (CCollider*)GetComponentByName(L"Feet Collider");
	m_Animator = GetComponent<CAnimator>();
	m_RigidBody = GetComponent<CRigidBody>();
}

CPlayer::~CPlayer()
{
	
}


void CPlayer::Tick()
{
	float fDT = CTimeMgr::Get()->GetDeltaTime();
	Vec2 vPos = GetPos();

	if (m_CheckOn)
	{
		m_CheckCollTime += DT;
		if (0.5f < m_CheckCollTime)
		{
			m_CheckCollTime = 0.f;
			m_Check->Deactivate();
			m_CheckOn = false;
		}
	}

	if (KEY_TAP(KEY::R))
	{
		int Levelnumber = CLevelMgr::Get()->GetCurLevelIndex();
		RestartLevel(Levelnumber);
	}

	if (m_Sm->GetState(PLAYER_STATE::ISDEAD))
	{
		m_Animator->Play(L"player_dead", false);
		if (m_HP != 0)
		{
			m_HP = 0;
			CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
			pLevel->SetChange(true);
		}
		CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
		CSound* pCurBGM = pLevel->GetBGM();
		pCurBGM->Stop();
	}
	else
	{

		if (m_HP <= 0)
		{
			m_Sm->SetState(PLAYER_STATE::ISDEAD, true);
		}
		if (m_Sm->GetState(PLAYER_STATE::ISHANG))
		{
			Vec2 RopePos = m_RopeOn->GetPos();
			vPos.x = RopePos.x;
			SetPos(vPos);
		}

		if (!m_Sm->GetState(PLAYER_STATE::ISSTUN))
		{
			if (m_Sm->GetState(PLAYER_STATE::ISEdge))
			{
				if (KEY_PRESSED(KEY::DOWN))
				{
					if (KEY_TAP(KEY::Z))
					{
						m_Sm->SetState(PLAYER_STATE::ISEdge, false);
						m_Sm->SetState(PLAYER_STATE::ISJUMPING, true);
						m_RigidBody->UseGravity(true);
					}
				}
				if (KEY_TAP(KEY::Z))
				{
					m_Sm->SetState(PLAYER_STATE::ISEdge, false);
					m_Sm->SetState(PLAYER_STATE::ISJUMPING, true);
					m_Sm->SetState(PLAYER_STATE::CANRIGHT, true);
					m_Sm->SetState(PLAYER_STATE::CANLEFT, true);
					m_RigidBody->UseGravity(true);
					m_RigidBody->Jump();
					m_Sound = CAssetMgr::Get()->LoadSound(L"jump", L"sound\\jump.wav");
					m_Sound->Play();
				}
			}

			if (KEY_TAP(KEY::SPACEBAR))
			{
				if (m_Sm->GetState(PLAYER_STATE::CANEXIT))
				{
					if (m_WithNPC)
					{
						m_HP += 1;
					}
					CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
					pLevel->SavePlayerInfo();
					int Levelnumber = CLevelMgr::Get()->GetCurLevelIndex();
					CLevel* pNextLevel = CLevelMgr::Get()->GetNextLevel();
					ChangeLevel(Levelnumber + 1);
					m_Sound = CAssetMgr::Get()->LoadSound(L"intodoor", L"sound\\intodoor.wav");
					m_Sound->Play();
				}
					
				if (m_Sm->GetState(PLAYER_STATE::CANBUY))
				{
					BuyItem();
				}

				if (m_Sm->GetState(PLAYER_STATE::GETEND))
				{
					CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
					pLevel->SavePlayerInfo();
					int Levelnumber = CLevelMgr::Get()->GetCurLevelIndex();
					CLevel* pNextLevel = CLevelMgr::Get()->GetNextLevel();

					ChangeLevel(Levelnumber + 1);
				}
			}



			Vec2 szVel = m_RigidBody->GetVelocity();
			wchar_t szVelocity[255] = {};
			//swprintf_s(szVelocity, 255, L"Velocity : %f", m_HP);
			//LOG(LOG_LEVEL::LOG, szVelocity);
			if (!m_Sm->GetState(PLAYER_STATE::ISEdge))
			{
				if (KEY_TAP(KEY::Z))
				{
					if (m_Sm->GetState(PLAYER_STATE::ISHANG))
					{
						m_RigidBody->UseGravity(true);
						m_Sm->SetState(PLAYER_STATE::ISHANG, false);
						m_Sm->SetState(PLAYER_STATE::ISCLIMB, false);
						m_RigidBody->Jump();
						m_Sound = CAssetMgr::Get()->LoadSound(L"jump", L"sound\\jump.wav");
						m_Sound->Play();
					}
					if (!m_Sm->GetState(PLAYER_STATE::ISJUMPING))
					{
						m_Sm->SetState(PLAYER_STATE::ISJUMPING, true);
						m_RigidBody->UseGravity(true);
						m_RigidBody->Jump();
						m_Sound = CAssetMgr::Get()->LoadSound(L"jump", L"sound\\jump.wav");
						m_Sound->Play();
					}
				}

				if (KEY_TAP(KEY::X))
				{
					if (!m_Sm->GetState(PLAYER_STATE::ISATTACK) && !m_Sm->GetState(PLAYER_STATE::ISDOWN)
						&& !m_Sm->GetState(PLAYER_STATE::ISHOLDING) && !m_Sm->GetState(PLAYER_STATE::GETSHOTGUN))
					{
						m_AttDelay = 0.f;
						m_Sm->SetState(PLAYER_STATE::ISATTACK, true);
						CWhip* pWhip = new CWhip;
						pWhip->SetScale(Vec2(40.f, 20.f));
						if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
						{
							pWhip->SetRight(true);
							pWhip->PlayAnimation();
						}
						else
						{
							pWhip->SetRight(false);
							pWhip->PlayAnimation();
						}
						m_Sound = CAssetMgr::Get()->LoadSound(L"whip", L"sound\\whip.wav");
						m_Sound->Play();
						CreateObject(pWhip, LAYER_TYPE::PLAYER_PROJECTILE);
					}
					else if (m_Obj && m_Sm->GetState(PLAYER_STATE::ISHOLDING) && !m_Sm->GetState(PLAYER_STATE::ISDOWN))
					{
						if (m_Sm->GetState(PLAYER_STATE::ISRIGHT) && m_Sm->GetState(PLAYER_STATE::ISUP))
							m_Obj->SetDir(Vec2(0.5f, 1.5f));
						else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT) && m_Sm->GetState(PLAYER_STATE::ISUP))
							m_Obj->SetDir(Vec2(-0.5f, 1.5f));
						else if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
							m_Obj->SetDir(Vec2(1.f, 1.f));
						else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT))
							m_Obj->SetDir(Vec2(-1.f, 1.f));
						m_Sm->SetState(PLAYER_STATE::ISHOLDING, false);
						m_Obj->SetHold(false);
						if (m_HasGlove)
						{
							if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
								m_Obj->SetIsRight(true);
							else
								m_Obj->SetIsRight(false);
							m_Obj->SetShot(true);
						}
						else
						{
							m_Obj->SetThrown(true);
						}
						m_Sound = CAssetMgr::Get()->LoadSound(L"throwitem", L"sound\\throw_item.wav");
						m_Sound->Play();
						m_Obj->SetGravityUse(true);
					}

					else if (m_Victim && m_Sm->GetState(PLAYER_STATE::ISHOLDING) && !m_Sm->GetState(PLAYER_STATE::ISDOWN))
					{
						if (m_Sm->GetState(PLAYER_STATE::ISRIGHT) && m_Sm->GetState(PLAYER_STATE::ISUP))
							m_Victim->SetDir(Vec2(0.5f, 1.5f));
						else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT) && m_Sm->GetState(PLAYER_STATE::ISUP))
							m_Victim->SetDir(Vec2(-0.5f, 1.5f));
						else if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
							m_Victim->SetDir(Vec2(1.f, 1.f));
						else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT))
							m_Victim->SetDir(Vec2(-1.f, 1.f));
						m_Sm->SetState(PLAYER_STATE::ISHOLDING, false);
						m_Victim->SetHold(false);
						m_Victim->SetThrown(true);
						m_Victim->SetForce(true);
						m_Victim->SetGravityUse(true);
						m_Victim = nullptr;
					}

					else if (m_Sm->GetState(PLAYER_STATE::GETSHOTGUN))
					{
						CMissile* pMissile = new CMissile;
						pMissile->SetName(L"Missile");
						pMissile->SetScale(10.f, 10.f);

						CMissile* pCloneMissile = pMissile->Clone();
						pMissile->SetName(L"CloneMissile");
						pMissile->SetScale(10.f, 10.f);

						CMissile* pCloneMissile2 = pMissile->Clone();
						pMissile->SetName(L"CloneMissile");
						pMissile->SetScale(10.f, 10.f);

						CMissile* pCloneMissile3 = pMissile->Clone();
						pMissile->SetName(L"CloneMissile");
						pMissile->SetScale(10.f, 10.f);

						if (m_Sm->GetState(PLAYER_STATE::ISRIGHT) && !m_Sm->GetState(PLAYER_STATE::ISDOWN))
						{
							pMissile->SetPos(GetPos().x + 50.f, GetPos().y + 20.f);
							pMissile->SetVelocity(Vec2(1.f, 0.f));

							pCloneMissile->SetPos(GetPos().x + 80.f, GetPos().y + 20.f);
							pCloneMissile->SetVelocity(Vec2(1.f, 0.f));
							pCloneMissile2->SetPos(GetPos().x + 55.f, GetPos().y + 10.f);
							pCloneMissile2->SetVelocity(Vec2(1.f, 0.f));
							pCloneMissile3->SetPos(GetPos().x + 55.f, GetPos().y + 35.f);
							pCloneMissile3->SetVelocity(Vec2(1.f, 0.f));
							CreateObject(pMissile, LAYER_TYPE::PLAYER_PROJECTILE);
							CreateObject(pCloneMissile, LAYER_TYPE::PLAYER_PROJECTILE);
							CreateObject(pCloneMissile2, LAYER_TYPE::PLAYER_PROJECTILE);
							CreateObject(pCloneMissile3, LAYER_TYPE::PLAYER_PROJECTILE);
							m_RigidBody->AddForce(Vec2(-1000000000000000.f, -1000000000000000.f));
							m_Sound = CAssetMgr::Get()->LoadSound(L"shotgun", L"sound\\shotgun.wav");
							m_Sound->Play();

						}
						else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT) && !m_Sm->GetState(PLAYER_STATE::ISDOWN))
						{
							pMissile->SetPos(GetPos().x - 50.f, GetPos().y + 20.f);
							pMissile->SetVelocity(Vec2(-1.f, 0.f));

							pCloneMissile->SetPos(GetPos().x - 80.f, GetPos().y + 20.f);
							pCloneMissile->SetVelocity(Vec2(-1.f, 0.f));
							pCloneMissile2->SetPos(GetPos().x - 55.f, GetPos().y + 10.f);
							pCloneMissile2->SetVelocity(Vec2(-1.f, 0.f));
							pCloneMissile3->SetPos(GetPos().x - 55.f, GetPos().y + 35.f);
							pCloneMissile3->SetVelocity(Vec2(-1.f, 0.f));
							CreateObject(pMissile, LAYER_TYPE::PLAYER_PROJECTILE);
							CreateObject(pCloneMissile, LAYER_TYPE::PLAYER_PROJECTILE);
							CreateObject(pCloneMissile2, LAYER_TYPE::PLAYER_PROJECTILE);
							CreateObject(pCloneMissile3, LAYER_TYPE::PLAYER_PROJECTILE);
							m_RigidBody->AddForce(Vec2(1000000000000000.f, -1000000000000000.f));
							m_Sound = CAssetMgr::Get()->LoadSound(L"shotgun", L"sound\\shotgun.wav");
							m_Sound->Play();
						}
					}
				}

				if (m_Sm->GetState(PLAYER_STATE::ISATTACK))
					m_AttDelay += DT;

				if (KEY_TAP(KEY::A))
				{
					if (0 != m_CurrentRope)
					{
						m_CurrentRope -= 1;
						m_Rope = new CRope;
						m_Rope->SetName(L"Rope");
						m_Rope->SetPos(GetPos().x, GetPos().y);
						m_Rope->SetScale(20.f, 800.f);
						CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
						pLevel->SetChange(true);
						CreateObject(m_Rope, LAYER_TYPE::PLAYER_PROJECTILE);
						m_Sound = CAssetMgr::Get()->LoadSound(L"ropetoss", L"sound\\ropetoss.wav");
						m_Sound->Play();
						//m_mapRopeID.insert(make_pair(m_Rope->GetID(), m_Rope));
					}
				}

				if (KEY_TAP(KEY::S))
				{
					if (0 != m_CurrentBomb)
					{
						m_CurrentBomb -= 1;
						CBomb* pBomb = new CBomb;
						pBomb->SetName(L"Bomb");
						pBomb->SetPos(GetPos().x, (GetPos().y + GetScale().y / 2.f) - (GetScale().y / 2.f));
						pBomb->SetScale(20.f, 20.f);
						pBomb->SetLifeTime(3.f);
						if (m_Sm->GetState(PLAYER_STATE::ISRIGHT) && m_Sm->GetState(PLAYER_STATE::ISUP))
							pBomb->SetDir(Vec2(0.5f, 1.5f));
						else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT) && m_Sm->GetState(PLAYER_STATE::ISUP))
							pBomb->SetDir(Vec2(-0.5f, 1.5f));
						else if (m_Sm->GetState(PLAYER_STATE::ISRIGHT) && m_Sm->GetState(PLAYER_STATE::ISDOWN))
							pBomb->SetDir(Vec2(0.1f, 0.f));
						else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT) && m_Sm->GetState(PLAYER_STATE::ISDOWN))
							pBomb->SetDir(Vec2(-0.1f, 0.f));
						else if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
							pBomb->SetDir(Vec2(1.f, 1.f));
						else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT))
							pBomb->SetDir(Vec2(-1.f, 1.f));
						m_Sound = CAssetMgr::Get()->LoadSound(L"throwitem", L"sound\\throw_item.wav");
						m_Sound->Play();
						CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
						pLevel->SetChange(true);
						CreateObject(pBomb, LAYER_TYPE::PLAYER_PROJECTILE);

					}
				}


				if (KEY_TAP(KEY::LEFT))
				{
					m_Sm->SetState(PLAYER_STATE::ISRIGHT, false);
					m_Sm->SetState(PLAYER_STATE::ISLEFT, true);
					m_Sm->SetState(PLAYER_STATE::ISMOVING, true);
					m_Sm->SetState(PLAYER_STATE::SEARCHING, false);
					m_Sm->SetState(PLAYER_STATE::CANRIGHT, true);
				}
				if (KEY_PRESSED(KEY::LEFT))
				{
					if (m_Sm->GetState(PLAYER_STATE::CANLEFT))
					{
						if (m_Sm->GetState(PLAYER_STATE::ISLEFT))
						{
							if (m_Sm->GetState(PLAYER_STATE::ISRUNNING) && !m_Sm->GetState(PLAYER_STATE::ISCLIMB))
								vPos.x -= 550.f * fDT;
							else if (m_Sm->GetState(PLAYER_STATE::ISCRAWL))
								vPos.x -= 200.f * fDT;
							else
								vPos.x -= 400.f * fDT;
						}
					}
				}
				if (KEY_RELEASED(KEY::LEFT))
				{
					m_Sm->SetState(PLAYER_STATE::ISLEFT, false);
					if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT))
						m_Sm->SetState(PLAYER_STATE::ISMOVING, false);
				}


				if (KEY_TAP(KEY::RIGHT))
				{
					m_Sm->SetState(PLAYER_STATE::ISRIGHT, true);
					m_Sm->SetState(PLAYER_STATE::ISMOVING, true);
					m_Sm->SetState(PLAYER_STATE::SEARCHING, false);
					m_Sm->SetState(PLAYER_STATE::CANLEFT, true);
				}
				if (KEY_PRESSED(KEY::RIGHT))
				{
					if (m_Sm->GetState(PLAYER_STATE::CANRIGHT))
					{
						if (m_Sm->GetState(PLAYER_STATE::ISRIGHT) && !m_Sm->GetState(PLAYER_STATE::ISCLIMB))
						{
							if (m_Sm->GetState(PLAYER_STATE::ISRUNNING))
								vPos.x += 550.f * fDT;
							else if (m_Sm->GetState(PLAYER_STATE::ISCRAWL))
								vPos.x += 200.f * fDT;
							else
								vPos.x += 400.f * fDT;
						}
					}
				}
				if (KEY_RELEASED(KEY::RIGHT))
				{
					if (m_Sm->GetState(PLAYER_STATE::ISLEFT))
						m_Sm->SetState(PLAYER_STATE::ISRIGHT, false);
					if (!m_Sm->GetState(PLAYER_STATE::ISLEFT))
						m_Sm->SetState(PLAYER_STATE::ISMOVING, false);
				}

				if (KEY_TAP(KEY::UP))
				{
					if (m_Sm->GetState(PLAYER_STATE::CANHANG))
					{
						m_RigidBody->SetGround(false);
						m_RigidBody->UseGravity(false);
						m_Sm->SetState(PLAYER_STATE::ISHANG, true);
						m_Sm->SetState(PLAYER_STATE::ISCLIMB, true);
						m_Sm->SetState(PLAYER_STATE::SEARCHING, false);
						m_Sm->SetState(PLAYER_STATE::ISJUMPING, true);
					}
					else
					{
						m_RigidBody->UseGravity(true);
						m_Sm->SetState(PLAYER_STATE::ISHANG, false);
						m_Sm->SetState(PLAYER_STATE::ISCLIMB, false);
						m_Sm->SetState(PLAYER_STATE::SEARCHING, false);
					}

					m_Sm->SetState(PLAYER_STATE::ISUP, true);
					if (m_Sm->GetState(PLAYER_STATE::ISMOVING) || m_Sm->GetState(PLAYER_STATE::ISJUMPING))
					{
						m_Sm->SetState(PLAYER_STATE::SEARCHING, false);
					}
					else
					{
						m_Sm->SetState(PLAYER_STATE::SEARCHING, true);
					}
				}
				if (KEY_PRESSED(KEY::UP))
				{
					// 로프를 타는중이면 위치를 이동시켜줌 아니면 카메라 작동
					if (m_Sm->GetState(PLAYER_STATE::ISCLIMB))
					{
						if (m_Sm->GetState(PLAYER_STATE::CANHANG))
							vPos.y -= 200.f * DT;
							
					}
					else if (m_Sm->GetState(PLAYER_STATE::SEARCHING))
					{
						m_AccTime += DT;
						if (1.f < m_AccTime)
						{
							
							CCamera::Get()->ChangeLookAt(Vec2(vPos.x, vPos.y - 300.f), true);
						}
						else if (2.f < m_AccTime)
						{
							m_AccTime = 0.f;
						}
						
					}
				}
				if (KEY_RELEASED(KEY::UP))
				{
					m_Sm->SetState(PLAYER_STATE::ISUP, false);
					m_Sm->SetState(PLAYER_STATE::ISCLIMB, false);
					m_Sm->SetState(PLAYER_STATE::SEARCHING, false);
					CCamera::Get()->ChangeLookAt(Vec2(vPos.x, vPos.y), false);
					m_AccTime = 0.f;
				}


				if (KEY_TAP(KEY::DOWN))
				{
					m_Sm->SetState(PLAYER_STATE::ISDOWN, true);
					if (m_Sm->GetState(PLAYER_STATE::ISMOVING) || m_Sm->GetState(PLAYER_STATE::ISJUMPING)
						|| m_Sm->GetState(PLAYER_STATE::CANHANG))
					{
						m_Sm->SetState(PLAYER_STATE::SEARCHING, false);
					}
					else
					{
						m_Sm->SetState(PLAYER_STATE::SEARCHING, true);
					}

					if (!m_Sm->GetState(PLAYER_STATE::ISJUMPING))
						m_Sm->SetState(PLAYER_STATE::ISCRAWL, true);

					if (m_Sm->GetState(PLAYER_STATE::ISHANG))
					{
						m_Sm->SetState(PLAYER_STATE::ISCLIMB, true);
					}
				}
				if (KEY_PRESSED(KEY::DOWN))
				{
					if (m_Sm->GetState(PLAYER_STATE::ISCLIMB))
					{
						vPos.y += 200.f * DT;
					}

					if (m_Sm->GetState(PLAYER_STATE::SEARCHING))
					{
						m_AccTime += DT;
						if (1.f < m_AccTime)
							CCamera::Get()->ChangeLookAt(Vec2(vPos.x, vPos.y + 300.f), false);
					}

					if (KEY_TAP(KEY::X))
					{
						if (m_Obj && !m_Sm->GetState(PLAYER_STATE::ISHOLDING))
						{
							m_Sm->SetState(PLAYER_STATE::ISHOLDING, true);
							m_Obj->SetHold(true);
							m_Obj->SetGravityUse(false);
							m_Sound = CAssetMgr::Get()->LoadSound(L"pickup", L"sound\\pickup.wav");
							m_Sound->Play();
						}
						else if (m_Obj && m_Sm->GetState(PLAYER_STATE::ISHOLDING))
						{
							if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
								m_Obj->SetDir(Vec2(0.1f, 0.f));
							else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT))
								m_Obj->SetDir(Vec2(-0.1f, 0.f));

							m_Sm->SetState(PLAYER_STATE::ISHOLDING, false);
							m_Obj->SetHold(false);
							m_Obj->SetThrown(true);
							m_Obj->SetGravityUse(true);
							m_Sound = CAssetMgr::Get()->LoadSound(L"itemdrop", L"sound\\item_drop.wav");
							m_Sound->Play();
						}

						if (m_Victim && !m_Sm->GetState(PLAYER_STATE::ISHOLDING))
						{
							m_Sm->SetState(PLAYER_STATE::ISHOLDING, true);
							CStateMachine* pSm = m_Victim->GetComponent<CStateMachine>();
							pSm->ChangeState(L"HoldState");
							m_Victim->SetHold(true);
							m_Victim->SetGravityUse(false);
						}
						else if (m_Victim && m_Sm->GetState(PLAYER_STATE::ISHOLDING))
						{
							if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
								m_Victim->SetDir(Vec2(0.1f, 0.f));
							else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT))
								m_Victim->SetDir(Vec2(-0.1f, 0.f));

							m_Sm->SetState(PLAYER_STATE::ISHOLDING, false);
							m_Victim->SetHold(false);
							m_Victim->SetThrown(true);
							m_Victim->SetGravityUse(true);
							m_Victim = nullptr;
						}


						if (m_Shotgun && !m_Sm->GetState(PLAYER_STATE::GETSHOTGUN) && !m_Sm->GetState(PLAYER_STATE::ISHOLDING))
						{
							m_Sm->SetState(PLAYER_STATE::GETSHOTGUN, true);
							m_Shotgun->SetHold(true);
							m_Shotgun->SetGravityUse(false);
							m_Sound = CAssetMgr::Get()->LoadSound(L"getshotgun", L"sound\\shotgunpump.wav");
							m_Sound->Play();
						}
						else if (m_Shotgun && m_Sm->GetState(PLAYER_STATE::GETSHOTGUN))
						{
							if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
								m_Shotgun->SetDir(Vec2(0.1f, 0.f));
							else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT))
								m_Shotgun->SetDir(Vec2(-0.1f, 0.f));

							m_Sm->SetState(PLAYER_STATE::GETSHOTGUN, false);
							m_Shotgun->SetHold(false);
							m_Shotgun->SetThrown(true);
							m_Shotgun->SetGravityUse(true);
							m_Sound = CAssetMgr::Get()->LoadSound(L"itemdrop", L"sound\\item_drop.wav");
							m_Sound->Play();
						}
					}
				}
				if (KEY_RELEASED(KEY::DOWN))
				{
					m_Sm->SetState(PLAYER_STATE::ISDOWN, false);
					m_Sm->SetState(PLAYER_STATE::ISCRAWL, false);
					m_Sm->SetState(PLAYER_STATE::ISCLIMB, false);
					m_Sm->SetState(PLAYER_STATE::SEARCHING, false);
					m_AccTime = 0.f;
					CCamera::Get()->ChangeLookAt(Vec2(vPos.x, vPos.y), true);
				}


				if (KEY_TAP(KEY::SHIFT))
				{
					m_Sm->SetState(PLAYER_STATE::ISRUNNING, true);
				}

				if (KEY_RELEASED(KEY::SHIFT))
				{
					m_Sm->SetState(PLAYER_STATE::ISRUNNING, false);
				}
			}
		}
		
		if (0.3f < m_AttDelay)
		{
			m_Sm->SetState(PLAYER_STATE::ISATTACK, false);
			m_AttDelay = 0.f;
		}

		// 상태에 따른 애니메이션 진행

		if (m_Sm->IsStateChange())
		{
			//LOG(LOG_LEVEL::LOG, L"STate Change");
			if (m_Sm->GetState(PLAYER_STATE::ISATTACK))
			{
				if (m_Sm->GetState(PLAYER_STATE::ISRIGHT()))
					m_Animator->Play(L"player_attack_r", false);
				else
					m_Animator->Play(L"player_attack_l", false);
			}
			else if (m_Sm->GetState(PLAYER_STATE::ISCLIMB))
			{
				m_Animator->Play(L"player_climb_up", true);
			}
			else if (m_Sm->GetState(PLAYER_STATE::ISHANG))
			{
				// 로프를 벗어나면 상태해제
				if (!m_Sm->GetState(PLAYER_STATE::CANHANG))
				{
					m_RigidBody->UseGravity(true);
					m_Sm->SetState(PLAYER_STATE::ISHANG, false);
					m_Sm->SetState(PLAYER_STATE::ISCLIMB, false);
				}
				m_Animator->Play(L"player_hang", false);
			}
			else if (m_Sm->GetState(PLAYER_STATE::ISJUMPING))
			{
				if (m_Sm->GetState(PLAYER_STATE::ISRIGHT()))
					m_Animator->Play(L"player_jump", false);
				else
					m_Animator->Play(L"player_jump_l", false);
			}
			else if (m_Sm->GetState(PLAYER_STATE::ISMOVING))
			{
	
				m_AccTime += DT;
				if (m_Sm->GetState(PLAYER_STATE::ISRIGHT) && m_Sm->GetState(PLAYER_STATE::ISCRAWL))
					m_Animator->Play(L"player_downmove_r", true);
				else if (!m_Sm->GetState(PLAYER_STATE::ISRIGHT) && m_Sm->GetState(PLAYER_STATE::ISCRAWL))
					m_Animator->Play(L"player_downmove_l", true);
				else if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
					m_Animator->Play(L"player_walk_r", true);
				else
					m_Animator->Play(L"player_walk_l", true);
			}
			else if (m_Sm->GetState(PLAYER_STATE::ISDOWN))
			{
				if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
					m_Animator->Play(L"player_kneeldown_r", false);
				else
					m_Animator->Play(L"player_kneeldown_l", false);
			}
			else if (m_Sm->GetState(PLAYER_STATE::ISUP))
			{
				if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
					m_Animator->Play(L"player_headup_r", false);
				else
					m_Animator->Play(L"player_headup_l", false);
			}
			else if (!m_Sm->GetState(PLAYER_STATE::ISMOVING))
			{
				if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
					m_Animator->Play(L"player_idle_r", true);
				else
					m_Animator->Play(L"player_idle_l", true);
			}
			if (m_Sm->GetState(PLAYER_STATE::ISSTUN))
			{
				m_Sm->ChangeState(L"PlayerStunState");
				m_Sm->SetState(PLAYER_STATE::ISMOVING, false);
				m_Sm->SetState(PLAYER_STATE::ISJUMPING, false);
				m_Sm->SetState(PLAYER_STATE::SEARCHING, false);
			}
		}


		if (m_Sm->GetState(PLAYER_STATE::ISHIT))
		{
			m_Sm->SetState(PLAYER_STATE::ISEdge, false);
			m_HitDelay += DT;
			m_Body->Deactivate();

			if (1.f < m_HitDelay)
			{
				m_Sm->SetState(PLAYER_STATE::ISHIT, false);
				m_Body->Activate();
				m_HitDelay = 0.f;
			}
		}

		if (m_Sm->GetState(PLAYER_STATE::ISJUMPING))
		{
			m_JumpTime += DT;
			if (0.3f < m_JumpTime)
			{
				m_Feet->Activate();
				m_JumpTime = 0.f;
			}
		}
		

			
		if (m_RigidBody->GetGravityVeloicity().y > 0.f && m_mapTileID.size() == 1)
		{
			CCollider* pCollider = m_mapTileID.begin()->second;
			Vec2 vCollPos = pCollider->GetFinalPos();
			Vec2 vCollSize = pCollider->GetScale();
			if (vCollPos.y - 50.f > vPos.y && vCollPos.y -64.f < vPos.y)
			{
				m_Sm->SetState(PLAYER_STATE::ISJUMPING, false);
				if (m_Sm->GetState(PLAYER_STATE::ISRIGHT))
				{
					if (vCollPos.x > vPos.x)
					{
						m_Animator->Play(L"player_edge_r", false);
						m_Sm->SetState(PLAYER_STATE::ISEdge, true);
						m_Sm->SetState(PLAYER_STATE::ISMOVING, false);
						m_Sm->SetState(PLAYER_STATE::ISCRAWL, false);
						m_Sm->SetState(PLAYER_STATE::SEARCHING, false);
						m_Sm->SetState(PLAYER_STATE::ISLEFT, false);
						m_Sm->SetState(PLAYER_STATE::ISUP, false);
						m_Sm->SetState(PLAYER_STATE::ISDOWN, false);
						m_Sm->SetState(PLAYER_STATE::ISRUNNING, false);
						m_Sm->SetState(PLAYER_STATE::CANRIGHT, true);
						m_Sm->SetState(PLAYER_STATE::CANLEFT, true);
						
						m_RigidBody->UseGravity(false);
						m_EdgePos.x = vCollPos.x - vCollSize.x/2.f - 25.f;
						m_EdgePos.y = vCollPos.y - 25.f;
					}
				}
				else
				{
					if (vCollPos.x < vPos.x)
					{
						m_Animator->Play(L"player_edge_l", false);
						m_Sm->SetState(PLAYER_STATE::ISEdge, true);
						m_Sm->SetState(PLAYER_STATE::ISMOVING, false);
						m_Sm->SetState(PLAYER_STATE::ISCRAWL, false);
						m_Sm->SetState(PLAYER_STATE::SEARCHING, false);
						m_Sm->SetState(PLAYER_STATE::ISRIGHT, false);
						m_Sm->SetState(PLAYER_STATE::ISLEFT, false);
						m_Sm->SetState(PLAYER_STATE::ISUP, false);
						m_Sm->SetState(PLAYER_STATE::ISDOWN, false);
						m_Sm->SetState(PLAYER_STATE::ISRUNNING, false);
						m_Sm->SetState(PLAYER_STATE::CANRIGHT, true);
						m_Sm->SetState(PLAYER_STATE::CANLEFT, true);
						m_RigidBody->UseGravity(false);
						m_EdgePos.x = vCollPos.x + vCollSize.x / 2.f + 25.f;
						m_EdgePos.y = vCollPos.y - 25.f;
					}
				}
			}

		}


		m_Sm->SetStateChange(false);
		if (m_Sm->GetState(PLAYER_STATE::ISEdge))
		{
			SetPos(m_EdgePos);
		}
		else
		{
			SetPos(vPos);
		}

		if (m_Sm->GetState(PLAYER_STATE::CANBUY) && !m_IsUIMake)
		{
		}
		
		if (false == m_Sm->GetState(PLAYER_STATE::SEARCHING))
		{
			if (m_Sm->GetState(PLAYER_STATE::ISEdge))
			{
				CCamera::Get()->SetLookAt(m_EdgePos);
			}
			else
			{
				CCamera::Get()->SetLookAt(vPos);
			}
		}
	}
}

void CPlayer::Render()
{
	m_Animator->Render();
}

void CPlayer::BuyItem()
{
	m_Check->Activate();
	m_CheckOn = true;
}

void CPlayer::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlatform* pPlatform = dynamic_cast<CPlatform*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);
	CRopeOn* pRope = dynamic_cast<CRopeOn*>(_OtherObj);
	CETCObj* pETCObj = dynamic_cast<CETCObj*>(_OtherObj);
	CShotgun* pShotgun = dynamic_cast<CShotgun*>(_OtherObj);
	CSnake* pSnake = dynamic_cast<CSnake*>(_OtherObj);
	CCaveman* pCaveman = dynamic_cast<CCaveman*>(_OtherObj);
	CSpider* pSpider = dynamic_cast<CSpider*>(_OtherObj);
	CSkeleton* pSkeleton = dynamic_cast<CSkeleton*>(_OtherObj);
	CBigSnake* pBigSnake = dynamic_cast<CBigSnake*>(_OtherObj);
	CExit* pExit = dynamic_cast<CExit*>(_OtherObj);
	CVictim* pVicTim = dynamic_cast<CVictim*>(_OtherObj);
	COlmec* pOlmec = dynamic_cast<COlmec*>(_OtherObj);
	CIdol* pIdol = dynamic_cast<CIdol*>(_OtherObj);
	CSplit* pSplit = dynamic_cast<CSplit*>(_OtherObj);
	CBorder* pBorder = dynamic_cast<CBorder*>(_OtherObj);
	CMissile* pMissile = dynamic_cast<CMissile*>(_OtherObj);

	CPassiveItem* pPassiveItem = dynamic_cast<CPassiveItem*>(_OtherObj);

	if (pExit)
	{
		m_Sm->SetState(PLAYER_STATE::CANEXIT, true);
	}

	if (pTile)
	{
		if (_OwnCollider->GetName() == L"Feet Collider")
		{
			m_Sm->SetState(PLAYER_STATE::ISJUMPING, false);
			if (m_RigidBody->GetGravityVeloicity().y > 600.f)
			{
				Vec2 vCollpos = _OtherCollider->GetFinalPos();
				Vec2 vPos = GetPos();
				vPos.y = vCollpos.y - 80.f;
				SetPos(vPos);
			}
			m_RigidBody->SetGround(true);
			m_Sm->SetState(PLAYER_STATE::ISJUMPING, false);
			m_Sound = CAssetMgr::Get()->LoadSound(L"land", L"sound\\land.wav");
			m_Sound->Play();
			if (m_IsPushed)
			{
				m_Sm->SetState(PLAYER_STATE::ISDEAD, true);
				m_Sm->ChangeState(L"PlayerHitState");
				m_Sound = CAssetMgr::Get()->LoadSound(L"splat", L"sound\\splat.wav");
			}
		}
		if (_OwnCollider->GetName() == L"Body Collider")
		{
			if (m_IsPushed)
			{
				m_Sm->SetState(PLAYER_STATE::ISDEAD, true);
				m_Sm->ChangeState(L"PlayerHitState");
				m_Sound = CAssetMgr::Get()->LoadSound(L"splat", L"sound\\splat.wav");
			}

			m_mapTileID.insert(make_pair(_OtherCollider->GetID(), _OtherCollider));
		}
		if (_OwnCollider->GetName() == L"Head Collider")
		{
			Vec2 vPos = GetPos();
			vPos.y = _OtherCollider->GetFinalPos().y + 50.f;
			SetPos(vPos);
			//m_RigidBody->AddVelociy(Vec2(0.f, 1000000.f));
		}
	}

	if (pBorder)
	{
		if (_OwnCollider->GetName() == L"Feet Collider")
		{
			m_Sm->SetState(PLAYER_STATE::ISJUMPING, false);
			if (m_RigidBody->GetGravityVeloicity().y > 600.f)
			{
				Vec2 vCollpos = _OtherCollider->GetFinalPos();
				Vec2 vPos = GetPos();
				vPos.y = vCollpos.y - 80.f;
				SetPos(vPos);
			}
			m_RigidBody->SetGround(true);
			m_Sm->SetState(PLAYER_STATE::ISJUMPING, false);
			m_Sound = CAssetMgr::Get()->LoadSound(L"land", L"sound\\land.wav");
			m_Sound->Play();
			if (m_IsPushed)
			{
				m_Sm->SetState(PLAYER_STATE::ISDEAD, true);
				m_Sm->ChangeState(L"PlayerHitState");
				m_Sound = CAssetMgr::Get()->LoadSound(L"splat", L"sound\\splat.wav");
			}
		}
		if (_OwnCollider->GetName() == L"Head Collider")
		{
			Vec2 vPos = GetPos();
			vPos.y = _OtherCollider->GetFinalPos().y + 50.f;
			SetPos(vPos);
			//m_RigidBody->AddVelociy(Vec2(0.f, 1000000.f));
		}
	}

	if (pRope)
	{
		m_RopeOn = pRope;
		m_Sm->SetState(PLAYER_STATE::CANHANG, true);
	}

	if (pETCObj)
	{
		m_Obj = pETCObj;
	}

	if (pVicTim)
	{
		CStateMachine* pSm = pVicTim->GetComponent<CStateMachine>();
		if (pSm->GetCurState()->GetName() != L"DeadState")
			m_Victim = pVicTim;
	}

	if (pShotgun && false == pShotgun->IsDisplay())
	{
		m_Shotgun = pShotgun;
	}

	if (pSnake)
	{
		if (_OwnCollider->GetName() == L"Feet Collider" &&
			_OtherCollider->GetName() == L"SnakeHead")
		{
			m_RigidBody->Squash();
		}

		if (_OwnCollider->GetName() == L"Body Collider" &&
			_OtherCollider->GetName() == L"SnakeBody")
		{
			m_HP -= 1;
			m_Sm->SetState(PLAYER_STATE::ISHIT, true);
			m_Sm->ChangeState(L"PlayerHitState");
			CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
			pLevel->SetChange(true);
			m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
			m_Sound->Play();
		}
	}
	if (pMissile && pMissile->GetName() != L"Split")
	{
		if (_OwnCollider->GetName() == L"Body Collider")
		{
			m_HP = 0;
			m_Sm->SetState(PLAYER_STATE::ISHIT, true);
			m_Sm->ChangeState(L"PlayerHitState");
			CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
			pLevel->SetChange(true);
			m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
			m_Sound->Play();
		}
	}

	if (pCaveman)
	{
		if (_OwnCollider->GetName() == L"Feet Collider" &&
			_OtherCollider->GetName() == L"CavemanHead")
		{
			m_RigidBody->Squash();
		}

		if (_OwnCollider->GetName() == L"Body Collider" &&
			_OtherCollider->GetName() == L"CavemanBody")
		{
			CStateMachine* pSm = pCaveman->GetComponent<CStateMachine>();
			if (pSm->GetCurState()->GetName() != L"StunState")
			{
				m_HP -= 1;
				m_Sm->SetState(PLAYER_STATE::ISHIT, true);
				m_Sm->ChangeState(L"PlayerHitState");
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
				m_Sound->Play();
			}

		}
	}

	if (pSpider)
	{
		if (_OwnCollider->GetName() == L"Feet Collider" &&
			_OtherCollider->GetName() == L"SpiderHead")
		{
			m_RigidBody->Jump();
		}

		if (_OwnCollider->GetName() == L"Body Collider" &&
			_OtherCollider->GetName() == L"SpiderBody")
		{
			m_HP -= 1;
			m_Sm->SetState(PLAYER_STATE::ISHIT, true);
			m_Sm->ChangeState(L"PlayerHitState");
			CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
			pLevel->SetChange(true);
			m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
			m_Sound->Play();
		}
	}

	if (pSkeleton)
	{
		if (_OwnCollider->GetName() == L"Feet Collider" &&
			_OtherCollider->GetName() == L"SkeletonHead")
		{
			LOG(LOG_LEVEL::LOG, L"Called");
			m_RigidBody->Jump();
		}

		if (_OwnCollider->GetName() == L"Body Collider" &&
			_OtherCollider->GetName() == L"SkeletonBody")
		{
			m_HP -= 1;
			m_Sm->SetState(PLAYER_STATE::ISHIT, true);
			m_Sm->ChangeState(L"PlayerHitState");
			CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
			pLevel->SetChange(true);
			m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
			m_Sound->Play();
		}
	}

	if (pBigSnake)
	{
		if (_OwnCollider->GetName() == L"Feet Collider" &&
			_OtherCollider->GetName() == L"BigSnakeHead")
		{
			m_RigidBody->Jump();
		}

		if (_OwnCollider->GetName() == L"Body Collider" &&
			_OtherCollider->GetName() == L"BigSnakeBody")
		{
			m_HP -= 1;
			m_Sm->SetState(PLAYER_STATE::ISHIT, true);
			m_Sm->ChangeState(L"PlayerHitState");
			CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
			pLevel->SetChange(true);
			m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
			m_Sound->Play();
		}
	}

	if (pSplit)
	{
		if (_OwnCollider->GetName() == L"Body Collider")
		{
			m_HP -= pSplit->GetAtt();
			m_Sm->SetState(PLAYER_STATE::ISSTUN, true);
			m_Sm->SetState(PLAYER_STATE::ISHIT, true);
			m_Sm->ChangeState(L"PlayerStunState");
			CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
			pLevel->SetChange(true);
			m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
			m_Sound->Play();
		}
		if (_OwnCollider->GetName() == L"Head Collider")
		{
			m_HP -= pSplit->GetAtt();
			m_Sm->SetState(PLAYER_STATE::ISSTUN, true);
			m_Sm->SetState(PLAYER_STATE::ISHIT, true);
			m_Sm->ChangeState(L"PlayerStunState");
			CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
			pLevel->SetChange(true);
			m_Sound = CAssetMgr::Get()->LoadSound(L"hit", L"sound\\hit.wav");
			m_Sound->Play();
		}
	}

	if (pIdol)
	{
		m_Sm->SetState(PLAYER_STATE::GETEND, true);
	}

	if (pPassiveItem && _OwnCollider->GetName() == L"Body Collider")
	{
		if (false == pPassiveItem->IsDisplay())
		{
			if (pPassiveItem->GetName() == L"SpikeShoe")
			{
				m_FeetDamage = 5;
				m_CurrentItem = L"SpikeShoe";
				m_ItemOrder += 1;
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"collect", L"sound\\collect.wav");
				m_Sound->Play();
				DeleteObject(pPassiveItem);
			}
			if (pPassiveItem->GetName() == L"SpringShoe")
			{
				m_HasSpring = true;
				m_RigidBody->SetJumpSpeed(700.f);
				m_CurrentItem = L"SpringShoe";
				m_ItemOrder += 1;
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"collect", L"sound\\collect.wav");
				m_Sound->Play();
				DeleteObject(pPassiveItem);
			}
			if (pPassiveItem->GetName() == L"BombPackage")
			{
				m_CurrentBomb += 3;
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"collect", L"sound\\collect.wav");
				m_Sound->Play();
				DeleteObject(pPassiveItem);
			}
			if (pPassiveItem->GetName() == L"BombSet")
			{
				m_CurrentBomb += 12;
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"collect", L"sound\\collect.wav");
				m_Sound->Play();
				DeleteObject(pPassiveItem);
			}
			if (pPassiveItem->GetName() == L"Emerald")
			{
				m_CurrentGold += 800;
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"goldpackage", L"sound\\chime4.wav");
				m_Sound->Play();
				DeleteObject(pPassiveItem);
			}
			if (pPassiveItem->GetName() == L"Gold")
			{
				m_CurrentGold += 500;
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"gold", L"sound\\chime.wav");
				m_Sound->Play();
				DeleteObject(pPassiveItem);
			}
			if (pPassiveItem->GetName() == L"GoldPackage")
			{
				m_CurrentGold += 1500;
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"goldpackage", L"sound\\chime3.wav");
				m_Sound->Play();
				DeleteObject(pPassiveItem);
			}
			if (pPassiveItem->GetName() == L"RopePackage")
			{
				m_CurrentRope += 3;
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"collect", L"sound\\collect.wav");
				m_Sound->Play();
				DeleteObject(pPassiveItem);
			}
			if (pPassiveItem->GetName() == L"Ruby")
			{
				m_CurrentGold += 1600;
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"goldpackage", L"sound\\chime4.wav");
				m_Sound->Play();
				DeleteObject(pPassiveItem);
			}
			if (pPassiveItem->GetName() == L"Sapphire")
			{
				m_CurrentGold += 1200;
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"goldpackage", L"sound\\chime4.wav");
				m_Sound->Play();
				DeleteObject(pPassiveItem);
			}
			if (pPassiveItem->GetName() == L"Glove")
			{
				m_HasGlove = true;
				m_CurrentItem = L"Glove";
				m_ItemOrder += 1;
				CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
				pLevel->SetChange(true);
				m_Sound = CAssetMgr::Get()->LoadSound(L"collect", L"sound\\collect.wav");
				m_Sound->Play();
				DeleteObject(pPassiveItem);
			}
		}
		else
		{

			m_IsUIMake = true;
			m_Sm->SetState(PLAYER_STATE::CANBUY, true);
		}
	}
	if (pPassiveItem && _OwnCollider->GetName() == L"Check Collider")
	{
		if (pPassiveItem->GetPrice() <= m_CurrentGold)
		{
			m_Sound = CAssetMgr::Get()->LoadSound(L"chaching", L"sound\\chaching.wav");
			m_Sound->Play();
			m_CurrentGold -= pPassiveItem->GetPrice();
			CLevel* pLevel = CLevelMgr::Get()->GetCurrentLevel();
			pLevel->SetChange(true);
			pPassiveItem->SetDisplay(false);
			CTextUI* pTextUI = pPassiveItem->GetTextUI();
			CTextureUI* pTextureUI = pPassiveItem->GetTextureUI();
			if (pTextUI)
			{
				DeleteObject(pTextUI);
			}
			if (pTextureUI)
			{
				DeleteObject(pTextureUI);
			}
				
		}
		else
		{
			m_Sound = CAssetMgr::Get()->LoadSound(L"uhoh", L"sound\\uhoh.wav");
			m_Sound->Play();
		}
	}

	if (pShotgun && _OwnCollider->GetName() == L"Check Collider")
	{
		if (pShotgun->GetPrice() < m_CurrentGold)
		{
			m_Sound = CAssetMgr::Get()->LoadSound(L"chaching", L"sound\\chaching.wav");
			m_Sound->Play();
			m_CurrentGold -= pShotgun->GetPrice();
			pShotgun->SetDisplay(false);
		}
		else
		{
			m_Sound = CAssetMgr::Get()->LoadSound(L"uhoh", L"sound\\uhoh.wav");
			m_Sound->Play();
		}
	}
}

void CPlayer::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);
	COlmec* pOlmec = dynamic_cast<COlmec*>(_OtherObj);
	CBorder* pBorder = dynamic_cast<CBorder*>(_OtherObj);


	if (pTile)
	{
		if (_OwnCollider->GetName() == L"TileCheck Collider")
		{
			Vec2 vPos = GetPos();
			Vec2 vCollPos = _OtherCollider->GetFinalPos();
			Vec2 vCollScale = _OtherCollider->GetScale();
			if (vPos.x < vCollPos.x)
			{
				if (m_Sm->GetState(PLAYER_STATE::ISRUNNING))
					vPos.x -= 700.f * DT;
				else if (m_Sm->GetState(PLAYER_STATE::ISCRAWL))
					vPos.x -= 200.f * DT;
				else
					vPos.x -= 400.f * DT;
				SetPos(vPos);
			}
			else if (vCollPos.x < vPos.x)
			{
				if (m_Sm->GetState(PLAYER_STATE::ISRUNNING))
					vPos.x += 700.f * DT;
				else if (m_Sm->GetState(PLAYER_STATE::ISCRAWL))
					vPos.x += 200.f * DT;
				else
					vPos.x += 400.f * DT;
				SetPos(vPos);
			}
		}
	}

	if (_OtherObj->GetName() == L"Olmec")
	{
		int a = 0;
	}

	if (pOlmec)
	{
		if (_OwnCollider->GetName() == L"Body Collider")
		{
			Vec2 vPos = GetPos();
			Vec2 vCollPos = _OtherCollider->GetFinalPos();
			Vec2 vCollScale = _OtherCollider->GetScale();
			if (vPos.x < vCollPos.x)
			{
				if (m_Sm->GetState(PLAYER_STATE::ISRUNNING))
					vPos.x -= 700.f * DT;
				else if (m_Sm->GetState(PLAYER_STATE::ISCRAWL))
					vPos.x -= 200.f * DT;
				else
					vPos.x -= 400.f * DT;
				SetPos(vPos);
			}
			else if (vCollPos.x < vPos.x)
			{
				if (m_Sm->GetState(PLAYER_STATE::ISRUNNING))
					vPos.x += 700.f * DT;
				else if (m_Sm->GetState(PLAYER_STATE::ISCRAWL))
					vPos.x += 200.f * DT;
				else
					vPos.x += 400.f * DT;
				SetPos(vPos);
			}
		}
	}

	if (pBorder)
	{
		if (_OwnCollider->GetName() == L"Body Collider")
		{
			Vec2 vPos = GetPos();
			Vec2 vCollPos = _OtherCollider->GetFinalPos();
			Vec2 vCollScale = _OtherCollider->GetScale();
			if (vPos.x < vCollPos.x)
			{
				if (m_Sm->GetState(PLAYER_STATE::ISRUNNING))
					vPos.x -= 700.f * DT;
				else if (m_Sm->GetState(PLAYER_STATE::ISCRAWL))
					vPos.x -= 200.f * DT;
				else
					vPos.x -= 400.f * DT;
				SetPos(vPos);
			}
			else if (vCollPos.x < vPos.x)
			{
				if (m_Sm->GetState(PLAYER_STATE::ISRUNNING))
					vPos.x += 700.f * DT;
				else if (m_Sm->GetState(PLAYER_STATE::ISCRAWL))
					vPos.x += 200.f * DT;
				else
					vPos.x += 400.f * DT;
				SetPos(vPos);
			}
		}
	}

}


void CPlayer::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CRopeOn* pRope = dynamic_cast<CRopeOn*>(_OtherObj);
	CETCObj* pETCObj = dynamic_cast<CETCObj*>(_OtherObj);
	CShotgun* pShotgun = dynamic_cast<CShotgun*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);
	CExit* pExit = dynamic_cast<CExit*>(_OtherObj);
	CVictim* pVicTim = dynamic_cast<CVictim*>(_OtherObj);
	CIdol * pIdol = dynamic_cast<CIdol*>(_OtherObj);
	CBorder* pBorder = dynamic_cast<CBorder*>(_OtherObj);

	CPassiveItem* pPassiveItem = dynamic_cast<CPassiveItem*>(_OtherObj);

	if (pExit)
	{
		m_Sm->SetState(PLAYER_STATE::CANEXIT, false);
	}

	if (pIdol)
	{
		if (pIdol)
		{
			m_Sm->SetState(PLAYER_STATE::GETEND, false);
		}
	}
	if (pTile)
	{
		if (_OwnCollider->GetName() == L"Body Collider")
		{
			m_mapTileID.erase(_OtherCollider->GetID());
		}

		if (_OwnCollider->GetName() == L"Feet Collider")
		{
			m_Sm->SetState(PLAYER_STATE::ISJUMPING, true);
			m_RigidBody->SetGround(false);
		}
	}
	if (pBorder)
	{
		if (_OwnCollider->GetName() == L"Feet Collider")
		{
			m_Sm->SetState(PLAYER_STATE::ISJUMPING, true);
			m_RigidBody->SetGround(false);
		}
	}

	if (pRope)
	{
		m_Sm->SetState(PLAYER_STATE::CANHANG, false);
	}

	if (pETCObj)
	{
		m_Obj = nullptr;
	}

	if(pShotgun)
	{
		m_Shotgun = nullptr;
	}

	if (pPassiveItem)
	{
		if (pPassiveItem->IsDisplay())
		{
			m_Sm->SetState(PLAYER_STATE::CANBUY, false);
			m_IsUIMake = false;
		}
	}
}