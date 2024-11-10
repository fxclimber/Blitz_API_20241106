#pragma once
#include <list>
#include <vector>
#include <map>
#include <string>

#include <EnginePlatform/EngineWinImage.h>
#include "EngineSprite.h"

// 설명 :
class UImageManager
{
public:
	// constrcuter destructer
	~UImageManager();

	// delete Function
	UImageManager(const UImageManager& _Other) = delete;
	UImageManager(UImageManager&& _Other) noexcept = delete;
	UImageManager& operator=(const UImageManager& _Other) = delete;
	UImageManager& operator=(UImageManager&& _Other) noexcept = delete;

	static UImageManager& GetInst()
	{
		static UImageManager Inst;
		return Inst;
	}

	// 경로를 통채로 넣어주면 자동으로 파일명이 키네임이 될겁니다.
	// 편의성 함수
	void Load(std::string_view Path);

	void LoadFolder(std::string_view Path);

	// 앞으로 네가 이미지를 로드하고 찾을때
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

	// void LoadDirectory(std::string_view _KeyName, std::string_view Path);

protected:

private:
	UImageManager();

	// UEngineWinImage*
	// 애니메이션 이란 뭐지?
	// 여러개의 프레임이 돌아가는것.
	// UEngineWinImage* 이걸 쪼개는 
	std::map<std::string, UEngineWinImage*> Images;

	std::map<std::string, UEngineSprite*> Sprites;

};

