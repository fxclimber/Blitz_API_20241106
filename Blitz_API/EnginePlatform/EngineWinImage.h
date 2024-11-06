#pragma once
#include <Windows.h>
#include <EngineBase/EngineMath.h>
#include <EngineBase/Object.h>

// 이녀석이 이미지 역할을 대신할 겁니다.
// 이녀석은 당연히 로드도 됩니다.

// 설명 : API 이번 랜더링 구조의 핵심 클래스
//       랩핑 클래스 winapi에서 우리한테 여러가지 랜더링 함수들을 제공하는데
//       핵심 자료형이 HDC야 엔진 프로그래머 입장에서 이걸 수정하거나 망치면
//       private으로 막고 싶다.
//       UEngineWinImage => 내부에 Has a로 HDC
//       ImageDC 가 만들어지지 않은 HDC는 의미가 없다.
class UEngineWinImage : public UObject
{
public:
	// constrcuter destructer
	UEngineWinImage();
	~UEngineWinImage();

	// delete Function
	UEngineWinImage(const UEngineWinImage& _Other) = delete;
	UEngineWinImage(UEngineWinImage&& _Other) noexcept = delete;
	UEngineWinImage& operator=(const UEngineWinImage& _Other) = delete;
	UEngineWinImage& operator=(UEngineWinImage&& _Other) noexcept = delete;

	// void Load(std::string_view _Path);
	HDC GetDC()
	{
		return ImageDC;
	}

	// 이미 만들어진 DC를 통해서 이미지를 만드는 타입
	void Create(HDC _DC)
	{
		ImageDC = _DC;
	}

	void Create(UEngineWinImage* _TargetImage, FVector2D _Scale);

	// bit블릿은 크기 조절아됩니다. 
	void CopyToBit(UEngineWinImage* _TargetImage,
		const FTransform& _Trans);

	// 트랜스 페어런츠 블릿을 사용한다.
	// 색깔제외도 되고 이미지 일부분만 처리하는 것도 된다.
	// 이미지의 어느 부분을 그릴지 정확하게 표현하기 위해서
	// 트랜스 폼이 하나더 필요합니다.
	void CopyToTrans(UEngineWinImage* _TargetImage,
		const FTransform& _RenderTrans,
		const FTransform& _LTImageTrans,
		UColor _Color = UColor(255, 0, 255, 0));

	void Load(UEngineWinImage* _TargetImage, std::string_view _Path);

	// 이미지 크기를 리턴
	FVector2D GetImageScale() const
	{
		return { Info.bmWidth, Info.bmHeight };
	}

protected:

private:
	// BMP 이미지 파일 그 자체에요
	// FVector2D Size;
	HDC ImageDC = nullptr;
	HBITMAP hBitMap = nullptr;

	// 이미지 정보 얻어오기
	BITMAP Info;
};

