#pragma once
#include <vector>

#include <EngineCore/Actor.h>
#include <EngineCore/ImageManager.h>
#include <EngineCore/SpriteRenderer.h>

class AScore : public AActor
{
public:
	// constrcuter destructer
	AScore();
	~AScore();

	void SetTextSpriteName(const std::string _Text);

	void SetTextScale(FVector2D _TextScale)
	{
		TextScale = _TextScale;
	}

	template<typename EnumType>
	void SetOrder(EnumType _Order)
	{
		SetOrder(static_cast<int>(_Order));
	}

	void SetOrder(int _Order);

	void SetValue(int _Score);

	std::vector<class USpriteRenderer*>* GetRenders()
	{
		return &Renders;
	}
	FVector2D SetPos(FVector2D _Pos)
	{
		Pos = _Pos;
	}

protected:
	void BeginPlay() override{}
	void Tick(float _DeltaTime) override{}

private:
	std::string TextSpriteName;
	FVector2D TextScale;
	std::vector<class USpriteRenderer*> Renders;
	FVector2D Pos = { 0,0 };
};

