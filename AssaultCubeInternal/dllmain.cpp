// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "mem.h"
#include "proc.h"

DWORD WINAPI HackThread(HMODULE hModule)
{
    //Create console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "TEST LINE" << std::endl;


    //Get module base
    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
    uintptr_t* ptrLocalPlayer = nullptr;
    uintptr_t ptrAmmo = 0;

    bool bHealth = false, bAmmo = false, bRecoil = false;
    
    //Hack loop
    while (true)
    {
        //Get key input
        if (GetAsyncKeyState(VK_END) & 1)
        {
            break;
        }

        if (GetAsyncKeyState(VK_NUMPAD1) & 1)
        {
            bHealth = !bHealth;
        }

        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            bAmmo = !bAmmo;
        }

        if (GetAsyncKeyState(VK_NUMPAD3) & 1)
        {
            bRecoil = !bRecoil;
            if (bRecoil)
            {
                //NOP
                mem::Nop((BYTE*)(moduleBase + 0x63786), 10);
            }
            else
            {
                //Write back original instructions
                // \x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2
                mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);
            }
        }

        //Continuous write/freeze
        //The game could end so we need to check if player still exists
        ptrLocalPlayer = (uintptr_t*)(moduleBase + 0x10f4f4);
        if (ptrLocalPlayer)
        {
            if (bHealth)
            {
                /*1.    Find the address where player object starts (*ptrLocalPlayer)
                * 2.    Shift 0xf8 forward in memory
                * 3.    Assume the result memory slot e.g. 0xabcd123 is actually a memory where
                *       there is an integer. Therefore we currently have a int* which has a value
                *       of 0xabcd123
                * 4.    Dereference 0xabcd123 (access whatever we assume to be at the address) and
                *       write 1337 to it.
                */
                *(int*)(*ptrLocalPlayer + 0xf8) = 1337;
            }
            if (bAmmo)
            {
                *(int*)mem::findDMAAddy(moduleBase + 0x10f4f4, { 0x374, 0x14, 0x0 }) = 123;
            }
            Sleep(5);
        }
    }


    //Cleanup & eject
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
    return 0;
}


BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}



