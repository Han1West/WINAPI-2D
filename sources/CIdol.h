#pragma once
#include "CItem.h"

class CTexture;
class CCollider;
class CRigidBody;

class CIdol :
    public CItem
{
private:
    CTexture* m_Texture;
    CCollider* m_Collider;
    CRigidBody* m_RigidBody;

public:
    void SetTexture(CTexture* _tex) { m_Texture = _tex; }

public:
    virtual void Tick();
    virtual void Render();

public:
    CLONE(CIdol);
    CIdol();
    ~CIdol();
};

