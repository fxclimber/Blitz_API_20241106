#pragma once
#include <EngineCore/GameMode.h>
#include "ATileMap.h"

// ���� :
class ATileMapGameMode : public AGameMode
{
public:
	// constrcuter destructer
	ATileMapGameMode();
	~ATileMapGameMode();


protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

private:
	ATileMap* WallTileMap = nullptr;
	ATileMap* GroundTileMap = nullptr;

	std::string LoadFileName;

};

