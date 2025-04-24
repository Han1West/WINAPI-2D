#pragma once

#include "CObj.h"

class CComponent :
    public CBase
{
private:
    const COMPONENT_TYPE  m_ComType;
    CObj*                 m_Owner;

public:
    COMPONENT_TYPE GetComponentType() { return m_ComType; }
    CObj* GetOwner() { return m_Owner; }

public:
    virtual void FinalTick() = 0;

public:
    virtual CComponent* Clone() = 0;
    CComponent(COMPONENT_TYPE _Type);
    CComponent(const CComponent& _Other);
    ~CComponent();

    friend class CObj;
};

