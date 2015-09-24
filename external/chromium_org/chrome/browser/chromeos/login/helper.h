// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_HELPER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_HELPER_H_

#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "third_party/skia/include/core/SkColor.h"
#include "ui/views/view.h"

class GURL;

namespace gfx {
class Rect;
class Size;
}  

namespace chromeos {

gfx::Rect CalculateScreenBounds(const gfx::Size& size);

int GetCurrentUserImageSize();

namespace login {

const int kMaxUserImageSize = 512;

class NetworkStateHelper {
 public:
  NetworkStateHelper();
  virtual ~NetworkStateHelper();

  
  
  
  
  
  virtual base::string16 GetCurrentNetworkName() const;

  
  virtual bool IsConnected() const;

  
  virtual bool IsConnecting() const;

 private:
  DISALLOW_COPY_AND_ASSIGN(NetworkStateHelper);
};

}  

}  

#endif  
