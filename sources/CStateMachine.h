#pragma once
#include "CComponent.h"

class CState;

enum PLAYER_STATE
{
    ISRIGHT,
    ISLEFT,
    ISJUMPING,
    ISMOVING,
    ISRUNNING,
    SEARCHING,
    ISCRAWL,
    ISATTACK,
    ISUP,
    ISDOWN,
    CANHANG,
    ISHANG,
    ISCLIMB,
    ISHOLDING,
    GETSHOTGUN,
    ISHIT,
    ISSTUN,
    ISDEAD,
    CANRIGHT,
    CANLEFT,
    ISEdge,
    CANEXIT,
    CANBUY,
    GETEND,
    
    END,
};

enum class MONSTER_STATE
{
    ISRIGHT,
    ISATTACK,
    ISJUMP,

    END,
};


class CStateMachine :
    public CComponent
{
private:
    map<wstring, CState*>   m_mapState;
    CState*                 m_CurState;
    CState*                 m_PrevState;

    vector<bool>            m_vecState;
    bool                    m_vecMonsterState[(int)MONSTER_STATE::END];
    bool                    m_IsStateChange;
    
public:
    void AddState(const wstring& _StateName, CState* _State);
    CState* FindState(const wstring& _StateName);
    void ChangeState(const wstring& _StateName);

    CState* GetCurState() { return m_CurState; }
    CState* GetPrevState() { return m_PrevState; }

    void SetState(int _StateName, bool _State) { m_vecState[_StateName] = _State; m_IsStateChange = true; }
    bool GetState(int _StateName) { return m_vecState[_StateName]; }

    void SetStateChange(bool _Change) { m_IsStateChange = _Change; }
    bool IsStateChange() { return m_IsStateChange; }

    void SetMonsterState(int _StateName, bool _State) { m_vecMonsterState[_StateName] = _State; }
    bool GetMonsterState(int _StateName) { return m_vecMonsterState[_StateName]; }

public:
    virtual void FinalTick() override;

public:
    CLONE(CStateMachine);
    CStateMachine();
    CStateMachine(const CStateMachine& _Other);
    ~CStateMachine();
};

