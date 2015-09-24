// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef JINGLE_GLUE_THREAD_WRAPPER_H_
#define JINGLE_GLUE_THREAD_WRAPPER_H_

#include <list>
#include <map>

#include "base/compiler_specific.h"
#include "base/message_loop/message_loop.h"
#include "base/synchronization/lock.h"
#include "base/synchronization/waitable_event.h"
#include "third_party/libjingle/source/talk/base/thread.h"

namespace jingle_glue {

class JingleThreadWrapper : public base::MessageLoop::DestructionObserver,
                            public talk_base::Thread {
 public:
  
  
  
  static void EnsureForCurrentMessageLoop();

  
  
  
  static JingleThreadWrapper* current();

  explicit JingleThreadWrapper(
     scoped_refptr<base::SingleThreadTaskRunner> task_runner);
  virtual ~JingleThreadWrapper();

  
  
  
  
  
  void set_send_allowed(bool allowed) { send_allowed_ = allowed; }

  
  virtual void WillDestroyCurrentMessageLoop() OVERRIDE;

  
  virtual void Post(talk_base::MessageHandler *phandler,
                    uint32 id,
                    talk_base::MessageData *pdata,
                    bool time_sensitive) OVERRIDE;
  virtual void PostDelayed(int delay_ms,
                           talk_base::MessageHandler* handler,
                           uint32 id,
                           talk_base::MessageData* data) OVERRIDE;
  virtual void Clear(talk_base::MessageHandler* handler,
                     uint32 id,
                     talk_base::MessageList* removed) OVERRIDE;
  virtual void Send(talk_base::MessageHandler *handler,
                    uint32 id,
                    talk_base::MessageData *data) OVERRIDE;

  
  
  
  
  virtual void Quit() OVERRIDE;
  virtual bool IsQuitting() OVERRIDE;
  virtual void Restart() OVERRIDE;
  virtual bool Get(talk_base::Message* message,
                   int delay_ms,
                   bool process_io) OVERRIDE;
  virtual bool Peek(talk_base::Message* message,
                    int delay_ms) OVERRIDE;
  virtual void PostAt(uint32 timestamp,
                      talk_base::MessageHandler* handler,
                      uint32 id,
                      talk_base::MessageData* data) OVERRIDE;
  virtual void Dispatch(talk_base::Message* message) OVERRIDE;
  virtual void ReceiveSends() OVERRIDE;
  virtual int GetDelay() OVERRIDE;

  
  virtual void Stop() OVERRIDE;
  virtual void Run() OVERRIDE;

 private:
  typedef std::map<int, talk_base::Message> MessagesQueue;
  struct PendingSend;

  void PostTaskInternal(
      int delay_ms, talk_base::MessageHandler* handler,
      uint32 message_id, talk_base::MessageData* data);
  void RunTask(int task_id);
  void ProcessPendingSends();

  
  scoped_refptr<base::SingleThreadTaskRunner> task_runner_;

  bool send_allowed_;

  
  base::Lock lock_;
  int last_task_id_;
  MessagesQueue messages_;
  std::list<PendingSend*> pending_send_messages_;
  base::WaitableEvent pending_send_event_;

  base::WeakPtr<JingleThreadWrapper> weak_ptr_;
  base::WeakPtrFactory<JingleThreadWrapper> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(JingleThreadWrapper);
};

}  

#endif  
