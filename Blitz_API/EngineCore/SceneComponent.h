#pragma once
#include "ActorComponent.h"

// 설명 :
class USceneComponent : public UActorComponent
{
public:
	USceneComponent();
	~USceneComponent();


	FTransform GetTransform()
	{
		return Transform;
	}

	// 엑터를 기준으로한 트랜스폼 
	FTransform GetActorTransform();

	FVector2D GetComponentScale()
	{
		return Transform.Scale;
	}

	FVector2D GetComponentLocation()
	{
		return Transform.Location;
	}


	// 액터 위치 기반으로 랜더링 할것이다.
	void SetComponentLocation(FVector2D _Location)
	{
		Transform.Location = _Location;
	}

	void SetComponentScale(FVector2D _Scale)
	{
		Transform.Scale = _Scale;
	}

protected:

private:
	FTransform Transform;
};

