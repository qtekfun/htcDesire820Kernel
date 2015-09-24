// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_APP_ICON_LOADER_H_
#define CHROME_BROWSER_EXTENSIONS_APP_ICON_LOADER_H_

#include <string>

#include "base/basictypes.h"

namespace gfx {
class ImageSkia;
}

namespace extensions {

class AppIconLoader {
 public:
  class Delegate {
   public:
    virtual ~Delegate() {}

    
    virtual void SetAppImage(const std::string& id,
                             const gfx::ImageSkia& image) = 0;
  };

  AppIconLoader() {}
  virtual ~AppIconLoader() {}

  
  
  virtual void FetchImage(const std::string& id) = 0;

  
  virtual void ClearImage(const std::string& id) = 0;

  
  
  
  virtual void UpdateImage(const std::string& id) = 0;

 private:
  DISALLOW_COPY_AND_ASSIGN(AppIconLoader);
};

}  

#endif  
