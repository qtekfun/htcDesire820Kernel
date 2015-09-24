// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_NAVIGATION_CONSTRAINTS_H_
#define CHROME_FRAME_NAVIGATION_CONSTRAINTS_H_

#include <urlmon.h>
#include <windows.h>

#include "base/win/scoped_comptr.h"
#include "url/gurl.h"

class NavigationConstraints {
 public:
  virtual ~NavigationConstraints() {}
  virtual bool AllowUnsafeUrls() = 0;
  virtual bool IsSchemeAllowed(const GURL& url) = 0;
  virtual bool IsZoneAllowed(const GURL& url) = 0;
};

class NavigationConstraintsImpl : public NavigationConstraints {
 public:
  NavigationConstraintsImpl();
  virtual ~NavigationConstraintsImpl() {}

  
  virtual bool AllowUnsafeUrls();
  virtual bool IsSchemeAllowed(const GURL& url);
  virtual bool IsZoneAllowed(const GURL& url);

  bool is_privileged() const;
  void set_is_privileged(bool is_privileged);

 private:
  base::win::ScopedComPtr<IInternetSecurityManager> security_manager_;

  
  
  
  
  
  bool is_privileged_;
};

#endif  
