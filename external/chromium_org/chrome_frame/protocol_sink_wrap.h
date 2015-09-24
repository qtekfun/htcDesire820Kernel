// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_PROTOCOL_SINK_WRAP_H_
#define CHROME_FRAME_PROTOCOL_SINK_WRAP_H_

#include <exdisp.h>
#include <urlmon.h>
#include <atlbase.h>
#include <atlcom.h>

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/win/scoped_comptr.h"
#include "base/win/scoped_bstr.h"
#include "chrome_frame/chrome_frame_delegate.h"
#include "chrome_frame/http_negotiate.h"
#include "chrome_frame/ie8_types.h"
#include "chrome_frame/utils.h"
#include "chrome_frame/vtable_patch_manager.h"

typedef HRESULT (STDMETHODCALLTYPE* InternetProtocol_Start_Fn)(
    IInternetProtocol* this_object, LPCWSTR url,
    IInternetProtocolSink* prot_sink, IInternetBindInfo* bind_info,
    DWORD flags, HANDLE_PTR reserved);
typedef HRESULT (STDMETHODCALLTYPE* InternetProtocol_Read_Fn)(
    IInternetProtocol* this_object, void* buffer, ULONG size,
    ULONG* size_read);
typedef HRESULT (STDMETHODCALLTYPE* InternetProtocol_StartEx_Fn)(
    IInternetProtocolEx* this_object, IUri* uri,
    IInternetProtocolSink* prot_sink, IInternetBindInfo* bind_info,
    DWORD flags, HANDLE_PTR reserved);
typedef HRESULT (STDMETHODCALLTYPE* InternetProtocol_LockRequest_Fn)(
    IInternetProtocol* this_object, DWORD options);
typedef HRESULT (STDMETHODCALLTYPE* InternetProtocol_UnlockRequest_Fn)(
    IInternetProtocol* this_object);
typedef HRESULT (STDMETHODCALLTYPE* InternetProtocol_Abort_Fn)(
    IInternetProtocol* this_object,  HRESULT hr, DWORD options);
typedef HRESULT (STDMETHODCALLTYPE* InternetProtocol_Terminate_Fn)(
    IInternetProtocol* this_object, DWORD options);

class ProtData;

class ProtocolSinkWrap
    : public CComObjectRootEx<CComMultiThreadModel>,
      public IInternetProtocolSink {
 public:
BEGIN_COM_MAP(ProtocolSinkWrap)
  COM_INTERFACE_ENTRY(IInternetProtocolSink)
  COM_INTERFACE_BLIND_DELEGATE()
END_COM_MAP()

  static base::win::ScopedComPtr<IInternetProtocolSink> CreateNewSink(
      IInternetProtocolSink* sink, ProtData* prot_data);

  
  
  static void set_ignore_xua(bool ignore_xua) { ignore_xua_ = ignore_xua; }
  static bool ignore_xua() { return ignore_xua_; }

  
  IInternetProtocolSink* delegate() {
    return delegate_;
  }

 protected:
  ProtocolSinkWrap();
  ~ProtocolSinkWrap();

 private:
  static bool ignore_xua_;

  
  STDMETHOD(Switch)(PROTOCOLDATA* protocol_data);
  STDMETHOD(ReportProgress)(ULONG status_code, LPCWSTR status_text);
  STDMETHOD(ReportData)(DWORD flags, ULONG progress, ULONG max_progress);
  STDMETHOD(ReportResult)(HRESULT result, DWORD error, LPCWSTR result_text);

  
  base::win::ScopedComPtr<IInternetProtocolSink> delegate_;
  base::win::ScopedComPtr<IServiceProvider> delegate_service_provider_;
  scoped_refptr<ProtData> prot_data_;
  DISALLOW_COPY_AND_ASSIGN(ProtocolSinkWrap);
};

class ProtData : public base::RefCounted<ProtData> {
 public:
  ProtData(IInternetProtocol* protocol, InternetProtocol_Read_Fn read_fun,
           const wchar_t* url);
  ~ProtData();
  HRESULT Read(void* buffer, ULONG size, ULONG* size_read);
  HRESULT ReportProgress(IInternetProtocolSink* delegate,
                         ULONG status_code,
                         LPCWSTR status_text);
  HRESULT ReportData(IInternetProtocolSink* delegate,
                     DWORD flags, ULONG progress, ULONG max_progress);
  HRESULT ReportResult(IInternetProtocolSink* delegate, HRESULT result,
                       DWORD error, LPCWSTR result_text);
  void UpdateUrl(const wchar_t* url);
  static scoped_refptr<ProtData> DataFromProtocol(IInternetProtocol* protocol);

  RendererType renderer_type() {
    return renderer_type_;
  }

  
  const std::string& referrer() const {
    return referrer_;
  }

  bool is_attach_external_tab_request() const {
    return read_fun_ == NULL;
  }

  
  void Invalidate();

  const std::wstring& url() const {
    return url_;
  }

 private:
  typedef std::map<IInternetProtocol*, ProtData*> ProtocolDataMap;
  static ProtocolDataMap datamap_;
  static base::Lock datamap_lock_;

  
  std::wstring url_;
  
  
  std::string referrer_;

  
  IInternetProtocol* protocol_;
  InternetProtocol_Read_Fn read_fun_;

  
  base::win::ScopedBstr suggested_mime_type_;
  
  
  
  
  bool has_suggested_mime_type_;
  
  bool has_server_mime_type_;

  RendererType renderer_type_;

  
  static const size_t kMaxContentSniffLength = 2 * 1024;
  char buffer_[kMaxContentSniffLength + 1];
  unsigned long buffer_size_;  
  unsigned long buffer_pos_;  

  HRESULT FillBuffer();
  void SaveSuggestedMimeType(LPCWSTR status_text);
  void FireSuggestedMimeType(IInternetProtocolSink* delegate);
  void SaveReferrer(IInternetProtocolSink* delegate);
};

struct TransactionHooks {
  void InstallHooks();
  void RevertHooks();
};

DECLSPEC_SELECTANY struct TransactionHooks g_trans_hooks;

#endif  
