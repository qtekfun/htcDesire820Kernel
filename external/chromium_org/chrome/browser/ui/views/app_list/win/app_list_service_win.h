// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_VIEWS_APP_LIST_WIN_APP_LIST_SERVICE_WIN_H_
#define CHROME_BROWSER_UI_VIEWS_APP_LIST_WIN_APP_LIST_SERVICE_WIN_H_

#include "base/memory/scoped_ptr.h"
#include "base/memory/singleton.h"
#include "chrome/browser/ui/app_list/app_list_service_impl.h"

namespace app_list{
class AppListModel;
}

class AppListControllerDelegateWin;
class AppListShower;

class AppListServiceWin : public AppListServiceImpl {
 public:
  AppListServiceWin();
  virtual ~AppListServiceWin();

  static AppListServiceWin* GetInstance();
  void set_can_close(bool can_close);
  void OnAppListClosing();

  
  virtual void SetAppListNextPaintCallback(void (*callback)()) OVERRIDE;
  virtual void HandleFirstRun() OVERRIDE;
  virtual void Init(Profile* initial_profile) OVERRIDE;
  virtual void CreateForProfile(Profile* requested_profile) OVERRIDE;
  virtual void ShowForProfile(Profile* requested_profile) OVERRIDE;
  virtual void DismissAppList() OVERRIDE;
  virtual bool IsAppListVisible() const OVERRIDE;
  virtual gfx::NativeWindow GetAppListWindow() OVERRIDE;
  virtual Profile* GetCurrentAppListProfile() OVERRIDE;
  virtual AppListControllerDelegate* GetControllerDelegate() OVERRIDE;

  
  virtual void CreateShortcut() OVERRIDE;

 private:
  friend struct DefaultSingletonTraits<AppListServiceWin>;

  bool IsWarmupNeeded();
  void ScheduleWarmup();

  
  
  
  
  void LoadProfileForWarmup();
  void OnLoadProfileForWarmup(Profile* initial_profile);

  bool enable_app_list_on_next_init_;

  
  scoped_ptr<AppListShower> shower_;

  scoped_ptr<AppListControllerDelegateWin> controller_delegate_;

  base::WeakPtrFactory<AppListServiceWin> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(AppListServiceWin);
};

namespace chrome {

AppListService* GetAppListServiceWin();

}  

#endif  
