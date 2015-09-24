// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_CHROME_FRAME_ACTIVEX_BASE_H_
#define CHROME_FRAME_CHROME_FRAME_ACTIVEX_BASE_H_

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <exdisp.h>
#include <wininet.h>
#include <shdeprecated.h>  
#include <shlguid.h>

#include <set>
#include <string>
#include <vector>

#include "base/metrics/histogram.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "base/win/scoped_bstr.h"
#include "base/win/scoped_comptr.h"
#include "base/win/scoped_variant.h"
#include "chrome/app/chrome_command_ids.h"
#include "chrome/common/url_constants.h"
#include "chrome_frame/chrome_frame_plugin.h"
#include "chrome_frame/chrome_tab.h"
#include "chrome_frame/com_message_event.h"
#include "chrome_frame/com_type_info_holder.h"
#include "chrome_frame/simple_resource_loader.h"
#include "chrome_frame/urlmon_url_request.h"
#include "chrome_frame/urlmon_url_request_private.h"
#include "chrome_frame/utils.h"
#include "grit/chrome_frame_resources.h"
#include "grit/generated_resources.h"
#include "net/cookies/cookie_monster.h"

template<class T>
class ATL_NO_VTABLE ProxyDIChromeFrameEvents
    : public IConnectionPointImpl<T, &DIID_DIChromeFrameEvents> {
 public:
  void FireMethodWithParams(ChromeFrameEventDispId dispid,
                            const VARIANT* params, size_t num_params) {
    T* me = static_cast<T*>(this);
    
    
    
    
    me->Lock();
    std::vector< base::win::ScopedComPtr<IUnknown> > sink_array(
        m_vec.GetSize());
    for (int connection = 0; connection < m_vec.GetSize(); ++connection)
      sink_array[connection] = m_vec.GetAt(connection);
    me->Unlock();

    for (size_t sink = 0; sink < sink_array.size(); ++sink) {
      DIChromeFrameEvents* events =
          static_cast<DIChromeFrameEvents*>(sink_array[sink].get());
      if (events) {
        DISPPARAMS disp_params = {
            const_cast<VARIANT*>(params),
            NULL,
            num_params,
            0};
        HRESULT hr = events->Invoke(static_cast<DISPID>(dispid),
                                    DIID_DIChromeFrameEvents,
                                    LOCALE_USER_DEFAULT, DISPATCH_METHOD,
                                    &disp_params, NULL, NULL, NULL);
        DLOG_IF(ERROR, FAILED(hr)) << "invoke(" << dispid << ") failed" <<
            base::StringPrintf("0x%08X", hr);
      }
    }
  }

  void FireMethodWithParam(ChromeFrameEventDispId dispid,
                           const VARIANT& param) {
    FireMethodWithParams(dispid, &param, 1);
  }

  void Fire_onload(IDispatch* event) {
    VARIANT var = { VT_DISPATCH };
    var.pdispVal = event;
    FireMethodWithParam(CF_EVENT_DISPID_ONLOAD, var);
  }

  void Fire_onloaderror(IDispatch* event) {
    VARIANT var = { VT_DISPATCH };
    var.pdispVal = event;
    FireMethodWithParam(CF_EVENT_DISPID_ONLOADERROR, var);
  }

  void Fire_onmessage(IDispatch* event) {
    VARIANT var = { VT_DISPATCH };
    var.pdispVal = event;
    FireMethodWithParam(CF_EVENT_DISPID_ONMESSAGE, var);
  }

  void Fire_onreadystatechanged(long readystate) {  
    VARIANT var = { VT_I4 };
    var.lVal = readystate;
    FireMethodWithParam(CF_EVENT_DISPID_ONREADYSTATECHANGED, var);
  }

  void Fire_onprivatemessage(IDispatch* event, BSTR target) {
    
    
    VARIANT args[2] = { { VT_BSTR, }, {VT_DISPATCH, } };
    args[0].bstrVal = target;
    args[1].pdispVal = event;

    FireMethodWithParams(CF_EVENT_DISPID_ONPRIVATEMESSAGE,
                         args,
                         arraysize(args));
  }

  void Fire_onchannelerror() {  
    FireMethodWithParams(CF_EVENT_DISPID_ONCHANNELERROR, NULL, 0);
  }

  void Fire_onclose() {  
    FireMethodWithParams(CF_EVENT_DISPID_ONCLOSE, NULL, 0);
  }
};

extern bool g_first_launch_by_process_;

namespace chrome_frame {
std::string ActiveXCreateUrl(const GURL& parsed_url,
                             const AttachExternalTabParams& params);
int GetDisposition(const AttachExternalTabParams& params);
void GetMiniContextMenuData(UINT cmd,
                            const MiniContextMenuParams& params,
                            GURL* referrer,
                            GURL* url);
}  

template <class T, const CLSID& class_id>
class ATL_NO_VTABLE ChromeFrameActivexBase :  
  public CComObjectRootEx<CComMultiThreadModel>,
  public IOleControlImpl<T>,
  public IOleObjectImpl<T>,
  public IOleInPlaceActiveObjectImpl<T>,
  public IViewObjectExImpl<T>,
  public IOleInPlaceObjectWindowlessImpl<T>,
  public ISupportErrorInfo,
  public IQuickActivateImpl<T>,
  public com_util::IProvideClassInfo2Impl<class_id,
                                          DIID_DIChromeFrameEvents>,
  public com_util::IDispatchImpl<IChromeFrame>,
  public IConnectionPointContainerImpl<T>,
  public ProxyDIChromeFrameEvents<T>,
  public IPropertyNotifySinkCP<T>,
  public CComCoClass<T, &class_id>,
  public CComControl<T>,
  public ChromeFramePlugin<T> {
 protected:
  typedef std::set<base::win::ScopedComPtr<IDispatch> > EventHandlers;
  typedef ChromeFrameActivexBase<T, class_id> BasePlugin;

 public:
  ChromeFrameActivexBase()
      : ready_state_(READYSTATE_UNINITIALIZED),
      url_fetcher_(new UrlmonUrlRequestManager()),
      failed_to_fetch_in_place_frame_(false),
      draw_sad_tab_(false) {
    m_bWindowOnly = TRUE;
    url_fetcher_->set_container(static_cast<IDispatch*>(this));
  }

  ~ChromeFrameActivexBase() {
    url_fetcher_->set_container(NULL);
  }

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | OLEMISC_CANTLINKINSIDE |
                       OLEMISC_INSIDEOUT | OLEMISC_ACTIVATEWHENVISIBLE |
                       OLEMISC_SETCLIENTSITEFIRST)

DECLARE_NOT_AGGREGATABLE(T)

BEGIN_COM_MAP(ChromeFrameActivexBase)
  COM_INTERFACE_ENTRY(IChromeFrame)
  COM_INTERFACE_ENTRY(IDispatch)
  COM_INTERFACE_ENTRY(IViewObjectEx)
  COM_INTERFACE_ENTRY(IViewObject2)
  COM_INTERFACE_ENTRY(IViewObject)
  COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
  COM_INTERFACE_ENTRY(IOleInPlaceObject)
  COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
  COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
  COM_INTERFACE_ENTRY(IOleControl)
  COM_INTERFACE_ENTRY(IOleObject)
  COM_INTERFACE_ENTRY(ISupportErrorInfo)
  COM_INTERFACE_ENTRY(IQuickActivate)
  COM_INTERFACE_ENTRY(IProvideClassInfo)
  COM_INTERFACE_ENTRY(IProvideClassInfo2)
  COM_INTERFACE_ENTRY(IConnectionPointContainer)
  COM_INTERFACE_ENTRY_FUNC_BLIND(0, InterfaceNotSupported)
END_COM_MAP()

BEGIN_CONNECTION_POINT_MAP(T)
  CONNECTION_POINT_ENTRY(IID_IPropertyNotifySink)
  CONNECTION_POINT_ENTRY(DIID_DIChromeFrameEvents)
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(ChromeFrameActivexBase)
  MESSAGE_HANDLER(WM_CREATE, OnCreate)
  MESSAGE_HANDLER(WM_DOWNLOAD_IN_HOST, OnDownloadRequestInHost)
  MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
  CHAIN_MSG_MAP(ChromeFramePlugin<T>)
  CHAIN_MSG_MAP(CComControl<T>)
  DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()

  
  DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

  inline HRESULT IViewObject_Draw(DWORD draw_aspect, LONG index,
      void* aspect_info, DVTARGETDEVICE* ptd, HDC info_dc, HDC dc,
      LPCRECTL bounds, LPCRECTL win_bounds) {
    
    DWORD aspect = draw_aspect;
    if (aspect == DVASPECT_DOCPRINT)
      aspect = DVASPECT_CONTENT;

    return CComControl<T>::IViewObject_Draw(aspect, index, aspect_info, ptd,
        info_dc, dc, bounds, win_bounds);
  }

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  void SetResourceModule() {
    SimpleResourceLoader* loader_instance = SimpleResourceLoader::GetInstance();
    DCHECK(loader_instance);
    HMODULE res_dll = loader_instance->GetResourceModuleHandle();
    _AtlBaseModule.SetResourceInstance(res_dll);
  }

  HRESULT FinalConstruct() {
    SetResourceModule();

    if (!Initialize())
      return E_OUTOFMEMORY;

    
    
    if (g_first_launch_by_process_) {
      g_first_launch_by_process_ = false;
      UMA_HISTOGRAM_CUSTOM_COUNTS("ChromeFrame.IEVersion",
                                  GetIEVersion(),
                                  IE_INVALID,
                                  IE_10,
                                  IE_10 + 1);
    }

    return S_OK;
  }

  void FinalRelease() {
    Uninitialize();
  }

  void ResetUrlRequestManager() {
    url_fetcher_.reset(new UrlmonUrlRequestManager());
  }

  static HRESULT WINAPI InterfaceNotSupported(void* pv, REFIID riid, void** ppv,
                                              DWORD dw) {
#ifndef NDEBUG
    wchar_t buffer[64] = {0};
    ::StringFromGUID2(riid, buffer, arraysize(buffer));
    DVLOG(1) << "E_NOINTERFACE: " << buffer;
#endif
    return E_NOINTERFACE;
  }

  
  STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid) {
    static const IID* interfaces[] = {
      &IID_IChromeFrame,
      &IID_IDispatch
    };

    for (int i = 0; i < arraysize(interfaces); ++i) {
      if (InlineIsEqualGUID(*interfaces[i], riid))
        return S_OK;
    }
    return S_FALSE;
  }

  
  virtual HRESULT OnDraw(ATL_DRAWINFO& draw_info) {  
    if (NULL == draw_info.prcBounds) {
      NOTREACHED();
      return E_FAIL;
    }

    if (draw_sad_tab_) {
      
      RECT rc = {0};
      if (draw_info.prcBounds) {
        rc.top = draw_info.prcBounds->top;
        rc.bottom = draw_info.prcBounds->bottom;
        rc.left = draw_info.prcBounds->left;
        rc.right = draw_info.prcBounds->right;
      } else {
        GetClientRect(&rc);
      }
      ::DrawTextA(draw_info.hdcDraw, ":-(", -1, &rc,
          DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    } else {
      
    }
    return S_OK;
  }

  
  
  HRESULT IOleObject_SetClientSite(IOleClientSite* client_site) {
    
    doc_site_.Release();
    if (client_site) {
      doc_site_.QueryFrom(client_site);
    }

    if (client_site == NULL) {
      in_place_frame_.Release();
    }

    return CComControlBase::IOleObject_SetClientSite(client_site);
  }

  bool HandleContextMenuCommand(UINT cmd, const MiniContextMenuParams& params) {
    if (cmd == IDC_ABOUT_CHROME_FRAME) {
      int tab_handle = automation_client_->tab()->handle();
      HostNavigate(GURL("about:version"), GURL(), NEW_WINDOW);
      return true;
    } else {
      switch (cmd) {
        case IDS_CONTENT_CONTEXT_SAVEAUDIOAS:
        case IDS_CONTENT_CONTEXT_SAVEVIDEOAS:
        case IDS_CONTENT_CONTEXT_SAVEIMAGEAS:
        case IDS_CONTENT_CONTEXT_SAVELINKAS: {
          GURL referrer, url;
          chrome_frame::GetMiniContextMenuData(cmd, params, &referrer, &url);
          DoFileDownloadInIE(UTF8ToWide(url.spec()).c_str());
          return true;
        }

        case IDC_PRINT: {
          automation_client_->PrintTab();
          return true;
        }
      }
    }

    return false;
  }

  
  
  
  
  
  
  
  
  
  
  
  bool is_frame_busting_enabled() const {
    return !is_privileged();
  }

  static void BringWebBrowserWindowToTop(IWebBrowser2* web_browser2) {
    DCHECK(web_browser2);
    if (web_browser2) {
      web_browser2->put_Visible(VARIANT_TRUE);
      HWND ie_window = NULL;
      web_browser2->get_HWND(reinterpret_cast<long*>(&ie_window));
      ::BringWindowToTop(ie_window);
    }
  }

 protected:
  virtual void GetProfilePath(const std::wstring& profile_name,
                              base::FilePath* profile_path) {
    bool is_IE = (lstrcmpi(profile_name.c_str(), kIexploreProfileName) == 0) ||
                 (lstrcmpi(profile_name.c_str(), kRundllProfileName) == 0);
    
    
    if (is_IE && GetIEVersion() < IE_8) {
      *profile_path = GetIETemporaryFilesFolder();
      *profile_path = profile_path->Append(L"Google Chrome Frame");
    } else {
      ChromeFramePlugin::GetProfilePath(profile_name, profile_path);
    }
    DVLOG(1) << __FUNCTION__ << ": " << profile_path->value();
  }

  void OnLoad(const GURL& url) {
    if (ready_state_ < READYSTATE_COMPLETE) {
      ready_state_ = READYSTATE_COMPLETE;
      FireOnChanged(DISPID_READYSTATE);
    }

    HRESULT hr = InvokeScriptFunction(onload_handler_, url.spec());
  }

  void OnLoadFailed(int error_code, const std::string& url) {
    HRESULT hr = InvokeScriptFunction(onerror_handler_, url);
  }

  void OnMessageFromChromeFrame(const std::string& message,
                                const std::string& origin,
                                const std::string& target) {
    base::win::ScopedComPtr<IDispatch> message_event;
    if (SUCCEEDED(CreateDomEvent("message", message, origin,
                                 message_event.Receive()))) {
      base::win::ScopedVariant event_var;
      event_var.Set(static_cast<IDispatch*>(message_event));
      InvokeScriptFunction(onmessage_handler_, event_var.AsInput());
    }
  }

  virtual void OnTabbedOut(bool reverse) {
    DCHECK(m_bInPlaceActive);

    HWND parent = ::GetParent(m_hWnd);
    ::SetFocus(parent);
    base::win::ScopedComPtr<IOleControlSite> control_site;
    control_site.QueryFrom(m_spClientSite);
    if (control_site)
      control_site->OnFocus(FALSE);
  }

  virtual void OnOpenURL(const GURL& url_to_open,
                         const GURL& referrer, int open_disposition) {
    HostNavigate(url_to_open, referrer, open_disposition);
  }

  
  
  
  
  
  LPARAM OnDownloadRequestInHost(UINT message, WPARAM wparam, LPARAM lparam,
                                 BOOL& handled) {
    ChromeFrameUrl cf_url;
    cf_url.Parse(UTF8ToWide(GetDocumentUrl()));

    
    
    
    
    VARIANT flags = { VT_I4 };
    V_I4(&flags) = navNoHistory;
    if (!cf_url.attach_to_external_tab())
      V_I4(&flags) |= navOpenInNewWindow;

    DownloadInHostParams* download_params =
        reinterpret_cast<DownloadInHostParams*>(wparam);
    DCHECK(download_params);
    
    
    
    
    if (download_params->moniker) {
      NavigateBrowserToMoniker(
          doc_site_, download_params->moniker,
          UTF8ToWide(download_params->request_headers).c_str(),
          download_params->bind_ctx, NULL, download_params->post_data,
          &flags);
    }
    delete download_params;
    return TRUE;
  }

  virtual void OnAttachExternalTab(const AttachExternalTabParams& params) {
    GURL current_url(static_cast<BSTR>(url_));
    std::string url = chrome_frame::ActiveXCreateUrl(current_url, params);
    
    HostNavigate(GURL(url), current_url, chrome_frame::GetDisposition(params));
  }

  virtual void OnHandleContextMenu(const ContextMenuModel& menu_model,
                                   int align_flags,
                                   const MiniContextMenuParams& params) {
    scoped_refptr<BasePlugin> ref(this);
    ChromeFramePlugin<T>::OnHandleContextMenu(menu_model, align_flags, params);
  }

  LRESULT OnCreate(UINT message, WPARAM wparam, LPARAM lparam,
                   BOOL& handled) {  
    ModifyStyle(0, WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0);
    url_fetcher_->put_notification_window(m_hWnd);
    if (automation_client_.get()) {
      automation_client_->SetParentWindow(m_hWnd);
    } else {
      NOTREACHED() << "No automation server";
      return -1;
    }
    
    if (ready_state_ < READYSTATE_INTERACTIVE) {
      ready_state_ = READYSTATE_INTERACTIVE;
      FireOnChanged(DISPID_READYSTATE);
    }
    return 0;
  }

  LRESULT OnDestroy(UINT message, WPARAM wparam, LPARAM lparam,
                    BOOL& handled) {  
    DVLOG(1) << __FUNCTION__;
    return 0;
  }

  
  virtual void OnAutomationServerReady() {
    draw_sad_tab_ = false;
    ChromeFramePlugin<T>::OnAutomationServerReady();

    ready_state_ = READYSTATE_COMPLETE;
    FireOnChanged(DISPID_READYSTATE);
  }

  
  virtual void OnAutomationServerLaunchFailed(
      AutomationLaunchResult reason, const std::string& server_version) {
    DVLOG(1) << __FUNCTION__;
    if (reason == AUTOMATION_SERVER_CRASHED)
      draw_sad_tab_ = true;

    ready_state_ = READYSTATE_UNINITIALIZED;
    FireOnChanged(DISPID_READYSTATE);
  }

  virtual void OnCloseTab() {
    Fire_onclose();
  }

  
  
  HRESULT FireOnChanged(DISPID dispid) {
    if (dispid == DISPID_READYSTATE) {
      Fire_onreadystatechanged(ready_state_);
    }
    return __super::FireOnChanged(dispid);
  }

  
  
  
  
  STDMETHOD(get_src)(BSTR* src) {
    if (NULL == src) {
      return E_POINTER;
    }

    *src = SysAllocString(url_);
    return S_OK;
  }

  STDMETHOD(put_src)(BSTR src) {
    if (src == NULL)
      return E_INVALIDARG;

    
    std::string src_utf8;
    WideToUTF8(src, SysStringLen(src), &src_utf8);
    std::string full_url = ResolveURL(GetDocumentUrl(), src_utf8);

    
    
    
    if (!automation_client_->InitiateNavigation(full_url,
                                                GetDocumentUrl(),
                                                this)) {
      
      
      return E_INVALIDARG;
    }

    
    url_.Reset(::SysAllocString(UTF8ToWide(full_url).c_str()));

    return S_OK;
  }

  STDMETHOD(get_onload)(VARIANT* onload_handler) {
    if (NULL == onload_handler)
      return E_INVALIDARG;

    *onload_handler = onload_handler_.Copy();

    return S_OK;
  }

  
  
  STDMETHOD(put_onload)(VARIANT onload_handler) {
    if (V_VT(&onload_handler) != VT_DISPATCH) {
      DLOG(WARNING) << "Invalid onload handler type: "
                    << onload_handler.vt
                    << " specified";
      return E_INVALIDARG;
    }

    onload_handler_ = onload_handler;

    return S_OK;
  }

  
  
  STDMETHOD(get_onloaderror)(VARIANT* onerror_handler) {
    if (NULL == onerror_handler)
      return E_INVALIDARG;

    *onerror_handler = onerror_handler_.Copy();

    return S_OK;
  }

  STDMETHOD(put_onloaderror)(VARIANT onerror_handler) {
    if (V_VT(&onerror_handler) != VT_DISPATCH) {
      DLOG(WARNING) << "Invalid onloaderror handler type: "
                    << onerror_handler.vt
                    << " specified";
      return E_INVALIDARG;
    }

    onerror_handler_ = onerror_handler;

    return S_OK;
  }

  
  
  
  STDMETHOD(put_onmessage)(VARIANT onmessage_handler) {
    if (V_VT(&onmessage_handler) != VT_DISPATCH) {
      DLOG(WARNING) << "Invalid onmessage handler type: "
                    << onmessage_handler.vt
                    << " specified";
      return E_INVALIDARG;
    }

    onmessage_handler_ = onmessage_handler;

    return S_OK;
  }

  STDMETHOD(get_onmessage)(VARIANT* onmessage_handler) {
    if (NULL == onmessage_handler)
      return E_INVALIDARG;

    *onmessage_handler = onmessage_handler_.Copy();

    return S_OK;
  }

  STDMETHOD(get_readyState)(long* ready_state) {  
    DVLOG(1) << __FUNCTION__;
    DCHECK(ready_state);

    if (!ready_state)
      return E_INVALIDARG;

    *ready_state = ready_state_;

    return S_OK;
  }

  
  
  
  STDMETHOD(get_useChromeNetwork)(VARIANT_BOOL* use_chrome_network) {
    if (!use_chrome_network)
      return E_INVALIDARG;

    *use_chrome_network =
        automation_client_->use_chrome_network() ? VARIANT_TRUE : VARIANT_FALSE;
    return S_OK;
  }

  STDMETHOD(put_useChromeNetwork)(VARIANT_BOOL use_chrome_network) {
    if (!is_privileged()) {
      DLOG(ERROR) << "Attempt to set useChromeNetwork in non-privileged mode";
      return E_ACCESSDENIED;
    }

    automation_client_->set_use_chrome_network(
        (VARIANT_FALSE != use_chrome_network));
    return S_OK;
  }

  
  STDMETHOD(postMessage)(BSTR message, VARIANT target) {
    if (NULL == message) {
      return E_INVALIDARG;
    }

    if (!automation_client_.get())
      return E_FAIL;

    std::string utf8_target;
    if (target.vt == VT_BSTR) {
      int len = ::SysStringLen(target.bstrVal);
      if (len == 1 && target.bstrVal[0] == L'*') {
        utf8_target = "*";
      } else {
        GURL resolved(target.bstrVal);
        if (!resolved.is_valid()) {
          Error(L"Unable to parse the specified target URL.");
          return E_INVALIDARG;
        }

        utf8_target = resolved.spec();
      }
    } else {
      utf8_target = "*";
    }

    std::string utf8_message;
    WideToUTF8(message, ::SysStringLen(message), &utf8_message);

    GURL url(GURL(document_url_).GetOrigin());
    std::string origin(url.is_empty() ? "null" : url.spec());
    if (!automation_client_->ForwardMessageFromExternalHost(utf8_message,
                                                            origin,
                                                            utf8_target)) {
      Error(L"Failed to post message to chrome frame");
      return E_FAIL;
    }

    return S_OK;
  }

  STDMETHOD(addEventListener)(BSTR event_type, IDispatch* listener,
                              VARIANT use_capture) {
    EventHandlers* handlers = NULL;
    HRESULT hr = GetHandlersForEvent(event_type, &handlers);
    if (FAILED(hr))
      return hr;

    DCHECK(handlers != NULL);

    handlers->insert(base::win::ScopedComPtr<IDispatch>(listener));

    return hr;
  }

  STDMETHOD(removeEventListener)(BSTR event_type, IDispatch* listener,
                                 VARIANT use_capture) {
    EventHandlers* handlers = NULL;
    HRESULT hr = GetHandlersForEvent(event_type, &handlers);
    if (FAILED(hr))
      return hr;

    DCHECK(handlers != NULL);
    handlers->erase(base::win::ScopedComPtr<IDispatch>(listener));

    return hr;
  }

  STDMETHOD(get_version)(BSTR* version) {
    if (!automation_client_.get()) {
      NOTREACHED();
      return E_FAIL;
    }

    if (version == NULL) {
      return E_INVALIDARG;
    }

    *version = SysAllocString(automation_client_->GetVersion().c_str());
    return S_OK;
  }

  STDMETHOD(postPrivateMessage)(BSTR message, BSTR origin, BSTR target) {
    if (NULL == message)
      return E_INVALIDARG;

    if (!is_privileged()) {
      DLOG(ERROR) << "Attempt to postPrivateMessage in non-privileged mode";
      return E_ACCESSDENIED;
    }

    DCHECK(automation_client_.get());
    std::string utf8_message, utf8_origin, utf8_target;
    WideToUTF8(message, ::SysStringLen(message), &utf8_message);
    WideToUTF8(origin, ::SysStringLen(origin), &utf8_origin);
    WideToUTF8(target, ::SysStringLen(target), &utf8_target);

    if (!automation_client_->ForwardMessageFromExternalHost(utf8_message,
                                                            utf8_origin,
                                                            utf8_target)) {
      Error(L"Failed to post message to chrome frame");
      return E_FAIL;
    }

    return S_OK;
  }

  STDMETHOD(installExtension)(BSTR crx_path) {
    NOTREACHED();  
    return E_NOTIMPL;
  }

  STDMETHOD(loadExtension)(BSTR path) {
    NOTREACHED();  
    return E_NOTIMPL;
  }

  STDMETHOD(getEnabledExtensions)() {
    NOTREACHED();  
    return E_NOTIMPL;
  }

  STDMETHOD(registerBhoIfNeeded)() {
    return E_NOTIMPL;
  }

  
  
  
  HRESULT GetHandlersForEvent(BSTR event_type, EventHandlers** handlers) {
    DCHECK(handlers != NULL);

    
    HRESULT hr = S_OK;
    const wchar_t* event_type_end = event_type + ::SysStringLen(event_type);
    if (LowerCaseEqualsASCII(event_type, event_type_end, "message")) {
      *handlers = &onmessage_;
    } else if (LowerCaseEqualsASCII(event_type, event_type_end, "load")) {
      *handlers = &onload_;
    } else if (LowerCaseEqualsASCII(event_type, event_type_end, "loaderror")) {
      *handlers = &onloaderror_;
    } else if (LowerCaseEqualsASCII(event_type, event_type_end,
                                    "readystatechanged")) {
      *handlers = &onreadystatechanged_;
    } else if (LowerCaseEqualsASCII(event_type, event_type_end,
                                    "privatemessage")) {
      
      if (is_privileged()) {
        *handlers = &onprivatemessage_;
      } else {
        Error("Event type 'privatemessage' is privileged");
        hr = E_ACCESSDENIED;
      }
    } else if (LowerCaseEqualsASCII(event_type, event_type_end,
                                    "extensionready")) {
      
      if (is_privileged()) {
        *handlers = &onextensionready_;
      } else {
        Error("Event type 'extensionready' is privileged");
        hr = E_ACCESSDENIED;
      }
    } else {
      Error(base::StringPrintf(
          "Event type '%ls' not found", event_type).c_str());
      hr = E_INVALIDARG;
    }

    return hr;
  }

  
  
  
  HRESULT CreateDomEvent(const std::string& event_type, const std::string& data,
                         const std::string& origin, IDispatch** event) {
    DCHECK(event_type.length() > 0);  
    DCHECK(event != NULL);

    CComObject<ComMessageEvent>* ev = NULL;
    HRESULT hr = CComObject<ComMessageEvent>::CreateInstance(&ev);
    if (SUCCEEDED(hr)) {
      ev->AddRef();

      base::win::ScopedComPtr<IOleContainer> container;
      m_spClientSite->GetContainer(container.Receive());
      if (ev->Initialize(container, data, origin, event_type)) {
        *event = ev;
      } else {
        NOTREACHED() << "event->Initialize";
        ev->Release();
        hr = E_UNEXPECTED;
      }
    }

    return hr;
  }

  
  HRESULT InvokeScriptFunction(const VARIANT& script_object,
                               const std::string& param) {
    base::win::ScopedVariant script_arg(UTF8ToWide(param.c_str()).c_str());
    return InvokeScriptFunction(script_object, script_arg.AsInput());
  }

  HRESULT InvokeScriptFunction(const VARIANT& script_object, VARIANT* param) {
    return InvokeScriptFunction(script_object, param, 1);
  }

  HRESULT InvokeScriptFunction(const VARIANT& script_object, VARIANT* params,
                               int param_count) {
    DCHECK_GE(param_count, 0);
    DCHECK(params);

    if (V_VT(&script_object) != VT_DISPATCH ||
        script_object.pdispVal == NULL) {
      return S_FALSE;
    }

    CComPtr<IDispatch> script(script_object.pdispVal);
    HRESULT hr = script.InvokeN(static_cast<DISPID>(DISPID_VALUE),
                                params,
                                param_count);
    
    
    DLOG_IF(ERROR, FAILED(hr) && hr != 0x80020101) << "Failed to invoke script";
    return hr;
  }

  
  
  
  HRESULT AllowFrameToTranslateAccelerator(const MSG& msg) {
    static const int kMayTranslateAcceleratorOffset = 0x5c;
    
    
    
    
    
    
    
    
    
    
    
    MSG accel_message = msg;
    accel_message.hwnd = ::GetParent(m_hWnd);
    HRESULT hr = S_FALSE;
    base::win::ScopedComPtr<IBrowserService2> bs2;

    
    
    
    
    
    
    
    if (!in_place_frame_ && !failed_to_fetch_in_place_frame_) {
      base::win::ScopedComPtr<IOleInPlaceUIWindow> dummy_ui_window;
      RECT dummy_pos_rect = {0};
      RECT dummy_clip_rect = {0};
      OLEINPLACEFRAMEINFO dummy_frame_info = {0};
      if (!m_spInPlaceSite ||
          FAILED(m_spInPlaceSite->GetWindowContext(in_place_frame_.Receive(),
                                                   dummy_ui_window.Receive(),
                                                   &dummy_pos_rect,
                                                   &dummy_clip_rect,
                                                   &dummy_frame_info))) {
        failed_to_fetch_in_place_frame_ = true;
      }
    }

    
    
    
    
    
    if (in_place_frame_) {
      hr = in_place_frame_->TranslateAccelerator(&accel_message, 0);
    } else if (S_OK == DoQueryService(
        SID_STopLevelBrowser, m_spInPlaceSite,
        bs2.Receive()) && bs2.get() &&
        *(*(reinterpret_cast<void***>(bs2.get())) +
        kMayTranslateAcceleratorOffset)) {
      hr = bs2->v_MayTranslateAccelerator(&accel_message);
    } else {
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      
      ::PostMessage(accel_message.hwnd, accel_message.message,
                    accel_message.wParam, accel_message.lParam);
    }

    return hr;
  }

  virtual void OnAcceleratorPressed(const MSG& accel_message) {
    DCHECK(m_spInPlaceSite != NULL);
    
    
    
    
    HRESULT hr = TranslateAccelerator(const_cast<MSG*>(&accel_message));
    if (hr != S_OK)
      hr = AllowFrameToTranslateAccelerator(accel_message);

    DVLOG(1) << __FUNCTION__ << " browser response: "
             << base::StringPrintf("0x%08x", hr);

    if (hr != S_OK) {
      
      
      
      
      
      
      
      
      
      
      
      if (accel_message.message == WM_SYSCHAR ||
          accel_message.message == WM_SYSKEYDOWN ||
          accel_message.message == WM_SYSKEYUP) {
        ::PostMessage(GetParent(), accel_message.message, accel_message.wParam,
                      accel_message.lParam);
        return;
      }
    }
    
    
    
    
    if (hr != S_OK && automation_client_.get()) {
      TabProxy* tab = automation_client_->tab();
      if (tab) {
        tab->ProcessUnhandledAccelerator(accel_message);
      }
    }
  }

 protected:
  void HostNavigate(const GURL& url_to_open,
                    const GURL& referrer, int open_disposition) {
    base::win::ScopedComPtr<IWebBrowser2> web_browser2;
    DoQueryService(SID_SWebBrowserApp, m_spClientSite, web_browser2.Receive());
    if (!web_browser2) {
      NOTREACHED() << "Failed to retrieve IWebBrowser2 interface";
      return;
    }
    base::win::ScopedVariant url;
    
    
    
    if (url_to_open.has_scheme() &&
        (url_to_open.SchemeIs(content::kViewSourceScheme) ||
        url_to_open.SchemeIs(chrome::kAboutScheme))) {
      std::wstring chrome_url;
      chrome_url.append(kChromeProtocolPrefix);
      chrome_url.append(UTF8ToWide(url_to_open.spec()));
      url.Set(chrome_url.c_str());
      open_disposition = NEW_WINDOW;
    } else {
      url.Set(UTF8ToWide(url_to_open.spec()).c_str());
    }

    VARIANT flags = { VT_I4 };
    V_I4(&flags) = 0;

    IEVersion ie_version = GetIEVersion();
    DCHECK(ie_version != NON_IE && ie_version != IE_UNSUPPORTED);
    
    if (ie_version == IE_6) {
      if (open_disposition == NEW_FOREGROUND_TAB ||
          open_disposition == NEW_BACKGROUND_TAB ||
          open_disposition == NEW_WINDOW ||
          open_disposition == NEW_POPUP) {
        V_I4(&flags) = navOpenInNewWindow;
      } else if (open_disposition != CURRENT_TAB) {
        NOTREACHED() << "Unsupported open disposition in IE6";
      }
    } else {
      switch (open_disposition) {
        case NEW_FOREGROUND_TAB:
          V_I4(&flags) = navOpenInNewTab;
          break;
        case NEW_BACKGROUND_TAB:
          V_I4(&flags) = navOpenInBackgroundTab;
          break;
        case NEW_WINDOW:
        case NEW_POPUP:
          V_I4(&flags) = navOpenInNewWindow;
          break;
        default:
          break;
      }
    }

    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    VARIANT empty = base::win::ScopedVariant::kEmptyVariant;
    base::win::ScopedVariant http_headers;

    if (referrer.is_valid()) {
      std::wstring referrer_header = L"Referer: ";
      referrer_header += UTF8ToWide(referrer.spec());
      referrer_header += L"\r\n\r\n";
      http_headers.Set(referrer_header.c_str());
    }

    
    
    
    
    if (ie_version == IE_6) {
      ChromeFrameUrl cf_url;
      cf_url.Parse(static_cast<BSTR>(url_));

      if (cf_url.attach_to_external_tab() &&
          (cf_url.disposition() == NEW_FOREGROUND_TAB ||
           cf_url.disposition() == NEW_POPUP)) {
        BringWebBrowserWindowToTop(web_browser2);
      }
    }

    HRESULT hr = web_browser2->Navigate2(url.AsInput(), &flags, &empty, &empty,
                                         http_headers.AsInput());
    
    
    
    
    if (FAILED(hr) && V_I4(&flags) != navOpenInNewWindow) {
      V_I4(&flags) = navOpenInNewWindow;
      hr = web_browser2->Navigate2(url.AsInput(), &flags, &empty, &empty,
                                   http_headers.AsInput());
      DLOG_IF(ERROR, FAILED(hr))
          << "Navigate2 failed with error: "
          << base::StringPrintf("0x%08X", hr);
    }
  }

  void InitializeAutomationSettings() {
    static const wchar_t kHandleTopLevelRequests[] = L"HandleTopLevelRequests";
    static const wchar_t kUseChromeNetworking[] = L"UseChromeNetworking";

    
    
    bool top_level_requests = GetConfigBool(true, kHandleTopLevelRequests);
    bool chrome_network = GetConfigBool(false, kUseChromeNetworking);
    automation_client_->set_handle_top_level_requests(top_level_requests);
    automation_client_->set_use_chrome_network(chrome_network);
  }

  base::win::ScopedBstr url_;
  base::win::ScopedComPtr<IOleDocumentSite> doc_site_;

  
  
  bool failed_to_fetch_in_place_frame_;
  bool draw_sad_tab_;

  base::win::ScopedComPtr<IOleInPlaceFrame> in_place_frame_;

  
  
  READYSTATE ready_state_;

  
  
  base::win::ScopedVariant onload_handler_;
  base::win::ScopedVariant onerror_handler_;
  base::win::ScopedVariant onmessage_handler_;

  EventHandlers onmessage_;
  EventHandlers onloaderror_;
  EventHandlers onload_;
  EventHandlers onreadystatechanged_;
  EventHandlers onprivatemessage_;
  EventHandlers onextensionready_;

  
  
  scoped_ptr<UrlmonUrlRequestManager> url_fetcher_;
};

#endif  
