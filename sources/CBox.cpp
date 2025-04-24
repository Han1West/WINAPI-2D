#include "pch.h"
#include "CBox.h"

#include "CEngine.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CCollider.h"
#include "CRigidBody.h"
#include "CTexture.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CTimeMgr.h"
#include "CObj.h"
#include "CPlayer.h"
#include "CWhip.h"
#include "CShotgun.h"
#include "CPassiveItem.h"
#include "CMissile.h"

#include "CLogMgr.h"
CBox::CBox()
	: m_Collider(nullptr)
	, m_Texture(nullptr)
	, m_RigidBody(nullptr)
	, m_Dir(Vec2(1.f, 1.f))
	, m_IsThrown(false)
	, m_Att(1)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetScale(Vec2(50.f, 50.f));

	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(1000.f);
	m_RigidBody->SetFriction(2400.f);
	m_RigidBody->SetJumpSpeed(300.f);
	m_RigidBody->SetMaxGravitySpeed(1500.f);
}


CBox::CBox(const CBox& _Other)
{
}


CBox::~CBox()
{
}

void CBox::Tick()
{
}

void CBox::Render()
{
	Vec2 vPos = GetRenderPos();
	UINT width = m_Texture->GetWidth();
	UINT height = m_Texture->GetHeight();

	// AlphaBlending
	{
		BLENDFUNCTION blend = {};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(BackDC, (int)vPos.x - width / 2, (int)vPos.y - height / 2
			, width, height, m_Texture->GetDC(), 0, 0, width, height, blend);
	}
}

void CBox::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CWhip* pWhip = dynamic_cast<CWhip*>(_OtherObj);
	CMissile* pMissile = dynamic_cast<CMissile*>(_OtherObj);


	if (pWhip || pMissile)
	{
		if (GetName() == L"BombBox")
		{
			CPassiveItem* pBombPackage = new CPassiveItem;
			pBombPackage->SetName(L"BombPackage");
			pBombPackage->SetPos(GetPos());
			pBombPackage->SetTexture(CAssetMgr::Get()->LoadTexture(L"BombPackage", L"texture\\bomb_package.png"));
			CreateObject(pBombPackage, LAYER_TYPE::PASSSIVEITEM);
		}
		else if (GetName() == L"RopeBox")
		{
			CPassiveItem* pRopePackage = new CPassiveItem;
			pRopePackage->SetName(L"RopePackage");
			pRopePackage->SetPos(GetPos());
			pRopePackage->SetTexture(CAssetMgr::Get()->LoadTexture(L"RopePackage", L"texture\\rope_package.png"));
			CreateObject(pRopePackage, LAYER_TYPE::PASSSIVEITEM);
		}
		else if (GetName() == L"ShotgunBox")
		{
			CShotgun* pShotgun = new CShotgun;
			pShotgun->SetName(L"Shotgun");
			pShotgun->SetPos(GetPos());
			CreateObject(pShotgun, LAYER_TYPE::ACTIVEITEM);
		}
		CSound* pSound = CAssetMgr::Get()->LoadSound(L"crateopen", L"sound\\crateopen.wav");
		pSound->Play();
		DeleteObject(this);
	}

}


