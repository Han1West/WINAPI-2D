#pragma once
#include "CObj.h"


class CBackground :
    public CObj
{
private:
    CTexture* m_Atlas;
    Vec2	  m_Resolution;
public:
    void SetAtlas(CTexture* _Atlas) { m_Atlas = _Atlas; }
    CTexture* GetAtlas() { return m_Atlas; }

public:
    virtual void Tick() override;
    virtual void Render() override;

public:
    CLONE(CBackground);
    CBackground();
    ~CBackground();

};

