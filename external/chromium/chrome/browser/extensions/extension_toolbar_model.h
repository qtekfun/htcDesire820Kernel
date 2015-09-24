// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_TOOLBAR_MODEL_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_TOOLBAR_MODEL_H_
#pragma once

#include "base/observer_list.h"
#include "chrome/common/extensions/extension.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class ExtensionService;
class PrefService;

class ExtensionToolbarModel : public NotificationObserver {
 public:
  explicit ExtensionToolbarModel(ExtensionService* service);
  ~ExtensionToolbarModel();

  
  
  void DestroyingProfile();

  
  
  class Observer {
   public:
    
    
    virtual void BrowserActionAdded(const Extension* extension, int index) {}

    
    virtual void BrowserActionRemoved(const Extension* extension) {}

    
    virtual void BrowserActionMoved(const Extension* extension, int index) {}

    
    virtual void ModelLoaded() {}

   protected:
    virtual ~Observer() {}
  };

  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);
  void MoveBrowserAction(const Extension* extension, int index);
  
  
  void SetVisibleIconCount(int count);
  
  int GetVisibleIconCount() { return visible_icon_count_; }

  bool extensions_initialized() const { return extensions_initialized_; }

  size_t size() const {
    return toolitems_.size();
  }

  ExtensionList::iterator begin() {
    return toolitems_.begin();
  }

  ExtensionList::iterator end() {
    return toolitems_.end();
  }

  const Extension* GetExtensionByIndex(int index) const;

  
  
  int IncognitoIndexToOriginal(int incognito_index);
  int OriginalIndexToIncognito(int original_index);

 private:
  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  
  void InitializeExtensionList();

  
  void UpdatePrefs();

  
  ObserverList<Observer> observers_;

  void AddExtension(const Extension* extension);
  void RemoveExtension(const Extension* extension);

  
  ExtensionService* service_;

  PrefService* prefs_;

  
  bool extensions_initialized_;

  
  ExtensionList toolitems_;

  
  std::string last_extension_removed_;

  
  size_t last_extension_removed_index_;

  
  
  int visible_icon_count_;

  NotificationRegistrar registrar_;
};

#endif  
