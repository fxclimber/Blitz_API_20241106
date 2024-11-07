#include "PreCompiledFile.h"
#include "ArkanoidContentsCore.h"
#include <EngineCore/EngineAPICore.h>

#include <EngineBase/EngineDirectory.h>

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/ImageManager.h>

#include "EndGameMode.h"
#include "TitleGameMode.h"
#include "PlayGameMode.h"
#include "Player.h"
#include "Map_Play.h"
#include "Map_Title.h"
#include "Map_Ending.h"

#include "TileMapGameMode.h"
#include "BrickEditorGameMode.h"

#include "TestGameMode.h"

ArkanoidContentsCore::ArkanoidContentsCore()
{
}

ArkanoidContentsCore::~ArkanoidContentsCore()
{
}

void ArkanoidContentsCore::BeginPlay()
{
	UEngineDirectory Dir;

	std::string ImageRes = "Resources//Images";

	// Resources ���ϵ��� �������
	if (false == Dir.MoveParentToDirectory(ImageRes))
	{
		MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
		return;
	}

	// ��� ���� �� ������
	std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();

	for (size_t i = 0; i < ImageFiles.size(); i++)
	{
		std::string FilePath = ImageFiles[i].GetPathToString();
		UImageManager::GetInst().Load(FilePath);
	}

	// �ڸ��� �۾� <- �ؽ��� ��� �ε��Ǿ���
	UImageManager::GetInst().CuttingSprite("Player_Right.png", { 128, 128 });

	// ���� ���� �ִ�
	{
		UEngineDirectory BombDir;
		BombDir.MoveParentToDirectory(ImageRes);
		BombDir.Append("bomb");

		UImageManager::GetInst().LoadFolder(BombDir.GetPathToString());
	}


	// ����- �е�ִ� �׽�Ʈ
	{
		//paddle_materialize
		UEngineDirectory paddle_materialize;
		paddle_materialize.MoveParentToDirectory(ImageRes);
		paddle_materialize.Append("paddle_materialize");

		UImageManager::GetInst().LoadFolder(paddle_materialize.GetPathToString());
	}

	{
		// Ÿ�ϼ� �ε�
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory(ImageRes + "//Arkanoid_Items");
		Dir.Append("Sprite_Static//Brick");

		UImageManager::GetInst().LoadFolder(Dir.GetPathToString());

	}

	{
		// brickFX
		UEngineDirectory exp;
		exp.MoveParentToDirectory(ImageRes);
		exp.Append("exp");

		UImageManager::GetInst().LoadFolder(exp.GetPathToString());

	}

	// ������ 
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("Blitz_Window");
	// �̰� �� ȣ������� �մϴ�.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, { 770, 1000 });

	// CreateLevel
	UEngineAPICore::GetCore()->CreateLevel<APlayGameMode,Map_Play>("Play");
	UEngineAPICore::GetCore()->CreateLevel<ATitleGameMode, Map_Title>("Title");
	UEngineAPICore::GetCore()->CreateLevel<AEndGameMode, Map_Ending>("Ending");

	UEngineAPICore::GetCore()->CreateLevel<ATestGameMode, Map_Play>("Test");

	//Ÿ�ϸ� �׽�Ʈ�� 
	//UEngineAPICore::GetCore()->CreateLevel<ATileMapGameMode, AActor>("Tile");
	// UEngineAPICore::GetCore()->CreateLevel<BrickEditorGameMode, AActor>("Tile");

	UEngineAPICore::GetCore()->OpenLevel("Test");


	// UEngineAPICore::GetCore()->OpenLevel("");

}

void ArkanoidContentsCore::Tick()
{
	// ���� ���� �뵵�� ã�� ���ؼ� ������� �ʴ� �Լ��Դϴ�.
}