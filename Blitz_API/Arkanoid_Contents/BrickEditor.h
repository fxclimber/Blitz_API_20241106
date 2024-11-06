#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineSerializer.h>
#include "Brick.h"

// Brick의 개별 위치와 크기는 BrickSize, Scale, IndexToBrickLocation,
// SetComponentLocation 함수에서 확인
class ABrick : public ISerializObject
{
public:
	// 이런 클래스를 저장할때
	// 내부 정부는 다 저장해야 한다.
	// 그중에서 쓸모없는게 누군가요?
	// 가상함수 테이블
	USpriteRenderer* SpriteRenderer;

	bool IsMove = true;
	int BrickType = -1;
	// 타일하나하나가 개별적인 크기를 가지고 있다면
	// 이걸로 직접 입력해주셔야 합니다.
	FVector2D Scale;
	FVector2D Pivot;
	int SpriteIndex;

	// 데이터를 직렬화(압축)
	void Serialize(UEngineSerializer& _Ser)
	{
		std::string SpriteName;
		if (nullptr != SpriteRenderer)
		{
			SpriteName = SpriteRenderer->GetCurSpriteName();
		}
		_Ser << SpriteName;
		_Ser << IsMove;
		_Ser << BrickType;
		_Ser << Scale;
		_Ser << Pivot;
		_Ser << SpriteIndex;
	}

	void DeSerialize(UEngineSerializer& _Ser)
	{
		std::string SpriteName;
		_Ser >> SpriteName;
		_Ser >> IsMove;
		_Ser >> BrickType;
		_Ser >> Scale;
		_Ser >> Pivot;
		_Ser >> SpriteIndex;
	}

};

// 사람들이 가장 먼저 원하는 기능.
// 플레이어가 어느 타일 위에 존재하는지 알고 싶어요.
class BrickEditor : public AActor, public ISerializObject
{
public:
	// constrcuter destructer
	BrickEditor(){}
	~BrickEditor(){}


	//             100, 100                  64 64 
	// 타일 이미지는 sprite 1개에서 
	void Create(std::string_view _Sprite, FIntPoint _Count, FVector2D _BrickSize);

	// 여기에서    이 인덱스에다가    이스프라이트 
	void SetBrickLocation(FVector2D _Location, int _SpriteIndex);

	void SetBrickIndex(FIntPoint _Index, int _SpriteIndex);
	// _SpriteScale : 벽돌 개별크기, 벽돌 개별위치 계산
	void SetBrickIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex);

	ABrick* GetBrickRef(FIntPoint _Index);
	ABrick* GetBrickRef(FVector2D _Location);

	FVector2D IndexToBrickLocation(FIntPoint _Index);

	FIntPoint LocationToIndex(FVector2D _Location);

	bool IsIndexOver(FIntPoint _Index);

	// 데이터를 직렬화(압축)
	void Serialize(UEngineSerializer& _Ser);
	// 데이터를 복구(할때)
	void DeSerialize(UEngineSerializer& _Ser);

	FVector2D CheckCollision(const FVector2D& playerPos, const FVector2D& playerSize, FIntPoint brickIndex);

	FVector2D GetBrickSize()
	{
		return BrickSize;
	}
	FIntPoint GetBrickCount()
	{
		return BrickCount;
	}
protected:

private:
	// 값형이니까 그냥 생겨버리는것.
	FIntPoint BrickCount;
	std::string SpriteName;
	FVector2D BrickSize;//벽돌 개별크기
	std::vector<std::vector<ABrick>> AllBricks;
};


