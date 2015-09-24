// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_ASH_MULTI_USER_MULTI_USER_WINDOW_MANAGER_H_
#define CHROME_BROWSER_UI_ASH_MULTI_USER_MULTI_USER_WINDOW_MANAGER_H_

#include <map>
#include <string>

class Browser;
class Profile;

namespace aura {
class Window;
}

namespace chrome {

class MultiUserWindowManagerChromeOS;

class MultiUserWindowManager {
 public:
  
  enum MultiProfileMode {
    MULTI_PROFILE_MODE_UNINITIALIZED,  
    MULTI_PROFILE_MODE_OFF,            
    MULTI_PROFILE_MODE_SEPARATED,      
    MULTI_PROFILE_MODE_MIXED           
  };

  
  
  
  static MultiUserWindowManager* CreateInstance();

  
  
  static MultiUserWindowManager* GetInstance();

  
  
  
  static MultiProfileMode GetMultiProfileMode();

  
  static void DeleteInstance();

  
  
  static void SetInstanceForTest(MultiUserWindowManager* instance,
                                 MultiProfileMode mode);

  
  
  
  
  
  virtual void SetWindowOwner(
      aura::Window* window, const std::string& user_id) = 0;

  
  
  virtual const std::string& GetWindowOwner(aura::Window* window) = 0;

  
  
  
  
  
  
  virtual void ShowWindowForUser(
      aura::Window* window, const std::string& user_id) = 0;

  
  virtual bool AreWindowsSharedAmongUsers() = 0;

  
  
  virtual bool IsWindowOnDesktopOfUser(aura::Window* window,
                                       const std::string& user_id) = 0;

  
  
  virtual const std::string& GetUserPresentingWindow(aura::Window* window) = 0;

  
  
  
  
  
  virtual void AddUser(Profile* profile) = 0;

 protected:
  virtual ~MultiUserWindowManager() {}

 private:
  
  static MultiProfileMode multi_user_mode_;
};

}  

#endif  
