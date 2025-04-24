#include "pch.h"
#include "CGuidedMissile.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CMonster.h"

#include "CTimeMgr.h"
#include "CPlayer.h"

CGuidedMissile::CGuidedMissile()
	: m_TargetObj(nullptr)
	, m_DetectRange(500.f)
	, m_RotateSpeed(180.f)
{
}

CGuidedMissile::CGuidedMissile(const CGuidedMissile& _Other)
	: CMissile(_Other)
	, m_TargetObj(nullptr)
	, m_DetectRange(_Other.m_DetectRange)
	, m_RotateSpeed(_Other.m_RotateSpeed)
{
}

CGuidedMissile::~CGuidedMissile()
{
}

void CGuidedMissile::Tick()
{
	/*DrawDebugShape(DEBUG_SHAPE::CIRCLE, PEN_TYPE::GREEN, BRUSH_TYPE::HOLLOW
		         , GetPos(), Vec2(m_DetectRange * 2.f, m_DetectRange * 2.f));*/



	// 1. 추적할 대상을 찾는다.
	//  - 미사일(본인)로부터 가장 가까운 대상을 찾는다.
	FindTarget();
	
	// 2. 타겟을 추적한다.
	if (IsValid(m_TargetObj))
	{
		Trace();
	}
	
	// 3. 지정된 각도(방행) 로 본인(미사일)을 이동시킨다.
	CMissile::Tick();
}

void CGuidedMissile::FindTarget()
{
	if (nullptr != m_TargetObj)
		return;

	// 탐지범위
	float MinDist = m_DetectRange;

	// 미사일의 위치
	Vec2 vMissilePos = GetPos();

	// 미사일(본인)과 가장 가까운 몬스터를 찾는다.
	CLevel* pCurLevel = CLevelMgr::Get()->GetCurrentLevel();
	const vector<CObj*>& vecMon = pCurLevel->GetLayer(LAYER_TYPE::MONSTER);
		
	for (size_t i = 0; i < vecMon.size(); ++i)
	{
		//Vec2 vDiff = vecMon[i]->GetPos() - vMissilePos;
		//float Dist = vDiff.Length();
	
		float Dist = vMissilePos.Distance(vecMon[i]->GetPos());

		if (Dist < MinDist)
		{
			MinDist = Dist;
			m_TargetObj = vecMon[i];
		}
	}
}

void CGuidedMissile::Trace()
{
	/*{
		Vec2 vDir = m_TargetObj->GetPos() - GetPos();
		vDir.Normalize();
		SetVelocity(vDir, 500.f);
	}*/

	//// 힘, 가속도
	//{
	//	Vec2 vDir = m_TargetObj->GetPos() - GetPos();
	//	vDir.Normalize();
	//	Vec2 vForce = vDir * 2000.f;
	//	Vec2 Accel = vForce / GetMass();
	//	AddVelocity(Accel * DT);
	//}

	// 방향 회전 
	{
		// 현재 이동중인 방향
		Vec2 vCurDir = GetVelocity();
		float speed = vCurDir.Length();
		vCurDir.Normalize();

		Vec2 vTargetDir = m_TargetObj->GetPos() - GetPos();

		//vCurDir.x;    vCurDir.y;      0.f;   vCurDir.x;      vCurDir.y;
		//vTargetDir.x; vTargetDir.y;   0.f;   vTargetDir.x;   vTargetDir.y;
		// 0.f, 0.f, vCurDir.x* vTargetDir.y - vCurDir.y * vTargetDir.x;
		if (0.f < vCurDir.x * vTargetDir.y - vCurDir.y * vTargetDir.x)
		{
			vCurDir = RotateDir(vCurDir, m_RotateSpeed * DT);
		}
		else
		{
			vCurDir = RotateDir(vCurDir, -m_RotateSpeed * DT);
		}

		SetVelocity(vCurDir);
	}
}