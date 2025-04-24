#pragma once
#include "CObj.h"


class CUI :
    public CObj
{
private:
    CUI*            m_Parent;
    vector<CUI*>    m_vecChild;

    Vec2            m_FinalPos;

    bool            m_MouseOn;      // 현재 마우스가 UI 위에 있는지
    bool            m_MouseOn_Prev; // 이전 프레임에 마우스가 UI 위에 있었는지
    bool            m_LbtnDown;     // UI 가 마우스 왼쪽버튼이 눌린 상태인지

public:
    CUI* GetParentUI() { return m_Parent; }
    const vector<CUI*>& GetChildUI() { return m_vecChild; }
    void AddChildUI(CUI* _ChildUI) 
    {
        m_vecChild.push_back(_ChildUI);
        _ChildUI->m_Parent = this;
    }

    Vec2 GetFinalPos() { return m_FinalPos; }

    bool IsLBtnDown() { return m_LbtnDown; }
    bool IsMouseOn() { return m_MouseOn; }

public:
    virtual void Tick() override final;
    virtual void Render() override final;
    virtual void FinalTick() override final;

    virtual void Tick_UI() = 0;
    virtual void Render_UI();

    virtual void BeginHovered() {} // 마우스가 UI 위에 막 올라와왔을 때
    virtual void OnHovered() {}   // 마우스 UI 위에 있을 때
    virtual void EndHovered() {}  // 마우스가 UI 를 빠져 나가는 순간

    virtual void LBtnDown() {}
    virtual void LBtnUp() {}
    virtual void LBtnClicked() {}

private:
    virtual void MouseOnCheck();

public:
    virtual CUI* Clone() override = 0;
    CUI();
    CUI(const CUI& _Other);
    ~CUI();

    friend class CUIMgr;
};

