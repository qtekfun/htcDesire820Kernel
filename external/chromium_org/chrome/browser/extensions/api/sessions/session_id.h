// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_SESSIONS_SESSION_ID_H__
#define CHROME_BROWSER_EXTENSIONS_API_SESSIONS_SESSION_ID_H__

#include <string>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"

namespace extensions {

class SessionId {
 public:
  
  
  
  static scoped_ptr<SessionId> Parse(const std::string& session_string);

  
  
  
  
  
  
  SessionId(const std::string& session_tag, int id);

  
  bool IsForeign() const;

  
  
  std::string ToString() const;

  const std::string& session_tag() const { return session_tag_; }
  int id() const { return id_; }

 private:
  
  
  std::string session_tag_;

  
  int id_;

  DISALLOW_COPY_AND_ASSIGN(SessionId);
};

}  

#endif  
