#include "pch.h"
#include "CEffect.h"

#include "CAnimator.h"

CEffect::CEffect()
	: m_Animator(nullptr)
{
	m_Animator = AddComponent(new CAnimator);

	m_Animator->LoadAnimation(L"animation\\DIZZY_BIRDS.anim");
	m_Animator->LoadAnimation(L"animation\\BLOOD.anim");
	m_Animator->LoadAnimation(L"animation\\OLMEC_WAKEUP.anim");
	m_Animator->LoadAnimation(L"animation\\SMOKE_RIGHT.anim");
	m_Animator->LoadAnimation(L"animation\\SMOKE_LEFT.anim");
	m_Animator->LoadAnimation(L"animation\\BLAST.anim");
}

CEffect::~CEffect()
{
}


void CEffect::Tick()
{
}

void CEffect::Render()
{
	m_Animator->Render();
}
