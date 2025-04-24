#include "pch.h"
#include "CRopeOn.h"

#include "CEngine.h"
#include "CAnimator.h"
#include "CCollider.h"
#include "CAssetMgr.h"
#include "CTexture.h"


CRopeOn::CRopeOn()
	: m_Collider(nullptr)
	, m_Texture(nullptr)
	, m_AccTime(0.f)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"RopeCollider");
	m_Collider->SetOffset(Vec2(0.f, 0.f));
	m_Collider->SetScale(Vec2(20.f, 500.f));

	m_Texture = CAssetMgr::Get()->LoadTexture(L"RopeOn", L"texture\\rope_on.png");
}

CRopeOn::CRopeOn(const CRopeOn& _Other)
	: CItem(_Other)
	, m_Collider(nullptr)
	, m_AccTime(0.f)
{
	m_Collider = GetComponent<CCollider>();
}

CRopeOn::~CRopeOn()
{
}

void CRopeOn::Tick()
{
}

void CRopeOn::Render()
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
