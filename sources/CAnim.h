#pragma once
#include "CBase.h"
#include "CAnimator.h"

struct tAnimFrm
{
    Vec2    vLeftTop;
    Vec2    vSlice;
    Vec2    vOffset;
    float   Duration;
};

class CTexture;

class CAnim :
    public CBase
{
private:
    CAnimator*          m_Owner;
    vector<tAnimFrm>    m_vecFrm;
    CTexture*           m_Atlas;
    int                 m_CurIdx;
    float               m_AccTime;
    float               m_AddTime;
    bool                m_Finish;
    bool                m_Alpha;

public:
    void Create(const tAnimDesc& _Info);
    bool IsFinish() { return m_Finish; }
    void Reset()
    {
        m_Finish = false;
        m_AccTime = 0.f;
        m_CurIdx = 0;
    }

    tAnimFrm& GetFrame(int _Idx) { return m_vecFrm[_Idx]; }
    CTexture* GetAtlas() { return m_Atlas; }



public:
    void FinalTick();
    void Render();

    void Save(const wstring& _FolderPath);
    void Load(const wstring& _FilePath);

public:
    CLONE(CAnim);
    CAnim();
    CAnim(const CAnim& _Other);
    ~CAnim();

    friend class CAnimator;
};

