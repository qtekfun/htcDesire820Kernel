// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_PUBLIC_BROWSER_PEPPER_FLASH_SETTINGS_HELPER_H_
#define CONTENT_PUBLIC_BROWSER_PEPPER_FLASH_SETTINGS_HELPER_H_

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "content/common/content_export.h"

namespace base {
class FilePath;
}

namespace IPC {
struct ChannelHandle;
}

namespace content {

class CONTENT_EXPORT PepperFlashSettingsHelper
    : public base::RefCounted<PepperFlashSettingsHelper> {
 public:
  static scoped_refptr<PepperFlashSettingsHelper> Create();

  
  
  
  typedef base::Callback<void(bool ,
                              const IPC::ChannelHandle& )>
      OpenChannelCallback;
  virtual void OpenChannelToBroker(const base::FilePath& path,
                                   const OpenChannelCallback& callback) = 0;

 protected:
  friend class base::RefCounted<PepperFlashSettingsHelper>;
  virtual ~PepperFlashSettingsHelper() {}
};

}  

#endif  
