#pragma once
#include "CLevel.h"

class CTileMap;
class CTextureUI;
class CTextUI;
class COlmec;
class CExit;

class CLevel_BossStage :
    public CLevel
{
private:
    CTileMap* m_TileMap;
    CTextureUI* m_StageUI;
    CTextUI* m_TextUI[4];
    float m_AccTime;
    float m_PlayTime;

    COlmec* m_Olmec;
    CExit* m_Exit;

public:
    //CTileMap* GetTileMap() { return m_TileMap; }
    void AddObjectToStage();

public:
    virtual void Init() override;   // 레벨 초기화
    virtual void Exit() override;   // 레벨을 종료할때 수행

    virtual void Tick() override;


public:
    CLevel_BossStage(const CLevel_BossStage& _Other) = delete;
    CLevel_BossStage();
    ~CLevel_BossStage();
};

