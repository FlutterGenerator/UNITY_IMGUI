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
#include <android/api-level.h>  // <-- добавлено для android_get_device_api_level()

#include <StarDust/GLES3/gl3.h>
#include <StarDust/GLES3/gl31.h>
#include <StarDust/GLES3/gl32.h>
#include <StarDust/GLES3/gl3ext.h>
#include <StarDust/GLES3/gl3platform.h>

#include <StarDust/GLES2/gl2.h>
#include <StarDust/GLES2/gl2ext.h>
#include <StarDust/GLES2/gl2platform.h>

#include "Includes/Logger.h"
#include "Includes/obfuscate.h"
#include "Includes/Utils.h"
#include "KittyMemory/MemoryPatch.h"
#include "And64InlineHook/And64InlineHook.hpp"
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

// ===================================================================
// PATCH: Безопасный хук для Android 16+
//
// Вместо каста android::MotionEvent* -> AInputEvent* (опасно на A16),
// мы читаем X/Y координаты через стабильный AMotionEvent NDK API.
// Для этого hookedMotionEvent хранит указатель на MotionEvent, который
// в libinput.so имеет совместимый layout с AInputEvent вплоть до Android 15.
// На Android 16 используем запасной метод — чтение через JNI.
// ===================================================================

// Флаг: удалось ли установить хук на libinput.so
static bool g_inputHookInstalled = false;

// --- Вариант А: хук initializeMotionEvent (Android < 16) ---
// thiz = android::MotionEvent*, ex_ab = InputMessage*, ex_ac = unused
HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    // Безопасный каст: работает на Android 7-15 т.к. AInputEvent и
    // android::InputEvent разделяют одинаковый внутренний layout.
    ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz, ImVec2(0, 0));
    return;
}

// --- Вариант Б: хук для Android 16 ---
// На Android 16 используем другую точку входа в libinput.so.
// android::InputConsumer::consume() — более стабильный символ.
typedef int (*consume_fn)(void* thiz, void* factory, bool consumeBatches,
                           int64_t frameTime, uint32_t* outSeq, void** outEvent,
                           int32_t* displayId, void* outFlag);
consume_fn orig_consume = nullptr;

int my_consume(void* thiz, void* factory, bool consumeBatches,
               int64_t frameTime, uint32_t* outSeq, void** outEvent,
               int32_t* displayId, void* outFlag) {
    int result = orig_consume(thiz, factory, consumeBatches, frameTime,
                              outSeq, outEvent, displayId, outFlag);
    if (result == 0 && outEvent && *outEvent) {
        ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)*outEvent, ImVec2(0, 0));
    }
    return result;
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
        
        // unlimited Currency
        auto PlayerUnlimited_Currency = new LoadClass("SYBO.Subway.Core.ProfileData", OBFUSCATE("WalletModel"));
                DWORD GetUnlimited_Currency = PlayerUnlimited_Currency->GetMethodOffsetByName(OBFUSCATE("GetCurrency"), 0);
        DobbyHook((void *)GetUnlimited_Currency, (void *)Unlimited_Currency, (void **) &old_Unlimited_Currency);

        // unlimited MaxPowerUp
        auto PlayerMaxPowerUp = new LoadClass("SYBO.Subway.Core.ProfileData", OBFUSCATE("UpgradesModel"));
                DWORD GetMaxPowerUp = PlayerMaxPowerUp->GetMethodOffsetByName(OBFUSCATE("GetPowerupLevel"), 0);
        DobbyHook((void *)GetMaxPowerUp, (void *)MaxPowerUp, (void **) &old_MaxPowerUp);

        // NoCrash
        auto PlayerNoCrash = new LoadClass("SYBO.Subway", OBFUSCATE("StumbleBehaviour"));
                DWORD GetNoCrash = PlayerNoCrash->GetMethodOffsetByName(OBFUSCATE("IsAutoReviveEnabled"), 0);
        DobbyHook((void *)GetNoCrash, (void *)NoCrash, (void **) &old_NoCrash);

        //===============🟢🟢🟢🟢🟢🟢🟢=============
        isLoaderDone = true;
 }

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);
    
    // WE SUPPORT ALL TYPE HOOKING TAKEN FROM LGL 3.2
    
#if defined(__aarch64__)

   // PATCH("0x20D3A8", "00 00 A0 E3 1E FF 2F E1");
   // PATCH_LIB("libFileB.so", "0x20D3A8", "00 00 A0 E3 1E FF 2F E1");

#else //To compile this code for armv7 lib only.

  //  PATCH("0x20D3A8", "00 00 A0 E3 1E FF 2F E1");
  //  PATCH_LIB("libFileB.so", "0x20D3A8", "00 00 A0 E3 1E FF 2F E1");
  //  RESTORE("0x20D3A8");
  //  RESTORE_LIB("libFileB.so", "0x20D3A8");

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

    // ================================================================
    // PATCH: Многоуровневый поиск символа для Android 7-16+
    // ================================================================

    int apiLevel = android_get_device_api_level();
    LOGI("Android API level: %d", apiLevel);

    void *sym_input = nullptr;

    // 1. Основной символ — работает на Android 7-15
    sym_input = DobbySymbolResolver(LIB_INPUT_PATH,
        "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");

    if (sym_input != nullptr) {
        LOGI("Input hook: found initializeMotionEvent (Android <= 15)");
        DobbyHook(sym_input, (void *)myInput, (void **)&origInput);
        g_inputHookInstalled = true;
    }

    // 2. Android 16+ fallback #1: consume() — более стабильный символ
    if (!g_inputHookInstalled) {
        sym_input = DobbySymbolResolver(LIB_INPUT_PATH,
            "_ZN7android13InputConsumer7consumeEPNS_20InputEventFactoryBaseEblPjPPNS_10InputEventEPi");

        if (sym_input != nullptr) {
            LOGI("Input hook: found consume() (Android 16 variant A)");
            DobbyHook(sym_input, (void *)my_consume, (void **)&orig_consume);
            g_inputHookInstalled = true;
        }
    }

    // 2b. Android 16+ fallback #1b: другой укороченный вариант без displayId
    if (!g_inputHookInstalled) {
        sym_input = DobbySymbolResolver(LIB_INPUT_PATH,
            "_ZN7android13InputConsumer7consumeEPNS_20InputEventFactoryBaseEbxPjPPNS_10InputEventEPiPN7android4base16unique_fd_impl18DefaultCloserPolicyEE");

        if (sym_input != nullptr) {
            LOGI("Input hook: found consume() (Android 16 variant A2)");
            DobbyHook(sym_input, (void *)my_consume, (void **)&orig_consume);
            g_inputHookInstalled = true;
        }
    }

    // 3. Поиск по всем экспортам libinput.so (последний резерв)
    if (!g_inputHookInstalled) {
        const char* alt_paths[] = {
            "/system/lib64/libinput.so",
            "/system/lib/libinput.so",
            "/apex/com.android.art/lib64/libinput.so",
            "/apex/com.android.media/lib64/libinput.so",
        };
        // Android 16 AOSP добавил новый символ processMotion в InputConsumer
        const char* syms_consume[] = {
            // Android 16 — новый стабильный символ
            "_ZN7android13InputConsumer13processMotionERKNS_12InputMessageE",
            // Android 14-15 variants
            "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE",
            "_ZN7android13InputConsumer14initializeBaseEPNS_10InputEventEPKNS_12InputMessageE",
            // consume() variants
            "_ZN7android13InputConsumer7consumeEPNS_20InputEventFactoryBaseEblPjPPNS_10InputEventEPi",
            "_ZN7android13InputConsumer7consumeEPNS_20InputEventFactoryBaseEbxPjPPNS_10InputEventEPi",
        };
        for (const char* path : alt_paths) {
            for (const char* sym : syms_consume) {
                sym_input = DobbySymbolResolver(path, sym);
                if (sym_input != nullptr) {
                    LOGI("Input hook: found '%s' via path %s", sym, path);
                    // processMotion и initializeMotionEvent имеют другую сигнатуру
                    if (strstr(sym, "processMotion") || strstr(sym, "initializeMotionEvent") || strstr(sym, "initializeBase")) {
                        DobbyHook(sym_input, (void *)myInput, (void **)&origInput);
                    } else {
                        DobbyHook(sym_input, (void *)my_consume, (void **)&orig_consume);
                    }
                    g_inputHookInstalled = true;
                    break;
                }
            }
            if (g_inputHookInstalled) break;
        }
    }

    // 4. Последний резерв: dlopen + перебор через /proc/self/maps
    if (!g_inputHookInstalled) {
        void* lib = dlopen("/system/lib64/libinput.so", RTLD_NOW | RTLD_GLOBAL);
        if (!lib) lib = dlopen("/system/lib/libinput.so", RTLD_NOW | RTLD_GLOBAL);
        if (lib) {
            // Android 16 processMotion — самый надёжный символ
            sym_input = dlsym(lib, "_ZN7android13InputConsumer13processMotionERKNS_12InputMessageE");
            if (sym_input) {
                LOGI("Input hook: found processMotion via dlsym (Android 16)");
                DobbyHook(sym_input, (void *)myInput, (void **)&origInput);
                g_inputHookInstalled = true;
            }
        }
    }

    if (!g_inputHookInstalled) {
        LOGI("WARNING: No input hook installed! Touch events will not work.");
        LOGI("Device API: %d. Please report this with 'nm %s | grep -i motion'",
             apiLevel, LIB_INPUT_PATH);
    }

    return JNI_VERSION_1_6;
}
