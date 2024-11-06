#pragma once
#include <EngineBase/Object.h>
#include <EngineBase/EngineMath.h>

#include "EngineSprite.h"

// 설명 :
class AActor : public UObject
{
public:
	typedef AActor Super;

	friend class ULevel;

	// constrcuter destructer
	AActor();
	~AActor();

	// delete Function
	AActor(const AActor& _Other) = delete;
	AActor(AActor&& _Other) noexcept = delete;
	AActor& operator=(const AActor& _Other) = delete;
	AActor& operator=(AActor&& _Other) noexcept = delete;

	// 이 객체가 레벨에서 처음 Tick을 돌리기 직전에 실행된다.
	virtual void BeginPlay() {}

	// 델타타임이란 무엇인가?
	virtual void Tick(float _DeltaTime) {}

	virtual void LevelChangeStart() {}
	virtual void LevelChangeEnd() {}

	class ULevel* GetWorld()
	{
		return World;
	}

	void SetActorLocation(FVector2D _Location)
	{
		Transform.Location = _Location;
	}

	void AddActorLocation(FVector2D _Direction)
	{
		Transform.Location += _Direction;
	}

	FTransform GetTransform()
	{
		return Transform;
	}

	FVector2D GetActorLocation()
	{
		return Transform.Location;
	}

	// 컴포넌트의 소유자는 액터 삭제도 액터가 해야한다.
	// 다른 클래스는 절대로 삭제하면 안된다.
	template<typename ComponentType>
	ComponentType* CreateDefaultSubObject()
	{
		ComponentType* NewComponent = new ComponentType();

		UActorComponent* ComponentPtr = dynamic_cast<UActorComponent*>(NewComponent);
		// 내가 널 만든 레벨이야.
		ComponentPtr->ParentActor = this;

		// 생성될때 하지 않습니다.
		// NewComponent->BeginPlay();
		// 만들기만 하고 실행 안한 상태가 된것.
		Components.push_back(NewComponent);

		// BeginPlay가 실행안된 컴포넌트들을 다 자료구조에 담는다.
		ComponentList.push_back(NewComponent);
		return NewComponent;
	}

protected:

private:
	static void ComponentBeginPlay();

	static bool IsNewActorCreate;
	static std::list<class UActorComponent*> ComponentList;

	void ReleaseCheck(float _DeltaTime) override;

	class ULevel* World = nullptr;
	FTransform Transform;

	std::list<class UActorComponent*> Components;
};

