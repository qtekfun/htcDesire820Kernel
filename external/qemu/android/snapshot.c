/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "bswap.h"
#include "android/utils/debug.h"
#include "android/utils/system.h"
#include "android/snapshot.h"

#define QCOW_MAGIC (('Q' << 24) | ('F' << 16) | ('I' << 8) | 0xfb)
#define QCOW_VERSION 2

static int
read_or_die(int fd, void *buf, size_t nbyte)
{
    int ret = 0;
    do {
        ret = read(fd, buf, nbyte);
    }
    while(ret < 0 && errno == EINTR);

    if (ret < 0) {
        derror("read failed: %s", strerror(errno));
        exit(1);
    }

    return ret;
}

static off_t
seek_or_die(int fd, off_t offset, int whence)
{
    off_t ret = lseek(fd, offset, whence);
    if (ret < 0) {
        derror("seek failed: %s", strerror(errno));
        exit(1);
    }
    return ret;
}


typedef struct SnapshotInfo {
    char *id_str;
    char *name;

    uint32_t date_sec;
    uint32_t date_nsec;

    uint64_t vm_clock_nsec;
    uint32_t vm_state_size;
} SnapshotInfo;

static SnapshotInfo*
snapshot_info_alloc()
{
    return android_alloc(sizeof(SnapshotInfo));
}

static void
snapshot_info_free( SnapshotInfo* info )
{
    AFREE(info->id_str);
    AFREE(info->name);
    AFREE(info);
}


static void
snapshot_info_read( int fd, SnapshotInfo* info )
{
    uint64_t start_offset = seek_or_die(fd, 0, SEEK_CUR);

    uint32_t extra_data_size;
    uint16_t id_str_size, name_size;

    
    seek_or_die(fd, 12, SEEK_CUR);  
    read_or_die(fd, &id_str_size,         sizeof(id_str_size));
    read_or_die(fd, &name_size,           sizeof(name_size));
    read_or_die(fd, &info->date_sec,      sizeof(info->date_sec));
    read_or_die(fd, &info->date_nsec,     sizeof(info->date_nsec));
    read_or_die(fd, &info->vm_clock_nsec, sizeof(info->vm_clock_nsec));
    read_or_die(fd, &info->vm_state_size, sizeof(info->vm_state_size));
    read_or_die(fd, &extra_data_size,     sizeof(extra_data_size));

    
    be16_to_cpus(&id_str_size);
    be16_to_cpus(&name_size);
    be32_to_cpus(&info->date_sec);
    be32_to_cpus(&info->date_nsec);
    be64_to_cpus(&info->vm_clock_nsec);
    be32_to_cpus(&info->vm_state_size);
    be32_to_cpus(&extra_data_size);
    be32_to_cpus(&extra_data_size);

    
    info->id_str = android_alloc(id_str_size + 1); 
    info->name   = android_alloc(name_size + 1);
    seek_or_die(fd, extra_data_size, SEEK_CUR);  
    read_or_die(fd, info->id_str, id_str_size);
    read_or_die(fd, info->name, name_size);

    info->id_str[id_str_size] = '\0';
    info->name[name_size] = '\0';

    
    uint64_t end_offset   = seek_or_die(fd, 0, SEEK_CUR);
    uint32_t total_size   = end_offset - start_offset;
    uint32_t aligned_size = ((total_size - 1) / 8 + 1) * 8;

    
    seek_or_die(fd, start_offset + aligned_size, SEEK_SET);
}


#define NB_SUFFIXES 4

/* Returns the size of a snapshot in a human-readable format.
 *
 * This function copyright (c) 2003 Fabrice Bellard
 */
static char*
snapshot_format_size( char *buf, int buf_size, int64_t size )
{
    static const char suffixes[NB_SUFFIXES] = "KMGT";
    int64_t base;
    int i;

    if (size <= 999) {
        snprintf(buf, buf_size, "%" PRId64, size);
    } else {
        base = 1024;
        for(i = 0; i < NB_SUFFIXES; i++) {
            if (size < (10 * base)) {
                snprintf(buf, buf_size, "%0.1f%c",
                         (double)size / base,
                         suffixes[i]);
                break;
            } else if (size < (1000 * base) || i == (NB_SUFFIXES - 1)) {
                snprintf(buf, buf_size, "%" PRId64 "%c",
                         ((size + (base >> 1)) / base),
                         suffixes[i]);
                break;
            }
            base = base * 1024;
        }
    }
    return buf;
}

static char*
snapshot_format_create_date( char *buf, size_t buf_size, time_t *time )
{
    struct tm *tm;
    tm = localtime(time);
    strftime(buf, buf_size, "%Y-%m-%d %H:%M:%S", tm);
    return buf;
}

static char*
snapshot_format_vm_clock( char *buf, size_t buf_size, uint64_t vm_clock_nsec )
{
    uint64_t secs = vm_clock_nsec / 1000000000;
    snprintf(buf, buf_size, "%02d:%02d:%02d.%03d",
                            (int)(secs / 3600),
                            (int)((secs / 60) % 60),
                            (int)(secs % 60),
                            (int)((vm_clock_nsec / 1000000) % 1000));
    return buf;
}

static void
snapshot_info_print( SnapshotInfo *info )
{
    char size_buf[8];
    char date_buf[21];
    char clock_buf[21];

    snapshot_format_size(size_buf, sizeof(size_buf), info->vm_state_size);
    snapshot_format_create_date(date_buf, sizeof(date_buf),
                                (time_t*) &info->date_sec);
    snapshot_format_vm_clock(clock_buf, sizeof(clock_buf), info->vm_clock_nsec);

    printf(" %-10s%-20s%7s%20s%15s\n",
           info->id_str, info->name, size_buf, date_buf, clock_buf);
}

static void
snapshot_print_table( int fd, uint32_t nb_snapshots, uint64_t snapshots_offset )
{
    printf(" %-10s%-20s%7s%20s%15s\n",
           "ID", "TAG", "VM SIZE", "DATE", "VM CLOCK");

    
    seek_or_die(fd, snapshots_offset, SEEK_SET);

    
    int i;
    for (i = 0; i < nb_snapshots; i++) {
        SnapshotInfo *info = snapshot_info_alloc();
        snapshot_info_read(fd, info);
        snapshot_info_print(info);

        snapshot_info_free(info);
    }
}

static void
snapshot_validate_qcow_file( int fd )
{
    
    uint32_t magic, version;
    read_or_die(fd, &magic, sizeof(magic));
    read_or_die(fd, &version, sizeof(version));
    be32_to_cpus(&magic);
    be32_to_cpus(&version);

    if (magic != QCOW_MAGIC) {
        derror("Not a valid Qcow snapshot file (expected magic value '%08x', got '%08x').",
               QCOW_MAGIC, magic);
        exit(1);
    }
    if (version != QCOW_VERSION) {
        derror("Unsupported Qcow version (need %d, got %d).",
               QCOW_VERSION, version);
        exit(1);
    }
}

static void
snapshot_read_qcow_header( int fd, uint32_t *nb_snapshots, uint64_t *snapshots_offset )
{
    snapshot_validate_qcow_file(fd);

    
    seek_or_die(fd, 52, SEEK_CUR);

    read_or_die(fd, nb_snapshots, sizeof(*nb_snapshots));
    read_or_die(fd, snapshots_offset, sizeof(*snapshots_offset));

    
    be32_to_cpus(nb_snapshots);
    be64_to_cpus(snapshots_offset);
}

void
snapshot_print_and_exit( const char *snapstorage )
{
    
    int fd = open(snapstorage, O_RDONLY);
    if (fd < 0) {
        derror("Could not open snapshot file '%s': %s", snapstorage, strerror(errno));
        exit(1);
    }

    
    uint32_t nb_snapshots;
    uint64_t snapshots_offset;
    snapshot_read_qcow_header(fd, &nb_snapshots, &snapshots_offset);

    if (nb_snapshots > 0) {
        printf("Snapshots in file '%s':\n", snapstorage);
        snapshot_print_table(fd, nb_snapshots, snapshots_offset);
    }
    else {
        printf("File '%s' contains no snapshots yet.\n", snapstorage);
    }

    close(fd);
    exit(0);
}
