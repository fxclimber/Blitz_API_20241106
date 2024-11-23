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
        // 카메라가 움직여도 이녀석은 움직이지 않는다.
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

    // 8자리로 패딩
    if (Number.size() < 8)
    {
        Number = std::string(8 - Number.size(), '0') + Number;
    }

    if (Renders.size() < 8)
    {
        MSGASSERT("렌더링할 자리수가 충분하지 않습니다.");
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
    // 문자갯수
    size_t TextLength = _Text.size();

    if (TextLength > Renders.size())
    {
            MSGASSERT("스프라이트 렌더링 개수가 부족합니다.");
            return;
    }
    // 문자갯수만큼만 순회한다
    for (size_t i = 0; i < TextLength; i++)
    {
        //알파벳은 ASCII 값기준으로 인덱스 생성
        char Character = _Text[i];
        // 소문자 ->대문자
        if (Character >= 'a' && Character <= 'z')
        {
            Character -= ('a' - 'A'); 
        }
        // 예외
        if (Character < 'A' || Character > 'Z')
        {
            MSGASSERT("알파벳 외 문자는 처리할 수 없습니다.");
            continue;
        }
        // 인덱싱
        int Index = Character - 'A';

        Renders[i]->SetSprite(TextSpriteName, Index);
        Renders[i]->SetComponentScale(TextScale);
        Renders[i]->SetComponentLocation(Pos);
        Renders[i]->SetActive(true);
        // 위치는 크기+자간
        Pos.X += spacing+ TextScale.X;

    }
    // 안쓰는 스프라이트 비활성 
    for (size_t i = 26; i < Renders.size(); i++)
    {
        Renders[i]->SetActive(false);
    }

}

