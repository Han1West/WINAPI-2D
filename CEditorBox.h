#pragma once
#include "CObj.h"
class CEditorBox :
    public CObj
{
private:
    vector<Vec2> m_SelectedIndex;
    Vec2 m_Size;
    int m_Row;
    int m_Col;

    bool m_Change;
    vector<bool> m_IsGreen;

public:
    void SetRowCol(int _Row, int _Col) { m_Row = _Row; m_Col = _Col; }
    void SetSize(Vec2 _Size) { m_Size = _Size; }
    void PushSelectedIndex(Vec2 _Index) { m_SelectedIndex.push_back(_Index); }
    int GetVectorSize() { return m_SelectedIndex.size(); }
    Vec2 GetSelectedIndex(int _Idx) { return m_SelectedIndex[_Idx]; }
    
    void ClearIndex() { m_SelectedIndex.clear(); }



    void SetSelectedChange(bool _Change) { m_Change = _Change; }
public:
    virtual void Tick() override;
    virtual void Render() override;

public:
    CLONE(CEditorBox);
    CEditorBox();
    ~CEditorBox();

};

