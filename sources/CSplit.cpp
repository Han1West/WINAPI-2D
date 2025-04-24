#include "pch.h"
#include "CSplit.h"


#include "CTimeMgr.h"
#include "CEngine.h"
#include "CCollider.h"
#include "CAssetMgr.h"
#include "CTexture.h"
#include "CPlayer.h"
#include "CRigidBody.h"

#include "CTile.h"
#include "CObj.h"

CSplit::CSplit()
	: m_Att(1)
	, m_AccTime(0.f)
	, m_RigidBody(nullptr)
	, m_Right(false)
{


	// RigidBody
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(300.f);
	m_RigidBody->SetFriction(1000.f);
	m_RigidBody->SetMaxGravitySpeed(900.f);
	m_RigidBody->SetJumpSpeed(580.f);
	m_RigidBody->UseGravity(false);
}

CSplit::~CSplit()
{
}

void CSplit::Tick()
{
	CMissile::Tick();

	m_AccTime += DT;
	if (0.3f < m_AccTime)
	{
		m_RigidBody->UseGravity(true);
	}
}



void CSplit::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if(pTile)
	{
		DeleteObject(this);
	}
	if (pPlayer)
	{
		DeleteObject(this);
	}
}
