#include "PreCompiledFile.h"
#include "DieGameMode.h"
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
//#include <EngineBase/EngineDebug.h>
#include <EngineCore/EngineCoreDebug.h>
#include "Score.h"
#include "Arkanoid_Contents/ContentsEnum.h"
#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>
#include "ContentsEnum.h"

DieGameMode::DieGameMode()
{
}

void DieGameMode::BeginPlay()
{
	//UI
	score = GetWorld()->SpawnActor<AScore>();
	score->SetActorLocation({ 385,500 });
	score->SetTextSpriteName("Text_Num.png");
	score->SetOrder(ERenderOrder::UI);
	score->SetTextScale({ 32, 32 });

	GetWorld()->SetCameraToMainPawn(false);

	// score
	score->SetValue(AScore::ScoreUI);//여기에 점수넣기
	score->SetFinalValue(AScore::FinalScoreUI);
	//UEngineAPICore::GetCore()->OpenLevel("Die");

}

void DieGameMode::Tick(float _DeltaTime)
{
	if (UEngineInput::GetInst().IsDown('P'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Editor");
	}



}
