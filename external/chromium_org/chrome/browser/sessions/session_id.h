// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_SESSION_ID_H_
#define CHROME_BROWSER_SESSIONS_SESSION_ID_H_

#include "base/basictypes.h"

namespace content {
class WebContents;
}

class SessionID {
 public:
  typedef int32 id_type;

  SessionID();
  ~SessionID() {}

  
  
  
  
  
  static id_type IdForTab(const content::WebContents* tab);

  
  
  
  
  
  
  
  
  static id_type IdForWindowContainingTab(const content::WebContents* tab);

  
  void set_id(id_type id) { id_ = id; }
  id_type id() const { return id_; }

 private:
  id_type id_;
};

#endif  
