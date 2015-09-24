// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_EXTENSION_HISTORY_API_H_
#define CHROME_BROWSER_EXTENSIONS_EXTENSION_HISTORY_API_H_
#pragma once

#include <map>
#include <string>

#include "base/memory/singleton.h"
#include "chrome/browser/extensions/extension_function.h"
#include "chrome/browser/history/history.h"
#include "chrome/browser/history/history_notifications.h"
#include "content/common/notification_registrar.h"

class ExtensionHistoryEventRouter : public NotificationObserver {
 public:
  
  static ExtensionHistoryEventRouter* GetInstance();

  
  void ObserveProfile(Profile* profile);

 private:
  friend struct DefaultSingletonTraits<ExtensionHistoryEventRouter>;

  ExtensionHistoryEventRouter();
  virtual ~ExtensionHistoryEventRouter();

  
  virtual void Observe(NotificationType type,
                       const NotificationSource& source,
                       const NotificationDetails& details);

  void HistoryUrlVisited(Profile* profile,
                         const history::URLVisitedDetails* details);

  void HistoryUrlsRemoved(Profile* profile,
                          const history::URLsDeletedDetails* details);

  void DispatchEvent(Profile* profile,
                     const char* event_name,
                     const std::string& json_args);

  
  NotificationRegistrar registrar_;

  
  typedef std::map<uintptr_t, Profile*> ProfileMap;
  ProfileMap profiles_;

  DISALLOW_COPY_AND_ASSIGN(ExtensionHistoryEventRouter);
};


class HistoryFunction : public AsyncExtensionFunction {
 public:
  virtual void Run();
  virtual bool RunImpl() = 0;

  bool GetUrlFromValue(Value* value, GURL* url);
  bool GetTimeFromValue(Value* value, base::Time* time);
};

class HistoryFunctionWithCallback : public HistoryFunction {
 public:
  HistoryFunctionWithCallback();
  ~HistoryFunctionWithCallback();

  
  virtual bool RunAsyncImpl() = 0;

  
  
  virtual void SendAsyncResponse();

  
  virtual bool RunImpl();

 protected:
  
  CancelableRequestConsumer cancelable_consumer_;

 private:
  
  
  void SendResponseToCallback();
};

class GetVisitsHistoryFunction : public HistoryFunctionWithCallback {
 public:
  
  virtual bool RunAsyncImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("history.getVisits");

  
  void QueryComplete(HistoryService::Handle request_service,
                     bool success,
                     const history::URLRow* url_row,
                     history::VisitVector* visits);
};

class SearchHistoryFunction : public HistoryFunctionWithCallback {
 public:
  virtual bool RunAsyncImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("history.search");

  
  void SearchComplete(HistoryService::Handle request_handle,
                      history::QueryResults* results);
};

class AddUrlHistoryFunction : public HistoryFunction {
 public:
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("history.addUrl");
};

class DeleteAllHistoryFunction : public HistoryFunctionWithCallback {
 public:
  virtual bool RunAsyncImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("history.deleteAll");

  
  void DeleteComplete();
};


class DeleteUrlHistoryFunction : public HistoryFunction {
 public:
  virtual bool RunImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("history.deleteUrl");
};

class DeleteRangeHistoryFunction : public HistoryFunctionWithCallback {
 public:
  virtual bool RunAsyncImpl();
  DECLARE_EXTENSION_FUNCTION_NAME("history.deleteRange");

  
  void DeleteComplete();
};

#endif  
