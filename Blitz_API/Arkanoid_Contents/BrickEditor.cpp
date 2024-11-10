#include "PreCompiledFile.h"
#include "BrickEditor.h"

#include <EnginePlatform/EngineInput.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/EngineCoreDebug.h>
#include "ContentsEnum.h"
#include "Ball.h"



void BrickEditor::Create(std::string_view _Sprite, FIntPoint _Count, FVector2D _BrickSize)
{
	SpriteName = _Sprite;
	BrickSize = _BrickSize;
	BrickCount = _Count;

	AllBricks.resize(_Count.Y);

	for (size_t y = 0; y < AllBricks.size(); y++)
	{
		AllBricks[y].resize(_Count.X);;
	}

	// 타일맵의 핵심 0, 0을 두가지를 정해야 한다.
	// 타일로서의 0,0이 어디야.
	// 타일의 위치는 월드로서의 타일맵 * 인덱스가 된다.
	float WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize().Half().X;
	//float WinSize = Globals::WinSize.X;//터지냐....왜........
	float AllSpriteWidth = (_BrickSize.X * static_cast<float>(_Count.X)) / 2;
	float gap = WinSize - AllSpriteWidth;
	PlusPos = { gap, Height };
	SetActorLocation(PlusPos);
}

FVector2D BrickEditor::IndexToBrickLocation(FIntPoint _Index)
{
	//인덱스를 기반으로 벽돌의 위치를 계산
	return FVector2D(_Index.X * BrickSize.X, _Index.Y * BrickSize.Y);
}

FIntPoint BrickEditor::LocationToIndex(FVector2D _Location)
{
	FVector2D Location = _Location / BrickSize;

	return FIntPoint(Location.iX(), Location.iY());
}

void BrickEditor::SetBrickLocation(FVector2D _Location, int _SpriteIndex)
{
	FVector2D TilePos = _Location - GetActorLocation();

	FIntPoint Point = LocationToIndex(TilePos);

	if (true == IsIndexOver(Point))
	{
		return;
	}

	SetBrickIndex(Point, _SpriteIndex);
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

void BrickEditor::SetBrickIndex(FIntPoint _Index, int _SpriteIndex)
{
	SetBrickIndex(_Index, { 0,0 }, BrickSize, _SpriteIndex);
}

// _SpriteScale : 벽돌 개별크기, 벽돌 개별위치 계산
//void BrickEditor::SetBrickIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex)
//{
//	if (true == IsIndexOver(_Index))
//	{
//		return;
//	}
//
//	// 미리 다 만들지 않고
//	// 지금 스프라이트 랜더러를 만든다.
//	if (nullptr == AllBricks[_Index.Y][_Index.X].SpriteRenderer)
//	{
//		USpriteRenderer* NewSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
//		NewSpriteRenderer->SetComponentScale(BrickSize);
//		AllBricks[_Index.Y][_Index.X].SpriteRenderer = NewSpriteRenderer;
//	}
//
//	// 스프라이트를 저장해 놨으므로.	// 그 이름으로 세팅해주면 된다.
//	USpriteRenderer* FindSprite = AllBricks[_Index.Y][_Index.X].SpriteRenderer;
//	FindSprite->SetSprite(SpriteName, _SpriteIndex);
//
//	FVector2D TileLocation = IndexToBrickLocation(_Index) + PlusPos;
//	FindSprite->SetComponentScale(_SpriteScale);
//	// 아래쪽에 있을수록 랜더링이 나중에 된다.
//	FindSprite->SetOrder(_Index.Y);
//
//	AllBricks[_Index.Y][_Index.X].SpriteRenderer->SetComponentLocation(TileLocation + BrickSize.Half() + _Pivot);
//	AllBricks[_Index.Y][_Index.X].Pivot = _Pivot;
//	AllBricks[_Index.Y][_Index.X].Scale = _SpriteScale;//개별 벽돌의 스케일을 저장
//	AllBricks[_Index.Y][_Index.X].SpriteIndex = _SpriteIndex;
//}


void BrickEditor::SetBrickIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex)
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
	AllBricks[_Index.Y][_Index.X].SpriteIndex = _SpriteIndex;
}




// 특정 위치에 해당하는 벽돌 참조를 얻기
ABrick* BrickEditor::GetBrickRef(FVector2D _Location)
{
	FIntPoint Point = LocationToIndex(_Location);

	return GetBrickRef(Point);
}

// 인덱스를 기반으로 특정위치의 벽돌포인터 리턴 
ABrick* BrickEditor::GetBrickRef(FIntPoint _Index)
{
	if (true == IsIndexOver(_Index))
	{
		return nullptr;
	}

	return &AllBricks[_Index.Y][_Index.X];
}





void BrickEditor::Serialize(UEngineSerializer& _Ser)
{
	_Ser << BrickCount;
	_Ser << BrickSize;
	_Ser << SpriteName;
	_Ser << AllBricks;

}

void BrickEditor::DeSerialize(UEngineSerializer& _Ser)
{
	_Ser >> BrickCount;
	_Ser >> BrickSize;
	_Ser >> SpriteName;

	std::vector<std::vector<ABrick>> LoadTiles;
	_Ser >> LoadTiles;

	// 데이터만 읽어왔을뿐이지.
	Create(SpriteName, BrickCount, BrickSize);

	for (int y = 0; y < LoadTiles.size(); y++)
	{
		for (int x = 0; x < LoadTiles[y].size(); x++)
		{
			SetBrickIndex({ x, y }, LoadTiles[y][x].Pivot, LoadTiles[y][x].Scale, LoadTiles[y][x].SpriteIndex);
		}
	}

}

FVector2D BrickEditor::CheckCollision(ABall* Ball, FIntPoint brickIndex)
{
	FVector2D Normal = CheckCollision(Ball->GetActorLocation(), Ball->GetRender()->GetComponentScale(), brickIndex);
	Ball->Reflect(Normal);
	return Normal;
}

//FVector2D BrickEditor::CheckCollision(const FVector2D& playerPos, const FVector2D& playerSize, FIntPoint brickIndex)
//{
//	// 벽돌의 위치와 크기를 가져오기 위해 brickIndex를 사용하여 AllBricks에서 접근
//	if (true == IsIndexOver(brickIndex))
//	{
//		return FVector2D(0.0f, 0.0f); // 충돌 없음
//	}
//	//FVector2D thisPos = GetTransform().Location;
//	auto& brick = AllBricks[brickIndex.Y][brickIndex.X];
//	FVector2D brickPos = IndexToBrickLocation(brickIndex);// -thisPos;
//	BrickSize = brick.Scale; // 개별 벽돌의 크기
//
//	// 0          0 
//	// AllBricks[y][x] => index 0, 0 => fvector 0, 0
//	// AllBricks[y][x] => index 1, 0 => fvector 64, 0
//
//	// 충돌체크고 뭐고 다 할수 있다.
//	for (int y = 0; y < AllBricks.size(); ++y) {
//		for (int x = 0; x < AllBricks[y].size(); ++x) {
//
//			if (nullptr == AllBricks[y][x].SpriteRenderer)
//			{
//				continue;
//			}
//
//			// if (충돌했을대ㅑ)
//			// {
//				AllBricks[y][x].SpriteRenderer->SetSprite("paddle_materialize_002.png", 0);
//			// }
//
//			AllBricks[y][x].SpriteRenderer->GetComponentLocation();
//
//			FVector2D brickPos = IndexToBrickLocation({ y, x });
//			BrickSize = AllBricks[y][x].Scale;
//
//			// 로그 출력
//			UEngineDebug::CoreOutPutString("brickPos[" + std::to_string(y) + "][" + std::to_string(x) + "] : " + brickPos.ToString(), { 30, 300 + y * 100 + x * 20 });
//		}
//	}
//
//	// 충돌 감지 로직
//	FVector2D HitResult = (playerPos - brickPos) / BrickSize;
//
//	for (int y = 0; y < AllBricks.size(); ++y) {
//		for (int x = 0; x < AllBricks[y].size(); ++x) {
//			auto& brick = AllBricks[y][x];
//			FVector2D brickPos = IndexToBrickLocation({ y, x });
//			FVector2D brickSize = brick.Scale;
//
//			// 충돌 검사
//			if (playerPos.X < brickPos.X + brickSize.X &&
//				playerPos.X + playerSize.X > brickPos.X &&
//				playerPos.Y < brickPos.Y + brickSize.Y &&
//				playerPos.Y + playerSize.Y > brickPos.Y) {
//				// 충돌한 벽돌 발견
//				UEngineDebug::CoreOutPutString("Collision with brick at index [" + std::to_string(y) + "][" + std::to_string(x) + "]", { 800, 800 });
//
//				// 필요한 로직을 여기에 추가 (예: 충돌 처리, 벽돌 파괴 등)
//				//brick.HandleCollision(); // 예시로 벽돌에 충돌 처리 메서드를 호출
//			}
//		}
//	}
//
//
//
//	if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
//		if (HitResult.X < HitResult.Y) {
//			if (HitResult.X > 1 - HitResult.Y) {
//				UEngineDebug::CoreOutPutString("HitResult : BOTTOM" + HitResult.ToString(), { 100, 600 });
//				return FVector2D(0.0f, -1.0f); // 아래쪽 반사 벡터
//			}
//			else {
//				UEngineDebug::CoreOutPutString("HitResult : LEFT" + HitResult.ToString(), { 100, 600 });
//				return FVector2D(-1.0f, 0.0f); // 왼쪽 반사 벡터
//			}
//		}
//		else if (HitResult.X > HitResult.Y) {
//			if (HitResult.Y > 1 - HitResult.X) {
//				UEngineDebug::CoreOutPutString("HitResult : RIGHT" + HitResult.ToString(), { 100, 600 });
//				return FVector2D(1.0f, 0.0f); // 오른쪽 반사 벡터
//			}
//			else {
//				UEngineDebug::CoreOutPutString("HitResult : TOP" + HitResult.ToString(), { 100, 600 });
//				return FVector2D(0.0f, 1.0f); // 위쪽 반사 벡터
//			}
//		}
//	}
//
//	return FVector2D(0.0f, 0.0f); // 충돌 없음
//}

//// 잘되는데, 삭제가 안됨
//FVector2D BrickEditor::CheckCollision(const FVector2D& ballPos, const FVector2D& ballSize, FIntPoint brickIndex)
//{
//	// 벽돌 인덱스 범위가 유효하지 않으면 충돌 없음 반환
//	if (IsIndexOver(brickIndex))
//	{
//		return FVector2D(0.0f, 0.0f);
//	}
//
//	FVector2D HitResult;
//
//	// 전체 벽돌 순회하며 충돌 검사
//	for (int y = 0; y < AllBricks.size(); ++y)
//	{
//		for (int x = 0; x < AllBricks[y].size(); ++x)
//		{
//			auto& brick = AllBricks[y][x];
//
//			// SpriteRenderer가 없는 벽돌은 건너뜀
//			if (brick.SpriteRenderer == nullptr)
//			{
//				continue;
//			}
//
//			// 벽돌 위치와 크기 가져오기
//			FVector2D brickPos = BrickPositions[y][x];
//			FVector2D brickSize = BrickSize;
//
//			// 벽돌과 공의 위치 로그 출력
//
//
//
//			// 충돌 감지 로직
//			HitResult = (ballPos - brickPos) / brickSize;
//			//UEngineDebug::CoreOutPutString("brickPos " + brickPos.ToString());
//
//			if (true == brick.IsCollide)
//			{
//				brick.SpriteRenderer->SetSprite("paddle_materialize_002.png", 0);
//			}
//
//			// 충돌 조건 유지
//			if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
//				if (HitResult.X < HitResult.Y) {
//					if (HitResult.X > 1 - HitResult.Y) {
//						UEngineDebug::CoreOutPutString("HitResult : BOTTOM" + HitResult.ToString());
//						brick.IsCollide = true;
//						RemoveBlock(brickIndex);
//						return FVector2D(0.0f, -1.0f); // 아래쪽 반사 벡터
//					}
//					else {
//						UEngineDebug::CoreOutPutString("HitResult : LEFT" + HitResult.ToString());
//						brick.IsCollide = true;
//						RemoveBlock(brickIndex);
//						return FVector2D(-1.0f, 0.0f); // 왼쪽 반사 벡터
//					}
//				}
//				else if (HitResult.X > HitResult.Y) {
//					if (HitResult.Y > 1 - HitResult.X) {
//						UEngineDebug::CoreOutPutString("HitResult : RIGHT" + HitResult.ToString());
//						brick.IsCollide = true;
//						RemoveBlock(brickIndex);
//						return FVector2D(1.0f, 0.0f); // 오른쪽 반사 벡터
//					}
//					else {
//						UEngineDebug::CoreOutPutString("HitResult : TOP" + HitResult.ToString());
//						brick.IsCollide = true;
//						RemoveBlock(brickIndex);
//						return FVector2D(0.0f, 1.0f); // 위쪽 반사 벡터
//					}
//				}
//			}
//
//		}
//
//	}
//
//	return FVector2D(0.0f, 0.0f); // 충돌 없음
//}

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

			if (true == brick.IsCollide)
			{
				//brick.SpriteRenderer->SetSprite("exp");
				//brick.SpriteRenderer->SetSpriteScale(0.5f);
				//brick.SpriteRenderer->CreateAnimation("exp1", "exp", 0, 7, 0.1f,false);
				//brick.SpriteRenderer->ChangeAnimation("exp1");

			}


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






//void BrickEditor::RemoveBlock(FIntPoint brickIndex)
//{
//	if (true == IsIndexOver(brickIndex))
//	{
//		MSGASSERT("인덱스가 오버되었습니다.");
//	}
//
//	if (nullptr == AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer)
//	{
//		return;
//	}
//
//	AllBricks[brickIndex.Y][brickIndex.X].HP -= 1;
//
//		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSprite("brick_silver_shine");
//		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetOrder(ERenderOrder::FX);
//		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSpriteScale(1.28f);
//		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->CreateAnimation("brick_silver_shine", "brick_silver_shine", 0, 9, 0.12f,true);
//		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->ChangeAnimation("brick_silver_shine");
//
//		if (AllBricks[brickIndex.Y][brickIndex.X].HP == 0)
//		{
//			AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSprite("exp");
//			AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetOrder(ERenderOrder::FX);
//			AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSpriteScale(0.9f);
//			AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->CreateAnimation("exp1", "exp", 1, 8, 0.09f,false);
//			AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->ChangeAnimation("exp1");
//
//			AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->Destroy(0.11f);
//			AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer = nullptr;
//
//			SpawnFX(brickPos);
//		}
//
//
//}

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

	// HPBrick일 경우에만 "brick_silver_shine" 효과 적용
	if (AllBricks[brickIndex.Y][brickIndex.X].HP>1 && AllBricks[brickIndex.Y][brickIndex.X].HP<3)
	{
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSprite("brick_silver_shine");
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetOrder(ERenderOrder::FX);
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSpriteScale(1.3f);
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->CreateAnimation("brick_silver_shine", "brick_silver_shine", 0, 9, 0.12f, true);
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->ChangeAnimation("brick_silver_shine");
	}

	if (AllBricks[brickIndex.Y][brickIndex.X].HP == 0)
	{
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSprite("exp");
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetOrder(ERenderOrder::FX);
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->SetSpriteScale(0.9f);
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->CreateAnimation("exp1", "exp", 1, 8, 0.09f, false);
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->ChangeAnimation("exp1");

		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer->Destroy(0.11f);
		AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer = nullptr;

		SpawnFX(brickPos);
	}
}




void BrickEditor::SpawnFX(FVector2D _brickPos)
{
	BonusA = GetWorld()->SpawnActor<Brick>();
	BonusA->SetActorLocation(_brickPos);
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

	AllBricks[_Index.Y][_Index.X].SpriteIndex = _SpriteIndex;
	AllBricks[_Index.Y][_Index.X].SpriteRenderer->SetSprite(_Sprite, _SpriteIndex);
}


BrickEditor::BrickEditor()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
}
void BrickEditor::setBrickType(FIntPoint _Index, BrickType _Type)
{
	//std::random
	int randomIndex = (std::rand() % 5) ; // 인덱스가 0~4까지

	switch (_Type)
	{
	case Default:

		SetBrickSprite(_Index, "Brick", randomIndex);
		SetBrickHp(_Index, 1);
		break;
	case HPBrick:
		SetBrickSprite(_Index, "Brick", 6);
		SetBrickHp(_Index, 3);
		break;
	case NotBreak:
		SetBrickSprite(_Index, "Brick", 5);
		SetBrickHp(_Index, 400001);
		break;
	default:
		break;
	}
}

