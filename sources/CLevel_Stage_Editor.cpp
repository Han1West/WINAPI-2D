#include "pch.h"
#include "CLevel_Stage_Editor.h"
#include "resource.h"

#include "CLevel.h"
#include "CLevelMgr.h"
#include "CAssetMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CPathMgr.h"


#include "CTile.h"
#include "CTileMap.h"
#include "CEditorBox.h"
#include "CCamera.h"

#include "CEngine.h"
#include "CTexture.h"
#include "CCollisionMgr.h"

#include "CBackground.h"
#include "CBigSnake.h"
#include "CCaveman.h"
#include "CSkeleton.h"
#include "CSnake.h"
#include "CSpider.h"
#include "CDispenser.h"
#include "CSpike.h"
#include "CShopkeeper.h"
#include "CVictim.h"
#include "CETCObj.h"
#include "CPassiveItem.h"


CLevel_Stage_Editor::CLevel_Stage_Editor()
	: m_hMenu(nullptr)
	, m_TileMap(nullptr)
	, m_hdlg(nullptr)
	, m_Box(nullptr)
	, m_ButtonHwnd(nullptr)
	, m_vecAddedObj{}
{
	m_hMenu = LoadMenu(nullptr, MAKEINTRESOURCE(IDC_STAGE_MENU));
}

CLevel_Stage_Editor::~CLevel_Stage_Editor()
{
	DestroyMenu(m_hMenu);
}


void CLevel_Stage_Editor::Init()
{
	// 메뉴 추가	
	SetMenu(CEngine::Get()->GetMainHwnd(), m_hMenu);

	// 메뉴가 붙었으니까, 다시 윈도우 크기 재계산
	UINT width = CEngine::Get()->GetResolution().x;
	UINT height = CEngine::Get()->GetResolution().y;
	CEngine::Get()->ChangeWindowResolution(width, height);

	// 오브젝트 생성
	CObj* pObj = nullptr;

	// Tile
	pObj = new CTile;
	pObj->SetName(L"Tile Object");
	pObj->SetPos(0.f, 0.f);

	m_TileMap = pObj->GetComponent<CTileMap>();

	//m_TileMap->SetAtlas(CAssetMgr::Get()->LoadTexture(L"TileAtlasTex", L"texture\\Tileset_STAGE1.png"));
	//m_TileMap->SetAtlasTileSize(Vec2(64.f, 64.f));

	m_TileMap->SetTileSize(Vec2(64.f, 64.f));
	m_TileMap->SetRowCol(1, 1);
	AddObject(pObj, LAYER_TYPE::PLATFORM);

	//// 에디터 박스 생성
	m_Box = new CEditorBox;
	m_Box->SetRowCol(1, 1);
	m_Box->SetSize(Vec2(64.f, 64.f));

	AddObject(m_Box, LAYER_TYPE::EIDTOR_RENDER);

	CBackground* pBackGround = new CBackground;
	pBackGround->SetName(L"BackGround");
	pBackGround->SetAtlas(CAssetMgr::Get()->LoadTexture(L"Cave", L"texture\\Cave.png"));
	pBackGround->SetPos(Vec2(0.f, 0.f));

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 14; ++j)
		{
			CBackground* pCloneBackGround = pBackGround->Clone();
			pCloneBackGround->SetPos(Vec2(256.f * i+1, 158.f * j+1));

			AddObject(pCloneBackGround, LAYER_TYPE::BACKGROUND);
		}
	}
		



	AddObject(pBackGround, LAYER_TYPE::BACKGROUND);


	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::ACTIVEITEM, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PASSSIVEITEM, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::MONSTER, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::HOSTILEPLATFORM, (UINT)LAYER_TYPE::PLATFORM, true);

}

void CLevel_Stage_Editor::Tick()
{
	CLevel::Tick();
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

	MouseCheck();
}


void CLevel_Stage_Editor::MouseCheck()
{
	Vec2 vMousePos = CKeyMgr::Get()->GetMousePos();
	vMousePos = CCamera::Get()->GetRealPos(vMousePos);
	int pRow = m_TileMap->GetRow();
	int pCol = m_TileMap->GetCol();
	m_vecAddedObj = GetObjectVector();
	
	if (KEY_PRESSED(KEY::LBTN))
	{
		Vec2 vMousePos = CKeyMgr::Get()->GetMousePos();
		vMousePos = CCamera::Get()->GetRealPos(vMousePos);

		for (int i = 0; i < m_vecAddedObj.size(); ++i)
		{
			Vec2 vPos = m_vecAddedObj[i]->GetPos();
			Vec2 vScale = m_vecAddedObj[i]->GetScale();
			if (vMousePos.x < vPos.x + vScale.x && vPos.x < vMousePos.x &&
				vMousePos.y < vPos.y + vScale.y && vPos.y < vMousePos.y)
			{
				m_vecAddedObj[i]->SetPos(Vec2(vMousePos.x - vScale.x/2.f, vMousePos.y - vScale.y/2.f));
			}
		}
		Vec2 TileSize = m_TileMap->GetTileSize();

		UINT Col = vMousePos.y / TileSize.y;
		UINT Row = vMousePos.x / TileSize.x;

		if (Col < pRow && Row < pCol)
		{
			tTileInfo* pTileInfo = m_TileMap->GetTileInfo(Col, Row);
			m_Box->PushSelectedIndex(Vec2(Col, Row));
			//IndexPush(Vec2(Col, Row));
		}
	}
	else if (KEY_PRESSED(KEY::RBTN))
	{
		m_Box->ClearIndex();
	}
}

void CLevel_Stage_Editor::ObjectRemove()
{
	m_vecAddedObj = GetObjectVector();
	int LatestObj = m_vecAddedObj.size() - 1;
	DeleteObject(m_vecAddedObj[LatestObj]);
	m_vecAddedObj.resize(LatestObj);
}

void CLevel_Stage_Editor::SaveTile()
{
	wstring strContentPath = CPathMgr::Get()->GetContentPath();

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Tile\0*.tile\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetSaveFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			m_TileMap->Save(szFilePath);
		}
	}
	m_TileMap->SetChange(true);
}

void CLevel_Stage_Editor::LoadTile()
{
	wstring strContentPath = CPathMgr::Get()->GetContentPath();

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Tile\0*.tile\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetOpenFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			m_TileMap->Load(szFilePath);
		}
	}
	int pRow = m_TileMap->GetRow();
	int pCol = m_TileMap->GetCol();
	m_Box->SetRowCol(pRow, pCol);
	m_TileMap->SetChange(true);
}



void CLevel_Stage_Editor::Exit()
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


wchar_t* CLevel_Stage_Editor::LoadTextureName()
{
	wstring strContentPath = CPathMgr::Get()->GetContentPath();
	wstring* ptr = nullptr;

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

	return Name;
}

void CLevel_Stage_Editor::LoadObject(wstring _str, Vec2 _Pos)
{
	InsertObj(_str, _Pos);

	//if (_str == L"CBigSnake")
	//{
	//	CBigSnake* pObj = new CBigSnake;
	//	pObj->SetName(L"BigSnake");
	//	pObj->SetPos(Vec2(250.f, -250.f));
	//	pObj->SetScale(Vec2(50.f, 50.f));
	//	pObj->SetLayerType((int)LAYER_TYPE::MONSTER);

	//	AddObject(pObj, LAYER_TYPE::MONSTER);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CCaveman")
	//{
	//	CCaveman* pObj = new CCaveman;
	//	pObj->SetName(L"Caveman");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(50.f, 50.f));
	//	pObj->SetLayerType((int)LAYER_TYPE::MONSTER);

	//	AddObject(pObj, LAYER_TYPE::MONSTER);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CSkeleton")
	//{
	//	CSkeleton* pObj = new CSkeleton;
	//	pObj->SetName(L"Skeleton");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(50.f, 50.f));
	//	pObj->SetLayerType((int)LAYER_TYPE::MONSTER);

	//	AddObject(pObj, LAYER_TYPE::MONSTER);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CSnake")
	//{
	//	CSnake* pObj = new CSnake;
	//	pObj->SetName(L"Snake");
	//	pObj->SetPos(Vec2(1000.f, 0.f));
	//	pObj->SetScale(Vec2(50.f, 50.f));
	//	pObj->SetLayerType((int)LAYER_TYPE::MONSTER);

	//	AddObject(pObj, LAYER_TYPE::MONSTER);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CSpider")
	//{
	//	CSpider* pObj = new CSpider;
	//	pObj->SetName(L"Spider");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(50.f, 50.f));
	//	pObj->SetLayerType((int)LAYER_TYPE::MONSTER);

	//	AddObject(pObj, LAYER_TYPE::MONSTER);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CDispenserR")
	//{
	//	CDispenser* pObj = new CDispenser;
	//	pObj->SetName(L"DispenserR");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetIsRight(true);
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"DispenserR", L"texture\\Dispenser.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::HOSTILEPLATFORM);

	//	AddObject(pObj, LAYER_TYPE::HOSTILEPLATFORM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CDispenserL")
	//{
	//	CDispenser* pObj = new CDispenser;
	//	pObj->SetName(L"DispenserL");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetIsRight(false);
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"DispenserL", L"texture\\Dispenser_left.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::HOSTILEPLATFORM);

	//	AddObject(pObj, LAYER_TYPE::HOSTILEPLATFORM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CSpike")
	//{
	//	CSpike* pObj = new CSpike;
	//	pObj->SetName(L"Spike");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Spike", L"texture\\Spike.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::HOSTILEPLATFORM);

	//	AddObject(pObj, LAYER_TYPE::HOSTILEPLATFORM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CSpikeShoe")
	//{
	//	CPassiveItem* pObj = new CPassiveItem;
	//	pObj->SetName(L"SpikeShoe");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"SpikeShoe", L"texture\\spikeshoe.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::PASSSIVEITEM);

	//	AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CSpringShoe")
	//{
	//	CPassiveItem* pObj = new CPassiveItem;
	//	pObj->SetName(L"SpringShoe");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"SpringShoe", L"texture\\springshoe.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::PASSSIVEITEM);

	//	AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CBombBox")
	//{
	//	CPassiveItem* pObj = new CPassiveItem;
	//	pObj->SetName(L"BombBox");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"BombBox", L"texture\\bomb_box.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::PASSSIVEITEM);

	//	AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CGold")
	//{
	//	CPassiveItem* pObj = new CPassiveItem;
	//	pObj->SetName(L"Gold");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Gold", L"texture\\gold.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::PASSSIVEITEM);

	//	AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CRuby")
	//{
	//	CPassiveItem* pObj = new CPassiveItem;
	//	pObj->SetName(L"Ruby");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Ruby", L"texture\\ruby.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::PASSSIVEITEM);

	//	AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CEmerald")
	//{
	//	CPassiveItem* pObj = new CPassiveItem;
	//	pObj->SetName(L"Emerald");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Emerald", L"texture\\emerald.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::PASSSIVEITEM);

	//	AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CRopePackage")
	//{
	//	CPassiveItem* pObj = new CPassiveItem;
	//	pObj->SetName(L"RopePackage");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"RopePackage", L"texture\\rope_package.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::PASSSIVEITEM);

	//	AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CGoldPackage")
	//{
	//	CPassiveItem* pObj = new CPassiveItem;
	//	pObj->SetName(L"GoldPackage");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"GoldPackage", L"texture\\gold_package.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::PASSSIVEITEM);

	//	AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CSapphire")
	//{
	//	CPassiveItem* pObj = new CPassiveItem;
	//	pObj->SetName(L"Sapphire");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Sapphire", L"texture\\sapphire.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::PASSSIVEITEM);

	//	AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CGlove")
	//{
	//	CPassiveItem* pObj = new CPassiveItem;
	//	pObj->SetName(L"Glove");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Glove", L"texture\\glove.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::PASSSIVEITEM);

	//	AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CRock")
	//{
	//	CETCObj* pObj = new CETCObj;
	//	pObj->SetName(L"Rock");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Rock", L"texture\\Rock.png"));
	//	pObj->SetLayerType((int)LAYER_TYPE::ACTIVEITEM);

	//	AddObject(pObj, LAYER_TYPE::ACTIVEITEM);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CShopkeeper")
	//{
	//	CShopkeeper* pObj = new CShopkeeper;
	//	pObj->SetName(L"Shopkeeper");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetLayerType((int)LAYER_TYPE::MONSTER);

	//	AddObject(pObj, LAYER_TYPE::MONSTER);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//}
	//else if (_str == L"CVictim")
	//{
	//	CVictim* pObj = new CVictim;
	//	pObj->SetName(L"Shopkeeper");
	//	pObj->SetPos(Vec2(0.f, 0.f));
	//	pObj->SetScale(Vec2(64.f, 64.f));
	//	pObj->SetLayerType((int)LAYER_TYPE::MONSTER);

	//	AddObject(pObj, LAYER_TYPE::MONSTER);
	//	PushObject(pObj);
	//	//m_vecAddedObj.push_back(pObj);
	//	}
}

void CLevel_Stage_Editor::SaveObj()
{
	wstring strContentPath = CPathMgr::Get()->GetContentPath();

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Stage\0*.stg\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetSaveFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			//m_TileMap->Save(szFilePath);
			ObjectSave(szFilePath);
		}
	}
	m_TileMap->SetChange(true);
}

void CLevel_Stage_Editor::LoadStage()
{
	wstring strContentPath = CPathMgr::Get()->GetContentPath();

	// 파일 경로 문자열
	wchar_t szFilePath[255] = {};

	OPENFILENAME Desc = {};

	Desc.lStructSize = sizeof(OPENFILENAME);
	Desc.hwndOwner = nullptr;
	Desc.lpstrFile = szFilePath;
	Desc.nMaxFile = 255;
	Desc.lpstrFilter = L"Stage\0*.stg\0ALL\0*.*";
	Desc.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	Desc.lpstrInitialDir = strContentPath.c_str();

	if (GetOpenFileName(&Desc))
	{
		if (wcslen(szFilePath) != 0)
		{
			//m_TileMap->Load(szFilePath);
			ObjectLoad(szFilePath);
		}
	}
	m_vecAddedObj = GetObjectVector();

	for (int i = 0; i < m_vecAddedObj.size(); ++i)
	{
		AddObject(m_vecAddedObj[i], (LAYER_TYPE)m_vecAddedObj[i]->GetLayerType());
	}

	int pRow = m_TileMap->GetRow();
	int pCol = m_TileMap->GetCol();
	m_Box->SetRowCol(pRow, pCol);
	m_TileMap->SetChange(true);
}

INT_PTR CALLBACK StageProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{ 
	CLevel_Stage_Editor* pLevel = dynamic_cast<CLevel_Stage_Editor*>(CLevelMgr::Get()->GetCurrentLevel());
	switch (message)
	{
	case WM_INITDIALOG:

		return (INT_PTR)TRUE;
	
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_TILE_APPLY)
		{
			int Col = GetDlgItemInt(hWnd, IDC_TILE_COL, nullptr, true);
			int Row = GetDlgItemInt(hWnd, IDC_TILE_ROW, nullptr, true);

			if (Col == 0 || Row == 0)
			{
				MessageBox(nullptr, L"타일의 행 or 열은 0 이 될 수 없습니다.", L"타일 행, 렬 설정 오류", MB_OK);
				return (INT_PTR)TRUE;
			}
			
			
			assert(pLevel);

			CTileMap* pTileMap = pLevel->GetTileMap();
			pTileMap->SetRowCol(Row, Col);
			CEditorBox* pBox = pLevel->GetEidtorBox();
			pBox->SetRowCol(Row, Col);
			pTileMap->SetChange(true);
		}
		else if (LOWORD(wParam) == IDC_TEXTURE_LOAD)
		{
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

			HWND hwndList = GetDlgItem(hWnd, IDC_TEXTURE_LIST);
			SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)Name);
		}
		else if (LOWORD(wParam) == IDC_TEXTURE_REMOVE)
		{
			HWND hWndList = GetDlgItem(hWnd, IDC_TEXTURE_LIST);

			int lbItem = (int)SendMessage(hWndList, LB_GETCURSEL, 0, 0);
			SendMessage(hWndList, LB_DELETESTRING, lbItem, 0);

		}
		else if (LOWORD(wParam) == IDC_TEXTURE_APPLY)
		{
			HWND hWndList = GetDlgItem(hWnd, IDC_TEXTURE_LIST);
			wchar_t str[20] = {};
			int lbItem = (int)SendMessage(hWndList, LB_GETCURSEL, 0, 0);
			SendMessage(hWndList, LB_GETTEXT, lbItem, (LPARAM) str);
			wstring TextureName = wstring(str);
			wstring TexturePath = L"texture\\" + TextureName + L".png";
			
			HWND hWndDC = GetDlgItem(hWnd, IDC_CURRENT_TEXTURE);
			HDC hDC = GetDC(hWndDC);

			// 오브젝트 생성
			CObj* pObj = nullptr;

			// Tile
			pObj = new CTile;
			pObj->SetName(L"Tile Object");
			pObj->SetPos(0.f, 0.f);

			CTileMap* m_TileMap = pObj->GetComponent<CTileMap>();

			m_TileMap->SetAtlas(CAssetMgr::Get()->LoadTexture(L"CurrentTile", TexturePath.c_str()));
			m_TileMap->SetAtlasTileSize(Vec2(63.375f, 63.75f));

			m_TileMap->SetTileSize(Vec2(63.375f, 63.75f));
			m_TileMap->SetRowCol(1, 1);
			SELECT_BRUSH(hDC, BRUSH_TYPE::GRAY);
			Rectangle(hDC, 0, 0, 64, 64);

 			tTileInfo* pTileInfo = m_TileMap->GetTileInfo(0, 0);

			pTileInfo->ImgIdx = GetDlgItemInt(hWnd, IDC_CURRENT_INDEX, nullptr, false);
			
			m_TileMap->Render_Tile(hDC, hWndDC);

		}
		else if (LOWORD(wParam) == IDC_PREV_INDEX)
		{
			int Idx = GetDlgItemInt(hWnd, IDC_CURRENT_INDEX, nullptr, false);
			if (0 == Idx)
				Idx = 64;
			else
				--Idx;
			SetDlgItemInt(hWnd, IDC_CURRENT_INDEX, Idx, false);
		}
		else if (LOWORD(wParam) == IDC_NEXT_INDEX)
		{
			int Idx = GetDlgItemInt(hWnd, IDC_CURRENT_INDEX, nullptr, false);
			if (64 == Idx)
				Idx = 0;
			else
				++Idx;
			SetDlgItemInt(hWnd, IDC_CURRENT_INDEX, Idx, false);
		}
		else if (LOWORD(wParam) == IDC_TEXTURE_ADD)
		{
			HWND hWndList = GetDlgItem(hWnd, IDC_TEXTURE_LIST);
			wchar_t str[20] = {};
			int lbItem = (int)SendMessage(hWndList, LB_GETCURSEL, 0, 0);
			SendMessage(hWndList, LB_GETTEXT, lbItem, (LPARAM)str);
			wstring TextureName = wstring(str);
			wstring TexturePath = L"texture\\" + TextureName + L".png";

			CTileMap* pTileMap = pLevel->GetTileMap();
			pTileMap->SetAtlas(CAssetMgr::Get()->LoadTexture(L"TileMap", TexturePath.c_str()));
			pTileMap->SetAtlasTileSize(Vec2(64.f, 64.f));
			pTileMap->SetTileSize(Vec2(64.f, 64.f));
			int TextureIdx = GetDlgItemInt(hWnd, IDC_CURRENT_INDEX, nullptr, false);
			
			CEditorBox* pBox = pLevel->GetEidtorBox();

			for (int i = 0; i < pBox->GetVectorSize(); ++i)
			{
				Vec2 pSeletedBoxIndex = pBox->GetSelectedIndex(i);
				tTileInfo* pTileInfo = pTileMap->GetTileInfo(pSeletedBoxIndex.y,pSeletedBoxIndex.x);
				pTileInfo->ImgIdx = TextureIdx;
			}

			pTileMap->SetChange(true);
		}
		else if (LOWORD(wParam) == IDC_TEXTURE_VACATE)
		{
			CTileMap* pTileMap = pLevel->GetTileMap();
			int TextureIdx = GetDlgItemInt(hWnd, IDC_CURRENT_INDEX, nullptr, false);
			CEditorBox* pBox = pLevel->GetEidtorBox();
			for (int i = 0; i < pBox->GetVectorSize(); ++i)
			{
				Vec2 pSeletedBoxIndex = pBox->GetSelectedIndex(i);
				tTileInfo* pTileInfo = pTileMap->GetTileInfo(pSeletedBoxIndex.y, pSeletedBoxIndex.x);
				pTileInfo->ImgIdx = -1;
			}
			pTileMap->SetChange(true);
		}
		else if (LOWORD(wParam) == IDC_LIST_LOAD)
		{
			HWND hWndCombo = GetDlgItem(hWnd, IDC_MONSTER_LIST);
			TCHAR listItem[][30] = { L"BigSnake", L"CavemanR", L"Skeleton", L"Snake"
			, L"Spider", L"DispenserR", L"DispenserL", L"Spike", L"ShopkeeperR", L"ShopkeeperL"
			, L"VictimR", L"VictimL", L"Rock", L"CavemanL"
			, L"Gold" , L"Ruby",  L"SpikeShoe",  L"BombSet",  L"Emerald",  L"GoldPackage" 
			, L"RopePackage", L"Sapphire", L"Glove", L"SpringShoe", L"Exit", L"BombBox"
			, L"RopeBox" , L"ShotgunBox" , L"BombPackage"};
			for (int i = 0; i < sizeof(listItem) / sizeof(listItem[0]); ++i)
				SendMessage(hWndCombo, LB_ADDSTRING, 0, (LPARAM)listItem[i]);
		}
		else if (LOWORD(wParam) == IDC_MONSTER_ADD)
		{
			CTileMap* pTileMap = pLevel->GetTileMap();
			Vec2 vTileSize = pTileMap->GetTileSize();
			Vec2 vPos = {};
			wchar_t str[20] = {};
			int TextureIdx = GetDlgItemInt(hWnd, IDC_CURRENT_INDEX, nullptr, false);
			CEditorBox* pBox = pLevel->GetEidtorBox();
			for (int i = 0; i < pBox->GetVectorSize(); ++i)
			{
				Vec2 pSeletedBoxIndex = pBox->GetSelectedIndex(i);
				vPos = Vec2(pSeletedBoxIndex.y * vTileSize.x + 32.f,
					pSeletedBoxIndex.x * vTileSize.y + 32.f);
				HWND hWndList = GetDlgItem(hWnd, IDC_MONSTER_LIST);
				int lbItem = (int)SendMessage(hWndList, LB_GETCURSEL, 0, 0);
				SendMessage(hWndList, LB_GETTEXT, lbItem, (LPARAM)str);
				wstring TextureName = wstring(str);
			}
			pLevel->LoadObject(str, vPos);
		}
		else if (LOWORD(wParam) == IDC_MONSTER_REMOVE)
		{
			pLevel->ObjectRemove();
		}
		else if (LOWORD(wParam) == IDOK)
		{
			pLevel->SaveTile();
		}
		return (INT_PTR)TRUE;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return (INT_PTR)TRUE;
	}

	return (INT_PTR)FALSE;
}


