#include "pch.h"
#include "CPJumping.h"

#include "CPlayer.h"
#include "CLevelMgr.h"
#include "CAnimator.h"
#include "CRigidBody.h"

CPJumping::CPJumping()
{
}

CPJumping::~CPJumping()
{
}

void CPJumping::Enter()
{
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	CRigidBody* pRigidBody = pPlayer->GetComponent<CRigidBody>();

	pRigidBody->SetGround(false);
}

void CPJumping::FinalTick()
{
	//LOG(LOG_LEVEL::LOG, L"JUMP STATE");
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	CAnimator* pAnimator = pPlayer->GetComponent<CAnimator>();
	CRigidBody* pRigidBody = pPlayer->GetComponent<CRigidBody>();


	//pRigidBody->Jump();

	//if (GetStateMachine()->IsDirRight())
	//	pAnimator->Play(L"player_jump", true);
	//else
	//	pAnimator->Play(L"player_jump_l", true);

}

void CPJumping::Exit()
{
	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	CRigidBody* pRigidBody = pPlayer->GetComponent<CRigidBody>();

	pRigidBody->SetGround(true);
}
