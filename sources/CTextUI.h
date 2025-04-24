#pragma once
#include "CUI.h"


class CTextUI :
    public CUI
{
private:
    wstring m_Str;

    int m_Scale;


public:
    void SetString(wstring _Text) { m_Str = _Text; }
    void SetScale(int _Scale) { m_Scale = _Scale; }

public:
    virtual void Tick_UI() override;
    virtual void Render_UI() override;


public:
    CLONE(CTextUI);
    CTextUI(const CTextUI& _Other);
    CTextUI();
    ~CTextUI();
};

