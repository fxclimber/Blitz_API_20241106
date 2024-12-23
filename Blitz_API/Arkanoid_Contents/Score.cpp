#include "PreCompiledFile.h"
#include "Score.h"

#include <EngineBase/EngineDirectory.h>
#include <EngineCore/EngineAPICore.h>

#include "Map_Play.h"
#include "Map_Die.h"
#include "TestGameMode.h"
#include "DieGameMode.h"
#include <EnginePlatform/EngineInput.h>






int AScore::ScoreUI = 0; // 정적 변수 정의
int AScore::FinalScoreUI = 0; // 정적 변수 정의 및 초기화
int AScore::ElapsedTime = 0;
int AScore::FinalElapsedTime = 0;

AScore::AScore()
{
	for (size_t i = 0; i < 10; i++)
	{
		// 카메라가 움직여도 이녀석은 움직이지 않는다.
		USpriteRenderer* Sprite = CreateDefaultSubObject<USpriteRenderer>();
		Sprite->SetCameraEffect(false);
		Renders.push_back(Sprite);
	}
}


AScore::~AScore()
{
}

void AScore::SetTextSpriteName(const std::string _Text)
{
	TextSpriteName = _Text;

	for (size_t i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetSprite(TextSpriteName);
	}
}


void AScore::SetOrder(int _Order)
{
	for (size_t i = 0; i < Renders.size(); i++)
	{
		Renders[i]->SetOrder(_Order);
	}

}


void AScore::SetValue(int _Score , int _Padding)
{
    std::string Number = std::to_string(_Score);

    // 8자리로 패딩
    if (Number.size() < _Padding)
    {
        Number = std::string(_Padding - Number.size(), '0') + Number;
    }

    if (Renders.size() < _Padding)
    {
        MSGASSERT("렌더링할 자리수가 충분하지 않습니다.");
        return;
    }

    FVector2D Pos = FVector2D::ZERO;

    // 자간 조절 변수 (원하는 값으로 설정)
    float LetterSpacing = -1.0f; // 원하는 자간 값으로 설정

    // 오른쪽 정렬을 위해 시작 위치를 설정합니다.
    Pos.X = -_Padding * (TextScale.X + LetterSpacing);  // 8자리 전체 길이를 기준으로 오른쪽 끝에서 시작

    for (size_t i = 0; i < _Padding; i++)
    {
        char Value = Number[i] - '0';
        Renders[i]->SetSprite(TextSpriteName, Value);
        Renders[i]->SetComponentScale(TextScale);
        Renders[i]->SetComponentLocation(Pos);
        Pos.X += TextScale.X + LetterSpacing; // 자간을 적용하여 위치를 조정
        Renders[i]->SetActive(true);
    }

    for (size_t i = _Padding; i < Renders.size(); i++)
    {
        Renders[i]->SetActive(false);
    }
}

