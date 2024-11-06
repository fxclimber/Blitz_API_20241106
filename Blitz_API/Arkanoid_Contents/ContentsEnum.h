#pragma once
#include "PreCompiledFile.h"
#include <EngineCore/EngineAPICore.h>
#include <EngineCore/SpriteRenderer.h>


enum class ERenderOrder
{
	BACKGROUND = -1000,
	Bricks = -500,
	Ball = -100,
	PLAYER = 0,
	NEW = 1000,
	UI = 2000,
};
enum class WhereIsBall
{
	NONE,
	TOP,
	BOTTOM,
	LEFT,
	RIGHT,
};

class Globals 
{
public:
	static FVector2D WinSize;

	static FVector2D BrickSize;

	static void setWinSize(const FVector2D& newSize)
	{
		WinSize = newSize;
	}
};

