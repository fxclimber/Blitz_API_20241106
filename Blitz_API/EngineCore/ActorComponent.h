#pragma once
#include <EngineBase/Object.h>

class UActorComponent : public UObject
{
public:
	// 액터만이 ParentActor 세팅할수 있게 제한하기
	friend class AActor;

	typedef UActorComponent Super;

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


	bool IsActive() override
	{
		// 랜더러는 자신을 가진 액터에게 종속된다.
		// 부모도        true            true
		return UObject::IsActive() && GetActor()->IsActive();
	}


	bool IsDestroy() override
	{
		// 부모도        true            true
		return UObject::IsDestroy() || GetActor()->IsDestroy();
	}



protected:

private:
	class AActor* ParentActor;
};

