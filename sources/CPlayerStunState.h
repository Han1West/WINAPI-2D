#pragma once
#include "CState.h"

class CEffect;

class CPlayerStunState :
    public CState
{
private:
    float m_AccTime;
    CEffect* m_Effect;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CPlayerStunState);
    CPlayerStunState();
    ~CPlayerStunState();
};

