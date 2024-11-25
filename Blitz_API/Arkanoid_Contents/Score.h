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

	void SetValue(int _Score , int _Padding);
	void SetFinalValue(int _FinalScore)
	{
		FinalScoreUI = _FinalScore;
	}
	void SetFinalTime(int _FinalElapsedTime)
	{
		FinalElapsedTime = _FinalElapsedTime;
	}


	std::vector<class USpriteRenderer*>* GetRenders()
	{
		return &Renders;
	}

	static int ScoreUI;
	static int FinalScoreUI;
	static int ElapsedTime;
	static int FinalElapsedTime;

protected:
	void BeginPlay() override{}
	void Tick(float _DeltaTime) override{}

private:
	
	std::string TextSpriteName;
	FVector2D TextScale;
	std::vector<class USpriteRenderer*> Renders;


};

