#include "pch.h"
#include "CWhip.h"

#include "CAnimator.h"
#include "CCollider.h"

#include "CEngine.h"
#include "CTimeMgr.h"

#include "CPlayer.h"
#include "CLevelMgr.h"

#include "CLevel.h"

CWhip::CWhip()
	: m_Collider(nullptr)
	, m_Animator(nullptr)
	, m_AccTime(0.f)
	, m_Duration(0.f)
	, m_Right(true)
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetOffset(Vec2(0.f, 20.f));
	m_Collider->SetScale(Vec2(70.f, 15.f));
	m_Collider->Deactivate();

	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\WHIP_R.anim");
	m_Animator->LoadAnimation(L"animation\\WHIP_L.anim");
}

CWhip::~CWhip()
{
}

void CWhip::PlayAnimation()
{
	if (m_Right)
		m_Animator->Play(L"whip_r", false);
	else
		m_Animator->Play(L"whip_l", false);
}

void CWhip::Tick()
{
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	Vec2 vPPos = pPlayer->GetPos();
	Vec2 vPScale = pPlayer->GetScale();
	if (m_Right)
	{
		SetPos(vPPos.x + vPScale.x / 2.f, vPPos.y + 10.f);
	}
	else
	{
		SetPos(vPPos.x - (vPScale.x - 1.f) / 2.f, vPPos.y + 10.f);
	}
	

	m_AccTime += DT;
	m_Duration += DT;

	if (0.015f < m_AccTime)
	{
		m_Collider->Activate();
		m_AccTime = 0.f;
	}
		

	if (0.3 < m_Duration)
	{
		DeleteObject(this);
		m_Duration = 0.f;
	}

}

void CWhip::Render()
{
	//Vec2 vPos = GetRenderPos();
	//Vec2 vScale = GetScale();

	//Rectangle(BackDC, vPos.x - (vScale.x / 2.f), vPos.y - (vScale.y / 2.f)
	//	, vPos.x + (vScale.x / 2.f), vPos.y + (vScale.y / 2.f));
	m_Animator->Render();
}

