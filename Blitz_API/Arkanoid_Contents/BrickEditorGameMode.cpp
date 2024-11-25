#include "PreCompiledFile.h"
#include "BrickEditorGameMode.h"
#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>
#include "ContentsEnum.h"
#include "TestGameMode.h"
#include "UI_Editor.h"
#include "Map_Die.h"

// 타일찍고 저장할 수 있는 레벨
void BrickEditorGameMode::BeginPlay()
{
	Super::BeginPlay();

	// ------------상단 리셋 메세지

	float center = 40.f;
	float height = 20.f;
	float smallFontsize = 20;
	float bigFontSize = 35;

	ALetter* letterReset = GetWorld()->SpawnActor<ALetter>();
	letterReset->SetActorLocation({ center,height });
	letterReset->SetTextSpriteName("Text_Letters1.png");
	letterReset->SetOrder(ERenderOrder::UI);
	letterReset->SetTextScale({ smallFontsize, smallFontsize });
	letterReset->SetText("reset");

	ALetter* letterResetAll = GetWorld()->SpawnActor<ALetter>();
	letterResetAll->SetActorLocation({ center + 120.f,height });
	letterResetAll->SetTextSpriteName("Text_Letters1.png");
	letterResetAll->SetOrder(ERenderOrder::UI);
	letterResetAll->SetTextScale({ smallFontsize, smallFontsize });
	letterResetAll->SetText("all");

	ALetter* letterResetR = GetWorld()->SpawnActor<ALetter>();
	letterResetR->SetActorLocation({ center + 212.f,height });
	letterResetR->SetTextSpriteName("Text_Letters1.png");
	letterResetR->SetOrder(ERenderOrder::UI);
	letterResetR->SetTextScale({ bigFontSize, bigFontSize });
	letterResetR->SetText("r");

	//----------------------------------------------------------------

	//SetActorLocation(UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize().Half());
	{
		//FVector2D Size = Globals::BrickSize; //{ 65,30 };
		FVector2D Size = { 57,26 };
		FIntPoint Num = { 11,23 };
		//int score = 0;
		Editor = GetWorld()->SpawnActor<BrickEditor>();
		Editor->Create("Brick", Num, Size);


		for (int y = 0; y < Num.X; y++)
		{
			for (int x = 0; x < Num.Y; x++)
			{
				Editor->SetBrickIndex({ y,x }, { 0, 0 }, Size, 4, 40001);
				Editor->setBrickType({ y,x }, BrickType::Default);
			}
		}
		Editor->SetActorLocation({80.f,300.f});
	}

	{
		UI_EditorLetters = GetWorld()->SpawnActor<UI_Editor>();
		UI_EditorLetters->SetActive(false);

		USpriteRenderer* SpriteRender = CreateDefaultSubObject<USpriteRenderer>();
		SpriteRender->SetOrder(ERenderOrder::SURF);
		SpriteRender->SetSprite("Text_Spacebar.png");

		FVector2D WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
		SpriteRender->SetComponentScale(WinSize);

		FVector2D MapScale = SpriteRender->SetSpriteScale(0.55f);
		//SpriteRender->SetComponentLocation(MapScale.Half());
		SpriteRender->SetComponentLocation({250.f,900.f});
	}


}


void BrickEditorGameMode::Tick(float _DeltaTime)
{

	Super::Tick(_DeltaTime);

	if (UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->ResetLevel<BrickEditorGameMode, Map_Die>("Editor");
	}




	ATestGameMode* playgamemode = GetWorld()->GetGameMode<ATestGameMode>();
	if (nullptr != playgamemode)
	{
		if (true == playgamemode->IsEditMode)
		{
			UI_EditorLetters->SetActive(true);
		}
	}


	if (UEngineInput::GetInst().IsDown('P'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Play");
	}

	if (UEngineInput::GetInst().IsDown('Q'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Tmp");
	}

	static bool IsSpaceBarPressed = false;
	static BrickType BrickType = BrickType::Default;

	if (UEngineInput::GetInst().IsPress(VK_SPACE))
	{
		IsSpaceBarPressed = true;
	}
	if (IsSpaceBarPressed)
	{
		if (UEngineInput::GetInst().IsPress('0'))
		{
			BrickType = BrickType::Default;
			IsSpaceBarPressed = false;
		}
		else if (UEngineInput::GetInst().IsPress('6'))
		{
			BrickType = BrickType::HPBrick;
			IsSpaceBarPressed = false;
		}
		else if (UEngineInput::GetInst().IsPress('5'))
		{
			BrickType = BrickType::NotBreak;
			IsSpaceBarPressed = false;
		}
	}
	if (true == UEngineInput::GetInst().IsPress(VK_LBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
		//ABrick* brick = GetWorld()->SpawnActor<ABrick>();		
		Editor->SetBrickLocation(MousePos, BrickType);
	}



	// 삭제
	if (true == UEngineInput::GetInst().IsPress(VK_RBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
		ABrick* BaseTile = Editor->GetBrickRef(MousePos);
		if (BaseTile != nullptr && BaseTile->SpriteRenderer != nullptr)
		{
			BaseTile->BrickType = -1;
			BaseTile->SpriteRenderer->Destroy();
			BaseTile->SpriteRenderer = nullptr;
		}
	}


	// 직렬화 저장
	if (true == UEngineInput::GetInst().IsPress('M'))
	{
		UEngineSerializer _Ser;
		Editor->Serialize(_Ser);
		// 순수한 바이트 덩어리 그냥 저장하면 끝난다.
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Data");
		std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
		UEngineFile NewFile = SaveFilePath;
		NewFile.FileOpen("wb");
		NewFile.Write(_Ser);
	}

	// 직렬화 로드
	if (true == UEngineInput::GetInst().IsPress('L'))
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
			return;
		}
		Dir.Append("Data");
		std::string SaveFilePath = Dir.GetPathToString() + "\\MapData.Data";
		UEngineFile NewFile = SaveFilePath;
		NewFile.FileOpen("rb");
		UEngineSerializer Ser;
		NewFile.Read(Ser);
		Editor->DeSerialize(Ser);
	}

	// 랜덤 사용 샘플
	if (true == UEngineInput::GetInst().IsPress('P'))
	{
		UEngineRandom Random;
		for (size_t i = 0; i < 10; i++)
		{
			int Value = Random.RandomInt(0, 100);
			//UEngineDebug::OutPutString(std::to_string(Value));
		}
	}


}