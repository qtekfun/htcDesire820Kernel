// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_URLMON_MONIKER_H_
#define CHROME_FRAME_URLMON_MONIKER_H_

#include <atlbase.h>
#include <atlcom.h>
#include <urlmon.h>
#include <string>

#include "base/lazy_instance.h"
#include "base/logging.h"
#include "base/threading/thread_local.h"
#include "base/win/scoped_variant.h"
#include "chrome_frame/utils.h"








class NavigationManager {
 public:
  NavigationManager() {
  }

  
  
  static NavigationManager* GetThreadInstance();

  void RegisterThreadInstance();
  void UnregisterThreadInstance();

  virtual ~NavigationManager() {
    DCHECK(GetThreadInstance() != this);
  }

  
  const std::wstring& url() const {
    return url_;
  }

  
  void set_url(const wchar_t* url) {
    DVLOG(1) << __FUNCTION__ << " " << url;
    url_ = url;
  }

  
  const std::string& referrer() const {
    return referrer_;
  }

  void set_referrer(const std::string& referrer) {
    referrer_ = referrer;
  }

  
  
  virtual bool IsTopLevelUrl(const wchar_t* url);

  
  
  virtual HRESULT NavigateToCurrentUrlInCF(IBrowserService* browser);

  void set_post_data(VARIANT* post_data) {
    post_data_.Reset();
    if (post_data) {
      if (V_VT(post_data) == (VT_BYREF | VT_VARIANT)) {
        post_data_.Set(*post_data->pvarVal);
      } else {
        NOTREACHED() << "unexpected type for post_data: "
            << std::hex << post_data->vt;
      }
    }
  }

  const base::win::ScopedVariant& post_data() const {
    return post_data_;
  }

  void set_headers(VARIANT* headers) {
    headers_.Reset();
    if (headers) {
      headers_ = *headers;
    }
  }

  const base::win::ScopedVariant& headers() const {
    return headers_;
  }

 protected:
  std::string referrer_;
  std::wstring url_;
  base::win::ScopedVariant post_data_;
  base::win::ScopedVariant headers_;

  static base::LazyInstance<base::ThreadLocalPointer<NavigationManager> >
      thread_singleton_;

 private:
  DISALLOW_COPY_AND_ASSIGN(NavigationManager);
};

class MonikerPatch {
  MonikerPatch() {}  
 public:
  
  static bool Initialize();

  
  static void Uninitialize();

  
  typedef HRESULT (STDMETHODCALLTYPE* IMoniker_BindToObject_Fn)(IMoniker* me,
      IBindCtx* bind_ctx, IMoniker* to_left, REFIID iid, void** obj);
  typedef HRESULT (STDMETHODCALLTYPE* IMoniker_BindToStorage_Fn)(IMoniker* me,
      IBindCtx* bind_ctx, IMoniker* to_left, REFIID iid, void** obj);

  static STDMETHODIMP BindToObject(IMoniker_BindToObject_Fn original,
                                   IMoniker* me, IBindCtx* bind_ctx,
                                   IMoniker* to_left, REFIID iid, void** obj);

  static STDMETHODIMP BindToStorage(IMoniker_BindToStorage_Fn original,
                                    IMoniker* me, IBindCtx* bind_ctx,
                                    IMoniker* to_left, REFIID iid, void** obj);
};

extern wchar_t* kChromeRequestParam;

#endif  
