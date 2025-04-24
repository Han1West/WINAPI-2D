#include "pch.h"
#include "CLevel_Stage1.h"

#include "CEngine.h"
#include "CPlayer.h"
#include "CForce.h"
#include "CMonster.h"
#include "CPlatform.h"
#include "CTile.h"

#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CSound.h"

#include "CETCObj.h"
#include "CShotgun.h"

#include "CTileMap.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"
#include "CLevelMgr.h"

#include "CBackground.h"
#include "CBigSnake.h"
#include "CCaveman.h"
#include "CSkeleton.h"
#include "CSnake.h"
#include "CSpider.h"
#include "CDispenser.h"
#include "CSpike.h"
#include "CShopkeeper.h"
#include "CEffect.h"
#include "CVictim.h"
#include "CTextureUI.h"
#include "CTextUI.h"
#include "CBorder.h"

#include "CPassiveItem.h"

CLevel_Stage1::CLevel_Stage1()
	: m_TileMap(nullptr)
	, m_AccTime(0.f)
{

}

CLevel_Stage1::~CLevel_Stage1()
{
}


void CLevel_Stage1::AddObjectToStage()
{
	// 오브젝트 위치 설정
	CPlayer* pPlayer = new CPlayer;
	pPlayer->SetName(L"Player");
	Vec2 vResol = CEngine::Get()->GetResolution();
	pPlayer->SetPos(vResol.x / 2.f, vResol.y / 4.f);
	pPlayer->SetScale(80.f, 75.f);

	// 플레이어를 현재 레벨에 등록
	RegisterPlayer(pPlayer);

	// 플레이어를 레벨에 추가
	AddObject(pPlayer, LAYER_TYPE::PLAYER);

	// 오브젝트 생성
	CObj* pObj = nullptr;

	// Tile
	pObj = new CTile;
	pObj->SetName(L"Tile Object");
	pObj->SetPos(0.f, 0.f);

	SetTileMap(pObj->GetComponent<CTileMap>());

	m_TileMap = GetTileMap();
	m_TileMap->SetTileSize(Vec2(64.f, 64.f));
	m_TileMap->SetRowCol(1, 1);


	wstring strContentPath = CPathMgr::Get()->GetContentPath();

	wstring strTilePath = strContentPath + L"\\tile\\Stage1.tile";

	m_TileMap->Load(strTilePath);

	AddObject(pObj, LAYER_TYPE::PLATFORM);

	wstring strObjPath = strContentPath + L"\\obj\\Stage1.stg";
	ObjectLoad(strObjPath);
	multimap<wstring, Vec2> pObjmap = GetObjmap();
	multimap<wstring, Vec2>::iterator iter = pObjmap.begin();
	for (; iter != pObjmap.end(); ++iter)
	{
		LoadObjectToStage(iter->first, iter->second);
	}

	CBackground* pBackGround = new CBackground;
	pBackGround->SetName(L"BackGround");
	pBackGround->SetAtlas(CAssetMgr::Get()->LoadTexture(L"Cave", L"texture\\Cave.png"));
	pBackGround->SetPos(Vec2(0.f, 0.f));

	AddObject(pBackGround, LAYER_TYPE::BACKGROUND);

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 14; ++j)
		{
			CBackground* pCloneBackGround = pBackGround->Clone();
			pCloneBackGround->SetName(L"BackGround");
			pCloneBackGround->SetPos(Vec2(256.f * i + 1, 158.f * j + 1));

			AddObject(pCloneBackGround, LAYER_TYPE::BACKGROUND);
		}
	}

	CBorder* pBorder = new CBorder;
	pBorder->SetName(L"TopWall");
	pBorder->SetPos(Vec2(1280.f, -250.f));
	pBorder->SetTexture(CAssetMgr::Get()->LoadTexture(L"TopWall", L"texture\\topwall.png"));
	AddObject(pBorder, LAYER_TYPE::BORDER);

	CBorder* pCloneBorder = pBorder->Clone();
	pCloneBorder->SetName(L"LeftWall");
	pCloneBorder->SetPos(Vec2(-260.f, 300.f));
	pCloneBorder->SetTexture(CAssetMgr::Get()->LoadTexture(L"LeftWall", L"texture\\leftwall.png"));
	AddObject(pCloneBorder, LAYER_TYPE::BORDER);

	pCloneBorder = pBorder->Clone();
	pCloneBorder->SetName(L"LeftWall");
	pCloneBorder->SetPos(Vec2(-260.f, 1600.f));
	pCloneBorder->SetTexture(CAssetMgr::Get()->LoadTexture(L"LeftWall", L"texture\\leftwall.png"));
	AddObject(pCloneBorder, LAYER_TYPE::BORDER);

	pCloneBorder = pBorder->Clone();
	pCloneBorder->SetName(L"RightWall");
	pCloneBorder->SetPos(Vec2(2820.f, 300.f));
	pCloneBorder->SetTexture(CAssetMgr::Get()->LoadTexture(L"RightWall", L"texture\\rightwall.png"));
	AddObject(pCloneBorder, LAYER_TYPE::BORDER);

	pCloneBorder = pBorder->Clone();
	pCloneBorder->SetName(L"RightWall");
	pCloneBorder->SetPos(Vec2(2820.f, 1600.f));
	pCloneBorder->SetTexture(CAssetMgr::Get()->LoadTexture(L"RightWall", L"texture\\rightwall.png"));
	AddObject(pCloneBorder, LAYER_TYPE::BORDER);

	pCloneBorder = pBorder->Clone();
	pCloneBorder->SetName(L"BottomWall");
	pCloneBorder->SetPos(Vec2(1280.f, 2360.f));
	pCloneBorder->SetTexture(CAssetMgr::Get()->LoadTexture(L"BottomWall", L"texture\\bottomwall.png"));
	AddObject(pCloneBorder, LAYER_TYPE::BORDER);


	//for (int i = 1; i < 4; ++i)
	//{
	//	for (int j = 1; j < 3; ++j)
	//	{
	//		CBorder* pCloneBorder = pBorder->Clone();
	//		pCloneBorder->SetName(L"TopWall");
	//		pCloneBorder->SetPos(Vec2(900.f * i, -90.f * j));
	//		pCloneBorder->SetTexture(CAssetMgr::Get()->LoadTexture(L"TopWall", L"texture\\topwall.png"));
	//		AddObject(pCloneBorder, LAYER_TYPE::PLATFORM);
	//	}
	//}

	//for (int i = 0; i < 4; ++i)
	//{
	//	for (int j = 0; j < 3; ++j)
	//	{
	//		CBorder* pCloneBorder = pBorder->Clone();
	//		pCloneBorder->SetName(L"BottomWall");
	//		pCloneBorder->SetPos(Vec2(400.f + (800.f * i), 2200.f + (180.f * j)));
	//		pCloneBorder->SetTexture(CAssetMgr::Get()->LoadTexture(L"BottomWall", L"texture\\bottomwall.png"));
	//		AddObject(pCloneBorder, LAYER_TYPE::PLATFORM);
	//	}
	//}

	//for (int i = 0; i < 3; ++i)
	//{
	//	for (int j = 0; j < 6; ++j)
	//	{
	//		CBorder* pCloneBorder = pBorder->Clone();
	//		pCloneBorder->SetName(L"RightWall");
	//		pCloneBorder->SetPos(Vec2(2640.f + (180.f * i), 0.f + (500.f * j)));
	//		pCloneBorder->SetTexture(CAssetMgr::Get()->LoadTexture(L"RightWall", L"texture\\rightwall.png"));
	//		AddObject(pCloneBorder, LAYER_TYPE::PLATFORM);
	//	}
	//}

	//for (int i = 0; i < 3; ++i)
	//{
	//	for (int j = 0; j < 6; ++j)
	//	{
	//		CBorder* pCloneBorder = pBorder->Clone();
	//		pCloneBorder->SetName(L"LeftWall");
	//		pCloneBorder->SetPos(Vec2(-90.f + (-180.f * i), 0.f + (500.f * j)));
	//		pCloneBorder->SetTexture(CAssetMgr::Get()->LoadTexture(L"LeftWall", L"texture\\leftwall.png"));
	//		AddObject(pCloneBorder, LAYER_TYPE::PLATFORM);
	//	}
	//}

	



	// 기본 Texture UI
	CTextureUI* pUI = new CTextureUI;
	pUI->SetName(L"HPUI");
	pUI->SetPos(Vec2(100.f, 70.f));
	pUI->SetTexture(CAssetMgr::Get()->LoadTexture(L"HPUI", L"texture\\HPUI.png"));
	AddObject(pUI, LAYER_TYPE::UI);

	CTextureUI* pCloneUI = pUI->Clone();
	pCloneUI->SetName(L"PageUI");
	pCloneUI->SetPos(Vec2(260.f, 75.f));
	pCloneUI->SetTexture(CAssetMgr::Get()->LoadTexture(L"PageUI", L"texture\\PageUI.png"));
	AddObject(pCloneUI, LAYER_TYPE::UI);

	pCloneUI = pUI->Clone();
	pCloneUI->SetName(L"BombUI");
	pCloneUI->SetPos(Vec2(180.f, 72.f));
	pCloneUI->SetTexture(CAssetMgr::Get()->LoadTexture(L"BombUI", L"texture\\BombUI.png"));
	AddObject(pCloneUI, LAYER_TYPE::UI);

	pCloneUI = pUI->Clone();
	pCloneUI->SetName(L"RopeUI");
	pCloneUI->SetPos(Vec2(275.f, 70.f));
	pCloneUI->SetTexture(CAssetMgr::Get()->LoadTexture(L"RopeUI", L"texture\\RopeUI.png"));
	AddObject(pCloneUI, LAYER_TYPE::UI);

	pCloneUI = pUI->Clone();
	pCloneUI->SetName(L"MoneyUI");
	pCloneUI->SetPos(Vec2(200.f, 160.f));
	pCloneUI->SetTexture(CAssetMgr::Get()->LoadTexture(L"MoneyUI", L"texture\\MoneyUI.png"));
	AddObject(pCloneUI, LAYER_TYPE::UI);

	m_StageUI = pUI->Clone();
	m_StageUI->SetName(L"Cave1UI");
	m_StageUI->SetPos(Vec2(1040, 100.f));
	m_StageUI->SetTexture(CAssetMgr::Get()->LoadTexture(L"Cave1UI", L"texture\\Cave1UI.png"));
	AddObject(m_StageUI, LAYER_TYPE::UI);


	// 기본 Text UI

	m_TextUI[0] = new CTextUI;
	m_TextUI[0]->SetName(L"HPText");
	m_TextUI[0]->SetPos(Vec2(82.f, 45.f));
	m_TextUI[0]->SetString(std::to_wstring(pPlayer->GetCurrentHP()));
	m_TextUI[0]->SetScale(50);
	AddObject(m_TextUI[0], LAYER_TYPE::UI);

	m_TextUI[1] = m_TextUI[0]->Clone();
	m_TextUI[1]->SetName(L"BombText");
	m_TextUI[1]->SetPos(Vec2(200.f, 55.f));
	m_TextUI[1]->SetString(std::to_wstring(pPlayer->GetCurrentBomb()));
	m_TextUI[1]->SetScale(30);
	AddObject(m_TextUI[1], LAYER_TYPE::UI);

	m_TextUI[2] = m_TextUI[0]->Clone();
	m_TextUI[2]->SetName(L"RopeText");
	m_TextUI[2]->SetPos(Vec2(305.f, 55.f));
	m_TextUI[2]->SetString(std::to_wstring(pPlayer->GetCurrentRope()));
	m_TextUI[2]->SetScale(30);
	AddObject(m_TextUI[2], LAYER_TYPE::UI);
	
	m_TextUI[3] = m_TextUI[0]->Clone();
	m_TextUI[3]->SetName(L"GoldText");
	m_TextUI[3]->SetPos(Vec2(180.f, 140.f));
	m_TextUI[3]->SetString(std::to_wstring(pPlayer->GetCurrentGold()));
	m_TextUI[3]->SetScale(30);
	AddObject(m_TextUI[3], LAYER_TYPE::UI);


	//// Snake

	//CSnake* pSnake = new CSnake;
	//pSnake->SetName(L"Snake");
	//pSnake->SetPos(vResol.x / 2.f - 200.f, vResol.y / 4.f + 20.f);
	//AddObject(pSnake, LAYER_TYPE::MONSTER);


	//// Caveman

	//CCaveman* pCaveman = new CCaveman;
	//pCaveman->SetName(L"Caveman");
	//pCaveman->SetPos(vResol.x / 2.f - 200.f, vResol.y / 4.f + 20.f);
	//AddObject(pCaveman, LAYER_TYPE::MONSTER);

	//// Spider

	//CSpider* pSpider = new CSpider;
	//pSpider->SetName(L"Spider");
	//pSpider->SetPos(vResol.x / 2.f - 500.f, vResol.y / 2.f - 40.f);
	//AddObject(pSpider, LAYER_TYPE::MONSTER);


	//// Skeleton
	//CPassiveItem* pPassiveItem = new CPassiveItem;
	//pPassiveItem->SetName(L"SpringShoe");
	//pPassiveItem->SetPos(vResol.x / 2.f - 200.f, vResol.y / 4.f + 20.f);
	//pPassiveItem->SetTexture(CAssetMgr::Get()->LoadTexture(L"SpringShoe", L"texture\\springshoe.png"));
	//AddObject(pPassiveItem, LAYER_TYPE::PASSSIVEITEM);

	//pPassiveItem = new CPassiveItem;
	//pPassiveItem->SetName(L"SpikeShoe");
	//pPassiveItem->SetPos(vResol.x / 2.f - 300.f, vResol.y / 4.f + 20.f);
	//pPassiveItem->SetTexture(CAssetMgr::Get()->LoadTexture(L"SpikeShoe", L"texture\\spikeshoe.png"));

	//AddObject(pPassiveItem, LAYER_TYPE::PASSSIVEITEM);
}

void CLevel_Stage1::Init()
{
	CSound* pBGM = CAssetMgr::Get()->LoadSound(L"MineA", L"sound\\MineA.wav");
	pBGM->SetVolume(30);
	pBGM->PlayToBGM();
	SetBGM(pBGM);

	AddObjectToStage();




	// 충돌 지정
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_PROJECTILE, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::ACTIVEITEM, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PASSSIVEITEM, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::MONSTER, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::MONSTER, (UINT)LAYER_TYPE::ACTIVEITEM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::MONSTER, (UINT)LAYER_TYPE::PLAYER_PROJECTILE, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::HOSTILEPLATFORM, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::EFFECT, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::LADDER, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::ACTIVEITEM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::PASSSIVEITEM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::ETRANCE, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::MONSTER, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::HOSTILEPLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::MONSTER_PROJECTILE, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::MONSTER, (UINT)LAYER_TYPE::ETRANCE, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::MONSTER, (UINT)LAYER_TYPE::PASSSIVEITEM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER_PROJECTILE, (UINT)LAYER_TYPE::PASSSIVEITEM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::HOSTILEPLATFORM, (UINT)LAYER_TYPE::MONSTER, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::HOSTILEPLATFORM, (UINT)LAYER_TYPE::ACTIVEITEM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::MONSTER_PROJECTILE, (UINT)LAYER_TYPE::PLATFORM, true);
	CCollisionMgr::Get()->CollisionCheck((UINT)LAYER_TYPE::PLAYER, (UINT)LAYER_TYPE::BORDER, true);
}

void CLevel_Stage1::Exit()
{
	// 모든 오브젝트를 삭제해둔다.
	UpdatePlayTime(m_PlayTime);
	DeleteAllObjects();
}

void CLevel_Stage1::Tick()
{
	CLevel::Tick();

	m_PlayTime += DT;
	m_AccTime += DT;
	if (2.f < m_AccTime)
	{
		DeleteObject(m_StageUI);
	}

	if (IsChange())
	{
		for (int i = 0; i < sizeof(m_TextUI) / sizeof(CTextUI*); ++i)
			DeleteObject(m_TextUI[i]);
		

		ChangePlayerInfo(m_TextUI);
		CheckItemUI();
		SetChange(false);
	}
	

	//Vec2 pLookAt = CCamera::Get()->GetLookAt();
	//float pCamSpeed = CCamera::Get()->GetCamSpeed();


	//CCamera::Get()->SetLookAt(pLookAt);
}

