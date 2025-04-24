#include "pch.h"
#include "CAnimator.h"

#include "CPathMgr.h"
#include "CAnim.h"

CAnimator::CAnimator()
	: CComponent(COMPONENT_TYPE::ANIMATOR)
	, m_CurAnim(nullptr)
	, m_Repeat(false)
{

}

CAnimator::CAnimator(const CAnimator& _Other)
	: CComponent(_Other)	
	, m_CurAnim(nullptr)
	, m_Repeat(_Other.m_Repeat)
{
	map<wstring, CAnim*>::const_iterator iter = _Other.m_mapAnim.begin();
	for (; iter != _Other.m_mapAnim.end(); ++iter)
	{
		CAnim* pCloneAnim = iter->second->Clone();

		pCloneAnim->m_Owner = this;
		m_mapAnim.insert(make_pair(iter->first, pCloneAnim));
	}


	if (_Other.m_CurAnim)
	{
		m_CurAnim = FindAnimation(_Other.m_CurAnim->GetName());		
	}
}

CAnimator::~CAnimator()
{
	Release_Map(m_mapAnim);
}

void CAnimator::CreateAnimation(const tAnimDesc& _Info)
{
	// 이미 같은 이름의 Animation 이 있는지 확인
	CAnim* pAnim = FindAnimation(_Info.AnimName);
	assert(nullptr == pAnim);
	
	// Animation 하나 생성하고 입력된 정보를 알려줌
	pAnim = new CAnim;
	pAnim->Create(_Info);

	// Animator 에 생성시킨 Animation 등록
	pAnim->m_Owner = this;
	m_mapAnim.insert(make_pair(_Info.AnimName, pAnim));
}

CAnim* CAnimator::FindAnimation(const wstring& _Name)
{
	map<wstring, CAnim*>::iterator iter = m_mapAnim.find(_Name);

	if (iter == m_mapAnim.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CAnimator::Play(const wstring& _Name, bool _Repeat)
{
	m_CurAnim = FindAnimation(_Name);
	m_CurAnim->Reset();

	m_Repeat = _Repeat;
}

void CAnimator::Render()
{
	// 재생중인 Animation 이 없으면 반환
	if (!m_CurAnim)
		return;

	m_CurAnim->Render();
}



void CAnimator::FinalTick()
{
	// 재생중인 Animation 이 없으면 반환
	if (!m_CurAnim)
		return;

	if (m_Repeat && m_CurAnim->IsFinish())
	{
		m_CurAnim->Reset();
	}

	m_CurAnim->FinalTick();
}

void CAnimator::SaveAnimation(const wstring& _RelativeFolder)
{
	wstring strFolderPath = CPathMgr::Get()->GetContentPath();
	strFolderPath += _RelativeFolder;

	map<wstring, CAnim*>::iterator iter = m_mapAnim.begin();

	for (; iter != m_mapAnim.end(); ++iter)
	{
		iter->second->Save(strFolderPath);
	}
}

void CAnimator::LoadAnimation(const wstring& _RelativePath)
{
	wstring strFilePath = CPathMgr::Get()->GetContentPath();	
	strFilePath += _RelativePath;

	CAnim* pNewAnim = new CAnim;
	pNewAnim->Load(strFilePath);
	assert(!FindAnimation(pNewAnim->GetName()));

	pNewAnim->m_Owner = this;
	m_mapAnim.insert(make_pair(pNewAnim->GetName(), pNewAnim));
}
