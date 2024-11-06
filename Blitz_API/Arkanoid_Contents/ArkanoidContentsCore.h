#pragma once
#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineWindow.h>

class ArkanoidContentsCore : public UContentsCore
{
public:
	ArkanoidContentsCore();
	~ArkanoidContentsCore();


protected:
	void BeginPlay() override;
	void Tick() override;

private:
	ULevel* TitleLevel=nullptr;

};

