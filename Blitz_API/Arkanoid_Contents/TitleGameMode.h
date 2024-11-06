#pragma once
#include <EngineCore/GameMode.h>

// Ό³Έν :
class ATitleGameMode : public AGameMode
{
public:
	// constrcuter destructer
	ATitleGameMode();
	~ATitleGameMode();

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

private:

};

