#pragma once
#include "CObj.h"

class CCollider;
class CTexture;

class CBorder :
    public CObj
{
private:
    CTexture* m_Texture;
    CCollider* m_Collider;

    bool m_Init;

public:
    void SetTexture(CTexture* _Tex) { m_Texture = _Tex; }


public:
    virtual void Tick() override;
    virtual void Render() override;
public:
    CLONE(CBorder);
    CBorder();
    CBorder(const CBorder& _Other);
    ~CBorder();
};

