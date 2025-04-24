#include "pch.h"
#include "CEditorBox.h"

#include "CEngine.h"

CEditorBox::CEditorBox()
	: m_SelectedIndex{}
	, m_Size(Vec2(0.f,0.f))
	, m_Row(0)
	, m_Col(0)
	, m_Change(false)
	, m_IsGreen{}
{
}

CEditorBox::~CEditorBox()
{
}

void CEditorBox::Tick()
{
}

void CEditorBox::Render()
{
	Vec2 vRenderPos = GetRenderPos();

	SELECT_PEN(BackDC, PEN_TYPE::GREEN);

	for (int i = 0; i < m_Row; ++i)
	{
		
		for (int j = 0; j < m_Col; ++j)
		{
			m_Change = false;
			for (int k = 0; k < m_SelectedIndex.size(); k++)
			{
				if (Vec2(i, j) == m_SelectedIndex[k])
					m_Change = true;
			}
			if (m_Change)
			{
				SELECT_BRUSH(BackDC, BRUSH_TYPE::GREEN);
				Rectangle(BackDC, vRenderPos.x + (m_Size.x * j), vRenderPos.y + (m_Size.y * i),
					vRenderPos.x + (m_Size.x * (j + 1)), vRenderPos.y + (m_Size.y * (i + 1)));
			}
			else
			{
				SELECT_BRUSH(BackDC, BRUSH_TYPE::HOLLOW);
				Rectangle(BackDC, vRenderPos.x + (m_Size.x * j), vRenderPos.y + (m_Size.y * i),
					vRenderPos.x + (m_Size.x * (j + 1)), vRenderPos.y + (m_Size.y * (i + 1)));
			}

		}
	}
}

