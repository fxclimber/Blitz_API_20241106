#pragma once
#include <EngineCore/GameMode.h>
#include "BrickEditor.h"


class BrickEditorGameMode : public AGameMode
{
public:
	BrickEditorGameMode(){}
	~BrickEditorGameMode(){}

protected:
	void BeginPlay() override;

	void Tick(float _DeltaTime) override;

private:
	BrickEditor* WallTileMap = nullptr;
	std::string LoadFileName;

};

