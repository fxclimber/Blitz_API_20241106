#pragma once
#include <EngineCore/GameMode.h>


class AEndGameMode : public AGameMode
{
public:
	AEndGameMode();
	~AEndGameMode();

	void BeginPlay()override;
	void Tick(float _DeltaTime) override;


protected:

private:


};

