#pragma once

namespace UEngineDebug
{
	enum class EDebugPosType
	{
		Rect,
		Circle,
	};

	// MainWindow가 있다.

	void SetIsDebug(bool _IsDebug);

	void SwitchIsDebug();

	void CoreOutPutString(std::string_view _Text);

	void CoreOutPutString(std::string_view _Text, FVector2D _Pos);

	void CoreDebugRender(FTransform _Trans, EDebugPosType _Type);

	// 분리해야 하겠네요

	void PrintEngineDebugRender();
}