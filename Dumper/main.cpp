#include "main.h"

void main::thread(HMODULE dll) {

	hook::instance = std::make_unique<hook::c_hook>();

	if (!hook::instance->attach())
		FreeLibraryAndExitThread(dll, 0);

	if (!hook::instance->set_callbacks()) {

		hook::instance->dettach();
		FreeLibraryAndExitThread(dll, 0);
	}

	std::cout << " - Hook succesfuly, starting dumping - " << std::endl;

	for (; !GetAsyncKeyState(VK_END); );

	hook::instance->dettach();
	FreeLibraryAndExitThread(dll, 0);
}