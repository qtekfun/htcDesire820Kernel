// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_MOBILE_MOBILE_ACTIVATOR_H_
#define CHROME_BROWSER_CHROMEOS_MOBILE_MOBILE_ACTIVATOR_H_

#include <map>
#include <string>

#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "base/timer/timer.h"
#include "chromeos/network/network_handler_callbacks.h"
#include "chromeos/network/network_state_handler_observer.h"
#include "content/public/browser/browser_thread.h"

namespace base {
class DictionaryValue;
}

namespace chromeos {

class NetworkState;
class TestMobileActivator;

class CellularConfigDocument
    : public base::RefCountedThreadSafe<CellularConfigDocument> {
 public:
  CellularConfigDocument();

  
  std::string GetErrorMessage(const std::string& code);
  void LoadCellularConfigFile();
  const std::string& version() { return version_; }

 private:
  friend class base::RefCountedThreadSafe<CellularConfigDocument>;
  typedef std::map<std::string, std::string> ErrorMap;

  virtual ~CellularConfigDocument();

  void SetErrorMap(const ErrorMap& map);
  bool LoadFromFile(const base::FilePath& config_path);

  std::string version_;
  ErrorMap error_map_;
  base::Lock config_lock_;

  DISALLOW_COPY_AND_ASSIGN(CellularConfigDocument);
};

class MobileActivator
    : public base::SupportsWeakPtr<MobileActivator>,
      public NetworkStateHandlerObserver {
 public:
  
  enum PlanActivationState {
    
    PLAN_ACTIVATION_PAGE_LOADING            = -1,
    
    PLAN_ACTIVATION_START                   = 0,
    
    PLAN_ACTIVATION_TRYING_OTASP            = 1,
    
    PLAN_ACTIVATION_INITIATING_ACTIVATION   = 3,
    
    
    PLAN_ACTIVATION_RECONNECTING            = 4,
    
    
    PLAN_ACTIVATION_WAITING_FOR_CONNECTION  = 5,
    
    PLAN_ACTIVATION_PAYMENT_PORTAL_LOADING  = 6,
    
    PLAN_ACTIVATION_SHOWING_PAYMENT         = 7,
    
    PLAN_ACTIVATION_RECONNECTING_PAYMENT    = 8,
    
    PLAN_ACTIVATION_DELAY_OTASP             = 9,
    
    PLAN_ACTIVATION_START_OTASP             = 10,
    
    PLAN_ACTIVATION_OTASP                   = 11,
    
    PLAN_ACTIVATION_DONE                    = 12,
    
    PLAN_ACTIVATION_ERROR                   = 0xFF,
  };

  
  class Observer {
   public:
    
    virtual void OnActivationStateChanged(
        const NetworkState* network,
        PlanActivationState state,
        const std::string& error_description) = 0;

   protected:
    Observer() {}
    virtual ~Observer() {}
  };

  static MobileActivator* GetInstance();

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  
  bool RunningActivation() const;
  
  PlanActivationState state() const { return state_; }
  
  void InitiateActivation(const std::string& service_path);
  
  void TerminateActivation();
  
  void OnPortalLoaded(bool success);
  
  void OnSetTransactionStatus(bool success);

 private:
  friend struct DefaultSingletonTraits<MobileActivator>;
  friend class TestMobileActivator;
  FRIEND_TEST_ALL_PREFIXES(MobileActivatorTest, BasicFlowForNewDevices);
  FRIEND_TEST_ALL_PREFIXES(MobileActivatorTest, OTASPScheduling);
  FRIEND_TEST_ALL_PREFIXES(MobileActivatorTest,
                           ReconnectOnDisconnectFromPaymentPortal);
  FRIEND_TEST_ALL_PREFIXES(MobileActivatorTest, StartAtStart);
  
  friend class MobileActivatorTest;

  MobileActivator();
  virtual ~MobileActivator();

  
  virtual void DefaultNetworkChanged(const NetworkState* network) OVERRIDE;
  virtual void NetworkPropertiesUpdated(const NetworkState* network) OVERRIDE;

  
  
  void ContinueActivation();
  void GetPropertiesAndContinueActivation(
      const std::string& service_path,
      const base::DictionaryValue& properties);
  void GetPropertiesFailure(const std::string& error_name,
                            scoped_ptr<base::DictionaryValue> error_data);
  
  void HandlePortalLoaded(bool success);
  
  void HandleSetTransactionStatus(bool success);
  
  void StartActivation();
  
  void RetryOTASP();
  
  
  void ContinueConnecting();

  
  void SendDeviceInfo();

  
  void StartOTASP();
  
  void HandleOTASPTimeout();
  
  void ForceReconnect(const NetworkState* network,
                      PlanActivationState next_state);
  
  void ReconnectTimedOut();

  
  
  void RefreshCellularNetworks();

  
  
  virtual const NetworkState* GetNetworkState(const std::string& service_path);

  
  virtual void EvaluateCellularNetwork(const NetworkState* network);
  
  
  PlanActivationState PickNextState(const NetworkState* network,
                                    std::string* error_description) const;
  
  
  PlanActivationState PickNextOnlineState(const NetworkState* network) const;
  PlanActivationState PickNextOfflineState(const NetworkState* network) const;
  
  bool GotActivationError(const NetworkState* network,
                          std::string* error) const;
  
  void UpdatePage(const NetworkState* network,
                  const std::string& error_description);

  
  void HandleActivationFailure(
      const std::string& service_path,
      PlanActivationState new_state,
      const std::string& error_name,
      scoped_ptr<base::DictionaryValue> error_data);

  
  
  
  virtual void RequestCellularActivation(
      const NetworkState* network,
      const base::Closure& success_callback,
      const network_handler::ErrorCallback& error_callback);

  
  virtual void ChangeState(const NetworkState* network,
                           PlanActivationState new_state,
                           std::string error_description);
  
  void CompleteActivation();
  
  
  
  void DisableCertRevocationChecking();
  
  void ReEnableCertRevocationChecking();
  
  std::string GetErrorMessage(const std::string& code) const;

  
  
  
  static bool EvaluateCellularDeviceState(bool* report_status,
                                          std::string* state,
                                          std::string* error);
  
  
  virtual void StartOTASPTimer();

  
  virtual void SignalCellularPlanPayment();

  
  virtual bool HasRecentCellularPlanPayment() const;

  static const char* GetStateDescription(PlanActivationState state);

  scoped_refptr<CellularConfigDocument> cellular_config_;
  
  PlanActivationState state_;
  
  std::string meid_;
  
  std::string iccid_;
  
  
  std::string service_path_;
  
  
  
  std::string device_path_;
  
  
  bool reenable_cert_check_;
  
  bool terminated_;
  
  
  bool pending_activation_request_;
  
  int connection_retry_count_;
  
  int initial_OTASP_attempts_;
  int trying_OTASP_attempts_;
  int final_OTASP_attempts_;
  
  int payment_reconnect_count_;
  
  base::RepeatingTimer<MobileActivator> state_duration_timer_;

  
  PlanActivationState post_reconnect_state_;
  
  base::RepeatingTimer<MobileActivator> continue_reconnect_timer_;
  
  base::OneShotTimer<MobileActivator> reconnect_timeout_timer_;
  
  base::Time cellular_plan_payment_time_;

  ObserverList<Observer> observers_;
  base::WeakPtrFactory<MobileActivator> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(MobileActivator);
};

}  

#endif  
