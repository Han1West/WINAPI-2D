#pragma once
#include "CItem.h"

class CTexture;
class CCollider;
class CRigidBody;
class CTextUI;
class CTextureUI;

class CPassiveItem :
    public CItem
{
private:
    CTexture* m_Texture;
    CCollider* m_Collider;
    CRigidBody* m_RigidBody;
    CTextUI* m_PriceText;
    CTextureUI* m_PriceTexture;

    bool    m_IsDisplay;
    int     m_Price;

public:
    void SetTexture(CTexture* _tex) { m_Texture = _tex; }
    void SetDisplay(bool _Display) { m_IsDisplay = _Display; }
    bool IsDisplay() { return m_IsDisplay; }

    void SetPrice(int _Price) { m_Price = _Price; }
    int GetPrice() { return m_Price; }

    CTextUI* GetTextUI() { return m_PriceText; }
    CTextureUI* GetTextureUI() { return m_PriceTexture; }

public:
    virtual void Tick();
    virtual void Render();

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);

public:
    CLONE(CPassiveItem);
    CPassiveItem();
    ~CPassiveItem();
};

