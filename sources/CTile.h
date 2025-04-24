#pragma once
#include "CObj.h"

class CTileMap;
class CCollider;

class CTile :
    public CObj
{
private:
    CTileMap*   m_TileMap;
    CCollider*  m_Collider;
    vector<CCollider*> m_vecCollider;
    
        
    bool        m_FirstCall;

public:
    void CombineCollider();
    void ChangeCombineCollider(CCollider* _Collider);
    void MakeColCollider();

public:
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _OwnCollider, CObj* _OtherObj, CCollider* _OtherCollider);


public:
    CLONE(CTile);
    CTile();
    CTile(const CTile& _Other);
    ~CTile();
};