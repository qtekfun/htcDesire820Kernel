// Copyright 2011 Google Inc. All Rights Reserved.
// Use of this source code is governed by a BSD-style license

#ifndef WEBP_UTILS_THREAD_H_
#define WEBP_UTILS_THREAD_H_

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

#if WEBP_USE_THREAD

#if defined(_WIN32)

#include <windows.h>
typedef HANDLE pthread_t;
typedef CRITICAL_SECTION pthread_mutex_t;
typedef struct {
  HANDLE waiting_sem_;
  HANDLE received_sem_;
  HANDLE signal_event_;
} pthread_cond_t;

#else

#include <pthread.h>

#endif    
#endif    

typedef enum {
  NOT_OK = 0,   
  OK,           
  WORK          
} WebPWorkerStatus;

typedef int (*WebPWorkerHook)(void*, void*);

typedef struct {
#if WEBP_USE_THREAD
  pthread_mutex_t mutex_;
  pthread_cond_t  condition_;
  pthread_t       thread_;
#endif
  WebPWorkerStatus status_;
  WebPWorkerHook hook;    
  void* data1;            
  void* data2;            
  int had_error;          
} WebPWorker;

void WebPWorkerInit(WebPWorker* const worker);
int WebPWorkerReset(WebPWorker* const worker);
int WebPWorkerSync(WebPWorker* const worker);
void WebPWorkerLaunch(WebPWorker* const worker);
void WebPWorkerEnd(WebPWorker* const worker);


#if defined(__cplusplus) || defined(c_plusplus)
}    
#endif

#endif  
