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


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <android/utils/panic.h>
#include <android/utils/path.h>
#include <android/utils/bufprint.h>
#include <android/avd/util.h>

int android_verbose;


#define DEBUG 1

#if DEBUG
#  define D(...)  do { if (android_verbose) printf("emulator:" __VA_ARGS__); } while (0)
#else
#  define D(...)  do{}while(0)
#endif

#ifdef _WIN32
#  define DLL_EXTENSION  ".dll"
#elif defined(__APPLE__)
#  define DLL_EXTENSION  ".dylib"
#else
#  define DLL_EXTENSION  ".so"
#endif

#if defined(__x86_64__)
#define  GLES_EMULATION_LIB  "lib64OpenglRender" DLL_EXTENSION
#elif defined(__i386__)
#define  GLES_EMULATION_LIB  "libOpenglRender" DLL_EXTENSION
#else
#error Unknown architecture for codegen
#endif


static char* getTargetEmulatorPath(const char* progName, const char* avdArch, const int force_32bit);
static char* getSharedLibraryPath(const char* progName, const char* libName);
static void  prependSharedLibraryPath(const char* prefix);

#ifdef _WIN32
static char* quotePath(const char* path);
#endif

#ifdef _WIN32
#  define safe_execv(_filepath,_argv)  execv((_filepath),(const char* const*)(_argv))
#else
#  define safe_execv(_filepath,_argv)  execv((_filepath),(_argv))
#endif

int main(int argc, char** argv)
{
    const char* avdName = NULL;
    char*       avdArch = NULL;
    char*       emulatorPath;
    int         force_32bit = 0;

    const char* debug = getenv("ANDROID_EMULATOR_DEBUG");

    if (debug != NULL && *debug && *debug != '0')
        android_verbose = 1;

    int  nn;
    for (nn = 1; nn < argc; nn++) {
        const char* opt = argv[nn];

        if (!strcmp(opt,"-qemu"))
            break;

        if (!strcmp(opt,"-force-32bit")) {
            force_32bit = 1;
            continue;
        }

        if (!avdName) {
            if (!strcmp(opt,"-avd") && nn+1 < argc) {
                avdName = argv[nn+1];
            }
            else if (opt[0] == '@' && opt[1] != '\0') {
                avdName = opt+1;
            }
        }
    }

    if (avdName != NULL) {
        D("Found AVD name '%s'\n", avdName);
        avdArch = path_getAvdTargetArch(avdName);
        D("Found AVD target architecture: %s\n", avdArch);
    } else {
        
        const char* androidOut = getenv("ANDROID_PRODUCT_OUT");

        if (androidOut != NULL && *androidOut != '\0') {
            D("Found ANDROID_PRODUCT_OUT: %s\n", androidOut);
            avdArch = path_getBuildTargetArch(androidOut);
            D("Found build target architecture: %s\n", avdArch);
        }
    }

    if (avdArch == NULL) {
        avdArch = "arm";
        D("Can't determine target AVD architecture: defaulting to %s\n", avdArch);
    }

    
    emulatorPath = getTargetEmulatorPath(argv[0], avdArch, force_32bit);
    D("Found target-specific emulator binary: %s\n", emulatorPath);

    
    argv[0] = emulatorPath;

#ifdef _WIN32
    if (strchr(emulatorPath, ' ')) {
        argv[0] = quotePath(emulatorPath);
        D("Quoted emulator binary path: %s\n", emulatorPath);
    }
#endif

    {
        char*  sharedLibPath = getSharedLibraryPath(emulatorPath, GLES_EMULATION_LIB);

        if (sharedLibPath != NULL) {
            D("Found OpenGLES emulation libraries in %s\n", sharedLibPath);
            prependSharedLibraryPath(sharedLibPath);
        } else {
            D("Could not find OpenGLES emulation host libraries!\n");
        }
    }

    
    safe_execv(emulatorPath, argv);

    
    fprintf(stderr, "Could not launch '%s': %s\n", emulatorPath, strerror(errno));
    return errno;
}

static int
getHostOSBitness()
{
    return system("file -L \"$SHELL\" | grep -q \"x86[_-]64\"") == 0 ? 64 : 32;
}

static char*
getTargetEmulatorPath(const char* progName, const char* avdArch, const int force_32bit)
{
    char*  progDir;
    char   path[PATH_MAX], *pathEnd=path+sizeof(path), *p;
    const char* emulatorPrefix = "emulator-";
    const char* emulator64Prefix = "emulator64-";
#ifdef _WIN32
    const char* exeExt = ".exe";
    int search_for_64bit_emulator = 0;
#else
    const char* exeExt = "";
    int search_for_64bit_emulator = !force_32bit && getHostOSBitness() == 64;
#endif

    
    path_split(progName, &progDir, NULL);

    if (search_for_64bit_emulator) {
        
        p = bufprint(path, pathEnd, "%s/%s%s%s", progDir, emulator64Prefix, avdArch, exeExt);
        if (p >= pathEnd) {
            APANIC("Path too long: %s\n", progName);
        }
        if (path_exists(path)) {
            free(progDir);
            return strdup(path);
        }
    }

    
    p = bufprint(path, pathEnd, "%s/%s%s%s", progDir, emulatorPrefix, avdArch, exeExt);
    free(progDir);
    if (p >= pathEnd) {
        APANIC("Path too long: %s\n", progName);
    }

    if (path_exists(path)) {
        return strdup(path);
    }

#ifdef _WIN32
    if (strchr(progName, '/') == NULL && strchr(progName, '\\') == NULL) {
#else
    if (strchr(progName, '/') == NULL) {
#endif
        if (search_for_64bit_emulator) {
           p = bufprint(path, pathEnd, "%s%s%s", emulator64Prefix, avdArch, exeExt);
           if (p < pathEnd) {
               char*  resolved = path_search_exec(path);
               if (resolved != NULL)
                   return resolved;
           }
        }

        p = bufprint(path, pathEnd, "%s%s%s", emulatorPrefix, avdArch, exeExt);
        if (p < pathEnd) {
            char*  resolved = path_search_exec(path);
            if (resolved != NULL)
                return resolved;
        }
    }

    
    APANIC("Missing arch-specific emulator program: %s\n", path);
    return NULL;
}

static int
probePathForFile(const char* path, const char* filename)
{
    char  temp[PATH_MAX], *p=temp, *end=p+sizeof(temp);
    p = bufprint(temp, end, "%s/%s", path, filename);
    D("Probing for: %s\n", temp);
    return (p < end && path_exists(temp));
}


static char*
getSharedLibraryPath(const char* progName, const char* libName)
{
    char* progDir;
    char* result = NULL;
    char  temp[PATH_MAX], *p=temp, *end=p+sizeof(temp);

    
    path_split(progName, &progDir, NULL);

    if (probePathForFile(progDir, libName)) {
        return progDir;
    }

    {
        p = bufprint(temp, end, "%s/lib", progDir);
        if (p < end && probePathForFile(temp, libName)) {
            result = strdup(temp);
            goto EXIT;
        }
    }

    {
        char* parentDir = path_parent(progDir, 1);

        if (parentDir == NULL) {
            parentDir = strdup(".");
        }
        p = bufprint(temp, end, "%s/lib", parentDir);
        free(parentDir);
        if (p < end && probePathForFile(temp, libName)) {
            result = strdup(temp);
            goto EXIT;
        }
    }

    
EXIT:
    free(progDir);
    return result;
}

static void
prependSharedLibraryPath(const char* prefix)
{
    char temp[2048], *p=temp, *end=p+sizeof(temp);
#ifdef _WIN32
    const char* path = getenv("PATH");
    if (path == NULL || path[0] == '\0') {
        p = bufprint(temp, end, "PATH=%s", prefix);
    } else {
        p = bufprint(temp, end, "PATH=%s;%s", path, prefix);
    }
    D("Setting %s\n", temp);
    putenv(strdup(temp));
#else
    const char* path = getenv("LD_LIBRARY_PATH");
    if (path != NULL && path[0] != '\0') {
        p = bufprint(temp, end, "%s:%s", prefix, path);
        prefix = temp;
    }
    setenv("LD_LIBRARY_PATH",prefix,1);
    D("Setting LD_LIBRARY_PATH=%s\n", prefix);
#endif
}

#ifdef _WIN32
static char*
quotePath(const char* path)
{
    int   len = strlen(path);
    char* ret = malloc(len+3);

    ret[0] = '"';
    memcpy(ret+1, path, len);
    ret[len+1] = '"';
    ret[len+2] = '\0';

    return ret;
}
#endif 
