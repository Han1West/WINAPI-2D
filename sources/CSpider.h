#pragma once
#include "CMonster.h"

class CCollider;
class CAnimator;
class CRigidBody;
class CStateMachine;
class CSound;

class CSpider :
    public CMonster
{
private:
    CCollider* m_Head;
    CCollider* m_Body;
    CRigidBody* m_RigidBody;
    CStateMachine* m_Sm;
    CAnimator* m_Animator;
    CSound* m_Sound;

    float m_AccTime;

public:
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;


public:
    CLONE(CSpider);
    CSpider();
    ~CSpider();

};

