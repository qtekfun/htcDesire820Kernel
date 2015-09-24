// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TOOLBAR_TOOLBAR_MODEL_H_
#define CHROME_BROWSER_UI_TOOLBAR_TOOLBAR_MODEL_H_

#include <string>

#include "base/basictypes.h"
#include "base/strings/string16.h"
#include "url/gurl.h"

namespace net {
class X509Certificate;
}

class ToolbarModel {
 public:
  
  
  
  enum SecurityLevel {
#define DEFINE_TOOLBAR_MODEL_SECURITY_LEVEL(name,value)  name = value,
#include "chrome/browser/ui/toolbar/toolbar_model_security_level_list.h"
#undef DEFINE_TOOLBAR_MODEL_SECURITY_LEVEL
  };

  virtual ~ToolbarModel();

  
  
  
  
  
  
  
  virtual base::string16 GetText() const = 0;

  
  
  
  
  
  virtual base::string16 GetCorpusNameForMobile() const = 0;

  
  virtual GURL GetURL() const = 0;

  
  
  
  
  virtual bool WouldPerformSearchTermReplacement(bool ignore_editing) const = 0;

  
  
  
  bool WouldReplaceURL() const;

  
  
  
  
  virtual SecurityLevel GetSecurityLevel(bool ignore_editing) const = 0;

  
  
  
  
  virtual int GetIcon() const = 0;

  
  
  virtual int GetIconForSecurityLevel(SecurityLevel level) const = 0;

  
  
  virtual base::string16 GetEVCertName() const = 0;

  
  
  virtual bool ShouldDisplayURL() const = 0;

  
  void set_input_in_progress(bool input_in_progress) {
    input_in_progress_ = input_in_progress;
  }
  bool input_in_progress() const { return input_in_progress_; }

  
  void set_url_replacement_enabled(bool enabled) {
    url_replacement_enabled_ = enabled;
  }
  bool url_replacement_enabled() const {
    return url_replacement_enabled_;
  }

 protected:
  ToolbarModel();

 private:
  
  
  
  
  virtual bool WouldOmitURLDueToOriginChip() const = 0;

  bool input_in_progress_;
  bool url_replacement_enabled_;

  DISALLOW_COPY_AND_ASSIGN(ToolbarModel);
};

#endif  
