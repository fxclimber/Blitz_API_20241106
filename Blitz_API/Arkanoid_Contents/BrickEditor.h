#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineBase/EngineSerializer.h>
#include "Brick.h"
#include <chrono>
#include <EnginePlatform/EngineSound.h>


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
	// 게임모드에서 호출하는 충돌체크함수
	FVector2D CheckCollision(class ABall* Ball, FIntPoint brickIndex);
	// 인덱스는 범위내 확인용으로만.
	FVector2D CheckCollision(const FVector2D& playerPos, const FVector2D& playerSize, FIntPoint brickIndex);

	FVector2D GetBrickSize()
	{
		return BrickSize;
	}
	FIntPoint GetBrickCount()
	{
		return BrickCount;
	}
	// 인덱스,컴포넌트(렌더러)체크하고, 실버블록효과,hp==0일때 destroy
	void RemoveBlock(FIntPoint brickIndex);
	// 에디터의 액터위치x는 자동으로 중심에 오게 되있으므로, y는 의도적으로 조절하기위함
	void SetBricksHeight(float _height)
	{
		Height = _height;
	}
	// 기존에 게임모드-비긴플레이에서 스폰하던것을 조건별로 스폰가능하게 함수로 추출
	void SpawnFX(FVector2D _brickPos);
	// 플레이시간 계산용 시간함수 
	float GetElapsedTime() const;
	// 게임모드에서 ui에 넘길 점수
	int GetScore()const
	{
		return Score;
	}
	// 맵저장시 디폴트벽돌의 랜덤갯수 다르게 주기위함.
	int RandomBrick = 5;

protected:

private:
	FIntPoint BrickCount;
	std::string SpriteName;
	FVector2D BrickSize;//벽돌 개별크기

	float Height = 0;// 벽돌 세부위치y 조절용
	FVector2D PlusPos;//액터 위치 조정용

	std::vector<std::vector<ABrick>> AllBricks;
	std::vector<std::vector<FVector2D>> BrickPositions;// 이넘 (위치) 저장용
	// b눌러서 보너스볼 생성
	class Brick* BonusA;
	FVector2D brickPos;// 벽돌개개의 위치

	WhereIsBall ballPosition = WhereIsBall::BOTTOM;
	std::chrono::time_point<std::chrono::steady_clock> stageStartTime;
	int Score = 0;

	USoundPlayer BallSoundPlayer;

};


