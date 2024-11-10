#include "PreCompiledFile.h"
#include "BrickEditorGameMode.h"

#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>

#include "ContentsEnum.h"





// 타일찍고 저장할 수 있는 레벨
void BrickEditorGameMode::BeginPlay()
{
	Super::BeginPlay();

	FVector2D Size = { 57,26 };
	FIntPoint Num = { 10,8 };
	WallTileMap = GetWorld()->SpawnActor<BrickEditor>();
	WallTileMap->SetBricksHeight(210);//위치안맞으면 지울것 
	WallTileMap->Create("Brick", Num, Size);
	WallTileMap->RandomBrick = 2;

	for (int y = 0; y < Num.Y; y++)
	{
		for (int x = 0; x < Num.X; x++)
		{
			int randomIndex = std::rand() % 3;//일반벽돌색 5개

			WallTileMap->SetBrickIndex({ x,y }, { 0, 0 }, Size, 1);
			WallTileMap->setBrickType({ x,y }, BrickType::Default);
		}
	}

	for (int x = 0; x < Num.X; x++)
	{
		//WallTileMap->setBrickType({ x,3 }, BrickType::HPBrick);
	}
	for (int x = 0; x < Num.X; x++)
	{
		//WallTileMap->setBrickType({ x,2 }, BrickType::NotBreak);
	}

}

void BrickEditorGameMode::Tick(float _DeltaTime)
{
	// 맵이동 키입력
	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Ending");
	}

	if (true == UEngineInput::GetInst().IsPress(VK_LBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
		WallTileMap->SetBrickLocation(MousePos, 5);
	}
	if (true == UEngineInput::GetInst().IsPress(VK_RBUTTON))
	{
		FVector2D MousePos = UEngineAPICore::GetCore()->GetMainWindow().GetMousePos();
		WallTileMap->SetBrickLocation(MousePos, 6);
	}


	// 직렬화,저장,로드
	if (true == UEngineInput::GetInst().IsPress('M'))
	{
		UEngineSerializer _Ser;
		WallTileMap->Serialize(_Ser);
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


		WallTileMap->DeSerialize(Ser);

	}


}
