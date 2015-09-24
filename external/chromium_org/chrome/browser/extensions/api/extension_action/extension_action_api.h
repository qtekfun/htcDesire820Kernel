// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_EXTENSION_ACTION_EXTENSION_ACTION_API_H_
#define CHROME_BROWSER_EXTENSIONS_API_EXTENSION_ACTION_EXTENSION_ACTION_API_H_

#include <string>

#include "base/memory/weak_ptr.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/extensions/extension_action.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"

namespace base {
class DictionaryValue;
}

namespace content {
class WebContents;
}

namespace extensions {
class ExtensionPrefs;
class TabHelper;

class ExtensionActionAPI : public ProfileKeyedAPI {
 public:
  explicit ExtensionActionAPI(Profile* profile);
  virtual ~ExtensionActionAPI();

  
  static ExtensionActionAPI* Get(Profile* profile);

  static bool GetBrowserActionVisibility(const ExtensionPrefs* prefs,
                                         const std::string& extension_id);
  static void SetBrowserActionVisibility(ExtensionPrefs* prefs,
                                         const std::string& extension_id,
                                         bool visible);

  
  static void PageActionExecuted(Profile* profile,
                                 const ExtensionAction& page_action,
                                 int tab_id,
                                 const std::string& url,
                                 int button);

  
  static void ScriptBadgeExecuted(Profile* profile,
                                  const ExtensionAction& script_badge,
                                  int tab_id);

  
  static void BrowserActionExecuted(Profile* profile,
                                    const ExtensionAction& browser_action,
                                    content::WebContents* web_contents);

  
  static ProfileKeyedAPIFactory<ExtensionActionAPI>* GetFactoryInstance();

 private:
  friend class ProfileKeyedAPIFactory<ExtensionActionAPI>;

  
  static void DispatchEventToExtension(Profile* profile,
                                       const std::string& extension_id,
                                       const std::string& event_name,
                                       scoped_ptr<base::ListValue> event_args);

  
  
  
  static void DispatchOldPageActionEvent(Profile* profile,
    const std::string& extension_id,
    const std::string& page_action_id,
    int tab_id,
    const std::string& url,
    int button);

  
  
  static void ExtensionActionExecuted(Profile* profile,
                                      const ExtensionAction& extension_action,
                                      content::WebContents* web_contents);

  
  static const char* service_name() { return "ExtensionActionAPI"; }

  DISALLOW_COPY_AND_ASSIGN(ExtensionActionAPI);
};

class ExtensionActionStorageManager
    : public content::NotificationObserver,
      public base::SupportsWeakPtr<ExtensionActionStorageManager> {
 public:
  explicit ExtensionActionStorageManager(Profile* profile);
  virtual ~ExtensionActionStorageManager();

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  void WriteToStorage(ExtensionAction* extension_action);
  void ReadFromStorage(
      const std::string& extension_id, scoped_ptr<base::Value> value);

  Profile* profile_;
  content::NotificationRegistrar registrar_;
};

class ExtensionActionFunction : public ChromeSyncExtensionFunction {
 public:
  static bool ParseCSSColorString(const std::string& color_string,
                                  SkColor* result);

 protected:
  ExtensionActionFunction();
  virtual ~ExtensionActionFunction();
  virtual bool RunImpl() OVERRIDE;
  virtual bool RunExtensionAction() = 0;

  bool ExtractDataFromArguments();
  void NotifyChange();
  void NotifyBrowserActionChange();
  void NotifyLocationBarChange();
  void NotifySystemIndicatorChange();
  bool SetVisible(bool visible);

  
  
  extensions::TabHelper& tab_helper() const;

  
  
  base::DictionaryValue* details_;

  
  
  int tab_id_;

  
  content::WebContents* contents_;

  
  ExtensionAction* extension_action_;
};


class ExtensionActionShowFunction : public ExtensionActionFunction {
 protected:
  virtual ~ExtensionActionShowFunction() {}
  virtual bool RunExtensionAction() OVERRIDE;
};

class ExtensionActionHideFunction : public ExtensionActionFunction {
 protected:
  virtual ~ExtensionActionHideFunction() {}
  virtual bool RunExtensionAction() OVERRIDE;
};

class ExtensionActionSetIconFunction : public ExtensionActionFunction {
 protected:
  virtual ~ExtensionActionSetIconFunction() {}
  virtual bool RunExtensionAction() OVERRIDE;
};

class ExtensionActionSetTitleFunction : public ExtensionActionFunction {
 protected:
  virtual ~ExtensionActionSetTitleFunction() {}
  virtual bool RunExtensionAction() OVERRIDE;
};

class ExtensionActionSetPopupFunction : public ExtensionActionFunction {
 protected:
  virtual ~ExtensionActionSetPopupFunction() {}
  virtual bool RunExtensionAction() OVERRIDE;
};

class ExtensionActionSetBadgeTextFunction : public ExtensionActionFunction {
 protected:
  virtual ~ExtensionActionSetBadgeTextFunction() {}
  virtual bool RunExtensionAction() OVERRIDE;
};

class ExtensionActionSetBadgeBackgroundColorFunction
    : public ExtensionActionFunction {
 protected:
  virtual ~ExtensionActionSetBadgeBackgroundColorFunction() {}
  virtual bool RunExtensionAction() OVERRIDE;
};

class ExtensionActionGetTitleFunction : public ExtensionActionFunction {
 protected:
  virtual ~ExtensionActionGetTitleFunction() {}
  virtual bool RunExtensionAction() OVERRIDE;
};

class ExtensionActionGetPopupFunction : public ExtensionActionFunction {
 protected:
  virtual ~ExtensionActionGetPopupFunction() {}
  virtual bool RunExtensionAction() OVERRIDE;
};

class ExtensionActionGetBadgeTextFunction : public ExtensionActionFunction {
 protected:
  virtual ~ExtensionActionGetBadgeTextFunction() {}
  virtual bool RunExtensionAction() OVERRIDE;
};

class ExtensionActionGetBadgeBackgroundColorFunction
    : public ExtensionActionFunction {
 protected:
  virtual ~ExtensionActionGetBadgeBackgroundColorFunction() {}
  virtual bool RunExtensionAction() OVERRIDE;
};


class BrowserActionSetIconFunction : public ExtensionActionSetIconFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.setIcon", BROWSERACTION_SETICON)

 protected:
  virtual ~BrowserActionSetIconFunction() {}
};

class BrowserActionSetTitleFunction : public ExtensionActionSetTitleFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.setTitle", BROWSERACTION_SETTITLE)

 protected:
  virtual ~BrowserActionSetTitleFunction() {}
};

class BrowserActionSetPopupFunction : public ExtensionActionSetPopupFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.setPopup", BROWSERACTION_SETPOPUP)

 protected:
  virtual ~BrowserActionSetPopupFunction() {}
};

class BrowserActionGetTitleFunction : public ExtensionActionGetTitleFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.getTitle", BROWSERACTION_GETTITLE)

 protected:
  virtual ~BrowserActionGetTitleFunction() {}
};

class BrowserActionGetPopupFunction : public ExtensionActionGetPopupFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.getPopup", BROWSERACTION_GETPOPUP)

 protected:
  virtual ~BrowserActionGetPopupFunction() {}
};

class BrowserActionSetBadgeTextFunction
    : public ExtensionActionSetBadgeTextFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.setBadgeText",
                             BROWSERACTION_SETBADGETEXT)

 protected:
  virtual ~BrowserActionSetBadgeTextFunction() {}
};

class BrowserActionSetBadgeBackgroundColorFunction
    : public ExtensionActionSetBadgeBackgroundColorFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.setBadgeBackgroundColor",
                             BROWSERACTION_SETBADGEBACKGROUNDCOLOR)

 protected:
  virtual ~BrowserActionSetBadgeBackgroundColorFunction() {}
};

class BrowserActionGetBadgeTextFunction
    : public ExtensionActionGetBadgeTextFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.getBadgeText",
                             BROWSERACTION_GETBADGETEXT)

 protected:
  virtual ~BrowserActionGetBadgeTextFunction() {}
};

class BrowserActionGetBadgeBackgroundColorFunction
    : public ExtensionActionGetBadgeBackgroundColorFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.getBadgeBackgroundColor",
                             BROWSERACTION_GETBADGEBACKGROUNDCOLOR)

 protected:
  virtual ~BrowserActionGetBadgeBackgroundColorFunction() {}
};

class BrowserActionEnableFunction : public ExtensionActionShowFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.enable", BROWSERACTION_ENABLE)

 protected:
  virtual ~BrowserActionEnableFunction() {}
};

class BrowserActionDisableFunction : public ExtensionActionHideFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.disable", BROWSERACTION_DISABLE)

 protected:
  virtual ~BrowserActionDisableFunction() {}
};

class BrowserActionOpenPopupFunction : public ChromeAsyncExtensionFunction,
                                       public content::NotificationObserver {
 public:
  DECLARE_EXTENSION_FUNCTION("browserAction.openPopup",
                             BROWSERACTION_OPEN_POPUP)
  BrowserActionOpenPopupFunction();

 private:
  virtual ~BrowserActionOpenPopupFunction() {}

  
  virtual bool RunImpl() OVERRIDE;

  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;
  void OpenPopupTimedOut();

  content::NotificationRegistrar registrar_;
  bool response_sent_;

  DISALLOW_COPY_AND_ASSIGN(BrowserActionOpenPopupFunction);
};


class ScriptBadgeSetPopupFunction : public ExtensionActionSetPopupFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("scriptBadge.setPopup", SCRIPTBADGE_SETPOPUP)

 protected:
  virtual ~ScriptBadgeSetPopupFunction() {}
};

class ScriptBadgeGetPopupFunction : public ExtensionActionGetPopupFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("scriptBadge.getPopup", SCRIPTBADGE_GETPOPUP)

 protected:
  virtual ~ScriptBadgeGetPopupFunction() {}
};

class ScriptBadgeGetAttentionFunction : public ExtensionActionFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("scriptBadge.getAttention",
                             SCRIPTBADGE_GETATTENTION)

  virtual bool RunExtensionAction() OVERRIDE;

 protected:
  virtual ~ScriptBadgeGetAttentionFunction();
};

}  


class PageActionShowFunction : public extensions::ExtensionActionShowFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("pageAction.show", PAGEACTION_SHOW)

 protected:
  virtual ~PageActionShowFunction() {}
};

class PageActionHideFunction : public extensions::ExtensionActionHideFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("pageAction.hide", PAGEACTION_HIDE)

 protected:
  virtual ~PageActionHideFunction() {}
};

class PageActionSetIconFunction
    : public extensions::ExtensionActionSetIconFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("pageAction.setIcon", PAGEACTION_SETICON)

 protected:
  virtual ~PageActionSetIconFunction() {}
};

class PageActionSetTitleFunction
    : public extensions::ExtensionActionSetTitleFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("pageAction.setTitle", PAGEACTION_SETTITLE)

 protected:
  virtual ~PageActionSetTitleFunction() {}
};

class PageActionSetPopupFunction
    : public extensions::ExtensionActionSetPopupFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("pageAction.setPopup", PAGEACTION_SETPOPUP)

 protected:
  virtual ~PageActionSetPopupFunction() {}
};

class PageActionGetTitleFunction
    : public extensions::ExtensionActionGetTitleFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("pageAction.getTitle", PAGEACTION_GETTITLE)

 protected:
  virtual ~PageActionGetTitleFunction() {}
};

class PageActionGetPopupFunction
    : public extensions::ExtensionActionGetPopupFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("pageAction.getPopup", PAGEACTION_GETPOPUP)

 protected:
  virtual ~PageActionGetPopupFunction() {}
};

class PageActionsFunction : public ChromeSyncExtensionFunction {
 protected:
  PageActionsFunction();
  virtual ~PageActionsFunction();
  bool SetPageActionEnabled(bool enable);
};

class EnablePageActionsFunction : public PageActionsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("pageActions.enableForTab",
                             PAGEACTIONS_ENABLEFORTAB)

 protected:
  virtual ~EnablePageActionsFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

class DisablePageActionsFunction : public PageActionsFunction {
 public:
  DECLARE_EXTENSION_FUNCTION("pageActions.disableForTab",
                             PAGEACTIONS_DISABLEFORTAB)

 protected:
  virtual ~DisablePageActionsFunction() {}

  
  virtual bool RunImpl() OVERRIDE;
};

#endif  
