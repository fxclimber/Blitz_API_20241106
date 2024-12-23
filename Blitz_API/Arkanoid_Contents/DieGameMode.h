#pragma once
#include <EngineCore/GameMode.h>
#include <vector>
#include <EnginePlatform/EngineSound.h>


class DieGameMode : public AGameMode
{
public:

	DieGameMode();
	~DieGameMode(){}
	bool GetIsOpenLevel()
	{
		return IsOpenLevel;
	}

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;
	bool IsOpenLevel=false;


private:
	class AScore* score=nullptr;
	class AScore* time=nullptr;
	FVector2D UIPos = { 0,0 };
	class USpriteRenderer* SpriteMove = nullptr;
};

