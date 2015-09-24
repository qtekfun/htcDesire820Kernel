// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CUSTOM_HANDLERS_PROTOCOL_HANDLER_REGISTRY_H_
#define CHROME_BROWSER_CUSTOM_HANDLERS_PROTOCOL_HANDLER_REGISTRY_H_

#include <map>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/sequenced_task_runner_helpers.h"
#include "base/values.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/shell_integration.h"
#include "chrome/common/custom_handlers/protocol_handler.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/notification_service.h"
#include "net/url_request/url_request.h"
#include "net/url_request/url_request_job.h"
#include "net/url_request/url_request_job_factory.h"

namespace user_prefs {
class PrefRegistrySyncable;
}

class ProtocolHandlerRegistry : public BrowserContextKeyedService {

 public:
  
  
  class DefaultClientObserver
      : public ShellIntegration::DefaultWebClientObserver {
   public:
    explicit DefaultClientObserver(ProtocolHandlerRegistry* registry);
    virtual ~DefaultClientObserver();

    
    
    virtual void SetDefaultWebClientUIState(
        ShellIntegration::DefaultWebClientUIState state) OVERRIDE;

    virtual bool IsInteractiveSetDefaultPermitted() OVERRIDE;

    
    
    void SetWorker(ShellIntegration::DefaultProtocolClientWorker* worker);

   protected:
    ShellIntegration::DefaultProtocolClientWorker* worker_;

   private:
    virtual bool IsOwnedByWorker() OVERRIDE;

    
    
    
    ProtocolHandlerRegistry* registry_;

    DISALLOW_COPY_AND_ASSIGN(DefaultClientObserver);
  };

  
  
  
  class Delegate {
   public:
    virtual ~Delegate();
    virtual void RegisterExternalHandler(const std::string& protocol);
    virtual void DeregisterExternalHandler(const std::string& protocol);
    virtual bool IsExternalHandlerRegistered(const std::string& protocol);
    virtual ShellIntegration::DefaultProtocolClientWorker* CreateShellWorker(
        ShellIntegration::DefaultWebClientObserver* observer,
        const std::string& protocol);
    virtual DefaultClientObserver* CreateShellObserver(
        ProtocolHandlerRegistry* registry);
    virtual void RegisterWithOSAsDefaultClient(
        const std::string& protocol,
        ProtocolHandlerRegistry* registry);
  };

  
  class IOThreadDelegate;

  
  
  
  
  
  
  class JobInterceptorFactory : public net::URLRequestJobFactory {
   public:
    
    explicit JobInterceptorFactory(IOThreadDelegate* io_thread_delegate);
    virtual ~JobInterceptorFactory();

    
    
    void Chain(scoped_ptr<net::URLRequestJobFactory> job_factory);

    
    virtual net::URLRequestJob* MaybeCreateJobWithProtocolHandler(
        const std::string& scheme,
        net::URLRequest* request,
        net::NetworkDelegate* network_delegate) const OVERRIDE;
    virtual bool IsHandledProtocol(const std::string& scheme) const OVERRIDE;
    virtual bool IsHandledURL(const GURL& url) const OVERRIDE;
    virtual bool IsSafeRedirectTarget(const GURL& location) const OVERRIDE;

   private:
    
    
    scoped_ptr<URLRequestJobFactory> job_factory_;
    
    
    scoped_refptr<IOThreadDelegate> io_thread_delegate_;

    DISALLOW_COPY_AND_ASSIGN(JobInterceptorFactory);
  };

  typedef std::map<std::string, ProtocolHandler> ProtocolHandlerMap;
  typedef std::vector<ProtocolHandler> ProtocolHandlerList;
  typedef std::map<std::string, ProtocolHandlerList> ProtocolHandlerMultiMap;
  typedef std::vector<DefaultClientObserver*> DefaultClientObserverList;

  
  ProtocolHandlerRegistry(Profile* profile, Delegate* delegate);
  virtual ~ProtocolHandlerRegistry();

  
  
  scoped_ptr<JobInterceptorFactory> CreateJobInterceptorFactory();

  
  
  
  
  bool SilentlyHandleRegisterHandlerRequest(const ProtocolHandler& handler);

  
  void OnAcceptRegisterProtocolHandler(const ProtocolHandler& handler);

  
  void OnDenyRegisterProtocolHandler(const ProtocolHandler& handler);

  
  
  void OnIgnoreRegisterProtocolHandler(const ProtocolHandler& handler);

  
  
  
  bool AttemptReplace(const ProtocolHandler& handler);

  
  
  ProtocolHandlerList GetReplacedHandlers(const ProtocolHandler& handler) const;

  
  void ClearDefault(const std::string& scheme);

  
  bool IsDefault(const ProtocolHandler& handler) const;

  
  
  
  void InitProtocolSettings();

  
  
  int GetHandlerIndex(const std::string& scheme) const;

  
  ProtocolHandlerList GetHandlersFor(const std::string& scheme) const;

  
  ProtocolHandlerList GetIgnoredHandlers();

  
  
  void GetRegisteredProtocols(std::vector<std::string>* output) const;

  
  
  bool CanSchemeBeOverridden(const std::string& scheme) const;

  
  bool IsRegistered(const ProtocolHandler& handler) const;

  
  bool IsIgnored(const ProtocolHandler& handler) const;

  
  bool HasRegisteredEquivalent(const ProtocolHandler& handler) const;

  
  bool HasIgnoredEquivalent(const ProtocolHandler& handler) const;

  
  void RemoveIgnoredHandler(const ProtocolHandler& handler);

  
  bool IsHandledProtocol(const std::string& scheme) const;

  
  void RemoveHandler(const ProtocolHandler& handler);

  
  void RemoveDefaultHandler(const std::string& scheme);

  
  
  const ProtocolHandler& GetHandlerFor(const std::string& scheme) const;

  
  
  void Enable();

  
  
  void Disable();

  
  
  virtual void Shutdown() OVERRIDE;

  
  static void RegisterProfilePrefs(user_prefs::PrefRegistrySyncable* registry);

  bool enabled() const { return enabled_; }

  
  
  void AddPredefinedHandler(const ProtocolHandler& handler);

 private:
  friend class base::DeleteHelper<ProtocolHandlerRegistry>;
  friend struct content::BrowserThread::DeleteOnThread<
      content::BrowserThread::IO>;

  
  friend class ProtocolHandlerRegistryFactory;

  friend class ProtocolHandlerRegistryTest;
  friend class RegisterProtocolHandlerBrowserTest;

  
  
  void PromoteHandler(const ProtocolHandler& handler);

  
  void Save();

  
  
  const ProtocolHandlerList* GetHandlerList(const std::string& scheme) const;

  
  
  void InstallDefaultsForChromeOS();

  
  void SetDefault(const ProtocolHandler& handler);

  
  void InsertHandler(const ProtocolHandler& handler);

  
  
  Value* EncodeRegisteredHandlers();

  
  
  Value* EncodeIgnoredHandlers();

  
  void NotifyChanged();

  
  void RegisterProtocolHandler(const ProtocolHandler& handler);

  
  
  std::vector<const DictionaryValue*> GetHandlersFromPref(
      const char* pref_name) const;

  
  void IgnoreProtocolHandler(const ProtocolHandler& handler);

  
  ProtocolHandlerMultiMap protocol_handlers_;

  
  ProtocolHandlerList ignored_protocol_handlers_;

  
  ProtocolHandlerMap default_handlers_;

  
  Profile* profile_;

  
  scoped_ptr<Delegate> delegate_;

  
  
  bool enabled_;

  
  bool is_loading_;

  
  
  bool is_loaded_;

  
  
  scoped_refptr<IOThreadDelegate> io_thread_delegate_;

  DefaultClientObserverList default_client_observers_;

  DISALLOW_COPY_AND_ASSIGN(ProtocolHandlerRegistry);
};
#endif  
