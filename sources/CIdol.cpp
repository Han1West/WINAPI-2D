#include "pch.h"
#include "CIdol.h"

#include "CCollider.h"
#include "CRigidBody.h"

#include "CEngine.h"
#include "CTexture.h"
#include "CPlayer.h"

CIdol::CIdol()
	: m_Texture(nullptr)
	, m_Collider(nullptr)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Idol");
	m_Collider->SetOffset(Vec2(0.f, 130.f));
	m_Collider->SetScale(Vec2(200.f, 200.f));
}

CIdol::~CIdol()
{
}

void CIdol::Tick()
{
}

void CIdol::Render()
{
	HDC dc = CEngine::Get()->GetBackDC();

	Vec2 vPos = GetRenderPos();
	UINT width = m_Texture->GetWidth();
	UINT height = m_Texture->GetHeight();

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


