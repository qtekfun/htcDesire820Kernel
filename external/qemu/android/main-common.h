/* Copyright (C) 2011 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/
#ifndef ANDROID_MAIN_COMMON_H
#define ANDROID_MAIN_COMMON_H

#include <stdint.h>
#include "android/cmdline-option.h"
#include "android/skin/keyset.h"
#include "android/config.h"
#include "android/avd/hw-config.h"



void user_config_init( void );
void user_config_done( void );

void user_config_get_window_pos( int *window_x, int *window_y );

#define  ONE_MB  (1024*1024)

unsigned convertBytesToMB( uint64_t  size );
uint64_t convertMBToBytes( unsigned  megaBytes );

extern SkinKeyset*  android_keyset;
void parse_keyset(const char*  keyset, AndroidOptions*  opts);
void write_default_keyset( void );

#define NETWORK_SPEED_DEFAULT  "full"
#define NETWORK_DELAY_DEFAULT  "none"

extern const char*  skin_network_speed;
extern const char*  skin_network_delay;

void parse_skin_files(const char*      skinDirPath,
                      const char*      skinName,
                      AndroidOptions*  opts,
                      AndroidHwConfig* hwConfig,
                      AConfig*        *skinConfig,
                      char*           *skinPath);

int64_t  get_screen_pixels(AConfig*  skinConfig);

void init_sdl_ui(AConfig*         skinConfig,
                 const char*      skinPath,
                 AndroidOptions*  opts);

void sanitizeOptions( AndroidOptions* opts );

struct AvdInfo* createAVD(AndroidOptions* opts, int* inAndroidBuild);

void findImagePaths( AndroidHwConfig*  hwConfig,
                     AndroidOptions*   opts );

void updateHwConfigFromAVD(AndroidHwConfig* hwConfig, struct AvdInfo* avd,
                           AndroidOptions* opts, int inAndroidBuild);

void handle_ui_options( AndroidOptions* opts );

int attach_ui_to_core( AndroidOptions* opts );

#endif 
