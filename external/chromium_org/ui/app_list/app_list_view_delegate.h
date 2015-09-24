// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_APP_LIST_APP_LIST_VIEW_DELEGATE_H_
#define UI_APP_LIST_APP_LIST_VIEW_DELEGATE_H_

#include <string>
#include <vector>

#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/strings/string16.h"
#include "ui/app_list/app_list_export.h"

namespace base {
class FilePath;
}

namespace content {
class WebContents;
}

namespace gfx {
class ImageSkia;
}

namespace app_list {

class AppListItemModel;
class AppListModel;
class AppListViewDelegateObserver;
class SearchResult;
class SigninDelegate;
class SpeechUIModel;

class APP_LIST_EXPORT AppListViewDelegate {
 public:
  
  struct APP_LIST_EXPORT User {
    User();
    ~User();

    
    bool active;

    
    base::string16 name;

    
    base::string16 email;

    
    base::FilePath profile_path;
  };
  typedef std::vector<User> Users;

  
  virtual ~AppListViewDelegate() {}

  
  
  virtual bool ForceNativeDesktop() const = 0;

  
  
  virtual void SetProfileByPath(const base::FilePath& profile_path) = 0;

  
  
  virtual AppListModel* GetModel() = 0;

  
  virtual SigninDelegate* GetSigninDelegate() = 0;

  
  virtual SpeechUIModel* GetSpeechUI() = 0;

  
  
  virtual void GetShortcutPathForApp(
      const std::string& app_id,
      const base::Callback<void(const base::FilePath&)>& callback) = 0;

  
  
  
  virtual void StartSearch() = 0;

  
  virtual void StopSearch() = 0;

  
  virtual void OpenSearchResult(SearchResult* result, int event_flags) = 0;

  
  
  virtual void InvokeSearchResultAction(SearchResult* result,
                                        int action_index,
                                        int event_flags) = 0;

  
  
  virtual void Dismiss() = 0;

  
  virtual void ViewClosing() = 0;

  
  virtual gfx::ImageSkia GetWindowIcon() = 0;

  
  virtual void OpenSettings() = 0;

  
  virtual void OpenHelp() = 0;

  
  virtual void OpenFeedback() = 0;

  
  virtual void ToggleSpeechRecognition() = 0;

  
  virtual void ShowForProfileByPath(const base::FilePath& profile_path) = 0;

  
  virtual content::WebContents* GetStartPageContents() = 0;

  
  virtual const Users& GetUsers() const = 0;

  
  virtual void AddObserver(AppListViewDelegateObserver* observer) {}
  virtual void RemoveObserver(AppListViewDelegateObserver* observer) {}
};

}  

#endif  
