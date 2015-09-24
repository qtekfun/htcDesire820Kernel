// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

void __asan_init() {
  static int inited = 0;
  if (inited) return;
  inited = 1;
#if __WORDSIZE == 64
  unsigned long start = 0x100000000000;
  unsigned long size  = 0x100000000000;
#else
  unsigned long start = 0x20000000;
  unsigned long size = 0x20000000;
#endif
  void *res = mmap((void*)start, size,
                   PROT_READ | PROT_WRITE,
                   MAP_PRIVATE | MAP_ANON | MAP_FIXED | MAP_NORESERVE,
                   0, 0);
  if (res == (void*)start) {
    fprintf(stderr, "Fake AddressSanitizer run-time initialized ok at %p\n",
            res);
  } else {
    fprintf(stderr, "Fake AddressSanitizer run-time failed to initialize.\n"
            "You have been warned. Aborting.");
    abort();
  }
}

void __asan_init_v1() {
  __asan_init();
}

void __asan_init_v3() {
  static int inited = 0;
  if (inited) return;
  inited = 1;
#if __WORDSIZE == 64
  unsigned long start = 0x00007fff8000;
  unsigned long size  = 0x100000000000;
#else
  unsigned long start = 0x20000000;
  unsigned long size = 0x20000000;
#endif
  void *res = mmap((void*)start, size,
                    PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_ANON | MAP_FIXED | MAP_NORESERVE,
                    0, 0);
  if (res == (void*)start) {
    fprintf(stderr, "Fake AddressSanitizer run-time initialized ok at %p\n",
           res);
  } else {
    fprintf(stderr, "Fake AddressSanitizer run-time failed to initialize.\n"
    "You have been warned. Aborting.");
  abort();
  }
}

void __asan_handle_no_return() { }
void __asan_register_globals() { }
void __asan_report_load1() { }
void __asan_report_load16() { }
void __asan_report_load2() { }
void __asan_report_load4() { }
void __asan_report_load8() { }
void __asan_report_load_n() { }
void __asan_report_store1() { }
void __asan_report_store16() { }
void __asan_report_store2() { }
void __asan_report_store4() { }
void __asan_report_store8() { }
void __asan_report_store_n() { }
void __asan_set_error_report_callback() { }
void __asan_unregister_globals() { }
void __sanitizer_sandbox_on_notify() { }
void __asan_before_dynamic_init(const char *module_name) { }
void __asan_after_dynamic_init() { }
int __asan_option_detect_stack_use_after_return;
typedef unsigned long long uptr;
uptr __asan_stack_malloc_1(uptr size, uptr real_stack) {}
uptr __asan_stack_malloc_2(uptr size, uptr real_stack) {}
uptr __asan_stack_malloc_3(uptr size, uptr real_stack) {}
uptr __asan_stack_malloc_4(uptr size, uptr real_stack) {}
uptr __asan_stack_malloc_5(uptr size, uptr real_stack) {}
uptr __asan_stack_malloc_6(uptr size, uptr real_stack) {}
uptr __asan_stack_malloc_7(uptr size, uptr real_stack) {}
uptr __asan_stack_malloc_8(uptr size, uptr real_stack) {}
uptr __asan_stack_malloc_9(uptr size, uptr real_stack) {}
uptr __asan_stack_malloc_10(uptr size, uptr real_stack) {}
void  __asan_stack_free_1(uptr ptr, uptr size, uptr real_stack) {}
void  __asan_stack_free_2(uptr ptr, uptr size, uptr real_stack) {}
void  __asan_stack_free_3(uptr ptr, uptr size, uptr real_stack) {}
void  __asan_stack_free_4(uptr ptr, uptr size, uptr real_stack) {}
void  __asan_stack_free_5(uptr ptr, uptr size, uptr real_stack) {}
void  __asan_stack_free_6(uptr ptr, uptr size, uptr real_stack) {}
void  __asan_stack_free_7(uptr ptr, uptr size, uptr real_stack) {}
void  __asan_stack_free_8(uptr ptr, uptr size, uptr real_stack) {}
void  __asan_stack_free_9(uptr ptr, uptr size, uptr real_stack) {}
void  __asan_stack_free_10(uptr ptr, uptr size, uptr real_stack) {}
