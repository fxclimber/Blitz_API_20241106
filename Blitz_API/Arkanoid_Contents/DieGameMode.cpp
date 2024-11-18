#include "PreCompiledFile.h"
#include "DieGameMode.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
//#include <EngineBase/EngineDebug.h>
#include <EngineCore/EngineCoreDebug.h>

DieGameMode::DieGameMode()
{
}

void DieGameMode::BeginPlay()
{
}

void DieGameMode::Tick(float _DeltaTime)
{
	if (UEngineInput::GetInst().IsDown('C'))
	{
		//IsOpenLevel == true;
		UEngineAPICore::GetCore()->OpenLevel("Test");

	}

}
