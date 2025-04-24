#pragma once
#include "CMonster.h"

class CCollider;
class CAnimator;
class CRigidBody;
class CStateMachine;
class CSound;

class CSnake :
    public CMonster
{
private:
    CCollider* m_Head;
    CCollider* m_Body;
    CCollider* m_Feet;
    CAnimator* m_Animator;
    CRigidBody* m_RigidBody;
    CStateMachine* m_Sm;
    CSound* m_Sound;

public:
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;

public:
    CLONE(CSnake);
    CSnake();
    ~CSnake();

};

