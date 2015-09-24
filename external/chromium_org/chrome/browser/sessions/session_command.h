// Copyright (c) 2006-2008 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_SESSION_COMMAND_H_
#define CHROME_BROWSER_SESSIONS_SESSION_COMMAND_H_

#include <string>

#include "base/basictypes.h"

class Pickle;

class SessionCommand {
 public:
  // These get written to disk, so we define types for them.
  
  typedef uint8 id_type;

  
  typedef uint16 size_type;

  
  
  SessionCommand(id_type id, size_type size);

  
  
  SessionCommand(id_type id, const Pickle& pickle);

  
  char* contents() { return const_cast<char*>(contents_.c_str()); }
  const char* contents() const { return contents_.c_str(); }

  
  id_type id() const { return id_; }

  
  size_type size() const { return static_cast<size_type>(contents_.size()); }

  
  
  bool GetPayload(void* dest, size_t count) const;

  
  
  
  
  Pickle* PayloadAsPickle() const;

 private:
  const id_type id_;
  std::string contents_;

  DISALLOW_COPY_AND_ASSIGN(SessionCommand);
};

#endif  
