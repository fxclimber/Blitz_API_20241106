#pragma once
#include <functional>
#include <list>

class EngineDelegate
{
public:
    EngineDelegate();

    EngineDelegate(std::function<void()> _Function)
    {
        // ��ü ���� �� ���޵� �Լ��� ����Ʈ�� �߰� (push_back)
        Functions.push_back(_Function);
    }

    ~EngineDelegate();

    bool IsBind()
    {
        // �Լ��� ����Ʈ�� �ϳ��� ������ true ��ȯ (���ε� ���� Ȯ��)
        return false == Functions.empty();
    }

    void operator+=(std::function<void()> _Function)
    {
        //list �� �߰��ϴ°��̹Ƿ� += �� ����
        Functions.push_back(_Function);
    }

    void operator()()
    {
        // �Լ� ȣ�� ������
        // ����Ʈ�� �� �Լ�(�ݹ�)�� ���������� ȣ��
        std::list<std::function<void()>>::iterator StartIter = Functions.begin();
        std::list<std::function<void()>>::iterator EndIter = Functions.end();

        for (; StartIter != EndIter; ++StartIter)
        {
            // ���� ���ͷ����Ͱ� ����Ű�� �Լ� ����
            std::function<void()>& Function = *StartIter;
            // �ش� �Լ� ȣ�� (�ݹ� �Լ� ����)
            Function();
        }
    }

    void Clear()
    {
        // ����Ʈ�� ����� ��� �Լ� ����
        Functions.clear();
    }

protected:
private:
    std::list<std::function<void()>> Functions; // �ݹ� �Լ����� �����ϴ� ����Ʈ
};
