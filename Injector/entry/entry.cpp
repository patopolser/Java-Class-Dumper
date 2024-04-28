#include "../injector/injector.h"

int __fastcall main() {

	std::string process_name;

	std::cout << "Process name (default: javaw.exe): ";
	std::getline(std::cin, process_name);

	process_name = process_name.size() < 4 ? "javaw.exe" : process_name;

	injector::instance = std::make_unique<injector::c_injector>(process_name);

	for (
		std::cout << "Waiting for " << process_name; 
		!injector::instance->inject();
		Sleep(200) // if a process crash add more delay
	);

	std::cout << "\nDumper injected! - Check C:\\Class-Dumper for classes" << std::endl;

	Sleep(5000);

	return 0;
}