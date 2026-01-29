/*
By @Starfustxdd
24/09/2024
*/

ImFont* titleFont;
ImFont* tabFont;
ImFont* uiFont;

static float menuTransparency = 0.95f;  
static ImVec4 backgroundColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);  
static float rounding = 5.0f; 
static int themeSelection = 0; 
static ImVec4 textColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);  

// Light theme colors
static ImVec4 borderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);   
static float borderSize = 1.0f; 
static ImVec4 buttonColor = ImVec4(0.25f, 0.68f, 0.85f, 1.0f); 
static ImVec4 sliderColor = ImVec4(0.45f, 0.78f, 0.92f, 1.0f);   
static ImVec4 sliderBgColor = ImVec4(0.95f, 0.95f, 0.95f, 1.0f); 
static ImVec4 checkboxColor = ImVec4(0.25f, 0.68f, 0.85f, 1.0f);


void ResetColorsToDefaults() {
    if (themeSelection == 0) {
        backgroundColor = ImVec4(0.1f, 0.1f, 0.1f, 1.0f); 
        textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);    
        borderColor = ImVec4(0.3f, 0.3f, 0.3f, 1.0f);   
        buttonColor = ImVec4(0.4f, 0.7f, 0.3f, 1.0f);   
        sliderColor = ImVec4(0.5f, 0.8f, 0.5f, 1.0f); 
        sliderBgColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f); 
        checkboxColor = ImVec4(0.4f, 0.7f, 0.3f, 1.0f); 
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.15f, 1.0f)); 

    } else if (themeSelection == 1) {
        backgroundColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // White background
        textColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);     // Black text
        borderColor = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);   
        buttonColor = ImVec4(0.25f, 0.68f, 0.85f, 1.0f);   // Vibrant cyan button
        sliderColor = ImVec4(0.45f, 0.78f, 0.92f, 1.0f);   // Vibrant light blue slider grab
        sliderBgColor = ImVec4(0.95f, 0.95f, 0.95f, 1.0f); // Light gray for slider background
        checkboxColor = ImVec4(0.25f, 0.68f, 0.85f, 1.0f); // Vibrant cyan checkbox mark
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.9f, 0.9f, 0.9f, 1.0f)); // Very light gray frame background

    } else if (themeSelection == 2) {
        backgroundColor = ImVec4(0.5f, 0.5f, 0.5f, 1.0f); // Medium gray background
        textColor = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);     // White text
        borderColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);   // Gray border
        buttonColor = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);   // Vibrant yellow button
        sliderColor = ImVec4(0.8f, 0.8f, 0.2f, 1.0f);   // Vibrant yellow slider grab
        sliderBgColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f); // Gray background for slider
        checkboxColor = ImVec4(0.8f, 0.8f, 0.2f, 1.0f); // Vibrant yellow checkbox mark
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // Medium gray frame background
    }
}

void ApplyTheme(int themeSelection) {
    
	if (themeSelection == 0) {
        ImGui::StyleColorsDark();
    } else if (themeSelection == 1) {
        ImGui::StyleColorsLight();
    } else if (themeSelection == 2) {
        ImGui::StyleColorsClassic();
    }
	
	ResetColorsToDefaults();
    
}
void SetupImgui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)glWidth, (float)glHeight);
	
    ImGui_ImplOpenGL3_Init("#version 300 es");

    // Title Font Configuration
    ImFontConfig titleFontCfg;
    titleFontCfg.SizePixels = 40.0f; 
    titleFontCfg.OversampleH = 3; 
    titleFontCfg.OversampleV = 3;
    titleFontCfg.PixelSnapH = true;
    titleFont = io.Fonts->AddFontFromMemoryCompressedTTF(GoogleSans_compressed_data, GoogleSans_compressed_size, titleFontCfg.SizePixels, &titleFontCfg);

    // Tab Font Configuration
    ImFontConfig tabFontCfg;
    tabFontCfg.SizePixels = 33.0f; 
    tabFontCfg.OversampleH = 3;
    tabFontCfg.OversampleV = 3;
    tabFontCfg.PixelSnapH = true;
    tabFont = io.Fonts->AddFontFromMemoryCompressedTTF(GoogleSans_compressed_data, GoogleSans_compressed_size, tabFontCfg.SizePixels, &tabFontCfg);

    // UI Font Configuration
    ImFontConfig uiFontCfg;
    uiFontCfg.SizePixels = 27.0f; 
    uiFontCfg.OversampleH = 3;
    uiFontCfg.OversampleV = 3;
    uiFontCfg.PixelSnapH = true;
    uiFont = io.Fonts->AddFontFromMemoryCompressedTTF(GoogleSans_compressed_data, GoogleSans_compressed_size, uiFontCfg.SizePixels, &uiFontCfg);
	
    ImGuiStyle& style = ImGui::GetStyle();
	style.GrabMinSize = 30.0f;
    style.ScrollbarSize = 40.0f; 
    // Apply default theme
    ApplyTheme(themeSelection);
}

// Method Offset Hooking Examples:
/*
bool IsBoolExample2 = false;
bool IsIntExample2 = false;
bool IsFloatExample2 = false;
bool IsDoubleExample2 = false;


// Bool Method Offset Hooking Example:
bool (*old_BoolExample2)(void *instance);
bool BoolExample2(void *instance) {
	if (instance != NULL) {
		if (IsBoolExample2) {
			return true; // Or you can put return false;.
		}
	}
	return old_BoolExample2(instance);
}

// Int Method Offset Hooking Example:
int (*old_IntExample2)(void *instance);
int IntExample2(void *instance) {
	if (instance != NULL) {
		if (IsIntExample2) {
			return 999999999; // Int Hook Value = 999,999,999
		}
	}
	return old_IntExample2(instance);
}

// Float Method Offset Hooking Example:
float (*old_FloatExample2)(void *instance);
float FloatExample2(void *instance) {
	if (instance != NULL) {
		if (IsFloatExample2) {
			return 999999999.0f; // Float Hook Value = 999,999,999
		}
	}
	return old_FloatExample2(instance);
}

// Double Method Offset Hooking Example:
double (*old_DoubleExample2)(void *instance);
double DoubleExample2(void *instance) {
	if (instance != NULL) {
		if (IsDoubleExample2) {
			return 999999999.0; // Double Hook Value = 999,999,999
		}
	}
	return old_DoubleExample2(instance);
}
*/

// =============[My Hooks]=================

// Hooks sane as lgl 3.2

bool Unlimited_jump = false;  // unlimited Jump Hack

// Int Method Offset Hooking Example:
int (*old_jump)(void *instance);
int jump(void *instance) {
	if (instance != NULL) {
		if (Unlimited_jump) {
			return 999999999; // Int Hook Value = 999,999,999
		}
	}
	return old_jump(instance);
}

bool Free_Shop = false;  // Free_Shop

// Bool Method Offset Hooking Example:
bool (*old_Free_Shop)(void *instance);
bool Hook_Free_Shop(void *instance) {
    if (instance != NULL && Free_Shop) {
          return false; // Or you can put return true;
	}
	return old_Free_Shop(instance);
}

// =============[🔴🔴🔴🔴🔴🔴]================

void DrawMenu() {
    // For Portrait Mode Game
    static ImVec2 window_pos = ImVec2(50, 200);
    static ImVec2 window_size = ImVec2(400, 400);
    
    // For Landscape Mode Game
    /*
    static ImVec2 window_pos = ImVec2(650, 20);
    static ImVec2 window_size = ImVec2(600, 400);
	*/

    ImGuiIO& io = ImGui::GetIO();
	
    // Set style for both window and element rounding using one rounding value
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w * menuTransparency));
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);  // Set custom text color
    ImGui::PushStyleColor(ImGuiCol_Border, borderColor);  // Set custom border color
    ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);  // Set custom button color
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, sliderColor);  // Set custom slider color
    ImGui::PushStyleColor(ImGuiCol_CheckMark, checkboxColor);  // Set custom checkbox color
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, rounding);  // Window corner rounding
	ImGui::PushStyleVar(ImGuiStyleVar_GrabRounding, rounding); // Apply rounding to the slider grab
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, rounding);   // Button/frame corner rounding
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, rounding);   // Child window corner rounding
    ImGui::PushStyleVar(ImGuiStyleVar_PopupRounding, rounding);   // Popup window corner rounding
    ImGui::PushStyleVar(ImGuiStyleVar_ScrollbarRounding, rounding); // Scrollbar corner rounding
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, borderSize);  // Window border size

    ImGui::SetNextWindowPos(window_pos, ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(window_size, ImGuiCond_FirstUseEver);
	
    if (ImGui::Begin(OBFUSCATE("IMGUI"), nullptr, ImGuiWindowFlags_NoSavedSettings)) {
        if (ImGui::BeginTabBar("MainTabBar", ImGuiTabBarFlags_FittingPolicyScroll)) {

    if (ImGui::BeginTabItem("Player Hacks")) {
		
       ImGui::Checkbox("Unlimited Jump", &Unlimited_jump);
       ImGui::Checkbox("Free Shop", &Free_Shop);
		
		ImGui::EndTabItem();
    }

    if (ImGui::BeginTabItem("THEME")) {
        ImGui::PushFont(uiFont);
		
		const char* themes[] = { "Dark", "Light", "Classic" };
        if (ImGui::Combo("Theme", &themeSelection, themes, IM_ARRAYSIZE(themes))) {
            ApplyTheme(themeSelection);
        }    

        ImGui::PopFont();
        ImGui::EndTabItem();
    }
			
    ImGui::EndTabBar();
}
		
	hexpatcher();
    ImGui::End();
		
	}
}

void RenderImGui() {
    if (!setup) {
        SetupImgui();
        setup = true;
    }
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2(glWidth, glHeight); 
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(glWidth, glHeight);
    ImGui::NewFrame();
    DrawMenu();
    ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::EndFrame();
}

EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    eglQuerySurface(dpy, surface, EGL_WIDTH, &glWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &glHeight);
    RenderImGui();
    return old_eglSwapBuffers(dpy, surface);
}
