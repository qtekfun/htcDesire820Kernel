/* Copyright (c) 2013 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */



#include <assert.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include "xray/xray_priv.h"

#if defined(XRAY)

#define FORCE_INLINE  __attribute__((always_inline))

#if defined(__amd64__) && !defined(XRAY_NO_RDTSC)
FORCE_INLINE uint64_t RDTSC64() {
  uint64_t a, d;
  __asm__ __volatile__("rdtsc" : "=a" (a), "=d" (d));
  return ((uint64_t)a) | (((uint64_t)d) << 32);
}
#define GTSC(_x) _x = RDTSC64()
#elif defined(__i386__) && !defined(XRAY_NO_RDTSC)
#define GTSC(_x)      __asm__ __volatile__ ("rdtsc" : "=A" (_x));
#else
FORCE_INLINE uint64_t GTOD() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (uint64_t)tv.tv_sec * 1000000 + (uint64_t)tv.tv_usec;
}
#define GTSC(_x) _x = GTOD();
#endif

__thread struct XRayTraceCapture* g_xray_capture = NULL;
__thread int g_xray_thread_id_placeholder = 0;


struct XRayTraceStackEntry {
  uint32_t depth_addr;
  uint64_t tsc;
  uint32_t dest;
  uint32_t annotation_index;
};


struct XRayTraceFrameEntry {
  
  int start;
  int end;
  uint64_t start_tsc;
  uint64_t end_tsc;
  uint64_t total_ticks;
  int annotation_count;
  bool valid;

#ifndef XRAY_DISABLE_BROWSER_INTEGRATION
  struct XRayTimestampPair start_time;
  struct XRayTimestampPair end_time;
#endif
};


struct XRayTraceFrame {
  struct XRayTraceFrameEntry* entry;
  int head;
  int tail;
  int count;
};


struct XRayTraceCapture {
  
  bool recording;
  uint32_t stack_depth;
  uint32_t max_stack_depth;
  int buffer_index;
  int buffer_size;
  int disabled;
  int annotation_count;
  struct XRaySymbolTable* symbols;
  bool initialized;
  uint32_t annotation_filter;
  uint32_t guard0;
  struct XRayTraceStackEntry stack[XRAY_TRACE_STACK_SIZE] XRAY_ALIGN64;
  uint32_t guard1;
  uint32_t guard2;
  char annotation[XRAY_ANNOTATION_STACK_SIZE] XRAY_ALIGN64;
  uint32_t guard3;
  struct XRayTraceBufferEntry* buffer;
  struct XRayTraceFrame frame;

#ifndef XRAY_DISABLE_BROWSER_INTEGRATION
  int32_t thread_id;
#endif
} XRAY_ALIGN64;


#ifdef __cplusplus
extern "C" {
#endif

#if defined(__pnacl__)
XRAY_NO_INSTRUMENT void __pnacl_profile_func_enter(const char* fname);
XRAY_NO_INSTRUMENT void __pnacl_profile_func_exit(const char* fname);
#else
XRAY_NO_INSTRUMENT void __cyg_profile_func_enter(void* this_fn,
                                                 void* call_site);
XRAY_NO_INSTRUMENT void __cyg_profile_func_exit(void* this_fn,
                                                void* call_site);
#endif

XRAY_NO_INSTRUMENT void __xray_profile_append_annotation(
    struct XRayTraceCapture* capture,
    struct XRayTraceStackEntry* se,
    struct XRayTraceBufferEntry* be);

#ifdef __cplusplus
}
#endif

void XRayCheckGuards(struct XRayTraceCapture* capture) {
  assert(capture->guard0 == XRAY_GUARD_VALUE_0x12345678);
  assert(capture->guard1 == XRAY_GUARD_VALUE_0x12345678);
  assert(capture->guard2 == XRAY_GUARD_VALUE_0x87654321);
  assert(capture->guard3 == XRAY_GUARD_VALUE_0x12345678);
}

XRAY_FORCE_INLINE int XRayTraceDecrementIndexInline(
    struct XRayTraceCapture* capture, int index) {
  --index;
  if (index < 0)
    index = capture->buffer_size - 1;
  return index;
}

XRAY_FORCE_INLINE int XRayTraceIncrementIndexInline(
    struct XRayTraceCapture* capture, int index) {
  ++index;
  if (index >= capture->buffer_size)
    index = 0;
  return index;
}

bool XRayTraceIsAnnotation(
    struct XRayTraceCapture* capture, int index) {
  struct XRayTraceBufferEntry* be = &capture->buffer[index];
  char* dst = (char*)be;
  return 0 == *dst;
}

int XRayTraceIncrementIndex(struct XRayTraceCapture* capture, int index) {
  return XRayTraceIncrementIndexInline(capture, index);
}

int XRayTraceDecrementIndex(struct XRayTraceCapture* capture, int index) {
  return XRayTraceDecrementIndexInline(capture, index);
}

int XRayTraceSkipAnnotation(struct XRayTraceCapture* capture, int index) {
  
  
  
  assert(capture);
  assert(XRayTraceIsAnnotation(capture, index));
  bool done = false;
  int start_index = 1;
  int i;
  while (!done) {
    char* str = (char*) &capture->buffer[index];
    const int num = sizeof(capture->buffer[index]);
    for (i = start_index; i < num; ++i) {
      if (0 == str[i]) {
        done = true;
        break;
      }
    }
    index = XRayTraceIncrementIndexInline(capture, index);
    start_index = 0;
  }
  return index;
}


struct XRayTraceBufferEntry* XRayTraceGetEntry(
    struct XRayTraceCapture* capture, int index) {
  return &capture->buffer[index];
}

int XRayTraceNextEntry(struct XRayTraceCapture* capture, int index) {
  if (XRayTraceIsAnnotation(capture, index))
    index = XRayTraceSkipAnnotation(capture, index);
  else
    index = XRayTraceIncrementIndexInline(capture, index);
  return index;
}

int XRayFrameGetTraceStartIndex(struct XRayTraceCapture* capture, int frame) {
  assert(capture);
  assert(capture->initialized);
  assert(!capture->recording);
  return capture->frame.entry[frame].start;
}

int XRayFrameGetTraceEndIndex(struct XRayTraceCapture* capture, int frame) {
  assert(capture);
  assert(capture->initialized);
  assert(!capture->recording);
  return capture->frame.entry[frame].end;
}

int XRayFrameGetTraceCount(
    struct XRayTraceCapture* capture, int frame) {
  assert(true == capture->initialized);
  assert(frame >= 0);
  assert(frame < capture->frame.count);
  assert(!capture->recording);
  int start = capture->frame.entry[frame].start;
  int end = capture->frame.entry[frame].end;
  int num;
  if (start < end)
    num = end - start;
  else
    num = capture->buffer_size - (start - end);
  return num;
}

void XRayTraceAppendString(struct XRayTraceCapture* capture, char* src) {
  int index = capture->buffer_index;
  bool done = false;
  int start_index = 1;
  int s = 0;
  int i;
  char* dst = (char*)&capture->buffer[index];
  const int num = sizeof(capture->buffer[index]);
  dst[0] = 0;
  while (!done) {
    for (i = start_index; i < num; ++i) {
      dst[i] = src[s];
      if (0 == src[s]) {
        dst[i] = 0;
        done = true;
        break;
      }
      ++s;
    }
    index = XRayTraceIncrementIndexInline(capture, index);
    dst = (char*)&capture->buffer[index];
    start_index = 0;
  }
  capture->buffer_index = index;
}

int XRayTraceCopyToString(
    struct XRayTraceCapture* capture, int index, char* dst) {
  assert(XRayTraceIsAnnotation(capture, index));
  bool done = false;
  int i;
  int d = 0;
  int start_index = 1;
  while (!done) {
    char* src = (char*) &capture->buffer[index];
    const int num = sizeof(capture->buffer[index]);
    for (i = start_index; i < num; ++i) {
      dst[d] = src[i];
      if (0 == src[i]) {
        done = true;
        break;
      }
      ++d;
    }
    index = XRayTraceIncrementIndexInline(capture, index);
    start_index = 0;
  }
  return index;
}


void* XRayMalloc(size_t t) {
  void* data;
  data = calloc(1, t);
  if (NULL == data) {
    printf("XRay: malloc(%d) failed, panic shutdown!\n", t);
    exit(-1);
  }
  return data;
}


void XRayFree(void* data) {
  assert(NULL != data);
  free(data);
}


#if defined(__pnacl__)
void __pnacl_profile_func_enter(const char* this_fn) {
#else
void __cyg_profile_func_enter(void* this_fn, void* call_site) {
#endif
  struct XRayTraceCapture* capture = g_xray_capture;
  if (capture && capture->recording) {
    uint32_t depth = capture->stack_depth;
    if (depth < capture->max_stack_depth) {
      struct XRayTraceStackEntry* se = &capture->stack[depth];
      uint32_t addr = (uint32_t)this_fn;
      se->depth_addr = XRAY_PACK_DEPTH_ADDR(depth, addr);
      se->dest = capture->buffer_index;
      se->annotation_index = 0;
      GTSC(se->tsc);
      capture->buffer_index =
        XRayTraceIncrementIndexInline(capture, capture->buffer_index);
    }
    ++capture->stack_depth;
  }
}


#if defined(__pnacl__)
void __pnacl_profile_func_exit(const char* this_fn) {
#else
void __cyg_profile_func_exit(void* this_fn, void* call_site) {
#endif
  struct XRayTraceCapture* capture = g_xray_capture;
  if (capture && capture->recording) {
    --capture->stack_depth;
    if (capture->stack_depth < capture->max_stack_depth) {
      uint32_t depth = capture->stack_depth;
      struct XRayTraceStackEntry* se = &capture->stack[depth];
      uint32_t buffer_index = se->dest;
      uint64_t tsc;
      struct XRayTraceBufferEntry* be = &capture->buffer[buffer_index];
      GTSC(tsc);
      be->depth_addr = se->depth_addr;
      be->start_tick = se->tsc;
      be->end_tick = tsc;
      be->annotation_index = 0;
      if (0 != se->annotation_index)
        __xray_profile_append_annotation(capture, se, be);
    }
  }
}

#ifndef XRAY_DISABLE_BROWSER_INTEGRATION
void XRayGetTSC(uint64_t* tsc) {
  GTSC(*tsc);
}

int32_t XRayGetSavedThreadID(struct XRayTraceCapture* capture) {
  return capture->thread_id;
}

struct XRayTimestampPair XRayFrameGetStartTimestampPair(
    struct XRayTraceCapture* capture, int frame) {
  return capture->frame.entry[frame].start_time;
}

struct XRayTimestampPair XRayFrameGetEndTimestampPair(
    struct XRayTraceCapture* capture, int frame) {
  return capture->frame.entry[frame].end_time;
}
#endif

void __xray_profile_append_annotation(struct XRayTraceCapture* capture,
                                      struct XRayTraceStackEntry* se,
                                      struct XRayTraceBufferEntry* be) {
  struct XRayTraceStackEntry* parent = se - 1;
  int start = parent->annotation_index;
  be->annotation_index = capture->buffer_index;
  char* str = &capture->annotation[start];
  XRayTraceAppendString(capture, str);
  *str = 0;
  ++capture->annotation_count;
}



void __XRayAnnotate(const char* fmt, ...) {
  va_list args;
  struct XRayTraceCapture* capture = g_xray_capture;
  
  if (capture && capture->initialized) {
    if (0 == capture->disabled) {
      if (capture->recording) {
        char buffer[1024];
        int r;
        va_start(args, fmt);
        r = vsnprintf(buffer, sizeof(buffer), fmt, args);
        va_end(args);
        {
          
          int depth = capture->stack_depth - 1;
          struct XRayTraceStackEntry* se = &capture->stack[depth];
          if (0 == se->annotation_index) {
            struct XRayTraceStackEntry* parent = se - 1;
            se->annotation_index = parent->annotation_index;
          }
          char* dst = &capture->annotation[se->annotation_index];
          strcpy(dst, buffer);
          int len = strlen(dst);
          se->annotation_index += len;
        }
      }
    }
  }
}


void __XRayAnnotateFiltered(const uint32_t filter, const char* fmt, ...) {
  va_list args;
  struct XRayTraceCapture* capture = g_xray_capture;
  if (capture && capture->initialized) {
    if (0 != (filter & capture->annotation_filter)) {
      if (0 == capture->disabled) {
        if (capture->recording) {
          char buffer[XRAY_TRACE_ANNOTATION_LENGTH];
          int r;
          va_start(args, fmt);
          r = vsnprintf(buffer, sizeof(buffer), fmt, args);
          va_end(args);
          {
            
            int depth = capture->stack_depth - 1;
            struct XRayTraceStackEntry* se = &capture->stack[depth];
            if (0 == se->annotation_index) {
              struct XRayTraceStackEntry* parent = se - 1;
              se->annotation_index = parent->annotation_index;
            }
            char* dst = &capture->annotation[se->annotation_index];
            strcpy(dst, buffer);
            int len = strlen(dst);
            se->annotation_index += len;
          }
        }
      }
    }
  }
}


void XRaySetAnnotationFilter(struct XRayTraceCapture* capture,
                             uint32_t filter) {
  capture->annotation_filter = filter;
}


void XRayReset(struct XRayTraceCapture* capture) {
  assert(capture);
  assert(capture->initialized);
  assert(!capture->recording);
  capture->buffer_index = 0;
  capture->stack_depth = 0;
  capture->disabled = 0;
  capture->frame.head = 0;
  capture->frame.tail = 0;
  memset(capture->frame.entry, 0,
    sizeof(capture->frame.entry[0]) * capture->frame.count);
  memset(&capture->stack, 0,
    sizeof(capture->stack[0]) * XRAY_TRACE_STACK_SIZE);
  XRayCheckGuards(capture);
}


void XRaySetMaxStackDepth(struct XRayTraceCapture* capture, int stack_depth) {
  assert(capture);
  assert(capture->initialized);
  assert(!capture->recording);
  if (stack_depth < 1)
    stack_depth = 1;
  if (stack_depth >= XRAY_TRACE_STACK_SIZE)
    stack_depth = (XRAY_TRACE_STACK_SIZE - 1);
  capture->max_stack_depth = stack_depth;
}


int XRayFrameGetCount(struct XRayTraceCapture* capture) {
  return capture->frame.count;
}

int XRayFrameGetTail(struct XRayTraceCapture* capture) {
  return capture->frame.tail;
}

int XRayFrameGetHead(struct XRayTraceCapture* capture) {
  return capture->frame.head;
}

int XRayFrameGetPrev(struct XRayTraceCapture* capture, int i) {
  i = i - 1;
  if (i < 0)
    i = capture->frame.count - 1;
  return i;
}

int XRayFrameGetNext(struct XRayTraceCapture* capture, int i) {
  i = i + 1;
  if (i >= capture->frame.count)
    i = 0;
  return i;
}

bool XRayFrameIsValid(struct XRayTraceCapture* capture, int i) {
  return capture->frame.entry[i].valid;
}

int XRayFrameGetTotalTicks(struct XRayTraceCapture* capture, int i) {
  return capture->frame.entry[i].total_ticks;
}

int XRayFrameGetAnnotationCount(struct XRayTraceCapture* capture, int i) {
  return capture->frame.entry[i].annotation_count;
}

void XRayFrameMakeLabel(struct XRayTraceCapture* capture,
                        int counter,
                        char* label) {
  snprintf(label, XRAY_MAX_LABEL, "@@@frame%d@@@", counter);
}


int XRayFrameFindTail(struct XRayTraceCapture* capture) {
  int head = capture->frame.head;
  int index = XRayFrameGetPrev(capture, head);
  int total_capture = 0;
  int last_valid_frame = index;
  
  if (capture->frame.head == capture->frame.tail)
    return capture->frame.head;
  
  while (index != head) {
    bool valid = capture->frame.entry[index].valid;
    if (valid) {
      total_capture += XRayFrameGetTraceCount(capture, index) + 1;
      if (total_capture < capture->buffer_size) {
        last_valid_frame = index;
        capture->frame.entry[index].valid = true;
      } else {
        capture->frame.entry[index].valid = false;
      }
    }
    index = XRayFrameGetPrev(capture, index);
  }
  return last_valid_frame;
}


void XRayStartFrame(struct XRayTraceCapture* capture) {
  int i;
  assert(NULL == g_xray_capture);
  assert(capture->initialized);
  assert(!capture->recording);
  i = capture->frame.head;
  XRayCheckGuards(capture);
  
  struct XRayTraceBufferEntry* be = &capture->buffer[capture->buffer_index];
  be->depth_addr = XRAY_FRAME_MARKER;
  capture->buffer_index =
      XRayTraceIncrementIndex(capture, capture->buffer_index);
  
  capture->frame.entry[i].start = capture->buffer_index;
  capture->disabled = 0;
  capture->stack_depth = 1;
  
  memset(&capture->stack[0], 0, sizeof(capture->stack[0]));
  
  capture->stack[0].annotation_index = 1;
  capture->annotation[0] = 0;
  capture->annotation[1] = 0;
  capture->annotation_count = 0;
  capture->recording = true;
  GTSC(capture->frame.entry[i].start_tsc);
  g_xray_capture = capture;

#ifndef XRAY_DISABLE_BROWSER_INTEGRATION
  capture->frame.entry[i].start_time = XRayGenerateTimestampsNow();
#endif

}


void XRayEndFrame(struct XRayTraceCapture* capture) {
  int i;
  assert(capture);
  assert(capture->initialized);
  assert(capture->recording);
  assert(g_xray_capture == capture);
  assert(0 == capture->disabled);
  assert(1 == capture->stack_depth);
  i = capture->frame.head;
  GTSC(capture->frame.entry[i].end_tsc);
  capture->frame.entry[i].total_ticks =
    capture->frame.entry[i].end_tsc - capture->frame.entry[i].start_tsc;
  capture->recording = NULL;
  capture->frame.entry[i].end = capture->buffer_index;
  capture->frame.entry[i].valid = true;
  capture->frame.entry[i].annotation_count = capture->annotation_count;
  capture->frame.head = XRayFrameGetNext(capture, capture->frame.head);
  
  if (capture->frame.head == capture->frame.tail)
    capture->frame.tail = XRayFrameGetNext(capture, capture->frame.tail);
  capture->frame.tail = XRayFrameFindTail(capture);
  
  int marker = XRayTraceDecrementIndex(capture, capture->frame.entry[i].start);
  struct XRayTraceBufferEntry* be = &capture->buffer[marker];
  if (be->depth_addr != XRAY_FRAME_MARKER) {
    fprintf(stderr,
      "XRay: XRayStopFrame() detects insufficient trace buffer size!\n");
    XRayReset(capture);
  } else {
    
    be->depth_addr = XRAY_NULL_ANNOTATION;
    XRayCheckGuards(capture);
  }
  g_xray_capture = NULL;

#ifndef XRAY_DISABLE_BROWSER_INTEGRATION
  capture->frame.entry[i].end_time = XRayGenerateTimestampsNow();
#endif
}


int XRayGetLastFrame(struct XRayTraceCapture* capture) {
  assert(capture);
  assert(capture->initialized);
  assert(!capture->recording);
  assert(0 == capture->disabled);
  assert(1 == capture->stack_depth);
  int last_frame = XRayFrameGetPrev(capture, capture->frame.head);
  return last_frame;
}


void XRayDisableCapture(struct XRayTraceCapture* capture) {
  assert(capture);
  assert(capture == g_xray_capture);
  assert(capture->initialized);
  ++capture->disabled;
  capture->recording = false;
}


void XRayEnableCapture(struct XRayTraceCapture* capture) {
  assert(capture);
  assert(capture == g_xray_capture);
  assert(capture->initialized);
  assert(0 < capture->disabled);
  --capture->disabled;
  if (0 == capture->disabled) {
    capture->recording = true;
  }
}



struct XRaySymbolTable* XRayGetSymbolTable(struct XRayTraceCapture* capture) {
  return capture->symbols;
}


struct XRayTraceCapture* XRayInit(int stack_depth,
                                  int buffer_size,
                                  int frame_count,
                                  const char* mapfilename) {
  struct XRayTraceCapture* capture;
  capture = (struct XRayTraceCapture*)XRayMalloc(
      sizeof(struct XRayTraceCapture));
  int adj_frame_count = frame_count + 1;
  size_t buffer_size_in_bytes =
      sizeof(capture->buffer[0]) * buffer_size;
  size_t frame_size_in_bytes =
      sizeof(capture->frame.entry[0]) * adj_frame_count;
  capture->buffer =
      (struct XRayTraceBufferEntry *)XRayMalloc(buffer_size_in_bytes);
  capture->frame.entry =
      (struct XRayTraceFrameEntry *)XRayMalloc(frame_size_in_bytes);
  capture->buffer_size = buffer_size;
  capture->frame.count = adj_frame_count;
  capture->frame.head = 0;
  capture->frame.tail = 0;
  capture->disabled = 0;
  capture->annotation_filter = 0xFFFFFFFF;
  capture->guard0 = XRAY_GUARD_VALUE_0x12345678;
  capture->guard1 = XRAY_GUARD_VALUE_0x12345678;
  capture->guard2 = XRAY_GUARD_VALUE_0x87654321;
  capture->guard3 = XRAY_GUARD_VALUE_0x12345678;
  capture->initialized = true;
  capture->recording = false;
  XRaySetMaxStackDepth(capture, stack_depth);
  XRayReset(capture);

  
  capture->symbols =
      XRaySymbolTableCreate(XRAY_DEFAULT_SYMBOL_TABLE_SIZE);
  if (NULL != mapfilename)
    XRaySymbolTableParseMapfile(capture->symbols, mapfilename);

#ifndef XRAY_DISABLE_BROWSER_INTEGRATION
  
  capture->thread_id = (int32_t)(&g_xray_thread_id_placeholder);
#endif

  return capture;
}


void XRayShutdown(struct XRayTraceCapture* capture) {
  assert(capture);
  assert(capture->initialized);
  assert(!capture->recording);
  XRayCheckGuards(capture);
  if (NULL != capture->symbols) {
    XRaySymbolTableFree(capture->symbols);
  }
  XRayFree(capture->frame.entry);
  XRayFree(capture->buffer);
  capture->initialized = false;
  XRayFree(capture);
}

#endif  
