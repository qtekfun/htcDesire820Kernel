// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#include "./dsp.h"

#if defined(__ANDROID__)
#include <cpu-features.h>
#endif

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif


#if (defined(__pic__) || defined(__PIC__)) && defined(__i386__)
static WEBP_INLINE void GetCPUInfo(int cpu_info[4], int info_type) {
  __asm__ volatile (
    "mov %%ebx, %%edi\n"
    "cpuid\n"
    "xchg %%edi, %%ebx\n"
    : "=a"(cpu_info[0]), "=D"(cpu_info[1]), "=c"(cpu_info[2]), "=d"(cpu_info[3])
    : "a"(info_type));
}
#elif defined(__i386__) || defined(__x86_64__)
static WEBP_INLINE void GetCPUInfo(int cpu_info[4], int info_type) {
  __asm__ volatile (
    "cpuid\n"
    : "=a"(cpu_info[0]), "=b"(cpu_info[1]), "=c"(cpu_info[2]), "=d"(cpu_info[3])
    : "a"(info_type));
}
#elif defined(WEBP_MSC_SSE2)
#define GetCPUInfo __cpuid
#endif

#if defined(__i386__) || defined(__x86_64__) || defined(WEBP_MSC_SSE2)
static int x86CPUInfo(CPUFeature feature) {
  int cpu_info[4];
  GetCPUInfo(cpu_info, 1);
  if (feature == kSSE2) {
    return 0 != (cpu_info[3] & 0x04000000);
  }
  if (feature == kSSE3) {
    return 0 != (cpu_info[2] & 0x00000001);
  }
  return 0;
}
VP8CPUInfo VP8GetCPUInfo = x86CPUInfo;
#elif defined(WEBP_ANDROID_NEON)
static int AndroidCPUInfo(CPUFeature feature) {
  const AndroidCpuFamily cpu_family = android_getCpuFamily();
  const uint64_t cpu_features = android_getCpuFeatures();
  if (feature == kNEON) {
    return (cpu_family == ANDROID_CPU_FAMILY_ARM &&
            0 != (cpu_features & ANDROID_CPU_ARM_FEATURE_NEON));
  }
  return 0;
}
VP8CPUInfo VP8GetCPUInfo = AndroidCPUInfo;
#elif defined(__ARM_NEON__)
static int armCPUInfo(CPUFeature feature) {
  (void)feature;
  return 1;
}
VP8CPUInfo VP8GetCPUInfo = armCPUInfo;
#else
VP8CPUInfo VP8GetCPUInfo = NULL;
#endif

#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif
