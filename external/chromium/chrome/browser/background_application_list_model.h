// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BACKGROUND_APPLICATION_LIST_MODEL_H_
#define CHROME_BROWSER_BACKGROUND_APPLICATION_LIST_MODEL_H_
#pragma once

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "chrome/common/extensions/extension.h"
#include "content/common/notification_observer.h"
#include "content/common/notification_registrar.h"

class ExtensionService;
class PrefService;
class Profile;

class BackgroundApplicationListModel : public NotificationObserver {
 public:
  
  
  
  
  
  class Observer {
   public:
    
    
    virtual void OnApplicationDataChanged(const Extension* extension);

    
    
    virtual void OnApplicationListChanged();

   protected:
    virtual ~Observer();
  };

  
  explicit BackgroundApplicationListModel(Profile* profile);

  ~BackgroundApplicationListModel();

  
  void AddObserver(Observer* observer);

  
  
  
  
  
  
  
  const SkBitmap* GetIcon(const Extension* extension);

  
  int GetPosition(const Extension* extension) const;

  
  const Extension* GetExtension(int position) const;

  
  static bool IsBackgroundApp(const Extension& extension);

  
  void RemoveObserver(Observer* observer);

  ExtensionList::const_iterator begin() const {
    return extensions_.begin();
  }

  ExtensionList::const_iterator end() const {
    return extensions_.end();
  }

  size_t size() const {
    return extensions_.size();
  }

 private:
  
  
  class Application;

  
  typedef std::map<std::string, Application*> ApplicationMap;

  
  void AssociateApplicationData(const Extension* extension);

  
  void DissociateApplicationData(const Extension* extension);

  
  const Application* FindApplication(const Extension* extension) const;

  
  Application* FindApplication(const Extension* extension);

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  
  
  void OnApplicationDataChanged(const Extension* extension);

  
  
  void OnApplicationListChanged();

  
  void OnExtensionLoaded(Extension* extension);

  
  void OnExtensionUnloaded(const Extension* extension);

  
  void Update();

  ApplicationMap applications_;
  ExtensionList extensions_;
  ObserverList<Observer> observers_;
  Profile* profile_;
  NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundApplicationListModel);
};

#endif  
