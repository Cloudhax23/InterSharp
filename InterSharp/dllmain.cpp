// dllmain.cpp : Defines the entry point for the DLL application.

#include "stdafx.h"
#include "intercept.hpp"

typedef bool(__cdecl* fnBootstrap)();

int intercept::api_version() {
	return 1;
}

namespace InterSharp
{
	namespace Native
	{
		bool Initialize()
		{
			FreeConsole();
			AllocConsole();
			freopen("CONOUT$", "w", stdout);

			LoadLibraryA("InterSharpWrapper.dll");

			HINSTANCE BootStrap = LoadLibraryA("InterSharpManaged.dll");
			fnBootstrap StartBootstrap = (fnBootstrap)GetProcAddress(BootStrap, "InitBootstrap");

			if (StartBootstrap())
				std::cout << "Bootstrap success!";
			else
				std::cout << "ERROR! Bootstrap failed!";
			return true;
		}
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(InterSharp::Native::Initialize), nullptr, NULL, nullptr);
		break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

