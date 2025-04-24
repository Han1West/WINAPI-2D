#pragma once
#include "CItem.h"

class CCollider;

class CRopeOn :
    public CItem
{
private:
    CCollider* m_Collider;
    CTexture* m_Texture;

    float      m_AccTime;

public:
    void SetTexture(CTexture* _Tex) { m_Texture = _Tex; }

public:
    virtual void Tick();
    virtual void Render();

public:
    CLONE(CRopeOn);
    CRopeOn(const CRopeOn& _Other);
    CRopeOn();
    ~CRopeOn();
};

