## Runtime Java Class Dumper 
Runtime dumping java classes using JNI

### Demonstration

[![Runtime Java Class Dumper](https://img.youtube.com/vi/-R28OsJ4qug/0.jpg)](https://www.youtube.com/watch?v=-R28OsJ4qug)

### How it works?
To be able to dump the classes of a Java program at runtime, we need to have access to them when they are being loaded. For this, we use the JNI (Java Native Interface) library, which provides us with the means to do so. 

#### First, we obtain the JVM, and then the JVMTI (Java Virtual Machine Tool Interface).

```
FARPROC processAddress = GetProcAddress(reinterpret_cast<HMODULE>(jvm), "JNI_GetCreatedJavaVMs");
t_createdvms created_java_vms = reinterpret_cast<t_createdvms>(processAddress);

created_java_vms(&vm, 1, nullptr);
vm->GetEnv(reinterpret_cast<void**>(&jvmti_env), JVMTI_VERSION_1_1);
```

#### After this, we will use the JVMTI to handle events through callbacks. [Documentation](https://docs.oracle.com/javase/8/docs/platform/jvmti/jvmti.html#Events).

```
jvmtiEventCallbacks callbacks;
memset(&callbacks, 0, sizeof(callbacks));

callbacks.ClassFileLoadHook = &__callback_class_file_load_hook;

jvmti_env->SetEventCallbacks(&callbacks, (jint) sizeof(callbacks));	
jvmti_env->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);
```

#### The event we are looking for is [ClassFileLoadHook](https://docs.oracle.com/javase/8/docs/platform/jvmti/jvmti.html#ClassFileLoadHook). This event is sent when the VM obtains class file data.

```
void __stdcall __callback_class_file_load_hook(jvmtiEnv* jvmti_env,
	JNIEnv* jni_env,
	jclass class_being_redefined,
	jobject loader,
	const char* name,
	jobject protection_domain,
	jint class_data_len,
	const unsigned char* class_data,
	jint* new_class_data_len,
	unsigned char** new_class_data)

```

#### To conclude, we read the classes using the parameters of our callback, and upon dettach our DLL, we clean up the callbacks and dettach our thread.

```
	jvmti_env->SetEventCallbacks(NULL, NULL);
	jvmti_env->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, NULL);

	vm->DetachCurrentThread();
```
