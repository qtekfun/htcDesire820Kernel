// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_ERROR_CONSOLE_ERROR_CONSOLE_H_
#define CHROME_BROWSER_EXTENSIONS_ERROR_CONSOLE_ERROR_CONSOLE_H_

#include <deque>
#include <map>

#include "base/gtest_prod_util.h"
#include "base/memory/scoped_ptr.h"
#include "base/observer_list.h"
#include "base/prefs/pref_change_registrar.h"
#include "base/strings/string16.h"
#include "base/threading/thread_checker.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/extension_error.h"

namespace content {
class NotificationDetails;
class NotificationSource;
class RenderViewHost;
}

class ExtensionService;
class Profile;

namespace extensions {
class ErrorConsoleUnitTest;
class Extension;

class ErrorConsole : public content::NotificationObserver {
 public:
  typedef std::deque<const ExtensionError*> ErrorList;
  typedef std::map<std::string, ErrorList> ErrorMap;

  class Observer {
   public:
    
    virtual void OnErrorAdded(const ExtensionError* error) = 0;

    
    
    virtual void OnErrorConsoleDestroyed();
  };

  explicit ErrorConsole(Profile* profile, ExtensionService* extension_service);
  virtual ~ErrorConsole();

  
  static ErrorConsole* Get(Profile* profile);

  
  void ReportError(scoped_ptr<ExtensionError> error);

  
  
  const ErrorList& GetErrorsForExtension(const std::string& extension_id) const;

  
  
  void AddObserver(Observer* observer);
  void RemoveObserver(Observer* observer);

  bool enabled() const { return enabled_; }
  const ErrorMap& errors() const { return errors_; }

 private:
  FRIEND_TEST_ALL_PREFIXES(ErrorConsoleUnitTest, AddAndRemoveErrors);

  
  
  void Enable(ExtensionService* extension_service);
  
  
  void Disable();

  
  
  
  void OnPrefChanged();

  
  void AddManifestErrorsForExtension(const Extension* extension);

  
  
  void RemoveIncognitoErrors();

  
  void RemoveErrorsForExtension(const std::string& extension_id);

  
  void RemoveAllErrors();

  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  bool enabled_;

  
  base::ThreadChecker thread_checker_;

  
  ObserverList<Observer> observers_;

  
  ErrorMap errors_;

  
  
  
  Profile* profile_;

  content::NotificationRegistrar notification_registrar_;
  PrefChangeRegistrar pref_registrar_;

  DISALLOW_COPY_AND_ASSIGN(ErrorConsole);
};

}  

#endif  
