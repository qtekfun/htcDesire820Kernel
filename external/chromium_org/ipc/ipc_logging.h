// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef IPC_IPC_LOGGING_H_
#define IPC_IPC_LOGGING_H_

#include "ipc/ipc_message.h"  

#ifdef IPC_MESSAGE_LOG_ENABLED

#include <vector>

#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/message_loop/message_loop.h"
#include "ipc/ipc_export.h"

typedef void (*LogFunction)(std::string* name,
                            const IPC::Message* msg,
                            std::string* params);

typedef base::hash_map<uint32, LogFunction > LogFunctionMap;

namespace IPC {

class Message;
class Sender;

class IPC_EXPORT Logging {
 public:
  
  class Consumer {
   public:
    virtual void Log(const LogData& data) = 0;

   protected:
    virtual ~Consumer() {}
  };

  void SetConsumer(Consumer* consumer);

  ~Logging();
  static Logging* GetInstance();

  
  
  
  
  void Enable();
  void Disable();
  bool Enabled() const { return enabled_; }

  
  
  void SetIPCSender(Sender* sender);

  
  
  void OnReceivedLoggingMessage(const Message& message);

  void OnSendMessage(Message* message, const std::string& channel_id);
  void OnPreDispatchMessage(const Message& message);
  void OnPostDispatchMessage(const Message& message,
                             const std::string& channel_id);

  
  
  
  static void GetMessageText(uint32 type, std::string* name,
                             const Message* message, std::string* params);

  static void set_log_function_map(LogFunctionMap* functions) {
    log_function_map_ = functions;
  }

  static LogFunctionMap* log_function_map() {
    return log_function_map_;
  }

 private:
  typedef enum {
    ANSI_COLOR_RESET = -1,
    ANSI_COLOR_BLACK,
    ANSI_COLOR_RED,
    ANSI_COLOR_GREEN,
    ANSI_COLOR_YELLOW,
    ANSI_COLOR_BLUE,
    ANSI_COLOR_MAGENTA,
    ANSI_COLOR_CYAN,
    ANSI_COLOR_WHITE
  } ANSIColor;
  const char* ANSIEscape(ANSIColor color);
  ANSIColor DelayColor(double delay);

  friend struct DefaultSingletonTraits<Logging>;
  Logging();

  void OnSendLogs();
  void Log(const LogData& data);

  bool enabled_;
  bool enabled_on_stderr_;  
  bool enabled_color_; 

  std::vector<LogData> queued_logs_;
  bool queue_invoke_later_pending_;

  Sender* sender_;
  base::MessageLoop* main_thread_;

  Consumer* consumer_;

  static LogFunctionMap* log_function_map_;
};

}  

#endif 

#endif  
