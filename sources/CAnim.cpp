#include "pch.h"
#include "CAnim.h"

#include "CObj.h"
#include "CEngine.h"
#include "CTexture.h"
#include "CTimeMgr.h"

#include "CAssetMgr.h"
#include "CLogMgr.h"

#include "CPlayer.h"
#include "CStateMachine.h"

CAnim::CAnim()
	: m_Owner(nullptr)
	, m_Atlas(nullptr)
	, m_CurIdx(0)
	, m_AccTime(0.f)
	, m_Finish(false)
	, m_AddTime(0.f)
	, m_Alpha(true)
{
}

CAnim::CAnim(const CAnim& _Other)
	: CBase(_Other)
	, m_Owner(nullptr)
	, m_vecFrm(_Other.m_vecFrm)
	, m_Atlas(_Other.m_Atlas)
	, m_CurIdx(_Other.m_CurIdx)
	, m_AccTime(_Other.m_AccTime)
	, m_AddTime(_Other.m_AddTime)
	, m_Finish(_Other.m_Finish)
	, m_Alpha(_Other.m_Alpha)
	
{
}

CAnim::~CAnim()
{
}

void CAnim::FinalTick()
{
	//LOG(LOG_LEVEL::LOG, L"AnimFinalTick");
	if (m_Finish)
		return;

	// 시간의 흐름을 체크
	m_AccTime += DT;

	// 진행된 시간이 현재 프레임의 Duration 을 초과하면
	if (m_vecFrm[m_CurIdx].Duration < m_AccTime)
	{	
		
		m_AccTime -= m_vecFrm[m_CurIdx].Duration;
		++m_CurIdx;

		if (m_vecFrm.size() <= m_CurIdx)
		{
			m_CurIdx = m_vecFrm.size() - 1.f;
			m_Finish = true;
		}
	}
}

void CAnim::Render()
{
	assert(m_Atlas);
	


	HDC dc = CEngine::Get()->GetBackDC();

	// Animation 을 소유한 오브젝트의 위치
	Vec2 vPos = m_Owner->GetOwner()->GetRenderPos();

	//TransparentBlt(dc, 
	//			  vPos.x - m_vecFrm[m_CurIdx].vSlice.x / 2.f + m_vecFrm[m_CurIdx].vOffset.x,
	//			  vPos.y - m_vecFrm[m_CurIdx].vSlice.y / 3.f + m_vecFrm[m_CurIdx].vOffset.y,
	//			  m_vecFrm[m_CurIdx].vSlice.x, m_vecFrm[m_CurIdx].vSlice.y,
	//			  m_Atlas->GetDC(),
	//			  m_vecFrm[m_CurIdx].vLeftTop.x, m_vecFrm[m_CurIdx].vLeftTop.y,
	//			  m_vecFrm[m_CurIdx].vSlice.x, m_vecFrm[m_CurIdx].vSlice.y,
	//			  RGB(255, 0, 255));

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_Owner->GetOwner());
	if (pPlayer)
	{
		CStateMachine* pSm = pPlayer->GetComponent<CStateMachine>();
		if (pSm->GetState(PLAYER_STATE::ISHIT))
		{
			blend.SourceConstantAlpha = 0;
			m_Alpha = false;

			m_AddTime += DT;
			if (m_AddTime > 0.05f)
			{
				if(false == m_Alpha)
				{
					blend.SourceConstantAlpha = 255;
				}
				if (m_AddTime > 0.1f)
				{
					m_Alpha = true;
					m_AddTime = 0.f;
				}
					
			}
		}
	}
		


	AlphaBlend(dc,
		vPos.x - m_vecFrm[m_CurIdx].vSlice.x / 2.f + m_vecFrm[m_CurIdx].vOffset.x,
		vPos.y - m_vecFrm[m_CurIdx].vSlice.y / 3.f + m_vecFrm[m_CurIdx].vOffset.y, 
		m_vecFrm[m_CurIdx].vSlice.x, m_vecFrm[m_CurIdx].vSlice.y,
		m_Atlas->GetDC(),
		m_vecFrm[m_CurIdx].vLeftTop.x, m_vecFrm[m_CurIdx].vLeftTop.y,
		m_vecFrm[m_CurIdx].vSlice.x, m_vecFrm[m_CurIdx].vSlice.y, blend);

	//AlphaBlend(hdc, 0 - vOffset.x, 0 - vOffset.y, width, height, m_Texture->GetDC(), vPos.x, vPos.y, width, height, blend);
}


void CAnim::Create(const tAnimDesc& _Info)
{
	SetName(_Info.AnimName);
	m_Atlas = _Info.pAtlas;
	
	for (int i = 0; i < _Info.FrmCount; ++i)
	{
		tAnimFrm frm = {};

		frm.vLeftTop.x = _Info.StartLeftTop.x + _Info.SliceSize.x * (float)i;
		frm.vLeftTop.y = _Info.StartLeftTop.y;

		frm.vSlice = _Info.SliceSize;

		frm.Duration = 1.f / (float)_Info.FPS;

		m_vecFrm.push_back(frm);
	}
}

void CAnim::Save(const wstring& _FolderPath)
{
	wstring strName = GetName();
	wstring FilePath = _FolderPath + strName + L".anim";
	
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, FilePath.c_str(), L"w");
	
	// 애니메이션 이름
	fwprintf_s(pFile, L"[ANIMATION_NAME]\n");
	fwprintf_s(pFile, strName.c_str());
	fwprintf_s(pFile, L"\n\n");

	// 참조하던 아틀라스 텍스쳐 정보
	fwprintf_s(pFile, L"[ATLAS_TEXTURE]\n");

	if (m_Atlas)
	{
		fwprintf_s(pFile, L"%s\n", m_Atlas->GetKey().c_str());
		fwprintf_s(pFile, L"%s\n", m_Atlas->GetRelativePath().c_str());
	}
	else
	{
		fwprintf_s(pFile, L"None\n");
		fwprintf_s(pFile, L"None\n");
	}
	fwprintf_s(pFile, L"\n");


	// 각 프레임별 데이터
	fwprintf_s(pFile, L"[FRAME_DATA]\n");
	fwprintf_s(pFile, L"Frame_Count %d\n\n", m_vecFrm.size());

	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		fwprintf_s(pFile, L"Frame_Index %d\n", i);
		fwprintf_s(pFile, L"Left_Top    %f %f\n", m_vecFrm[i].vLeftTop.x, m_vecFrm[i].vLeftTop.y);
		fwprintf_s(pFile, L"Offset      %f %f\n", m_vecFrm[i].vOffset.x, m_vecFrm[i].vOffset.y);
		fwprintf_s(pFile, L"Slice       %f %f\n", m_vecFrm[i].vSlice.x, m_vecFrm[i].vSlice.y);
		fwprintf_s(pFile, L"Duration    %f\n\n", m_vecFrm[i].Duration);
	}	
	
		
	
	fclose(pFile);
}

void CAnim::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _FilePath.c_str(), L"r");

	wchar_t szRead[255] = {};
	while (true)
	{		
		if (EOF == fwscanf_s(pFile, L"%s", szRead, 255))
			break;

		wstring str = szRead;

		if (str == L"[ANIMATION_NAME]")
		{
			fwscanf_s(pFile, L"%s", szRead, 255);
			SetName(szRead);
		}
		else if (str == L"[ATLAS_TEXTURE]")
		{
			wstring strKey, strRelativePath;
			fwscanf_s(pFile, L"%s", szRead, 255);
			strKey = szRead;

			fwscanf_s(pFile, L"%s", szRead, 255);
			strRelativePath = szRead;

			if (strKey != L"None")
			{
				m_Atlas = CAssetMgr::Get()->LoadTexture(strKey, strRelativePath);
			}			
		}
		else if (str == L"[FRAME_DATA]")			
		{
			wchar_t FrameBuff[255] = {};

			tAnimFrm frm = {};

			while (true)
			{
				if (EOF == fwscanf_s(pFile, L"%s", FrameBuff, 255))
				{
					break;
				}
				
				if (!wcscmp(L"Left_Top", FrameBuff))
				{
					fwscanf_s(pFile, L"%f %f", &frm.vLeftTop.x, &frm.vLeftTop.y);
				}
				else if (!wcscmp(L"Offset", FrameBuff))
				{
					fwscanf_s(pFile, L"%f %f", &frm.vOffset.x, &frm.vOffset.y);
				}
				else if (!wcscmp(L"Slice", FrameBuff))
				{
					fwscanf_s(pFile, L"%f %f", &frm.vSlice.x, &frm.vSlice.y);
				}
				else if (!wcscmp(L"Duration", FrameBuff))
				{
					fwscanf_s(pFile, L"%f", &frm.Duration);
					m_vecFrm.push_back(frm);
				}				
			}			
		}
	}

	fclose(pFile);
}

//void CAnim::Save(const wstring& _FolderPath)
//{
//	wstring strName = GetName();
//	wstring FilePath = _FolderPath + strName + L".anim";
//
//	FILE* pFile = nullptr;
//	_wfopen_s(&pFile, FilePath.c_str(), L"wb");
//
//	// 애니메이션 이름
//	SaveWString(strName, pFile);
//	
//	// 각 프레임별 데이터
//	size_t size = m_vecFrm.size();
//	fwrite(&size, sizeof(size_t), 1, pFile);
//	fwrite(m_vecFrm.data(), sizeof(tAnimFrm), size, pFile);
//
//	// 참조하던 아틀라스 텍스쳐 정보
//	SaveAssetRef(m_Atlas, pFile);
//	
//
//	fclose(pFile);
//}
//
//void CAnim::Load(const wstring& _FilePath)
//{
//	FILE* pFile = nullptr;
//	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
//
//	// Animation 이름
//	wstring strName;
//	LoadWString(strName, pFile);
//	SetName(strName);
//
//	// 프레임 데이터
//	size_t size = 0;
//	fread(&size, sizeof(size_t), 1, pFile);
//
//	m_vecFrm.resize(size);
//	fread(m_vecFrm.data(), sizeof(tAnimFrm), size, pFile);
//
//	// Atlas 텍스쳐
//	m_Atlas = LoadTextureRef(pFile);
//
//	fclose(pFile);
//}