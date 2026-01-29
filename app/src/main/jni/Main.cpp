#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <iostream>
#include <EGL/egl.h>
#include <map>

#include <StarDust/GLES3/gl3.h>
#include <StarDust/GLES3/gl3ext.h>
#include <StarDust/GLES3/gl3platform.h>

#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
//#include "And64InlineHook/And64InlineHook.hpp"
#include "StarDust/Dobby/dobby.h"
#include "StarDust/Fonts/GoogleSans.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_android.h"
#include "AutoHook/AutoHook.h"
#include "Includes/Macros.h"

#include "Global.h"
#include "MenuUi.h"

#include "Esp/Inc.h"

#define targetLibName OBFUSCATE("libil2cpp.so")

uintptr_t address;

HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
    return;
}

static int oneMenuX = 0;
static int oneMenuY = 0;


void *hack_thread(void *) {

    do {
        sleep(5);
    } while (!isLibraryLoaded(targetLibName));

        // Anti-Lib Rename !
        do {
        sleep(2);
    } while (!isLibraryLoaded("libstardust.so"));

if (!isLoaderDone) {

        //===============🟢🟢🟢🟢🟢🟢🟢=============

        // Auto update offset - Namespace > class > method
    
        // unlimited jump hack
        auto PlayerUnlimited_jump = new LoadClass("SYBO.RunnerCore.Character", OBFUSCATE("CharacterMotorAbilities"));
		DWORD GetUnlimited_jump = PlayerUnlimited_jump->GetMethodOffsetByName(OBFUSCATE("get_JumpLimit"), 0);
        DobbyHook((void *)GetUnlimited_jump, (void *)Unlimited_jump, (void **) &old_Unlimited_jump);
    
        // Free shop hack
        auto PlayerFree_Shop = new LoadClass("SYBO.Subway.Core.CommonData", OBFUSCATE("Currency"));
		DWORD GetFree_Shop = PlayerFree_Shop->GetMethodOffsetByName(OBFUSCATE("get_IsIAP"), 0);
        DobbyHook((void *)GetFree_Shop, (void *)Free_Shop, (void **) &old_Free_Shop);
    
        //===============🟢🟢🟢🟢🟢🟢🟢=============
        isLoaderDone = true;
 }

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

        // WE SUPPORT ALL TYPE HOOKING TAKEN FROM LGL 3.2

#if defined(__aarch64__) 

    // Patching offsets directly. Strings are automatically obfuscated too!
   // PATCH("0x20D3A8", "00 00 A0 E3 1E FF 2F E1");
    // PATCH_LIB("libFileB.so", "0x20D3A8", "00 00 A0 E3 1E FF 2F E1");


#else //To compile this code for armv7 lib only.

  //  PATCH("0x20D3A8", "00 00 A0 E3 1E FF 2F E1");
  //  PATCH_LIB("libFileB.so", "0x20D3A8", "00 00 A0 E3 1E FF 2F E1");

    //Restore changes to original
   // RESTORE("0x20D3A8");
   // RESTORE_LIB("libFileB.so", "0x20D3A8");

    LOGI(OBFUSCATE("Done"));
#endif


        pthread_exit(nullptr);
    return nullptr;

}

void hexpatcher() {
            // PATCH_SWITCH_EXT
        // Additional macro to apply hex patches from an external const char* source.
        // All external hex patches are added in global.h; you can add more there if desired.
                // PATCH_SWITCH_EXT Additionl macro to use hex patches from external const char*


                //More Switches
                //PATCH_SWITCH_AU(OFFSET.GetMaxRank, "0A 00 A0 E3 1E FF 2F E1", isMaxLevel);
            //PATCH_SWITCH("0x1234",  "0A 00 A0 E3 1E FF 2F E1", isMaxLevel)
                //PATCH_LIB_SWITCH("libil2cpp.so", "0x1234", "0A 00 A0 E3 1E FF 2F E1", isMaxLevel)
}

void *imgui_go(void *) {
    uintptr_t address = findLibrary("libil2cpp.so");
    void *addr = dlsym(dlopen(libEGL, RTLD_NOW), "eglSwapBuffers");
    if (addr) {
        DobbyHook(addr, (void *)hook_eglSwapBuffers, (void **)&old_eglSwapBuffers);
    } else {
                LOGI(OBFUSCATE("eglSwapBuffers is not found"));
    }

    pthread_exit(nullptr);
    return nullptr;
}


__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, imgui_go, NULL);
    pthread_t hacks;
    pthread_create(&hacks, NULL, hack_thread, NULL);
}


JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    void *sym_input = DobbySymbolResolver(LIB_INPUT_PATH, "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");

    if (sym_input != NULL) {
        DobbyHook(sym_input, (void *)myInput, (void **)&origInput);
    }

    return JNI_VERSION_1_6;
}
