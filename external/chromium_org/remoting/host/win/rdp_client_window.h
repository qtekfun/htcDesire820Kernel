// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTING_HOST_WIN_RDP_HOST_WINDOW_H_
#define REMOTING_HOST_WIN_RDP_HOST_WINDOW_H_

#include <atlbase.h>
#include <atlcom.h>
#include <atlcrack.h>
#include <atlctl.h>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/message_loop/message_loop.h"
#include "base/win/scoped_comptr.h"
#include "net/base/ip_endpoint.h"
#include "third_party/webrtc/modules/desktop_capture/desktop_geometry.h"

#import "PROGID:MsTscAx.MsTscAx" \
    exclude("wireHWND", "_RemotableHandle", "__MIDL_IWinTypes_0009"), \
    rename_namespace("mstsc") raw_interfaces_only no_implementation

namespace remoting {

class RdpClientWindow
    : public CWindowImpl<RdpClientWindow, CWindow, CFrameWinTraits>,
      public IDispEventImpl<1, RdpClientWindow,
                            &__uuidof(mstsc::IMsTscAxEvents),
                            &__uuidof(mstsc::__MSTSCLib), 1, 0> {
 public:
  
  
  
  
  
  
  class EventHandler {
   public:
    virtual ~EventHandler() {}

    
    virtual void OnConnected() = 0;

    
    
    
    
    
    
    
    
    
    
    
    virtual void OnDisconnected() = 0;
  };

  DECLARE_WND_CLASS(L"RdpClientWindow")

  
  
  RdpClientWindow(const net::IPEndPoint& server_endpoint,
                  const std::string& terminal_id,
                  EventHandler* event_handler);
  ~RdpClientWindow();

  
  
  
  bool Connect(const webrtc::DesktopSize& screen_size);

  
  
  void Disconnect();

  
  
  void InjectSas();

 private:
  typedef IDispEventImpl<1, RdpClientWindow,
                         &__uuidof(mstsc::IMsTscAxEvents),
                         &__uuidof(mstsc::__MSTSCLib), 1, 0> RdpEventsSink;

  
  BEGIN_MSG_MAP_EX(RdpClientWindow)
    MSG_WM_CLOSE(OnClose)
    MSG_WM_CREATE(OnCreate)
    MSG_WM_DESTROY(OnDestroy)
  END_MSG_MAP()

  
  void OnClose();

  
  
  LRESULT OnCreate(CREATESTRUCT* create_struct);

  
  void OnDestroy();

  BEGIN_SINK_MAP(RdpClientWindow)
    SINK_ENTRY_EX(1, __uuidof(mstsc::IMsTscAxEvents), 2, OnConnected)
    SINK_ENTRY_EX(1, __uuidof(mstsc::IMsTscAxEvents), 4, OnDisconnected)
    SINK_ENTRY_EX(1, __uuidof(mstsc::IMsTscAxEvents), 10, OnFatalError)
    SINK_ENTRY_EX(1, __uuidof(mstsc::IMsTscAxEvents), 15, OnConfirmClose)
    SINK_ENTRY_EX(1, __uuidof(mstsc::IMsTscAxEvents), 18,
                  OnAuthenticationWarningDisplayed)
    SINK_ENTRY_EX(1, __uuidof(mstsc::IMsTscAxEvents), 19,
                  OnAuthenticationWarningDismissed)
  END_SINK_MAP()

  
  STDMETHOD(OnAuthenticationWarningDisplayed)();
  STDMETHOD(OnAuthenticationWarningDismissed)();
  STDMETHOD(OnConnected)();
  STDMETHOD(OnDisconnected)(long reason);
  STDMETHOD(OnFatalError)(long error_code);
  STDMETHOD(OnConfirmClose)(VARIANT_BOOL* allow_close);

  
  
  
  void NotifyConnected();
  void NotifyDisconnected();

  
  EventHandler* event_handler_;

  
  webrtc::DesktopSize screen_size_;

  
  net::IPEndPoint server_endpoint_;

  
  std::string terminal_id_;

  
  base::win::ScopedComPtr<mstsc::IMsRdpClient> client_;
  base::win::ScopedComPtr<mstsc::IMsRdpClientAdvancedSettings> client_settings_;

  
  class WindowHook;
  scoped_refptr<WindowHook> window_activate_hook_;
};

}  

#endif  
