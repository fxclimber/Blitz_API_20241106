#pragma once
#include <string>

// 설명 : 문자열에 관련된 static 함수들만 만들겁니다.
//        헬퍼클래스 문자열에 도움주는 클래스
class UEngineString
{
public:
	~UEngineString();

	static std::string ToUpper(std::string_view);

	static std::wstring AnsiToUnicode(std::string_view _Name);

private:
	UEngineString();
};

