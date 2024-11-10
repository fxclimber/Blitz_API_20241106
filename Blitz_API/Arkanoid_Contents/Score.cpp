#include "PreCompiledFile.h"
#include "Score.h"

int AScore::ScoreUI = 0; // ���� ���� ����

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

//void AScore::SetValue(int _Score)
//{
//	std::string Number = std::to_string(_Score);
//
//	// 2000;
//	// 0000002000
//	if (Renders.size() <= Number.size())
//	{
//		MSGASSERT("�ڸ����� �Ѱ���ϴ�.");
//		return;
//	}
//
//	FVector2D Pos = FVector2D::ZERO;
//
//	for (size_t i = 0; i < Number.size(); i++)
//	{
//		char Value = Number[i] - '0';
//		Renders[i]->SetSprite(TextSpriteName, Value);
//		Renders[i]->SetComponentScale(TextScale);
//		Renders[i]->SetComponentLocation(Pos);
//		Pos.X += TextScale.X;
//		Renders[i]->SetActive(true);
//	}
//
//	for (size_t i = Number.size(); i < Renders.size(); i++)
//	{
//		Renders[i]->SetActive(false);
//	}
//
//}


//void AScore::SetValue(int _Score)
//{
//    std::string Number = std::to_string(_Score);
//
//    // 8�ڸ��� �е�
//    if (Number.size() < 8)
//    {
//        Number = std::string(8 - Number.size(), '0') + Number;
//    }
//
//    if (Renders.size() < 8)
//    {
//        MSGASSERT("�������� �ڸ����� ������� �ʽ��ϴ�.");
//        return;
//    }
//
//    FVector2D Pos = FVector2D::ZERO;
//
//    // ������ ������ ���� ���� ��ġ�� �����մϴ�.
//    Pos.X = -8 * TextScale.X;  // 8�ڸ� ��ü ���̸� �������� ������ ������ ����
//
//    for (size_t i = 0; i < 8; i++)
//    {
//        char Value = Number[i] - '0';
//        Renders[i]->SetSprite(TextSpriteName, Value);
//        Renders[i]->SetComponentScale(TextScale);
//        Renders[i]->SetComponentLocation(Pos);
//        Pos.X += TextScale.X;
//        Renders[i]->SetActive(true);
//    }
//
//    for (size_t i = 8; i < Renders.size(); i++)
//    {
//        Renders[i]->SetActive(false);
//    }
//}


void AScore::SetValue(int _Score)
{
    std::string Number = std::to_string(_Score);

    // 8�ڸ��� �е�
    if (Number.size() < 8)
    {
        Number = std::string(8 - Number.size(), '0') + Number;
    }

    if (Renders.size() < 8)
    {
        MSGASSERT("�������� �ڸ����� ������� �ʽ��ϴ�.");
        return;
    }

    FVector2D Pos = FVector2D::ZERO;

    // �ڰ� ���� ���� (���ϴ� ������ ����)
    float LetterSpacing = -1.0f; // ���ϴ� �ڰ� ������ ����

    // ������ ������ ���� ���� ��ġ�� �����մϴ�.
    Pos.X = -8 * (TextScale.X + LetterSpacing);  // 8�ڸ� ��ü ���̸� �������� ������ ������ ����

    for (size_t i = 0; i < 8; i++)
    {
        char Value = Number[i] - '0';
        Renders[i]->SetSprite(TextSpriteName, Value);
        Renders[i]->SetComponentScale(TextScale);
        Renders[i]->SetComponentLocation(Pos);
        Pos.X += TextScale.X + LetterSpacing; // �ڰ��� �����Ͽ� ��ġ�� ����
        Renders[i]->SetActive(true);
    }

    for (size_t i = 8; i < Renders.size(); i++)
    {
        Renders[i]->SetActive(false);
    }
}
