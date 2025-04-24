#pragma once
#include "CItem.h"

class CCollider;
class CAnimator;

class CWhip :
    public CItem
{
private:
    CCollider* m_Collider;
    CAnimator* m_Animator;
    float      m_AccTime;
    float      m_Duration;
    bool       m_Right;

public:
    void SetRight(int _Right) { m_Right = _Right; }
    void PlayAnimation();


public:
    virtual void Tick();
    virtual void Render();

public:
    CLONE(CWhip);
    CWhip();
    ~CWhip();
};

