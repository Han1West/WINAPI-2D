#pragma once
#include "CObj.h"

class CAnimator;

class CEffect :
    public CObj
{
    CAnimator* m_Animator;


public:
    virtual void Tick() override;
    virtual void Render() override;


public:
    CLONE(CEffect);
    CEffect();
    ~CEffect();
};

