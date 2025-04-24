#include "pch.h"
#include "CLevel_BossStage.h"

#include "CEngine.h"
#include "CPlayer.h"
#include "CForce.h"
#include "CMonster.h"
#include "CPlatform.h"
#include "CTile.h"

#include "CAssetMgr.h"
#include "CTimeMgr.h"
#include "CSound.h"
#include "CCollider.h"

#include "CETCObj.h"
#include "CShotgun.h"

#include "CTileMap.h"
#include "CCollisionMgr.h"
#include "CKeyMgr.h"
#include "CPathMgr.h"
#include "CStateMachine.h"

#include "CBackground.h"
#include "CBigSnake.h"
#include "CCaveman.h"
#include "CSkeleton.h"
#include "CSnake.h"
#include "CSpider.h"
#include "CDispenser.h"
#include "CSpike.h"
#include "CShopkeeper.h"
#include "COlmec.h"
#include "CTextureUI.h"
#include "CTextUI.h"
#include "CLevelMgr.h"
#include "CExit.h"

#include "CPassiveItem.h"
#include "CBorder.h"

CLevel_BossStage::CLevel_BossStage()
	: m_TileMap(nullptr)
{
}

CLevel_BossStage::~CLevel_BossStage()
{
}

void CLevel_BossStage::AddObjectToStage()
{
	// 오브젝트 위치 설정
	CPlayer* pPlayer = new CPlayer;
	pPlayer->SetName(L"Player");
	Vec2 vResol = CEngine::Get()->GetResolution();
	pPlayer->SetPos(vResol.x / 2.f, vResol.y / 2.f + 700.f);
	pPlayer->SetScale(80.f, 75.f);

	CSound* pSound = CAssetMgr::Get()->LoadSound(L"Chime", L"sound\\Chime.wav");
	pSound->PlayToBGM();
	pSound->Stop();

	// 플레이어를 현재 레벨에 등록
	RegisterPlayer(pPlayer);

	CLevel* pPrevLevel = CLevelMgr::Get()->GetPrevLevel();
	SetpInfo(pPrevLevel->GetpIfno());
	LoadPlayerInfo();
	m_PlayTime = pPrevLevel->LoadPlayTime();
	



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

	wstring strTilePath = strContentPath + L"\\tile\\Boss.tile";

	m_TileMap->Load(strTilePath);

	AddObject(pObj, LAYER_TYPE::PLATFORM);


	// 올멕
	m_Olmec = new COlmec;
	m_Olmec->SetName(L"Olmec");
	m_Olmec->SetPos(Vec2(1280.f, 1000.f));
	m_Olmec->SetScale(Vec2(250.f, 250.f));

	AddObject(m_Olmec, LAYER_TYPE::MONSTER);

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
	pCloneBorder->SetName(L"Lava");
	pCloneBorder->SetPos(Vec2(1280.f, 2360.f));
	pCloneBorder->SetTexture(CAssetMgr::Get()->LoadTexture(L"Lava", L"texture\\lava.png"));
	AddObject(pCloneBorder, LAYER_TYPE::BORDER);

	// 출구
	m_Exit = new CExit;
	m_Exit->SetName(L"Boss Exit");
	m_Exit->SetPos(Vec2(2300.f, 1080.f));
	m_Exit->SetAtlas(CAssetMgr::Get()->LoadTexture(L"Exit", L"texture\\Exit.png"));
	AddObject(m_Exit, LAYER_TYPE::ETRANCE);

	CCollider* pCollider = m_Exit->GetComponent<CCollider>();
	pCollider->Deactivate();

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

	//m_StageUI = pUI->Clone();
	//m_StageUI->SetName(L"Cave1UI");
	//m_StageUI->SetPos(Vec2(1040, 100.f));
	//m_StageUI->SetTexture(CAssetMgr::Get()->LoadTexture(L"Cave1UI", L"texture\\Cave1UI.png"));
	//AddObject(m_StageUI, LAYER_TYPE::UI);


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

}

void CLevel_BossStage::Init()
{
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

void CLevel_BossStage::Exit()
{
	// 모든 오브젝트를 삭제해둔다.
	DeleteAllObjects();
}

void CLevel_BossStage::Tick()
{
	CLevel::Tick();

	m_PlayTime += DT;
	m_AccTime += DT;
	if (2.f < m_AccTime)
	{
		//DeleteObject(m_StageUI);
	}

	if (IsChange())
	{
		for (int i = 0; i < sizeof(m_TextUI) / sizeof(CTextUI*); ++i)
			DeleteObject(m_TextUI[i]);

		ChangePlayerInfo(m_TextUI);
		CheckItemUI();
		SetChange(false);
	}

	if (m_Olmec->GetPos().y > 3000.f)
	{
		CCollider* pCollider = m_Exit->GetComponent<CCollider>();
		pCollider->Activate();
	}

	CPlayer* pPlayer = CLevelMgr::Get()->GetCurrentLevel()->GetPlayer();
	if (pPlayer->GetPos().y > 2360.f)
	{
		CStateMachine* pSm = pPlayer->GetComponent<CStateMachine>();
		pSm->SetState(PLAYER_STATE::ISDEAD, true);
	}
	//Vec2 pLookAt = CCamera::Get()->GetLookAt();
	//float pCamSpeed = CCamera::Get()->GetCamSpeed();


	//CCamera::Get()->SetLookAt(pLookAt);
}


