#pragma once
#include <filesystem>

// 설명 :
class UEnginePath
{
public:
	// constrcuter destructer
	UEnginePath();
	UEnginePath(std::string_view _Path);
	UEnginePath(std::filesystem::path _Path);
	~UEnginePath();

	//// delete Function
	//UEnginePath(const UEnginePath& _Other) = delete;
	//UEnginePath(UEnginePath&& _Other) noexcept = delete;
	//UEnginePath& operator=(const UEnginePath& _Other) = delete;
	//UEnginePath& operator=(UEnginePath&& _Other) noexcept = delete;

	bool IsExists();
	void MoveParent();

	std::string GetPathToString();

	// 파일명 + 확장자 포함
	std::string GetFileName();

	// 파일명 + 확장자 포함
	std::string GetDirectoryName();

	// 확장자
	std::string GetExtension();


	// "C:\\AAAA\\BBBB\\";
	// "C:\\AAAA\\BBBB\\Resources";
	// "C:\\AAAA\\"
	// "C:\\AAAA\\Resources"
	// "C:\\"
	// "C:\\Resources"
	// 특정 디렉토리가 나올때까지 MoveParent를 반복하는 함수.
	bool MoveParentToDirectory(std::string_view _Path);

	bool IsDirectory();

	bool IsFile();

	void Append(std::string_view _AppendName);


protected:
	std::filesystem::path Path;

private:
};

