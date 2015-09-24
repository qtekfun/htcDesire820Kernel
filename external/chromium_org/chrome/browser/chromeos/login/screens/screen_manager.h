// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_SCREEN_MANAGER_H_
#define CHROME_BROWSER_CHROMEOS_LOGIN_SCREENS_SCREEN_MANAGER_H_

#include <map>
#include <stack>
#include <string>

#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "chrome/browser/ui/webui/chromeos/login/screen_manager_handler.h"

namespace chromeos {

class BaseScreen;
class OobeDisplay;
class ScreenContext;
class ScreenFactory;
class ScreenFlow;

class ScreenManager : public ScreenManagerHandler::Delegate {
 public:
  ScreenManager(ScreenFactory* factory,
                OobeDisplay* oobe_display,
                ScreenFlow* initial_flow);
  virtual ~ScreenManager();

  
  void WarmupScreen(const std::string& id,
                    ScreenContext* context);

  
  
  
  void ShowScreen(const std::string& id);

  
  
  
  void ShowScreenWithParameters(const std::string& id,
                                ScreenContext* context);

  
  void PopupScreen(const std::string& id);

  
  
  void PopupScreenWithParameters(const std::string& id,
                                 ScreenContext* context);

  
  void HidePopupScreen(const std::string& screen_id);

  std::string GetCurrentScreenId();

  
  void SetScreenFlow(ScreenFlow* flow);

 private:
  void ShowScreenImpl(const std::string& id,
                      ScreenContext* context,
                      bool isPopup);
  void TransitionScreen(const std::string& from_id,
                        const std::string& to_id);

  void TearDownTopmostScreen();

  void OnDisplayIsReady();

  BaseScreen* GetTopmostScreen();
  BaseScreen* FindOrCreateScreen(const std::string& id);

  
  
  template<typename A1>
  void CallOnScreen(const std::string& screen_name,
                    void (BaseScreen::*method)(A1 arg1),
                    A1 arg1) {
    ScreenMap::const_iterator it = existing_screens_.find(screen_name);
    if (it != existing_screens_.end()) {
      BaseScreen* screen = it->second.get();
      (screen->*method)(arg1);
    } else {
      NOTREACHED();
    }
  }

  
  virtual void OnButtonPressed(const std::string& screen_name,
                               const std::string& button_id) OVERRIDE;
  virtual void OnContextChanged(const std::string& screen_name,
                                const DictionaryValue* diff) OVERRIDE;

  typedef std::map<std::string, linked_ptr<BaseScreen> > ScreenMap;

  
  scoped_ptr<ScreenFactory> factory_;

  
  OobeDisplay* display_;

  
  scoped_ptr<ScreenFlow> flow_;

  base::WeakPtrFactory<ScreenManager> weak_factory_;

  
  ScreenMap existing_screens_;

  
  
  std::stack<std::string> screen_stack_;

  
  bool js_is_ready_;

  
  
  std::string start_screen_;
  scoped_ptr<ScreenContext> start_screen_params_;
  bool start_screen_popup_;

  DISALLOW_COPY_AND_ASSIGN(ScreenManager);
};

}  

#endif  
