////////////////////////////////////////////////////////////////////////////////
//                               --  THYME --                                 //
////////////////////////////////////////////////////////////////////////////////
//
//  Project Name:: Thyme
//
//          File:: DLLMAIN.CPP
//
//        Author:: OmniBlade
//
//  Contributors:: CCHyper
//
//   Description:: Defines the entry point for the DLL application.
//
//       License:: Thyme is free software: you can redistribute it and/or 
//                 modify it under the terms of the GNU General Public License 
//                 as published by the Free Software Foundation, either version 
//                 2 of the License, or (at your option) any later version.
//
//                 A full copy of the GNU General Public License can be found in
//                 LICENSE
//
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
//  Includes
////////////////////////////////////////////////////////////////////////////////
#include "hooker.h"
#include "archivefile.h"
#include "asciistring.h"
#include "copyprotect.h"
#include "force_nocd.h"
#include "gamememory.h"
#include "gamememoryinit.h"
#include "gamedebug.h"
#include "main.h"
#include "win32gameengine.h"
#include "win32localfilesystem.h"
#include <windows.h>
#include <stdarg.h>
#include <stdio.h>

void Setup_Hooks()
{
    //
    // Hook WinMain
    //
    Hook_StdCall_Function((Make_StdCall_Ptr<int, HINSTANCE, HINSTANCE, LPSTR, int>(0x00401700)), Main_Func);

	//
	// Code that checks the launcher is running, launcher does CD check.
	//
    Hook_Function((Make_Function_Ptr<void, unsigned int, int>(0x00412420)), CopyProtect::checkForMessage);
    Hook_Function((Make_Function_Ptr<bool>(0x00412450)), CopyProtect::validate);

    //
    // Returns true for any CD checks
    //
    Hook_Function((Make_Function_Ptr<bool>(0x004469C0)), IsFirstCDPresent);
    
    //
    // Replace memory allocation operators
    //
    Hook_Function((Make_Function_Ptr<void *, size_t>(0x00414450)), New_New);
    Hook_Function((Make_Function_Ptr<void *, size_t>(0x00414490)), New_Array_New);
    Hook_Function((Make_Function_Ptr<void, void *>(0x004144D0)), New_Delete);
    Hook_Function((Make_Function_Ptr<void, void *>(0x004144F0)), New_Array_Delete);
    Hook_Function((Make_Function_Ptr<MemoryPool *, const char *, int>(0x00414B30)), Create_Named_Pool);
    
    //
    // Replace pool functions
    //
    Hook_Method((Make_Method_Ptr<void *, MemoryPool>(0x00413C10)), &MemoryPool::Allocate_Block);
    Hook_Method((Make_Method_Ptr<void, MemoryPool, void *>(0x00413C40)), &MemoryPool::Free_Block);

    //
    // Replace DMA functions
    //
    Hook_Method((Make_Method_Ptr<void *, DynamicMemoryAllocator, int>(0x00413EE0)), &DynamicMemoryAllocator::Allocate_Bytes_No_Zero);
    Hook_Method((Make_Method_Ptr<void *, DynamicMemoryAllocator, int>(0x00413FE0)), &DynamicMemoryAllocator::Allocate_Bytes);
    Hook_Method((Make_Method_Ptr<void, DynamicMemoryAllocator, void *>(0x00414010)), &DynamicMemoryAllocator::Free_Bytes);
    Hook_Method((Make_Method_Ptr<int, DynamicMemoryAllocator, int>(0x00414140)), &DynamicMemoryAllocator::Get_Actual_Allocation_Size);
    
    //
    // Replace MemoryPoolFactory functions
    //
    Hook_Method((Make_Method_Ptr<MemoryPool *, MemoryPoolFactory, char const *, int, int, int>(0x00414180)), static_cast<MemoryPool *(MemoryPoolFactory::*const)(char const*, int, int, int)>(&MemoryPoolFactory::Create_Memory_Pool));
    
    //
    // Replace AsciiString
    //
    Hook_Method((Make_Method_Ptr<void, AsciiString, char const *>(0x0040D640)), static_cast<void (AsciiString::*)(char const*)>(&AsciiString::Set));
    Hook_Method((Make_Method_Ptr<void, AsciiString, int, bool, char const *, char const *>(0x00415290)), &AsciiString::Ensure_Unique_Buffer_Of_Size);
    Hook_Method((Make_Method_Ptr<void, AsciiString, char const *>(0x0040FB40)), &AsciiString::Concat);

    //
    // Replace FileSystem
    //
}

// Use DLLMain to Set up our hooks when the DLL loads. The launcher should stall
// the main thread at the entry point so hooked code called after that should
// be our code.
BOOL WINAPI DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch ( ul_reason_for_call ) {

        case DLL_PROCESS_ATTACH:
			StartHooking();
            Setup_Hooks();
            break;

        case DLL_PROCESS_DETACH:
            StopHooking();
            break;
            
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        default:
            break;

    }

    return TRUE;
}
