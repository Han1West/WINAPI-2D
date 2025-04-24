#include "pch.h"
#include "CGold.h"

#include "CCollider.h"
#include "CRigidBody.h"

#include "CEngine.h"
#include "CTexture.h"
#include "CPlayer.h"

CGold::CGold()
	: m_Texture(nullptr)
	, m_Collider(nullptr)
	, m_RigidBody(nullptr)
{
}

CGold::~CGold()
{
	m_Collider = AddComponent(new CCollider);
	m_Collider->SetName(L"SpikeShoe");
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

void CGold::Tick()
{
}

void CGold::Render()
{
}

void CGold::BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider)
{
}


