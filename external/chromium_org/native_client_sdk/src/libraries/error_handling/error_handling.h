/*
 * Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef ERROR_HANDLING_ERROR_HANDLING_H_
#define ERROR_HANDLING_ERROR_HANDLING_H_

#include "error_handling/string_stream.h"
#include "sdk_util/macros.h"

EXTERN_C_BEGIN

struct NaClExceptionContext;

typedef void (*EHRawHandler)(struct NaClExceptionContext* context);
typedef void (*EHJsonHandler)(const char* str);

typedef struct {
  uint32_t prog_ctr;
  uint32_t frame_ptr;
  uint32_t next_ptr;
} EHFrame;


void EHRequestExceptionsRaw(EHRawHandler callback);
void EHRequestExceptionsJson(EHJsonHandler callback);


void *EHRequestExceptionStackOnThread(size_t stack_size);


int EHHanderInstalled();


int EHGetTopFrame(sstream_t* ss, struct NaClExceptionContext* context,
                  EHFrame* frame);


int EHUnwindFrame(EHFrame* frame);

EXTERN_C_END

#endif  

