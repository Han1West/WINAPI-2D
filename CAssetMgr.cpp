#include "pch.h"
#include "CAssetMgr.h"

#include "CPathMgr.h"
#include "CTexture.h"
#include "CSound.h"

CAssetMgr::CAssetMgr()
{

}

CAssetMgr::~CAssetMgr()
{
	Release_Map(m_mapTex);
	Release_Map(m_mapSound);
}

void CAssetMgr::Init()
{
}

CTexture* CAssetMgr::LoadTexture(const wstring& _strKey, const wstring& _RelativePath)
{
	// 동일한 키값으로 이미 텍스쳐가 있는지 확인
	CTexture* pTexture = FindTexture(_strKey);

	// 이미 있으면 그걸 반환
	if (pTexture)
		return pTexture;

	// 없었으면, 입력으로 들어온 상대경로와 Content 폴더까지의 경로를 합쳐서 파일의 최종 경로를 만들어냄
	wstring strFilePath = CPathMgr::Get()->GetContentPath();
 	strFilePath += _RelativePath;

	// 텍스쳐 객체하나 생성, 완성된 경로로 비트맵 로딩 진행
	pTexture = new CTexture;
	if (FAILED(pTexture->Load(strFilePath)))
	{
		delete pTexture;
		return nullptr;
	}
	
	// 로딩된 텍스쳐를 AssetMgr 에 등록
	m_mapTex.insert(make_pair(_strKey, pTexture));

	// 로딩된 텍스쳐가 자신의 Key, RelativePath 를 알게 한다.
	pTexture->m_Key = _strKey;
	pTexture->m_RelativePath = _RelativePath;

	// 로딩한 텍스쳐를 반환
	return pTexture;
}

CTexture* CAssetMgr::FindTexture(const wstring& _strKey)
{
	// 동일한 키값으로 텍스쳐가 있는지 확인
	map<wstring, CTexture*>::iterator iter = m_mapTex.find(_strKey);

	// 텍스쳐가 이미 있었으면 그걸 반환
	if (iter == m_mapTex.end())
	{
		return nullptr;
	}

	return iter->second;
}

CTexture* CAssetMgr::CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height)
{
	// 동일한 키값으로 이미 텍스쳐가 있는지 확인
	CTexture* pTexture = FindTexture(_strKey);
	assert(!pTexture);

	pTexture = new CTexture;
	pTexture->Create(_Width, _Height);

	m_mapTex.insert(make_pair(_strKey, pTexture));
	pTexture->m_Key = _strKey;

	return pTexture;
}

CTexture* CAssetMgr::LoadAtlas(const wstring& _RelativePath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _RelativePath.c_str(), L"rb");

	// 타일이 참조하던 아틀라스 텍스쳐 정보
	CTexture* pTexture = LoadTextureRef(File);

	fclose(File);
	return pTexture;
}

CSound* CAssetMgr::LoadSound(const wstring& _strKey, const wstring& _RelativePath)
{
	// 동일한 키값으로 이미 사운드가 있는지 확인
	CSound* pSound = FindSound(_strKey);

	// 이미 있으면 그걸 반환
	if (pSound)
		return pSound;

	// 없었으면, 입력으로 들어온 상대경로와 Content 폴더까지의 경로를 합쳐서 파일의 최종 경로를 만들어냄
	wstring strFilePath = CPathMgr::Get()->GetContentPath();
	strFilePath += _RelativePath;

	// 사운드 객체하나 생성, 완성된 경로로 사운드 로딩 진행
	pSound = new CSound;
	if (FAILED(pSound->Load(strFilePath)))
	{
		delete pSound;
		return nullptr;
	}

	// 로딩된 사운드를 AssetMgr 에 등록
	m_mapSound.insert(make_pair(_strKey, pSound));

	// 로딩된 사운드가 자신의 Key, RelativePath 를 알게 한다.
	pSound->m_Key = _strKey;
	pSound->m_RelativePath = _RelativePath;

	// 로딩한 사운드를 반환
	return pSound;
}

CSound* CAssetMgr::FindSound(const wstring& _strKey)
{
	// 동일한 키값으로 사운드가 있는지 확인
	map<wstring, CSound*>::iterator iter = m_mapSound.find(_strKey);
	
	// 키값에 해당하는 사운드가 없으면 nullptr 반환
	if (iter == m_mapSound.end())
	{
		return nullptr;
	}

	// 찾았으면, 찾은 사운드 반환
	return iter->second;
}
