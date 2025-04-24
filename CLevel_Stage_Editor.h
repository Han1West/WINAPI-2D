#pragma once
#include "CLevel.h"

class CTileMap;
class CEditorBox;
class CObj;

class CLevel_Stage_Editor :
    public CLevel
{
private:
	HMENU		m_hMenu;
	CTileMap*	m_TileMap;
	HWND		m_hdlg;
	HWND		m_ButtonHwnd;

	CEditorBox*  m_Box;
	
	vector<CObj*> m_vecAddedObj;
	multimap<wstring, Vec2>  m_mapObj;
	//vector<Vec2> m_vecIndex;
	


public:
	CTileMap* GetTileMap() { return m_TileMap; }
	CEditorBox* GetEidtorBox() { return m_Box; }
	

	wchar_t* LoadTextureName();
	
	void MouseCheck();
	void ObjectRemove();
	void SaveTile();
	void LoadTile();
	
	void LoadObject(wstring _str, Vec2 _Pos);
	

	void SaveObj();
	void LoadStage();

private:
	//void IndexPush(Vec2 _Index) { m_vecIndex.push_back(_Index); }
	//Vec2 GetCurrentIndex(int _Idx) { return m_vecIndex[_Idx]; }

private:
	virtual void Init() override;
	virtual void Exit() override;
	virtual void Tick() override;

public:
	CLevel_Stage_Editor();
	~CLevel_Stage_Editor();
};



