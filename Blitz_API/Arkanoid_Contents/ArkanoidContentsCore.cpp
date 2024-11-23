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

	// Resources 파일들의 폴더경로
	if (false == Dir.MoveParentToDirectory(ImageRes))
	{
		MSGASSERT("리소스 폴더를 찾지 못했습니다.");
		return;
	}

	// 모든 파일 다 얻어낸다음
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
			MSGASSERT("리소스 폴더를 찾지 못했습니다.");
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
	// 자르는 작업 <- 텍스쳐 모두 로딩되야함
	UImageManager::GetInst().CuttingSprite("Player_Right.png", { 128, 128 });

	//UI
	UImageManager::GetInst().CuttingSprite("Text_Num.png", { 64, 64 });

	//----------------폴더 애니들 ----------------------------------------
	// 폴더- silver block fx
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

	// 폴더- 보너스 아이템 
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
		// 타일셋 로드
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


	//-------------------------------근간이 되는 부분들 ---------------------------------
	// 윈도우 
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("Blitz_Window");
	// 이거 꼭 호출해줘야 합니다.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, { 770, 1000 });

	// CreateLevel
	UEngineAPICore::GetCore()->CreateLevel<ATestGameMode, Map_Play>("Play");
	UEngineAPICore::GetCore()->CreateLevel<DieGameMode, Map_Die>("Die");
	UEngineAPICore::GetCore()->CreateLevel<BrickEditorGameMode, Map_Die>("Editor");
	UEngineAPICore::GetCore()->CreateLevel<TmpGameMode, Map_Die>("Tmp");

	// OpenLevel
	UEngineAPICore::GetCore()->OpenLevel("Play");

}
