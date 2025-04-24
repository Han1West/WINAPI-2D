#pragma once
#include "CState.h"


class CAttackState :
    public CState
{
private:
    float m_AccTime;
    float m_AttDelay;

    Vec2 m_PlayerPos;
    Vec2 m_StartPos;

    bool m_IsDirRight;
    bool m_CanAttack;
public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CAttackState);
    CAttackState();
    ~CAttackState();
};

