#pragma once
#include <EngineCore/GameMode.h>
#include <vector>
#include <EnginePlatform/EngineSound.h>

class TitleGameMode : public AGameMode
{
public:
	TitleGameMode();
	~TitleGameMode(){}


protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;



};

