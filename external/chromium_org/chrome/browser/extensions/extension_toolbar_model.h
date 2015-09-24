// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_TOOLBAR_MODEL_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_TOOLBAR_MODEL_H_

#include "base/compiler_specific.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "chrome/browser/extensions/extension_prefs.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/common/extension.h"

class Browser;
class ExtensionService;
class PrefService;
class Profile;

class ExtensionToolbarModel : public content::NotificationObserver,
                              public BrowserContextKeyedService   {
 public:
  ExtensionToolbarModel(Profile* profile,
                        extensions::ExtensionPrefs* extension_prefs);
  virtual ~ExtensionToolbarModel();

  
  enum Action {
    ACTION_NONE,
    ACTION_SHOW_POPUP,
    
    
    
  };

  
  
  class Observer {
   public:
    
    
    virtual void BrowserActionAdded(const extensions::Extension* extension,
                                    int index) {}

    
    virtual void BrowserActionRemoved(const extensions::Extension* extension) {}

    
    virtual void BrowserActionMoved(const extensions::Extension* extension,
                                    int index) {}

    
    
    virtual bool BrowserActionShowPopup(const extensions::Extension* extension);

    
    
    virtual void VisibleCountChanged() {}

   protected:
    virtual ~Observer() {}
  };

  
  static ExtensionToolbarModel* Get(Profile* profile);

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);
  void MoveBrowserAction(const extensions::Extension* extension, int index);
  
  
  
  
  
  
  
  Action ExecuteBrowserAction(const extensions::Extension* extension,
                              Browser* browser,
                              GURL* popup_url_out,
                              bool should_grant);
  
  
  void SetVisibleIconCount(int count);
  
  int GetVisibleIconCount() const { return visible_icon_count_; }

  bool extensions_initialized() const { return extensions_initialized_; }

  const extensions::ExtensionList& toolbar_items() const {
    return toolbar_items_;
  }

  
  
  int IncognitoIndexToOriginal(int incognito_index);
  int OriginalIndexToIncognito(int original_index);

  void OnExtensionToolbarPrefChange();

  
  
  bool ShowBrowserActionPopup(const extensions::Extension* extension);

  
  
  
  void EnsureVisibility(const extensions::ExtensionIdList& extension_ids);

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  void InitializeExtensionList(ExtensionService* service);
  void Populate(const extensions::ExtensionIdList& positions,
                ExtensionService* service);

  
  void FillExtensionList(const extensions::ExtensionIdList& order,
                         ExtensionService* service);

  
  void UpdatePrefs();

  
  
  size_t FindNewPositionFromLastKnownGood(
      const extensions::Extension* extension);

  
  ObserverList<Observer> observers_;

  void AddExtension(const extensions::Extension* extension);
  void RemoveExtension(const extensions::Extension* extension);
  void UninstalledExtension(const extensions::Extension* extension);

  
  Profile* profile_;

  extensions::ExtensionPrefs* extension_prefs_;
  PrefService* prefs_;

  
  bool extensions_initialized_;

  
  extensions::ExtensionList toolbar_items_;

  extensions::ExtensionIdList last_known_positions_;

  
  
  int visible_icon_count_;

  content::NotificationRegistrar registrar_;

  
  PrefChangeRegistrar pref_change_registrar_;
  base::Closure pref_change_callback_;

  base::WeakPtrFactory<ExtensionToolbarModel> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionToolbarModel);
};

#endif  
