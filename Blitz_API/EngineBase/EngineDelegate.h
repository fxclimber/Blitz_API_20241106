#pragma once
#include <functional>
#include <list>

class EngineDelegate
{
public:
    EngineDelegate();

    EngineDelegate(std::function<void()> _Function)
    {
        // 객체 생성 시 전달된 함수를 리스트에 추가 (push_back)
        Functions.push_back(_Function);
    }

    ~EngineDelegate();

    bool IsBind()
    {
        // 함수가 리스트에 하나라도 있으면 true 반환 (바인딩 여부 확인)
        return false == Functions.empty();
    }

    void operator+=(std::function<void()> _Function)
    {
        //list 에 추가하는것이므로 += 로 변경
        Functions.push_back(_Function);
    }

    void operator()()
    {
        // 함수 호출 연산자
        // 리스트의 각 함수(콜백)를 순차적으로 호출
        std::list<std::function<void()>>::iterator StartIter = Functions.begin();
        std::list<std::function<void()>>::iterator EndIter = Functions.end();

        for (; StartIter != EndIter; ++StartIter)
        {
            // 현재 이터레이터가 가리키는 함수 참조
            std::function<void()>& Function = *StartIter;
            // 해당 함수 호출 (콜백 함수 실행)
            Function();
        }
    }

    void Clear()
    {
        // 리스트에 저장된 모든 함수 제거
        Functions.clear();
    }

protected:
private:
    std::list<std::function<void()>> Functions; // 콜백 함수들을 저장하는 리스트
};
