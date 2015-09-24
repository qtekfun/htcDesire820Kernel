// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_MAC_LAUNCHD_H_
#define CHROME_COMMON_MAC_LAUNCHD_H_

#include <CoreFoundation/CoreFoundation.h>

#include "base/basictypes.h"
#include "base/memory/singleton.h"

class Launchd {
 public:
  enum Type {
    Agent,  
    Daemon  
  };

  
  
  enum Domain {
    User = 1,  
    Local = 2,  
    Network = 4,  
    System = 8  
  };

  
  
  
  static Launchd* GetInstance();

  virtual ~Launchd();

  
  virtual CFDictionaryRef CopyExports();

  
  virtual CFDictionaryRef CopyJobDictionary(CFStringRef label);

  
  virtual CFDictionaryRef CopyDictionaryByCheckingIn(CFErrorRef* error);

  
  virtual bool RemoveJob(CFStringRef label, CFErrorRef* error);

  
  
  
  
  
  
  virtual bool RestartJob(Domain domain,
                          Type type,
                          CFStringRef name,
                          CFStringRef session_type);

  
  
  virtual CFMutableDictionaryRef CreatePlistFromFile(Domain domain,
                                                     Type type,
                                                     CFStringRef name);
  
  
  virtual bool WritePlistToFile(Domain domain,
                                Type type,
                                CFStringRef name,
                                CFDictionaryRef dict);

  
  
  virtual bool DeletePlist(Domain domain, Type type, CFStringRef name);

  
  
  static void SetInstance(Launchd* instance);
  class ScopedInstance {
   public:
    explicit ScopedInstance(Launchd* instance) {
      Launchd::SetInstance(instance);
    }
    ~ScopedInstance() {
      Launchd::SetInstance(NULL);
    }
  };

 protected:
  Launchd() { }

 private:
  
  
  friend struct DefaultSingletonTraits<Launchd>;
  static Launchd* g_instance_;

  DISALLOW_COPY_AND_ASSIGN(Launchd);
};

#endif  
