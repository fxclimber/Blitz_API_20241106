#pragma once
#include <EngineBase/Object.h>

// 설명 :
class UActorComponent : public UObject
{
public:
	// 액터만이 ParentActor 세팅할수 있게 제한하기 위해서 아래와 같이 짠다.
	friend class AActor;

	typedef UActorComponent Super;

	// constrcuter destructer
	UActorComponent();
	// 객체화 못시키게 하는법중 하나
	~UActorComponent();

	// delete Function
	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;

	virtual void BeginPlay() {}
	// 델타타임이란 무엇인가?
	virtual void ComponentTick(float _DeltaTime) {}

	class AActor* GetActor()
	{
		return ParentActor;
	}

protected:

private:
	class AActor* ParentActor;
};

