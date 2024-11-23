#include "PreCompiledFile.h"
#include "ArkanoidContentsCore.h"
#include <EngineCore/EngineAPICore.h>
#include <EnginePlatform/EngineSound.h>
#include <EngineBase/EngineDirectory.h>

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/ImageManager.h>

#include "Map_Play.h"
#include "Map_Die.h"
#include "TestGameMode.h"
#include "DieGameMode.h"
#include "BrickEditorGameMode.h"
#include "TmpGameMode.h"

void ArkanoidContentsCore::BeginPlay()
{
	//------- image load -------------------------------------------------
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
	//------- sound load -------------------------------------------------
	{
		UEngineDirectory Dir;
		if (false == Dir.MoveParentToDirectory("Resources"))
		{
			MSGASSERT("���ҽ� ������ ã�� ���߽��ϴ�.");
			return;
		}
		Dir.Append("Sound");
		std::vector<UEngineFile> ImageFiles = Dir.GetAllFile();
		for (size_t i = 0; i < ImageFiles.size(); i++)
		{
			std::string FilePath = ImageFiles[i].GetPathToString();
			UEngineSound::Load(FilePath);
		}

	}


	//--------------------------------------------------------
	// �ڸ��� �۾� <- �ؽ��� ��� �ε��Ǿ���
	UImageManager::GetInst().CuttingSprite("Player_Right.png", { 128, 128 });

	//UI
	UImageManager::GetInst().CuttingSprite("Text_Num.png", { 64, 64 });

	//----------------���� �ִϵ� ----------------------------------------
	// ����- silver block fx
	{
		{
			//powerup_laser
			UEngineDirectory powerup_laser;
			powerup_laser.MoveParentToDirectory(ImageRes + "//Arkanoid_Items//Sprite_Ani");
			powerup_laser.Append("brick_silver_shine");
			UImageManager::GetInst().LoadFolder(powerup_laser.GetPathToString());
		}
	}
		{
			////edge_top
			//UEngineDirectory edge_top;
			//edge_top.MoveParentToDirectory(ImageRes + "//Arkanoid_Items//Sprite_Static");
			//edge_top.Append("edge_top");
			//UImageManager::GetInst().LoadFolder(edge_top.GetPathToString());
		}

	// ����- ���ʽ� ������ 
	{
		{
			//powerup_laser
			UEngineDirectory powerup_laser;
			powerup_laser.MoveParentToDirectory(ImageRes + "//Arkanoid_Items//Sprite_Ani");
			powerup_laser.Append("powerup_laser");
			UImageManager::GetInst().LoadFolder(powerup_laser.GetPathToString());
		}
		{
			//powerup_catch
			UEngineDirectory powerup_catch;
			powerup_catch.MoveParentToDirectory(ImageRes + "//Arkanoid_Items//Sprite_Ani");
			powerup_catch.Append("powerup_catch");
			UImageManager::GetInst().LoadFolder(powerup_catch.GetPathToString());
		}
		{
			//powerup_duplicate
			UEngineDirectory powerup_duplicate;
			powerup_duplicate.MoveParentToDirectory(ImageRes + "//Arkanoid_Items//Sprite_Ani");
			powerup_duplicate.Append("powerup_duplicate");
			UImageManager::GetInst().LoadFolder(powerup_duplicate.GetPathToString());
		}
		{
			//powerup_expand
			UEngineDirectory powerup_expand;
			powerup_expand.MoveParentToDirectory(ImageRes + "//Arkanoid_Items//Sprite_Ani");
			powerup_expand.Append("powerup_expand");
			UImageManager::GetInst().LoadFolder(powerup_expand.GetPathToString());
		}
		{
			//powerup_life
			UEngineDirectory powerup_life;
			powerup_life.MoveParentToDirectory(ImageRes + "//Arkanoid_Items//Sprite_Ani");
			powerup_life.Append("powerup_life");
			UImageManager::GetInst().LoadFolder(powerup_life.GetPathToString());
		}
		{
			//powerup_slow
			UEngineDirectory powerup_slow;
			powerup_slow.MoveParentToDirectory(ImageRes + "//Arkanoid_Items//Sprite_Ani");
			powerup_slow.Append("powerup_slow");
			UImageManager::GetInst().LoadFolder(powerup_slow.GetPathToString());
		}
		{
			//powerup_warp
			UEngineDirectory powerup_warp;
			powerup_warp.MoveParentToDirectory(ImageRes + "//Arkanoid_Items//Sprite_Ani");
			powerup_warp.Append("powerup_warp");
			UImageManager::GetInst().LoadFolder(powerup_warp.GetPathToString());
		}

	}

	{
		// Ÿ�ϼ� �ε�
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory(ImageRes + "//Arkanoid_Items");
		Dir.Append("Sprite_Static//Brick");
		UImageManager::GetInst().LoadFolder(Dir.GetPathToString());
	}

	{
		// brick exp FX
		UEngineDirectory exp;
		exp.MoveParentToDirectory(ImageRes);
		exp.Append("exp");

		UImageManager::GetInst().LoadFolder(exp.GetPathToString());

	}


	//-------------------------------�ٰ��� �Ǵ� �κе� ---------------------------------
	// ������ 
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("Blitz_Window");
	// �̰� �� ȣ������� �մϴ�.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, { 770, 1000 });

	// CreateLevel
	UEngineAPICore::GetCore()->CreateLevel<ATestGameMode, Map_Play>("Play");
	UEngineAPICore::GetCore()->CreateLevel<DieGameMode, Map_Die>("Die");
	UEngineAPICore::GetCore()->CreateLevel<BrickEditorGameMode, Map_Die>("Editor");
	UEngineAPICore::GetCore()->CreateLevel<TmpGameMode, Map_Die>("Tmp");

	// OpenLevel
	UEngineAPICore::GetCore()->OpenLevel("Play");

}
