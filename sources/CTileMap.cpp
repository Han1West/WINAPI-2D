#include "pch.h"
#include "CTileMap.h"

#include "CEngine.h"
#include "CTexture.h"
#include "CCamera.h"

CTileMap::CTileMap()
	: CComponent(COMPONENT_TYPE::TILEMAP)
	, m_Row(1)
	, m_Col(1)
	, m_TileSize(Vec2(32.f, 32.f))
	, m_IsChange(true)
{
}

CTileMap::~CTileMap()
{
}

void CTileMap::FinalTick()
{
}

void CTileMap::Render()
{
	Vec2 vPos = GetOwner()->GetRenderPos();
	

	SELECT_PEN(BackDC, PEN_TYPE::MAGENTA);

	//for (int iRow = 0; iRow < m_Row + 1; ++iRow)
	//{
	//	MoveToEx(BackDC, (int)vPos.x, (int)vPos.y + iRow * (int)m_TileSize.y, nullptr);
	//	LineTo(BackDC, (int)vPos.x + m_Col * (int)m_TileSize.x, (int)vPos.y + iRow * (int)m_TileSize.y);		
	//}
	//
	//for (int iCol = 0; iCol < m_Col + 1; ++iCol)
	//{
	//	MoveToEx(BackDC, (int)vPos.x + iCol * (int)m_TileSize.x, (int)vPos.y, nullptr);
	//	LineTo(BackDC, (int)vPos.x + iCol * (int)m_TileSize.x, (int)vPos.y + m_Row * (int)m_TileSize.y);
	//}

	if (nullptr != m_Atlas)
	{
		Render_Tile();
	}
}

void CTileMap::Render_Tile()
{
	//화면 해상도
	Vec2 vResolution = CEngine::Get()->GetResolution();

		
	// 타일 오브젝트 시작 좌상단 위치
	Vec2 vRenderPos = GetOwner()->GetRenderPos();

	// 카메라가 바라보는 실제 좌표
	Vec2 vLookAt = CCamera::Get()->GetLookAt();
	Vec2 vCamLookLeftTop = vLookAt - (vResolution / 2.f);

	// 카메라 시야 좌상단과 타일오브젝트 시작위치의 차이값
	Vec2 vOffset = vCamLookLeftTop -  GetOwner()->GetPos();

	// 카메라 시야 좌상단이 타일 오브젝트의 몇행, 몇열 지점인지 확인
	int LeftTopCol = vOffset.x / m_TileSize.x;
	int LeftTopRow = vOffset.y / m_TileSize.y;

	if (vOffset.x < 0.f)
		LeftTopCol = 0;
	if (vOffset.y < 0.f)
		LeftTopRow = 0;
	
	// 화면안에 들어오는 타일의 가로 개수, 세로 개수를 계산
	int TileMaxCol = (vResolution.x / m_TileSize.x) + 1;
	int TileMaxRow = (vResolution.y / m_TileSize.y) + 1;
	
	int RightBotCol = LeftTopCol + TileMaxCol;
	int RightBotRow = LeftTopRow + TileMaxRow;

	if (m_Col <= RightBotCol)	
		RightBotCol = m_Col;

	if (m_Row <= RightBotRow)
		RightBotRow = m_Row;
	
	for (int iRow = LeftTopRow; iRow < RightBotRow; ++iRow)
	{
		for (int iCol = LeftTopCol; iCol < RightBotCol; ++iCol)
		{
			// 화면상에서 타일을 렌더링할 좌상단 위치
			Vec2 Pos = vRenderPos + Vec2(iCol * m_TileSize.x, iRow * m_TileSize.y);

			// 렌더링 포지션이 화면밖에 벗어나는 타일이면, 화면에 잡히지 않는 타일이라는 뜻
			// 이런 타일들은 화면에 그리지 않는다.
			if (Pos.x < -m_TileSize.x || vResolution.x < Pos.x ||
				Pos.y < -m_TileSize.y || vResolution.y < Pos.y)
			{
				continue;
			}

			// 벡터에 들어있는 타일의 정보를 가져오기 위해서 현재 타일의 행렬로 1차원 인덱스를 구함
			int idx = iRow * m_Col + iCol;			

			if (-1 == m_vecTileInfo[idx].ImgIdx)
				continue;

			// 타일정보에 들어있는 이미지 인덱스를 이용해서, 아틀라스의 어디지점을 가져올지 좌상단 죄표를 계산함
			Vec2 vLeftTop = Vec2(m_AtlasTileSize.x * (m_vecTileInfo[idx].ImgIdx % m_AtlasMaxCol)
								, m_AtlasTileSize.y * (m_vecTileInfo[idx].ImgIdx / m_AtlasMaxCol));
			
			// 타일 위치에, 아틀라스에서 가져올 이미지를 잘라서 붙여넣음
			BLENDFUNCTION blend = {};
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			blend.SourceConstantAlpha = 255;
			blend.AlphaFormat = AC_SRC_ALPHA;

			//BitBlt(BackDC, (int)Pos.x, (int)Pos.y, (int)m_TileSize.x, (int)m_TileSize.y
				//  , m_Atlas->GetDC(), (int)vLeftTop.x, (int)vLeftTop.y, SRCCOPY);
			AlphaBlend(BackDC, (int)Pos.x, (int)Pos.y, (int)m_TileSize.x, (int)m_TileSize.y
				, m_Atlas->GetDC(), (int)vLeftTop.x, (int)vLeftTop.y, 
				(int)m_TileSize.x, (int)m_TileSize.y,blend);
		}
	}
}

void CTileMap::Render_Tile(HDC dc, HWND hWnd)
{
	// 화면 해상도
	Vec2 vResolution = {};
	if (hWnd == CEngine::Get()->GetMainHwnd())
		vResolution = CEngine::Get()->GetResolution();
	else
	{
		RECT rt = {};
		GetClientRect(hWnd, &rt);
		vResolution = Vec2(rt.right, rt.bottom);
	}

	// 타일 오브젝트 시작 좌상단 위치
	Vec2 vRenderPos = GetOwner()->GetPos();

	// 카메라가 바라보는 실제 좌표
	Vec2 vLookAt = CCamera::Get()->GetLookAt();
	Vec2 vCamLookLeftTop = vLookAt - (vResolution / 2.f);

	// 카메라 시야 좌상단과 타일오브젝트 시작위치의 차이값
	Vec2 vOffset = GetOwner()->GetPos();

	// 카메라 시야 좌상단이 타일 오브젝트의 몇행, 몇열 지점인지 확인
	int LeftTopCol = vOffset.x / m_TileSize.x;
	int LeftTopRow = vOffset.y / m_TileSize.y;

	if (vOffset.x < 0.f)
		LeftTopCol = 0;
	if (vOffset.y < 0.f)
		LeftTopRow = 0;

	// 화면안에 들어오는 타일의 가로 개수, 세로 개수를 계산
	int TileMaxCol = (vResolution.x / m_TileSize.x) + 1;
	int TileMaxRow = (vResolution.y / m_TileSize.y) + 1;

	int RightBotCol = LeftTopCol + TileMaxCol;
	int RightBotRow = LeftTopRow + TileMaxRow;

	if (m_Col <= RightBotCol)
		RightBotCol = m_Col;

	if (m_Row <= RightBotRow)
		RightBotRow = m_Row;

	for (int iRow = LeftTopRow; iRow < RightBotRow; ++iRow)
	{
		for (int iCol = LeftTopCol; iCol < RightBotCol; ++iCol)
		{
			// 화면상에서 타일을 렌더링할 좌상단 위치
			Vec2 Pos = vRenderPos + Vec2(iCol * m_TileSize.x, iRow * m_TileSize.y);

			// 렌더링 포지션이 화면밖에 벗어나는 타일이면, 화면에 잡히지 않는 타일이라는 뜻
			// 이런 타일들은 화면에 그리지 않는다.
			if (Pos.x < -m_TileSize.x || vResolution.x < Pos.x ||
				Pos.y < -m_TileSize.y || vResolution.y < Pos.y)
			{
				continue;
			}

			// 벡터에 들어있는 타일의 정보를 가져오기 위해서 현재 타일의 행렬로 1차원 인덱스를 구함
			int idx = iRow * m_Col + iCol;

			if (-1 == m_vecTileInfo[idx].ImgIdx)
				continue;

			// 타일정보에 들어있는 이미지 인덱스를 이용해서, 아틀라스의 어디지점을 가져올지 좌상단 죄표를 계산함
			Vec2 vLeftTop = Vec2(m_AtlasTileSize.x * (m_vecTileInfo[idx].ImgIdx % m_AtlasMaxCol)
				, m_AtlasTileSize.y * (m_vecTileInfo[idx].ImgIdx / m_AtlasMaxCol));

			// 타일 위치에, 아틀라스에서 가져올 이미지를 잘라서 붙여넣음
			BLENDFUNCTION blend = {};
			blend.BlendOp = AC_SRC_OVER;
			blend.BlendFlags = 0;
			blend.SourceConstantAlpha = 255;
			blend.AlphaFormat = AC_SRC_ALPHA;

			//BitBlt(BackDC, (int)Pos.x, (int)Pos.y, (int)m_TileSize.x, (int)m_TileSize.y
				//  , m_Atlas->GetDC(), (int)vLeftTop.x, (int)vLeftTop.y, SRCCOPY);
			AlphaBlend(dc, (int)Pos.x, (int)Pos.y, (int)m_TileSize.x, (int)m_TileSize.y
				, m_Atlas->GetDC(), (int)vLeftTop.x, (int)vLeftTop.y,
				(int)m_TileSize.x, (int)m_TileSize.y, blend);
		}
	}
}

void CTileMap::SetRowCol(UINT _Row, UINT _Col)
{
	m_Row = _Row; 
	m_Col = _Col;

	if (m_vecTileInfo.size() < m_Row * m_Col)
	{
		m_vecTileInfo.resize(m_Row * m_Col);
	}
	else if (m_Row * m_Col < m_vecTileInfo.size())
	{
		vector<tTileInfo> temp;
		temp.resize(m_Row * m_Col);
		m_vecTileInfo.swap(temp);
	}

	for (int i = 0; i < m_vecTileInfo.size(); ++i)
	{
		m_vecTileInfo[i].ImgIdx = 0;
		//int MaxTileImgCount = m_AtlasMaxCol * m_AtlasMaxRow;
		//m_vecTileInfo[i].ImgIdx = i % MaxTileImgCount;
	}
}



void CTileMap::SetAtlas(CTexture* _Atlas)
{
	m_Atlas = _Atlas;

	if (m_Atlas)
	{
		m_AtlasResolution = Vec2(m_Atlas->GetWidth(), m_Atlas->GetHeight());
	}
}

void CTileMap::SetAtlasTileSize(Vec2 _Size)
{
	m_AtlasTileSize = _Size;

	if (m_Atlas)
	{
		m_AtlasMaxCol = m_AtlasResolution.x / m_AtlasTileSize.x;
		m_AtlasMaxRow = m_AtlasResolution.y / m_AtlasTileSize.y;
	}
}

tTileInfo* CTileMap::GetTileInfo(UINT _Col, UINT _Row)
{
	if (m_Col <= _Col || m_Row <= _Row)
	{
		return nullptr;
	}

	return &m_vecTileInfo[_Row * m_Col + _Col];
}

void CTileMap::Save(const wstring& _strPath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _strPath.c_str(), L"wb");

	// 타일 행,렬 정보
	fwrite(&m_Row, sizeof(UINT), 1, File);
	fwrite(&m_Col, sizeof(UINT), 1, File);

	// 타일 1개의 크기
	fwrite(&m_TileSize, sizeof(Vec2), 1, File);
	
	// 타일이 참조하던 아틀라스 텍스쳐 정보
	SaveAssetRef(m_Atlas, File);
	fwrite(&m_AtlasTileSize, sizeof(Vec2), 1, File);

	// 각 타일이 어떤 이미지를 사용했는지
	fwrite(m_vecTileInfo.data(), sizeof(tTileInfo), m_Row * m_Col, File);

	fclose(File);
}

void CTileMap::Load(const wstring& _strPath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _strPath.c_str(), L"rb");


	// 타일 행,렬 정보
	fread(&m_Row, sizeof(UINT), 1, File);
	fread(&m_Col, sizeof(UINT), 1, File);
	SetRowCol(m_Row, m_Col);


	// 타일 1개의 크기
	fread(&m_TileSize, sizeof(Vec2), 1, File);

	// 타일이 참조하던 아틀라스 텍스쳐 정보
	m_Atlas = LoadTextureRef(File);
	if (nullptr != m_Atlas)
		SetAtlas(m_Atlas);

	fread(&m_AtlasTileSize, sizeof(Vec2), 1, File);
	SetAtlasTileSize(m_AtlasTileSize);

	// 각 타일이 어떤 이미지를 사용했는지
	fread(m_vecTileInfo.data(), sizeof(tTileInfo), m_Row * m_Col, File);

	fclose(File);
}