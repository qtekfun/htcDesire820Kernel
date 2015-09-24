// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_RDP_DESKTOP_SESSION_H_
#define REMOTING_HOST_WIN_RDP_DESKTOP_SESSION_H_

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include "base/memory/scoped_ptr.h"
#include "base/win/scoped_comptr.h"
#include "remoting/host/chromoting_lib.h"
#include "remoting/host/win/rdp_client.h"

namespace remoting {

class __declspec(uuid(RDP_DESKTOP_SESSION_CLSID)) RdpDesktopSession
    : public ATL::CComObjectRootEx<ATL::CComSingleThreadModel>,
      public ATL::CComCoClass<RdpDesktopSession, &__uuidof(RdpDesktopSession)>,
      public IRdpDesktopSession,
      public RdpClient::EventHandler {
 public:
  
  
  
  
  
  
  
  
  typedef ATL::CComCreator<ATL::CComObjectNoLock<ATL::CComClassFactory> >
      _ClassFactoryCreatorClass;

  RdpDesktopSession();

  
  STDMETHOD(Connect)(long width, long height, BSTR terminal_id,
                     IRdpDesktopSessionEventHandler* event_handler);
  STDMETHOD(Disconnect)();
  STDMETHOD(ChangeResolution)(long width, long height);
  STDMETHOD(InjectSas)();

  DECLARE_NO_REGISTRY()

 private:
  
  virtual void OnRdpConnected() OVERRIDE;
  virtual void OnRdpClosed() OVERRIDE;

  BEGIN_COM_MAP(RdpDesktopSession)
    COM_INTERFACE_ENTRY(IRdpDesktopSession)
    COM_INTERFACE_ENTRY(IUnknown)
  END_COM_MAP()

  
  scoped_ptr<RdpClient> client_;

  
  
  base::win::ScopedComPtr<IRdpDesktopSessionEventHandler> event_handler_;

  DECLARE_PROTECT_FINAL_CONSTRUCT()
};

} 

#endif  
