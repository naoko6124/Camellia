#pragma once

#include "Core/Application.h"

#ifdef _WIN32

#include <Windows.h>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	Camellia::Application game;
	game.Run();
	return 0;
}


#endif