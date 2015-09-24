/*
 * Block driver for the QCOW version 2 format
 *
 * Copyright (c) 2004-2006 Fabrice Bellard
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

#include <zlib.h>

#include "qemu-common.h"
#include "block_int.h"
#include "block/qcow2.h"

int qcow2_grow_l1_table(BlockDriverState *bs, int min_size)
{
    BDRVQcowState *s = bs->opaque;
    int new_l1_size, new_l1_size2, ret, i;
    uint64_t *new_l1_table;
    int64_t new_l1_table_offset;
    uint8_t data[12];

    new_l1_size = s->l1_size;
    if (min_size <= new_l1_size)
        return 0;
    if (new_l1_size == 0) {
        new_l1_size = 1;
    }
    while (min_size > new_l1_size) {
        new_l1_size = (new_l1_size * 3 + 1) / 2;
    }
#ifdef DEBUG_ALLOC2
    printf("grow l1_table from %d to %d\n", s->l1_size, new_l1_size);
#endif

    new_l1_size2 = sizeof(uint64_t) * new_l1_size;
    new_l1_table = qemu_mallocz(align_offset(new_l1_size2, 512));
    memcpy(new_l1_table, s->l1_table, s->l1_size * sizeof(uint64_t));

    
    BLKDBG_EVENT(bs->file, BLKDBG_L1_GROW_ALLOC_TABLE);
    new_l1_table_offset = qcow2_alloc_clusters(bs, new_l1_size2);
    if (new_l1_table_offset < 0) {
        qemu_free(new_l1_table);
        return new_l1_table_offset;
    }

    BLKDBG_EVENT(bs->file, BLKDBG_L1_GROW_WRITE_TABLE);
    for(i = 0; i < s->l1_size; i++)
        new_l1_table[i] = cpu_to_be64(new_l1_table[i]);
    ret = bdrv_pwrite_sync(bs->file, new_l1_table_offset, new_l1_table, new_l1_size2);
    if (ret < 0)
        goto fail;
    for(i = 0; i < s->l1_size; i++)
        new_l1_table[i] = be64_to_cpu(new_l1_table[i]);

    
    BLKDBG_EVENT(bs->file, BLKDBG_L1_GROW_ACTIVATE_TABLE);
    cpu_to_be32w((uint32_t*)data, new_l1_size);
    cpu_to_be64w((uint64_t*)(data + 4), new_l1_table_offset);
    ret = bdrv_pwrite_sync(bs->file, offsetof(QCowHeader, l1_size), data,sizeof(data));
    if (ret < 0) {
        goto fail;
    }
    qemu_free(s->l1_table);
    qcow2_free_clusters(bs, s->l1_table_offset, s->l1_size * sizeof(uint64_t));
    s->l1_table_offset = new_l1_table_offset;
    s->l1_table = new_l1_table;
    s->l1_size = new_l1_size;
    return 0;
 fail:
    qemu_free(new_l1_table);
    qcow2_free_clusters(bs, new_l1_table_offset, new_l1_size2);
    return ret;
}

void qcow2_l2_cache_reset(BlockDriverState *bs)
{
    BDRVQcowState *s = bs->opaque;

    memset(s->l2_cache, 0, s->l2_size * L2_CACHE_SIZE * sizeof(uint64_t));
    memset(s->l2_cache_offsets, 0, L2_CACHE_SIZE * sizeof(uint64_t));
    memset(s->l2_cache_counts, 0, L2_CACHE_SIZE * sizeof(uint32_t));
}

static inline int l2_cache_new_entry(BlockDriverState *bs)
{
    BDRVQcowState *s = bs->opaque;
    uint32_t min_count;
    int min_index, i;

    
    min_index = 0;
    min_count = 0xffffffff;
    for(i = 0; i < L2_CACHE_SIZE; i++) {
        if (s->l2_cache_counts[i] < min_count) {
            min_count = s->l2_cache_counts[i];
            min_index = i;
        }
    }
    return min_index;
}


static uint64_t *seek_l2_table(BDRVQcowState *s, uint64_t l2_offset)
{
    int i, j;

    for(i = 0; i < L2_CACHE_SIZE; i++) {
        if (l2_offset == s->l2_cache_offsets[i]) {
            
            if (++s->l2_cache_counts[i] == 0xffffffff) {
                for(j = 0; j < L2_CACHE_SIZE; j++) {
                    s->l2_cache_counts[j] >>= 1;
                }
            }
            return s->l2_cache + (i << s->l2_bits);
        }
    }
    return NULL;
}


static int l2_load(BlockDriverState *bs, uint64_t l2_offset,
    uint64_t **l2_table)
{
    BDRVQcowState *s = bs->opaque;
    int min_index;
    int ret;

    

    *l2_table = seek_l2_table(s, l2_offset);
    if (*l2_table != NULL) {
        return 0;
    }

    

    min_index = l2_cache_new_entry(bs);
    *l2_table = s->l2_cache + (min_index << s->l2_bits);

    BLKDBG_EVENT(bs->file, BLKDBG_L2_LOAD);
    ret = bdrv_pread(bs->file, l2_offset, *l2_table,
        s->l2_size * sizeof(uint64_t));
    if (ret < 0) {
        return ret;
    }

    s->l2_cache_offsets[min_index] = l2_offset;
    s->l2_cache_counts[min_index] = 1;

    return 0;
}

#define L1_ENTRIES_PER_SECTOR (512 / 8)
static int write_l1_entry(BlockDriverState *bs, int l1_index)
{
    BDRVQcowState *s = bs->opaque;
    uint64_t buf[L1_ENTRIES_PER_SECTOR];
    int l1_start_index;
    int i, ret;

    l1_start_index = l1_index & ~(L1_ENTRIES_PER_SECTOR - 1);
    for (i = 0; i < L1_ENTRIES_PER_SECTOR; i++) {
        buf[i] = cpu_to_be64(s->l1_table[l1_start_index + i]);
    }

    BLKDBG_EVENT(bs->file, BLKDBG_L1_UPDATE);
    ret = bdrv_pwrite_sync(bs->file, s->l1_table_offset + 8 * l1_start_index,
        buf, sizeof(buf));
    if (ret < 0) {
        return ret;
    }

    return 0;
}


static int l2_allocate(BlockDriverState *bs, int l1_index, uint64_t **table)
{
    BDRVQcowState *s = bs->opaque;
    int min_index;
    uint64_t old_l2_offset;
    uint64_t *l2_table;
    int64_t l2_offset;
    int ret;

    old_l2_offset = s->l1_table[l1_index];

    

    l2_offset = qcow2_alloc_clusters(bs, s->l2_size * sizeof(uint64_t));
    if (l2_offset < 0) {
        return l2_offset;
    }

    

    min_index = l2_cache_new_entry(bs);
    l2_table = s->l2_cache + (min_index << s->l2_bits);

    if (old_l2_offset == 0) {
        
        memset(l2_table, 0, s->l2_size * sizeof(uint64_t));
    } else {
        
        BLKDBG_EVENT(bs->file, BLKDBG_L2_ALLOC_COW_READ);
        ret = bdrv_pread(bs->file, old_l2_offset, l2_table,
            s->l2_size * sizeof(uint64_t));
        if (ret < 0) {
            goto fail;
        }
    }
    
    BLKDBG_EVENT(bs->file, BLKDBG_L2_ALLOC_WRITE);
    ret = bdrv_pwrite_sync(bs->file, l2_offset, l2_table,
        s->l2_size * sizeof(uint64_t));
    if (ret < 0) {
        goto fail;
    }

    
    s->l1_table[l1_index] = l2_offset | QCOW_OFLAG_COPIED;
    ret = write_l1_entry(bs, l1_index);
    if (ret < 0) {
        goto fail;
    }

    

    s->l2_cache_offsets[min_index] = l2_offset;
    s->l2_cache_counts[min_index] = 1;

    *table = l2_table;
    return 0;

fail:
    s->l1_table[l1_index] = old_l2_offset;
    qcow2_l2_cache_reset(bs);
    return ret;
}

static int count_contiguous_clusters(uint64_t nb_clusters, int cluster_size,
        uint64_t *l2_table, uint64_t start, uint64_t mask)
{
    int i;
    uint64_t offset = be64_to_cpu(l2_table[0]) & ~mask;

    if (!offset)
        return 0;

    for (i = start; i < start + nb_clusters; i++)
        if (offset + (uint64_t) i * cluster_size != (be64_to_cpu(l2_table[i]) & ~mask))
            break;

	return (i - start);
}

static int count_contiguous_free_clusters(uint64_t nb_clusters, uint64_t *l2_table)
{
    int i = 0;

    while(nb_clusters-- && l2_table[i] == 0)
        i++;

    return i;
}

void qcow2_encrypt_sectors(BDRVQcowState *s, int64_t sector_num,
                           uint8_t *out_buf, const uint8_t *in_buf,
                           int nb_sectors, int enc,
                           const AES_KEY *key)
{
    union {
        uint64_t ll[2];
        uint8_t b[16];
    } ivec;
    int i;

    for(i = 0; i < nb_sectors; i++) {
        ivec.ll[0] = cpu_to_le64(sector_num);
        ivec.ll[1] = 0;
        AES_cbc_encrypt(in_buf, out_buf, 512, key,
                        ivec.b, enc);
        sector_num++;
        in_buf += 512;
        out_buf += 512;
    }
}


static int qcow_read(BlockDriverState *bs, int64_t sector_num,
                     uint8_t *buf, int nb_sectors)
{
    BDRVQcowState *s = bs->opaque;
    int ret, index_in_cluster, n, n1;
    uint64_t cluster_offset;

    while (nb_sectors > 0) {
        n = nb_sectors;

        ret = qcow2_get_cluster_offset(bs, sector_num << 9, &n,
            &cluster_offset);
        if (ret < 0) {
            return ret;
        }

        index_in_cluster = sector_num & (s->cluster_sectors - 1);
        if (!cluster_offset) {
            if (bs->backing_hd) {
                
                n1 = qcow2_backing_read1(bs->backing_hd, sector_num, buf, n);
                if (n1 > 0) {
                    BLKDBG_EVENT(bs->file, BLKDBG_READ_BACKING);
                    ret = bdrv_read(bs->backing_hd, sector_num, buf, n1);
                    if (ret < 0)
                        return -1;
                }
            } else {
                memset(buf, 0, 512 * n);
            }
        } else if (cluster_offset & QCOW_OFLAG_COMPRESSED) {
            if (qcow2_decompress_cluster(bs, cluster_offset) < 0)
                return -1;
            memcpy(buf, s->cluster_cache + index_in_cluster * 512, 512 * n);
        } else {
            BLKDBG_EVENT(bs->file, BLKDBG_READ);
            ret = bdrv_pread(bs->file, cluster_offset + index_in_cluster * 512, buf, n * 512);
            if (ret != n * 512)
                return -1;
            if (s->crypt_method) {
                qcow2_encrypt_sectors(s, sector_num, buf, buf, n, 0,
                                &s->aes_decrypt_key);
            }
        }
        nb_sectors -= n;
        sector_num += n;
        buf += n * 512;
    }
    return 0;
}

static int copy_sectors(BlockDriverState *bs, uint64_t start_sect,
                        uint64_t cluster_offset, int n_start, int n_end)
{
    BDRVQcowState *s = bs->opaque;
    int n, ret;

    n = n_end - n_start;
    if (n <= 0)
        return 0;
    BLKDBG_EVENT(bs->file, BLKDBG_COW_READ);
    ret = qcow_read(bs, start_sect + n_start, s->cluster_data, n);
    if (ret < 0)
        return ret;
    if (s->crypt_method) {
        qcow2_encrypt_sectors(s, start_sect + n_start,
                        s->cluster_data,
                        s->cluster_data, n, 1,
                        &s->aes_encrypt_key);
    }
    BLKDBG_EVENT(bs->file, BLKDBG_COW_WRITE);
    ret = bdrv_write_sync(bs->file, (cluster_offset >> 9) + n_start,
        s->cluster_data, n);
    if (ret < 0)
        return ret;
    return 0;
}



int qcow2_get_cluster_offset(BlockDriverState *bs, uint64_t offset,
    int *num, uint64_t *cluster_offset)
{
    BDRVQcowState *s = bs->opaque;
    unsigned int l1_index, l2_index;
    uint64_t l2_offset, *l2_table;
    int l1_bits, c;
    unsigned int index_in_cluster, nb_clusters;
    uint64_t nb_available, nb_needed;
    int ret;

    index_in_cluster = (offset >> 9) & (s->cluster_sectors - 1);
    nb_needed = *num + index_in_cluster;

    l1_bits = s->l2_bits + s->cluster_bits;


    nb_available = (1ULL << l1_bits) - (offset & ((1ULL << l1_bits) - 1));

    

    nb_available = (nb_available >> 9) + index_in_cluster;

    if (nb_needed > nb_available) {
        nb_needed = nb_available;
    }

    *cluster_offset = 0;

    

    l1_index = offset >> l1_bits;
    if (l1_index >= s->l1_size)
        goto out;

    l2_offset = s->l1_table[l1_index];

    

    if (!l2_offset)
        goto out;

    

    l2_offset &= ~QCOW_OFLAG_COPIED;
    ret = l2_load(bs, l2_offset, &l2_table);
    if (ret < 0) {
        return ret;
    }

    

    l2_index = (offset >> s->cluster_bits) & (s->l2_size - 1);
    *cluster_offset = be64_to_cpu(l2_table[l2_index]);
    nb_clusters = size_to_clusters(s, nb_needed << 9);

    if (!*cluster_offset) {
        
        c = count_contiguous_free_clusters(nb_clusters, &l2_table[l2_index]);
    } else {
        
        c = count_contiguous_clusters(nb_clusters, s->cluster_size,
                &l2_table[l2_index], 0, QCOW_OFLAG_COPIED);
    }

   nb_available = (c * s->cluster_sectors);
out:
    if (nb_available > nb_needed)
        nb_available = nb_needed;

    *num = nb_available - index_in_cluster;

    *cluster_offset &=~QCOW_OFLAG_COPIED;
    return 0;
}

static int get_cluster_table(BlockDriverState *bs, uint64_t offset,
                             uint64_t **new_l2_table,
                             uint64_t *new_l2_offset,
                             int *new_l2_index)
{
    BDRVQcowState *s = bs->opaque;
    unsigned int l1_index, l2_index;
    uint64_t l2_offset;
    uint64_t *l2_table = NULL;
    int ret;

    

    l1_index = offset >> (s->l2_bits + s->cluster_bits);
    if (l1_index >= s->l1_size) {
        ret = qcow2_grow_l1_table(bs, l1_index + 1);
        if (ret < 0) {
            return ret;
        }
    }
    l2_offset = s->l1_table[l1_index];

    

    if (l2_offset & QCOW_OFLAG_COPIED) {
        
        l2_offset &= ~QCOW_OFLAG_COPIED;
        ret = l2_load(bs, l2_offset, &l2_table);
        if (ret < 0) {
            return ret;
        }
    } else {
        if (l2_offset)
            qcow2_free_clusters(bs, l2_offset, s->l2_size * sizeof(uint64_t));
        ret = l2_allocate(bs, l1_index, &l2_table);
        if (ret < 0) {
            return ret;
        }
        l2_offset = s->l1_table[l1_index] & ~QCOW_OFLAG_COPIED;
    }

    

    l2_index = (offset >> s->cluster_bits) & (s->l2_size - 1);

    *new_l2_table = l2_table;
    *new_l2_offset = l2_offset;
    *new_l2_index = l2_index;

    return 0;
}


uint64_t qcow2_alloc_compressed_cluster_offset(BlockDriverState *bs,
                                               uint64_t offset,
                                               int compressed_size)
{
    BDRVQcowState *s = bs->opaque;
    int l2_index, ret;
    uint64_t l2_offset, *l2_table;
    int64_t cluster_offset;
    int nb_csectors;

    ret = get_cluster_table(bs, offset, &l2_table, &l2_offset, &l2_index);
    if (ret < 0) {
        return 0;
    }

    cluster_offset = be64_to_cpu(l2_table[l2_index]);
    if (cluster_offset & QCOW_OFLAG_COPIED)
        return cluster_offset & ~QCOW_OFLAG_COPIED;

    if (cluster_offset)
        qcow2_free_any_clusters(bs, cluster_offset, 1);

    cluster_offset = qcow2_alloc_bytes(bs, compressed_size);
    if (cluster_offset < 0) {
        return 0;
    }

    nb_csectors = ((cluster_offset + compressed_size - 1) >> 9) -
                  (cluster_offset >> 9);

    cluster_offset |= QCOW_OFLAG_COMPRESSED |
                      ((uint64_t)nb_csectors << s->csize_shift);

    

    

    BLKDBG_EVENT(bs->file, BLKDBG_L2_UPDATE_COMPRESSED);
    l2_table[l2_index] = cpu_to_be64(cluster_offset);
    if (bdrv_pwrite_sync(bs->file,
                    l2_offset + l2_index * sizeof(uint64_t),
                    l2_table + l2_index,
                    sizeof(uint64_t)) < 0)
        return 0;

    return cluster_offset;
}

#define L2_ENTRIES_PER_SECTOR (512 / 8)
static int write_l2_entries(BlockDriverState *bs, uint64_t *l2_table,
    uint64_t l2_offset, int l2_index, int num)
{
    int l2_start_index = l2_index & ~(L1_ENTRIES_PER_SECTOR - 1);
    int start_offset = (8 * l2_index) & ~511;
    int end_offset = (8 * (l2_index + num) + 511) & ~511;
    size_t len = end_offset - start_offset;
    int ret;

    BLKDBG_EVENT(bs->file, BLKDBG_L2_UPDATE);
    ret = bdrv_pwrite_sync(bs->file, l2_offset + start_offset,
        &l2_table[l2_start_index], len);
    if (ret < 0) {
        return ret;
    }

    return 0;
}

int qcow2_alloc_cluster_link_l2(BlockDriverState *bs, QCowL2Meta *m)
{
    BDRVQcowState *s = bs->opaque;
    int i, j = 0, l2_index, ret;
    uint64_t *old_cluster, start_sect, l2_offset, *l2_table;
    uint64_t cluster_offset = m->cluster_offset;

    if (m->nb_clusters == 0)
        return 0;

    old_cluster = qemu_malloc(m->nb_clusters * sizeof(uint64_t));

    
    start_sect = (m->offset & ~(s->cluster_size - 1)) >> 9;
    if (m->n_start) {
        ret = copy_sectors(bs, start_sect, cluster_offset, 0, m->n_start);
        if (ret < 0)
            goto err;
    }

    if (m->nb_available & (s->cluster_sectors - 1)) {
        uint64_t end = m->nb_available & ~(uint64_t)(s->cluster_sectors - 1);
        ret = copy_sectors(bs, start_sect + end, cluster_offset + (end << 9),
                m->nb_available - end, s->cluster_sectors);
        if (ret < 0)
            goto err;
    }

    
    ret = get_cluster_table(bs, m->offset, &l2_table, &l2_offset, &l2_index);
    if (ret < 0) {
        goto err;
    }

    for (i = 0; i < m->nb_clusters; i++) {
        if(l2_table[l2_index + i] != 0)
            old_cluster[j++] = l2_table[l2_index + i];

        l2_table[l2_index + i] = cpu_to_be64((cluster_offset +
                    (i << s->cluster_bits)) | QCOW_OFLAG_COPIED);
     }

    ret = write_l2_entries(bs, l2_table, l2_offset, l2_index, m->nb_clusters);
    if (ret < 0) {
        qcow2_l2_cache_reset(bs);
        goto err;
    }

    for (i = 0; i < j; i++)
        qcow2_free_any_clusters(bs,
            be64_to_cpu(old_cluster[i]) & ~QCOW_OFLAG_COPIED, 1);

    ret = 0;
err:
    qemu_free(old_cluster);
    return ret;
 }

int qcow2_alloc_cluster_offset(BlockDriverState *bs, uint64_t offset,
    int n_start, int n_end, int *num, QCowL2Meta *m)
{
    BDRVQcowState *s = bs->opaque;
    int l2_index, ret;
    uint64_t l2_offset, *l2_table;
    int64_t cluster_offset;
    unsigned int nb_clusters, i = 0;
    QCowL2Meta *old_alloc;

    ret = get_cluster_table(bs, offset, &l2_table, &l2_offset, &l2_index);
    if (ret < 0) {
        return ret;
    }

    nb_clusters = size_to_clusters(s, n_end << 9);

    nb_clusters = MIN(nb_clusters, s->l2_size - l2_index);

    cluster_offset = be64_to_cpu(l2_table[l2_index]);

    

    if (cluster_offset & QCOW_OFLAG_COPIED) {
        nb_clusters = count_contiguous_clusters(nb_clusters, s->cluster_size,
                &l2_table[l2_index], 0, 0);

        cluster_offset &= ~QCOW_OFLAG_COPIED;
        m->nb_clusters = 0;
        m->depends_on = NULL;

        goto out;
    }

    

    if (cluster_offset & QCOW_OFLAG_COMPRESSED)
        nb_clusters = 1;

    

    while (i < nb_clusters) {
        i += count_contiguous_clusters(nb_clusters - i, s->cluster_size,
                &l2_table[l2_index], i, 0);
        if ((i >= nb_clusters) || be64_to_cpu(l2_table[l2_index + i])) {
            break;
        }

        i += count_contiguous_free_clusters(nb_clusters - i,
                &l2_table[l2_index + i]);
        if (i >= nb_clusters) {
            break;
        }

        cluster_offset = be64_to_cpu(l2_table[l2_index + i]);

        if ((cluster_offset & QCOW_OFLAG_COPIED) ||
                (cluster_offset & QCOW_OFLAG_COMPRESSED))
            break;
    }
    assert(i <= nb_clusters);
    nb_clusters = i;

    QLIST_FOREACH(old_alloc, &s->cluster_allocs, next_in_flight) {

        uint64_t end_offset = offset + nb_clusters * s->cluster_size;
        uint64_t old_offset = old_alloc->offset;
        uint64_t old_end_offset = old_alloc->offset +
            old_alloc->nb_clusters * s->cluster_size;

        if (end_offset < old_offset || offset > old_end_offset) {
            
        } else {
            if (offset < old_offset) {
                
                nb_clusters = (old_offset - offset) >> s->cluster_bits;
            } else {
                nb_clusters = 0;
            }

            if (nb_clusters == 0) {
                
                m->depends_on = old_alloc;
                m->nb_clusters = 0;
                *num = 0;
                return 0;
            }
        }
    }

    if (!nb_clusters) {
        abort();
    }

    QLIST_INSERT_HEAD(&s->cluster_allocs, m, next_in_flight);

    

    cluster_offset = qcow2_alloc_clusters(bs, nb_clusters * s->cluster_size);
    if (cluster_offset < 0) {
        QLIST_REMOVE(m, next_in_flight);
        return cluster_offset;
    }

    
    m->offset = offset;
    m->n_start = n_start;
    m->nb_clusters = nb_clusters;

out:
    m->nb_available = MIN(nb_clusters << (s->cluster_bits - 9), n_end);
    m->cluster_offset = cluster_offset;

    *num = m->nb_available - n_start;

    return 0;
}

static int decompress_buffer(uint8_t *out_buf, int out_buf_size,
                             const uint8_t *buf, int buf_size)
{
    z_stream strm1, *strm = &strm1;
    int ret, out_len;

    memset(strm, 0, sizeof(*strm));

    strm->next_in = (uint8_t *)buf;
    strm->avail_in = buf_size;
    strm->next_out = out_buf;
    strm->avail_out = out_buf_size;

    ret = inflateInit2(strm, -12);
    if (ret != Z_OK)
        return -1;
    ret = inflate(strm, Z_FINISH);
    out_len = strm->next_out - out_buf;
    if ((ret != Z_STREAM_END && ret != Z_BUF_ERROR) ||
        out_len != out_buf_size) {
        inflateEnd(strm);
        return -1;
    }
    inflateEnd(strm);
    return 0;
}

int qcow2_decompress_cluster(BlockDriverState *bs, uint64_t cluster_offset)
{
    BDRVQcowState *s = bs->opaque;
    int ret, csize, nb_csectors, sector_offset;
    uint64_t coffset;

    coffset = cluster_offset & s->cluster_offset_mask;
    if (s->cluster_cache_offset != coffset) {
        nb_csectors = ((cluster_offset >> s->csize_shift) & s->csize_mask) + 1;
        sector_offset = coffset & 511;
        csize = nb_csectors * 512 - sector_offset;
        BLKDBG_EVENT(bs->file, BLKDBG_READ_COMPRESSED);
        ret = bdrv_read(bs->file, coffset >> 9, s->cluster_data, nb_csectors);
        if (ret < 0) {
            return -1;
        }
        if (decompress_buffer(s->cluster_cache, s->cluster_size,
                              s->cluster_data + sector_offset, csize) < 0) {
            return -1;
        }
        s->cluster_cache_offset = coffset;
    }
    return 0;
}
