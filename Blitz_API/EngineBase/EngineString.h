#pragma once
#include <string>

// ���� : ���ڿ��� ���õ� static �Լ��鸸 ����̴ϴ�.
//        ����Ŭ���� ���ڿ��� �����ִ� Ŭ����
class UEngineString
{
public:
	~UEngineString();

	static std::string ToUpper(std::string_view);

	static std::wstring AnsiToUnicode(std::string_view _Name);

private:
	UEngineString();
};

