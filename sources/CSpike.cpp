#include "pch.h"
#include "CSpike.h"


#include "CTexture.h"
#include "CEngine.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CStateMachine.h"
#include "CAssetMgr.h"
#include "CSound.h"

#include "CMonster.h"
#include "CPlayer.h"

#include "CObj.h"



CSpike::CSpike()
	: m_Texture(nullptr)
	, m_Collider(nullptr)
	, m_SoundCall{ false,false,false }
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"Spike");
	m_Collider->SetOffset(Vec2(0.f, -10.f));
	m_Collider->SetScale(Vec2(64.f, 24.f));
}

CSpike::~CSpike()
{
}

void CSpike::Tick()
{
	
}

void CSpike::Render()
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

void CSpike::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(_OtherObj);
	CMonster* pMonster = dynamic_cast<CMonster*>(_OtherObj);
	


	if (pPlayer && _OtherCollider->GetName() == L"Body Collider")
	{
		CRigidBody* pRigidBody = pPlayer->GetComponent<CRigidBody>();
		Vec2 pVelo = pRigidBody->GetGravityVeloicity();
		if (0.f < pVelo.y)
		{
			CSound* pSound = CAssetMgr::Get()->LoadSound(L"spikehit", L"sound\\spike_hit.wav");
			pSound->Play();
			CStateMachine* pSm = pPlayer->GetComponent<CStateMachine>();
			pSm->SetState(PLAYER_STATE::ISDEAD, true);
		}
	}


	if (pMonster)
	{
		CRigidBody* pRigidBody = pMonster->GetComponent<CRigidBody>();
		CStateMachine* pSm = pMonster->GetComponent<CStateMachine>();
		Vec2 pVelo = pRigidBody->GetGravityVeloicity();
		if (0.f < pVelo.y)
		{
			CSound* pSound = CAssetMgr::Get()->LoadSound(L"spikehit", L"sound\\spike_hit.wav");
			pSound->Play();
			if(pMonster->GetName() == L"Caveman" && !m_SoundCall[0])
			{
				pSm->ChangeState(L"DeadState");
				m_SoundCall[0] = true;
			}
			else if (pMonster->GetName() == L"Shopkeeper" && !m_SoundCall[1])
			{
				pSm->ChangeState(L"DeadState");
				m_SoundCall[1] = true;
			}
			else if (pMonster->GetName() == L"Victim" && !m_SoundCall[2])
			{
				pSm->ChangeState(L"DeadState");
				m_SoundCall[2] = true;
			}
			else
			{
				MakeEffect();
				DeleteObject(pMonster);
			}
		}
	}
}

