#pragma once
#include "GameMode.h"


// 이런식으로 
class CollisionLinkData
{
public:
	union
	{
		struct
		{
			int Left;
			int Right;
		};
		__int64 Key;
	};
};


// 설명 :
class ULevel : public UObject
{
public:
	friend class U2DCollision;
	friend class USpriteRenderer;
	friend class UEngineAPICore;
	// constrcuter destructer
	ULevel();
	~ULevel();

	// delete Function
	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

	ULevel& operator=(const ULevel& _Other) = delete;
	// 내가 CurLevel 됐을대
	void LevelChangeStart();

	// 나 이제 새로운 레벨로 바뀔거야.
	void LevelChangeEnd();

	void Tick(float _DeltaTime);
	void Render(float _DeltaTime);
	void Collision(float _DeltaTime);
	void Release(float _DeltaTime);

	template<typename ActorType>
	ActorType* SpawnActor()
	{
		ActorType* NewActor = new ActorType();

		AActor* ActorPtr = dynamic_cast<AActor*>(NewActor);
		// 내가 널 만든 레벨이야.
		ActorPtr->World = this;

		BeginPlayList.push_back(ActorPtr);
		// NewActor->BeginPlay();
		// AllActors.push_back(NewActor);
		return NewActor;
	}

	void SetCameraToMainPawn(bool _IsCameraToMainPawn)
	{
		IsCameraToMainPawn = _IsCameraToMainPawn;
	}

	void SetCameraPivot(FVector2D _Pivot)
	{
		CameraPivot = _Pivot;
	}

	void SetCameraPos(FVector2D _Pos)
	{
		CameraPos = _Pos;
	}

	void AddCameraPos(FVector2D _Value)
	{
		CameraPos += _Value;
	}

	FVector2D GetCameraPivot()
	{
		return CameraPivot;
	}

	FVector2D GetCameraPos()
	{
		return CameraPos;
	}

	AActor* GetPawn()
	{
		return MainPawn;
	}

	template<typename ConvertType>
	ConvertType* GetPawn()
	{
		return dynamic_cast<ConvertType*>(MainPawn);
	}

	AActor* GetGameMode()
	{
		return GameMode;
	}

	template<typename ConvertType>
	ConvertType* GetGameMode()
	{
		return dynamic_cast<ConvertType*>(GameMode);
	}


	template<typename LeftEnumType, typename RightEnumType>
	static void CollisionGroupLink(LeftEnumType _Left, RightEnumType _Right)
	{
		CollisionGroupLink(static_cast<int>(_Left), static_cast<int>(_Right));
	}

	static void CollisionGroupLink(int _Left, int _Right)
	{
		CollisionLinkData LinkData;
		LinkData.Left = _Left;
		LinkData.Right = _Right;

		for (size_t i = 0; i < CollisionLink.size(); i++)
		{
			// 조금 추하지만 vector
			if (CollisionLink[i].Key == LinkData.Key)
			{
				return;
			}
		}

		CollisionLink.push_back(LinkData);
	}

	template<typename ActorType>
	std::list<ActorType*> GetActorsFromClass()
	{
		std::list<ActorType*> Result;

		std::list<AActor*>::iterator StartIter = AllActors.begin();
		std::list<AActor*>::iterator EndIter = AllActors.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			AActor* CurActor = *StartIter;

			ActorType* ConvertActor = dynamic_cast<ActorType*>(CurActor);

			if (nullptr == ConvertActor)
			{
				continue;
			}

			Result.push_back(ConvertActor);
		}
		return Result;
	}


protected:

private:
	void ScreenClear();
	void DoubleBuffering();
	void BeginPlayCheck();


	// 게임레벨과 메인폰을 만들어서 게임을 준비시키는 함수로도 만들었다.
	template<typename GameModeType, typename MainPawnType>
	void CreateGameMode()
	{
		GameMode = new GameModeType();

		// 화면을 바라봐주는 카메라라고 생각하고 만드셔도 됩니다.
		MainPawn = new MainPawnType();

		// 월드세팅이 먼저되는것이 굉장히 중요하다.
		MainPawn->World = this;
		GameMode->World = this;

		BeginPlayList.push_back(GameMode);
		BeginPlayList.push_back(MainPawn);

		//GameMode->BeginPlay();
		//MainPawn->BeginPlay();
		//AllActors.push_back(GameMode);
		//AllActors.push_back(MainPawn);
	}

	// 아무나 함부로 호출하지 못하게 하기 위해서 private 이어야 한다.
	void PushRenderer(class USpriteRenderer* _Renderer);
	void ChangeRenderOrder(class USpriteRenderer* _Renderer, int _PrevOrder);

	void PushCollision(class U2DCollision* _Collision);

	void PushCheckCollision(class U2DCollision* _Collision);

	void CollisionEventCheck(class U2DCollision* _Left, class U2DCollision* _Right);


	// 헝가리안 표기법
	// 이름은 마음대로
	// 맴버변수의 이름은 대문자
	// 음역하지마세요
	// dujumsaigury
	// 영어의미로 해주시면 됩니다.
	// 맨앞만 
	class AGameMode* GameMode = nullptr;

	// 주인공
	class AActor* MainPawn = nullptr;

	std::list<AActor*> AllActors;

	std::list<AActor*> BeginPlayList;

	bool IsCameraToMainPawn = true;
	// 아래 포지션 2개가 카메라.
	FVector2D CameraPos;
	FVector2D CameraPivot;

	// 오더링을 할것이다.
	std::map<int, std::list<class USpriteRenderer*>> Renderers;

	// 직접호출을 위해서 들고만 있는 용도
	std::map<int, std::list<class U2DCollision*>> Collisions;

	// 이벤트 체크방식을 위해서 어떤 그룹이 어떤 그룹과 충돌하지 기록해 놓은 자료구조
	static std::vector<CollisionLinkData> CollisionLink;

	// 프레임마다 충돌체크를 하는 콜리전들을 따로 모아 놓은 자료구조 => 이거 피하고 싶다.
	std::map<int, std::list<class U2DCollision*>> CheckCollisions;
};

