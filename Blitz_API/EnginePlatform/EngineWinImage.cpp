#include "PreCompiledFile.h"
#include "EngineWinImage.h"
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EnginePath.h>
#include <EngineBase/EngineString.h>

// GDI Plus 용 헤더
#include <objidl.h>
#include <gdiplus.h>

// $(WindowsSDK_LibraryPath_x64) <= 여기 경로안에 들어있는 라이브러리 입니다.

// 이미지용 window기본 라이브러리가 2가지가 있습니다.
// BMP 확장용 라이브러리
#pragma comment(lib, "Msimg32.lib")

// PNG 를 통한 window 네이티브 그래픽 확장용 라이브러리
#pragma comment(lib, "Gdiplus.lib")

UEngineWinImage::UEngineWinImage()
{
}

UEngineWinImage::~UEngineWinImage()
{
	// 윈도우 만들어준 메모리라도 릭은 안되겠지만
	// 왠만하면 내가 명시적으로 지워주는것을 선호합니다.
	if (nullptr != hBitMap)
	{
		DeleteObject(hBitMap);
		hBitMap = nullptr;
	}

	if (nullptr != ImageDC)
	{
		DeleteDC(ImageDC);
		ImageDC = nullptr;
	}

}

void UEngineWinImage::Create(UEngineWinImage* _TargetImage, FVector2D _Scale)
{
	if (nullptr == _TargetImage)
	{
		MSGASSERT("Main windowDC를 넣지않고 이미지를 생성하려고 했습니다");
		return;
	}

	// HDC 아무것도 없곳에서
	// HDC 넣어달라고 하는데 window HDC
	// int X; 만들려는 빈 이미지의 x크기
	// int Y; 만들려는 빈 이미지의 y크기

	// 아무것도 없는곳에 
	// HDC hdc 타겟 이 이미지를 만들어서 그리려고 하는 타겟
	// int cx 만들려는 이미지의 크기
	// int cy 만들려는 이미지의 크기



	HBITMAP NewBitmap = static_cast<HBITMAP>(CreateCompatibleBitmap(_TargetImage->GetDC(), _Scale.iX(), _Scale.iY()));

	// 이미지 그자체
	// 이미지를 수정할수 있는 권한 HDC
	// 이미지의 데이터 권한 HBITMAP
	// HDC<=>HBITMAP 이 합쳐져서 이미지와 이미지를 수정하는 권한이 연결되는 개념

	// 붓
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());

	// 윈도우가 정하기를 붓과 도화지를 한쌍으로 안엮으면 못그림.
	// 붓을 만들기만 해도  1, 1 이미지랑 연결해 놓는다. window 

	// 붓과 도화지를 연결하는 작업을 거쳐야 한다.
	HBITMAP OldBitMap = static_cast<HBITMAP>(SelectObject(NewImageDC, NewBitmap));
	// OldBitMap 1, 1 크기의 이미지
	// 가만히 내버려두면 leck
	DeleteObject(OldBitMap);

	hBitMap = NewBitmap;
	ImageDC = NewImageDC;

	GetObject(hBitMap, sizeof(BITMAP), &Info);
}

void UEngineWinImage::CopyToBit(UEngineWinImage* _TargetImage, const FTransform& _Trans)
{
	if (nullptr == _TargetImage)
	{
		MSGASSERT("복사받을 대상이 존재하지 않습니다");
	}

	HDC CopyDC = ImageDC;
	HDC TargetDC = _TargetImage->ImageDC;
	// CopyDC => TargetDC에 카피하겠다.
	// 윈도우는 이런 HDC간의 Copy를 위해서 
	// 카피용 함수들을 몇개 준비했고 그중에서
	// 가장 기본적인 함수가 바로 Bitblt

	//HDC hdc, 카피 받을 대상 <= 여기에다가 복사해라
	//int x, <= 위치x
	//int y, <= 위치y
	//int cx, <= 크기x
	//int cy, <= 크기x
	// HDC hdcSrc, 카피할 이미지
	// int x1, 이미지 시작점 0
	// int y1, 
	// rop => 카피 옵션

	FVector2D LeftTop = _Trans.CenterLeftTop();
	FVector2D RightBot = _Trans.CenterRightBottom();

	// 이미지 
	BitBlt(
		TargetDC,
		LeftTop.iX(),
		LeftTop.iY(),
		_Trans.Scale.iX(),
		_Trans.Scale.iY(),
		CopyDC,
		0,
		0,
		SRCCOPY);

	// SRCCOPY 카피할때 

	FVector2D Vector;
}

void UEngineWinImage::CopyToTrans(UEngineWinImage* _TargetImage, const FTransform& _RenderTrans, const FTransform& _LTImageTrans, UColor _Color /*= UColor(255, 0, 255, 255)*/)
{
	// _RenderTrans 액터의 트랜스폼

	// _LTImageTrans 이미지 쪼갠 크기가

	HDC CopyDC = ImageDC;
	HDC TargetDC = _TargetImage->ImageDC;

	//_In_ HDC hdcDest, 여기에다가 카피해라
	//_In_ int xoriginDest, 그려지는 위치
	//_In_ int yoriginDest, 그려지는 위치
	//_In_ int wDest, 그려지는 크기
	//_In_ int hDest, 그려지는 크기
	//_In_ HDC hdcSrc, 복사될 이미지 => 여러분들이 로드한 이미지
	//_In_ int xoriginSrc, 이미지 이부분 
	//_In_ int yoriginSrc, int X
	//_In_ int wSrc,
	//_In_ int hSrc,
	//_In_ UINT crTransparent

	FVector2D LeftTop = _RenderTrans.CenterLeftTop();

	TransparentBlt(
		TargetDC,
		LeftTop.iX(),
		LeftTop.iY(),
		_RenderTrans.Scale.iX(),
		_RenderTrans.Scale.iY(),
		CopyDC,
		_LTImageTrans.Location.iX(),
		_LTImageTrans.Location.iY(),
		_LTImageTrans.Scale.iX(),
		_LTImageTrans.Scale.iY(),
		_Color.Color
	);
}

void UEngineWinImage::Load(UEngineWinImage* _TargetImage, std::string_view _Path)
{
	// Png랑 Bmp일때 로드하는 방식이 달라진다.
	// Png로는 윈도우의 기본 이미지가 아니라서.
	// BMP는 쉽게 로딩되지만 PNG는 조금더 코드를 쳐야 한다.
	// 그래픽스는 결국 WinApi

	// Png를 로드해서 Bmp로 변환하는 과정을 거치는 개념에 가깝다.
	// 하지만 결국 로드하는 함수만 다를뿐이지
	// 로드가 끝났다면 결국
	// class Color
	// {
	//    char R, G, B, 255;
	// }
	// Color Arr[??][??]

	// 로드가 끝나면 window의 관할이다. 

	// 언리얼이 그렇게 합니다.
	// Window.png => WINDOW.PNG
	// WIndow.png
	// WIndow.Png
	// WIndow.pNg
	// WIndow.pnG

	UEnginePath Path = _Path;

	std::string UpperExt = UEngineString::ToUpper(Path.GetExtension());

	HBITMAP NewBitmap = nullptr;

	if (".PNG" == UpperExt)
	{
		// 로드까지만 GDI Plus를 이용 랜더링은 
		// WInapi함수를 사용하는 개념으로 간다./

		// GDIPLus용 핸들을 표현할때 
		ULONG_PTR gidplustoken = 0;

		// GDI plus를 사용하기 위한 인풋
		Gdiplus::GdiplusStartupInput StartupInput;
		Gdiplus::GdiplusStartup(&gidplustoken, &StartupInput, nullptr);


		std::wstring WidePath = UEngineString::AnsiToUnicode(_Path);

		// 경로 넣어주면 이미지 로딩해주는 함수
		Gdiplus::Image* pImage = Gdiplus::Image::FromFile(WidePath.c_str());

		// 복사본을 생성하고 거기에서 bitmap 부분을 뽑아내는 방식
		Gdiplus::Bitmap* pBitMap = reinterpret_cast<Gdiplus::Bitmap*>(pImage->Clone());

		Gdiplus::Status stat = pBitMap->GetHBITMAP(Gdiplus::Color(255, 255, 0, 255), &NewBitmap);

		if (Gdiplus::Status::Ok != stat)
		{
			MSGASSERT("Png 이미지 로드에 실패했습니다." + std::string(_Path));
			return;
		}

		// 
		delete pBitMap;
		delete pImage;
	}
	else if (".BMP" == UpperExt)
	{
		HANDLE NewHandle = LoadImageA(nullptr, _Path.data(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		NewBitmap = reinterpret_cast<HBITMAP>(NewHandle);
	}

	if (nullptr == NewBitmap)
	{
		MSGASSERT("이미지 로딩에 실패했습니다");
		return;
	}

	// 붓
	HDC NewImageDC = CreateCompatibleDC(_TargetImage->GetDC());

	HBITMAP OldBitMap = static_cast<HBITMAP>(SelectObject(NewImageDC, NewBitmap));
	DeleteObject(OldBitMap);

	hBitMap = NewBitmap;
	ImageDC = NewImageDC;

	GetObject(hBitMap, sizeof(BITMAP), &Info);
}