#include "pch.h"
#include "CTexture.h"

#include "CEngine.h"
#include "CEngine.h"

CTexture::CTexture()
	: m_hBitmap(nullptr)
	, m_hDC(nullptr)
	, m_BitInfo{}
{
}

CTexture::~CTexture()
{
	DeleteDC(m_hDC);
	DeleteObject(m_hBitmap);
}

int CTexture::Load(const wstring& _FilePath)
{
	wchar_t szExt[50] = {};
	_wsplitpath_s(_FilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);

	if ( !wcscmp(szExt, L".bmp") || !wcscmp(szExt, L".BMP"))
	{
		// 지정된 경로로부터 비트맵 이미지를 메모리에 로딩하고 핸들값을 받아둠
		m_hBitmap = (HBITMAP)LoadImage(nullptr, _FilePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		// 로딩이 정상적으로 되었는지 비트맵 핸들값을 확인
		if (nullptr == m_hBitmap)
		{
			DWORD err = GetLastError();
			wchar_t buffer[256] = {};
			swprintf_s(buffer, 256, L"텍스쳐 로딩 실패\n에러 번호 : %d", err);
			MessageBox(nullptr, buffer, L"에러 발생", MB_OK);

			return E_FAIL;
		}		
	}

	else if (!wcscmp(szExt, L".png") || !wcscmp(szExt, L".PNG"))
	{
		ULONG_PTR gdiplustoken = 0;
		GdiplusStartupInput input = {};
		GdiplusStartup(&gdiplustoken, &input, nullptr);		

		Image* pImage = Image::FromFile(_FilePath.c_str());
		Bitmap* pBitmap = (Bitmap*)pImage->Clone();
		delete pImage;

		pBitmap->GetHBITMAP(Color(0, 0, 0, 0), &m_hBitmap);
	}

	// 비트맵을 선택해서 연결해줄 DC 를 생성
	m_hDC = CreateCompatibleDC(CEngine::Get()->GetMainDC());

	// 비트맵과 생성한 DC 를 연결
	DeleteObject(SelectObject(m_hDC, m_hBitmap));

	// 메모리에 로딩한 비트맵의 정보를 받아온다.
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitInfo);

	return S_OK;
}

int CTexture::Create(UINT _Width, UINT _Height)
{
	HDC DC = CEngine::Get()->GetMainDC();

	// BackBuffer 생성
	m_hBitmap = CreateCompatibleBitmap(DC, _Width, _Height);

	// BackBuffer 용 DC 생성
	m_hDC = CreateCompatibleDC(DC);

	// BackBufferDC 가 BackBuffer 비트맵을 렌더링 타겟으로 지정하고, 
	// DC 가 원래 들고있던 1픽셀짜리 비트맵을 폐기한다.
	DeleteObject(SelectObject(m_hDC, m_hBitmap));

	// 메모리에 로딩한 비트맵의 정보를 받아온다.
	GetObject(m_hBitmap, sizeof(BITMAP), &m_BitInfo);

	return S_OK;
}
