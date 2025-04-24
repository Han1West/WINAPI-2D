#pragma once
#include "CUI.h"


typedef void(*BTN_CALLBACK)(void);
typedef void(CBase::* BaseFunc)(void);
typedef void(CBase::* BaseFunc_Param1)(DWORD_PTR);
typedef void(CBase::* BaseFunc_Param2)(DWORD_PTR, DWORD_PTR);


class CBtnUI :
    public CUI
{
private:
    BTN_CALLBACK            m_CallBack;

    CBase*                  m_Inst;
    BaseFunc                m_MemFunc;
    BaseFunc_Param1         m_MemFunc_1;
    BaseFunc_Param2         m_MemFunc_2;

    CTexture*               m_NormalImg;
    CTexture*               m_HoverImg;
    CTexture*               m_PressedImg;


public:
    void SetCallBack(BTN_CALLBACK _CallBack) { m_CallBack = _CallBack; }
    void SetDelegate(CBase* _Inst, BaseFunc _MemFunc)
    {
        m_Inst = _Inst;
        m_MemFunc = _MemFunc;
    }

    void SetNormalImg(CTexture* _Tex) { m_NormalImg = _Tex; }
    void SetHoverImg(CTexture* _Tex) { m_HoverImg = _Tex; }

    virtual void Tick_UI() override {};
    virtual void Render_UI() override;

public:
    virtual void LBtnDown() override;
    virtual void LBtnClicked() override;

public:
    CLONE(CBtnUI);
    CBtnUI();
    ~CBtnUI();
};

