# To AIDE Users: If you are using 32-bit/ARMv7 phone, please remove arm64-v8a
APP_ABI := armeabi-v7a arm64-v8a
APP_PLATFORM := android-18 #APP_PLATFORM does not need to be set. It will automatically defaulting
APP_STL := c++_static
APP_OPTIMA := release
APP_THIN_ARCHIVE := true
APP_PIE := true
APP_SUPPORT_FLEXIBLE_PAGE_SIZES := true

ifneq ($(APP_OPTIM), debug)
  $(info APP_OPTIM is $(APP_OPTIM) ...)
  APP_LDFLAGS += -Wl,--strip-all
  APP_CFLAGS  += -fvisibility=hidden -fvisibility-inlines-hidden
  APP_CFLAGS  += -g0 -O3 -fomit-frame-pointer -ffunction-sections -fdata-sections
endif