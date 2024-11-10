#pragma once
#include "SceneComponent.h"
#include "EngineSprite.h"
#include <EngineBase/EngineDelegate.h>
#include <map>

enum class PivotType
{
	Center,
	Bot,
	Top,
};

// 설명 :
class USpriteRenderer : public USceneComponent
{
	// 애가 다 담당한다.
	// 클래스를 심화분류해서
public:
	class FrameAnimation
	{
	public:
		UEngineSprite* Sprite = nullptr;
		std::vector<int> FrameIndex;
		std::vector<float> FrameTime;
		std::map<int, EngineDelegate> Events;

		int CurIndex = 0;
		int ResultIndex = 0;
		float CurTime = 0.0f;
		bool Loop = true;

		void Reset()
		{
			CurIndex = 0;
			CurTime = 0;
			ResultIndex = 0;
		}
	};


public:
	// constrcuter destructer
	USpriteRenderer();
	~USpriteRenderer();

	// delete Function
	USpriteRenderer(const USpriteRenderer& _Other) = delete;
	USpriteRenderer(USpriteRenderer&& _Other) noexcept = delete;
	USpriteRenderer& operator=(const USpriteRenderer& _Other) = delete;
	USpriteRenderer& operator=(USpriteRenderer&& _Other) noexcept = delete;

	void Render(float _DeltaTime);
	void BeginPlay() override;
	void ComponentTick(float _DeltaTime) override;

	// int를 주는 함수들은 일반적으로 Enum으로 대체해서 넣고 싶을때가 많다.
	// 그런데 그건 커텐츠 만드는 사람이 만드는 자신만의 enum일 것이기 때문에 
	// 템플릿을 사용하여 어떤 enum이건 받게 만드는 방식을 선호한다.
	template<typename EnumType>
	void SetOrder(EnumType _Order)
	{
		SetOrder(static_cast<int>(_Order));
	}

	void SetOrder(int _Order);

	int GetOrder()
	{
		return Order;
	}

	FVector2D SetSpriteScale(float _Ratio = 1.0f, int _CurIndex = 0);


	void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, int _Start, int _End, float Time = 0.1f, bool _Loop = true);

	// _AnimationName 이름의 애니메이션을 만들어라.
	// _SpriteName 이 이름의 스프라이트로
	// _Indexs 프레임은 이녀석들을 사용해서   0     1      2     3     4     5 
	// _Frame 이 시간을 드려서                 0.1    0.1    0.1   0.1  0.1   0.1
	// _Loop = true면 반복 false면 마지막 프레임에서 정지
	void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, std::vector<float> _Frame, bool _Loop = true);

	void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, float _Frame, bool _Loop = true);

	// 내가 Idle인데 Idle 바꾸라고 했다. 
	void ChangeAnimation(std::string_view _AnimationName, bool _Force = false);

	void SetAnimationEvent(std::string_view _AnimationName, int _Frame, std::function<void()> _Function);

	std::string GetCurSpriteName()
	{
		return Sprite->GetName();
	}


	void SetCameraEffect(bool _Value)
	{
		IsCameraEffect = _Value;
	}

	void SetPivot(FVector2D _Pivot)
	{
		Pivot = _Pivot;
	}

	void SetPivotType(PivotType _Type);

	void SetCameraEffectScale(float _Effect);
	void SetSprite(std::string_view _Name, int _CurIndex = 0);

protected:

private:
	int Order = 0;
	int CurIndex = 0;
	bool IsCameraEffect = true;
	float CameraEffectScale = 1.0f;
	FVector2D Pivot = FVector2D::ZERO;

	class UEngineSprite* Sprite = nullptr;

	std::map<std::string, FrameAnimation> FrameAnimations;
	FrameAnimation* CurAnimation = nullptr;
};

