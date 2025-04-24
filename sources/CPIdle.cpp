#include "pch.h"
#include "CPIdle.h"

#include "CPlayer.h"
#include "CLevelMgr.h"
#include "CAnimator.h"
#include "CRigidBody.h"


CPIdle::CPIdle()
{
}

CPIdle::~CPIdle()
{
}

void CPIdle::Enter()
{
	//CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	//CRigidBody* pRigidBody = pPlayer->GetComponent<CRigidBody>();
	//CAnimator* pAnimator = pPlayer->GetComponent<CAnimator>();

	//pRigidBody->SetGround(true);
}

void CPIdle::FinalTick()
{
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	CRigidBody* pRigidBody = pPlayer->GetComponent<CRigidBody>();
	CAnimator* pAnimator = pPlayer->GetComponent<CAnimator>();

}

void CPIdle::Exit()
{
}


