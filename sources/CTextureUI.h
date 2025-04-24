#pragma once
#include "CUI.h"

class CTexture;

class CTextureUI :
    public CUI
{
private:
    CTexture* m_Texture;
    Vec2    m_GrabPos;

public:
    void SetTexture(CTexture* _Tex) { m_Texture = _Tex; }

public:
    virtual void Tick_UI() override;
    virtual void Render_UI() override;


public:
    CLONE(CTextureUI);
    CTextureUI(const CTextureUI& _Other);
    CTextureUI();
    ~CTextureUI();
};

