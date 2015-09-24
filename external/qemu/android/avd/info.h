/* Copyright (C) 2008 The Android Open Source Project
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
#ifndef ANDROID_AVD_INFO_H
#define ANDROID_AVD_INFO_H

#include "android/utils/ini.h"
#include "android/avd/hw-config.h"
#include "android/config/config.h"



#define  AVD_IMAGE_LIST \
    _AVD_IMG(KERNEL,"kernel-qemu","kernel") \
    _AVD_IMG(RAMDISK,"ramdisk.img","ramdisk") \
    _AVD_IMG(INITSYSTEM,"system.img","init system") \
    _AVD_IMG(INITDATA,"userdata.img","init data") \
    _AVD_IMG(USERSYSTEM,"system-qemu.img","user system") \
    _AVD_IMG(USERDATA,"userdata-qemu.img", "user data") \
    _AVD_IMG(CACHE,"cache.img","cache") \
    _AVD_IMG(SDCARD,"sdcard.img","SD Card") \
    _AVD_IMG(SNAPSHOTS,"snapshots.img","snapshots") \

#define _AVD_IMG(x,y,z)   AVD_IMAGE_##x ,
typedef enum {
    AVD_IMAGE_LIST
    AVD_IMAGE_MAX 
} AvdImageType;
#undef  _AVD_IMG

typedef struct AvdInfo  AvdInfo;

typedef enum {
    
    AVDINFO_WIPE_DATA = (1 << 0),
    
    AVDINFO_NO_CACHE  = (1 << 1),
    
    AVDINFO_WIPE_CACHE = (1 << 2),
    
    AVDINFO_NO_SDCARD = (1 << 3),
    
    AVDINFO_WIPE_SYSTEM = (1 << 4),
    
    AVDINFO_NO_SNAPSHOTS = (1 << 5),
} AvdFlags;

typedef struct {
    unsigned     flags;
    const char*  skinName;
    const char*  skinRootPath;
    const char*  forcePaths[AVD_IMAGE_MAX];
} AvdInfoParams;

AvdInfo*  avdInfo_new( const char*  name, AvdInfoParams*  params );

int avdInfo_getSkinHardwareIni( AvdInfo* i, char* skinName, char* skinDirPath);

AvdInfo*  avdInfo_newForAndroidBuild( const char*     androidBuildRoot,
                                      const char*     androidOut,
                                      AvdInfoParams*  params );

void        avdInfo_free( AvdInfo*  i );

const char*  avdInfo_getName( AvdInfo*  i );

int    avdInfo_getApiLevel( AvdInfo*  i );

char*  avdInfo_getKernelPath( AvdInfo*  i );
char*  avdInfo_getRamdiskPath( AvdInfo*  i );
char*  avdInfo_getSdCardPath( AvdInfo* i );
char*  avdInfo_getSnapStoragePath( AvdInfo* i );

char*  avdInfo_getCachePath( AvdInfo*  i );
char*  avdInfo_getDefaultCachePath( AvdInfo*  i );


char*  avdInfo_getSystemImagePath( AvdInfo* i );

char*  avdInfo_getSystemInitImagePath( AvdInfo*  i );

char*  avdInfo_getDataImagePath( AvdInfo*  i );
char*  avdInfo_getDefaultDataImagePath( AvdInfo*  i );
char*  avdInfo_getDataInitImagePath( AvdInfo* i );

const char*  avdInfo_getImagePath( AvdInfo*  i, AvdImageType  imageType );

const char*  avdInfo_getImageDefaultPath( AvdInfo*  i, AvdImageType  imageType );


const char*  avdInfo_getImageFile( AvdInfo*  i, AvdImageType  imageType );

uint64_t     avdInfo_getImageFileSize( AvdInfo*  i, AvdImageType  imageType );

int          avdInfo_isImageReadOnly( AvdInfo*  i, AvdImageType  imageType );

int          avdInfo_lockImageFile( AvdInfo*  i, AvdImageType  imageType, int  abortOnError);

void         avdInfo_setImageFile( AvdInfo*  i, AvdImageType  imageType, const char*  imagePath );

const char*  avdInfo_getContentPath( AvdInfo*  i );

void         avdInfo_getSkinInfo( AvdInfo*  i, char** pSkinName, char** pSkinDir );

int          avdInfo_shouldUseDynamicSkin( AvdInfo* i);

char*        avdInfo_getCharmapFile( AvdInfo* i, const char* charmapName );

int          avdInfo_inAndroidBuild( AvdInfo*  i );

char*        avdInfo_getTargetAbi( AvdInfo*  i );

int          avdInfo_initHwConfig( AvdInfo*  i, AndroidHwConfig*  hw );

char*        avdInfo_getTracePath( AvdInfo*  i, const char*  traceName );

const char*  avdInfo_getCoreHwIniPath( AvdInfo* i );

int          avdInfo_getAdbdCommunicationMode( AvdInfo* i );

int          avdInfo_getSnapshotPresent(AvdInfo* i);


#endif 
