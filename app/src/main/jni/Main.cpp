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
// INPUT HOOK — Android 7-16+
// ===================================================================

static bool g_inputHookInstalled = false;

// --- Вариант 1: initializeMotionEvent (Android <= 15) ---
HOOKAF(void, Input, void *thiz, void *ex_ab, void *ex_ac) {
    origInput(thiz, ex_ab, ex_ac);
    if (setup && thiz)
        ImGui_ImplAndroid_HandleInputEvent((AInputEvent *)thiz);
}

// --- Вариант 2: MotionEvent::initialize (Android 16) ---
// Символ подтверждён через readelf на реальном Android 16 устройстве.
// Сигнатура взята из AOSP android::MotionEvent::initialize
typedef void (*motioninit_fn)(
    void* thiz, int deviceId, uint32_t source, int32_t displayId,
    const void* hmac, int32_t action, int32_t actionButton, int32_t flags,
    int32_t edgeFlags, int32_t metaState, int32_t buttonState,
    int32_t classification, const void* transform,
    float xPrecision, float yPrecision,
    float rawXCursorPosition, float rawYCursorPosition,
    const void* rawTransform, int64_t downTime, int64_t eventTime,
    uint32_t pointerCount, const void* pointerProperties, const void* pointerCoords);

static motioninit_fn orig_motioninit = nullptr;

static void my_motioninit(
    void* thiz, int deviceId, uint32_t source, int32_t displayId,
    const void* hmac, int32_t action, int32_t actionButton, int32_t flags,
    int32_t edgeFlags, int32_t metaState, int32_t buttonState,
    int32_t classification, const void* transform,
    float xPrecision, float yPrecision,
    float rawXCursorPosition, float rawYCursorPosition,
    const void* rawTransform, int64_t downTime, int64_t eventTime,
    uint32_t pointerCount, const void* pointerProperties, const void* pointerCoords) {

    orig_motioninit(thiz, deviceId, source, displayId, hmac, action,
        actionButton, flags, edgeFlags, metaState, buttonState, classification,
        transform, xPrecision, yPrecision, rawXCursorPosition, rawYCursorPosition,
        rawTransform, downTime, eventTime, pointerCount, pointerProperties, pointerCoords);

    // setup — переменная из Global.h, true после инициализации ImGui
    if (setup && thiz)
        ImGui_ImplAndroid_HandleInputEvent((AInputEvent*)thiz);
}

// --- Вариант 3: resampleTouchState (Android 16 резерв) ---
typedef void (*resample_fn)(void*, int64_t, void*, const void*);
static resample_fn orig_resample = nullptr;

static void my_resample(void* thiz, int64_t frameTime, void* ev, const void* next) {
    orig_resample(thiz, frameTime, ev, next);
    if (setup && ev)
        ImGui_ImplAndroid_HandleInputEvent((AInputEvent*)ev);
}

static int oneMenuX = 0;
static int oneMenuY = 0;

void *hack_thread(void *) {

    do { sleep(5); } while (!isLibraryLoaded(targetLibName));
    do { sleep(2); } while (!isLibraryLoaded("libstardust.so"));

    // ================================================================
    // INPUT HOOK — делаем здесь, после загрузки всех библиотек
    // ================================================================
    void *sym_input = nullptr;

    // 1. Android <= 15
    sym_input = DobbySymbolResolver(LIB_INPUT_PATH,
        "_ZN7android13InputConsumer21initializeMotionEventEPNS_11MotionEventEPKNS_12InputMessageE");
    if (sym_input) {
        LOGI("Input hook: initializeMotionEvent (Android <= 15)");
        DobbyHook(sym_input, (void*)myInput, (void**)&origInput);
        g_inputHookInstalled = true;
    }

    // 2. Android 16 — MotionEvent::initialize (ПОДТВЕРЖДЁННЫЙ символ из readelf)
    if (!g_inputHookInstalled) {
        sym_input = DobbySymbolResolver(LIB_INPUT_PATH,
            "_ZN7android11MotionEvent10initializeEiijNS_2ui16LogicalDisplayIdENSt3__15arrayIhLm32EEEiiiiiiNS_20MotionClassificationERKNS1_9TransformEffffS9_llmPKNS_17PointerPropertiesEPKNS_13PointerCoordsE");
        if (sym_input) {
            LOGI("Input hook: MotionEvent::initialize (Android 16)");
            DobbyHook(sym_input, (void*)my_motioninit, (void**)&orig_motioninit);
            g_inputHookInstalled = true;
        }
    }

    // 3. Android 16 резерв — resampleTouchState (ПОДТВЕРЖДЁННЫЙ символ из readelf)
    if (!g_inputHookInstalled) {
        sym_input = DobbySymbolResolver(LIB_INPUT_PATH,
            "_ZN7android13InputConsumer18resampleTouchStateElPNS_11MotionEventEPKNS_12InputMessageE");
        if (sym_input) {
            LOGI("Input hook: resampleTouchState (Android 16 fallback)");
            DobbyHook(sym_input, (void*)my_resample, (void**)&orig_resample);
            g_inputHookInstalled = true;
        }
    }

    if (!g_inputHookInstalled) {
        LOGI("WARNING: No input hook installed!");
    }

    if (!isLoaderDone) {

        //===============🟢🟢🟢🟢🟢🟢🟢=============

        auto PlayerUnlimited_jump = new LoadClass("SYBO.RunnerCore.Character", OBFUSCATE("CharacterMotorAbilities"));
        DWORD GetUnlimited_jump = PlayerUnlimited_jump->GetMethodOffsetByName(OBFUSCATE("get_JumpLimit"), 0);
        DobbyHook((void *)GetUnlimited_jump, (void *)Unlimited_jump, (void **) &old_Unlimited_jump);

        auto PlayerFree_Shop = new LoadClass("SYBO.Subway.Core.CommonData", OBFUSCATE("Currency"));
        DWORD GetFree_Shop = PlayerFree_Shop->GetMethodOffsetByName(OBFUSCATE("get_IsIAP"), 0);
        DobbyHook((void *)GetFree_Shop, (void *)Free_Shop, (void **) &old_Free_Shop);

        auto PlayerUnlimited_Currency = new LoadClass("SYBO.Subway.Core.ProfileData", OBFUSCATE("WalletModel"));
        DWORD GetUnlimited_Currency = PlayerUnlimited_Currency->GetMethodOffsetByName(OBFUSCATE("GetCurrency"), 0);
        DobbyHook((void *)GetUnlimited_Currency, (void *)Unlimited_Currency, (void **) &old_Unlimited_Currency);

        auto PlayerMaxPowerUp = new LoadClass("SYBO.Subway.Core.ProfileData", OBFUSCATE("UpgradesModel"));
        DWORD GetMaxPowerUp = PlayerMaxPowerUp->GetMethodOffsetByName(OBFUSCATE("GetPowerupLevel"), 0);
        DobbyHook((void *)GetMaxPowerUp, (void *)MaxPowerUp, (void **) &old_MaxPowerUp);

        auto PlayerNoCrash = new LoadClass("SYBO.Subway", OBFUSCATE("StumbleBehaviour"));
        DWORD GetNoCrash = PlayerNoCrash->GetMethodOffsetByName(OBFUSCATE("IsAutoReviveEnabled"), 0);
        DobbyHook((void *)GetNoCrash, (void *)NoCrash, (void **) &old_NoCrash);

        //===============🟢🟢🟢🟢🟢🟢🟢=============
        isLoaderDone = true;
    }

    LOGI(OBFUSCATE("%s has been loaded"), (const char *) targetLibName);

#if defined(__aarch64__)
   // PATCH("0x20D3A8", "00 00 A0 E3 1E FF 2F E1");
#else
    LOGI(OBFUSCATE("Done"));
#endif

    pthread_exit(nullptr);
    return nullptr;
}

void hexpatcher() {
    //PATCH_SWITCH("0x1234", "0A 00 A0 E3 1E FF 2F E1", isMaxLevel)
}

void *imgui_go(void *) {
    findLibrary("libil2cpp.so");

    // libEGL.so — правильное имя для всех архитектур (не lib64EGL.so!)
    void *eglLib = dlopen("libEGL.so", RTLD_NOW);
    if (!eglLib) eglLib = dlopen("/system/lib64/libEGL.so", RTLD_NOW);
    if (!eglLib) eglLib = dlopen("/system/lib/libEGL.so", RTLD_NOW);

    void *addr = eglLib ? dlsym(eglLib, "eglSwapBuffers") : nullptr;
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
    // Хуки делаются в hack_thread после загрузки всех библиотек
    return JNI_VERSION_1_6;
}
