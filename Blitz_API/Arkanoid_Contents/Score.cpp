#include "PreCompiledFile.h"
#include "Score.h"

#include <EngineBase/EngineDirectory.h>
#include <EngineCore/EngineAPICore.h>

#include "Map_Play.h"
#include "Map_Die.h"
#include "TestGameMode.h"
#include "DieGameMode.h"
#include <EnginePlatform/EngineInput.h>






int AScore::ScoreUI = 0; // ���� ���� ����
int AScore::FinalScoreUI = 0; // ���� ���� ���� �� �ʱ�ȭ
int AScore::ElapsedTime = 0;
int AScore::FinalElapsedTime = 0;

AScore::AScore()
{
	for (size_t i = 0; i < 10; i++)
	{
		// ī�޶� �������� �̳༮�� �������� �ʴ´�.
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

    // 8�ڸ��� �е�
    if (Number.size() < _Padding)
    {
        Number = std::string(_Padding - Number.size(), '0') + Number;
    }

    if (Renders.size() < _Padding)
    {
        MSGASSERT("�������� �ڸ����� ������� �ʽ��ϴ�.");
        return;
    }

    FVector2D Pos = FVector2D::ZERO;

    // �ڰ� ���� ���� (���ϴ� ������ ����)
    float LetterSpacing = -1.0f; // ���ϴ� �ڰ� ������ ����

    // ������ ������ ���� ���� ��ġ�� �����մϴ�.
    Pos.X = -_Padding * (TextScale.X + LetterSpacing);  // 8�ڸ� ��ü ���̸� �������� ������ ������ ����

    for (size_t i = 0; i < _Padding; i++)
    {
        char Value = Number[i] - '0';
        Renders[i]->SetSprite(TextSpriteName, Value);
        Renders[i]->SetComponentScale(TextScale);
        Renders[i]->SetComponentLocation(Pos);
        Pos.X += TextScale.X + LetterSpacing; // �ڰ��� �����Ͽ� ��ġ�� ����
        Renders[i]->SetActive(true);
    }

    for (size_t i = _Padding; i < Renders.size(); i++)
    {
        Renders[i]->SetActive(false);
    }
}

