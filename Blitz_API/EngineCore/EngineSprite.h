#pragma once
#include <EnginePlatform/EngineWinImage.h>
#include <vector>
#include <EngineBase/EngineDebug.h>

// 설명 :
class UEngineSprite : public UObject
{
public:
	class USpriteData
	{
	public:
		// 이 이미지의
		UEngineWinImage* Image;
		// 여기서부터 xx 크기까지
		FTransform Transform;
		// 잘라서 쓰겠다.
	};

	// constrcuter destructer
	UEngineSprite();
	~UEngineSprite();

	void PushData(UEngineWinImage* Image, const FTransform& _Trans);

	USpriteData GetSpriteData(int _Index = 0)
	{
		if (_Index >= Data.size())
		{
			MSGASSERT("스프라이트의 인덱스를 오버하여 사용하려고 했습니다." + GetName());
		}

		return Data[_Index];
	}

	void ClearSpriteData()
	{
		Data.clear();
	}

protected:
	std::vector<USpriteData> Data;
};


