// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_CHROME_FRAME_PLUGIN_H_
#define CHROME_FRAME_CHROME_FRAME_PLUGIN_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted.h"
#include "base/win/win_util.h"
#include "chrome_frame/chrome_frame_automation.h"
#include "chrome/common/chrome_paths.h"
#include "chrome/common/chrome_paths_internal.h"
#include "chrome_frame/simple_resource_loader.h"
#include "chrome_frame/navigation_constraints.h"
#include "chrome_frame/utils.h"
#include "grit/chromium_strings.h"

#define IDC_ABOUT_CHROME_FRAME 40018

void ChromeFramePluginGetParamsCoordinates(
    const MiniContextMenuParams& params,
    int* x,
    int* y);

template <typename T>
class ChromeFramePlugin
    : public ChromeFrameDelegateImpl,
      public NavigationConstraintsImpl  {
 public:
  ChromeFramePlugin() : ignore_setfocus_(false){
  }
  ~ChromeFramePlugin() {
    Uninitialize();
  }

BEGIN_MSG_MAP(T)
  MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
  MESSAGE_HANDLER(WM_SIZE, OnSize)
  MESSAGE_HANDLER(WM_PARENTNOTIFY, OnParentNotify)
END_MSG_MAP()

  bool Initialize() {
    DVLOG(1) << __FUNCTION__;
    DCHECK(!automation_client_.get());
    automation_client_ = CreateAutomationClient();
    if (!automation_client_.get()) {
      NOTREACHED() << "new ChromeFrameAutomationClient";
      return false;
    }

    return true;
  }

  void Uninitialize() {
    DVLOG(1) << __FUNCTION__;
    if (IsValid()) {
      automation_client_->Uninitialize();
      automation_client_ = NULL;
    }
  }

  bool InitializeAutomation(const std::wstring& profile_name,
                            bool incognito, bool is_widget_mode,
                            const GURL& url, const GURL& referrer,
                            bool route_all_top_level_navigations) {
    DCHECK(IsValid());
    DCHECK(launch_params_ == NULL);
    
    
    bool incognito_mode = !is_privileged() && incognito;
    base::FilePath profile_path;
    GetProfilePath(profile_name, &profile_path);
    
    
    
    base::FilePath actual_profile_name = profile_path.BaseName();
    launch_params_ = new ChromeFrameLaunchParams(url, referrer, profile_path,
        actual_profile_name.value(), SimpleResourceLoader::GetLanguage(),
        incognito_mode, is_widget_mode, route_all_top_level_navigations);
    return automation_client_->Initialize(this, launch_params_);
  }

  
  virtual WindowType GetWindow() const {
    return (static_cast<const T*>(this))->m_hWnd;
  }

  virtual void GetBounds(RECT* bounds) {
    if (bounds) {
      if (::IsWindow(GetWindow())) {
        (static_cast<T*>(this))->GetClientRect(bounds);
      }
    }
  }
  virtual std::string GetDocumentUrl() {
    return document_url_;
  }
  virtual void OnAutomationServerReady() {
  }

  virtual bool IsValid() const {
    return automation_client_.get() != NULL;
  }

  virtual void OnHostMoved() {
    if (IsValid())
      automation_client_->OnChromeFrameHostMoved();
  }

 protected:
  virtual void OnNavigationFailed(int error_code, const GURL& gurl) {
    OnLoadFailed(error_code, gurl.spec());
  }

  virtual void OnHandleContextMenu(const ContextMenuModel& menu_model,
                                   int align_flags,
                                   const MiniContextMenuParams& params) {
    if (!automation_client_.get()) {
      NOTREACHED();
      return;
    }

    HMENU menu = BuildContextMenu(menu_model);
    if (!menu)
      return;

    T* self = static_cast<T*>(this);
    if (self->PreProcessContextMenu(menu)) {
      
      
      ignore_setfocus_ = true;
      SetFocus(GetWindow());
      ignore_setfocus_ = false;
      UINT flags = align_flags | TPM_LEFTBUTTON | TPM_RETURNCMD | TPM_RECURSE;
      int x, y;
      ChromeFramePluginGetParamsCoordinates(params, &x, &y);
      UINT selected = TrackPopupMenuEx(menu, flags, x, y, GetWindow(), NULL);
      
      GiveFocusToChrome(false);
      if (IsValid() && selected != 0 &&
          !self->HandleContextMenuCommand(selected, params)) {
        automation_client_->SendContextMenuCommandToChromeFrame(selected);
      }
    }

    DestroyMenu(menu);
  }

  LRESULT OnSetFocus(UINT message, WPARAM wparam, LPARAM lparam,
                     BOOL& handled) {  
    if (!ignore_setfocus_ && IsValid()) {
      
      
      
      GiveFocusToChrome(false);
    }
    return 0;
  }

  LRESULT OnSize(UINT message, WPARAM wparam, LPARAM lparam,
                 BOOL& handled) {  
    handled = FALSE;
    
    if (IsValid())
      automation_client_->Resize(LOWORD(lparam), HIWORD(lparam),
                                 SWP_NOACTIVATE | SWP_NOZORDER);
    return 0;
  }

  LRESULT OnParentNotify(UINT message, WPARAM wparam, LPARAM lparam,
                         BOOL& handled) {  
    switch (LOWORD(wparam)) {
      case WM_LBUTTONDOWN:
      case WM_MBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_XBUTTONDOWN: {
        
        
        
        HWND focus = ::GetFocus();
        HWND plugin_window = GetWindow();

        
        
        
        
        if (focus != plugin_window &&
            ::GetAncestor(plugin_window, GA_ROOTOWNER) !=
                ::GetAncestor(focus, GA_ROOTOWNER)) {
          ignore_setfocus_ = true;
          SetFocus(plugin_window);
          ignore_setfocus_ = false;
        }
        break;
      }
    }

    return 0;
  }

  
  
  
  bool PreProcessContextMenu(HMENU menu) {
    
    AppendMenu(menu, MF_STRING, IDC_ABOUT_CHROME_FRAME,
               SimpleResourceLoader::Get(IDS_CHROME_FRAME_MENU_ABOUT).c_str());
    return true;
  }

  
  
  bool HandleContextMenuCommand(UINT cmd,
                                const MiniContextMenuParams& params) {
    return false;
  }

  
  virtual ChromeFrameAutomationClient* CreateAutomationClient() {
    return new ChromeFrameAutomationClient;
  }

  void GiveFocusToChrome(bool restore_focus_to_view) {
    if (IsValid()) {
      TabProxy* tab = automation_client_->tab();
      HWND chrome_window = automation_client_->tab_window();
      if (tab && ::IsWindow(chrome_window)) {
        DVLOG(1) << "Setting initial focus";
        tab->SetInitialFocus(base::win::IsShiftPressed(), restore_focus_to_view);
      }
    }
  }

  virtual void GetProfilePath(const std::wstring& profile_name,
                              base::FilePath* profile_path) {
    return GetChromeFrameProfilePath(profile_name, profile_path);
  }

 protected:
  
  scoped_refptr<ChromeFrameAutomationClient> automation_client_;

  
  scoped_refptr<ChromeFrameLaunchParams> launch_params_;

  
  std::string document_url_;

  
  
  
  bool ignore_setfocus_;
};

#endif  
