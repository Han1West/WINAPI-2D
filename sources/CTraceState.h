#pragma once

#include "CState.h"

class CTraceState
	: public CState
{
private: 
    float m_AccTime;

    bool m_IsDirRight;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CTraceState);
    CTraceState();
    ~CTraceState();
};

