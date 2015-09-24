// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_SIMPLE_PS_INSTANCE_H_
#define PPAPI_SIMPLE_PS_INSTANCE_H_

#include <stdarg.h>

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/c/ppb_var.h"
#include "ppapi/c/ppb_view.h"

#include "ppapi/cpp/fullscreen.h"
#include "ppapi/cpp/graphics_3d_client.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/message_loop.h"
#include "ppapi/cpp/mouse_lock.h"

#include "ppapi/utility/completion_callback_factory.h"

#include "ppapi_simple/ps_event.h"
#include "ppapi_simple/ps_main.h"

#include "sdk_util/thread_safe_queue.h"

typedef void (*MessageHandler_t)(const pp::Var& key,
                                 const pp::Var& value,
                                 void* user_data);

struct MessageHandler {
  MessageHandler_t handler;
  void* user_data;
};

class PSInstance : public pp::Instance, pp::MouseLock, pp::Graphics3DClient {
 public:
  
  
  enum Verbosity {
    PSV_SILENT = 0,
    PSV_ERROR = 1,
    PSV_WARN = 2,
    PSV_LOG = 3,
    PSV_TRACE = 4,
  };

  
  static PSInstance* GetInstance();

  PSInstance(PP_Instance inst);
  virtual ~PSInstance();

  
  
  
  void SetMain(PSMainFunc_t func);

  
  virtual int MainThread(int argc, char* argv[]);

  
  void SetVerbosity(Verbosity verbosity);
  void Trace(const char *fmt, ...);
  void Log(const char *fmt, ...);
  void Warn(const char *fmt, ...);
  void Error(const char *fmt, ...);

  
  void SetEnabledEvents(uint32_t mask);
  void PostEvent(PSEventType type);
  void PostEvent(PSEventType type, PP_Bool bool_value);
  void PostEvent(PSEventType type, PP_Resource resource);
  void PostEvent(PSEventType type, const PP_Var& var);

  PSEvent* TryAcquireEvent();
  PSEvent* WaitAcquireEvent();
  void ReleaseEvent(PSEvent* event);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void RegisterMessageHandler(std::string message_name,
                              MessageHandler_t handler,
                              void* user_data);

 protected:
  typedef std::map<std::string, MessageHandler> MessageHandlerMap;

  
  
  
  
  
  
  
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);

  
  
  void VALog(Verbosity verbosity, const char *fmt, va_list args);

  
  
  virtual void DidChangeView(const pp::View& view);

  
  virtual void DidChangeFocus(bool has_focus);

  
  virtual void HandleMessage(const pp::Var& message);

  
  
  virtual bool HandleInputEvent(const pp::InputEvent& event);

  
  virtual void Graphics3DContextLost();

  
  virtual void MouseLockLost();

  
  
  virtual bool ProcessProperties();

 private:
  static void* MainThreadThunk(void *start_info);
  ssize_t TtyOutputHandler(const char* buf, size_t count);
  void MessageHandlerInput(const pp::Var& message);
  void MessageHandlerResize(const pp::Var& message);
  void HandleResize(int width, int height);

  static ssize_t TtyOutputHandlerStatic(const char* buf, size_t count,
                                        void* user_data);

  
  
  static void MessageHandlerInputStatic(const pp::Var& key,
                                        const pp::Var& value,
                                        void* user_data);


  
  
  
  static void MessageHandlerResizeStatic(const pp::Var& key,
                                         const pp::Var& value,
                                         void* user_data);

 protected:
  pp::MessageLoop* main_loop_;

  sdk_util::ThreadSafeQueue<PSEvent> event_queue_;
  uint32_t events_enabled_;
  Verbosity verbosity_;

  
  int tty_fd_;
  const char* tty_prefix_;
  MessageHandlerMap message_handlers_;

  
  
  char* exit_message_;

  PSMainFunc_t main_cb_;

  const PPB_Core* ppb_core_;
  const PPB_Var* ppb_var_;
  const PPB_View* ppb_view_;
};

#endif  
