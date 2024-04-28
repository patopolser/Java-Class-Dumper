#include "hook.h"

void __stdcall __callback_class_file_load_hook(jvmtiEnv* jvmti_env,
	JNIEnv* jni_env,
	jclass class_being_redefined,
	jobject loader,
	const char* name,
	jobject protection_domain,
	jint class_data_len,
	const unsigned char* class_data,
	jint* new_class_data_len,
	unsigned char** new_class_data) {

	
	if (class_data_len <= 0 || name == NULL || class_data == NULL)
		return;

	std::string path_name = "C:/Class-Dumper/Dump/" + std::string(name) + ".class";

	std::filesystem::path path = std::filesystem::path(path_name);

	if (!std::filesystem::exists(path.parent_path()) && !std::filesystem::create_directories(path.parent_path()))
		return;

	std::ofstream outfile(path_name, std::ios_base::binary | std::ios_base::out);

	if (!outfile.is_open())
		return;

	std::cout << "Class loaded: " << name << std::endl;

	outfile.write(reinterpret_cast<const char*>(class_data), class_data_len);

	outfile.close();
}

bool hook::c_hook::set_callbacks() {

	jvmtiEventCallbacks callbacks;
	memset(&callbacks, 0, sizeof(callbacks));

	callbacks.ClassFileLoadHook = &__callback_class_file_load_hook;

	jvmtiError set_callback_error = jvmti_env->SetEventCallbacks(&callbacks, (jint) sizeof(callbacks));

	if (set_callback_error)
	{
		logger("SetEventCallback Failed");
		return false;
	}
	
	jvmtiError set_event_error = jvmti_env->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);

	if (set_event_error)
	{
		logger("SetEventNotificationMode Failed");
		return false;
	}
	
	return true;
}

bool hook::c_hook::attach()
{

	HMODULE jvm = GetModuleHandleA("jvm.dll");

	using t_createdvms = jint(__stdcall*)(JavaVM**, jsize, jsize*);

	FARPROC processAddress = GetProcAddress(reinterpret_cast<HMODULE>(jvm), "JNI_GetCreatedJavaVMs");
	t_createdvms created_java_vms = reinterpret_cast<t_createdvms>(processAddress);

	auto error = created_java_vms(&vm, 1, nullptr);

	if (error != JNI_OK) {
		logger("JNI_GetCreatedJavaVMs Failed");
		return false;
	}
		

	error = vm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);

	if (error != JNI_OK) {
		logger("AttachCurrentThread Failed");
		return false;
	}

	vm->GetEnv(reinterpret_cast<void**>(&jvmti_env), JVMTI_VERSION_1_1);

	if (!jvmti_env) {
		logger("GetEnv Failed");
		return false;
	}
	
	return true;
}

void hook::c_hook::dettach() {

	jvmti_env->SetEventCallbacks(NULL, NULL);
	jvmti_env->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);

	vm->DetachCurrentThread();

	env = nullptr;
	jvmti_env = nullptr;
	vm = nullptr;
}