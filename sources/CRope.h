#pragma once
#include "CItem.h"

class CAnimator;
class CCollider;
class CTexture;

class CRope :
    public CItem

{
private:
    CCollider* m_Collider;
    CAnimator* m_Animator;
    CTexture*  m_Texture;
    
    float      m_AccTime;
    bool       m_IsLodge;

public:
    void SetTexture(CTexture* _Tex) { m_Texture = _Tex; }
 
public:
    virtual void Tick();
    virtual void Render();

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(CRope);
    CRope(const CRope& _Other);
    CRope();
    ~CRope();

};

