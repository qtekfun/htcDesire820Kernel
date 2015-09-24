// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_IPC_INPUT_INJECTOR_H_
#define REMOTING_HOST_IPC_INPUT_INJECTOR_H_

#include "base/memory/ref_counted.h"
#include "remoting/host/input_injector.h"
#include "remoting/proto/event.pb.h"

namespace remoting {

class DesktopSessionProxy;

class IpcInputInjector : public InputInjector {
 public:
  explicit IpcInputInjector(
      scoped_refptr<DesktopSessionProxy> desktop_session_proxy);
  virtual ~IpcInputInjector();

  
  virtual void InjectClipboardEvent(
      const protocol::ClipboardEvent& event) OVERRIDE;

  
  virtual void InjectKeyEvent(const protocol::KeyEvent& event) OVERRIDE;
  virtual void InjectMouseEvent(const protocol::MouseEvent& event) OVERRIDE;

  
  virtual void Start(
      scoped_ptr<protocol::ClipboardStub> client_clipboard) OVERRIDE;

 private:
  
  scoped_refptr<DesktopSessionProxy> desktop_session_proxy_;

  DISALLOW_COPY_AND_ASSIGN(IpcInputInjector);
};

}  

#endif  
