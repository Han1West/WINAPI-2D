#include "pch.h"
#include "CPassiveItem.h"

#include "CCollider.h"
#include "CRigidBody.h"

#include "CEngine.h"
#include "CTexture.h"
#include "CPlayer.h"

#include "CTextureUI.h"
#include "CTextUI.h"
#include "CAssetMgr.h"

CPassiveItem::CPassiveItem()
	: m_Texture(nullptr)
	, m_Collider(nullptr)
	, m_RigidBody(nullptr)
	, m_IsDisplay(false)
	, m_Price(0)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"PassiveItem");
	m_Collider->SetOffset(Vec2(0.f, 0.f));
	m_Collider->SetScale(Vec2(50.f, 50.f));

	// ¹°¸®
	m_RigidBody = AddComponent(new CRigidBody);
	m_RigidBody->SetMode(RIGIDBODY_MODE::PLATFOMER);
	m_RigidBody->SetMass(1.f);
	m_RigidBody->SetMaxSpeed(0.f);
	m_RigidBody->SetFriction(0.f);
	m_RigidBody->SetMaxGravitySpeed(1000.f);
	m_RigidBody->SetJumpSpeed(0.f);
}

CPassiveItem::~CPassiveItem()
{
}

void CPassiveItem::Tick()
{
}

void CPassiveItem::Render()
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

void CPassiveItem::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);

	if (pPlayer)
	{
		if (m_IsDisplay)
		{
			if (_OtherCollider->GetName() == L"Body Collider")
			{
				Vec2 vCurPos = GetPos();
				Vec2 vPos = pPlayer->GetRenderPos();
				Vec2 vResol = CEngine::Get()->GetResolution();
		/*		vPos.x -= vResol.x;
				vPos.y -= vResol.y;*/


				m_PriceTexture = new CTextureUI;
				m_PriceTexture->SetName(L"PriceTextureUI");
				m_PriceTexture->SetPos(Vec2(vPos.x, vPos.y-50.f));
				m_PriceTexture->SetTexture(CAssetMgr::Get()->LoadTexture(L"PriceTextureUI", L"texture\\shopUI.png"));
				CreateObject(m_PriceTexture, LAYER_TYPE::UI);


				m_PriceText = new CTextUI;
				m_PriceText->SetName(L"PriceTextUI");
				m_PriceText->SetPos(Vec2(vPos.x, vPos.y-50.f));
				m_PriceText->SetScale(30);
				m_PriceText->SetString(std::to_wstring(m_Price));
				CreateObject(m_PriceText, LAYER_TYPE::UI);
			}
		}
	}
}

void CPassiveItem::EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);

	if (pPlayer && m_IsDisplay)
	{
		if (_OtherCollider->GetName() == L"Body Collider")
		{
			DeleteObject(m_PriceTexture);
			DeleteObject(m_PriceText);
		}
	}
}


