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

	// 폴더 낱장 애니
	{
		UEngineDirectory BombDir;
		BombDir.MoveParentToDirectory(ImageRes);
		BombDir.Append("bomb");

		UImageManager::GetInst().LoadFolder(BombDir.GetPathToString());
	}


	// 폴더- 패들애니 테스트
	{
		//paddle_materialize
		UEngineDirectory paddle_materialize;
		paddle_materialize.MoveParentToDirectory(ImageRes);
		paddle_materialize.Append("paddle_materialize");

		UImageManager::GetInst().LoadFolder(paddle_materialize.GetPathToString());
	}

	{
		// 타일셋 로드
		UEngineDirectory Dir;
		Dir.MoveParentToDirectory(ImageRes + "//Arkanoid_Items");
		Dir.Append("Sprite_Static//Brick");

		UImageManager::GetInst().LoadFolder(Dir.GetPathToString());

	}



	// 윈도우 
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowTitle("Blitz_Window");
	// 이거 꼭 호출해줘야 합니다.
	UEngineAPICore::GetCore()->GetMainWindow().SetWindowPosAndScale({ 0, 0 }, { 770, 1000 });

	// CreateLevel
	UEngineAPICore::GetCore()->CreateLevel<APlayGameMode,Map_Play>("Play");
	UEngineAPICore::GetCore()->CreateLevel<ATitleGameMode, Map_Title>("Title");
	UEngineAPICore::GetCore()->CreateLevel<AEndGameMode, Map_Ending>("Ending");
	//타일맵 테스트용 
	//UEngineAPICore::GetCore()->CreateLevel<ATileMapGameMode, AActor>("Tile");
	UEngineAPICore::GetCore()->CreateLevel<BrickEditorGameMode, AActor>("Tile");


	UEngineAPICore::GetCore()->OpenLevel("Play");

}

void ArkanoidContentsCore::Tick()
{
	// 지금 전혀 용도를 찾지 못해서 사용하지 않는 함수입니다.
}