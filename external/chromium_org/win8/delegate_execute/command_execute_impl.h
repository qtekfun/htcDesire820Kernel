// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <ShObjIdl.h>
#include <WinInet.h>

#include <string>

#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/process/process_handle.h"
#include "win8/delegate_execute/resource.h"       

using namespace ATL;

EXTERN_C const GUID CLSID_CommandExecuteImpl;

class ATL_NO_VTABLE DECLSPEC_UUID("071BB5F2-85A4-424F-BFE7-5F1609BE4C2C")
    CommandExecuteImpl
    : public CComObjectRootEx<CComSingleThreadModel>,
      public CComCoClass<CommandExecuteImpl, &CLSID_CommandExecuteImpl>,
      public IExecuteCommand,
      public IObjectWithSiteImpl<CommandExecuteImpl>,
      public IInitializeCommand,
      public IObjectWithSelection,
      public IExecuteCommandApplicationHostEnvironment,
      public IForegroundTransfer {
 public:
  CommandExecuteImpl();

  DECLARE_REGISTRY_RESOURCEID(IDR_COMMANDEXECUTEIMPL)

  BEGIN_COM_MAP(CommandExecuteImpl)
    COM_INTERFACE_ENTRY(IExecuteCommand)
    COM_INTERFACE_ENTRY(IObjectWithSite)
    COM_INTERFACE_ENTRY(IInitializeCommand)
    COM_INTERFACE_ENTRY(IObjectWithSelection)
    COM_INTERFACE_ENTRY(IExecuteCommandApplicationHostEnvironment)
    COM_INTERFACE_ENTRY(IForegroundTransfer)
  END_COM_MAP()

  DECLARE_PROTECT_FINAL_CONSTRUCT()

  HRESULT FinalConstruct() {
    return S_OK;
  }

  void FinalRelease() {
  }

 public:
  
  STDMETHOD(SetKeyState)(DWORD key_state);
  STDMETHOD(SetParameters)(LPCWSTR params);
  STDMETHOD(SetPosition)(POINT pt);
  STDMETHOD(SetShowWindow)(int show);
  STDMETHOD(SetNoShowUI)(BOOL no_show_ui);
  STDMETHOD(SetDirectory)(LPCWSTR directory);
  STDMETHOD(Execute)(void);

  
  STDMETHOD(Initialize)(LPCWSTR name, IPropertyBag* bag);

  
  STDMETHOD(SetSelection)(IShellItemArray* item_array);
  STDMETHOD(GetSelection)(REFIID riid, void** selection);

  
  STDMETHOD(GetValue)(enum AHE_TYPE* pahe);

  
  STDMETHOD(AllowForegroundTransfer)(void* reserved);

 private:
  static bool FindChromeExe(base::FilePath* chrome_exe);

  static bool path_provider_initialized_;

  bool GetLaunchScheme(string16* display_name, INTERNET_SCHEME* scheme);
  HRESULT LaunchDesktopChrome();
  
  EC_HOST_UI_MODE GetLaunchMode();

  CComPtr<IShellItemArray> item_array_;
  CommandLine parameters_;
  base::FilePath chrome_exe_;
  STARTUPINFO start_info_;
  string16 verb_;
  string16 display_name_;
  INTERNET_SCHEME launch_scheme_;

  base::IntegrityLevel integrity_level_;
  EC_HOST_UI_MODE chrome_mode_;
};

OBJECT_ENTRY_AUTO(__uuidof(CommandExecuteImpl), CommandExecuteImpl)
