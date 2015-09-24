// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_NET_NETWORK_PORTAL_DETECTOR_H_
#define CHROME_BROWSER_CHROMEOS_NET_NETWORK_PORTAL_DETECTOR_H_

#include "base/basictypes.h"
#include "net/url_request/url_fetcher.h"

namespace chromeos {

class NetworkState;

class NetworkPortalDetector {
 public:
  enum CaptivePortalStatus {
    CAPTIVE_PORTAL_STATUS_UNKNOWN  = 0,
    CAPTIVE_PORTAL_STATUS_OFFLINE  = 1,
    CAPTIVE_PORTAL_STATUS_ONLINE   = 2,
    CAPTIVE_PORTAL_STATUS_PORTAL   = 3,
    CAPTIVE_PORTAL_STATUS_PROXY_AUTH_REQUIRED = 4,
    CAPTIVE_PORTAL_STATUS_COUNT
  };

  struct CaptivePortalState {
    CaptivePortalState()
        : status(CAPTIVE_PORTAL_STATUS_UNKNOWN),
          response_code(net::URLFetcher::RESPONSE_CODE_INVALID) {
    }

    bool operator==(const CaptivePortalState& o) const {
      return status == o.status && response_code == o.response_code;
    }

    CaptivePortalStatus status;
    int response_code;
  };

  class Observer {
   public:
    
    
    
    
    
    
    
    virtual void OnPortalDetectionCompleted(
        const NetworkState* network,
        const CaptivePortalState& state) = 0;

   protected:
    virtual ~Observer() {}
  };

  
  virtual void AddObserver(Observer* observer) = 0;

  
  
  
  
  
  
  
  
  
  
  virtual void AddAndFireObserver(Observer* observer) = 0;

  
  virtual void RemoveObserver(Observer* observer) = 0;

  
  virtual CaptivePortalState GetCaptivePortalState(
      const chromeos::NetworkState* network) = 0;

  
  virtual bool IsEnabled() = 0;

  
  
  
  
  
  virtual void Enable(bool start_detection) = 0;

  
  
  
  virtual bool StartDetectionIfIdle() = 0;

  
  
  virtual void EnableLazyDetection() = 0;

  
  virtual void DisableLazyDetection() = 0;

  
  
  
  static void InitializeForTesting(
      NetworkPortalDetector* network_portal_detector);

  
  static void Initialize();

  
  static void Shutdown();

  
  static NetworkPortalDetector* Get();

 protected:
  NetworkPortalDetector() {}
  virtual ~NetworkPortalDetector() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkPortalDetector);
};

}  

#endif  
