#pragma once
#include "CObj.h"

class CAnimator;
class CCollider;

class CForce :
    public CObj
{
private:
    float   m_Force;        // 힘의 크기
    float   m_Range;        // 힘이 적용되는 범위
    float   m_Duration;     // 힘의 라이프
    float   m_AccTime;      // 라이프 체크용
    float   m_Att;

    CAnimator* m_Animator;
    CCollider* m_Collider;

public:
    void SetForce(float _ForceScale, float _Range)
    {
        m_Force = _ForceScale;
        m_Range = _Range;
    }

    void SetLifeTime(float _time) { m_Duration = _time; }


public:
    virtual void Tick() override;
    virtual void Render() override;


    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
public:
    CLONE(CForce);
    CForce();
    CForce(const CForce& _Other);
    ~CForce();
};

