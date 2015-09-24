// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SYSTEM_CPU_CPU_INFO_PROVIDER_H_
#define CHROME_BROWSER_EXTENSIONS_API_SYSTEM_CPU_CPU_INFO_PROVIDER_H_

#include "base/lazy_instance.h"
#include "chrome/browser/extensions/api/system_info/system_info_provider.h"
#include "chrome/common/extensions/api/system_cpu.h"

namespace extensions {

class CpuInfoProvider : public SystemInfoProvider {
 public:
  
  static CpuInfoProvider* Get();

  const api::system_cpu::CpuInfo& cpu_info() const;

  static void InitializeForTesting(scoped_refptr<CpuInfoProvider> provider);

 private:
  friend class MockCpuInfoProviderImpl;

  CpuInfoProvider();
  virtual ~CpuInfoProvider();

  
  virtual bool QueryInfo() OVERRIDE;

  
  
  
  
  
  
  
  api::system_cpu::CpuInfo info_;

  static base::LazyInstance<scoped_refptr<CpuInfoProvider> > provider_;

  DISALLOW_COPY_AND_ASSIGN(CpuInfoProvider);
};

}  

#endif  
