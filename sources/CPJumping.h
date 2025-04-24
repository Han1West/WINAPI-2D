#pragma once
#include "CState.h"
class CPJumping :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CPJumping);
    CPJumping();
    ~CPJumping();
};

