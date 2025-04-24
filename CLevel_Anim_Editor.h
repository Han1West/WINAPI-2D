#pragma once
#include "CLevel.h"

class CBackground;
class CAnim;

struct tFrameInfo
{
	float		Duration;
	Vec2		StartPosition;
	Vec2		SliceSize;
	Vec2		Offset;
	Vec2		ColliderPosition;
	Vec2		ColliderSize;
};

struct tAnimInfo
{
	wstring		TextureName;
	wstring		TexturePath;
	wstring		AnimName;
	int			MaxFrame;
	int			CurFrame;
	vector<tFrameInfo>	vecFrameInfo;
};



class CLevel_Anim_Editor
	: public CLevel
{
private:
	HMENU				m_hMenu;
	CBackground*		m_Texture;
	vector<tFrameInfo>	m_vecFrm;

	Vec2				m_CurPos;
	Vec2				m_FrameScale;


public:
	wchar_t* LoadTextureName();
	void LoadAnimation();
	void SaveAnimation(tAnimInfo& _Info);

	void SetFrameInfo(HWND hWnd, int _CurFrame);
	void SetCurPos(Vec2 _Pos) { m_CurPos = _Pos; }
	void SetFrameScale(Vec2 _Scale) { m_FrameScale = _Scale; }
	
	tFrameInfo& GetFrame(int _Idx) { return m_vecFrm[_Idx]; }
	void ApplyFrameInfo(int _Idx, tFrameInfo& _FrameInfo) { m_vecFrm[_Idx] = _FrameInfo; }
	
	void AddFrame(tFrameInfo& _Info) { m_vecFrm.push_back(_Info); }
	void PopFrame(int _Idx) { m_vecFrm.erase(m_vecFrm.begin() + _Idx); }
	void ApplyFrameInfo(tFrameInfo& _Info, int _Idx) { m_vecFrm[_Idx] = _Info; }



	CBackground* GetBackground() { return m_Texture; }

private:
	virtual void Init() override;
	virtual void Exit() override;
	virtual void Tick() override;

public:
	CLevel_Anim_Editor();
	~CLevel_Anim_Editor();
};