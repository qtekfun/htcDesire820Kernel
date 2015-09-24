// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_BROWSER_DEBUGGER_DEBUGGER_HOST_H_
#define CHROME_BROWSER_DEBUGGER_DEBUGGER_HOST_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

class ListValue;

class DebuggerHost : public base::RefCountedThreadSafe<DebuggerHost> {
 public:
  DebuggerHost() {}
  virtual ~DebuggerHost() {}

  
  virtual void Start() = 0;

  
  
  virtual void DebugMessage(const std::wstring& msg) = 0;
  
  virtual void OnDebugAttach() = 0;
  
  virtual void OnDebugDisconnect() = 0;

  virtual void DidDisconnect() = 0;
  virtual void DidConnect() {}
  virtual void ProcessCommand(const std::wstring& data) {}

  
  virtual void OnDebuggerHostMsg(const ListValue* args) {}

  
  virtual bool ShowWindow() { return false; }

 private:

  DISALLOW_COPY_AND_ASSIGN(DebuggerHost);
};

#endif  
