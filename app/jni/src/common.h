#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <android/log.h>

#define NATIVE_W   1440
#define NATIVE_H   2560
#define PLAYBG_W   1640
#define PLAYBG_H   2560
#define BUBBLES_W  144
#define BUBBLES_H  432
#define BUBBLES1_X 144
#define BUBBLES1_Y 1296
#define SHARK_W    852
#define SHARK_H    292
#define MAX_FISH   20

#define  LOG_TAG    "carp"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)