/*
 * libjingle
 * Copyright 2004--2006, Google Inc.
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

#ifndef TALK_BASE_TASK_H__
#define TALK_BASE_TASK_H__

#include <string>
#include "talk/base/basictypes.h"
#include "talk/base/scoped_ptr.h"
#include "talk/base/sigslot.h"
#include "talk/base/taskparent.h"


namespace talk_base {

class Task : public TaskParent {
 public:
  Task(TaskParent *parent);
  virtual ~Task();

  int32 unique_id() { return unique_id_; }

  void Start();
  void Step();
  int GetState() const { return state_; }
  bool HasError() const { return (GetState() == STATE_ERROR); }
  bool Blocked() const { return blocked_; }
  bool IsDone() const { return done_; }
  int64 ElapsedTime();

  
  void Abort(bool nowake = false);

  bool TimedOut();

  int64 timeout_time() const { return timeout_time_; }
  int timeout_seconds() const { return timeout_seconds_; }
  void set_timeout_seconds(int timeout_seconds);

  sigslot::signal0<> SignalTimeout;

  
  void Wake();

 protected:

  enum {
    STATE_BLOCKED = -1,
    STATE_INIT = 0,
    STATE_START = 1,
    STATE_DONE = 2,
    STATE_ERROR = 3,
    STATE_RESPONSE = 4,
    STATE_NEXT = 5,  
  };

  
  void Error();

  int64 CurrentTime();

  virtual std::string GetStateName(int state) const;
  virtual int Process(int state);
  virtual void Stop();
  virtual int ProcessStart() = 0;
  virtual int ProcessResponse() { return STATE_DONE; }

  void ResetTimeout();
  void ClearTimeout();

  void SuspendTimeout();
  void ResumeTimeout();

 protected:
  virtual int OnTimeout() {
    
    return STATE_DONE;
  }

 private:
  void Done();

  int state_;
  bool blocked_;
  bool done_;
  bool aborted_;
  bool busy_;
  bool error_;
  int64 start_time_;
  int64 timeout_time_;
  int timeout_seconds_;
  bool timeout_suspended_;
  int32 unique_id_;
  
  static int32 unique_id_seed_;
};

}  

#endif  
