#include <Windows.h>
#include <thread>

#define JUMP_ADDR 0x184BE00
#define LOCAL_PLAYER_OFFSET 0x18530D0
#define FLAGS_OFFSET 0x3EC
#define VK_XBUTTON1 0x05

DWORD64 client = 0;
DWORD64 getLocalPlayer() {
    return *(DWORD64*)(client + LOCAL_PLAYER_OFFSET);
}

int getFlags(DWORD64 localPlayer) {
    return *(int*)(localPlayer + FLAGS_OFFSET);
}

void bhopThread() {
    while (true) {
        if (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) {
            DWORD64 localPlayer = getLocalPlayer();
            if (!localPlayer) continue;

            int flags = getFlags(localPlayer);
            if (flags == 65667 || flags == 65665) {
                *(int*)(client + JUMP_ADDR) = 65537;
                Sleep(10);
                *(int*)(client + JUMP_ADDR) = 256;
                Sleep(10);
            }
        }
        Sleep(1);
    }
}

DWORD WINAPI MainThread(HMODULE hModule) {
    // Wait until client.dll is loaded
    while (!client) {
        client = (DWORD64)GetModuleHandle("client.dll");
        Sleep(100);
    }

    std::thread(bhopThread).detach();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, 0, nullptr);
    }
    return TRUE;
}
