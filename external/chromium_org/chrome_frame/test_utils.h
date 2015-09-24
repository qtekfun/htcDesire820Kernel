// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TEST_UTILS_H_
#define CHROME_FRAME_TEST_UTILS_H_

#include <string>

#include <atlbase.h>
#include <atlcom.h>

#include "base/strings/string16.h"

namespace base {
class FilePath;
}

extern const wchar_t kChromeFrameDllName[];
extern const wchar_t kChromeLauncherExeName[];

class ScopedChromeFrameRegistrar {
 public:
  enum RegistrationType {
    PER_USER,
    SYSTEM_LEVEL,
  };

  explicit ScopedChromeFrameRegistrar(RegistrationType registration_type);
  ScopedChromeFrameRegistrar(const std::wstring& path,
                             RegistrationType registration_type);
  virtual ~ScopedChromeFrameRegistrar();

  void RegisterChromeFrameAtPath(const std::wstring& path);
  void UnegisterChromeFrameAtPath(const std::wstring& path);
  void RegisterReferenceChromeFrameBuild();

  std::wstring GetChromeFrameDllPath() const;

  static void RegisterAtPath(const std::wstring& path,
                             RegistrationType registration_type);
  static void UnregisterAtPath(const std::wstring& path,
                               RegistrationType registration_type);
  static void RegisterDefaults();
  static base::FilePath GetReferenceChromeFrameDllPath();

  
  
  
  
  
  
  static void RegisterAndExitProcessIfDirected();

 private:
  enum RegistrationOperation {
    REGISTER,
    UNREGISTER,
  };

  
  static const wchar_t kCallRegistrationEntrypointSwitch[];

  static void DoRegistration(const string16& path,
                             RegistrationType registration_type,
                             RegistrationOperation registration_operation);

  
  std::wstring new_chrome_frame_dll_path_;

  
  std::wstring original_dll_path_;

  
  RegistrationType registration_type_;
  
  
  static bool register_chrome_path_provider_;
};

base::FilePath GetChromeFrameBuildPath();

static _ATL_FUNC_INFO g_single_param = {CC_STDCALL, VT_EMPTY, 1, {VT_VARIANT}};
template <typename T>
class DispCallback
    : public IDispEventSimpleImpl<1, DispCallback<T>, &IID_IDispatch> {
 public:
  typedef HRESULT (T::*Method)(const VARIANT* param);

  DispCallback(T* owner, Method method) : owner_(owner), method_(method) {
  }

  BEGIN_SINK_MAP(DispCallback)
    SINK_ENTRY_INFO(1, IID_IDispatch, DISPID_VALUE, OnCallback, &g_single_param)
  END_SINK_MAP()

  virtual ULONG STDMETHODCALLTYPE AddRef() {
    return owner_->AddRef();
  }
  virtual ULONG STDMETHODCALLTYPE Release() {
    return owner_->Release();
  }

  STDMETHOD(OnCallback)(VARIANT param) {
    return (owner_->*method_)(&param);
  }

  IDispatch* ToDispatch() {
    return reinterpret_cast<IDispatch*>(this);
  }

  T* owner_;
  Method method_;
};

bool IsWorkstationLocked();

#endif  
