#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineSerializer.h>
#include "Brick.h"
#include <chrono>

enum BrickType
{
	Default,
	HPBrick,
	NotBreak,
};

// Brick의 개별 위치와 크기는 BrickSize, Scale, IndexToBrickLocation,
// SetComponentLocation 함수에서 확인
class ABrick : public ISerializObject
{
public:
	USpriteRenderer* SpriteRenderer;

	bool IsMove = true;
	int BrickType = -1;
	FVector2D Scale;
	FVector2D Pivot;
	int SpriteIndex;
	int HP = 400001;

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
		_Ser << HP;
	}
	// 직렬화 데이타 로드
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

class BrickEditor : public AActor, public ISerializObject
{
public:
	BrickEditor();
	~BrickEditor(){}
	//hp set
	void SetBrickHp(FIntPoint _Index, int _Hp);
	// sprite set
	void SetBrickSprite(FIntPoint _Index, std::string_view _Sprite, int _SpriteIndex);
	// brick type set (hp,sprite)
	void setBrickType(FIntPoint _Index, BrickType _Type);
	// 자료구조에 넣기,자료구조 크기조절,액터(벽돌더미)위치셋
	void Create(std::string_view _Sprite, FIntPoint _Count, FVector2D _BrickSize);
	// 위치,인덱스 정해서, SetBrickIndex호출 
	void SetBrickLocation(FVector2D _Location, int _SpriteIndex);
	// 오버로딩 SetBrickIndex호출-벡터
	void SetBrickIndex(FIntPoint _Index, int _SpriteIndex);
	// 개별 스프라이트 세팅들.
	void SetBrickIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex);
	// 벽돌의 위치를 계산하여 BrickPositions에 저장
	FVector2D IndexToBrickLocation(FIntPoint _Index);
	// 개별위치/벽돌크기 가 인덱스별 위치
	FIntPoint LocationToIndex(FVector2D _Location);
	// 인덱스는 벽돌x,y갯수보다 작고,0보다 커야한다
	bool IsIndexOver(FIntPoint _Index);

	// 데이터를 직렬화(압축)
	void Serialize(UEngineSerializer& _Ser);
	// 데이터를 복구(할때)
	void DeSerialize(UEngineSerializer& _Ser);

	FVector2D CheckCollision(class ABall* Ball, FIntPoint brickIndex);

	FVector2D CheckCollision(const FVector2D& playerPos, const FVector2D& playerSize, FIntPoint brickIndex);

	FVector2D GetBrickSize()
	{
		return BrickSize;
	}
	FIntPoint GetBrickCount()
	{
		return BrickCount;
	}

	void RemoveBlock(FIntPoint brickIndex);

	void SetBricksHeight(float _height)
	{
		Height = _height;
	}

	void SpawnFX(FVector2D _brickPos);

	void StartStage();
	float GetElapsedTime() const;

	int GetScore()const
	{
		return Score;
	}
	int RandomBrick = 5;

protected:

private:
	// 값형이니까 그냥 생겨버리는것.
	FIntPoint BrickCount;
	std::string SpriteName;
	FVector2D BrickSize;//벽돌 개별크기
	float Height = 0;
	FVector2D PlusPos;
	std::vector<std::vector<ABrick>> AllBricks;
	std::vector<std::vector<FVector2D>> BrickPositions;
	class Brick* BonusA;
	FVector2D brickPos;

	WhereIsBall ballPosition = WhereIsBall::BOTTOM;
	std::chrono::time_point<std::chrono::steady_clock> stageStartTime;
	float Duration;
	int Score = 0;


};


