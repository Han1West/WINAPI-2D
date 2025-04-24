#pragma once
#include "CState.h"

class CPatrolState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CPatrolState);
    CPatrolState();
    ~CPatrolState();
};

