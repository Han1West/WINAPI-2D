#pragma once
#include "CComponent.h"

struct tTileInfo
{
    int ImgIdx;

    tTileInfo()
        : ImgIdx(-1)
    {}
};

class CCollider;

class CTileMap :
    public CComponent
{
private:
    UINT                m_Row;
    UINT                m_Col;
    Vec2                m_TileSize;

    CTexture*           m_Atlas;
    Vec2                m_AtlasTileSize;
    Vec2                m_AtlasResolution;
    UINT                m_AtlasMaxRow;
    UINT                m_AtlasMaxCol;

    vector<tTileInfo>   m_vecTileInfo;

    bool                m_IsChange;

public:


    void SetChange(bool _Change) { m_IsChange = _Change; }
    bool IsChange() { return m_IsChange; }

    void SetRowCol(UINT _Row, UINT _Col);
    UINT GetRow() { return m_Row; }
    UINT GetCol() { return m_Col; }

    void SetTileSize(Vec2 _TileSize) { m_TileSize = _TileSize; }
    Vec2 GetTileSize() { return m_TileSize; }

    void SetAtlas(CTexture* _Atlas);
    CTexture* GetAtlas() { return m_Atlas; }

    void SetAtlasTileSize(Vec2 _Size);

    tTileInfo* GetTileInfo(UINT _Col, UINT _Row);


public:
    virtual void FinalTick() override;
    void Render();
    void Render_Tile();
    void Render_Tile(HDC dc, HWND hWnd);

public:
    void Save(const wstring& _strPath);
    void Load(const wstring& _strPath);


public:    
    CLONE(CTileMap);
    CTileMap();
    ~CTileMap();

    friend class CLevel_Statge_Editor;
};

