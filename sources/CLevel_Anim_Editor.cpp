#include "pch.h"
#include "CLevel_Anim_Editor.h"
#include "pch.h"
#include "CLevel_Editor.h"

#include "CObj.h"

#include "CEngine.h"
#include "CLogMgr.h"
#include "CPathMgr.h"

#include "CKeyMgr.h"
#include "CTile.h"
#include "CTileMap.h"
#include "CBackground.h"
#include "resource.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CAssetMgr.h"
#include "CTimeMgr.h"

#include "CEngine.h"
#include "CTileMap.h"

#include "CBtnUI.h"
#include "CPanel.h"
#include "CUIMgr.h"

#include "CAnimator.h"
#include "CAnim.h"

#include "CTexture.h"
#include "CPlatform.h"

#include "CSound.h"
#include "CDebugMgr.h"

#include "resource.h"

tAnimInfo pAnimInfo = {};
tFrameInfo pFrameInfo = {};
HWND g_hModlessWnd = nullptr;

CLevel_Anim_Editor::CLevel_Anim_Editor()
	: m_hMenu(nullptr)
{
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCE(IDC_WINAPI));
}

CLevel_Anim_Editor::~CLevel_Anim_Editor()
{
	DestroyMenu(m_hMenu);
}



void CLevel_Anim_Editor::Init()
{
	// BGM 지정
	//CSound* pBGM = CAssetMgr::Get()->LoadSound(L"BattleBGM", L"sound\\BGM_Stage1.wav");
	//pBGM->Play();

	// 메뉴 추가	
	SetMenu(CEngine::Get()->GetMainHwnd(), m_hMenu);

	// 메뉴가 붙었으니까, 다시 윈도우 크기 재계산
	UINT width = CEngine::Get()->GetResolution().x;
	UINT height = CEngine::Get()->GetResolution().y;
	CEngine::Get()->ChangeWindowResolution(width, height);


	m_Texture = new CBackground;
	m_Texture->SetName(L"Current Atlas");

	AddObject(m_Texture, LAYER_TYPE::BACKGROUND);
}

void CLevel_Anim_Editor::Exit()
{
	// 메뉴 제거
	SetMenu(CEngine::Get()->GetMainHwnd(), nullptr);

	// 메뉴가 제거되었으니, 윈도우 크기 다시 계산
	UINT width = CEngine::Get()->GetResolution().x;
	UINT height = CEngine::Get()->GetResolution().y;
	CEngine::Get()->ChangeWindowResolution(width, height);


	// 오브젝트 삭제
	DeleteAllObjects();
}

void CLevel_Anim_Editor::Tick()
{
	CLevel::Tick();
	DrawDebugShape(DEBUG_SHAPE::RECT_ANIM, PEN_TYPE::GREEN, BRUSH_TYPE::HOLLOW, m_CurPos, m_FrameScale);

	if (KEY_TAP(KEY::ENTER))
	{
		ChangeLevel((int)LEVEL_TYPE::START);
		CCamera::Get()->SetFilterEffect(FADE_OUT, 1.f);
		CCamera::Get()->SetFilterEffect(FADE_IN, 1.f);
	}

	Vec2 pLookAt = CCamera::Get()->GetLookAt();
	float pCamSpeed = CCamera::Get()->GetCamSpeed();

	if (KEY_PRESSED(KEY::W))
		pLookAt.y -= pCamSpeed * DT;

	if (KEY_PRESSED(KEY::S))
		pLookAt.y += pCamSpeed * DT;

	if (KEY_PRESSED(KEY::A))
		pLookAt.x -= pCamSpeed * DT;

	if (KEY_PRESSED(KEY::D))
		pLookAt.x += pCamSpeed * DT;

	CCamera::Get()->SetLookAt(pLookAt);
}

wchar_t* CLevel_Anim_Editor::LoadTextureName()
{
	wstring strContentPath = CPathMgr::Get()->GetContentPath();
	wstring* ptr = nullptr;
	m_vecFrm.clear();

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};
	wchar_t Name[255] = {};
	wchar_t* Pos = nullptr;
	

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Evrey File(*.*)\0*.*\0";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetOpenFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			CAssetMgr::Get()->LoadAtlas(szFilePath);
		}
	}

	wcscpy_s(Name, szFilePath);
	Pos = wcsrchr(Name, '\\');
	wcscpy_s(Name, Pos + 1);

	CUIMgr::Get()->SetPreventFrame(1);

	return Name;
}

void CLevel_Anim_Editor::LoadAnimation()
{
	wstring strContentPath = CPathMgr::Get()->GetContentPath();

	// 파일 경로 문자열
	wchar_t szFilePath[100] = {};
	m_vecFrm.clear();

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Evrey File(*.*)\0*.anim\0";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	GetOpenFileName(&Desc);
	
	//pAnim->Load(szFilePath);
	wstring wFilePath = (wstring)szFilePath;

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, wFilePath.c_str(), L"r");

	wchar_t szRead[255] = {};
	while (true)
	{
		if (EOF == fwscanf_s(pFile, L"%s", szRead, 255))
			break;

		wstring str = szRead;

		if (str == L"[ANIMATION_NAME]")
		{
			fwscanf_s(pFile, L"%s", szRead, 255);
			pAnimInfo.AnimName = szRead;
		}
		else if (str == L"[ATLAS_TEXTURE]")
		{
			wstring strKey, strRelativePath;
			fwscanf_s(pFile, L"%s", szRead, 255);
			pAnimInfo.TextureName = szRead;

			fwscanf_s(pFile, L"%s", szRead, 255);
			pAnimInfo.TexturePath = szRead;
		}
		else if (str == L"[FRAME_DATA]")
		{
			wchar_t FrameBuff[255] = {};
			fwscanf_s(pFile, L"%s", FrameBuff, 255);
			if (!wcscmp(L"Frame_Count", FrameBuff))
			{
				fwscanf_s(pFile, L"%d", &pAnimInfo.MaxFrame);
			}

			while (true)
			{
				if (EOF == fwscanf_s(pFile, L"%s", FrameBuff, 255))
				{
					break;
				}
			

				if (!wcscmp(L"Left_Top", FrameBuff))
				{
					fwscanf_s(pFile, L"%f %f", &pFrameInfo.StartPosition.x, &pFrameInfo.StartPosition.y);
				}
				else if (!wcscmp(L"Offset", FrameBuff))
				{
					fwscanf_s(pFile, L"%f %f", &pFrameInfo.Offset.x, &pFrameInfo.Offset.y);
				}
				else if (!wcscmp(L"Slice", FrameBuff))
				{
					fwscanf_s(pFile, L"%f %f", &pFrameInfo.SliceSize.x, &pFrameInfo.SliceSize.y);
				}
				else if (!wcscmp(L"Duration", FrameBuff))
				{
					fwscanf_s(pFile, L"%f", &pFrameInfo.Duration);
					m_vecFrm.push_back(pFrameInfo);
				}
			}
			
		}
	}
	pAnimInfo.vecFrameInfo = m_vecFrm;
	pAnimInfo.CurFrame = 0;

	fclose(pFile);

	CUIMgr::Get()->SetPreventFrame(1);
}

void CLevel_Anim_Editor::SaveAnimation(tAnimInfo& _Info)
{
	wstring strContentPath = CPathMgr::Get()->GetContentPath();

	strContentPath = strContentPath + L"animation\\";

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Anim(*.*)\0*.anim\0";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	GetSaveFileName(&Desc);

	//wstring FilePath = strContentPath + L"animation\\" + _Info.AnimName + L".anim";
	wstring FilePath = wstring(szFilePath);

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, FilePath.c_str(), L"w");

	// 애니메이션 이름
	fwprintf_s(pFile, L"[ANIMATION_NAME]\n");
	fwprintf_s(pFile, _Info.AnimName.c_str());
	fwprintf_s(pFile, L"\n\n");

	// 참조하던 아틀라스 텍스쳐 정보
	fwprintf_s(pFile, L"[ATLAS_TEXTURE]\n");

	fwprintf_s(pFile, L"%s\n", pAnimInfo.TextureName.c_str());
	fwprintf_s(pFile, L"%s\n", pAnimInfo.TexturePath.c_str());
	fwprintf_s(pFile, L"\n");


	// 각 프레임별 데이터
	fwprintf_s(pFile, L"[FRAME_DATA]\n");
	fwprintf_s(pFile, L"Frame_Count %d\n\n", pAnimInfo.MaxFrame);

	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		fwprintf_s(pFile, L"Frame_Index %d\n", i);
		fwprintf_s(pFile, L"Left_Top    %f %f\n", m_vecFrm[i].StartPosition.x, m_vecFrm[i].StartPosition.y);
		fwprintf_s(pFile, L"Offset      %f %f\n", m_vecFrm[i].Offset.x, m_vecFrm[i].Offset.y);
		fwprintf_s(pFile, L"Slice       %f %f\n", m_vecFrm[i].SliceSize.x, m_vecFrm[i].SliceSize.y);
		fwprintf_s(pFile, L"Duration    %f\n\n", m_vecFrm[i].Duration);
	}

	fclose(pFile);
	CUIMgr::Get()->SetPreventFrame(1);
}

void CLevel_Anim_Editor::SetFrameInfo(HWND hWnd, int _CurFrame)
{

	tFrameInfo CurFrameInfo = GetFrame(_CurFrame);

	SetCurPos(Vec2(CurFrameInfo.StartPosition.x, CurFrameInfo.StartPosition.y));
	SetFrameScale(Vec2(CurFrameInfo.SliceSize.x, CurFrameInfo.SliceSize.y));

	wstring StartPositionX = std::to_wstring(CurFrameInfo.StartPosition.x);
	wstring StartPositionY = std::to_wstring(CurFrameInfo.StartPosition.y);
	wstring SliceSizeX = std::to_wstring(CurFrameInfo.SliceSize.x);
	wstring SliceSizeY = std::to_wstring(CurFrameInfo.SliceSize.y);
	wstring OffsetX = std::to_wstring(CurFrameInfo.Offset.x);
	wstring OffsetY = std::to_wstring(CurFrameInfo.Offset.y);
	wstring fDuration = std::to_wstring(CurFrameInfo.Duration);
	
	SetDlgItemInt(hWnd, IDC_MAX_FRAME, pAnimInfo.MaxFrame, false);	
	SetDlgItemInt(hWnd, IDC_FRAME_INDEX, pAnimInfo.CurFrame + 1, false);
	SetDlgItemText(hWnd, IDC_DURATION, fDuration.c_str());
	SetDlgItemText(hWnd, IDC_SP_X, StartPositionX.c_str());
	SetDlgItemText(hWnd, IDC_SP_Y, StartPositionY.c_str());
	//SetDlgItemText(hWnd, IDC_CP_X, Name);
	//SetDlgItemText(hWnd, IDC_CP_Y, Name);
	SetDlgItemText(hWnd, IDC_SS_X, SliceSizeX.c_str());
	SetDlgItemText(hWnd, IDC_SS_Y, SliceSizeY.c_str());
	//SetDlgItemText(hWnd, IDC_CS_X, Name);
	//SetDlgItemText(hWnd, IDC_CS_X, Name);
	SetDlgItemText(hWnd, IDC_OFFSET_X, OffsetX.c_str());
	SetDlgItemText(hWnd, IDC_OFFSET_y, OffsetY.c_str());
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK AnimationProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		CLevel_Anim_Editor* pLevel = dynamic_cast<CLevel_Anim_Editor*>(CLevelMgr::Get()->GetCurrentLevel());

		if (LOWORD(wParam) == IDC_TEXTURE_LOAD)
		{
			// 에디터 레벨에서만 사용해야하는 다이얼로그 윈도우가 다른레벨에서 띄워진 경우
			assert(pLevel);
			wchar_t pFilePath[255] = {};
			wchar_t* pTextureName = pLevel->LoadTextureName();
			wchar_t pFileName[10] = L"texture\\";
			wchar_t Name[20] = {};
			wchar_t* Pos = nullptr;

			wcscpy_s(Name, pTextureName);
			Pos = wcsrchr(Name, '.');
			*Pos = '\0';

			wcscat_s(pFilePath, pFileName);
			wcscat_s(pFilePath, pTextureName);

			std::wstring strFilePath(pFilePath);
			std::wstring strName(Name);

			CBackground* pAtlas = pLevel->GetBackground();

			pAtlas->SetAtlas(CAssetMgr::Get()->LoadTexture(strName, strFilePath));

			pAnimInfo = {};
			pFrameInfo = {};

			wstring StartPositionX = std::to_wstring(pFrameInfo.StartPosition.x);
			wstring StartPositionY = std::to_wstring(pFrameInfo.StartPosition.y);
			wstring SliceSizeX = std::to_wstring(pFrameInfo.SliceSize.x);
			wstring SliceSizeY = std::to_wstring(pFrameInfo.SliceSize.y);
			wstring OffsetX = std::to_wstring(pFrameInfo.Offset.x);
			wstring OffsetY = std::to_wstring(pFrameInfo.Offset.y);
			wstring fDuration = std::to_wstring(pFrameInfo.Duration);

			
			SetDlgItemInt(hWnd, IDC_MAX_FRAME, pAnimInfo.MaxFrame, false);
			SetDlgItemInt(hWnd, IDC_FRAME_INDEX, pAnimInfo.CurFrame + 1, false);
			SetDlgItemText(hWnd, IDC_ANIMATION_NAME, pAnimInfo.AnimName.c_str());
			SetDlgItemText(hWnd, IDC_DURATION, fDuration.c_str());
			SetDlgItemText(hWnd, IDC_SP_X, StartPositionX.c_str());
			SetDlgItemText(hWnd, IDC_SP_Y, StartPositionY.c_str());
			SetDlgItemText(hWnd, IDC_SS_X, SliceSizeX.c_str());
			SetDlgItemText(hWnd, IDC_SS_Y, SliceSizeY.c_str());
			SetDlgItemText(hWnd, IDC_OFFSET_X, OffsetX.c_str());
			SetDlgItemText(hWnd, IDC_OFFSET_y, OffsetY.c_str());

	
			

			SetDlgItemText(hWnd, IDC_TEXTURE_NAME, Name);
			pAnimInfo.TextureName = strName;
			pAnimInfo.TexturePath = strFilePath;
			

			// Render를 위한 Dialog창 생성
			
			g_hModlessWnd = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_CUR_ANIM), hWnd, DlgProc);
			ShowWindow(g_hModlessWnd, SW_SHOW);

			// 화면 Update;
			CEngine::Get()->Progress();
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_ANIMATION_LOAD)
		{
			// 에디터 레벨에서만 사용해야하는 다이얼로그 윈도우가 다른레벨에서 띄워진 경우
			assert(pLevel);
			pLevel->LoadAnimation();
			

			CBackground* pAtlas = pLevel->GetBackground();
			pAtlas->SetAtlas(CAssetMgr::Get()->LoadTexture(pAnimInfo.TextureName, pAnimInfo.TexturePath));

			tFrameInfo CurFrameInfo = pAnimInfo.vecFrameInfo[pAnimInfo.CurFrame];
			pLevel->SetFrameInfo(hWnd, pAnimInfo.CurFrame);
			SetDlgItemText(hWnd, IDC_TEXTURE_NAME , pAnimInfo.TextureName.c_str());
			SetDlgItemText(hWnd, IDC_ANIMATION_NAME, pAnimInfo.AnimName.c_str());
			g_hModlessWnd = CreateDialog(nullptr, MAKEINTRESOURCE(IDD_CUR_ANIM), hWnd, DlgProc);
	
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_NEXT_FRAME)
		{	
			// Frmae 정보 표시 변경
			if (pAnimInfo.CurFrame >= GetDlgItemInt(hWnd, IDC_MAX_FRAME, nullptr, true) - 1)
				pAnimInfo.CurFrame = 0;
			else
				++pAnimInfo.CurFrame;
			
			pLevel->SetFrameInfo(hWnd, pAnimInfo.CurFrame);

			// WM_PAINT 재 호출
			SendMessage(g_hModlessWnd, WM_PAINT, 0, 0);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_PREV_FRAME)
		{
			// Frmae 정보 표시 변경
			if (pAnimInfo.CurFrame <= 0)
				pAnimInfo.CurFrame = GetDlgItemInt(hWnd, IDC_MAX_FRAME, nullptr, true) - 1;
			else
				--pAnimInfo.CurFrame;
			 
			// WM_PAINT 재 호출
			SendMessage(g_hModlessWnd, WM_PAINT, 0, 0);
			pLevel->SetFrameInfo(hWnd, pAnimInfo.CurFrame);

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_ADD_FRAME)
		{
			// 현재 프레임이 마지막 프레임 이였다면 다음 프레임으로 자동 증가
			if (pAnimInfo.CurFrame == (pAnimInfo.MaxFrame - 1))
				++pAnimInfo.CurFrame;

			// Max Frame 증가
			++pAnimInfo.MaxFrame;

			tFrameInfo pNewFrame = {};
			pLevel->AddFrame(pNewFrame);
			pLevel->SetFrameInfo(hWnd, pAnimInfo.CurFrame);

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_DELETE_FRAME)
		{
			if (pAnimInfo.CurFrame == pAnimInfo.MaxFrame - 1)
				--pAnimInfo.CurFrame;

			--pAnimInfo.MaxFrame;

			pLevel->PopFrame(pAnimInfo.CurFrame);
			pLevel->SetFrameInfo(hWnd, pAnimInfo.CurFrame);
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_SPX_UP)
		{
			int val = GetDlgItemInt(hWnd, IDC_SP_X, nullptr, true);
			SetDlgItemInt(hWnd, IDC_SP_X, val + 1, true);
		}
		else if (LOWORD(wParam) == IDC_SPX_DOWN)
		{
			int val = GetDlgItemInt(hWnd, IDC_SP_X, nullptr, true);
			SetDlgItemInt(hWnd, IDC_SP_X, val - 1, true);
		}
		else if (LOWORD(wParam) == IDC_SPY_UP)
		{
			int val = GetDlgItemInt(hWnd, IDC_SP_Y, nullptr, true);
			SetDlgItemInt(hWnd, IDC_SP_Y, val + 1, true);
		}
		else if (LOWORD(wParam) == IDC_SPY_DOWN)
		{
			int val = GetDlgItemInt(hWnd, IDC_SP_Y, nullptr, true);
			SetDlgItemInt(hWnd, IDC_SP_Y, val - 1, true);
		}
		else if (LOWORD(wParam) == IDC_SSX_UP)
		{
			int val = GetDlgItemInt(hWnd, IDC_SS_X, nullptr, true);
			SetDlgItemInt(hWnd, IDC_SS_X, val + 1, true);
		}
		else if (LOWORD(wParam) == IDC_SSX_DOWN)
		{
			int val = GetDlgItemInt(hWnd, IDC_SS_X, nullptr, true);
			SetDlgItemInt(hWnd, IDC_SS_X, val - 1, true);
		}
		else if (LOWORD(wParam) == IDC_SSY_UP)
		{
			int val = GetDlgItemInt(hWnd, IDC_SS_Y, nullptr, true);
			SetDlgItemInt(hWnd, IDC_SS_Y, val + 1, true);
		}
		else if (LOWORD(wParam) == IDC_SSY_DOWN)
		{
			int val = GetDlgItemInt(hWnd, IDC_SS_Y, nullptr, true);
			SetDlgItemInt(hWnd, IDC_SS_Y, val - 1, true);
		}
		else if (LOWORD(wParam) == IDC_OFFSETX_UP)
		{
			int val = GetDlgItemInt(hWnd, IDC_OFFSET_X, nullptr, true);
			SetDlgItemInt(hWnd, IDC_OFFSET_X, val + 1, true);
		}
		else if (LOWORD(wParam) == IDC_OFFSETX_DOWN)
		{
			int val = GetDlgItemInt(hWnd, IDC_OFFSET_X, nullptr, true);
			SetDlgItemInt(hWnd, IDC_OFFSET_X, val - 1, true);
		}
		else if (LOWORD(wParam) == IDC_OFFSETY_UP)
		{
			int val = GetDlgItemInt(hWnd, IDC_OFFSET_y, nullptr, true);
			SetDlgItemInt(hWnd, IDC_OFFSET_y, val + 1, true);
		}
		else if (LOWORD(wParam) == IDC_OFFSETY_DOWN)
		{
			int val = GetDlgItemInt(hWnd, IDC_OFFSET_y, nullptr, true);
			SetDlgItemInt(hWnd, IDC_OFFSET_y, val - 1, true);
		}
		else if (LOWORD(wParam) == IDC_INFO_APPLy)
		{
			tFrameInfo CurFrameInfo = pLevel->GetFrame(pAnimInfo.CurFrame);
			wchar_t str[20] = {};
			GetDlgItemText(hWnd, IDC_DURATION, str, 20);			
			CurFrameInfo.Duration = _wtof(str);
			GetDlgItemText(hWnd, IDC_OFFSET_X, str, 20);
			CurFrameInfo.Offset.x = _wtof(str);
			GetDlgItemText(hWnd, IDC_OFFSET_y, str, 20);
			CurFrameInfo.Offset.y = _wtof(str);
			GetDlgItemText(hWnd, IDC_SP_X, str, 20);
			CurFrameInfo.StartPosition.x = _wtof(str);
			GetDlgItemText(hWnd, IDC_SP_Y, str, 20);
			CurFrameInfo.StartPosition.y = _wtof(str);
			GetDlgItemText(hWnd, IDC_SS_X, str, 20);
			CurFrameInfo.SliceSize.x = _wtof(str);
			GetDlgItemText(hWnd, IDC_SS_Y, str, 20);
			CurFrameInfo.SliceSize.y = _wtof(str);
			pAnimInfo.MaxFrame = GetDlgItemInt(hWnd, IDC_MAX_FRAME, nullptr, false);

			//CurFrameInfo.StartPosition.x = GetDlgItemInt(hWnd, IDC_SP_X, nullptr, true);
			//CurFrameInfo.StartPosition.y = GetDlgItemInt(hWnd, IDC_SP_Y, nullptr, true);
			//CurFrameInfo.SliceSize.x = GetDlgItemInt(hWnd, IDC_SS_X, nullptr, true);
			//CurFrameInfo.SliceSize.y = GetDlgItemInt(hWnd, IDC_SS_Y, nullptr, true);
			//CurFrameInfo.Offset.x = GetDlgItemInt(hWnd, IDC_OFFSET_X, nullptr, true);
			//CurFrameInfo.Offset.y = GetDlgItemInt(hWnd, IDC_OFFSET_y, nullptr, true);
			

			pLevel->ApplyFrameInfo(pAnimInfo.CurFrame, CurFrameInfo);
			pLevel->SetCurPos(Vec2(CurFrameInfo.StartPosition.x, CurFrameInfo.StartPosition.y + 20));
			pLevel->SetFrameScale(Vec2(CurFrameInfo.SliceSize.x, CurFrameInfo.SliceSize.y));

			
			SendMessage(g_hModlessWnd, WM_PAINT, 0, 0);

			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDC_PLAY)
		{
			for (pAnimInfo.CurFrame = 0; pAnimInfo.CurFrame < pAnimInfo.MaxFrame; pAnimInfo.CurFrame++)
			{
				tFrameInfo CurFrameInfo = pLevel->GetFrame(pAnimInfo.CurFrame);
				SendMessage(g_hModlessWnd, WM_PAINT, 0, 0);
				SetDlgItemInt(hWnd, IDC_FRAME_INDEX, pAnimInfo.CurFrame, false);
				Sleep(CurFrameInfo.Duration * 1000);
			}
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDOK)
		{
			wchar_t str[64] = {};
			GetDlgItemText(hWnd, IDC_ANIMATION_NAME, str, 64);
 			pAnimInfo.AnimName = wstring(str);
			pLevel->SaveAnimation(pAnimInfo);
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hWnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
	

	CEngine::Get()->Progress();
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:

		return (INT_PTR)TRUE;
	case WM_PAINT:
	{
		//LOG(LOG_LEVEL::LOG, L"CALLED DlgProc WM_PAINT");
		InvalidateRect(hDlg, NULL, TRUE);

		CLevel_Anim_Editor* pLevel = dynamic_cast<CLevel_Anim_Editor*>(CLevelMgr::Get()->GetCurrentLevel());
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hDlg, &ps);

		SELECT_PEN(hdc, PEN_TYPE::GREEN);

		Vec2 vPos = {};
		Vec2 vScale = {};
		Vec2 vOffset = {};

		UINT width = 0;
		UINT height = 0;
		if (0 != pAnimInfo.MaxFrame)
		{
			tFrameInfo CurFrameInfo = pLevel->GetFrame(pAnimInfo.CurFrame);
			vPos = Vec2(CurFrameInfo.StartPosition.x, CurFrameInfo.StartPosition.y);
			vScale = Vec2(CurFrameInfo.SliceSize.x, CurFrameInfo.SliceSize.y);
			vOffset = Vec2(CurFrameInfo.Offset.x, CurFrameInfo.Offset.y);
			width = CurFrameInfo.SliceSize.x;
			height = CurFrameInfo.SliceSize.y;
		}
		CBackground* pAtlas = new CBackground;
		pAtlas->SetPos(Vec2(0.f, 0.f));
		//pAtlas->SetScale(Vec2(110.f, 110.f));
		pAtlas->SetAtlas(CAssetMgr::Get()->LoadTexture(pAnimInfo.TextureName, pAnimInfo.TexturePath));

		CTexture* m_Texture = pAtlas->GetAtlas();
		// 현재 작업중인 텍스쳐의 일부분 Render
		RECT rc = { 0, 0, width, height };

		BLENDFUNCTION blend = {};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;

		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
		
		SetWindowPos(hDlg, 0, 820, 50, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
	
		SELECT_BRUSH(hdc, BRUSH_TYPE::GRAY);

		AlphaBlend(hdc, 0 , 0 , width, height , m_Texture->GetDC(), vPos.x + vOffset.x, vPos.y + vOffset.y, width, height, blend);
		
		// 화면 중앙을 통과하는 십자 선 그리기
		MoveToEx(hdc, width / 2.f, 0, nullptr);
		LineTo(hdc, width / 2.f, height);

		MoveToEx(hdc, 0.f, height / 2.f, nullptr);
		LineTo(hdc, width, height / 2.f);
		
		delete pAtlas;

		EndPaint(hDlg, &ps);
	}
	return (INT_PTR)TRUE;
	case WM_CLOSE:
		//닫기 버튼을 누르면 WM_CLOSE 메시지가 발생되며
		//모달리스형 다이얼로그를 해지함. hDlg는 이 모달리스형의 윈도우 핸들
		
		//생성된 오브젝트 삭제
		
		DestroyWindow(hDlg);
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}