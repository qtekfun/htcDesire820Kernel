// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_EXTENSIONS_DEFAULT_APP_ORDER_H_
#define CHROME_BROWSER_CHROMEOS_EXTENSIONS_DEFAULT_APP_ORDER_H_

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/synchronization/waitable_event.h"

namespace chromeos {
namespace default_app_order {

class ExternalLoader {
 public:
  
  
  explicit ExternalLoader(bool async);
  ~ExternalLoader();

  const std::vector<std::string>& GetAppIds();

 private:
  void Load();

  
  std::vector<std::string> app_ids_;

  base::WaitableEvent loaded_;

  DISALLOW_COPY_AND_ASSIGN(ExternalLoader);
};

void Get(std::vector<std::string>* app_ids);

}  
}  

#endif  
