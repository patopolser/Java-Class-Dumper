#pragma once

#include <jni.h>
#include <jvmti.h>

#include <iostream>
#include <vector>
#include <windows.h>
#include <fstream>
#include <filesystem>

namespace hook {

	class c_hook
	{
	private:
		JNIEnv* env;
		jvmtiEnv* jvmti_env;
		JavaVM* vm;

		void logger(std::string text) { MessageBox(NULL, text.c_str(), "Logger", MB_ICONWARNING); }

	public:

		bool attach();
		void dettach();

		bool set_callbacks();
	};

	inline std::unique_ptr<c_hook> instance;
}