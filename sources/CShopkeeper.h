#pragma once
#include "CMonster.h"

class CCollider;
class CAnimator;
class CRigidBody;
class CStateMachine;
class CShotgun;
class CPassiveItem;
class CSound;

class CShopkeeper :
    public CMonster
{
private:
    CCollider* m_Head;
    CCollider* m_Body;
    CCollider* m_Feet;
    CCollider* m_Check;
    CCollider* m_Shop;
    CAnimator* m_Animator;
    CRigidBody* m_RigidBody;
    CStateMachine* m_Sm;
    CShotgun* m_Shotgun;
    CSound* m_Sound;
    CSound* m_PrevBGM;

    float     m_AttDelay;
    float     m_AccTime;
    bool      m_IsCall;
    bool      m_IsDead;
    bool      m_DisplayChange;
    bool      m_InitDir;
    bool      m_Init;
    bool      m_RageOn;
    bool      m_EncounterPlayer;

    vector<CObj*> m_vecShopObj;
public:
    vector<CObj*> GetShopItem() { return m_vecShopObj; }
    void SetPrice(CPassiveItem* _Obj);
    void SetInitDir(bool _Right) { m_InitDir = _Right; }

    void SetDisplayChange(bool _change) { m_DisplayChange = _change; }
    bool IsDisplayChange() { return m_DisplayChange; }

public:
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider) override;


public:
    CLONE(CShopkeeper);
    CShopkeeper();
    ~CShopkeeper();
};

