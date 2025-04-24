#pragma once
#include "CItem.h"

class CRigidBody;
class CAnimator;
class CCollider;
class CForce;

class CBomb :
    public CItem
{
private:
    float       m_AddTime;
    float       m_AccTime;
    float       m_Duration;

    bool        m_IsBounce;
    bool        m_IsThrow;
    bool        m_IsCheck;
    bool        m_Init;

    Vec2        m_PrevPos;
    Vec2        m_PrevForce;

    CCollider*  m_Collider;

    CRigidBody* m_RigidBody;
    CAnimator*  m_Animator;
    CForce*     m_Force;

    Vec2        m_Dir;
    Vec2        m_BouceForce;

public:
    void SetLifeTime(float _time) { m_Duration = _time; }

    void SetDir(Vec2 _Dir) { m_Dir = _Dir; }

public:
    virtual void Tick();
    virtual void Render();

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
public:
    CLONE(CBomb);
    CBomb(const CBomb& _Other);
    CBomb();
    ~CBomb();
};

