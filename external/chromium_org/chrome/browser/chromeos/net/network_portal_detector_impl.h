// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NET_NETWORK_PORTAL_DETECTOR_IMPL_H_
#define CHROME_BROWSER_CHROMEOS_NET_NETWORK_PORTAL_DETECTOR_IMPL_H_

#include <string>

#include "base/basictypes.h"
#include "base/cancelable_callback.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "base/time/time.h"
#include "chrome/browser/captive_portal/captive_portal_detector.h"
#include "chrome/browser/chromeos/net/network_portal_detector.h"
#include "chromeos/network/network_state_handler_observer.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "net/url_request/url_fetcher.h"
#include "url/gurl.h"

namespace net {
class URLRequestContextGetter;
}

namespace chromeos {

class NetworkState;

class NetworkPortalDetectorImpl
    : public NetworkPortalDetector,
      public base::NonThreadSafe,
      public chromeos::NetworkStateHandlerObserver,
      public content::NotificationObserver {
 public:
  explicit NetworkPortalDetectorImpl(
      const scoped_refptr<net::URLRequestContextGetter>& request_context);
  virtual ~NetworkPortalDetectorImpl();

  
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void AddAndFireObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual CaptivePortalState GetCaptivePortalState(
      const chromeos::NetworkState* network) OVERRIDE;
  virtual bool IsEnabled() OVERRIDE;
  virtual void Enable(bool start_detection) OVERRIDE;
  virtual bool StartDetectionIfIdle() OVERRIDE;
  virtual void EnableLazyDetection() OVERRIDE;
  virtual void DisableLazyDetection() OVERRIDE;

  
  virtual void DefaultNetworkChanged(const NetworkState* network) OVERRIDE;

 private:
  friend class NetworkPortalDetectorImplTest;

  typedef std::string NetworkId;
  typedef base::hash_map<NetworkId, CaptivePortalState> CaptivePortalStateMap;

  enum State {
    
    STATE_IDLE = 0,
    
    STATE_PORTAL_CHECK_PENDING,
    
    STATE_CHECKING_FOR_PORTAL,
  };

  
  static const int kBaseRequestTimeoutSec = 5;

  
  static const int kLazyRequestTimeoutSec = 15;

  
  
  bool CanPerformDetection() const;

  
  
  void DetectCaptivePortal(const base::TimeDelta& delay);

  void DetectCaptivePortalTask();

  
  
  
  void PortalDetectionTimeout();

  void CancelPortalDetection();

  
  void OnPortalDetectionCompleted(
      const captive_portal::CaptivePortalDetector::Results& results);

  
  
  void TryLazyDetection();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  bool IsPortalCheckPending() const;

  
  bool IsCheckingForPortal() const;

  
  void SetCaptivePortalState(const NetworkState* network,
                             const CaptivePortalState& results);

  
  void NotifyPortalDetectionCompleted(const NetworkState* network,
                                      const CaptivePortalState& state);

  
  base::TimeTicks GetCurrentTimeTicks() const;

  State state() const { return state_; }

  bool lazy_detection_enabled() const { return lazy_detection_enabled_; }

  
  
  int attempt_count_for_testing() const { return attempt_count_; }

  
  
  void set_attempt_count_for_testing(int attempt_count) {
    attempt_count_ = attempt_count;
  }

  
  
  void set_min_time_between_attempts_for_testing(const base::TimeDelta& delta) {
    min_time_between_attempts_ = delta;
  }

  
  
  void set_lazy_check_interval_for_testing(const base::TimeDelta& delta) {
    lazy_check_interval_ = delta;
  }

  
  void set_request_timeout_for_testing(const base::TimeDelta& timeout) {
    request_timeout_for_testing_ = timeout;
    request_timeout_for_testing_initialized_ = true;
  }

  
  const base::TimeDelta& next_attempt_delay_for_testing() const {
    return next_attempt_delay_;
  }

  
  void set_time_ticks_for_testing(const base::TimeTicks& time_ticks) {
    time_ticks_for_testing_ = time_ticks;
  }

  
  void advance_time_ticks_for_testing(const base::TimeDelta& delta) {
    time_ticks_for_testing_ += delta;
  }

  
  
  bool DetectionTimeoutIsCancelledForTesting() const;

  
  
  
  
  
  int GetRequestTimeoutSec() const;

  
  std::string default_network_name_;

  
  std::string default_network_id_;

  
  std::string default_service_path_;

  
  std::string default_connection_state_;

  State state_;
  CaptivePortalStateMap portal_state_map_;
  ObserverList<Observer> observers_;

  base::CancelableClosure detection_task_;
  base::CancelableClosure detection_timeout_;

  
  GURL test_url_;

  
  scoped_ptr<captive_portal::CaptivePortalDetector> captive_portal_detector_;

  
  bool enabled_;

  base::WeakPtrFactory<NetworkPortalDetectorImpl> weak_ptr_factory_;

  
  int attempt_count_;

  bool lazy_detection_enabled_;

  
  
  base::TimeDelta lazy_check_interval_;

  
  
  base::TimeDelta min_time_between_attempts_;

  
  base::TimeTicks detection_start_time_;

  
  base::TimeTicks attempt_start_time_;

  
  base::TimeDelta next_attempt_delay_;

  
  base::TimeTicks time_ticks_for_testing_;

  
  base::TimeDelta request_timeout_for_testing_;

  
  bool request_timeout_for_testing_initialized_;

  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(NetworkPortalDetectorImpl);
};

}  

#endif  
