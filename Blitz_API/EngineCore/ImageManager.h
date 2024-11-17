#pragma once
#include <list>
#include <vector>
#include <map>
#include <string>

#include <EnginePlatform/EngineWinImage.h>
#include "EngineSprite.h"

class UImageManager
{
public:
	~UImageManager();


	static UImageManager& GetInst()
	{
		static UImageManager Inst;
		return Inst;
	}

	// 경로를 통채로 넣어주면 자동으로 파일명이 키네임이
	void Load(std::string_view Path);

	void LoadFolder(std::string_view Path);

	// 실제로드는 여기에서 처리
	void Load(std::string_view _KeyName, std::string_view Path);

	void LoadFolder(std::string_view _KeyName, std::string_view Path);



	void CuttingSprite(std::string_view _KeyName, int _X, int _Y);

	void CuttingSprite(std::string_view _KeyName, FVector2D _CuttingSize);

	void CuttingSprite(std::string_view _NewSpriteName, std::string_view _Image, FVector2D _CuttingSize);

	// 신동민 함수
	void CreateCutSprite(std::string_view _SearchKeyName, std::string_view _NewSpriteKeyName, FVector2D _StartPos, FVector2D _CuttingSize, FVector2D _XYOffSet, UINT _Xcount, UINT _ImageCount);


	bool IsLoadSprite(std::string_view _KeyName);
	UEngineSprite* FindSprite(std::string_view _KeyName);
	UEngineWinImage* FindImage(std::string_view _KeyName);


protected:

private:
	UImageManager();

	std::map<std::string, UEngineWinImage*> Images;

	std::map<std::string, UEngineSprite*> Sprites;

};

