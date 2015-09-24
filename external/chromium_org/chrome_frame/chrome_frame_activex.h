// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_CHROME_FRAME_ACTIVEX_H_
#define CHROME_FRAME_CHROME_FRAME_ACTIVEX_H_

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>

#include <set>
#include <string>
#include <vector>

#include "chrome_frame/chrome_frame_activex_base.h"
#include "chrome_frame/chrome_tab.h"
#include "chrome_frame/com_type_info_holder.h"
#include "grit/chrome_frame_resources.h"

#define WM_HOST_MOVED_NOTIFICATION (WM_APP + 1)

class ATL_NO_VTABLE ChromeFrameActivex
    : public ChromeFrameActivexBase<ChromeFrameActivex, CLSID_ChromeFrame>,
      public IObjectSafetyImpl<ChromeFrameActivex,
                                 INTERFACESAFE_FOR_UNTRUSTED_CALLER |
                                 INTERFACESAFE_FOR_UNTRUSTED_DATA>,
      public IObjectWithSiteImpl<ChromeFrameActivex>,
      public IPersistPropertyBag {
 public:
  typedef ChromeFrameActivexBase<ChromeFrameActivex, CLSID_ChromeFrame> Base;
  ChromeFrameActivex();
  ~ChromeFrameActivex();

DECLARE_REGISTRY_RESOURCEID(IDR_CHROMEFRAME_ACTIVEX)

BEGIN_COM_MAP(ChromeFrameActivex)
  COM_INTERFACE_ENTRY(IObjectWithSite)
  COM_INTERFACE_ENTRY(IObjectSafety)
  COM_INTERFACE_ENTRY(IPersist)
  COM_INTERFACE_ENTRY(IPersistPropertyBag)
  COM_INTERFACE_ENTRY_CHAIN(Base)
END_COM_MAP()

BEGIN_MSG_MAP(ChromeFrameActivex)
  MESSAGE_HANDLER(WM_CREATE, OnCreate)
  MESSAGE_HANDLER(WM_HOST_MOVED_NOTIFICATION, OnHostMoved)
  CHAIN_MSG_MAP(Base)
END_MSG_MAP()

  HRESULT FinalConstruct();

  virtual HRESULT OnDraw(ATL_DRAWINFO& draw_info);  

  
  STDMETHOD(GetClassID)(CLSID* class_id) {
    if (class_id != NULL)
      *class_id = GetObjectCLSID();
    return S_OK;
  }

  STDMETHOD(InitNew)() {
    return S_OK;
  }

  STDMETHOD(Load)(IPropertyBag* bag, IErrorLog* error_log);

  STDMETHOD(Save)(IPropertyBag* bag, BOOL clear_dirty, BOOL save_all) {
    return E_NOTIMPL;
  }

  
  
  HRESULT IOleObject_SetClientSite(IOleClientSite* client_site);

  
  STDMETHOD(put_src)(BSTR src);

  
  
  
  
  STDMETHOD(registerBhoIfNeeded)();

 protected:
  
  virtual void OnLoad(const GURL& url);
  virtual void OnMessageFromChromeFrame(const std::string& message,
                                        const std::string& origin,
                                        const std::string& target);
  virtual void OnLoadFailed(int error_code, const std::string& url);
  virtual void OnAutomationServerLaunchFailed(
      AutomationLaunchResult reason, const std::string& server_version);
  virtual void OnChannelError();

  
  static bool ShouldShowVersionMismatchDialog(bool is_privileged,
                                              IOleClientSite* client_site);

 private:
  LRESULT OnCreate(UINT message, WPARAM wparam, LPARAM lparam,
                   BOOL& handled);  
  LRESULT OnHostMoved(UINT message, WPARAM wparam, LPARAM lparam,
                      BOOL& handled);  

  HRESULT GetContainingDocument(IHTMLDocument2** doc);
  HRESULT GetDocumentWindow(IHTMLWindow2** window);

  
  HRESULT GetObjectScriptId(IHTMLObjectElement* object_elem, BSTR* id);

  
  
  
  HRESULT GetObjectElement(IHTMLObjectElement** element);

  HRESULT CreateScriptBlockForEvent(IHTMLElement2* insert_after,
                                    BSTR instance_id, BSTR script,
                                    BSTR event_name);

  
  
  
  void FireEvent(const EventHandlers& handlers, const std::string& arg);

  
  void FireEvent(const EventHandlers& handlers, IDispatch* event);

  
  void FireEvent(const EventHandlers& handlers, IDispatch* event,
                 BSTR target);

  
  HRESULT InstallTopLevelHook(IOleClientSite* client_site);

  
  HHOOK chrome_wndproc_hook_;

  
  
  bool attaching_to_existing_cf_tab_;
};

#endif  
