// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_PROCESSES_PROCESSES_API_H__
#define CHROME_BROWSER_EXTENSIONS_API_PROCESSES_PROCESSES_API_H__

#include <set>
#include <string>

#include "base/memory/scoped_ptr.h"
#include "chrome/browser/extensions/api/profile_keyed_api_factory.h"
#include "chrome/browser/extensions/chrome_extension_function.h"
#include "chrome/browser/task_manager/task_manager.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/notification_registrar.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/render_widget_host.h"
#include "extensions/browser/event_router.h"

class Profile;

namespace base {
class ListValue;
}

namespace extensions {

class ProcessesEventRouter : public TaskManagerModelObserver,
                             public content::NotificationObserver {
 public:
  explicit ProcessesEventRouter(Profile* profile);
  virtual ~ProcessesEventRouter();

  
  void ListenerAdded();

  
  void ListenerRemoved();

  
  
  
  void StartTaskManagerListening();

  bool is_task_manager_listening() { return task_manager_listening_; }

 private:
  
  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  virtual void OnItemsAdded(int start, int length) OVERRIDE;
  virtual void OnModelChanged() OVERRIDE {}
  virtual void OnItemsChanged(int start, int length) OVERRIDE;
  virtual void OnItemsRemoved(int start, int length) OVERRIDE {}
  virtual void OnItemsToBeRemoved(int start, int length) OVERRIDE;

  
  void ProcessHangEvent(content::RenderWidgetHost* widget);
  void ProcessClosedEvent(
      content::RenderProcessHost* rph,
      content::RenderProcessHost::RendererClosedDetails* details);

  void DispatchEvent(const std::string& event_name,
                     scoped_ptr<base::ListValue> event_args);

  
  
  bool HasEventListeners(const std::string& event_name);

  
  content::NotificationRegistrar registrar_;

  Profile* profile_;

  
  TaskManagerModel* model_;

  
  int listeners_;

  
  
  bool task_manager_listening_;

  DISALLOW_COPY_AND_ASSIGN(ProcessesEventRouter);
};

class ProcessesAPI : public ProfileKeyedAPI,
                     public EventRouter::Observer {
 public:
  explicit ProcessesAPI(Profile* profile);
  virtual ~ProcessesAPI();

  
  virtual void Shutdown() OVERRIDE;

  
  static ProfileKeyedAPIFactory<ProcessesAPI>* GetFactoryInstance();

  
  static ProcessesAPI* Get(Profile* profile);

  ProcessesEventRouter* processes_event_router();

  
  virtual void OnListenerAdded(const EventListenerInfo& details) OVERRIDE;
  virtual void OnListenerRemoved(const EventListenerInfo& details) OVERRIDE;

 private:
  friend class ProfileKeyedAPIFactory<ProcessesAPI>;

  Profile* profile_;

  
  static const char* service_name() {
    return "ProcessesAPI";
  }
  static const bool kServiceRedirectedInIncognito = true;
  static const bool kServiceIsNULLWhileTesting = true;

  
  scoped_ptr<ProcessesEventRouter> processes_event_router_;
};

class GetProcessIdForTabFunction : public ChromeAsyncExtensionFunction {
 public:
  GetProcessIdForTabFunction();

 private:
  virtual ~GetProcessIdForTabFunction() {}
  virtual bool RunImpl() OVERRIDE;

  void GetProcessIdForTab();

  
  int tab_id_;

  DECLARE_EXTENSION_FUNCTION("processes.getProcessIdForTab",
                             PROCESSES_GETPROCESSIDFORTAB)
};

class TerminateFunction : public ChromeAsyncExtensionFunction {
 public:
  TerminateFunction();

 private:
  virtual ~TerminateFunction() {}
  virtual bool RunImpl() OVERRIDE;


  void TerminateProcess();

  
  int process_id_;

  DECLARE_EXTENSION_FUNCTION("processes.terminate",
                             PROCESSES_TERMINATE)
};

class GetProcessInfoFunction : public ChromeAsyncExtensionFunction {
 public:
  GetProcessInfoFunction();

 private:
  virtual ~GetProcessInfoFunction();
  virtual bool RunImpl() OVERRIDE;

  void GatherProcessInfo();

  
  std::vector<int> process_ids_;
#if defined(ENABLE_TASK_MANAGER)
  bool memory_;
#endif

  DECLARE_EXTENSION_FUNCTION("processes.getProcessInfo",
                             PROCESSES_GETPROCESSINFO)
};

}  

#endif  
