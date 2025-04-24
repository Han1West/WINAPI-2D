#pragma once
#include "CState.h"

class CEffect;

class CStunState :
    public CState
{
private:
    float m_AccTime;
    bool m_DisplayChange;
    CEffect* m_Effect;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CStunState);
    CStunState();
    ~CStunState();
};

