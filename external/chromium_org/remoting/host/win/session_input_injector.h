// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_SESSION_INPUT_INJECTOR_H_
#define REMOTING_HOST_WIN_SESSION_INPUT_INJECTOR_H_

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "remoting/host/input_injector.h"

namespace base {
class SingleThreadTaskRunner;
} 

namespace remoting {

class SessionInputInjectorWin : public InputInjector {
 public:
  
  
  SessionInputInjectorWin(
      scoped_refptr<base::SingleThreadTaskRunner> input_task_runner,
      scoped_ptr<InputInjector> nested_executor,
      scoped_refptr<base::SingleThreadTaskRunner> inject_sas_task_runner,
      const base::Closure& inject_sas);
  virtual ~SessionInputInjectorWin();

  
  virtual void Start(
      scoped_ptr<protocol::ClipboardStub> client_clipboard) OVERRIDE;

  
  virtual void InjectClipboardEvent(
      const protocol::ClipboardEvent& event) OVERRIDE;

  
  virtual void InjectKeyEvent(const protocol::KeyEvent& event) OVERRIDE;
  virtual void InjectMouseEvent(const protocol::MouseEvent& event) OVERRIDE;

 private:
  
  class Core;
  scoped_refptr<Core> core_;

  DISALLOW_COPY_AND_ASSIGN(SessionInputInjectorWin);
};

}  

#endif  
