int glHeight, glWidth;
bool setup;
bool isLoaderDone = false;

struct Offset {
    DWORD Coin, Gem, Score, TrUp, SetPlayerVelocity, IsInvincible, StartBolt, EndBolt,
          StartShield, EndShield, RpUp, IsCharacterPurchased, IsPowerPurchased,
          CanAffordRegion, IsAdRemovalPurchased, OnTriggerEnter, StartZipLine,
          CalcPickupDistance, GetDoubleCoinStartDistance, GetTripleCoinStartDistance,
          GetCurrentRank, GetMaxRank, IsAdRemovalPurchasedX;
} OFFSET;


static bool isWin = false;
static bool isStatueFall = false;
static bool isLose = false;
static bool isPopulation = false;
static bool isGold = false;
static bool isInstantBuild = false;
static bool isFreeUnit = false;

static bool isGemHack = false;
static bool isCoinHack = false;
static bool isScoreHack = false;
static bool isCrazyPlayer = false;
static bool isDoubleCoin = false;
static bool isGodMode = false;
static bool isTripleCoin = false;
static bool isFarPickup = false;
static bool isGodPlayer = false;
static bool isShield = false;
static bool isBolt = false;

static int AddValue = 100;
static int AddScore = 1000;

static int sliderValue = 0;
static float PlayerSpeed = 1.0f;

static bool isNoAd = false;
static bool isUnlockRegion = false;
static bool isUnlockPower = false;
static bool isUnlockCharacter = false;
static bool isMaxLevel = false;

bool isHexPatch = false;
bool featureHookToggle = false;
bool isFieldHook = false;

int level = 10;

// Dummy variables
static bool isESPTab2 = false;
static bool isESPLineTab2 = false;
static bool isESPBoxTab2 = false;
static bool isESPDistanceTab2 = false;
static bool isESPObjectsCountTab2 = false;


static float floatSliderValue = 0.0f;
static char inputText[128] = "";
static float color[4] = { 0.4f, 0.7f, 0.0f, 1.0f };
static const char* items[] = { "Item 1", "Item 2", "Item 3", "Item 4", "Item 5" };
static int item_current = 0;

// Global Function
void hexpatcher();



// Hex Patches

#if defined(__aarch64__)
    const char *libEGL = "lib64EGL.so";
    #define LIB_INPUT_PATH "/system/lib64/libinput.so"
    const char* aarch = "arm64-v8a - 64Bit";
    struct patches {
        // Boolean
        const char* BoolTrue = OBFUSCATE("20 00 80 D2 C0 03 5F D6");  // True
        const char* BoolFalse = OBFUSCATE("00 00 80 D2 C0 03 5F D6"); // False

        // Int
        const char* Int0 = BoolFalse;  // 0
        //const char* Int1 = BoolTrue;  // 1
        const char* Int10 = OBFUSCATE("40 01 80 52 C0 03 5F D6");  // 10
        const char* Int100 = OBFUSCATE("80 0C 80 52 C0 03 5F D6"); // 100
        const char* Int1000 = OBFUSCATE("00 7D 80 52 C0 03 5F D6"); // 1000
        const char* Int10000 = OBFUSCATE("00 E2 84 52 C0 03 5F D6"); // 10,000
        const char* IntInfinite = OBFUSCATE("00 E0 AF D2 C0 03 5F D6"); // Infinite

        // Float
        const char* Float0 = OBFUSCATE("E0 03 27 1E C0 03 5F D6");  // 0
        const char* Float1 = OBFUSCATE("00 10 2E 1E C0 03 5F D6");  // 1
        const char* Float10 = OBFUSCATE("00 90 24 1E C0 03 5F D6"); // 10
        const char* Float100 = OBFUSCATE("00 59 A8 52 00 00 27 1E C0 03 5F D6"); // 100
        const char* Float1000 = OBFUSCATE("40 8F A8 52 00 00 27 1E C0 03 5F D6"); // 1000
        const char* Float2000 = OBFUSCATE("FA 04 04 E3 1E FF 2F E1"); // 2000
		
		//Double ( Taken From Billonz Offset Tester So Credit To Him )
		const char* Double0 = OBFUSCATE("E003679EC0035FD6");
		const char* Double1 = OBFUSCATE("00106E1EC0035FD6");
		const char* Double10 = OBFUSCATE("0090641EC0035FD6");
		const char* Double100 = OBFUSCATE("800C8052C0035FD6");
		const char* Double1000 = OBFUSCATE("000080F20000A0F20000C8F2E011E8F20000679EC0035FD6");
		const char* Double10000 = OBFUSCATE("000080F20000A0F20000D1F20019E8F20000679EC0035FD6");
		const char* DoubleInfinite =  OBFUSCATE("000080F20000A0F20040CDF2001FE8F20000679EC0035FD6");
	
    } HEXPATCH;

#else // 32-bit architecture
    const char *libEGL = "libEGL.so";
    #define LIB_INPUT_PATH "/system/lib/libinput.so"
	const char* aarch = "armeabi-v7a - 32Bit";
    
    struct patches {
        // Boolean
        const char* BoolTrue = OBFUSCATE("01 00 A0 E3 1E FF 2F E1");  // True
        const char* BoolFalse = OBFUSCATE("00 00 A0 E3 1E FF 2F E1"); // False

        // Int
        const char* Int0 = BoolFalse;  // 0
        const char* Int1 = BoolTrue;   // 1
        const char* Int10 = OBFUSCATE("0A 00 A0 E3 1E FF 2F E1");  // 10
        const char* Int100 = OBFUSCATE("64 00 00 E3 1E FF 2F E1"); // 100
        const char* Int1000 = OBFUSCATE("01 0A A0 E3 1E FF 2F E1"); // 1000
        const char* Int10000 = OBFUSCATE("10 07 02 E3 1E FF 2F E1"); // 10,000
        const char* IntInfinite = OBFUSCATE("02 01 E0 E3 1E FF 2F E1"); // Infinite

        // Float
        const char* Float0 = OBFUSCATE("00 00 40 E3 1E FF 2F E1");  // 0
        const char* Float1 = OBFUSCATE("80 0F 43 E3 1E FF 2F E1");  // 1
        const char* Float10 = OBFUSCATE("20 01 04 E3 1E FF 2F E1");  // 10
        const char* Float100 = OBFUSCATE("C8 02 44 E3 1E FF 2F E1"); // 100
        const char* Float1000 = OBFUSCATE("7A 04 44 E3 1E FF 2F E1"); // 1000
        const char* Float2000 = OBFUSCATE("FA 04 04 E3 1E FF 2F E1"); // 2000
		
		//Double ( Taken From Billonz Offset Tester So Credit To Him )
		const char* Double0 = OBFUSCATE("000000E3001040E3100B41EC1EFF2FE1");
		const char* Double1 = OBFUSCATE("0000A0E3001000E3F01F43E3100B41EC1EFF2FE1");
		const char* Double10 = OBFUSCATE("0000A0E3001000E3241044E3100B41EC1EFF2FE1");
		const char* Double100 = OBFUSCATE("0000A0E3001000E3591044E3100B41EC1EFF2FE1");
		const char* Double1000 = OBFUSCATE("0000A0E3001004E38F1044E3100B41EC1EFF2FE1");
		const char* Double10000 = OBFUSCATE("0000A0E3001808E3C31044E3100B41EC1EFF2FE1");
		const char* DoubleInfinite = OBFUSCATE("000000E3C00F4FE3FF1F0FE3DF1144E3100B41EC1EFF2FE1");
    } HEXPATCH;

#endif

/*
 * The separate HEXPATCH structures for aarch64 and 32-bit architectures
 * Dont Need To Seprate One-By-One Everything Will be done automaticly
 * are necessary due to differences in memory layouts and instruction sets.
 * Each architecture may require unique hex patches to function correctly 
 * within its specific environment. Ensure to modify the appropriate structure 
 * based on the target architecture for accurate patching.
 */

