#include "PreCompiledFile.h"
#include "Score.h"

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

void AScore::SetValue(int _Score)
{
	std::string Number = std::to_string(_Score);

	// 2000;
	// 0000002000
	if (Renders.size() <= Number.size())
	{
		MSGASSERT("�ڸ����� �Ѱ���ϴ�.");
		return;
	}

	Pos = FVector2D::ZERO;

	for (size_t i = 0; i < Number.size(); i++)
	{
		char Value = Number[i] - '0';
		Renders[i]->SetSprite(TextSpriteName, Value);
		Renders[i]->SetComponentScale(TextScale);
		Renders[i]->SetComponentLocation(Pos);
		Pos.X += TextScale.X;
		Renders[i]->SetActive(true);
	}

	for (size_t i = Number.size(); i < Renders.size(); i++)
	{
		Renders[i]->SetActive(false);
	}

}