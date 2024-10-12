#include "Misc/Utils.h"


std::string GetCurrentDLLPath() {
    HMODULE hModule = NULL;
    if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           reinterpret_cast<LPCSTR>(&GetCurrentDLLPath), &hModule)) {
        char dllPath[MAX_PATH];
        DWORD result = GetModuleFileNameA(hModule, dllPath, MAX_PATH);
        if (result != 0) {
            return std::string(dllPath);
        } else {
            spdlog::error("Failed to get DLL path.");
        }
    } else {
        spdlog::error("Failed to get DLL path.");
    }
    return "";
}