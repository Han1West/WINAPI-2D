#pragma once
#include "CMonster.h"

class CCollider;
class CAnimator;
class CRigidBody;
class CStateMachine;
class CSound;

class CVictim :
    public CMonster
{
    float m_AccTime;
    float m_Delay;
    float m_ForceTime;
    float m_StunTime;

    CCollider* m_Head;
    CCollider* m_Body;
    CCollider* m_Feet;
    CAnimator* m_Animator;
    CRigidBody* m_RigidBody;
    CStateMachine* m_Sm;
    CSound* m_Sound;

    Vec2    m_Dir;

    bool    m_ForceOn;
    bool    m_IsThrown;
    bool    m_CanHold;
    bool    m_IsHold;
    bool    m_InitDir;
    bool    m_Init;

public:
    void SetDir(Vec2 _Dir) { m_Dir = _Dir; }
    void SetThrown(bool _IsThrown) { m_IsThrown = _IsThrown; }
    void SetInitDir(bool _Right) { m_InitDir = _Right; }

    void SetHold(bool _Hold) { m_IsHold = _Hold; }
    void SetForce(bool _Force) { m_ForceOn = _Force; }
    bool IsHold() { return m_IsHold; }

    void SetGravityUse(bool _Use);
    

public:
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;


public:
    CLONE(CVictim);
    CVictim();
    ~CVictim();
};

