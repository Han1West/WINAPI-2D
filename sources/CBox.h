#pragma once
#include "CItem.h"

class CBox :
    public CItem
{
private:
    float       m_AccTime;
    int         m_Att;

    CCollider* m_Collider;
    CRigidBody* m_RigidBody;
    CTexture* m_Texture;

    Vec2        m_Dir;

    bool        m_IsThrown;
    bool        m_CanHold;
    bool        m_IsHold;
    bool        m_IsShot;
    bool        m_IsRight;

public:
    void SetTexture(CTexture* _tex) { m_Texture = _tex; }

    void SetDir(Vec2 _Dir) { m_Dir = _Dir; }
    void SetThrown(bool _IsThrown) { m_IsThrown = _IsThrown; }

    void SetHold(bool _Hold) { m_IsHold = _Hold; }
    bool IsHold() { return m_IsHold; }

    void SetAtt(int _Att) { m_Att = _Att; }
    int GetAtt() { return m_Att; }



public:
    virtual void Tick();
    virtual void Render();

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;

public:
    CLONE(CBox);
    CBox(const CBox& _Other);
    CBox();
    ~CBox();
};

