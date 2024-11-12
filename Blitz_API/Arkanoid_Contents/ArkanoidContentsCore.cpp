#include "PreCompiledFile.h"
#include "ArkanoidContentsCore.h"
#include <EngineCore/EngineAPICore.h>

#include <EngineBase/EngineDirectory.h>

#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineFile.h>
#include <EngineCore/ImageManager.h>

#include "Map_Play.h"
#include "TestGameMode.h"


void ArkanoidContentsCore::BeginPlay()
{

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

	// 자르는 작업 <- 텍스쳐 모두 로딩되야함
	UImageManager::GetInst().CuttingSprite("Player_Right.png", { 128, 128 });

	//UI
	UImageManager::GetInst().CuttingSprite("Text_Num.png", { 64, 64 });


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

	// 윈도우 
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("Blitz_Window");
	// 이거 꼭 호출해줘야 합니다.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, { 770, 1000 });

	// CreateLevel
	UEngineAPICore::GetCore()->CreateLevel<ATestGameMode, Map_Play>("Test");

	UEngineAPICore::GetCore()->OpenLevel("Test");

}
