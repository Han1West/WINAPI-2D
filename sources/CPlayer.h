#pragma once
#include "CObj.h"

class CTexture;
class CCollider;
class CAnimator;
class CRigidBody;
class CStateMachine;
class CRopeOn;
class CRope;
class CETCObj;
class CShotgun;
class CPassiveItem;
class CVictim;
class CSound;
class CTextureUI;
class CTextUI;

class CPlayer :
    public CObj
{
private:
    CTexture*   m_Texture;
    CSound*     m_Sound;
    float       m_HP;
    float       m_AccTime;
    float       m_AttDelay;
    float       m_HitDelay;
    float       m_JumpTime;
    float       m_CheckCollTime;
    int         m_FeetDamage;

    int         m_CurrentGold;
    int         m_CurrentBomb;
    int         m_CurrentRope;

    bool        m_HasGlove;
    bool        m_WithNPC;
    bool        m_IsPushed;
    bool        m_CheckOn;
    bool        m_HasSpring;
    bool        m_IsUIMake;

    wstring     m_CurrentItem;
    int         m_ItemOrder;
    map<int, CCollider*>  m_mapTileID;

    Vec2               m_EdgePos;

    CCollider*         m_Body;
    CCollider*         m_Feet;
    CCollider*         m_Head;
    CCollider*         m_Check;
    CCollider*         m_TileCheckCol;
    CAnimator*         m_Animator;
    CRigidBody*        m_RigidBody;
    CStateMachine*     m_Sm;
    
    map<int, CRopeOn*>           m_mapRopeID;
    map<int, CETCObj*>         m_mapObjID;

    CRope*             m_Rope;
    CRopeOn*           m_RopeOn;
    CETCObj*           m_Obj;
    CVictim*           m_Victim;
    CShotgun*          m_Shotgun;

    CTextureUI*        m_PriceTexture;
    CTextUI*           m_PriceText;


public:
    int GetFeetDamage() { return m_FeetDamage; }
    int GetCurrentHP() { return m_HP; }

    int GetCurrentGold() { return m_CurrentGold; }
    int GetCurrentBomb() { return m_CurrentBomb; }
    int GetCurrentRope() { return m_CurrentRope; }

    void SetGold(int _Gold) { m_CurrentGold = _Gold; }
    void SetBomb(int _Bomb) { m_CurrentBomb = _Bomb; }
    void SetRope(int _Rope) { m_CurrentRope = _Rope; }
    void SetFeetDamage(int _Dmg) { m_FeetDamage = _Dmg; }
    
    void SetCurItem(wstring _Item) { m_CurrentItem = _Item; }
    void SetItemOrder(int _Order) { m_ItemOrder = _Order; }

    void SetPushed(bool _push) { m_IsPushed = _push; }

    void SetHP(int _HP) { m_HP = _HP; }
    void InsertRopeID(int _ID, CRopeOn* _Rope)
    {
        m_mapRopeID.insert(make_pair(_ID, _Rope));
    }

    void WithNPC(bool _yes) { m_WithNPC = _yes; }
    void NulltoNPC() { m_Victim = nullptr; };

    void BuyItem();
    wstring GetCurItem() { return m_CurrentItem; }
    int GetItemOrder() { return m_ItemOrder; }

    bool HasGlove() { return m_HasGlove; }
    bool HasSpring() { return m_HasSpring; }
    
    void SetGlove(bool _Glove) { m_HasGlove = _Glove; }

public:
    virtual void Tick() override;    
    virtual void Render() override;


public:
    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void Overlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(CPlayer);
    CPlayer();
    CPlayer(const CPlayer& _Other);
    ~CPlayer();
};

