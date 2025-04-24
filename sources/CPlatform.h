#pragma once
#include "CObj.h"

class CCollider;

class CPlatform :
    public CObj
{
private:
    CCollider*  m_Collider;
    

public:
    virtual void SetScale(Vec2 _Scale) override;
    virtual void SetScale(float _Width, float _Height) override;

public:
    virtual void Tick() override;
    virtual void Render() override {}
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;

public:
    CLONE(CPlatform);
    CPlatform();
    CPlatform(const CPlatform& _Other);
    ~CPlatform();
};

