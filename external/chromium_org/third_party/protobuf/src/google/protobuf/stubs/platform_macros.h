// Copyright 2012 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef GOOGLE_PROTOBUF_PLATFORM_MACROS_H_
#define GOOGLE_PROTOBUF_PLATFORM_MACROS_H_

#include <google/protobuf/stubs/common.h>

#if defined(_M_X64) || defined(__x86_64__)
#define GOOGLE_PROTOBUF_ARCH_X64 1
#define GOOGLE_PROTOBUF_ARCH_64_BIT 1
#elif defined(_M_IX86) || defined(__i386__)
#define GOOGLE_PROTOBUF_ARCH_IA32 1
#define GOOGLE_PROTOBUF_ARCH_32_BIT 1
#elif defined(__QNX__)
#define GOOGLE_PROTOBUF_ARCH_ARM_QNX 1
#define GOOGLE_PROTOBUF_ARCH_32_BIT 1
#elif defined(__ARMEL__)
#define GOOGLE_PROTOBUF_ARCH_ARM 1
#define GOOGLE_PROTOBUF_ARCH_32_BIT 1
#elif defined(__MIPSEL__)
#define GOOGLE_PROTOBUF_ARCH_MIPS 1
#define GOOGLE_PROTOBUF_ARCH_32_BIT 1
#elif defined(__pnacl__)
#define GOOGLE_PROTOBUF_ARCH_32_BIT 1
#elif defined(__ppc__)
#define GOOGLE_PROTOBUF_ARCH_PPC 1
#define GOOGLE_PROTOBUF_ARCH_32_BIT 1
#else
#error Host architecture was not detected as supported by protobuf
#endif

#if defined(__APPLE__)
#define GOOGLE_PROTOBUF_OS_APPLE
#elif defined(__native_client__)
#define GOOGLE_PROTOBUF_OS_NACL
#endif

#endif  
