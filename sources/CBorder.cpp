#include "pch.h"
#include "CBorder.h"

#include "CEngine.h"
#include "CTexture.h"

#include "CPlayer.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CETCObj.h"
#include "CShotgun.h"
#include "CBigSnake.h"
#include "CCaveman.h"
#include "CSkeleton.h"
#include "CSnake.h"
#include "CSpider.h"
#include "CDispenser.h"
#include "CSpike.h"

#include "CPassiveItem.h"

CBorder::CBorder()
	:  m_Texture(nullptr)
	, m_Collider(nullptr)
	, m_Init(true)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Border");
	m_Collider->SetOffset(Vec2(0.f, 0.f));
}

CBorder::CBorder(const CBorder& _Other)
	: CObj(_Other)
	, m_Texture(nullptr)
	, m_Collider(nullptr)
	, m_Init(true)
{
	m_Collider = GetComponent<CCollider>();
}


CBorder::~CBorder()
{
}


void CBorder::Tick()
{
	if (m_Init)
	{
		UINT width = m_Texture->GetWidth();
		UINT height = m_Texture->GetHeight();
		m_Collider->SetScale(Vec2(width, height));
		m_Init = false;
	}
}

void CBorder::Render()
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


