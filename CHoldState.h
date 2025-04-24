#pragma once
#include "CState.h"

class CEffect;

class CHoldState :
    public CState
{
private:
    CEffect* m_Effect;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHoldState);
    CHoldState();
    ~CHoldState();
};

