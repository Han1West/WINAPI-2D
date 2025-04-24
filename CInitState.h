#pragma once
#include "CState.h"
class CInitState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CInitState);
    CInitState();
    ~CInitState();
};

