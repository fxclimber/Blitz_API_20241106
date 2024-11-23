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
    for (size_t i = 0; i < 10; i++)
    {
        // ī�޶� �������� �̳༮�� �������� �ʴ´�.
        USpriteRenderer* Sprite = CreateDefaultSubObject<USpriteRenderer>();
        Sprite->SetCameraEffect(false);
        Renders.push_back(Sprite);
    }
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
    // ���ڰ���
    size_t TextLength = _Text.size();

    if (TextLength > Renders.size())
    {
            MSGASSERT("��������Ʈ ������ ������ �����մϴ�.");
            return;
    }
    // ���ڰ�����ŭ�� ��ȸ�Ѵ�
    for (size_t i = 0; i < TextLength; i++)
    {
        //���ĺ��� ASCII ���������� �ε��� ����
        char Character = _Text[i];
        // �ҹ��� ->�빮��
        if (Character >= 'a' && Character <= 'z')
        {
            Character -= ('a' - 'A'); 
        }
        // ����
        if (Character < 'A' || Character > 'Z')
        {
            MSGASSERT("���ĺ� �� ���ڴ� ó���� �� �����ϴ�.");
            continue;
        }
        // �ε���
        int Index = Character - 'A';

        Renders[i]->SetSprite(TextSpriteName, Index);
        Renders[i]->SetComponentScale(TextScale);
        Renders[i]->SetComponentLocation(Pos);
        Renders[i]->SetActive(true);
        // ��ġ�� ũ��+�ڰ�
        Pos.X += spacing+ TextScale.X;

    }
    // �Ⱦ��� ��������Ʈ ��Ȱ�� 
    for (size_t i = 26; i < Renders.size(); i++)
    {
        Renders[i]->SetActive(false);
    }

}

