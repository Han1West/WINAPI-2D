#pragma once
#include "CMissile.h"

class CRigidBody;

class CSplit :
    public CMissile
{
private:
    CRigidBody* m_RigidBody;

    int m_Att;
    float m_AccTime;
    bool m_Right;

public:
    int GetAtt() { return m_Att; }
    void SetRight(bool _Right) { m_Right = _Right; }

public:
    virtual void Tick() override;
 
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);


public:
    CLONE(CSplit);
    CSplit();
    ~CSplit();
};

