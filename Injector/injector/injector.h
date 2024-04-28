#pragma once

#include <iostream>
#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <filesystem>

namespace injector {

	class c_injector
	{
	public:
		c_injector(std::string process_name) { this->process_name = process_name; }
	
		DWORD get_process_id();

		bool inject();

	private:
		std::string process_name;
		std::string dll_path = std::filesystem::current_path().string() + "\\Dumper.dll";
	};

	inline std::unique_ptr<c_injector> instance;
}

