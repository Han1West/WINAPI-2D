#include "pch.h"
#include "CExit.h"

#include "CEngine.h"
#include "CAssetMgr.h"
#include "CTexture.h"
#include "CCollider.h"

CExit::CExit()
	: m_Collider(nullptr)
	, m_Atlas(nullptr)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Name");
	m_Collider->SetOffset(Vec2(40.f, 30.f));
	m_Collider->SetScale(Vec2(80.f, 70.f));

}

CExit::~CExit()
{

}


void CExit::Tick()
{
}

void CExit::Render()
{
	HDC dc = CEngine::Get()->GetBackDC();
	m_Resolution = CEngine::Get()->GetResolution();
	if (nullptr == m_Atlas)
	{
		Rectangle(dc, -1, -1, (int)m_Resolution.x + 1, (int)m_Resolution.y + 1);
		return;
	}


	Vec2 vPos = GetRenderPos();
	UINT width = m_Atlas->GetWidth();
	UINT height = m_Atlas->GetHeight();

	// TransparentBlt
	//{
	//	TransparentBlt(BackDC, (int)vPos.x - width / 2, (int)vPos.y - height / 2
	//		, width, height, m_Atlas->GetDC(), 0, 0, width, height, RGB(255, 0, 255));
	//}

	// AlphaBlending
	{
		BLENDFUNCTION blend = {};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(BackDC, vPos.x, vPos.y
			, width, height, m_Atlas->GetDC(), 0, 0, width, height, blend);
	}
}


