#include "../main.h"

bool __stdcall DllMain(HINSTANCE dll, DWORD reason, LPVOID reserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		AllocConsole();
		
		freopen("CONOUT$", "w", stdout);

		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)main::thread, dll, 0, 0);
		
	}

	else if (reason == DLL_PROCESS_DETACH)
	{
		FreeConsole();
		fclose(stdout);
	}
	return true;
}