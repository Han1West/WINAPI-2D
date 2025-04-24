#pragma once
#include "CObj.h"

class CCollider;
class CTexture;

class CDispenser :
    public CObj
{
private:
    CCollider* m_Collider;
    CCollider* m_DetectCollider;
    CTexture* m_Texture;

    bool m_IsRight;
    bool m_IsShot;

public:
    void SetTexture(CTexture* _tex) { m_Texture = _tex; }
    void ShotArrow();
    void SetIsRight(bool _Right) { m_IsRight = _Right; }

public:
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(CDispenser);
    CDispenser();
    ~CDispenser();
};

