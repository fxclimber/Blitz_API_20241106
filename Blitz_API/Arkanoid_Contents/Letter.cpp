#include "PreCompiledFile.h"
#include "Letter.h"

#include <EngineBase/EngineDirectory.h>
#include <EngineCore/EngineAPICore.h>

#include "Map_Play.h"
#include "Map_Die.h"
#include "TestGameMode.h"
#include "DieGameMode.h"
#include <EnginePlatform/EngineInput.h>






ALetter::ALetter()
{
    //for (size_t i = 0; i < 10; i++)
    //{
    //    // ī�޶� �������� �̳༮�� �������� �ʴ´�.
    //    USpriteRenderer* Sprite = CreateDefaultSubObject<USpriteRenderer>();
    //    Sprite->SetCameraEffect(false);
    //    Renders.push_back(Sprite);
    //}
}


ALetter::~ALetter()
{
}

void ALetter::SetTextSpriteName(const std::string _Text)
{
    TextSpriteName = _Text;

    for (size_t i = 0; i < Renders.size(); i++)
    {
        Renders[i]->SetSprite(TextSpriteName);
    }
}


void ALetter::SetOrder(int _Order)
{
    for (size_t i = 0; i < Renders.size(); i++)
    {
        Renders[i]->SetOrder(_Order);
    }

}


void ALetter::SetValue(int _Score)
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

    for (size_t i = 0; i < 8; i++)
    {
        char Value = Number[i] - '0';
        Renders[i]->SetSprite(TextSpriteName, Value);
        Renders[i]->SetComponentScale(TextScale);
        Renders[i]->SetComponentLocation(Pos);
        Renders[i]->SetActive(true);
    }

    for (size_t i = 8; i < Renders.size(); i++)
    {
        Renders[i]->SetActive(false);
    }
}

void ALetter::SetText(const std::string& _Text)
{
    std::string text = _Text;

    float spacing = 1.f;
    FVector2D Pos = FVector2D::ZERO;

    size_t TextLength = _Text.size();

    // Renders ũ�� Ȯ�� �� ����
    if (TextLength > Renders.size()) {
        int PrevSize = Renders.size(); // ũ�� ���� �� Ȯ��
        Renders.resize(TextLength);

        for (size_t i = PrevSize; i < Renders.size() - PrevSize; i++)
        {
            USpriteRenderer* Sprite = CreateDefaultSubObject<USpriteRenderer>();
            Sprite->SetCameraEffect(false);
            Renders[i] = Sprite;
        }
    }

    // ���� ������ŭ�� ��ȸ
    for (size_t i = 0; i < TextLength; i++) {
        char Character = _Text[i];
        // �ҹ��� -> �빮��
        if (Character >= 'a' && Character <= 'z') {
            Character -= ('a' - 'A');
        }
        // ���ĺ� �� ���� ó��
        if (Character < 'A' || Character > 'Z') {
            std::cerr << "Error: Unsupported character in text!" << std::endl;
            continue;
        }
        int Index = Character - 'A';

        Renders[i]->SetSprite(TextSpriteName, Index);
        Renders[i]->SetComponentScale(TextScale);
        Renders[i]->SetComponentLocation(Pos);
        Renders[i]->SetActive(true);

        Pos.X += spacing + TextScale.X; // ��ġ ������Ʈ
    }

    // ������ �ʴ� ���� ��Ȱ��ȭ
    for (size_t i = TextLength; i < Renders.size(); i++) {
        Renders[i]->SetActive(false);
    }
}

