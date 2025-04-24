#pragma once
#include "CItem.h"

class CCollider;
class CRigidBody;
class CAnimator;


class CShotgun :
    public CItem
{
private:
    float       m_AccTime;

    CCollider* m_Collider;
    CRigidBody* m_RigidBody;
    CAnimator* m_Animator;

    Vec2        m_Dir;
    int         m_Price;

    bool        m_IsDisplay;
    bool        m_IsThrown;
    bool        m_CanHold;
    bool        m_IsHold;
    bool        m_IsNPC;
    bool        m_NPCDirRight;

public:

    void SetDir(Vec2 _Dir) { m_Dir = _Dir; }
    void SetThrown(bool _IsThrown) { m_IsThrown = _IsThrown; }

    void SetHold(bool _Hold) { m_IsHold = _Hold; }
    void SetIsNPC(bool _NPC) { m_IsNPC = _NPC; }
    void SetNPCDirRight(bool _Right) { m_NPCDirRight = _Right; }
    bool IsHold() { return m_IsHold; }

    void SetGravityUse(bool _Use);

    void SetDisplay(bool _Display) { m_IsDisplay = _Display; }
    bool IsDisplay() { return m_IsDisplay; }

    void SetPrice(int _Price) { m_Price = _Price; }
    int GetPrice() { return m_Price; }

public:
    virtual void Tick();
    virtual void Render();

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
public:
    CLONE(CShotgun);
    CShotgun();
    ~CShotgun();

};

