#pragma once
#include "CObj.h"

class CCollider;
class CTexture;

class CSpike :
    public CObj
{
private:
    CCollider* m_Collider;
    CTexture* m_Texture;

    bool m_IsShot;
    bool m_SoundCall[3];

public:
    void SetTexture(CTexture* _tex) { m_Texture = _tex; }
public:
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;

public:
    CLONE(CSpike);
    CSpike();
    ~CSpike();

};

