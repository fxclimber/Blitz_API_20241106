#include "PreCompiledFile.h"
#include "Bullet.h"
#include <EngineCore/EngineAPICore.h>
#include <math.h>

ABullet::ABullet()
{
	//SetActorScale({ 50,50 });
}

ABullet::~ABullet()
{
}

void ABullet::MoveFunction(FVector2D _Dir)
{
	float DeltaTime = UEngineAPICore::GetCore()->GetDeltaTime();
	AddActorLocation(_Dir * DeltaTime * 200.f);
}

void ABullet::Tick(float _DeltaTime)
{
	AddActorLocation(FVector2D::RIGHT * _DeltaTime * 300.0f);
}
