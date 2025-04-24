#pragma once
#include "CState.h"

class CIdleState :
    public CState
{
private:
    float m_AttDelay;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CIdleState);
    CIdleState();
    ~CIdleState();
};

