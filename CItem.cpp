#include "pch.h"
#include "CItem.h"


#include "CRigidBody.h"
#include "CCollider.h"
#include "CTimeMgr.h"

#include "CLogMgr.h"

CItem::CItem()
	: m_ForceOn(true)
	, m_AccTime(0.f)
{
}

CItem::~CItem()
{
}


void CItem::Throw(CRigidBody* _RigidBody, Vec2 _Dir)
{
	m_AccTime += DT;

	if (m_ForceOn == true)
	{
		//LOG(LOG_LEVEL::LOG, L"Called");
		//_RigidBody->SetInitVelo(Vec2(1500.f * _Dir.x, -1500.f * _Dir.y));
		_RigidBody->UseGravity(false);
		_RigidBody->AddForce(Vec2(1000.f * _Dir.x, -500.f * _Dir.y));
	}

	if (0.6f < m_AccTime)
	{
		_RigidBody->UseGravity(true);
		_RigidBody->SetForce(Vec2(1000.f * _Dir.x, 0.f));
		//m_ForceOn = false;
		//_RigidBody->AddForce(Vec2(-100.f * _Dir.x, 1000.f * _Dir.y));
		//if (_RigidBody->IsGround())
		//{
		//	_RigidBody->SetForce(Vec2(0.f, 0.f));
		//}
		//m_AccTime = 0.f;
	}
}

void CItem::Bounce(CRigidBody* _RigidBody, Vec2 _Dir, Vec2 _Force)
{
	//LOG(LOG_LEVEL::LOG, L"Called");
	//_RigidBody->SetInitVelo(Vec2(1500.f * _Dir.x, -1500.f * _Dir.y));
	

	m_AccTime += DT;

	if (m_ForceOn == true)
	{
		//LOG(LOG_LEVEL::LOG, L"Called");
		//_RigidBody->SetInitVelo(Vec2(1500.f * _Dir.x, -1500.f * _Dir.y));
		_RigidBody->AddForce(Vec2(500.f * _Dir.x, 500.f * _Dir.y));
	}

	if (0.6f < m_AccTime)
	{
		m_ForceOn = false;
		_RigidBody->AddForce(Vec2(-100.f * _Dir.x, 200.f * _Dir.y));
		if (_RigidBody->IsGround())
		{
			_RigidBody->SetForce(Vec2(0.f, 0.f));
		}
		m_AccTime = 0.f;
	}
}

