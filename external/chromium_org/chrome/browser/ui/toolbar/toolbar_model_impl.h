// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_TOOLBAR_TOOLBAR_MODEL_IMPL_H_
#define CHROME_BROWSER_UI_TOOLBAR_TOOLBAR_MODEL_IMPL_H_

#include <string>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/strings/string16.h"
#include "chrome/browser/ui/toolbar/toolbar_model.h"
#include "url/gurl.h"

class Profile;
class ToolbarModelDelegate;

namespace content {
class NavigationController;
class WebContents;
}

namespace net {
class X509Certificate;
}

class ToolbarModelImpl : public ToolbarModel {
 public:
  explicit ToolbarModelImpl(ToolbarModelDelegate* delegate);
  virtual ~ToolbarModelImpl();

  static SecurityLevel GetSecurityLevelForWebContents(
      content::WebContents* web_contents);

  
  static base::string16 GetEVCertName(const net::X509Certificate& cert);

 private:
  
  virtual base::string16 GetText() const OVERRIDE;
  virtual base::string16 GetCorpusNameForMobile() const OVERRIDE;
  virtual GURL GetURL() const OVERRIDE;
  virtual bool WouldOmitURLDueToOriginChip() const OVERRIDE;
  virtual bool WouldPerformSearchTermReplacement(
      bool ignore_editing) const OVERRIDE;
  virtual SecurityLevel GetSecurityLevel(bool ignore_editing) const OVERRIDE;
  virtual int GetIcon() const OVERRIDE;
  virtual int GetIconForSecurityLevel(SecurityLevel level) const OVERRIDE;
  virtual base::string16 GetEVCertName() const OVERRIDE;
  virtual bool ShouldDisplayURL() const OVERRIDE;

  
  
  
  content::NavigationController* GetNavigationController() const;

  
  Profile* GetProfile() const;

  
  
  
  
  base::string16 GetSearchTerms(bool ignore_editing) const;

  ToolbarModelDelegate* delegate_;

  DISALLOW_IMPLICIT_CONSTRUCTORS(ToolbarModelImpl);
};

#endif  
