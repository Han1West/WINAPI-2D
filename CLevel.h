#pragma once
#include "CBase.h"

class CObj;
class CPlayer;
class CTileMap;
class CTextUI;
class CSound;

struct PlayerInfo
{
    int HP;
    int Gold;
    int Bomb;
    int Rope;
    int FeetDamage;
    
    float PlayTime;
    bool JumpSpeed;
    bool HasGlove;
    bool GetShotgun;

    vector<wstring> ItemList;
};

class CLevel :
    public CBase
{
private:    
    vector<CObj*>   m_arrLayer[(int)LAYER_TYPE::END];
    CPlayer*        m_CurPlayer;
    vector<CObj*>   m_vecObject;
    CTileMap*       m_TileMap;
    multimap<wstring, Vec2>  m_mapObj;
    PlayerInfo      m_pInfo;
    CSound*         m_BGM;

    bool m_Change;

public:
    void SetChange(bool _Change) { m_Change = _Change; }
    bool IsChange() { return m_Change; }

    void AddObject(CObj* _Obj, LAYER_TYPE _Type);
    vector<CObj*>& GetLayer(LAYER_TYPE _Type) { return m_arrLayer[(int)_Type]; }

    void RegisterPlayer(CPlayer* _Player) { m_CurPlayer = _Player; }
    CPlayer* GetPlayer() { return m_CurPlayer; }
    void DeleteObjects(LAYER_TYPE _Type);
    void DeleteAllObjects();

    void PushObject(CObj* _Obj) { m_vecObject.push_back(_Obj); }
    vector<CObj*> GetObjectVector() { return m_vecObject; }

    void InsertObj(wstring _Str, Vec2 _Pos) { m_mapObj.insert(make_pair(_Str, _Pos)); }
    multimap<wstring, Vec2> GetObjmap() { return m_mapObj; }

    CTileMap* GetTileMap() { return m_TileMap; }
    void SetTileMap(CTileMap* _TileMap) { m_TileMap = _TileMap; }

    void LoadObjectToStage(wstring _str, Vec2 _pos);

    void ChangePlayerInfo(CTextUI* _Text[]);
    void CheckItemUI();

    void SavePlayerInfo();
    void LoadPlayerInfo();
    void UpdatePlayTime(float _AccTime);
    float LoadPlayTime();

    void SetBGM(CSound* _BGM) { m_BGM = _BGM; }
    CSound* GetBGM() { return m_BGM; }
    
    void SetpInfo(PlayerInfo _pInfo) { m_pInfo = _pInfo; }
    PlayerInfo GetpIfno() { return m_pInfo; }
public:
    virtual void Init() = 0;
    virtual void Exit() = 0;
  

    virtual void Tick();
    virtual void FinalTick();
    virtual void Render();

    virtual void ObjectSave(const wstring& _strPath);
    virtual void ObjectLoad(const wstring& _strPath);
    //virtual void SaveStage(CTileMap* _TileMap, vector<CObj*> _vectorObj, const wstring& _strPath);
    //virtual void LoadStage();

public:
    CLONE_DISABLE(CLevel);
    CLevel();
    virtual ~CLevel();
};

