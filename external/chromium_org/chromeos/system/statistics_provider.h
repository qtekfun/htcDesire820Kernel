// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_SYSTEM_STATISTICS_PROVIDER_H_
#define CHROMEOS_SYSTEM_STATISTICS_PROVIDER_H_

#include <string>

#include "base/memory/ref_counted.h"
#include "chromeos/chromeos_export.h"

namespace base {
class TaskRunner;
}

namespace chromeos {
namespace system {

CHROMEOS_EXPORT extern const char kDevSwitchBootMode[];

CHROMEOS_EXPORT extern const char kHardwareClassKey[];

CHROMEOS_EXPORT extern const char kOemCanExitEnterpriseEnrollmentKey[];

CHROMEOS_EXPORT extern const char kOemDeviceRequisitionKey[];

CHROMEOS_EXPORT extern const char kOemIsEnterpriseManagedKey[];

CHROMEOS_EXPORT extern const char kOemKeyboardDrivenOobeKey[];

CHROMEOS_EXPORT extern const char kOffersCouponCodeKey[];

CHROMEOS_EXPORT extern const char kOffersGroupCodeKey[];

class CHROMEOS_EXPORT StatisticsProvider {
 public:
  
  
  virtual void StartLoadingMachineStatistics(
      const scoped_refptr<base::TaskRunner>& file_task_runner,
      bool load_oem_manifest) = 0;

  
  
  
  
  
  virtual bool GetMachineStatistic(const std::string& name,
                                   std::string* result) = 0;

  
  virtual bool GetMachineFlag(const std::string& name, bool* result) = 0;

  
  virtual void Shutdown() = 0;

  
  static StatisticsProvider* GetInstance();

  
  static void SetTestProvider(StatisticsProvider* test_provider);

 protected:
  virtual ~StatisticsProvider() {}
};

}  
}  

#endif  
