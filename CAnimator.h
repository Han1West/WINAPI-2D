#pragma once
#include "CComponent.h"

class CTexture;

struct tAnimDesc
{
    wstring     AnimName;
    CTexture*   pAtlas;
    Vec2        StartLeftTop;
    Vec2        SliceSize;
    int         FrmCount;
    int         FPS;    
};

class CAnim;

// 다수의 Animation 을 관리
class CAnimator :
    public CComponent
{
private:
    map<wstring, CAnim*>    m_mapAnim;
    CAnim*                  m_CurAnim;
    bool                    m_Repeat;

public:
    void CreateAnimation(const tAnimDesc& _Info);
    CAnim* FindAnimation(const wstring& _Name);
    void Play(const wstring& _Name, bool _Repeat);
    void Render();
    void SaveAnimation(const wstring& _RelativeFolder);

    void LoadAnimation(const wstring& _RelativePath);

public:
    virtual void FinalTick() override;

public:
    CLONE(CAnimator);
    CAnimator();
    CAnimator(const CAnimator& _Other);
    ~CAnimator();
};

