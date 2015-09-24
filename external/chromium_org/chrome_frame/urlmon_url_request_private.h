// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_URLMON_URL_REQUEST_PRIVATE_H_
#define CHROME_FRAME_URLMON_URL_REQUEST_PRIVATE_H_

#include <atlbase.h>
#include <atlcom.h>

#include <string>

#include "base/gtest_prod_util.h"
#include "base/threading/platform_thread.h"
#include "net/base/net_errors.h"
#include "net/http/http_response_headers.h"
#include "net/url_request/url_request_status.h"

class UrlmonUrlRequest
  : public CComObjectRootEx<CComMultiThreadModel>,
    public PluginUrlRequest,
    public IServiceProviderImpl<UrlmonUrlRequest>,
    public IBindStatusCallback,
    public IHttpNegotiate,
    public IAuthenticate,
    public IHttpSecurity {
 public:
  virtual bool Start();
  virtual void Stop();
  virtual bool Read(int bytes_to_read);

  
  HRESULT InitPending(const GURL& url, IMoniker* moniker, IBindCtx* bind_ctx,
                      bool enable_frame_busting, bool privileged_mode,
                      HWND notification_window, IStream* cache);

  
  
  
  typedef base::Callback<void(IMoniker*, IBindCtx*, IStream*, const char*)>
      TerminateBindCallback;
  void TerminateBind(const TerminateBindCallback& callback);

  
  void set_parent_window(HWND parent_window) {
    parent_window_ = parent_window;
  }

  
  
  void set_privileged_mode(bool privileged_mode) {
    privileged_mode_ = privileged_mode;
  }

  
  
  std::string me() const;

 protected:
  UrlmonUrlRequest();
  ~UrlmonUrlRequest();

  BEGIN_COM_MAP(UrlmonUrlRequest)
    COM_INTERFACE_ENTRY(IHttpNegotiate)
    COM_INTERFACE_ENTRY(IServiceProvider)
    COM_INTERFACE_ENTRY(IBindStatusCallback)
    COM_INTERFACE_ENTRY(IWindowForBindingUI)
    COM_INTERFACE_ENTRY(IAuthenticate)
    COM_INTERFACE_ENTRY(IHttpSecurity)
  END_COM_MAP()

  BEGIN_SERVICE_MAP(UrlmonUrlRequest)
    SERVICE_ENTRY(IID_IHttpNegotiate);
  END_SERVICE_MAP()

  
  STDMETHOD(OnStartBinding)(DWORD reserved, IBinding* binding);
  STDMETHOD(GetPriority)(LONG* priority);
  STDMETHOD(OnLowResource)(DWORD reserved);
  STDMETHOD(OnProgress)(ULONG progress, ULONG max_progress,
      ULONG status_code, LPCWSTR status_text);
  STDMETHOD(OnStopBinding)(HRESULT result, LPCWSTR error);
  STDMETHOD(GetBindInfo)(DWORD* bind_flags, BINDINFO* bind_info);
  STDMETHOD(OnDataAvailable)(DWORD flags, DWORD size, FORMATETC* formatetc,
      STGMEDIUM* storage);
  STDMETHOD(OnObjectAvailable)(REFIID iid, IUnknown* object);

  
  STDMETHOD(BeginningTransaction)(const wchar_t* url,
      const wchar_t* current_headers, DWORD reserved,
      wchar_t** additional_headers);
  STDMETHOD(OnResponse)(DWORD dwResponseCode, const wchar_t* response_headers,
      const wchar_t* request_headers, wchar_t** additional_headers);

  
  
  STDMETHOD(GetWindow)(REFGUID guid_reason, HWND* parent_window);

  
  
  STDMETHOD(Authenticate)(HWND* parent_window, LPWSTR* user_name,
      LPWSTR* password);

  
  STDMETHOD(OnSecurityProblem)(DWORD problem);

  void set_pending(bool pending) {
    pending_ = pending;
  }

  bool pending() const {
    return pending_;
  }

  bool terminate_requested() const {
    return !terminate_bind_callback_.is_null();
  }

  std::string response_headers() {
    return response_headers_;
  }

 protected:
  void ReleaseBindings();

  HRESULT StartAsyncDownload();
  void NotifyDelegateAndDie();
  void TerminateTransaction();
  static net::Error HresultToNetError(HRESULT hr);

 private:
  size_t SendDataToDelegate(size_t bytes);

  
  
  
  
  
  
  class Status {
   public:
    enum State {DONE, ABORTING, WORKING};
    struct Redirection {
      Redirection() : http_code(0) { }
      int http_code;
      std::string utf8_url;
    };

    Status() : state_(Status::DONE) {
    }

    State get_state() const {
      return state_;
    }

    
    void Start() {
      DCHECK_EQ(state_, DONE);
      state_ = WORKING;
    }

    
    
    void SetRedirected(int http_code, const std::string& utf8_url) {
      DCHECK_EQ(state_, WORKING);
      DCHECK_EQ(result_.status(), net::URLRequestStatus::SUCCESS);
      redirect_.utf8_url = utf8_url;

      
      
      redirect_.http_code = http_code;
      if (!net::HttpResponseHeaders::IsRedirectResponseCode(http_code))
        redirect_.http_code = 302;

      state_ = ABORTING;
    }

    
    
    void Cancel() {
      if (state_ == DONE)
        return;

      if (state_ == WORKING) {
        state_ =  ABORTING;
      } else {
        
        redirect_.http_code = 0;
        redirect_.utf8_url.clear();
      }

      set_result(net::URLRequestStatus::CANCELED, 0);
    }

    void Done() {
      state_ = DONE;
    }

    bool was_redirected() const {
      return redirect_.http_code != 0;
    }

    const Redirection& get_redirection() const {
      return redirect_;
    }

    const net::URLRequestStatus& get_result() const {
      return result_;
    }

    void set_result(net::URLRequestStatus::Status status, int error) {
      result_.set_status(status);
      result_.set_error(error);
    }

    void set_result(HRESULT hr) {
      result_.set_status(FAILED(hr)? net::URLRequestStatus::FAILED:
                                     net::URLRequestStatus::SUCCESS);
      result_.set_error(HresultToNetError(hr));
    }

   private:
    Redirection redirect_;
    State state_;
    net::URLRequestStatus result_;
  };

  Status status_;
  base::win::ScopedComPtr<IBinding> binding_;
  base::win::ScopedComPtr<IMoniker> moniker_;
  base::win::ScopedComPtr<IBindCtx> bind_context_;
  base::win::ScopedComPtr<IStream> cache_;
  base::win::ScopedComPtr<IStream> pending_data_;

  size_t pending_read_size_;
  base::PlatformThreadId thread_;
  HWND parent_window_;
  bool headers_received_;
  int calling_delegate_;  
  
  bool privileged_mode_;
  bool pending_;
  TerminateBindCallback terminate_bind_callback_;
  std::string response_headers_;
  
  
  
  bool is_expecting_download_;
  
  
  
  bool cleanup_transaction_;
  
  std::string request_headers_;

  DISALLOW_COPY_AND_ASSIGN(UrlmonUrlRequest);
};

#endif  
