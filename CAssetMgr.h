#pragma once

class CTexture;
class CSound;

class CAssetMgr
{
	SINGLE(CAssetMgr);
private:
	map<wstring, CTexture*>		m_mapTex;
	map<wstring, CSound*>		m_mapSound;

public:
	void Init();

public:
	CTexture* LoadTexture(const wstring& _strKey, const wstring& _RelativePath);
	CTexture* FindTexture(const wstring& _strKey);
	CTexture* CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height);
	CTexture* LoadAtlas(const wstring& _RelativePath);

	CSound* LoadSound(const wstring& _strKey, const wstring& _RelativePath);
	CSound* FindSound(const wstring& _strKey);
};

