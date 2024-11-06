#pragma once
#include <EnginePlatform/EngineWinImage.h>
#include <vector>
#include <EngineBase/EngineDebug.h>

// ���� :
class UEngineSprite : public UObject
{
public:
	class USpriteData
	{
	public:
		// �� �̹�����
		UEngineWinImage* Image;
		// ���⼭���� xx ũ�����
		FTransform Transform;
		// �߶� ���ڴ�.
	};

	// constrcuter destructer
	UEngineSprite();
	~UEngineSprite();

	void PushData(UEngineWinImage* Image, const FTransform& _Trans);

	USpriteData GetSpriteData(int _Index = 0)
	{
		if (_Index >= Data.size())
		{
			MSGASSERT("��������Ʈ�� �ε����� �����Ͽ� ����Ϸ��� �߽��ϴ�." + GetName());
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


