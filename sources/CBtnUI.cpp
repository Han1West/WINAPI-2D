#include "pch.h"
#include "CBtnUI.h"

#include "Resource.h"
#include "CEngine.h"
#include "CTexture.h"
#include "CAssetMgr.h"
#include "CSound.h"





CBtnUI::CBtnUI()
	: m_CallBack(nullptr)
	, m_Inst(nullptr)
	, m_MemFunc(nullptr)
	, m_MemFunc_1(nullptr)
	, m_MemFunc_2(nullptr)
{
}

CBtnUI::~CBtnUI()
{
}

LRESULT CALLBACK TileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void CBtnUI::LBtnDown()
{
	
}

void CBtnUI::LBtnClicked()
{
	if (m_CallBack)
	{
		CSound* pSound = CAssetMgr::Get()->LoadSound(L"MenuEnter", L"sound\\menu_enter.wav");
		pSound->Play();
		m_CallBack();
	}

	if (m_Inst && m_MemFunc)
	{
		(m_Inst->*m_MemFunc)();
	}
}

void CBtnUI::Render_UI()
{
	//PEN_TYPE Pen = PEN_TYPE::GREEN;


	//SELECT_PEN(BackDC, Pen);
	//SELECT_BRUSH(BackDC, BRUSH_TYPE::GRAY);

	//Vec2 vFinalPos = GetFinalPos();
	//Vec2 vScale = GetScale();
	//Rectangle(BackDC, (int)vFinalPos.x, (int)vFinalPos.y
	//	, (int)vFinalPos.x + (int)vScale.x, (int)vFinalPos.y + (int)vScale.y);

	Vec2 vPos = GetFinalPos();



	if (IsMouseOn())
	{
		UINT width = m_HoverImg->GetWidth();
		UINT height = m_HoverImg->GetHeight();

		BLENDFUNCTION blend = {};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(BackDC, (int)vPos.x - width / 2, (int)vPos.y - height / 2
			, width, height, m_HoverImg->GetDC(), 0, 0, width, height, blend);
	}
	else
	{
		UINT width = m_NormalImg->GetWidth();
		UINT height = m_NormalImg->GetHeight();

		BLENDFUNCTION blend = {};
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = 0;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;

		AlphaBlend(BackDC, (int)vPos.x - width / 2, (int)vPos.y - height / 2
			, width, height, m_NormalImg->GetDC(), 0, 0, width, height, blend);
	}

	//else if (IsLBtnDown())
	//{
	//	Pen = PEN_TYPE::RED;
	//}

	// AlphaBlending
	//{
	//	BLENDFUNCTION blend = {};
	//	blend.BlendOp = AC_SRC_OVER;
	//	blend.BlendFlags = 0;
	//	blend.SourceConstantAlpha = 255;
	//	blend.AlphaFormat = AC_SRC_ALPHA;

	//	AlphaBlend(BackDC, (int)vPos.x - width / 2, (int)vPos.y - height / 2
	//		, width, height, m_Texture->GetDC(), 0, 0, width, height, blend);
	//}
}