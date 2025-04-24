#include "pch.h"
#include "CLevel.h"

#include "CObj.h"
#include "CBigSnake.h"
#include "CCaveman.h"
#include "CSkeleton.h"
#include "CSnake.h"
#include "CSpider.h"
#include "CDispenser.h"
#include "CSpike.h"
#include "CShopkeeper.h"
#include "CShotgun.h"
#include "CExit.h"
#include "CVictim.h"
#include "CPassiveItem.h"
#include "CETCObj.h"
#include "CBox.h"
#include "CPlayer.h"
#include "CTextUI.h"
#include "CPassiveItem.h"
#include "CTextureUI.h"

#include "CAssetMgr.h"
#include "CStateMachine.h"
#include "CRigidBody.h"

CLevel::CLevel()
	: m_CurPlayer(nullptr)
	, m_mapObj{}
{
}

CLevel::~CLevel()
{
	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		Release_Vector(m_arrLayer[i]);
	}	
}

void CLevel::Tick()
{
 	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		for (int j = 0; j < m_arrLayer[i].size(); ++j)
		{
			m_arrLayer[i][j]->Tick();
		}
 	}
}


void CLevel::FinalTick()
{
	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		for (int j = 0; j < m_arrLayer[i].size(); ++j)
		{
			m_arrLayer[i][j]->FinalTick();
		}
	}
}

void CLevel::Render()
{
	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		vector<CObj*>& vecObj = m_arrLayer[i];

		vector<CObj*>::iterator iter = vecObj.begin();
		for (; iter != vecObj.end(); )
		{
			if ((*iter)->IsDead())
			{
				iter = vecObj.erase(iter);
			}
			else
			{
				(*iter)->Render();
				++iter;
			}		
		}
	}
}

void CLevel::ObjectSave(const wstring& _strPath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _strPath.c_str(), L"wb");

	map<wstring, Vec2>::iterator iter = m_mapObj.begin();
	
	int count = m_mapObj.size();

	fwrite(&count, sizeof(int), 1, File);
	
	for (; iter != m_mapObj.end(); ++iter)
	{
		// 오브젝트의 이름
		SaveWString(iter->first, File);

		// 오브젝트의 위치
		fwrite(&iter->second.x, sizeof(float), 1, File);
		fwrite(&iter->second.y, sizeof(float), 1, File);

	}


	

	//for (int i = 0; i < count; ++i)
	//{
	//	wstring pObjName = m_vecObject[i]->GetName();
	//	Vec2 pObjPos = m_vecObject[i]->GetPos();
	//	Vec2 pObjScale = m_vecObject[i]->GetScale();
	//	int pObjLayerType = m_vecObject[i]->GetLayerType();

	//	// 오브젝트의 이름
	//	SaveWString(pObjName, File);

	//	// 오브젝트의 위치
	//	fwrite(&pObjPos.x, sizeof(float), 1, File);
	//	fwrite(&pObjPos.y, sizeof(float), 1, File);

	//	// 오브젝트의 크기
	//	fwrite(&pObjScale.x, sizeof(float), 1, File);
	//	fwrite(&pObjScale.y, sizeof(float), 1, File);

	//	// 오브젝트의 레이어 타입
	//	fwrite(&pObjLayerType, sizeof(int), 1, File);

	//	// 오브젝트가 참조하던 아틀라스 텍스쳐 정보
	//	//SaveAssetRef(m_Atlas, File);
	//	//fwrite(&m_AtlasTileSize, sizeof(Vec2), 1, File);
	//}

	fclose(File);
}

void CLevel::ObjectLoad(const wstring& _strPath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _strPath.c_str(), L"rb");

	int count = 0;
	
	fread(&count, sizeof(int), 1, File);

	for (int i = 0; i < count; ++i)
	{
		wstring pObjStr;
		Vec2 pObjPos;
		// 오브젝트의 이름
		LoadWString(pObjStr, File);

		// 오브젝트의 위치
		fread(&pObjPos.x, sizeof(float), 1, File);
		fread(&pObjPos.y, sizeof(float), 1, File);

		m_mapObj.insert(make_pair(pObjStr, pObjPos));
	}
	



	//	// 오브젝트가 참조하던 아틀라스 텍스쳐 정보
	//	//SaveAssetRef(m_Atlas, File);
	//	//fwrite(&m_AtlasTileSize, sizeof(Vec2), 1, File);
	//}

	//fclose(File);
}


void CLevel::LoadObjectToStage(wstring _str, Vec2 _pos)
{
	Vec2 vPos = _pos;
	
	if (_str == L"BigSnake")
	{
		CBigSnake* pObj = new CBigSnake;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		AddObject(pObj, LAYER_TYPE::MONSTER);
	}
	else if (_str == L"CavemanR")
	{
		CCaveman* pObj = new CCaveman;
		vPos.y -= 20.f;
		pObj->SetName(L"Caveman");
		pObj->SetInitDir(true);
		pObj->SetPos(vPos);
		AddObject(pObj, LAYER_TYPE::MONSTER);
	}
	else if (_str == L"CavemanL")
	{
		CCaveman* pObj = new CCaveman;
		vPos.y -= 20.f;
		pObj->SetName(L"Caveman");
		pObj->SetInitDir(false);
		pObj->SetPos(vPos);
		AddObject(pObj, LAYER_TYPE::MONSTER);
	}
	else if (_str == L"Skeleton")
	{
		CSkeleton* pObj = new CSkeleton;
		vPos.y -= 10.f;
		pObj->SetName(_str);
		pObj->SetPos(vPos);
		AddObject(pObj, LAYER_TYPE::MONSTER);
	}
	else if (_str == L"Snake")
	{
		CSnake* pObj = new CSnake;
		vPos.y -= 10.f;
		pObj->SetName(_str);
		pObj->SetPos(vPos);
		AddObject(pObj, LAYER_TYPE::MONSTER);
	}
	else if (_str == L"Spider")
	{
		CSpider* pObj = new CSpider;
		vPos.y -= 10.f;
		pObj->SetName(_str);
		pObj->SetPos(vPos);
		AddObject(pObj, LAYER_TYPE::MONSTER);
	}
	else if (_str == L"DispenserR")
	{
		CDispenser* pObj = new CDispenser;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"DispenserR", L"texture\\Dispenser.png"));
		pObj->SetIsRight(true);
		AddObject(pObj, LAYER_TYPE::HOSTILEPLATFORM);
	}
	else if (_str == L"DispenserL")
	{
		CDispenser* pObj = new CDispenser;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"DispenserL", L"texture\\Dispenser_left.png"));
		pObj->SetIsRight(false);
		AddObject(pObj, LAYER_TYPE::HOSTILEPLATFORM);
	}
	else if (_str == L"Spike")
	{
		CSpike* pObj = new CSpike;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Spike", L"texture\\Spike.png"));
		AddObject(pObj, LAYER_TYPE::HOSTILEPLATFORM);
	}
	else if (_str == L"ShopkeeperR")
	{
		CShopkeeper* pObj = new CShopkeeper;
		pObj->SetName(L"Shopkeeper");
		pObj->SetInitDir(true);
		pObj->SetPos(_pos);

		AddObject(pObj, LAYER_TYPE::MONSTER);
	}
	else if (_str == L"ShopkeeperL")
	{
		CShopkeeper* pObj = new CShopkeeper;
		pObj->SetName(L"Shopkeeper");
		pObj->SetInitDir(false);
		pObj->SetPos(_pos);

		AddObject(pObj, LAYER_TYPE::MONSTER);
	}
	else if (_str == L"VictimR")
	{
		CVictim* pObj = new CVictim;
		pObj->SetName(L"Victim");
		pObj->SetInitDir(true);
		pObj->SetPos(_pos);

		AddObject(pObj, LAYER_TYPE::MONSTER);
	}
	else if (_str == L"VictimL")
	{
		CVictim* pObj = new CVictim;
		pObj->SetName(L"Victim");
		pObj->SetInitDir(false);
		pObj->SetPos(_pos);

		AddObject(pObj, LAYER_TYPE::MONSTER);
	}
	else if (_str == L"Rock")
	{
		CETCObj* pObj = new CETCObj;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Rock", L"texture\\Rock.png"));

		AddObject(pObj, LAYER_TYPE::ACTIVEITEM);
	}
	else if (_str == L"Glove")
	{
		CPassiveItem* pObj = new CPassiveItem;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Glove", L"texture\\glove.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	}
	else if (_str == L"Gold")
	{
		CPassiveItem* pObj = new CPassiveItem;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Gold", L"texture\\gold.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	}
	else if (_str == L"Ruby")
	{
		CPassiveItem* pObj = new CPassiveItem;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Ruby", L"texture\\ruby.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	}
	else if (_str == L"SpikeShoe")
	{
		CPassiveItem* pObj = new CPassiveItem;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"SpikeShoe", L"texture\\spikeshoe.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
		}
	else if (_str == L"BombSet")
	{
		CPassiveItem* pObj = new CPassiveItem;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"BombSet", L"texture\\bomb_box.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
		}
	else if (_str == L"Emerald")
	{
		CPassiveItem* pObj = new CPassiveItem;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Emerald", L"texture\\emerald.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
		}
	else if (_str == L"GoldPackage")
	{
		CPassiveItem* pObj = new CPassiveItem;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"GoldPackage", L"texture\\gold_package.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
		}
	else if (_str == L"RopePackage")
	{
		CPassiveItem* pObj = new CPassiveItem;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"RopePackage", L"texture\\rope_package.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	}
	else if (_str == L"BombPackage")
	{
		CPassiveItem* pObj = new CPassiveItem;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"BombPackage", L"texture\\bomb_package.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	}
	else if (_str == L"Sapphire")
	{
		CPassiveItem* pObj = new CPassiveItem;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Sapphire", L"texture\\sapphire.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	}
	else if (_str == L"SpringShoe")
	{
		CPassiveItem* pObj = new CPassiveItem;
		pObj->SetName(_str);
		pObj->SetPos(_pos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"SpringShoe", L"texture\\springshoe.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
		}
	else if (_str == L"Exit")
	{
		CExit* pObj = new CExit;
		vPos.y -= 30.f;
		pObj->SetName(_str);
		pObj->SetPos(vPos);
		pObj->SetAtlas(CAssetMgr::Get()->LoadTexture(L"Exit", L"texture\\Exit.png"));

		AddObject(pObj, LAYER_TYPE::ETRANCE);
		}
	else if (_str == L"BombBox")
	{
		CBox* pObj = new CBox;
		vPos.y -= 40.f;
		pObj->SetName(_str);
		pObj->SetPos(vPos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Box", L"texture\\box.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	}
	else if (_str == L"RopeBox")
	{
		CBox* pObj = new CBox;
		vPos.y -= 40.f;
		pObj->SetName(_str);
		pObj->SetPos(vPos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Box", L"texture\\box.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
		}
	else if (_str == L"ShotgunBox")
	{
		CBox* pObj = new CBox;
		vPos.y -= 40.f;
		pObj->SetName(_str);
		pObj->SetPos(vPos);
		pObj->SetTexture(CAssetMgr::Get()->LoadTexture(L"Box", L"texture\\box.png"));

		AddObject(pObj, LAYER_TYPE::PASSSIVEITEM);
	}
}

void CLevel::ChangePlayerInfo(CTextUI* _Text[])
{

	_Text[0] = new CTextUI;
	_Text[0]->SetName(L"HPText");
	_Text[0]->SetPos(Vec2(82.f, 45.f));
	_Text[0]->SetString(std::to_wstring(m_CurPlayer->GetCurrentHP()));
	_Text[0]->SetScale(50);
	AddObject(_Text[0], LAYER_TYPE::UI);

	_Text[1] = _Text[0]->Clone();
	_Text[1]->SetName(L"BombText");
	_Text[1]->SetPos(Vec2(200.f, 55.f));
	_Text[1]->SetString(std::to_wstring(m_CurPlayer->GetCurrentBomb()));
	_Text[1]->SetScale(30);
	AddObject(_Text[1], LAYER_TYPE::UI);

	_Text[2] = _Text[0]->Clone();
	_Text[2]->SetName(L"RopeText");
	_Text[2]->SetPos(Vec2(305.f, 55.f));
	_Text[2]->SetString(std::to_wstring(m_CurPlayer->GetCurrentRope()));
	_Text[2]->SetScale(30);
	AddObject(_Text[2], LAYER_TYPE::UI);

	_Text[3] = _Text[0]->Clone();
	_Text[3]->SetName(L"GoldText");
	_Text[3]->SetPos(Vec2(180.f, 140.f));
	_Text[3]->SetString(std::to_wstring(m_CurPlayer->GetCurrentGold()));
	_Text[3]->SetScale(30);
	AddObject(_Text[3], LAYER_TYPE::UI);
}

void CLevel::CheckItemUI()
{
	wstring pVector = m_CurPlayer->GetCurItem();
	int pOrder = m_CurPlayer->GetItemOrder();
	
	if (pVector == L"SpikeShoe")
	{
		CTextureUI* pUI = new CTextureUI;
		pUI->SetName(L"ItemSpikeShoe");
		pUI->SetPos(Vec2(125 + pOrder *40, 105));
		pUI->SetTexture(CAssetMgr::Get()->LoadTexture(L"SpikeShoe", L"texture\\spikeshoe.png"));
		AddObject(pUI, LAYER_TYPE::UI);
	}
	else if (pVector == L"SpringShoe")
	{
		CTextureUI* pUI = new CTextureUI;
		pUI->SetName(L"ItemSpringShoe");
		pUI->SetPos(Vec2(120 + pOrder * 40, 110));
		pUI->SetTexture(CAssetMgr::Get()->LoadTexture(L"SpringShoe", L"texture\\springshoe.png"));
		AddObject(pUI, LAYER_TYPE::UI);
	}
	else if (pVector == L"Glove")
	{
		CTextureUI* pUI = new CTextureUI;
		pUI->SetName(L"ItemGlove");
		pUI->SetPos(Vec2(120 + pOrder * 40, 110));
		pUI->SetTexture(CAssetMgr::Get()->LoadTexture(L"Glove", L"texture\\glove.png"));
		AddObject(pUI, LAYER_TYPE::UI);
	}
}

void CLevel::SavePlayerInfo()
{
	m_pInfo.HP = m_CurPlayer->GetCurrentHP();
	m_pInfo.Bomb = m_CurPlayer->GetCurrentBomb();
	m_pInfo.Rope = m_CurPlayer->GetCurrentRope();
	m_pInfo.Gold = m_CurPlayer->GetCurrentGold();
	m_pInfo.FeetDamage = m_CurPlayer->GetFeetDamage();
	m_pInfo.HasGlove = m_CurPlayer->HasGlove();
	m_pInfo.JumpSpeed = m_CurPlayer->HasSpring();

	CStateMachine* pSm = m_CurPlayer->GetComponent<CStateMachine>();
	m_pInfo.GetShotgun = pSm->GetState(PLAYER_STATE::GETSHOTGUN);

	if (m_pInfo.HasGlove)
		m_pInfo.ItemList.push_back(L"Glove");
	if (m_pInfo.JumpSpeed)
		m_pInfo.ItemList.push_back(L"SpringShoe");
	if (4 < m_pInfo.FeetDamage)
		m_pInfo.ItemList.push_back(L"SpikeShoe");
}

void CLevel::LoadPlayerInfo()
{
	m_CurPlayer->SetHP(m_pInfo.HP);
	m_CurPlayer->SetBomb(m_pInfo.Bomb);
	m_CurPlayer->SetRope(m_pInfo.Rope);
	m_CurPlayer->SetGold(m_pInfo.Gold);
	m_CurPlayer->SetFeetDamage(m_pInfo.FeetDamage);
	m_CurPlayer->SetGlove(m_pInfo.HasGlove);

	CRigidBody* pRigidBody = m_CurPlayer->GetComponent<CRigidBody>();
	if (m_pInfo.JumpSpeed)
		pRigidBody->SetJumpSpeed(700.f);
	else
		pRigidBody->SetJumpSpeed(580.f);

	CStateMachine* pSm = m_CurPlayer->GetComponent<CStateMachine>();
	if (m_pInfo.GetShotgun)
		pSm->SetState(PLAYER_STATE::GETSHOTGUN, true);
	else
		pSm->SetState(PLAYER_STATE::GETSHOTGUN, false);

	for (int i = 0; i < m_pInfo.ItemList.size(); ++i)
	{
		m_CurPlayer->SetCurItem(m_pInfo.ItemList[i]);
		m_CurPlayer->SetItemOrder(i);
		CheckItemUI();
	}

}

void CLevel::UpdatePlayTime(float _AccTime)
{
	m_pInfo.PlayTime = _AccTime;
}

float CLevel::LoadPlayTime()
{
	return m_pInfo.PlayTime;
}

void CLevel::AddObject(CObj* _Obj, LAYER_TYPE _Type)
{
	m_arrLayer[(int)_Type].push_back(_Obj);
	_Obj->m_LayerType = (int)_Type;
}

void CLevel::DeleteObjects(LAYER_TYPE _Type)
{
	vector<CObj*>& vecObj = m_arrLayer[(int)_Type];
	Release_Vector(vecObj);
}

void CLevel::DeleteAllObjects()
{
	for (int i = 0; i < (int)LAYER_TYPE::END; ++i)
	{
		DeleteObjects((LAYER_TYPE)i);
	}
}

