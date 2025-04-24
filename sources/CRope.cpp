#include "pch.h"
#include "CRope.h"

#include "CEngine.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CAssetMgr.h"
#include "CTexture.h"
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CRopeOn.h"
#include "CPlayer.h"

#include "CTimeMgr.h"
#include "CTile.h"

CRope::CRope()
	: m_Animator(nullptr)
	, m_Collider(nullptr)
	, m_Texture(nullptr)
	, m_AccTime(0.f)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"RopeCollider");
	m_Collider->SetOffset(Vec2(0.f, 0.f));
	m_Collider->SetScale(Vec2(20.f,20.f));
	
	m_Texture = CAssetMgr::Get()->LoadTexture(L"Rope", L"texture\\rope.png");
}


CRope::CRope(const CRope& _Other)
	: CItem(_Other)
	, m_Animator(nullptr)
	, m_Collider(nullptr)
	, m_AccTime(0.f)
	, m_IsLodge(false)
{
	m_Animator = GetComponent<CAnimator>();
	m_Collider = GetComponent<CCollider>();
}

CRope::~CRope()
{
}

void CRope::Tick()
{
	m_AccTime += DT;
	Vec2 vPos = GetPos();

	vPos.y -= 800.f * DT;

	CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
	CPlayer* vPlayer = pCurLevel->GetPlayer();

	if (0.5f < m_AccTime)
	{
		m_IsLodge = true;
	}

	if (m_IsLodge)
	{
		CRopeOn* pRope = new CRopeOn;
		pRope->SetName(L"RopeOn");
		Vec2 vPos = GetPos();
		vPos.y += 200.f;
		pRope->SetPos(vPos);

		CSound* pSound = CAssetMgr::Get()->LoadSound(L"ropecatch", L"sound\\ropecatch.wav");
		pSound->Play();

		CreateObject(pRope, LAYER_TYPE::LADDER);
		vPlayer->InsertRopeID(pRope->GetID(), pRope);
		DeleteObject(this);
	}

	else
	{
		SetPos(vPos);
	}
	
}

void CRope::Render()
{
	/*
	Vec2 vPos = GetRenderPos();
	Vec2 vScale = GetScale();

	Rectangle(BackDC, vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f)
		, vPos.x + (vScale.x / 2.f), vPos.y + (vScale.y / 2.f));*/
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

void CRope::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CTile* pTile = dynamic_cast<CTile*>(_OtherObj);

	if (pTile)
	{
		Vec2 vPos = GetPos();
		vPos.y = _OtherCollider->GetFinalPos().y;
		
		SetPos(vPos);
		m_IsLodge = true;
	}
}

