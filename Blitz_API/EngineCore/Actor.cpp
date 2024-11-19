#include "PreCompiledFile.h"
#include "Actor.h"

#include <EngineCore/EngineAPICore.h>
#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineDebug.h>
#include <EnginePlatform/EngineWindow.h>
#include <EnginePlatform/EngineWinImage.h>

#include "EngineSprite.h"
#include <EngineBase/EngineDebug.h>

#include "ImageManager.h"
#include "EngineCoreDebug.h"

// delete 도 헤더가 있어야 호출할수 있습니다.
#include "ActorComponent.h"

std::list<UActorComponent*> AActor::ComponentList;

void AActor::ComponentBeginPlay()
{
	{
		std::list<UActorComponent*>::iterator StartIter = ComponentList.begin();
		std::list<UActorComponent*>::iterator EndIter = ComponentList.end();

		for (; StartIter != EndIter; ++StartIter)
		{
			UActorComponent* CurActor = *StartIter;
			CurActor->BeginPlay();
		}

		ComponentList.clear();
	}

}

AActor::AActor()
{
}

AActor::~AActor()
{
	// 컴포넌트의 생성주기는 액터의 생명주기와 같다고 한다.
	std::list<UActorComponent*>::iterator StartIter = Components.begin();
	std::list<UActorComponent*>::iterator EndIter = Components.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		UActorComponent* Component = *StartIter;

		if (nullptr != Component)
		{
			delete Component;
		}
	}

	Components.clear();
}

void AActor::Tick(float _DeltaTime)
{
	if (true == IsDebug())
	{
		FVector2D Pos = GetActorLocation();
		FVector2D CameraPos = GetWorld()->GetCameraPos();

		FTransform Trans;
		Trans.Location = Pos - CameraPos;
		Trans.Scale = { 6, 6 };

		//UEngineDebug::CoreDebugRender(Trans, UEngineDebug::EDebugPosType::Circle);
	}

	TimeEventer.Update(_DeltaTime);

	std::list<class UActorComponent*>::iterator StartIter = Components.begin();
	std::list<class UActorComponent*>::iterator EndIter = Components.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		if (false == (*StartIter)->IsActive())
		{
			continue;
		}

		(*StartIter)->ComponentTick(_DeltaTime);
	}

}

void AActor::ReleaseTimeCheck(float _DeltaTime)
{
	UObject::ReleaseTimeCheck(_DeltaTime);

	std::list<UActorComponent*>::iterator StartIter = Components.begin();
	std::list<UActorComponent*>::iterator EndIter = Components.end();
	for (; StartIter != EndIter; ++StartIter)
	{
		UActorComponent* Component = *StartIter;
		Component->ReleaseTimeCheck(_DeltaTime);
	}
}

void AActor::ReleaseCheck(float _DeltaTime)
{
	UObject::ReleaseCheck(_DeltaTime);

	// 컴포넌트의 생성주기는 액터의 생명주기와 같다고 한다.
	std::list<UActorComponent*>::iterator StartIter = Components.begin();
	std::list<UActorComponent*>::iterator EndIter = Components.end();
	for (; StartIter != EndIter; )
	{
		UActorComponent* Component = *StartIter;

		if (false == Component->IsDestroy())
		{
			Component->ReleaseCheck(_DeltaTime);
			++StartIter;
			continue;
		}

		// 액터는 죽을 컴포넌트가 있으면 진짜 죽
		delete Component;
		StartIter = Components.erase(StartIter);
	}
}