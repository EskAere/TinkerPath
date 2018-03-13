#include <windows.h>
#include <iostream>
#include "Application.h"

int WINAPI WinMain(HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow)
{
	try
	{
		Application app;
		app.run();
	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION : " << e.what() << std::endl;
	}
}