#pragma once
#include "CState.h"

class CJumpState :
    public CState
{
private:
    bool m_IsDirRight;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CJumpState);
    CJumpState();
    ~CJumpState();
};

