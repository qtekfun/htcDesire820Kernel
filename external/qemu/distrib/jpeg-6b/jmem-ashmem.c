/*
 * Copyright (C) 2007-2008 The Android Open Source Project
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

#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "jmemsys.h"		

#include <unistd.h>
#include <sys/mman.h>

#ifndef HAVE_STDLIB_H		
extern void * malloc JPP((size_t size));
extern void free JPP((void *ptr));
#endif


GLOBAL(void *)
jpeg_get_small (j_common_ptr cinfo, size_t sizeofobject)
{
  return (void *) malloc(sizeofobject);
}

GLOBAL(void)
jpeg_free_small (j_common_ptr cinfo, void * object, size_t sizeofobject)
{
  free(object);
}



GLOBAL(void FAR *)
jpeg_get_large (j_common_ptr cinfo, size_t sizeofobject)
{
  return (void FAR *) malloc(sizeofobject);
}

GLOBAL(void)
jpeg_free_large (j_common_ptr cinfo, void FAR * object, size_t sizeofobject)
{
  free(object);
}


#ifndef DEFAULT_MAX_MEM		
#define DEFAULT_MAX_MEM		10000000L 
#endif

GLOBAL(long)
jpeg_mem_available (j_common_ptr cinfo, long min_bytes_needed,
		    long max_bytes_needed, long already_allocated)
{
  return cinfo->mem->max_memory_to_use - already_allocated;
}



METHODDEF(void)
read_backing_store (j_common_ptr cinfo, backing_store_ptr info,
		    void FAR * buffer_address,
		    long file_offset, long byte_count)
{
  memmove(buffer_address, info->addr + file_offset, byte_count);
}


METHODDEF(void)
write_backing_store (j_common_ptr cinfo, backing_store_ptr info,
		     void FAR * buffer_address,
		     long file_offset, long byte_count)
{
  memmove(info->addr + file_offset, buffer_address, byte_count);
}


METHODDEF(void)
close_backing_store (j_common_ptr cinfo, backing_store_ptr info)
{
  munmap(info->addr, info->size);
  close(info->temp_file);
}

LOCAL(int)
get_ashmem(backing_store_ptr info, long total_bytes_needed)
{
  char path[1024];
  snprintf(path, 1023, "%d.tmp.ashmem", getpid());
  int fd = ashmem_create_region(path, total_bytes_needed);
  if (fd == -1) {
      return -1;
  }
  int err = ashmem_set_prot_region(fd, PROT_READ | PROT_WRITE);
  if (err) {
      return -1;
  }
  info->addr = mmap(NULL, total_bytes_needed, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
  info->size = total_bytes_needed;
  info->temp_file = fd;
  return fd;
}


GLOBAL(void)
jpeg_open_backing_store (j_common_ptr cinfo, backing_store_ptr info,
			 long total_bytes_needed)
{
  if (get_ashmem(info, total_bytes_needed) == -1)
    ERREXITS(cinfo, JERR_TFILE_CREATE, "");
  info->read_backing_store = read_backing_store;
  info->write_backing_store = write_backing_store;
  info->close_backing_store = close_backing_store;
}


GLOBAL(long)
jpeg_mem_init (j_common_ptr cinfo)
{
  return DEFAULT_MAX_MEM;	
}

GLOBAL(void)
jpeg_mem_term (j_common_ptr cinfo)
{
  
}