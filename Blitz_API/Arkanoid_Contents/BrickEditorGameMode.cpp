#include "PreCompiledFile.h"
#include "BrickEditorGameMode.h"

#include <EngineCore/Level.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>

#include "ContentsEnum.h"





// Ÿ����� ������ �� �ִ� ����
void BrickEditorGameMode::BeginPlay()
{
	Super::BeginPlay();

	FVector2D Size = { 57,26 };
	FIntPoint Num = { 10,8 };
	WallTileMap = GetWorld()->SpawnActor<BrickEditor>();
	WallTileMap->SetBricksHeight(210);//��ġ�ȸ����� ����� 
	WallTileMap->Create("Brick", Num, Size);
	WallTileMap->RandomBrick = 2;

	for (int y = 0; y < Num.Y; y++)
	{
		for (int x = 0; x < Num.X; x++)
		{
			int randomIndex = std::rand() % 3;//�Ϲݺ����� 5��

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
	// ���̵� Ű�Է�
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


	// ����ȭ,����,�ε�
	if (true == UEngineInput::GetInst().IsPress('M'))
	{
		UEngineSerializer _Ser;
		WallTileMap->Serialize(_Ser);
		// ������ ����Ʈ ��� �׳� �����ϸ� ������.
		UEngineDirectory Dir;

		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
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
