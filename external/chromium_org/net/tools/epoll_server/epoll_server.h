// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_EPOLL_SERVER_EPOLL_SERVER_H_
#define NET_TOOLS_EPOLL_SERVER_EPOLL_SERVER_H_

#include <fcntl.h>
#include <sys/queue.h>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#ifdef EPOLL_SERVER_EVENT_TRACING
#include <ostream>
#include "base/logging.h"
#endif

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/memory/scoped_ptr.h"
#include <sys/epoll.h>

namespace net {

class EpollServer;
class EpollAlarmCallbackInterface;
class ReadPipeCallback;

struct EpollEvent {
  EpollEvent(int events, bool is_epoll_wait)
      : in_events(events),
        out_ready_mask(0) {
  }

  int in_events;            
  int out_ready_mask;       
                            
                            
                            
};

class EpollCallbackInterface {
 public:
  
  
  
  
  
  
  
  
  virtual void OnRegistration(EpollServer* eps, int fd, int event_mask) = 0;

  
  
  
  
  
  
  
  virtual void OnModification(int fd, int event_mask) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void OnEvent(int fd, EpollEvent* event) = 0;

  
  
  
  
  
  
  
  virtual void OnUnregistration(int fd, bool replaced) = 0;

  
  
  
  
  
  
  virtual void OnShutdown(EpollServer* eps, int fd) = 0;

  virtual ~EpollCallbackInterface() {}

 protected:
  EpollCallbackInterface() {}
};


class EpollServer {
 public:
  typedef EpollAlarmCallbackInterface AlarmCB;
  typedef EpollCallbackInterface CB;

  typedef std::multimap<int64, AlarmCB*> TimeToAlarmCBMap;
  typedef TimeToAlarmCBMap::iterator AlarmRegToken;

  
  
  
  
  
  EpollServer();

  

  
  virtual ~EpollServer();

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void RegisterFD(int fd, CB* cb, int event_mask);

  

  
  
  
  
  
  
  virtual void RegisterFDForWrite(int fd, CB* cb);

  

  
  
  
  
  
  
  virtual void RegisterFDForReadWrite(int fd, CB* cb);

  

  
  
  
  
  
  
  virtual void RegisterFDForRead(int fd, CB* cb);

  

  
  
  
  
  
  
  
  
  virtual void UnregisterFD(int fd);

  

  
  
  
  
  
  
  
  
  virtual void ModifyCallback(int fd, int event_mask);

  

  
  
  
  
  
  
  
  virtual void StopRead(int fd);

  

  
  
  
  
  
  
  
  virtual void StartRead(int fd);

  

  
  
  
  
  
  
  
  virtual void StopWrite(int fd);

  

  
  
  
  
  
  
  
  virtual void StartWrite(int fd);

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void HandleEvent(int fd, int event_mask);

  
  
  
  
  
  
  
  virtual void WaitForEventsAndExecuteCallbacks();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void SetFDReady(int fd, int events_to_fake);

  virtual void SetFDNotReady(int fd);

  
  
  
  
  
  
  
  virtual bool IsFDReady(int fd) const;

  size_t ReadyListSize() const { return ready_list_size_; }

  void VerifyReadyList() const;

  

  
  
  
  
  
  
  
  
  
  
  
  
  virtual void RegisterAlarm(int64 timeout_time_in_us, AlarmCB* ac);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void RegisterAlarmApproximateDelta(int64 delta_in_us, AlarmCB* ac) {
    RegisterAlarm(ApproximateNowInUsec() + delta_in_us, ac);
  }

  

  
  
  
  
  
  
  virtual void UnregisterAlarm(
      const EpollServer::AlarmRegToken& iterator_token);

  

  
  
  
  
  virtual int NumFDsRegistered() const;

  
  
  virtual void Wake();

  
  
  
  
  
  
  virtual int64 NowInUsec() const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual int64 ApproximateNowInUsec() const;

  static std::string EventMaskToString(int event_mask);

  
  
  void LogStateOnCrash();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void set_timeout_in_us(int64 timeout_in_us) {
    timeout_in_us_ = timeout_in_us;
  }

  

  
  
  int timeout_in_us() const { return timeout_in_us_; }

  
  
  bool in_shutdown() const { return in_shutdown_; }

  bool ContainsAlarm(EpollAlarmCallbackInterface* alarm) const {
    return all_alarms_.find(alarm) != all_alarms_.end();
  }

  
  
  
  
  
  void CallReadyListCallbacks();

 protected:
  virtual int GetFlags(int fd);
  inline int SetFlags(int fd, int flags) {
    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  }

  virtual void SetNonblocking(int fd);

  
  
  virtual int epoll_wait_impl(int epfd,
                              struct epoll_event* events,
                              int max_events,
                              int timeout_in_ms);

  
  
  
  
  
  
  
  
  struct CBAndEventMask {
    CBAndEventMask()
        : cb(NULL),
          fd(-1),
          event_mask(0),
          events_asserted(0),
          events_to_fake(0),
          in_use(false) {
      entry.le_next = NULL;
      entry.le_prev = NULL;
    }

    CBAndEventMask(EpollCallbackInterface* cb,
                   int event_mask,
                   int fd)
        : cb(cb), fd(fd), event_mask(event_mask), events_asserted(0),
          events_to_fake(0), in_use(false) {
      entry.le_next = NULL;
      entry.le_prev = NULL;
    }

    
    
    
    bool operator==(const CBAndEventMask& cb_and_mask) const {
      return fd == cb_and_mask.fd;
    }
    
    
    mutable EpollCallbackInterface* cb;

    mutable LIST_ENTRY(CBAndEventMask) entry;
    
    int fd;
    
    mutable int event_mask;
    
    mutable int events_asserted;
    
    mutable int events_to_fake;
    
    
    mutable bool in_use;
  };

  
  struct CBAndEventMaskHash {
    size_t operator()(const CBAndEventMask& cb_and_eventmask) const {
      return static_cast<size_t>(cb_and_eventmask.fd);
    }
  };

  typedef base::hash_set<CBAndEventMask, CBAndEventMaskHash> FDToCBMap;

  
  
  

  
  
  
  
  
  
  
  
  virtual void DelFD(int fd) const;

  

  
  
  
  
  
  
  
  
  
  
  virtual void AddFD(int fd, int event_mask) const;

  

  
  
  
  
  
  
  
  
  
  
  virtual void ModFD(int fd, int event_mask) const;

  

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual void ModifyFD(int fd, int remove_event, int add_event);

  

  
  
  
  
  
  
  
  virtual void WaitForEventsAndCallHandleEvents(int64 timeout_in_us,
                                                struct epoll_event events[],
                                                int events_size);



  
  
  
  
  void AddToReadyList(CBAndEventMask* cb_and_mask);

  
  
  
  
  void RemoveFromReadyList(const CBAndEventMask& cb_and_mask);

  
  
  
  virtual void CallAndReregisterAlarmEvents();

  
  int epoll_fd_;

  
  FDToCBMap cb_map_;

  
  struct AlarmCBHash {
    size_t operator()(AlarmCB*const& p) const {
      return reinterpret_cast<size_t>(p);
    }
  };


  
  
  
  
  typedef base::hash_set<AlarmCB*, AlarmCBHash> AlarmCBMap;
  AlarmCBMap all_alarms_;

  TimeToAlarmCBMap alarm_map_;

  
  
  
  
  
  int64 timeout_in_us_;

  
  
  
  
  
  int64 recorded_now_in_us_;

  
  
  
  
  
  
  
  AlarmCBMap alarms_reregistered_and_should_be_skipped_;

  LIST_HEAD(ReadyList, CBAndEventMask) ready_list_;
  LIST_HEAD(TmpList, CBAndEventMask) tmp_list_;
  int ready_list_size_;
  
  static const int events_size_ = 256;
  struct epoll_event events_[256];

#ifdef EPOLL_SERVER_EVENT_TRACING
  struct EventRecorder {
   public:
    EventRecorder() : num_records_(0), record_threshold_(10000) {}

    ~EventRecorder() {
      Clear();
    }

    
    // the collected data summary for all FDs will be written
    
    
    
    
    
    
    
    
    void set_record_threshold(int64 new_threshold) {
      record_threshold_ = new_threshold;
    }

    void Clear() {
      for (int i = 0; i < debug_events_.size(); ++i) {
        delete debug_events_[i];
      }
      debug_events_.clear();
      unregistered_fds_.clear();
      event_counts_.clear();
    }

    void MaybeRecordAndClear() {
      ++num_records_;
      if ((num_records_ > record_threshold_) &&
          (record_threshold_ > 0)) {
        LOG(INFO) << "\n" << *this;
        num_records_ = 0;
        Clear();
      }
    }

    void RecordFDMaskEvent(int fd, int mask, const char* function) {
      FDMaskOutput* fdmo = new FDMaskOutput(fd, mask, function);
      debug_events_.push_back(fdmo);
      MaybeRecordAndClear();
    }

    void RecordEpollWaitEvent(int timeout_in_ms,
                              int num_events_generated) {
      EpollWaitOutput* ewo = new EpollWaitOutput(timeout_in_ms,
                                                  num_events_generated);
      debug_events_.push_back(ewo);
      MaybeRecordAndClear();
    }

    void RecordEpollEvent(int fd, int event_mask) {
      Events& events_for_fd = event_counts_[fd];
      events_for_fd.AssignFromMask(event_mask);
      MaybeRecordAndClear();
    }

    friend ostream& operator<<(ostream& os, const EventRecorder& er) {
      for (int i = 0; i < er.unregistered_fds_.size(); ++i) {
        os << "fd: " << er.unregistered_fds_[i] << "\n";
        os << er.unregistered_fds_[i];
      }
      for (EventCountsMap::const_iterator i = er.event_counts_.begin();
           i != er.event_counts_.end();
           ++i) {
        os << "fd: " << i->first << "\n";
        os << i->second;
      }
      for (int i = 0; i < er.debug_events_.size(); ++i) {
        os << *(er.debug_events_[i]) << "\n";
      }
      return os;
    }

    void RecordUnregistration(int fd) {
      EventCountsMap::iterator i = event_counts_.find(fd);
      if (i != event_counts_.end()) {
        unregistered_fds_.push_back(i->second);
        event_counts_.erase(i);
      }
      MaybeRecordAndClear();
    }

   protected:
    class DebugOutput {
     public:
      friend ostream& operator<<(ostream& os, const DebugOutput& debug_output) {
        debug_output.OutputToStream(os);
        return os;
      }
      virtual void OutputToStream(ostream* os) const = 0;
      virtual ~DebugOutput() {}
    };

    class FDMaskOutput : public DebugOutput {
     public:
      FDMaskOutput(int fd, int mask, const char* function) :
          fd_(fd), mask_(mask), function_(function) {}
      virtual void OutputToStream(ostream* os) const {
        (*os) << "func: " << function_
              << "\tfd: " << fd_;
        if (mask_ != 0) {
           (*os) << "\tmask: " << EventMaskToString(mask_);
        }
      }
      int fd_;
      int mask_;
      const char* function_;
    };

    class EpollWaitOutput : public DebugOutput {
     public:
      EpollWaitOutput(int timeout_in_ms,
                      int num_events_generated) :
          timeout_in_ms_(timeout_in_ms),
          num_events_generated_(num_events_generated) {}
      virtual void OutputToStream(ostream* os) const {
        (*os) << "timeout_in_ms: " << timeout_in_ms_
              << "\tnum_events_generated: " << num_events_generated_;
      }
     protected:
      int timeout_in_ms_;
      int num_events_generated_;
    };

    struct Events {
      Events() :
          epoll_in(0),
          epoll_pri(0),
          epoll_out(0),
          epoll_rdnorm(0),
          epoll_rdband(0),
          epoll_wrnorm(0),
          epoll_wrband(0),
          epoll_msg(0),
          epoll_err(0),
          epoll_hup(0),
          epoll_oneshot(0),
          epoll_et(0) {}

      void AssignFromMask(int event_mask) {
        if (event_mask & EPOLLIN) ++epoll_in;
        if (event_mask & EPOLLPRI) ++epoll_pri;
        if (event_mask & EPOLLOUT) ++epoll_out;
        if (event_mask & EPOLLRDNORM) ++epoll_rdnorm;
        if (event_mask & EPOLLRDBAND) ++epoll_rdband;
        if (event_mask & EPOLLWRNORM) ++epoll_wrnorm;
        if (event_mask & EPOLLWRBAND) ++epoll_wrband;
        if (event_mask & EPOLLMSG) ++epoll_msg;
        if (event_mask & EPOLLERR) ++epoll_err;
        if (event_mask & EPOLLHUP) ++epoll_hup;
        if (event_mask & EPOLLONESHOT) ++epoll_oneshot;
        if (event_mask & EPOLLET) ++epoll_et;
      };

      friend ostream& operator<<(ostream& os, const Events& ev) {
        if (ev.epoll_in) {
          os << "\t      EPOLLIN: " << ev.epoll_in << "\n";
        }
        if (ev.epoll_pri) {
          os << "\t     EPOLLPRI: " << ev.epoll_pri << "\n";
        }
        if (ev.epoll_out) {
          os << "\t     EPOLLOUT: " << ev.epoll_out << "\n";
        }
        if (ev.epoll_rdnorm) {
          os << "\t  EPOLLRDNORM: " << ev.epoll_rdnorm << "\n";
        }
        if (ev.epoll_rdband) {
          os << "\t  EPOLLRDBAND: " << ev.epoll_rdband << "\n";
        }
        if (ev.epoll_wrnorm) {
          os << "\t  EPOLLWRNORM: " << ev.epoll_wrnorm << "\n";
        }
        if (ev.epoll_wrband) {
          os << "\t  EPOLLWRBAND: " << ev.epoll_wrband << "\n";
        }
        if (ev.epoll_msg) {
          os << "\t     EPOLLMSG: " << ev.epoll_msg << "\n";
        }
        if (ev.epoll_err) {
          os << "\t     EPOLLERR: " << ev.epoll_err << "\n";
        }
        if (ev.epoll_hup) {
          os << "\t     EPOLLHUP: " << ev.epoll_hup << "\n";
        }
        if (ev.epoll_oneshot) {
          os << "\t EPOLLONESHOT: " << ev.epoll_oneshot << "\n";
        }
        if (ev.epoll_et) {
          os << "\t      EPOLLET: " << ev.epoll_et << "\n";
        }
        return os;
      }

      unsigned int epoll_in;
      unsigned int epoll_pri;
      unsigned int epoll_out;
      unsigned int epoll_rdnorm;
      unsigned int epoll_rdband;
      unsigned int epoll_wrnorm;
      unsigned int epoll_wrband;
      unsigned int epoll_msg;
      unsigned int epoll_err;
      unsigned int epoll_hup;
      unsigned int epoll_oneshot;
      unsigned int epoll_et;
    };

    std::vector<DebugOutput*> debug_events_;
    std::vector<Events> unregistered_fds_;
    typedef base::hash_map<int, Events> EventCountsMap;
    EventCountsMap event_counts_;
    int64 num_records_;
    int64 record_threshold_;
  };

  void ClearEventRecords() {
    event_recorder_.Clear();
  }
  void WriteEventRecords(ostream* os) const {
    (*os) << event_recorder_;
  }

  mutable EventRecorder event_recorder_;

#endif

 private:
  
  void CleanupFDToCBMap();
  void CleanupTimeToAlarmCBMap();

  
  
  
  scoped_ptr<ReadPipeCallback> wake_cb_;

  
  
  int read_fd_;
  int write_fd_;

  
  
  
  
  
  bool in_wait_for_events_and_execute_callbacks_;

  
  bool in_shutdown_;

  DISALLOW_COPY_AND_ASSIGN(EpollServer);
};

class EpollAlarmCallbackInterface {
 public:
  
  
  
  
  
  
  
  virtual int64 OnAlarm() = 0;

  
  
  
  
  
  
  
  virtual void OnRegistration(const EpollServer::AlarmRegToken& token,
                              EpollServer* eps) = 0;

  
  
  
  
  virtual void OnUnregistration() = 0;

  
  
  
  
  virtual void OnShutdown(EpollServer* eps) = 0;

  virtual ~EpollAlarmCallbackInterface() {}

 protected:
  EpollAlarmCallbackInterface() {}
};

class EpollAlarm : public EpollAlarmCallbackInterface {
 public:
  EpollAlarm();

  virtual ~EpollAlarm();

  
  
  virtual int64 OnAlarm() OVERRIDE;

  
  virtual void OnRegistration(const EpollServer::AlarmRegToken& token,
                              EpollServer* eps) OVERRIDE;

  
  virtual void OnUnregistration() OVERRIDE;

  
  virtual void OnShutdown(EpollServer* eps) OVERRIDE;

  
  void UnregisterIfRegistered();

  bool registered() const { return registered_; }

  const EpollServer* eps() const { return eps_; }

 private:
  EpollServer::AlarmRegToken token_;
  EpollServer* eps_;
  bool registered_;
};

}  

#endif  
