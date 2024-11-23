#pragma once
#include <EngineCore/GameMode.h>
#include "BrickEditor.h"



class TmpGameMode : public AGameMode
{
public:
	TmpGameMode(){}
	~TmpGameMode(){}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;
private:
	BrickEditor* Editor = nullptr;
	std::string LoadFileName;

};

