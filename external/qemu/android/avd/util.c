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
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "android/utils/debug.h"
#include "android/utils/bufprint.h"
#include "android/utils/ini.h"
#include "android/utils/panic.h"
#include "android/utils/path.h"
#include "android/utils/system.h"
#include "android/avd/util.h"
#include "android/avd/keys.h"

#define D(...) VERBOSE_PRINT(init,__VA_ARGS__)

#define  ANDROID_AVD_DIR    "avd"


char*
path_getSdkRoot( char *pFromEnv )
{
    const char*  env;
    char*        sdkPath;
    char         temp[PATH_MAX], *p=temp, *end=p+sizeof(temp);

#define  SDK_ROOT_ENV  "ANDROID_SDK_ROOT"

    env = getenv(SDK_ROOT_ENV);
    if (env != NULL && env[0] != 0) {
        if (path_exists(env)) {
            D("found " SDK_ROOT_ENV ": %s", env);
            *pFromEnv = 1;
            return ASTRDUP(env);
        }
        D(SDK_ROOT_ENV " points to unknown directory: %s", env);
    }

    *pFromEnv = 0;

    (void) bufprint_app_dir(temp, end);
    sdkPath = path_parent(temp, 1);
    if (sdkPath == NULL) {
        derror("can't find root of SDK directory");
        return NULL;
    }
    D("found SDK root at %s", sdkPath);
    return sdkPath;
}


char*
path_getRootIniPath( const char*  avdName )
{
    char temp[PATH_MAX], *p=temp, *end=p+sizeof(temp);

    p = bufprint_config_path(temp, end);
    p = bufprint(p, end, PATH_SEP ANDROID_AVD_DIR PATH_SEP "%s.ini", avdName);
    if (p >= end) {
        return NULL;
    }
    if (!path_exists(temp)) {
        return NULL;
    }
    return ASTRDUP(temp);
}


char*
path_getSdkHome(void)
{
    char temp[PATH_MAX], *p=temp, *end=p+sizeof(temp);
    p = bufprint_config_path(temp, end);
    if (p >= end) {
        APANIC("User path too long!: %s\n", temp);
    }
    return strdup(temp);
}


static char*
_getAvdContentPath(const char* avdName)
{
    char temp[PATH_MAX], *p=temp, *end=p+sizeof(temp);
    IniFile* ini = NULL;
    char*    iniPath = path_getRootIniPath(avdName);
    char*    avdPath = NULL;

    if (iniPath != NULL) {
        ini = iniFile_newFromFile(iniPath);
        AFREE(iniPath);
    }

    if (ini == NULL) {
        APANIC("Could not open: %s\n", iniPath == NULL ? avdName : iniPath);
    }

    avdPath = iniFile_getString(ini, ROOT_ABS_PATH_KEY, NULL);

    if (!path_is_dir(avdPath)) {
        
        
        const char* relPath = iniFile_getString(ini, ROOT_REL_PATH_KEY, NULL);
        if (relPath != NULL) {
            p = bufprint_config_path(temp, end);
            p = bufprint(p, end, PATH_SEP "%s", relPath);
            if (p < end && path_is_dir(temp)) {
                AFREE(avdPath);
                avdPath = ASTRDUP(temp);
            }
        }
    }

    iniFile_free(ini);

    return avdPath;
}


static char*
_getAvdTargetArch(const char* avdPath)
{
    IniFile* ini;
    char*    targetArch = NULL;
    char     temp[PATH_MAX], *p=temp, *end=p+sizeof(temp);
    p = bufprint(temp, end, "%s" PATH_SEP "config.ini", avdPath);
    if (p >= end) {
        APANIC("AVD path too long: %s\n", avdPath);
    }
    ini = iniFile_newFromFile(temp);
    if (ini == NULL) {
        APANIC("Could not open AVD config file: %s\n", temp);
    }
    targetArch = iniFile_getString(ini, "hw.cpu.arch", "arm");
    iniFile_free(ini);

    return targetArch;
}

char*
path_getAvdTargetArch( const char* avdName )
{
    char*  avdPath = _getAvdContentPath(avdName);
    char*  avdArch = _getAvdTargetArch(avdPath);

    return avdArch;
}

static char*
_getSystemProperty( const char* propFile, const char* propName )
{
    FILE*  file;
    char   temp[PATH_MAX], *p=temp, *end=p+sizeof(temp);
    int    propNameLen = strlen(propName);
    char*  result = NULL;

    file = fopen(propFile, "rb");
    if (file == NULL) {
        D("Could not open file: %s: %s", propFile, strerror(errno));
        return NULL;
    }

    while (fgets(temp, sizeof temp, file) != NULL) {
        
        p = memchr(temp, '\0', sizeof temp);
        if (p == NULL)
            p = end;
        if (p > temp && p[-1] == '\n') {
            *--p = '\0';
        }
        if (p > temp && p[-1] == '\r') {
            *--p = '\0';
        }
        
        if (p == end)
            *--p = '\0';

        
        if (memcmp(temp, propName, propNameLen) != 0) {
            continue;
        }
        p = temp + propNameLen;

        
        if (p >= end || *p != '=')
            continue;
        p++;

        
        if (p >= end || !*p)
            break;

        result = ASTRDUP(p);
        break;
    }
    fclose(file);
    return result;
}

static char*
_getBuildProperty( const char* androidOut, const char* propName )
{
    char temp[PATH_MAX], *p=temp, *end=p+sizeof(temp);

    p = bufprint(temp, end, "%s/system/build.prop", androidOut);
    if (p >= end) {
        D("%s: ANDROID_PRODUCT_OUT too long: %s", __FUNCTION__, androidOut);
        return NULL;
    }
    return _getSystemProperty(temp, propName);
}

char*
path_getBuildTargetArch( const char* androidOut )
{
    const char* defaultArch = "arm";
    char*       result = NULL;
    char*       cpuAbi = _getBuildProperty(androidOut, "ro.product.cpu.abi");

    if (cpuAbi == NULL) {
        D("Coult not find CPU ABI in build properties!");
        D("Default target architecture=%s", defaultArch);
        result = ASTRDUP(defaultArch);
    } else {
        
        if (!strcmp("armeabi",cpuAbi))
            result = "arm";
        else if (!strcmp("armeabi-v7a", cpuAbi))
            result = "arm";
        else if (!strncmp("mips", cpuAbi, 4))
            result = "mips";
        else
            result = cpuAbi;

        D("Found target ABI=%s, architecture=%s", cpuAbi, result);
        result = ASTRDUP(result);
        AFREE(cpuAbi);
    }
    return result;
}

char*
path_getBuildTargetAbi( const char* androidOut )
{
    const char* defaultAbi = "armeabi";
    char*       result = NULL;
    char*       cpuAbi = _getBuildProperty(androidOut, "ro.product.cpu.abi");

    if (cpuAbi == NULL) {
        D("Coult not find CPU ABI in build properties!");
        D("Default target ABI: %s", defaultAbi);
        result = ASTRDUP(defaultAbi);
    } else {
        D("Found target ABI=%s", cpuAbi);
        result = cpuAbi;
    }
    return result;
}


int
path_getBuildTargetApiLevel( const char* androidOut )
{
    const int  defaultLevel = 1000;
    int        level        = defaultLevel;
    char*      sdkVersion = _getBuildProperty(androidOut, "ro.build.version.sdk");

    if (sdkVersion != NULL) {
        long  value;
        char* end;
        value = strtol(sdkVersion, &end, 10);
        if (end == NULL || *end != '\0' || value != (int)value) {
            D("Invalid SDK version build property: '%s'", sdkVersion);
            D("Defaulting to target API level %d", level);
        } else {
            level = (int)value;
            if (level < 3)
                level = 3;
            D("Found target API level: %d", level);
        }
        AFREE(sdkVersion);
    } else {
        D("Could not find target API level / SDK version in build properties!");
        D("Default target API level: %d", level);
    }
    return level;
}

int
path_getAdbdCommunicationMode( const char* androidOut )
{
    char* prop = _getBuildProperty(androidOut, "ro.adb.qemud");
    if (prop != NULL) {
        long val = 0;
        char* end;
        val = strtol(prop, &end, 10);
        if (end == NULL || *end != '\0' || val != (int)val) {
            D("Invalid ro.adb.qemud build property: '%s'", prop);
            val = 0;
        } else {
            D("Found ro.adb.qemud build property: %d", val);
        }
        AFREE(prop);
        return (int)val;
    } else {
        
        return 0;
    }
}
