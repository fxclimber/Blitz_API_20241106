#pragma once
#include <EngineCore/Actor.h>

class ABullet : public AActor
{
public:
	ABullet();
	~ABullet();
	void MoveFunction(FVector2D _Dir);


protected:
	void Tick(float _DeltaTime) override;


private:
};

