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
    //    // 카메라가 움직여도 이녀석은 움직이지 않는다.
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

    size_t TextLength = _Text.size();

    // Renders 크기 확인 및 조정
    if (TextLength > Renders.size()) {
        int PrevSize = Renders.size(); // 크기 부족 시 확장
        Renders.resize(TextLength);

        for (size_t i = PrevSize; i < Renders.size() - PrevSize; i++)
        {
            USpriteRenderer* Sprite = CreateDefaultSubObject<USpriteRenderer>();
            Sprite->SetCameraEffect(false);
            Renders[i] = Sprite;
        }
    }

    // 문자 갯수만큼만 순회
    for (size_t i = 0; i < TextLength; i++) {
        char Character = _Text[i];
        // 소문자 -> 대문자
        if (Character >= 'a' && Character <= 'z') {
            Character -= ('a' - 'A');
        }
        // 알파벳 외 문자 처리
        if (Character < 'A' || Character > 'Z') {
            std::cerr << "Error: Unsupported character in text!" << std::endl;
            continue;
        }
        int Index = Character - 'A';

        Renders[i]->SetSprite(TextSpriteName, Index);
        Renders[i]->SetComponentScale(TextScale);
        Renders[i]->SetComponentLocation(Pos);
        Renders[i]->SetActive(true);

        Pos.X += spacing + TextScale.X; // 위치 업데이트
    }

    // 사용되지 않는 렌더 비활성화
    for (size_t i = TextLength; i < Renders.size(); i++) {
        Renders[i]->SetActive(false);
    }
}

