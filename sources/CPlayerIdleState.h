#pragma once
#include "CState.h"

class CPlayerIdleState :
    public CState
{
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CPlayerIdleState);
    CPlayerIdleState();
    ~CPlayerIdleState();
};

