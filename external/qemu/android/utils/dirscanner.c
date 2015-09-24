/* Copyright (C) 2007-2008 The Android Open Source Project
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
#include "android/utils/dirscanner.h"
#include "android/utils/bufprint.h"
#include "android/utils/system.h"
#include "android/utils/path.h"
#include <stddef.h>

#define  DIRSCANNER_BASE     \
    char  root[PATH_MAX];    \
    int   rootLen;           \
    char  full[PATH_MAX];    \


#if _WIN32

#include <io.h>

struct DirScanner {
    DIRSCANNER_BASE
    intptr_t            findIndex1;
    struct _finddata_t  findData;
};


static int
_dirScannerInit( DirScanner*  s )
{
    char*  p   = s->root + s->rootLen;
    char*  end = s->root + sizeof s->root;
    int    ret;

    
    p = bufprint(p, end, "\\*");
    if (p >= end)
        return -1;

    ret = _findfirst(s->root, &s->findData);

    s->findIndex1 = ret+1;
    return ret;
}

static void
_dirScanner_done( DirScanner*  s )
{
    if (s->findIndex1 > 0) {
        _findclose(s->findIndex1-1);
        s->findIndex1 = 0;
    }
}

const char*
dirScanner_next( DirScanner*  s )
{
    char*  ret = NULL;

    if (!s || s->findIndex1 <= 0)
        return NULL;

    while (ret == NULL) {
        ret = s->findData.name;

        
        if (!strcmp(ret, ".") || !strcmp(ret, "..")) {
            ret = NULL;
        }
        
        if (_findnext(s->findIndex1-1, &s->findData) < 0) {
            _dirScanner_done(s);
            break;
        }
    }
    return ret;
}

#else 

#include <dirent.h>
struct DirScanner {
    DIRSCANNER_BASE
    DIR*            dir;
    struct dirent*  entry;
};

static int
_dirScannerInit( DirScanner*  s )
{
    s->dir = opendir(s->root);

    if (s->dir == NULL)
        return -1;

    s->entry = NULL;
    return 0;
}

static void
_dirScanner_done( DirScanner*  s )
{
    if (s->dir) {
        closedir(s->dir);
        s->dir = NULL;
    }
}

const char*
dirScanner_next( DirScanner*  s )
{
    char*  ret = NULL;

    if (!s || s->dir == NULL)
        return NULL;

    for (;;)
    {
        
        s->entry = readdir(s->dir);
        if (s->entry == NULL) {
            _dirScanner_done(s);
            break;
        }

        
        ret = s->entry->d_name;

        if (!strcmp(ret,".") || !strcmp(ret,"..")) {
            ret = NULL;
            continue;
        }
        break;
    }
    return ret;
}

#endif 

DirScanner*
dirScanner_new ( const char*  rootPath )
{
    DirScanner*  s   = android_alloc0(sizeof *s);
    char*        p   = s->root;
    char*        end = p + sizeof s->root;

    p = bufprint(p, end, "%s", rootPath);
    if (p >= end)
        goto FAIL;

    s->rootLen = (p - s->root);

    if (_dirScannerInit(s) < 0)
        goto FAIL;

    return s;

FAIL:
    dirScanner_free(s);
    return NULL;
}


void
dirScanner_free( DirScanner*  s )
{
    if (!s)
        return;

    _dirScanner_done(s);
    AFREE(s);
}


const char*
dirScanner_nextFull( DirScanner*  s )
{
    const char*  name = dirScanner_next(s);
    char*        p;
    char*        end;

    if (name == NULL)
        return NULL;

    p   = s->full;
    end = p + sizeof s->full;

    p = bufprint(p, end, "%.*s/%s", s->rootLen, s->root, name);
    if (p >= end) {
        
        return dirScanner_nextFull(s);
    }
    return s->full;
}
