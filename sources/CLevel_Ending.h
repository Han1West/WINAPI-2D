#pragma once
#include "CLevel.h"

class CLevel_Ending :
    public CLevel
{
private:
    int m_Gold;
    float m_PlayTime;
    float m_AccTime;

    bool m_GoldCall;
    bool m_TimeCall;

public:
    virtual void Init() override;   // ���� �ʱ�ȭ
    virtual void Exit() override;   // ������ �����Ҷ� ����

    virtual void Tick() override;


public:
    CLevel_Ending(const CLevel_Ending& _Other) = delete;
    CLevel_Ending();
    ~CLevel_Ending();
};

