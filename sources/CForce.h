#pragma once
#include "CObj.h"

class CAnimator;
class CCollider;

class CForce :
    public CObj
{
private:
    float   m_Force;        // ���� ũ��
    float   m_Range;        // ���� ����Ǵ� ����
    float   m_Duration;     // ���� ������
    float   m_AccTime;      // ������ üũ��
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

