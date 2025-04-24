#include "pch.h"
#include "CShopkeeper.h"

#include "CMonster.h"
#include "CEngine.h"
#include "CCollider.h"
#include "CAnimator.h"
#include "CRigidBody.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CSound.h"
#include "CSoundMgr.h"

#include "CStateMachine.h"
#include "CIdleState.h"
#include "CTraceState.h"
#include "CStunState.h"
#include "CDeadState.h"


#include "CTile.h"
#include "CMissile.h"
#include "CWhip.h"
#include "CPlayer.h"
#include "CShotgun.h"
#include "CPassiveItem.h"

CShopkeeper::CShopkeeper()
	: m_Body(nullptr)
	, m_Head(nullptr)
	, m_Check(nullptr)
	, m_Animator(nullptr)
	, m_RigidBody(nullptr)
	, m_Sm(nullptr)
	, m_Feet(nullptr)
	, m_AttDelay(0.f)
	, m_AccTime(0.f)
	, m_IsCall(false)
	, m_Init(true)
	, m_IsDead(false)
	, m_RageOn(false)
	, m_PrevBGM(nullptr)
	, m_Sound(nullptr)
{
	// 몬스터 정보
	tMonsterInfo pInfo = {};
	pInfo.MaxHP = 5;
	pInfo.HP = 5;
	pInfo.Speed = 500.f;
	pInfo.AttackRange = 0.f;
	pInfo.DetectRange = 1000.f;

	SetMonsterInfo(pInfo);

	// 몸 충돌체
	m_Body = AddComponent(new CCollider);
	m_Body->SetName(L"ShopkeeperBody");
	m_Body->SetOffset(Vec2(0.f, 15.f));
	m_Body->SetScale(Vec2(40.f, 30.f));

	// 머리 충돌체
	m_Head = AddComponent(new CCollider);
	m_Head->SetName(L"ShopkeeperHead");
	m_Head->SetOffset(Vec2(0.f, -10.f));
	m_Head->SetScale(Vec2(10.f, 10.f));

	// 발 충돌체
	m_Feet = AddComponent(new CCollider);
	m_Feet->SetName(L"ShopkeeperFeet");
	m_Feet->SetOffset(Vec2(0.f, 40.f));
	m_Feet->SetScale(Vec2(10.f, 10.f));
	
	// 아이템 확인 충돌체
	m_Check = AddComponent(new CCollider);
	m_Check->SetName(L"ShopItemCheck");
	m_Check->SetOffset(Vec2(0.f, 40.f));
	m_Check->SetScale(Vec2(1000.f, 100.f));
	
	// 플레이어 상점 트리거 충돌체
	m_Shop = AddComponent(new CCollider);
	m_Shop->SetName(L"ShopPlayerCheck");
	m_Shop->SetScale(Vec2(700, 450.f));


	// 물리
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(300.f);
	m_RigidBody->SetFriction(1000.f);
	m_RigidBody->SetMaxGravitySpeed(1500.f);
	m_RigidBody->SetJumpSpeed(600.f);
	
	// StateMachine
	m_Sm = AddComponent(new CStateMachine);
	m_Sm->AddState(L"IdleState", new CIdleState);
	m_Sm->AddState(L"TraceState", new CTraceState);
	m_Sm->AddState(L"StunState", new CStunState);
	m_Sm->AddState(L"DeadState", new CDeadState);

	m_Sm->ChangeState(L"IdleState");

	// 애니메이터
	m_Animator = AddComponent(new CAnimator);
	m_Animator->LoadAnimation(L"animation\\SHOPKEEPER_DIE_R.anim");
	m_Animator->LoadAnimation(L"animation\\SHOPKEEPER_DIE_L.anim");

	m_Animator->LoadAnimation(L"animation\\SHOPKEEPER_HIT_R.anim");
	m_Animator->LoadAnimation(L"animation\\SHOPKEEPER_HIT_L.anim");

	m_Animator->LoadAnimation(L"animation\\SHOPKEEPER_MOVE_R.anim");
	m_Animator->LoadAnimation(L"animation\\SHOPKEEPER_MOVE_L.anim");

	m_Animator->LoadAnimation(L"animation\\SHOPKEEPER_IDLE_R.anim");
	m_Animator->LoadAnimation(L"animation\\SHOPKEEPER_IDLE_L.anim");

	//if(m_InitDir)
	//	m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
	//	//m_Animator->Play(L"shopkeeper_idle_r", false);
	//else
	//	m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);
	//	//m_Animator->Play(L"shopkeeper_idle_l", false);
	

	m_Shotgun = new CShotgun;
	m_Shotgun->SetName(L"Shotgun");
	m_Shotgun->SetPos(GetPos().x + 40.f / 2.f,
		GetPos().y + (GetScale().y / 2.f) - 15.f);
	CreateObject(m_Shotgun, LAYER_TYPE::ACTIVEITEM);
}

CShopkeeper::~CShopkeeper()
{
}

void CShopkeeper::Tick()
{
	Vec2 vPos = GetPos();
	m_AccTime += DT;
	if (m_Init)
	{
		if (m_InitDir)
		{
			m_Animator->Play(L"shopkeeper_idle_r", false);
			m_Shop->SetOffset(Vec2(170.f, -130.f));
		}
		else
		{
			m_Animator->Play(L"shopkeeper_idle_l", false);
			m_Shop->SetOffset(Vec2(-170.f, -130.f));
		}

		m_Init = false;
	}

	if (1.f < m_AccTime && !m_IsCall)
	{
		m_Check->Deactivate();
		m_IsCall = true;
	}

	if (m_Sm->GetCurState()->GetName() == L"StunState" && !m_RageOn)
	{
		//m_PrevBGM = CSoundMgr::Get()->GetCurBGM();
		CSound* pSound = CAssetMgr::Get()->LoadSound(L"shoprage", L"sound\\Shopkeeper_rage.wav");
		pSound->SetVolume(30);
		pSound->PlayToBGM();
		m_Shop->Deactivate();
		m_RageOn = true;
	}

	if (m_Sm->GetCurState()->GetName() == L"TraceState")
	{
		m_AttDelay += DT;

		m_Shotgun->SetHold(true);
		m_Shotgun->SetIsNPC(true);

		if (1.2f < m_AttDelay)
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

			if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
			{
				pMissile->SetPos(GetPos().x + 50.f, GetPos().y + 20.f);
				pMissile->SetVelocity(Vec2(1.f, 0.f));

				pCloneMissile->SetPos(GetPos().x + 80.f, GetPos().y + 20.f);
				pCloneMissile->SetVelocity(Vec2(1.f, 0.f));
				pCloneMissile2->SetPos(GetPos().x + 55.f, GetPos().y + 10.f);
				pCloneMissile2->SetVelocity(Vec2(1.f, 0.f));
				pCloneMissile3->SetPos(GetPos().x + 55.f, GetPos().y + 35.f);
				pCloneMissile3->SetVelocity(Vec2(1.f, 0.f));
				CreateObject(pMissile, LAYER_TYPE::MONSTER_PROJECTILE);
				CreateObject(pCloneMissile, LAYER_TYPE::MONSTER_PROJECTILE);
				CreateObject(pCloneMissile2, LAYER_TYPE::MONSTER_PROJECTILE);
				CreateObject(pCloneMissile3, LAYER_TYPE::MONSTER_PROJECTILE);
			}
			else
			{
				pMissile->SetPos(GetPos().x - 50.f, GetPos().y + 20.f);
				pMissile->SetVelocity(Vec2(-1.f, 0.f));

				pCloneMissile->SetPos(GetPos().x - 80.f, GetPos().y + 20.f);
				pCloneMissile->SetVelocity(Vec2(-1.f, 0.f));
				pCloneMissile2->SetPos(GetPos().x - 55.f, GetPos().y + 10.f);
				pCloneMissile2->SetVelocity(Vec2(-1.f, 0.f));
				pCloneMissile3->SetPos(GetPos().x - 55.f, GetPos().y + 35.f);
				pCloneMissile3->SetVelocity(Vec2(-1.f, 0.f));
				CreateObject(pMissile, LAYER_TYPE::MONSTER_PROJECTILE);
				CreateObject(pCloneMissile, LAYER_TYPE::MONSTER_PROJECTILE);
				CreateObject(pCloneMissile2, LAYER_TYPE::MONSTER_PROJECTILE);
				CreateObject(pCloneMissile3, LAYER_TYPE::MONSTER_PROJECTILE);
			}
			m_Sound = CAssetMgr::Get()->LoadSound(L"shotgun", L"sound\\shotgun.wav");
			m_Sound->Play();
			m_AttDelay = 0.f;
		}


		m_Head->Activate();
		if (m_Sm->GetMonsterState((int)MONSTER_STATE::ISRIGHT))
		{
			vPos.x += GetMonsterInfo().Speed * DT;
			m_Sm->GetCurState()->Enter();
			m_Shotgun->SetNPCDirRight(true);
			m_Shotgun->SetPos(GetPos().x + 40.f / 2.f,
				GetPos().y + (GetScale().y / 2.f));
		}
		else
		{
			vPos.x -= GetMonsterInfo().Speed * DT;
			m_Sm->GetCurState()->Enter();
			m_Shotgun->SetNPCDirRight(false);
			m_Shotgun->SetPos(GetPos().x - 20.f / 2.f,
				GetPos().y + (GetScale().y / 2.f));
		}
		if (!m_Sm->GetMonsterState((int)MONSTER_STATE::ISJUMP))
		{
			m_RigidBody->Jump();
			m_Sm->SetMonsterState((int)MONSTER_STATE::ISJUMP, true);
		}
		SetPos(vPos);

	}

	if (m_Sm->GetCurState()->GetName() == L"DeadState" && !m_IsDead)
	{
		CSound* pSound = m_PrevBGM;
		pSound->SetVolume(30);
		pSound->PlayToBGM();
		m_Shotgun->SetHold(false);
		m_Shotgun->SetIsNPC(false);
		m_IsDead = true;
	}

	if(!m_Feet->IsCollision())
	{
		m_RigidBody->SetGround(false);
	}


}

void CShopkeeper::Render()
{
	m_Animator->Render();
}

void CShopkeeper::SetPrice(CPassiveItem* _Obj)
{
	wstring pStr = _Obj->GetName();

	if (pStr == L"SpikeShoe")
	{
		_Obj->SetPrice(4500);
	}
	else if (pStr == L"SpringShoe")
	{
		_Obj->SetPrice(4500);
	}
	else if (pStr == L"BombSet")
	{
		_Obj->SetPrice(10000);
	}
	else if (pStr == L"BombPackage")
	{
		_Obj->SetPrice(3500);
	}
	else if (pStr == L"RopePackage")
	{
		_Obj->SetPrice(2500);
	}
	else if (pStr == L"Glove")
	{
		_Obj->SetPrice(6500);
	}
}

void CShopkeeper::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CWhip* pWhip = dynamic_cast<CWhip*>(_OtherObj);
	CMissile* pMissile = dynamic_cast<CMissile*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);
	CPassiveItem* pPassiveItem = dynamic_cast<CPassiveItem*>(_OtherObj);
	CShotgun* pShotgun = dynamic_cast<CShotgun*>(_OtherObj);
	

	if (pTile)
	{
		if (_OwnCollider->GetName() == L"ShopkeeperBody")
		{
			Vec2 vPos = GetPos();
			Vec2 vCollPos = _OtherCollider->GetFinalPos();
			if (vPos.x < vCollPos.x)
			{
				vPos.x -= 20.f;
				SetPos(vPos);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, false);

			}
			else if(vCollPos.x < vPos.x)
			{
				vPos.x += 20.f;
				SetPos(vPos);
				m_Sm->SetMonsterState((int)MONSTER_STATE::ISRIGHT, true);
			}
		}

		if (_OwnCollider->GetName() == L"ShopkeeperFeet")
		{
			m_RigidBody->SetGround(true);
			m_Sm->SetMonsterState((int)MONSTER_STATE::ISJUMP, false);
		}
		if (_OwnCollider->GetName() == L"ShopkeeperHead")
		{
			Vec2 vPos = GetPos();
			vPos.y = _OtherCollider->GetFinalPos().y + 50.f;
			SetPos(vPos);
			//m_RigidBody->AddVelociy(Vec2(0.f, 1000000.f));
		}
	}

	wstring pCurState = m_Sm->GetCurState()->GetName();

	if (pCurState == L"IdleState")
	{
		if (pPlayer && _OwnCollider->GetName() == L"ShopPlayerCheck"
			&& !m_EncounterPlayer && _OtherCollider->GetName() == L"Body Collider")
		{
			m_PrevBGM = CSoundMgr::Get()->GetCurBGM();
			CSound* pSound = CAssetMgr::Get()->LoadSound(L"shop", L"sound\\shop.wav");
			pSound->SetVolume(30);
			pSound->PlayToBGM();
			m_Sound = CAssetMgr::Get()->LoadSound(L"shopbell", L"sound\\shop_bells.wav");
			m_Sound->Play();
			m_EncounterPlayer = true;
		}
	}

	if (pWhip && pCurState != L"DeadState" && _OwnCollider->GetName() != L"ShopPlayerCheck")
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
			m_Sound = CAssetMgr::Get()->LoadSound(L"splat", L"sound\\splat.wav");
			m_Sound->Play();
			m_Sm->ChangeState(L"DeadState");
		}
	}

	if (pMissile && pCurState != L"DeadState" && _OwnCollider->GetName() != L"ShopPlayerCheck")
	{
		m_Sound = CAssetMgr::Get()->LoadSound(L"splat", L"sound\\splat.wav");
		m_Sound->Play();
		m_Sm->ChangeState(L"DeadState");
	}

	if (m_Sm->GetCurState()->GetName() != L"IdleState")
	{
		if (_OwnCollider->GetName() == L"ShopkeeperHead")
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
							m_Animator->Play(L"shopkeeper_hit_r", false);
							m_Head->Deactivate();
						}
						else
						{
							m_Sound = CAssetMgr::Get()->LoadSound(L"squish", L"sound\\squish.wav");
							m_Sound->Play();
							m_Animator->Play(L"shopkeeper_hit_r", false);
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

	if (pPassiveItem)
	{
		if (_OwnCollider->GetName() == L"ShopItemCheck")
		{
			pPassiveItem->SetDisplay(true);
			SetPrice(pPassiveItem);
			m_vecShopObj.push_back(pPassiveItem);
		}
	}
	
	if (pShotgun)
	{
		if (_OwnCollider->GetName() == L"ShopItemCheck")
		{
			pShotgun->SetDisplay(true);
			pShotgun->SetPrice(15000);
			m_vecShopObj.push_back(pPassiveItem);
		}
	}
}

void CShopkeeper::Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);


	//if (pTile)
	//{
	//	if (_OwnCollider->GetName() == L"Body Collider")
	//	{
	//		Vec2 vPos = GetPos();
	//		Vec2 vCollPos = _OtherCollider->GetFinalPos();
	//		Vec2 vCollScale = _OtherCollider->GetScale();
	//		if (vPos.x < vCollPos.x)
	//		{
	//			vPos.x += GetMonsterInfo().Speed * DT;
	//			SetPos(vPos);
	//		}
	//		else if (vCollPos.x < vPos.x)
	//		{
	//			vPos.x -= GetMonsterInfo().Speed * DT;
	//			SetPos(vPos);
	//		}
	//	}
	//}
}
void CShopkeeper::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);

	wstring pCurState = m_Sm->GetCurState()->GetName();

	if (pCurState == L"IdleState")
	{
		if (pPlayer && _OwnCollider->GetName() == L"ShopPlayerCheck"
			&& m_EncounterPlayer && _OtherCollider->GetName() == L"Body Collider")
		{
			CSound* pSound = m_PrevBGM;
			pSound->SetVolume(30);
			pSound->PlayToBGM();
			m_EncounterPlayer = false;
		}
	}
}


