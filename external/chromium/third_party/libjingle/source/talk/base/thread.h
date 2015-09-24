/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
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

#ifndef TALK_BASE_THREAD_H_
#define TALK_BASE_THREAD_H_

#include <algorithm>
#include <list>
#include <string>
#include <vector>

#ifdef POSIX
#include <pthread.h>
#endif

#include "talk/base/messagequeue.h"

#ifdef WIN32
#include "talk/base/win32.h"
#endif

namespace talk_base {

class Thread;

class ThreadManager {
public:
  ThreadManager();
  ~ThreadManager();

  static Thread *CurrentThread();
  static void SetCurrent(Thread *thread);
  void Add(Thread *thread);
  void Remove(Thread *thread);

  
  
  
  
  
  
  
  
  
  
  
  
  
  static Thread *WrapCurrentThread();
  static void UnwrapCurrentThread();

  static void StopAllThreads_();  

private:
  Thread *main_thread_;
  std::vector<Thread *> threads_;
  CriticalSection crit_;

#ifdef POSIX
  static pthread_key_t key_;
#endif

#ifdef WIN32
  static DWORD key_;
#endif
};

class Thread;

struct _SendMessage {
  _SendMessage() {}
  Thread *thread;
  Message msg;
  bool *ready;
};

enum ThreadPriority {
  PRIORITY_HIGH,
  PRIORITY_ABOVE_NORMAL,
  PRIORITY_NORMAL,
  PRIORITY_IDLE,
};

class Runnable {
 public:
  virtual ~Runnable() {}
  virtual void Run(Thread* thread) = 0;
};

class Thread : public MessageQueue {
public:
  Thread(SocketServer* ss = NULL);
  virtual ~Thread();

  static inline Thread* Current() {
    return ThreadManager::CurrentThread();
  }

  bool IsCurrent() const {
    return (ThreadManager::CurrentThread() == this);
  }

  
  
  
  static bool SleepMs(int millis);

  
  
  const std::string& name() const { return name_; }
  bool SetName(const std::string& name, const void* obj);

  
  ThreadPriority priority() const { return priority_; }
  bool SetPriority(ThreadPriority priority);

  
  bool started() const { return started_; }
  bool Start(Runnable* runnable = NULL);

  
  
  
  
  virtual void Stop();

  
  
  
  virtual void Run();

  virtual void Send(MessageHandler *phandler, uint32 id = 0,
      MessageData *pdata = NULL);

  
  virtual void Clear(MessageHandler *phandler, uint32 id = MQID_ANY,
                     MessageList* removed = NULL);
  virtual void ReceiveSends();

  
  
  
  bool ProcessMessages(int cms);

  
  
  
  
  
  
  bool IsOwned();

#ifdef WIN32
  HANDLE GetHandle() {
    return thread_;
  }
#elif POSIX
  pthread_t GetPThread() {
    return thread_;
  }
#endif

private:
  static void *PreRun(void *pv);
  
  void Join();

  std::list<_SendMessage> sendlist_;
  std::string name_;
  ThreadPriority priority_;
  bool started_;
  bool has_sends_;

#ifdef POSIX
  pthread_t thread_;
#endif

#ifdef WIN32
  HANDLE thread_;
#endif

  bool owned_;

  friend class ThreadManager;
};


class AutoThread : public Thread {
public:
  AutoThread(SocketServer* ss = 0);
  virtual ~AutoThread();
};

#ifdef WIN32
class ComThread : public Thread {
 protected:
  virtual void Run();
};
#endif

class SocketServerScope {
 public:
  explicit SocketServerScope(SocketServer* ss) {
    old_ss_ = Thread::Current()->socketserver();
    Thread::Current()->set_socketserver(ss);
  }
  ~SocketServerScope() {
    Thread::Current()->set_socketserver(old_ss_);
  }
 private:
  SocketServer* old_ss_;
};

}  

#endif  
