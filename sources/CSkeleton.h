#pragma once
#include "CMonster.h"

class CCollider;
class CAnimator;
class CStateMachine;
class CRigidBody;
class CSound;

class CSkeleton :
    public CMonster
{
private:
    CCollider* m_Head;
    CCollider* m_Body;
    CCollider* m_Feet;
    CStateMachine* m_Sm;
    CAnimator* m_Animator;
    CRigidBody* m_RigidBody;
    CSound* m_Sound;

    bool m_StateChange;

public:
    virtual void Tick() override;
    virtual void Render() override;

    void SetChange(bool _Change) { m_StateChange = _Change; }
    bool IsChange() { return m_StateChange; }

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;


public:
    CLONE(CSkeleton);
    CSkeleton();
    ~CSkeleton();

};

