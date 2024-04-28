#include "injector.h"


bool injector::c_injector::inject() {

    DWORD pid = get_process_id();

    if (pid == NULL)
        return false;
    
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);

    if (!process || process == INVALID_HANDLE_VALUE)
        return false;

    void* alloc_memory = VirtualAllocEx(process, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    WriteProcessMemory(process, alloc_memory, dll_path.c_str(), dll_path.size() + 1, 0);

    CloseHandle(CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, alloc_memory, 0, 0));
    CloseHandle(process);

    return true;
}


DWORD injector::c_injector::get_process_id() {

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (snapshot == INVALID_HANDLE_VALUE)
        return NULL;

    PROCESSENTRY32 process_entry;
    process_entry.dwSize = sizeof(PROCESSENTRY32);

    for (
        bool next = Process32First(snapshot, &process_entry); 
        next; 
        next = Process32Next(snapshot, &process_entry)) 
    {

        if (process_name == (std::string) process_entry.szExeFile)
            return process_entry.th32ProcessID;

    }

    CloseHandle(snapshot);
    return NULL;
}