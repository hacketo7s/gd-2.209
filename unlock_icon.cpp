#include <Windows.h>
#include <MinHook.h>
#include <any>

using is_icon_unlocked_type = bool(*)(void*, int, std::any);
using is_color_unlocked_type = bool(*)(void*, int, std::any);

is_icon_unlocked_type o_icon_unlocked = nullptr;
is_color_unlocked_type o_color_unlocked = nullptr;

bool is_color_unlocked_detour(void* this_ptr, int key, std::any unlock_type) {
    return true;
}
bool is_icon_unlocked_detour(void* this_ptr, int key, std::any icon_type) {
    return true;
}

void mod() {   
    HMODULE base = GetModuleHandle(0);
    uintptr_t base_address = reinterpret_cast<uintptr_t>(base);

    uintptr_t is_color_unlocked_address = base_address + 0x3cff0;
    uintptr_t is_icon_unlocked_address = base_address + 0x274d60;

    is_color_unlocked_type is_color_unlocked = 
    reinterpret_cast<is_color_unlocked_type>(is_color_unlocked_address);
    is_icon_unlocked_type is_icon_unlocked = 
    reinterpret_cast<is_icon_unlocked_type>(is_icon_unlocked_address);

    MH_Initialize();

    // is color unlocked hook
    MH_CreateHook(
    reinterpret_cast<LPVOID>(is_color_unlocked), 
    reinterpret_cast<LPVOID>(&is_color_unlocked_detour),
    reinterpret_cast<LPVOID*>(&o_color_unlocked));
    
    // is icon unlocked hook
    MH_CreateHook(
    reinterpret_cast<LPVOID>(is_icon_unlocked),
    reinterpret_cast<LPVOID>(&is_icon_unlocked_detour),
    reinterpret_cast<LPVOID*>(&o_icon_unlocked)
    );

    MH_EnableHook(MH_ALL_HOOKS);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
       mod();
    }
    return TRUE;
}
