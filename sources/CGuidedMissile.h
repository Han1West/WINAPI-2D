#pragma once
#include "CMissile.h"


class CGuidedMissile :
    public CMissile
{
private:
    CObj*       m_TargetObj;
    float       m_DetectRange;
    float       m_RotateSpeed;


public:
    virtual void Tick() override;

private:
    void FindTarget();
    void Trace();


public:
    CLONE(CGuidedMissile);
    CGuidedMissile();
    CGuidedMissile(const CGuidedMissile& _Other);
    ~CGuidedMissile();
};

