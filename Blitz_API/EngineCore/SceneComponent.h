#pragma once
#include "ActorComponent.h"

// ���� :
class USceneComponent : public UActorComponent
{
public:
	USceneComponent();
	~USceneComponent();


	FTransform GetTransform()
	{
		return Transform;
	}

	// ���͸� ���������� Ʈ������ 
	FTransform GetActorTransform();

	FVector2D GetComponentScale()
	{
		return Transform.Scale;
	}

	FVector2D GetComponentLocation()
	{
		return Transform.Location;
	}


	// ���� ��ġ ������� ������ �Ұ��̴�.
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

