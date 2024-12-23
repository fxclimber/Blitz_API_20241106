#include "PreCompiledFile.h"
#include "BrickEditor.h"

#include <EngineBase/EngineRandom.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include "ContentsEnum.h"
#include "Ball.h"
#include "Score.h"


BrickEditor* BrickEditor::Create(std::string_view _Sprite, FIntPoint _Count, FVector2D _BrickSize)
{
	SpriteName = _Sprite;
	BrickSize = _BrickSize;
	BrickCount = _Count;
	AllBricks.resize(_Count.Y);

	for (size_t y = 0; y < AllBricks.size(); y++)
	{
		AllBricks[y].resize(_Count.X);;
	}

	// 타일의 위치는 월드로서의 타일맵 * 인덱스
	float WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize().Half().X;
	float AllSpriteWidth = (_BrickSize.X * static_cast<float>(_Count.X)) / 2;
	float gap = WinSize - AllSpriteWidth;
	PlusPos = { gap, Height };
	SetActorLocation(PlusPos);
	return this;
}

FVector2D BrickEditor::IndexToBrickLocation(FIntPoint _Index)
{
	//인덱스를 기반으로 벽돌의 위치를 계산
	return FVector2D(_Index.X * BrickSize.X, _Index.Y * BrickSize.Y);
}


FIntPoint BrickEditor::LocationToIndex(FVector2D _Location)
{

	FVector2D Location = _Location / BrickSize;

	int LocationX = static_cast<int>(std::floor(Location.X));
	int LocationY = static_cast<int>(std::floor(Location.Y));

	return FIntPoint(LocationX, LocationY);
}


void BrickEditor::SetBrickLocation(FVector2D _Location, BrickType _Type)
{
	FVector2D TilePos = _Location - GetActorLocation();//개별위치-액터위치

	FIntPoint Point = LocationToIndex(TilePos);

	if (true == IsIndexOver(Point))
	{
		return;
	}

	SetBrickIndex(Point, 0, 1);
	setBrickType(Point, _Type);
}

bool BrickEditor::IsIndexOver(FIntPoint _Index)
{
	if (0 > _Index.X)
	{
		return true;
	}

	if (0 > _Index.Y)
	{
		return true;
	}

	if (BrickCount.X - 1 < _Index.X)
	{
		return true;
	}

	if (BrickCount.Y - 1 < _Index.Y)
	{
		return true;
	}

	return false;
}

ABrick* BrickEditor::GetBrickRef(FIntPoint _Index)
{
	if (true == IsIndexOver(_Index))
	{
		return nullptr;
	}
	return &AllBricks[_Index.Y][_Index.X];
}

ABrick* BrickEditor::GetBrickRef(FVector2D _Location)
{
	FVector2D TilePos = _Location - GetActorLocation();//개별위치-액터위치

	FIntPoint Point = LocationToIndex(TilePos);
	return GetBrickRef(Point);
}

void BrickEditor::SetBrickIndex(FIntPoint _Index, int _SpriteIndex, int _HP)
{
	SetBrickIndex(_Index, { 0,0 }, BrickSize, _SpriteIndex, _HP);
}


void BrickEditor::SetBrickIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex, int _HP)
{
	// 인덱스가 범위를 벗어나면 리턴
	if (IsIndexOver(_Index))
	{
		return;
	}

	// 벽돌의 위치를 계산하여 BrickPositions에 저장
	FVector2D TileLocation = IndexToBrickLocation(_Index);
	if (BrickPositions.size() <= _Index.Y) {
		BrickPositions.resize(_Index.Y + 1); // 행 크기 조정
	}
	if (BrickPositions[_Index.Y].size() <= _Index.X) {
		BrickPositions[_Index.Y].resize(_Index.X + 1); // 열 크기 조정
	}
	BrickPositions[_Index.Y][_Index.X] = FVector2D(TileLocation.X + (PlusPos.X), TileLocation.Y + Height);

	// 벽돌의 SpriteName 설정
	if (SpriteName.empty())
	{
		SpriteName = "DefaultSprite"; // 기본 스프라이트 이름 설정 (필요에 따라 수정 가능)
	}

	// 벽돌 크기 설정 (초기화되지 않은 경우)
	if (BrickSize == FVector2D{ 0.0f, 0.0f })
	{
		BrickSize = _SpriteScale; // 기본 벽돌 크기를 개별 스케일로 설정
	}

	// 스프라이트 렌더러가 없는 경우 새로 생성
	if (AllBricks[_Index.Y][_Index.X].SpriteRenderer == nullptr)
	{
		USpriteRenderer* NewSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		NewSpriteRenderer->SetComponentScale(BrickSize);
		AllBricks[_Index.Y][_Index.X].SpriteRenderer = NewSpriteRenderer;
	}

	// 스프라이트 설정
	USpriteRenderer* FindSprite = AllBricks[_Index.Y][_Index.X].SpriteRenderer;
	FindSprite->SetSprite(SpriteName, _SpriteIndex); // 지정된 스프라이트 인덱스 적용
	FindSprite->SetComponentScale(_SpriteScale); // 스프라이트 크기 설정
	FindSprite->SetOrder(_Index.Y); // Y 인덱스를 기준으로 렌더 순서 설정

	// 위치 설정
	FVector2D BrickCenterOffset = BrickSize.Half() + _Pivot;
	FindSprite->SetComponentLocation(TileLocation + BrickCenterOffset);

	// AllBricks의 개별 벽돌 속성 설정
	AllBricks[_Index.Y][_Index.X].Pivot = _Pivot;
	AllBricks[_Index.Y][_Index.X].Scale = _SpriteScale;
	AllBricks[_Index.Y][_Index.X].HP = _HP;
}



void BrickEditor::Serialize(UEngineSerializer& _Ser)
{
	_Ser << BrickCount;
	_Ser << BrickSize;
	_Ser << SpriteName;
	_Ser << AllBricks;
	//_Ser << Score;

}

void BrickEditor::DeSerialize(UEngineSerializer& _Ser)
{
	_Ser >> BrickCount;
	_Ser >> BrickSize;
	_Ser >> SpriteName;
	//_Ser >> Score;

	std::vector<std::vector<ABrick>> LoadTiles;
	_Ser >> LoadTiles;

	// 데이터만 읽어왔을뿐이지.
	Create(SpriteName, BrickCount, BrickSize);

	for (int y = 0; y < LoadTiles.size(); y++)
	{
		for (int x = 0; x < LoadTiles[y].size(); x++)
		{
			if (LoadTiles[y][x].BrickType == -1)
			{
				continue;
			}

			SetBrickIndex({ x, y }, LoadTiles[y][x].Pivot, LoadTiles[y][x].Scale, 0, LoadTiles[y][x].HP);
			setBrickType({ x, y }, static_cast<BrickType>(LoadTiles[y][x].BrickType));
		}
	}

}

FVector2D BrickEditor::CheckCollision(ABall* Ball, FIntPoint brickIndex)
{
	FVector2D Normal = CheckCollision(Ball->GetActorLocation(), Ball->GetRender()->GetComponentScale(), brickIndex);
	Ball->Reflect(Normal);
	return Normal;
}


FVector2D BrickEditor::CheckCollision(const FVector2D& ballPos, const FVector2D& ballSize, FIntPoint brickIndex)
{
	// 벽돌 인덱스 범위가 유효하지 않으면 충돌 없음 반환
	if (IsIndexOver(brickIndex))
	{
		return FVector2D(0.0f, 0.0f);
	}

	FVector2D HitResult;
	
	// 전체 벽돌 순회하며 충돌 검사
	for (int y = 0; y < AllBricks.size(); ++y)
	{
		for (int x = 0; x < AllBricks[y].size(); ++x)
		{
			auto& brick = AllBricks[y][x];

			// SpriteRenderer가 없는 벽돌은 건너뜀
			if (brick.SpriteRenderer == nullptr)
			{
				continue;
			}

			// 벽돌 위치와 크기 가져오기
			brickPos = BrickPositions[y][x];
			FVector2D brickSize = BrickSize;

			// 충돌 감지 로직
			HitResult = (ballPos - brickPos) / brickSize;


			// 충돌 조건 유지
			if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
				if (HitResult.X < HitResult.Y) {
					if (HitResult.X > 1 - HitResult.Y) {
						RemoveBlock(FIntPoint(x, y)); // 실제 충돌한 벽돌의 인덱스를 전달
						//SpawnFX(brickPos);
						ballPosition = WhereIsBall::BOTTOM;
						return FVector2D(0.0f, -1.0f); // 아래쪽 반사 벡터
					}
					else {
						RemoveBlock(FIntPoint(x, y)); // 실제 충돌한 벽돌의 인덱스를 전달
						//SpawnFX(brickPos);
						ballPosition = WhereIsBall::LEFT;
						return FVector2D(-1.0f, 0.0f); // 왼쪽 반사 벡터
					}
				}
				else if (HitResult.X > HitResult.Y) {
					if (HitResult.Y > 1 - HitResult.X) {
						RemoveBlock(FIntPoint(x, y)); // 실제 충돌한 벽돌의 인덱스를 전달
						//SpawnFX(brickPos);
						ballPosition = WhereIsBall::RIGHT;
						return FVector2D(1.0f, 0.0f); // 오른쪽 반사 벡터
					}
					else {
						RemoveBlock(FIntPoint(x, y)); // 실제 충돌한 벽돌의 인덱스를 전달
						//SpawnFX(brickPos);
						ballPosition = WhereIsBall::TOP;
						return FVector2D(0.0f, 1.0f); // 위쪽 반사 벡터
					}
				}
			}
		}
	}

	return FVector2D(0.0f, 0.0f); // 충돌 없음
}



void BrickEditor::RemoveBlock(FIntPoint brickIndex)
{
	if (true == IsIndexOver(brickIndex))
	{
		MSGASSERT("인덱스가 오버되었습니다.");
	}

	if (nullptr == AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer)
	{
		return;
	}

	AllBricks[brickIndex.Y][brickIndex.X].HP -= 1;
	Score += 1;

	// HPBrick일 경우에만 "brick_silver_shine" 효과 적용
	if (AllBricks[brickIndex.Y][brickIndex.X].HP>1 && AllBricks[brickIndex.Y][brickIndex.X].HP<3)
	{
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSprite("brick_silver_shine");
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetOrder(ERenderOrder::FX);
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSpriteScale(1.3f);
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->CreateAnimation("brick_silver_shine", "brick_silver_shine", 0, 9, 0.12f, true);
		// Animation* curanimation = nullptr;
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->ChangeAnimation("brick_silver_shine");
	}

	if (AllBricks[brickIndex.Y][brickIndex.X].HP == 0)
	{

		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSprite("exp");
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetOrder(ERenderOrder::FX);
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSpriteScale(0.9f);
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->CreateAnimation("exp", "exp", 1, 8, 0.09f, false);
		
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->ChangeAnimation("exp");
		DeathCount++;
		SpawnFX(brickPos);
		BallSoundPlayer = UEngineSound::Play("BlockCrashBall.wav");
	}
}




void BrickEditor::SpawnFX(FVector2D _brickPos)
{
	UEngineRandom Random;

	int Value = Random.RandomInt(0, 100);
	int fmod = Value%12;//단지 스폰 빈도확률

	OutputDebugString(std::to_string(Value).c_str());

	//if (fmod == 0)
	if(nullptr==BonusA)
	{
		BonusA = GetWorld()->SpawnActor<Brick>();
		BonusA->SetActorLocation(_brickPos);
	}
}

void BrickEditor::SetBrickHp(FIntPoint _Index, int _Hp)
{
	AllBricks[_Index.Y][_Index.X].HP = _Hp;
}

void BrickEditor::SetBrickSprite(FIntPoint _Index, std::string_view _Sprite, int _SpriteIndex)
{
	// 인덱스가 범위를 벗어나면 리턴
	if (IsIndexOver(_Index))
	{
		return;
	}

	AllBricks[_Index.Y][_Index.X].SpriteRenderer->SetSprite(_Sprite, _SpriteIndex);
	
}


BrickEditor::BrickEditor()
{
	BonusA = nullptr;
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	stageStartTime = std::chrono::steady_clock::now();
}

void BrickEditor::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	for (size_t y = 0; y < AllBricks.size(); ++y)
	{
		for (size_t x = 0; x < AllBricks[y].size(); ++x)
		{
			if (0 < AllBricks[y][x].HP)
			{
				continue;
			}

			if (AllBricks[y][x].SpriteRenderer && AllBricks[y][x].SpriteRenderer->IsCurAnimationEnd())
			{
				AllBricks[y][x].SpriteRenderer->Destroy();
				AllBricks[y][x].SpriteRenderer = nullptr;
			}
		}
	}


}
void BrickEditor::setBrickType(FIntPoint _Index, BrickType _Type)
{
	//std::random
	int randomIndex = (std::rand() % RandomBrick) ; // 인덱스가 0~4까지

	ABrick* Brick = GetBrickRef(_Index);

	switch (_Type)
	{
	case Default:
		Brick->BrickType = _Type;
		Brick->SpriteRenderer->SetSprite("Brick", 3);
		Brick->HP = 1;
		break;
	case HPBrick:
		Brick->BrickType = _Type;
		Brick->SpriteRenderer->SetSprite("Brick", 6);
		Brick->HP = 3;
		break;
	case NotBreak:
		Brick->BrickType = _Type;
		Brick->SpriteRenderer->SetSprite("Brick", 5);
		Brick->HP = 400001;
		break;
	default:
		break;
	}
}

float BrickEditor::GetElapsedTime() const
{
	auto Now = std::chrono::steady_clock::now();
	auto Duration = std::chrono::duration<float>(Now - stageStartTime);  // 초 단위로 변환
	return Duration.count();  // float 타입으로 반환
}

