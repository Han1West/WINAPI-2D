#include "pch.h"
#include "CComponent.h"

CComponent::CComponent(COMPONENT_TYPE _Type)
	: m_ComType(_Type)
	, m_Owner(nullptr)
{
}

CComponent::CComponent(const CComponent& _Other)
	: CBase(_Other)
	, m_ComType(_Other.m_ComType)
	, m_Owner(nullptr)
{
}

CComponent::~CComponent()
{
}
