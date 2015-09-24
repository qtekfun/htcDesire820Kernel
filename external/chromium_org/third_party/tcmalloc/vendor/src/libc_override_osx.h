// Copyright (c) 2011, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_LIBC_OVERRIDE_OSX_INL_H_
#define TCMALLOC_LIBC_OVERRIDE_OSX_INL_H_

#include <config.h>
#ifdef HAVE_FEATURES_H
#include <features.h>
#endif
#include <gperftools/tcmalloc.h>

#if !defined(__APPLE__)
# error libc_override_glibc-osx.h is for OS X distributions only.
#endif

#include <AvailabilityMacros.h>
#include <malloc/malloc.h>

#if defined(MAC_OS_X_VERSION_10_6) && \
    MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
extern "C" {
  
  
  
  
  extern malloc_zone_t *malloc_default_purgeable_zone(void)
      WEAK_IMPORT_ATTRIBUTE;
}
#endif

namespace {
size_t mz_size(malloc_zone_t* zone, const void* ptr) {
  if (MallocExtension::instance()->GetOwnership(ptr) != MallocExtension::kOwned)
    return 0;  

  
  return MallocExtension::instance()->GetAllocatedSize(const_cast<void*>(ptr));
}

void* mz_malloc(malloc_zone_t* zone, size_t size) {
  return tc_malloc(size);
}

void* mz_calloc(malloc_zone_t* zone, size_t num_items, size_t size) {
  return tc_calloc(num_items, size);
}

void* mz_valloc(malloc_zone_t* zone, size_t size) {
  return tc_valloc(size);
}

void mz_free(malloc_zone_t* zone, void* ptr) {
  return tc_free(ptr);
}

void* mz_realloc(malloc_zone_t* zone, void* ptr, size_t size) {
  return tc_realloc(ptr, size);
}

void* mz_memalign(malloc_zone_t* zone, size_t align, size_t size) {
  return tc_memalign(align, size);
}

void mz_destroy(malloc_zone_t* zone) {
  
}

kern_return_t mi_enumerator(task_t task, void *,
                            unsigned type_mask, vm_address_t zone_address,
                            memory_reader_t reader,
                            vm_range_recorder_t recorder) {
  
  return KERN_FAILURE;
}

size_t mi_good_size(malloc_zone_t *zone, size_t size) {
  
  return size;
}

boolean_t mi_check(malloc_zone_t *zone) {
  return MallocExtension::instance()->VerifyAllMemory();
}

void mi_print(malloc_zone_t *zone, boolean_t verbose) {
  int bufsize = 8192;
  if (verbose)
    bufsize = 102400;   
  char* buffer = new char[bufsize];
  MallocExtension::instance()->GetStats(buffer, bufsize);
  fprintf(stdout, "%s", buffer);
  delete[] buffer;
}

void mi_log(malloc_zone_t *zone, void *address) {
  
}

void mi_force_lock(malloc_zone_t *zone) {
  
}

void mi_force_unlock(malloc_zone_t *zone) {
  
}

void mi_statistics(malloc_zone_t *zone, malloc_statistics_t *stats) {
  
  stats->blocks_in_use = 0;
  stats->size_in_use = 0;
  stats->max_size_in_use = 0;
  stats->size_allocated = 0;
}

boolean_t mi_zone_locked(malloc_zone_t *zone) {
  return false;  
}

}  

extern "C" {
  void  cfree(void* p)                   { tc_cfree(p);               }
  void* pvalloc(size_t s)                { return tc_pvalloc(s);      }
  void malloc_stats(void)                { tc_malloc_stats();         }
  int mallopt(int cmd, int v)            { return tc_mallopt(cmd, v); }
  
  
  size_t malloc_usable_size(void* p)     { return tc_malloc_size(p); }
}  

static void ReplaceSystemAlloc() {
  static malloc_introspection_t tcmalloc_introspection;
  memset(&tcmalloc_introspection, 0, sizeof(tcmalloc_introspection));

  tcmalloc_introspection.enumerator = &mi_enumerator;
  tcmalloc_introspection.good_size = &mi_good_size;
  tcmalloc_introspection.check = &mi_check;
  tcmalloc_introspection.print = &mi_print;
  tcmalloc_introspection.log = &mi_log;
  tcmalloc_introspection.force_lock = &mi_force_lock;
  tcmalloc_introspection.force_unlock = &mi_force_unlock;

  static malloc_zone_t tcmalloc_zone;
  memset(&tcmalloc_zone, 0, sizeof(malloc_zone_t));

  
  tcmalloc_zone.version = 4;
  tcmalloc_zone.zone_name = "tcmalloc";
  tcmalloc_zone.size = &mz_size;
  tcmalloc_zone.malloc = &mz_malloc;
  tcmalloc_zone.calloc = &mz_calloc;
  tcmalloc_zone.valloc = &mz_valloc;
  tcmalloc_zone.free = &mz_free;
  tcmalloc_zone.realloc = &mz_realloc;
  tcmalloc_zone.destroy = &mz_destroy;
  tcmalloc_zone.batch_malloc = NULL;
  tcmalloc_zone.batch_free = NULL;
  tcmalloc_zone.introspect = &tcmalloc_introspection;

  
#if defined(MAC_OS_X_VERSION_10_6) && \
    MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6
  
  tcmalloc_zone.version = 6;
  tcmalloc_zone.free_definite_size = NULL;
  tcmalloc_zone.memalign = &mz_memalign;
  tcmalloc_introspection.zone_locked = &mi_zone_locked;

  
  
  
  
  
  
  
  
  
  if (malloc_default_purgeable_zone) {
    malloc_default_purgeable_zone();
  }
#endif

  
  
  malloc_zone_register(&tcmalloc_zone);

  
  
  
  
  
  
  malloc_zone_t *default_zone = malloc_default_zone();
  malloc_zone_unregister(default_zone);
  malloc_zone_register(default_zone);
}

#endif  
