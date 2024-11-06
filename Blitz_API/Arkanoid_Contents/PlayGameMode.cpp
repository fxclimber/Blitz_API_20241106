#include "PreCompiledFile.h"
#include "playGameMode.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineCoreDebug.h>

#include "ContentsEnum.h"
#include "TileMapGameMode.h"
#include <EngineCore/Level.h>
#include <EngineBase/EngineFile.h>
#include <EngineBase/EngineDirectory.h>
#include <EngineBase/EngineRandom.h>

#include "Arkanoid_Contents/Ball.h"
#include "Map_Play.h"
#include "Player.h"
#include "Paddle.h"
#include "Brick.h"
#include "Ball.h"

#include <string>

#include "BrickEditor.h"


void APlayGameMode::BeginPlay()
{
	Super::BeginPlay();

	// 1. 카메라 사용안하기로 설정
	GetWorld()->SetCameraToMainPawn(false);


	// 2. 타일맵 객체 초기화
	{
		//GroundTileMap = GetWorld()->SpawnActor<ATileMap>();
	}

	{
		WallTileMap = GetWorld()->SpawnActor<BrickEditor>();
		WallTileMap->Create("Brick", { 10, 12 }, { 77, 38 });

		for (int y = 0; y < 12; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				//WallTileMap->SetTileIndex({ y,x }, { 0, 0 }, { 77, 38 }, 0);
			}
		}
	}
	// 3. 타일맵 데이타 로드
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

	// 플레이 게임모드에서 사용할 액터들 스폰
	// 맵(배경) , 플레이어(임시-본게임에선 삭제예정) , 
	// [중요] 패들(현재 "플레이어-벽돌충돌위치" 로직을, "볼-벽돌"관계로 변경하고,로직을 벽돌로 옮겨야함), 
	// [중요] 볼(현재 "볼의 "벽에 충돌하는""로직을 , 볼로 옮겨야함
	Map_Play* NewActor = GetWorld()->SpawnActor<Map_Play>();
	FVector2D brickSize = WallTileMap->GetBrickSize();
	FIntPoint size = brickSize.ConvertToPoint();
	FIntPoint brickCount = WallTileMap->GetBrickCount();

	// FIntPoint를 FVector2D로 변환하여 SetActorLocation에 전달
	FVector2D location((size.X * brickCount.X), (size.Y * brickCount.Y));
	FVector2D winSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize();
	float tilePos = (winSize.X - location.X ) / 2;
	WallTileMap->SetActorLocation(FVector2D(tilePos, 180.0f));


	Player = GetWorld()->SpawnActor<APlayer>();
	paddle = GetWorld()->SpawnActor<Paddle>();
	brick = GetWorld()->SpawnActor<Brick>();

	ball = GetWorld()->SpawnActor<ABall>();
	ball->SetActorLocation((paddle->GetActorLocation()) + ball->GetRender()->GetComponentScale()/2);
	//FVector2D ballInitVel = { 0.7f,-1.f };
	//ball->vel = ballInitVel;
}

void APlayGameMode::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	ball->Tick(_DeltaTime); // 공의 이동 및 반사 로직

	// 충돌 검사 및 반사 처리
	FVector2D normal = brick->CheckCollision(Player->GetTransform().Location, ball->GetRender()->GetComponentScale());
	if (normal != FVector2D(0.0f, 0.0f)) {
		ball->Reflect(normal);
	}

	// 디버그 출력 등 나머지 로직 유지






	// 4. 볼 반사로직 
	//{
	//// 4-1. 볼의 이동 경계값
	//int MaxTop = 122;
	//int MaxBottom = 1250;
	//int MaxLeft = 52;
	//int MaxRight = 948;
	//// 4-2. 볼의 위치,크기,경계에 닿았을때의 오차, 델타타임
	//FVector2D ballPos = ball->GetActorLocation();
	//FVector2D ballScale = ball->GetRender()->GetComponentScale();
	//float tolerance = 0.02f + ballScale.X / 2;
	//float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	//// 4-3. 볼이 벽 내부에서만 이동하도록 설정
	//if (MaxLeft < ballPos.X && ballPos.X < MaxRight && MaxTop < ballPos.Y && ballPos.Y < MaxBottom)
	//{
	//	// 왼쪽 및 오른쪽 벽에 닿았는지 확인
	//	if (ballPos.X <= MaxLeft + tolerance || ballPos.X >= MaxRight - tolerance)
	//	{
	//		ball->vel.X *= -1.0f; // X축 반전
	//	}
	//	// 위쪽 및 아래쪽 벽에 닿았는지 확인
	//	if (ballPos.Y <= MaxTop + tolerance || ballPos.Y >= MaxBottom - tolerance)
	//	{
	//		ball->vel.Y *= -1.0f; // Y축 반전
	//	}
	//}

	//}
	// 4-4. 볼 이동?? 
	//{
	//ball->MoveFunction(ball->vel);
	//UEngineDebug::CoreOutPutString("ballVel : " + ball->vel.ToString(), { 100,200 });// 4-5. 볼 속도로그

	//}

	// 4-5. 반사속도로 볼 이동?? 
	//{
	//FVector2D incoming = GetVectorForBallPos(ballEnum);
	//FVector2D normal(0.0f, 1.0f);
	//reflected = Reflect(incoming, normal);
	//UEngineDebug::CoreOutPutString("reflected : " + reflected.ToString(), FVector2D(100, 640));

	//}


	// 5.필요정보-디버그 출력 
	FVector2D playerPos = Player->GetTransform().Location;
	FVector2D brickPos = brick->GetTransform().Location;
	FVector2D brickSpriteScale = brick->GetRender()->GetComponentScale();

	UEngineDebug::CoreOutPutString("FPS : " + std::to_string(1.0f / _DeltaTime));
	//UEngineDebug::CoreOutPutString("PlayerPos : " + playerPos.ToString(), playerPos+ FVector2D(25, -80));
	//UEngineDebug::CoreOutPutString("brickPos : " + brickPos.ToString(), { 100,130 });

	// 6. "플레이어-벽돌"의 충돌 판정 로직
	// 6-1. 벽돌파츠 판정
	//{
	//	FVector2D HitResult = (playerPos-brickPos) / brickSpriteScale;
	//	if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1)
	//	{
	//		if(HitResult.X<HitResult.Y)//left,bottom
	//		{
	//			if (HitResult.X > 1- HitResult.Y)
	//			{
	//			UEngineDebug::CoreOutPutString("HitResult : BOTTOM" + HitResult.ToString(),FVector2D(100, 600));
	//			ballEnum = WhereIsBall::BOTTOM;
	//			}
	//			else
	//			{
	//			UEngineDebug::CoreOutPutString("HitResult : LEFT" + HitResult.ToString(), FVector2D(100, 600));
	//			ballEnum = WhereIsBall::LEFT;
	//			}
	//		}
	//		else if (HitResult.X > HitResult.Y)//right, top
	//		{
	//			if (HitResult.Y > 1 - HitResult.X)
	//			{
	//				UEngineDebug::CoreOutPutString("HitResult : RIGHT" + HitResult.ToString(), FVector2D(100, 600));
	//				ballEnum = WhereIsBall::RIGHT;
	//			}
	//			else
	//			{
	//				UEngineDebug::CoreOutPutString("HitResult : TOP" + HitResult.ToString(), FVector2D(100, 600));
	//				ballEnum = WhereIsBall::TOP;
	//			}
	//			
	//		}
	//		else
	//		{
	//			UEngineDebug::CoreOutPutString("HitResult : " + HitResult.ToString(), FVector2D(100, 600));
	//		}
	//	}
	//	else
	//	{
	//	UEngineDebug::CoreOutPutString("HitResult : " + HitResult.ToString(), FVector2D(100, 600));
	//	}

	//}

	// 맵이동 키입력
	if (true == UEngineInput::GetInst().IsDown('R'))
	{
		UEngineAPICore::GetCore()->OpenLevel("Ending");
	}
}


// 4-6. 볼의 반사벡터 계산함수
//FVector2D APlayGameMode::Reflect(const FVector2D& incoming, const FVector2D& normal)
//{
//	// 입사벡터와 법선벡터의 도트곱
//	float dotProduct = incoming.Dot(normal);
//	return incoming - normal * (2 * dotProduct);
//}

// 6-2. 벽돌의 충돌위치 결과 이넘들
//FVector2D APlayGameMode::GetVectorForBallPos(WhereIsBall position)
//{
//	switch (position) {
//	case WhereIsBall::TOP:
//		return FVector2D(0.0f, 1.0f); // 위쪽
//	case WhereIsBall::BOTTOM:
//		return FVector2D(0.0f, -1.0f); // 아래쪽
//	case WhereIsBall::LEFT:
//		return FVector2D(-1.0f, 0.0f); // 왼쪽
//	case WhereIsBall::RIGHT:
//		return FVector2D(1.0f, 0.0f); // 오른쪽
//	default:
//		return FVector2D(0.0f, 0.0f); // NONE 또는 예외처리
//	}
//}