// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_BACKGROUND_BACKGROUND_APPLICATION_LIST_MODEL_H_
#define CHROME_BROWSER_BACKGROUND_BACKGROUND_APPLICATION_LIST_MODEL_H_

#include <map>
#include <string>

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/common/extension.h"

class Profile;

namespace gfx {
class ImageSkia;
}

class BackgroundApplicationListModel : public content::NotificationObserver {
 public:
  
  
  
  
  
  class Observer {
   public:
    
    
    virtual void OnApplicationDataChanged(
        const extensions::Extension* extension,
        Profile* profile);

    
    
    virtual void OnApplicationListChanged(Profile* profile);

   protected:
    virtual ~Observer();
  };

  
  explicit BackgroundApplicationListModel(Profile* profile);

  virtual ~BackgroundApplicationListModel();

  
  void AddObserver(Observer* observer);

  
  
  
  
  
  
  
  
  const gfx::ImageSkia* GetIcon(const extensions::Extension* extension);

  
  int GetPosition(const extensions::Extension* extension) const;

  
  const extensions::Extension* GetExtension(int position) const;

  
  static bool IsBackgroundApp(const extensions::Extension& extension,
                              Profile* profile);

  
  void RemoveObserver(Observer* observer);

  extensions::ExtensionList::const_iterator begin() const {
    return extensions_.begin();
  }

  extensions::ExtensionList::const_iterator end() const {
    return extensions_.end();
  }

  size_t size() const {
    return extensions_.size();
  }

 private:
  
  
  class Application;

  
  typedef std::map<std::string, Application*> ApplicationMap;

  
  void AssociateApplicationData(const extensions::Extension* extension);

  
  void DissociateApplicationData(const extensions::Extension* extension);

  
  const Application* FindApplication(
      const extensions::Extension* extension) const;

  
  Application* FindApplication(const extensions::Extension* extension);

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  void SendApplicationDataChangedNotifications(
      const extensions::Extension* extension);

  
  
  void SendApplicationListChangedNotifications();

  
  void OnExtensionLoaded(const extensions::Extension* extension);

  
  void OnExtensionUnloaded(const extensions::Extension* extension);

  
  void OnExtensionPermissionsUpdated(
      const extensions::Extension* extension,
      extensions::UpdatedExtensionPermissionsInfo::Reason reason,
      const extensions::PermissionSet* permissions);

  
  void Update();

  
  
  
  
  static bool RequiresBackgroundModeForPushMessaging(
      const extensions::Extension& extension);

  ApplicationMap applications_;
  extensions::ExtensionList extensions_;
  ObserverList<Observer> observers_;
  Profile* profile_;
  content::NotificationRegistrar registrar_;

  DISALLOW_COPY_AND_ASSIGN(BackgroundApplicationListModel);
};

#endif  
