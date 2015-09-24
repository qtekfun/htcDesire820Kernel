/* Copyright (C) 2007-2010 The Android Open Source Project
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


#ifndef _ANDROID_UTILS_FILEIO_H
#define _ANDROID_UTILS_FILEIO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct MapFile MapFile;

#ifdef WIN32
#define PROT_READ   0x1
#define PROT_WRITE  0x2
#define PROT_EXEC   0x4
#define PROT_NONE   0x0
#endif

static inline int
mapfile_is_valid(MapFile* handle)
{
    return handle != (void*)(ptrdiff_t)-1;
}

extern MapFile* mapfile_open(const char* path, int oflag, int share_mode);

extern int mapfile_close(MapFile* handle);

extern ssize_t mapfile_read(MapFile* handle, void* buf, size_t nbyte);

extern ssize_t mapfile_read_at(MapFile* handle,
                               size_t offset,
                               void* buf,
                               size_t nbyte);

extern void* mapfile_map(MapFile* handle,
                         size_t offset,
                         size_t size,
                         int prot,
                         void** mapped_offset,
                         size_t* mapped_size);

extern int mapfile_unmap(void* mapped_at, size_t len);

#ifdef __cplusplus
}   
#endif

#endif  
