// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_EXCEPTION_BARRIER_H_
#define CHROME_FRAME_EXCEPTION_BARRIER_H_

#include <windows.h>

extern "C" IMAGE_DOS_HEADER __ImageBase;

typedef EXCEPTION_DISPOSITION (__cdecl* ExceptionHandlerFunc)(
                                struct _EXCEPTION_RECORD* exception_record,
                                void*  establisher_frame,
                                struct _CONTEXT* context,
                                void*  reserved);

struct EXCEPTION_REGISTRATION {
  EXCEPTION_REGISTRATION* prev;
  ExceptionHandlerFunc  handler;
};

extern "C" EXCEPTION_DISPOSITION __cdecl
ExceptionBarrierHandler(struct _EXCEPTION_RECORD* exception_record,
                        void* establisher_frame,
                        struct _CONTEXT* context,
                        void* reserved);

extern "C" EXCEPTION_DISPOSITION __cdecl
ExceptionBarrierReportOnlyModuleHandler(
    struct _EXCEPTION_RECORD* exception_record,
    void* establisher_frame,
    struct _CONTEXT* context,
    void* reserved);

extern "C" EXCEPTION_DISPOSITION __cdecl
ExceptionBarrierCallCustomHandler(
    struct _EXCEPTION_RECORD* exception_record,
    void* establisher_frame,
    struct _CONTEXT* context,
    void* reserved);


extern "C" void WINAPI RegisterExceptionRecord(
                          EXCEPTION_REGISTRATION* registration,
                          ExceptionHandlerFunc func);
extern "C" void WINAPI UnregisterExceptionRecord(
                          EXCEPTION_REGISTRATION* registration);


class EBTraitsBase {
 public:
  static void UnregisterException(EXCEPTION_REGISTRATION* registration) {
    UnregisterExceptionRecord(registration);
  }
};

class EBReportAllTraits : public EBTraitsBase {
 public:
  static void RegisterException(EXCEPTION_REGISTRATION* registration) {
    RegisterExceptionRecord(registration, ExceptionBarrierHandler);
  }
};

class EBReportOnlyThisModuleTraits : public EBTraitsBase {
 public:
  static void RegisterException(EXCEPTION_REGISTRATION* registration) {
    RegisterExceptionRecord(registration,
                            ExceptionBarrierReportOnlyModuleHandler);
  }
};

class EBCustomHandlerTraits : public EBTraitsBase {
 public:
  static void RegisterException(EXCEPTION_REGISTRATION* registration) {
    RegisterExceptionRecord(registration,
                            ExceptionBarrierCallCustomHandler);
  }
};

class ExceptionBarrierConfig {
 public:
  
  
  static void set_enabled(bool enabled) {
    s_enabled_ = enabled;
  }
  static bool enabled() { return s_enabled_; }

  
  static bool s_enabled_;
};

template <typename RegistrarTraits>
class ExceptionBarrierT {
 public:
  
  ExceptionBarrierT() {
    RegistrarTraits::RegisterException(&registration_);
  }

  
  virtual ~ExceptionBarrierT() {
    RegistrarTraits::UnregisterException(&registration_);
  }

 protected:
  
  EXCEPTION_REGISTRATION registration_;
};

template <typename RegistrarTraits>
class ExceptionBarrierCustomHandlerT :
    public ExceptionBarrierT<typename RegistrarTraits> {
 public:
  
  
  typedef void (CALLBACK* CustomExceptionHandler)(EXCEPTION_POINTERS* ptrs);

  
  
  static void set_custom_handler(CustomExceptionHandler handler) {
    s_custom_handler_ = handler;
  }
  static CustomExceptionHandler custom_handler() { return s_custom_handler_; }

 private:
  static CustomExceptionHandler s_custom_handler_;
};

typedef ExceptionBarrierT<EBReportAllTraits> ExceptionBarrier;
typedef ExceptionBarrierT<EBReportOnlyThisModuleTraits>
    ExceptionBarrierReportOnlyModule;
typedef ExceptionBarrierCustomHandlerT<EBCustomHandlerTraits>
    ExceptionBarrierCustomHandler;

#endif  
