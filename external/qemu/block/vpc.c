/*
 * Block driver for Connectix / Microsoft Virtual PC images
 *
 * Copyright (c) 2005 Alex Beregszaszi
 * Copyright (c) 2009 Kevin Wolf <kwolf@suse.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "qemu-common.h"
#include "block_int.h"
#include "module.h"


#define HEADER_SIZE 512


enum vhd_type {
    VHD_FIXED           = 2,
    VHD_DYNAMIC         = 3,
    VHD_DIFFERENCING    = 4,
};

#define VHD_TIMESTAMP_BASE 946684800

struct vhd_footer {
    char        creator[8]; 
    uint32_t    features;
    uint32_t    version;

    
    uint64_t    data_offset;

    
    uint32_t    timestamp;

    char        creator_app[4]; 
    uint16_t    major;
    uint16_t    minor;
    char        creator_os[4]; 

    uint64_t    orig_size;
    uint64_t    size;

    uint16_t    cyls;
    uint8_t     heads;
    uint8_t     secs_per_cyl;

    uint32_t    type;

    
    
    uint32_t    checksum;

    
    uint8_t     uuid[16];

    uint8_t     in_saved_state;
};

struct vhd_dyndisk_header {
    char        magic[8]; 

    
    uint64_t    data_offset;

    
    uint64_t    table_offset;

    uint32_t    version;
    uint32_t    max_table_entries; 

    
    uint32_t    block_size;

    uint32_t    checksum;
    uint8_t     parent_uuid[16];
    uint32_t    parent_timestamp;
    uint32_t    reserved;

    
    uint8_t     parent_name[512];

    struct {
        uint32_t    platform;
        uint32_t    data_space;
        uint32_t    data_length;
        uint32_t    reserved;
        uint64_t    data_offset;
    } parent_locator[8];
};

typedef struct BDRVVPCState {
    BlockDriverState *hd;

    uint8_t footer_buf[HEADER_SIZE];
    uint64_t free_data_block_offset;
    int max_table_entries;
    uint32_t *pagetable;
    uint64_t bat_offset;
    uint64_t last_bitmap_offset;

    uint32_t block_size;
    uint32_t bitmap_size;

#ifdef CACHE
    uint8_t *pageentry_u8;
    uint32_t *pageentry_u32;
    uint16_t *pageentry_u16;

    uint64_t last_bitmap;
#endif
} BDRVVPCState;

static uint32_t vpc_checksum(uint8_t* buf, size_t size)
{
    uint32_t res = 0;
    int i;

    for (i = 0; i < size; i++)
        res += buf[i];

    return ~res;
}


static int vpc_probe(const uint8_t *buf, int buf_size, const char *filename)
{
    if (buf_size >= 8 && !strncmp((char *)buf, "conectix", 8))
	return 100;
    return 0;
}

static int vpc_open(BlockDriverState *bs, int flags)
{
    BDRVVPCState *s = bs->opaque;
    int i;
    struct vhd_footer* footer;
    struct vhd_dyndisk_header* dyndisk_header;
    uint8_t buf[HEADER_SIZE];
    uint32_t checksum;

    if (bdrv_pread(bs->file, 0, s->footer_buf, HEADER_SIZE) != HEADER_SIZE)
        goto fail;

    footer = (struct vhd_footer*) s->footer_buf;
    if (strncmp(footer->creator, "conectix", 8))
        goto fail;

    checksum = be32_to_cpu(footer->checksum);
    footer->checksum = 0;
    if (vpc_checksum(s->footer_buf, HEADER_SIZE) != checksum)
        fprintf(stderr, "block-vpc: The header checksum of '%s' is "
            "incorrect.\n", bs->filename);

    
    
    
    bs->total_sectors = (int64_t)
        be16_to_cpu(footer->cyls) * footer->heads * footer->secs_per_cyl;

    if (bdrv_pread(bs->file, be64_to_cpu(footer->data_offset), buf, HEADER_SIZE)
            != HEADER_SIZE)
        goto fail;

    dyndisk_header = (struct vhd_dyndisk_header*) buf;

    if (strncmp(dyndisk_header->magic, "cxsparse", 8))
        goto fail;


    s->block_size = be32_to_cpu(dyndisk_header->block_size);
    s->bitmap_size = ((s->block_size / (8 * 512)) + 511) & ~511;

    s->max_table_entries = be32_to_cpu(dyndisk_header->max_table_entries);
    s->pagetable = qemu_malloc(s->max_table_entries * 4);

    s->bat_offset = be64_to_cpu(dyndisk_header->table_offset);
    if (bdrv_pread(bs->file, s->bat_offset, s->pagetable,
            s->max_table_entries * 4) != s->max_table_entries * 4)
	    goto fail;

    s->free_data_block_offset =
        (s->bat_offset + (s->max_table_entries * 4) + 511) & ~511;

    for (i = 0; i < s->max_table_entries; i++) {
        be32_to_cpus(&s->pagetable[i]);
        if (s->pagetable[i] != 0xFFFFFFFF) {
            int64_t next = (512 * (int64_t) s->pagetable[i]) +
                s->bitmap_size + s->block_size;

            if (next> s->free_data_block_offset)
                s->free_data_block_offset = next;
        }
    }

    s->last_bitmap_offset = (int64_t) -1;

#ifdef CACHE
    s->pageentry_u8 = qemu_malloc(512);
    s->pageentry_u32 = s->pageentry_u8;
    s->pageentry_u16 = s->pageentry_u8;
    s->last_pagetable = -1;
#endif

    return 0;
 fail:
    return -1;
}

static inline int64_t get_sector_offset(BlockDriverState *bs,
    int64_t sector_num, int write)
{
    BDRVVPCState *s = bs->opaque;
    uint64_t offset = sector_num * 512;
    uint64_t bitmap_offset, block_offset;
    uint32_t pagetable_index, pageentry_index;

    pagetable_index = offset / s->block_size;
    pageentry_index = (offset % s->block_size) / 512;

    if (pagetable_index >= s->max_table_entries || s->pagetable[pagetable_index] == 0xffffffff)
        return -1; 

    bitmap_offset = 512 * (uint64_t) s->pagetable[pagetable_index];
    block_offset = bitmap_offset + s->bitmap_size + (512 * pageentry_index);

    
    
    
    
    
    if (write && (s->last_bitmap_offset != bitmap_offset)) {
        uint8_t bitmap[s->bitmap_size];

        s->last_bitmap_offset = bitmap_offset;
        memset(bitmap, 0xff, s->bitmap_size);
        bdrv_pwrite_sync(bs->file, bitmap_offset, bitmap, s->bitmap_size);
    }


#if 0
#ifdef CACHE
    if (bitmap_offset != s->last_bitmap)
    {
	lseek(s->fd, bitmap_offset, SEEK_SET);

	s->last_bitmap = bitmap_offset;

	
	
	read(s->fd, s->pageentry_u8, 512);
	for (i = 0; i < 128; i++)
	    be32_to_cpus(&s->pageentry_u32[i]);
    }

    if ((s->pageentry_u8[pageentry_index / 8] >> (pageentry_index % 8)) & 1)
	return -1;
#else
    lseek(s->fd, bitmap_offset + (pageentry_index / 8), SEEK_SET);

    read(s->fd, &bitmap_entry, 1);

    if ((bitmap_entry >> (pageentry_index % 8)) & 1)
	return -1; 
#endif
#endif

    return block_offset;
}

static int rewrite_footer(BlockDriverState* bs)
{
    int ret;
    BDRVVPCState *s = bs->opaque;
    int64_t offset = s->free_data_block_offset;

    ret = bdrv_pwrite_sync(bs->file, offset, s->footer_buf, HEADER_SIZE);
    if (ret < 0)
        return ret;

    return 0;
}

static int64_t alloc_block(BlockDriverState* bs, int64_t sector_num)
{
    BDRVVPCState *s = bs->opaque;
    int64_t bat_offset;
    uint32_t index, bat_value;
    int ret;
    uint8_t bitmap[s->bitmap_size];

    
    if ((sector_num < 0) || (sector_num > bs->total_sectors))
        return -1;

    
    index = (sector_num * 512) / s->block_size;
    if (s->pagetable[index] != 0xFFFFFFFF)
        return -1;

    s->pagetable[index] = s->free_data_block_offset / 512;

    
    memset(bitmap, 0xff, s->bitmap_size);
    bdrv_pwrite_sync(bs->file, s->free_data_block_offset, bitmap,
        s->bitmap_size);

    // Write new footer (the old one will be overwritten)
    s->free_data_block_offset += s->block_size + s->bitmap_size;
    ret = rewrite_footer(bs);
    if (ret < 0)
        goto fail;

    
    bat_offset = s->bat_offset + (4 * index);
    bat_value = be32_to_cpu(s->pagetable[index]);
    ret = bdrv_pwrite_sync(bs->file, bat_offset, &bat_value, 4);
    if (ret < 0)
        goto fail;

    return get_sector_offset(bs, sector_num, 0);

fail:
    s->free_data_block_offset -= (s->block_size + s->bitmap_size);
    return -1;
}

static int vpc_read(BlockDriverState *bs, int64_t sector_num,
                    uint8_t *buf, int nb_sectors)
{
    BDRVVPCState *s = bs->opaque;
    int ret;
    int64_t offset;
    int64_t sectors, sectors_per_block;

    while (nb_sectors > 0) {
        offset = get_sector_offset(bs, sector_num, 0);

        sectors_per_block = s->block_size >> BDRV_SECTOR_BITS;
        sectors = sectors_per_block - (sector_num % sectors_per_block);
        if (sectors > nb_sectors) {
            sectors = nb_sectors;
        }

        if (offset == -1) {
            memset(buf, 0, sectors * BDRV_SECTOR_SIZE);
        } else {
            ret = bdrv_pread(bs->file, offset, buf,
                sectors * BDRV_SECTOR_SIZE);
            if (ret != sectors * BDRV_SECTOR_SIZE) {
                return -1;
            }
        }

        nb_sectors -= sectors;
        sector_num += sectors;
        buf += sectors * BDRV_SECTOR_SIZE;
    }
    return 0;
}

static int vpc_write(BlockDriverState *bs, int64_t sector_num,
    const uint8_t *buf, int nb_sectors)
{
    BDRVVPCState *s = bs->opaque;
    int64_t offset;
    int64_t sectors, sectors_per_block;
    int ret;

    while (nb_sectors > 0) {
        offset = get_sector_offset(bs, sector_num, 1);

        sectors_per_block = s->block_size >> BDRV_SECTOR_BITS;
        sectors = sectors_per_block - (sector_num % sectors_per_block);
        if (sectors > nb_sectors) {
            sectors = nb_sectors;
        }

        if (offset == -1) {
            offset = alloc_block(bs, sector_num);
            if (offset < 0)
                return -1;
        }

        ret = bdrv_pwrite(bs->file, offset, buf, sectors * BDRV_SECTOR_SIZE);
        if (ret != sectors * BDRV_SECTOR_SIZE) {
            return -1;
        }

        nb_sectors -= sectors;
        sector_num += sectors;
        buf += sectors * BDRV_SECTOR_SIZE;
    }

    return 0;
}


static int calculate_geometry(int64_t total_sectors, uint16_t* cyls,
    uint8_t* heads, uint8_t* secs_per_cyl)
{
    uint32_t cyls_times_heads;

    if (total_sectors > 65535 * 16 * 255)
        return -EFBIG;

    if (total_sectors > 65535 * 16 * 63) {
        *secs_per_cyl = 255;
        *heads = 16;
        cyls_times_heads = total_sectors / *secs_per_cyl;
    } else {
        *secs_per_cyl = 17;
        cyls_times_heads = total_sectors / *secs_per_cyl;
        *heads = (cyls_times_heads + 1023) / 1024;

        if (*heads < 4)
            *heads = 4;

        if (cyls_times_heads >= (*heads * 1024) || *heads > 16) {
            *secs_per_cyl = 31;
            *heads = 16;
            cyls_times_heads = total_sectors / *secs_per_cyl;
        }

        if (cyls_times_heads >= (*heads * 1024)) {
            *secs_per_cyl = 63;
            *heads = 16;
            cyls_times_heads = total_sectors / *secs_per_cyl;
        }
    }

    *cyls = cyls_times_heads / *heads;

    return 0;
}

static int vpc_create(const char *filename, QEMUOptionParameter *options)
{
    uint8_t buf[1024];
    struct vhd_footer* footer = (struct vhd_footer*) buf;
    struct vhd_dyndisk_header* dyndisk_header =
        (struct vhd_dyndisk_header*) buf;
    int fd, i;
    uint16_t cyls = 0;
    uint8_t heads = 0;
    uint8_t secs_per_cyl = 0;
    size_t block_size, num_bat_entries;
    int64_t total_sectors = 0;

    
    while (options && options->name) {
        if (!strcmp(options->name, "size")) {
            total_sectors = options->value.n / 512;
        }
        options++;
    }

    
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
    if (fd < 0)
        return -EIO;

    for (i = 0; total_sectors > (int64_t)cyls * heads * secs_per_cyl; i++) {
        if (calculate_geometry(total_sectors + i,
                               &cyls, &heads, &secs_per_cyl)) {
            return -EFBIG;
        }
    }
    total_sectors = (int64_t) cyls * heads * secs_per_cyl;

    
    memset(buf, 0, 1024);

    memcpy(footer->creator, "conectix", 8);
    
    memcpy(footer->creator_app, "qemu", 4);
    memcpy(footer->creator_os, "Wi2k", 4);

    footer->features = be32_to_cpu(0x02);
    footer->version = be32_to_cpu(0x00010000);
    footer->data_offset = be64_to_cpu(HEADER_SIZE);
    footer->timestamp = be32_to_cpu(time(NULL) - VHD_TIMESTAMP_BASE);

    
    footer->major = be16_to_cpu(0x0005);
    footer->minor =be16_to_cpu(0x0003);

    footer->orig_size = be64_to_cpu(total_sectors * 512);
    footer->size = be64_to_cpu(total_sectors * 512);

    footer->cyls = be16_to_cpu(cyls);
    footer->heads = heads;
    footer->secs_per_cyl = secs_per_cyl;

    footer->type = be32_to_cpu(VHD_DYNAMIC);

    

    footer->checksum = be32_to_cpu(vpc_checksum(buf, HEADER_SIZE));

    
    block_size = 0x200000;
    num_bat_entries = (total_sectors + block_size / 512) / (block_size / 512);

    if (write(fd, buf, HEADER_SIZE) != HEADER_SIZE)
        return -EIO;

    if (lseek(fd, 1536 + ((num_bat_entries * 4 + 511) & ~511), SEEK_SET) < 0)
        return -EIO;
    if (write(fd, buf, HEADER_SIZE) != HEADER_SIZE)
        return -EIO;

    
    if (lseek(fd, 3 * 512, SEEK_SET) < 0)
        return -EIO;

    memset(buf, 0xFF, 512);
    for (i = 0; i < (num_bat_entries * 4 + 511) / 512; i++)
        if (write(fd, buf, 512) != 512)
            return -EIO;


    
    memset(buf, 0, 1024);

    memcpy(dyndisk_header->magic, "cxsparse", 8);

    dyndisk_header->data_offset = be64_to_cpu(0xFFFFFFFF);
    dyndisk_header->table_offset = be64_to_cpu(3 * 512);
    dyndisk_header->version = be32_to_cpu(0x00010000);
    dyndisk_header->block_size = be32_to_cpu(block_size);
    dyndisk_header->max_table_entries = be32_to_cpu(num_bat_entries);

    dyndisk_header->checksum = be32_to_cpu(vpc_checksum(buf, 1024));

    
    if (lseek(fd, 512, SEEK_SET) < 0)
        return -EIO;
    if (write(fd, buf, 1024) != 1024)
        return -EIO;

    close(fd);
    return 0;
}

static void vpc_close(BlockDriverState *bs)
{
    BDRVVPCState *s = bs->opaque;
    qemu_free(s->pagetable);
#ifdef CACHE
    qemu_free(s->pageentry_u8);
#endif
}

static QEMUOptionParameter vpc_create_options[] = {
    {
        .name = BLOCK_OPT_SIZE,
        .type = OPT_SIZE,
        .help = "Virtual disk size"
    },
    { NULL }
};

static BlockDriver bdrv_vpc = {
    .format_name	= "vpc",
    .instance_size	= sizeof(BDRVVPCState),
    .bdrv_probe		= vpc_probe,
    .bdrv_open		= vpc_open,
    .bdrv_read		= vpc_read,
    .bdrv_write		= vpc_write,
    .bdrv_close		= vpc_close,
    .bdrv_create	= vpc_create,

    .create_options = vpc_create_options,
};

static void bdrv_vpc_init(void)
{
    bdrv_register(&bdrv_vpc);
}

block_init(bdrv_vpc_init);
