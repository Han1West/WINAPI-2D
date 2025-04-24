#pragma once
#include "CState.h"
class CPIdle :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CPIdle);
    CPIdle();
    ~CPIdle();
};

