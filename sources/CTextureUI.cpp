#include "pch.h"
#include "CTextureUI.h"

#include "CTexture.h"
#include "CEngine.h"


CTextureUI::CTextureUI()
	:m_Texture(nullptr)
{
}

CTextureUI::CTextureUI(const CTextureUI& _Other)
	: CUI(_Other)
	, m_Texture(nullptr)
{
}

CTextureUI::~CTextureUI()
{

}
void CTextureUI::Tick_UI()
{
}

void CTextureUI::Render_UI()
{
	Vec2 vPos = GetFinalPos();
	UINT width = m_Texture->GetWidth();
	UINT height = m_Texture->GetHeight();

	BLENDFUNCTION blend = {};
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(BackDC, (int)vPos.x - width / 2, (int)vPos.y - height / 2
			, width, height, m_Texture->GetDC(), 0, 0, width, height, blend);

}


