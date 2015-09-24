/*
 * libjingle
 * Copyright 2004--2009, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TALK_BASE_SIGNALTHREAD_H_
#define TALK_BASE_SIGNALTHREAD_H_

#include <string>

#include "talk/base/thread.h"
#include "talk/base/sigslot.h"

namespace talk_base {


class SignalThread : public sigslot::has_slots<>, protected MessageHandler {
 public:
  SignalThread();

  
  bool SetName(const std::string& name, const void* obj);

  
  bool SetPriority(ThreadPriority priority);

  
  void Start();

  
  
  
  
  
  void Destroy(bool wait);

  
  
  
  void Release();

  
  sigslot::signal1<SignalThread *> SignalWorkDone;

  enum { ST_MSG_WORKER_DONE, ST_MSG_FIRST_AVAILABLE };

 protected:
  virtual ~SignalThread();

  Thread* worker() { return &worker_; }

  
  virtual void OnWorkStart() { }

  
  virtual void DoWork() = 0;

  
  
  bool ContinueWork();

  
  
  virtual void OnWorkStop() { }

  
  virtual void OnWorkDone() { }

  
  
  virtual void OnMessage(Message *msg);

 private:
  enum State {
    kInit,            
    kRunning,         
    kReleasing,       
    kComplete,        
    kStopping,        
  };

  friend class Worker;
  class Worker : public Thread {
   public:
    SignalThread* parent_;
    virtual void Run() { parent_->Run(); }
  };

  friend class EnterExit;
  class EnterExit {
   public:
    explicit EnterExit(SignalThread* t) : t_(t) {
      t_->cs_.Enter();
      t_->refcount_ += 1;
    }
    ~EnterExit() {
      bool d = (0 == (--(t_->refcount_)));
      t_->cs_.Leave();
      if (d)
        delete t_;
    }
   private:
    SignalThread* t_;
  };

  void Run();
  void OnMainThreadDestroyed();

  Thread* main_;
  Worker worker_;
  CriticalSection cs_;
  State state_;
  int refcount_;
};


}  

#endif  
