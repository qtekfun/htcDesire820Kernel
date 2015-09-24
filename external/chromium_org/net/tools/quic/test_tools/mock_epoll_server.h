// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_QUIC_TEST_TOOLS_MOCK_EPOLL_SERVER_H_
#define NET_TOOLS_QUIC_TEST_TOOLS_MOCK_EPOLL_SERVER_H_

#include "base/logging.h"
#include "net/tools/epoll_server/epoll_server.h"
#include "testing/gmock/include/gmock/gmock.h"

namespace net {
namespace tools {
namespace test {

class FakeTimeEpollServer : public EpollServer {
 public:
  FakeTimeEpollServer();
  virtual ~FakeTimeEpollServer();

  
  virtual int64 NowInUsec() const OVERRIDE;

  void set_now_in_usec(int64 nius) { now_in_usec_ = nius; }

  
  void AdvanceBy(int64 advancement_usec) {
    set_now_in_usec(NowInUsec() + advancement_usec);
  }

  
  
  
  
  
  void AdvanceByAndCallCallbacks(int64 advancement_usec) {
    AdvanceBy(advancement_usec);
    WaitForEventsAndExecuteCallbacks();
  }

 private:
  int64 now_in_usec_;
};

class MockEpollServer : public FakeTimeEpollServer {
 public:  
  typedef base::hash_multimap<int64, struct epoll_event> EventQueue;

  MockEpollServer();
  virtual ~MockEpollServer();

  
  
  
  
  
  void AddEvent(int64 time_in_usec, const struct epoll_event& ee) {
    event_queue_.insert(std::make_pair(time_in_usec, ee));
  }

  
  
  
  
  void AdvanceByExactly(int64 advancement_usec) {
    until_in_usec_ = NowInUsec() + advancement_usec;
    set_now_in_usec(NowInUsec() + advancement_usec);
  }

  
  void AdvanceByExactlyAndCallCallbacks(int64 advancement_usec) {
    AdvanceByExactly(advancement_usec);
    WaitForEventsAndExecuteCallbacks();
  }

  base::hash_set<AlarmCB*>::size_type NumberOfAlarms() const {
    return all_alarms_.size();
  }

 protected:  
  
  
  virtual void DelFD(int fd) const OVERRIDE { }
  virtual void AddFD(int fd, int event_mask) const OVERRIDE { }
  virtual void ModFD(int fd, int event_mask) const OVERRIDE { }

  
  virtual int epoll_wait_impl(int epfd,
                              struct epoll_event* events,
                              int max_events,
                              int timeout_in_ms) OVERRIDE;
  virtual void SetNonblocking (int fd) OVERRIDE { }

 private:  
  EventQueue event_queue_;
  int64 until_in_usec_;
};

}  
}  
}  

#endif  
