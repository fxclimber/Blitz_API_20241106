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
#include "Map_Play.h"
#include "TestGameMode.h"
#include "Map_Die.h"
#include "Letter.h"

DieGameMode::DieGameMode()
{
	score = nullptr;
}

void DieGameMode::BeginPlay()
{
	// ------------상단 리셋 메세지

	float center = 160.f;
	float height = 450.f;
	float smallFontsize = 20;
	float bigFontSize = 35;

	ALetter* letterFinalScore = GetWorld()->SpawnActor<ALetter>();
	letterFinalScore->SetActorLocation({ center,height });
	letterFinalScore->SetTextSpriteName("Text_Letters1.png");
	letterFinalScore->SetOrder(ERenderOrder::UI);
	letterFinalScore->SetTextScale({ smallFontsize, smallFontsize });
	letterFinalScore->SetText("FinalScore");

	ALetter* letterElapsedTime = GetWorld()->SpawnActor<ALetter>();
	letterElapsedTime->SetActorLocation({ center,height+ 200.f});
	letterElapsedTime->SetTextSpriteName("Text_Letters1.png");
	letterElapsedTime->SetOrder(ERenderOrder::UI);
	letterElapsedTime->SetTextScale({ smallFontsize, smallFontsize });
	letterElapsedTime->SetText("ElapsedTime");

	//ALetter* letterResetR = GetWorld()->SpawnActor<ALetter>();
	//letterResetR->SetActorLocation({ center + 212.f,height });
	//letterResetR->SetTextSpriteName("Text_Letters1.png");
	//letterResetR->SetOrder(ERenderOrder::UI);
	//letterResetR->SetTextScale({ bigFontSize, bigFontSize });
	//letterResetR->SetText("r");


	//----------------------------------------------------------------

	GetWorld()->SetCameraToMainPawn(false);

	//----------------------------------------------------------------
	if (true == ATestGameMode::GameClear)
	{
		if (nullptr == SpriteMove)
		{
			SpriteMove = CreateDefaultSubObject<USpriteRenderer>();
			SpriteMove->SetOrder(ERenderOrder::Bricks);
			SpriteMove->SetSprite("Text_edit_P.png");
			SpriteMove->SetComponentLocation({ 160.f,120.f });
			SpriteMove->SetPivot({ 0,0 });
			SpriteMove->SetComponentScale({ 230,38 });
			SpriteMove->SetAlphafloat(0.75f);
			ATestGameMode::GameClear = false;
		}
	}
	else
	{
		return;
	}


	//UI _score
	score = GetWorld()->SpawnActor<AScore>();
	score->SetActorLocation({ 385,500 });
	score->SetTextSpriteName("Text_Num.png");
	score->SetOrder(ERenderOrder::UI);
	score->SetTextScale({ 32, 32 });
	//UI _time
	time = GetWorld()->SpawnActor<AScore>();
	time->SetActorLocation({ 385,700 });
	time->SetTextSpriteName("Text_Num.png");
	time->SetOrder(ERenderOrder::UI);
	time->SetTextScale({ 32, 32 });


	// score
	score->SetValue(AScore::ScoreUI,8);//여기에 점수넣기
	score->SetFinalValue(AScore::FinalScoreUI);
	// time
	time->SetValue(AScore::ElapsedTime,4);//여기에 점수넣기
	time->SetFinalValue(AScore::FinalElapsedTime);

}

void DieGameMode::Tick(float _DeltaTime)
{
	if (UEngineInput::GetInst().IsDown('P'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Editor");
	}

	if (UEngineInput::GetInst().IsDown('R'))
	{
		//UEngineAPICore::GetCore()->ResetLevel<DieGameMode, Map_Die>("Die");
		UEngineAPICore::GetCore()->ResetLevel<ATestGameMode, Map_Play>("Play");

	}


}
