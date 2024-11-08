#pragma once
#include <EngineCore/GameMode.h>
#include "ATileMap.h"



class APlayGameMode : public AGameMode
{
public:
	APlayGameMode(){}
	~APlayGameMode() {}
	
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	//FVector2D Reflect(const FVector2D& incoming, const FVector2D& normal);
	FVector2D reflected = { 0,0 };

	WhereIsBall ballEnum = WhereIsBall::NONE;
	//FVector2D GetVectorForBallPos(WhereIsBall position);

	// 사용할 액터들
	class APlayer* Player = nullptr;
	class APaddle* paddle = nullptr;
	class Brick* brick = nullptr;
	class ABall* ball = nullptr;
protected:

private:
	// 타일맵 포인터들
	class BrickEditor* WallTileMap = nullptr;

};

