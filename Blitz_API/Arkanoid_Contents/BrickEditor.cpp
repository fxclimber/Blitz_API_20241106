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

	// Ÿ�ϸ��� �ٽ� 0, 0�� �ΰ����� ���ؾ� �Ѵ�.
	// Ÿ�Ϸμ��� 0,0�� ����.
	// Ÿ���� ��ġ�� ����μ��� Ÿ�ϸ� * �ε����� �ȴ�.
	float WinSize = UEngineAPICore::GetCore()->GetMainWindow().GetWindowSize().Half().X;
	//float WinSize = Globals::WinSize.X;//������....��........
	float AllSpriteWidth = (_BrickSize.X * static_cast<float>(_Count.X)) / 2;
	float gap = WinSize - AllSpriteWidth;
	PlusPos = { gap, Height };
	SetActorLocation(PlusPos);
}

FVector2D BrickEditor::IndexToBrickLocation(FIntPoint _Index)
{
	//�ε����� ������� ������ ��ġ�� ���
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

// _SpriteScale : ���� ����ũ��, ���� ������ġ ���
//void BrickEditor::SetBrickIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex)
//{
//	if (true == IsIndexOver(_Index))
//	{
//		return;
//	}
//
//	// �̸� �� ������ �ʰ�
//	// ���� ��������Ʈ �������� �����.
//	if (nullptr == AllBricks[_Index.Y][_Index.X].SpriteRenderer)
//	{
//		USpriteRenderer* NewSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
//		NewSpriteRenderer->SetComponentScale(BrickSize);
//		AllBricks[_Index.Y][_Index.X].SpriteRenderer = NewSpriteRenderer;
//	}
//
//	// ��������Ʈ�� ������ �����Ƿ�.	// �� �̸����� �������ָ� �ȴ�.
//	USpriteRenderer* FindSprite = AllBricks[_Index.Y][_Index.X].SpriteRenderer;
//	FindSprite->SetSprite(SpriteName, _SpriteIndex);
//
//	FVector2D TileLocation = IndexToBrickLocation(_Index) + PlusPos;
//	FindSprite->SetComponentScale(_SpriteScale);
//	// �Ʒ��ʿ� �������� �������� ���߿� �ȴ�.
//	FindSprite->SetOrder(_Index.Y);
//
//	AllBricks[_Index.Y][_Index.X].SpriteRenderer->SetComponentLocation(TileLocation + BrickSize.Half() + _Pivot);
//	AllBricks[_Index.Y][_Index.X].Pivot = _Pivot;
//	AllBricks[_Index.Y][_Index.X].Scale = _SpriteScale;//���� ������ �������� ����
//	AllBricks[_Index.Y][_Index.X].SpriteIndex = _SpriteIndex;
//}


void BrickEditor::SetBrickIndex(FIntPoint _Index, FVector2D _Pivot, FVector2D _SpriteScale, int _SpriteIndex)
{
	// �ε����� ������ ����� ����
	if (IsIndexOver(_Index))
	{
		return;
	}

	// ������ ��ġ�� ����Ͽ� BrickPositions�� ����
	FVector2D TileLocation = IndexToBrickLocation(_Index);
	if (BrickPositions.size() <= _Index.Y) {
		BrickPositions.resize(_Index.Y + 1); // �� ũ�� ����
	}
	if (BrickPositions[_Index.Y].size() <= _Index.X) {
		BrickPositions[_Index.Y].resize(_Index.X + 1); // �� ũ�� ����
	}
	BrickPositions[_Index.Y][_Index.X] = FVector2D(TileLocation.X + (PlusPos.X), TileLocation.Y + Height);

	// ������ SpriteName ����
	if (SpriteName.empty())
	{
		SpriteName = "DefaultSprite"; // �⺻ ��������Ʈ �̸� ���� (�ʿ信 ���� ���� ����)
	}

	// ���� ũ�� ���� (�ʱ�ȭ���� ���� ���)
	if (BrickSize == FVector2D{ 0.0f, 0.0f })
	{
		BrickSize = _SpriteScale; // �⺻ ���� ũ�⸦ ���� �����Ϸ� ����
	}

	// ��������Ʈ �������� ���� ��� ���� ����
	if (AllBricks[_Index.Y][_Index.X].SpriteRenderer == nullptr)
	{
		USpriteRenderer* NewSpriteRenderer = CreateDefaultSubObject<USpriteRenderer>();
		NewSpriteRenderer->SetComponentScale(BrickSize);
		AllBricks[_Index.Y][_Index.X].SpriteRenderer = NewSpriteRenderer;
	}

	// ��������Ʈ ����
	USpriteRenderer* FindSprite = AllBricks[_Index.Y][_Index.X].SpriteRenderer;
	FindSprite->SetSprite(SpriteName, _SpriteIndex); // ������ ��������Ʈ �ε��� ����
	FindSprite->SetComponentScale(_SpriteScale); // ��������Ʈ ũ�� ����
	FindSprite->SetOrder(_Index.Y); // Y �ε����� �������� ���� ���� ����

	// ��ġ ����
	FVector2D BrickCenterOffset = BrickSize.Half() + _Pivot;
	FindSprite->SetComponentLocation(TileLocation + BrickCenterOffset);

	// AllBricks�� ���� ���� �Ӽ� ����
	AllBricks[_Index.Y][_Index.X].Pivot = _Pivot;
	AllBricks[_Index.Y][_Index.X].Scale = _SpriteScale;
	AllBricks[_Index.Y][_Index.X].SpriteIndex = _SpriteIndex;
}




// Ư�� ��ġ�� �ش��ϴ� ���� ������ ���
ABrick* BrickEditor::GetBrickRef(FVector2D _Location)
{
	FIntPoint Point = LocationToIndex(_Location);

	return GetBrickRef(Point);
}

// �ε����� ������� Ư����ġ�� ���������� ���� 
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

	// �����͸� �о����������.
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
//	// ������ ��ġ�� ũ�⸦ �������� ���� brickIndex�� ����Ͽ� AllBricks���� ����
//	if (true == IsIndexOver(brickIndex))
//	{
//		return FVector2D(0.0f, 0.0f); // �浹 ����
//	}
//	//FVector2D thisPos = GetTransform().Location;
//	auto& brick = AllBricks[brickIndex.Y][brickIndex.X];
//	FVector2D brickPos = IndexToBrickLocation(brickIndex);// -thisPos;
//	BrickSize = brick.Scale; // ���� ������ ũ��
//
//	// 0          0 
//	// AllBricks[y][x] => index 0, 0 => fvector 0, 0
//	// AllBricks[y][x] => index 1, 0 => fvector 64, 0
//
//	// �浹üũ�� ���� �� �Ҽ� �ִ�.
//	for (int y = 0; y < AllBricks.size(); ++y) {
//		for (int x = 0; x < AllBricks[y].size(); ++x) {
//
//			if (nullptr == AllBricks[y][x].SpriteRenderer)
//			{
//				continue;
//			}
//
//			// if (�浹�������)
//			// {
//				AllBricks[y][x].SpriteRenderer->SetSprite("paddle_materialize_002.png", 0);
//			// }
//
//			AllBricks[y][x].SpriteRenderer->GetComponentLocation();
//
//			FVector2D brickPos = IndexToBrickLocation({ y, x });
//			BrickSize = AllBricks[y][x].Scale;
//
//			// �α� ���
//			UEngineDebug::CoreOutPutString("brickPos[" + std::to_string(y) + "][" + std::to_string(x) + "] : " + brickPos.ToString(), { 30, 300 + y * 100 + x * 20 });
//		}
//	}
//
//	// �浹 ���� ����
//	FVector2D HitResult = (playerPos - brickPos) / BrickSize;
//
//	for (int y = 0; y < AllBricks.size(); ++y) {
//		for (int x = 0; x < AllBricks[y].size(); ++x) {
//			auto& brick = AllBricks[y][x];
//			FVector2D brickPos = IndexToBrickLocation({ y, x });
//			FVector2D brickSize = brick.Scale;
//
//			// �浹 �˻�
//			if (playerPos.X < brickPos.X + brickSize.X &&
//				playerPos.X + playerSize.X > brickPos.X &&
//				playerPos.Y < brickPos.Y + brickSize.Y &&
//				playerPos.Y + playerSize.Y > brickPos.Y) {
//				// �浹�� ���� �߰�
//				UEngineDebug::CoreOutPutString("Collision with brick at index [" + std::to_string(y) + "][" + std::to_string(x) + "]", { 800, 800 });
//
//				// �ʿ��� ������ ���⿡ �߰� (��: �浹 ó��, ���� �ı� ��)
//				//brick.HandleCollision(); // ���÷� ������ �浹 ó�� �޼��带 ȣ��
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
//				return FVector2D(0.0f, -1.0f); // �Ʒ��� �ݻ� ����
//			}
//			else {
//				UEngineDebug::CoreOutPutString("HitResult : LEFT" + HitResult.ToString(), { 100, 600 });
//				return FVector2D(-1.0f, 0.0f); // ���� �ݻ� ����
//			}
//		}
//		else if (HitResult.X > HitResult.Y) {
//			if (HitResult.Y > 1 - HitResult.X) {
//				UEngineDebug::CoreOutPutString("HitResult : RIGHT" + HitResult.ToString(), { 100, 600 });
//				return FVector2D(1.0f, 0.0f); // ������ �ݻ� ����
//			}
//			else {
//				UEngineDebug::CoreOutPutString("HitResult : TOP" + HitResult.ToString(), { 100, 600 });
//				return FVector2D(0.0f, 1.0f); // ���� �ݻ� ����
//			}
//		}
//	}
//
//	return FVector2D(0.0f, 0.0f); // �浹 ����
//}

//// �ߵǴµ�, ������ �ȵ�
//FVector2D BrickEditor::CheckCollision(const FVector2D& ballPos, const FVector2D& ballSize, FIntPoint brickIndex)
//{
//	// ���� �ε��� ������ ��ȿ���� ������ �浹 ���� ��ȯ
//	if (IsIndexOver(brickIndex))
//	{
//		return FVector2D(0.0f, 0.0f);
//	}
//
//	FVector2D HitResult;
//
//	// ��ü ���� ��ȸ�ϸ� �浹 �˻�
//	for (int y = 0; y < AllBricks.size(); ++y)
//	{
//		for (int x = 0; x < AllBricks[y].size(); ++x)
//		{
//			auto& brick = AllBricks[y][x];
//
//			// SpriteRenderer�� ���� ������ �ǳʶ�
//			if (brick.SpriteRenderer == nullptr)
//			{
//				continue;
//			}
//
//			// ���� ��ġ�� ũ�� ��������
//			FVector2D brickPos = BrickPositions[y][x];
//			FVector2D brickSize = BrickSize;
//
//			// ������ ���� ��ġ �α� ���
//
//
//
//			// �浹 ���� ����
//			HitResult = (ballPos - brickPos) / brickSize;
//			//UEngineDebug::CoreOutPutString("brickPos " + brickPos.ToString());
//
//			if (true == brick.IsCollide)
//			{
//				brick.SpriteRenderer->SetSprite("paddle_materialize_002.png", 0);
//			}
//
//			// �浹 ���� ����
//			if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
//				if (HitResult.X < HitResult.Y) {
//					if (HitResult.X > 1 - HitResult.Y) {
//						UEngineDebug::CoreOutPutString("HitResult : BOTTOM" + HitResult.ToString());
//						brick.IsCollide = true;
//						RemoveBlock(brickIndex);
//						return FVector2D(0.0f, -1.0f); // �Ʒ��� �ݻ� ����
//					}
//					else {
//						UEngineDebug::CoreOutPutString("HitResult : LEFT" + HitResult.ToString());
//						brick.IsCollide = true;
//						RemoveBlock(brickIndex);
//						return FVector2D(-1.0f, 0.0f); // ���� �ݻ� ����
//					}
//				}
//				else if (HitResult.X > HitResult.Y) {
//					if (HitResult.Y > 1 - HitResult.X) {
//						UEngineDebug::CoreOutPutString("HitResult : RIGHT" + HitResult.ToString());
//						brick.IsCollide = true;
//						RemoveBlock(brickIndex);
//						return FVector2D(1.0f, 0.0f); // ������ �ݻ� ����
//					}
//					else {
//						UEngineDebug::CoreOutPutString("HitResult : TOP" + HitResult.ToString());
//						brick.IsCollide = true;
//						RemoveBlock(brickIndex);
//						return FVector2D(0.0f, 1.0f); // ���� �ݻ� ����
//					}
//				}
//			}
//
//		}
//
//	}
//
//	return FVector2D(0.0f, 0.0f); // �浹 ����
//}

FVector2D BrickEditor::CheckCollision(const FVector2D& ballPos, const FVector2D& ballSize, FIntPoint brickIndex)
{
	// ���� �ε��� ������ ��ȿ���� ������ �浹 ���� ��ȯ
	if (IsIndexOver(brickIndex))
	{
		return FVector2D(0.0f, 0.0f);
	}

	FVector2D HitResult;

	// ��ü ���� ��ȸ�ϸ� �浹 �˻�
	for (int y = 0; y < AllBricks.size(); ++y)
	{
		for (int x = 0; x < AllBricks[y].size(); ++x)
		{
			auto& brick = AllBricks[y][x];

			// SpriteRenderer�� ���� ������ �ǳʶ�
			if (brick.SpriteRenderer == nullptr)
			{
				continue;
			}

			// ���� ��ġ�� ũ�� ��������
			brickPos = BrickPositions[y][x];
			FVector2D brickSize = BrickSize;

			// �浹 ���� ����
			HitResult = (ballPos - brickPos) / brickSize;

			if (true == brick.IsCollide)
			{
				//brick.SpriteRenderer->SetSprite("exp");
				//brick.SpriteRenderer->SetSpriteScale(0.5f);
				//brick.SpriteRenderer->CreateAnimation("exp1", "exp", 0, 7, 0.1f,false);
				//brick.SpriteRenderer->ChangeAnimation("exp1");

			}


			// �浹 ���� ����
			if (HitResult.X > 0 && HitResult.Y > 0 && HitResult.X < 1 && HitResult.Y < 1) {
				if (HitResult.X < HitResult.Y) {
					if (HitResult.X > 1 - HitResult.Y) {
						RemoveBlock(FIntPoint(x, y)); // ���� �浹�� ������ �ε����� ����
						//SpawnFX(brickPos);
						ballPosition = WhereIsBall::BOTTOM;
						return FVector2D(0.0f, -1.0f); // �Ʒ��� �ݻ� ����
					}
					else {
						RemoveBlock(FIntPoint(x, y)); // ���� �浹�� ������ �ε����� ����
						//SpawnFX(brickPos);
						ballPosition = WhereIsBall::LEFT;
						return FVector2D(-1.0f, 0.0f); // ���� �ݻ� ����
					}
				}
				else if (HitResult.X > HitResult.Y) {
					if (HitResult.Y > 1 - HitResult.X) {
						RemoveBlock(FIntPoint(x, y)); // ���� �浹�� ������ �ε����� ����
						//SpawnFX(brickPos);
						ballPosition = WhereIsBall::RIGHT;
						return FVector2D(1.0f, 0.0f); // ������ �ݻ� ����
					}
					else {
						RemoveBlock(FIntPoint(x, y)); // ���� �浹�� ������ �ε����� ����
						//SpawnFX(brickPos);
						ballPosition = WhereIsBall::TOP;
						return FVector2D(0.0f, 1.0f); // ���� �ݻ� ����
					}
				}
			}
		}
	}

	return FVector2D(0.0f, 0.0f); // �浹 ����
}






//void BrickEditor::RemoveBlock(FIntPoint brickIndex)
//{
//	if (true == IsIndexOver(brickIndex))
//	{
//		MSGASSERT("�ε����� �����Ǿ����ϴ�.");
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
		MSGASSERT("�ε����� �����Ǿ����ϴ�.");
	}

	if (nullptr == AllBricks[brickIndex.Y][brickIndex.X].SpriteRenderer)
	{
		return;
	}

	AllBricks[brickIndex.Y][brickIndex.X].HP -= 1;

	// HPBrick�� ��쿡�� "brick_silver_shine" ȿ�� ����
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
	// �ε����� ������ ����� ����
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
	int randomIndex = (std::rand() % 5) ; // �ε����� 0~4����

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

