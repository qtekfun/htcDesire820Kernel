// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_WARNING_SERVICE_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_WARNING_SERVICE_H_

#include <set>
#include <string>
#include <vector>

#include "base/observer_list.h"
#include "base/threading/non_thread_safe.h"
#include "chrome/browser/extensions/extension_warning_set.h"
#include "chrome/common/extensions/extension_set.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"


class Profile;

namespace content {
class NotificationDetails;
class NotificationSource;
}

namespace extensions {

class ExtensionWarningService : public content::NotificationObserver,
                                public base::NonThreadSafe {
 public:
  class Observer {
   public:
    virtual void ExtensionWarningsChanged() = 0;
  };

  
  
  explicit ExtensionWarningService(Profile* profile);
  virtual ~ExtensionWarningService();

  
  
  void ClearWarnings(const std::set<ExtensionWarning::WarningType>& types);

  
  std::set<ExtensionWarning::WarningType> GetWarningTypesAffectingExtension(
      const std::string& extension_id) const;

  
  std::vector<std::string> GetWarningMessagesForExtension(
      const std::string& extension_id) const;

  const ExtensionWarningSet& warnings() const { return warnings_; }

  
  void AddWarnings(const ExtensionWarningSet& warnings);

  
  
  static void NotifyWarningsOnUI(void* profile_id,
                                 const ExtensionWarningSet& warnings);

  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

 private:
  void NotifyWarningsChanged();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  ExtensionWarningSet warnings_;

  content::NotificationRegistrar registrar_;

  Profile* profile_;

  ObserverList<Observer> observer_list_;
};

}  

#endif  
