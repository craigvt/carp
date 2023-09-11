LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := main

SDL_PATH := ../SDL

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include $(LOCAL_PATH)/../SDL_image $(LOCAL_PATH)/../SDL2_ttf $(LOCAL_PATH)/../SDL2_mixer

# Add your application source files here...
LOCAL_SRC_FILES := main.c system.c render.c input.c game.c background.c entity.c ui.c audio.c

LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_ttf SDL2_mixer

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lOpenSLES -llog -landroid

include $(BUILD_SHARED_LIBRARY)
