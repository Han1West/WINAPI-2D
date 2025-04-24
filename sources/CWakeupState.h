#pragma once
#include "CState.h"


class CWakeupState :
    public CState
{
private:
    float m_AccTime;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CWakeupState);
    CWakeupState();
    ~CWakeupState();
};

