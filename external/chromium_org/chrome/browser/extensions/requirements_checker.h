// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_REQUIREMENTS_CHECKER_H_
#define CHROME_BROWSER_EXTENSIONS_REQUIREMENTS_CHECKER_H_

#include <vector>

#include "base/callback.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/extensions/extension_service.h"

class GPUFeatureChecker;

namespace extensions {
class Extension;

class RequirementsChecker : public base::SupportsWeakPtr<RequirementsChecker> {
 public:
  RequirementsChecker();
  ~RequirementsChecker();

  
  
  
  
  
  void Check(scoped_refptr<const Extension> extension,
      base::Callback<void(std::vector<std::string> requirement)> callback);

 private:
  
  void SetWebGLAvailability(bool available);
  void SetCSS3DAvailability(bool available);

  void MaybeRunCallback();

  std::vector<std::string> errors_;

  
  
  int pending_requirement_checks_;

  scoped_refptr<GPUFeatureChecker> webgl_checker_;
  scoped_refptr<GPUFeatureChecker> css3d_checker_;

  base::Callback<void(std::vector<std::string> requirement_errorss)> callback_;
};

}  

#endif  
