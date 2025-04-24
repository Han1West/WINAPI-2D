#pragma once
#include "CObj.h"

class CRigidBody;

class CItem :
    public CObj

{
private:
    bool        m_ForceOn;
    float       m_AccTime;

public:
    virtual void Tick() = 0;
    virtual void Throw(CRigidBody* _RigidBody, Vec2 _Dir);
    virtual void Bounce(CRigidBody* _RigidBody, Vec2 _Dir, Vec2 _Force);
    

    void SetForceOn(bool _On) { m_ForceOn = _On; }
    void ResetTime() { m_AccTime = 0.f; }

public:
    virtual CItem* Clone() = 0;
    CItem();
    ~CItem();
};

