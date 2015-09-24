// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_BROWSER_EVENT_ROUTER_H_
#define EXTENSIONS_BROWSER_EVENT_ROUTER_H_

#include <map>
#include <set>
#include <string>
#include <utility>

#include "base/callback.h"
#include "base/compiler_specific.h"
#include "base/containers/hash_tables.h"
#include "base/gtest_prod_util.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/ref_counted.h"
#include "base/values.h"
#include "content/public/browser/notification_observer.h"
#include "content/public/browser/notification_registrar.h"
#include "extensions/browser/event_listener_map.h"
#include "extensions/common/event_filtering_info.h"
#include "ipc/ipc_sender.h"

class GURL;
class PrefService;

namespace content {
class BrowserContext;
class RenderProcessHost;
}

namespace extensions {
class ActivityLog;
class Extension;
class ExtensionHost;
class ExtensionPrefs;

struct Event;
struct EventDispatchInfo;
struct EventListenerInfo;

class EventRouter : public content::NotificationObserver,
                    public EventListenerMap::Delegate {
 public:
  
  
  enum UserGestureState {
    USER_GESTURE_UNKNOWN = 0,
    USER_GESTURE_ENABLED = 1,
    USER_GESTURE_NOT_ENABLED = 2,
  };

  
  
  static const char kRegisteredEvents[];

  
  
  
  
  class Observer {
   public:
    
    virtual void OnListenerAdded(const EventListenerInfo& details) {}
    
    virtual void OnListenerRemoved(const EventListenerInfo& details) {}
  };

  
  
  class EventDispatchObserver {
   public:
    virtual void OnWillDispatchEvent(scoped_ptr<EventDispatchInfo> details) = 0;
  };

  
  
  static std::string GetBaseEventName(const std::string& full_event_name);

  
  
  static void DispatchEvent(IPC::Sender* ipc_sender,
                            void* browser_context_id,
                            const std::string& extension_id,
                            const std::string& event_name,
                            scoped_ptr<base::ListValue> event_args,
                            UserGestureState user_gesture,
                            const EventFilteringInfo& info);

  
  
  EventRouter(content::BrowserContext* browser_context,
              ExtensionPrefs* extension_prefs);
  virtual ~EventRouter();

  
  
  
  
  void AddEventListener(const std::string& event_name,
                        content::RenderProcessHost* process,
                        const std::string& extension_id);
  void RemoveEventListener(const std::string& event_name,
                           content::RenderProcessHost* process,
                           const std::string& extension_id);

  EventListenerMap& listeners() { return listeners_; }

  
  
  
  void RegisterObserver(Observer* observer,
                        const std::string& event_name);

  
  void UnregisterObserver(Observer* observer);

  
  
  void SetEventDispatchObserver(EventDispatchObserver* observer);

  
  
  
  
  void AddLazyEventListener(const std::string& event_name,
                            const std::string& extension_id);
  void RemoveLazyEventListener(const std::string& event_name,
                               const std::string& extension_id);

  
  void AddFilteredEventListener(const std::string& event_name,
                                content::RenderProcessHost* process,
                                const std::string& extension_id,
                                const base::DictionaryValue& filter,
                                bool add_lazy_listener);

  
  
  void RemoveFilteredEventListener(const std::string& event_name,
                                   content::RenderProcessHost* process,
                                   const std::string& extension_id,
                                   const base::DictionaryValue& filter,
                                   bool remove_lazy_listener);

  
  bool HasEventListener(const std::string& event_name);

  
  bool ExtensionHasEventListener(const std::string& extension_id,
                                 const std::string& event_name);

  
  
  std::set<std::string> GetRegisteredEvents(const std::string& extension_id);
  void SetRegisteredEvents(const std::string& extension_id,
                           const std::set<std::string>& events);

  
  virtual void BroadcastEvent(scoped_ptr<Event> event);

  
  virtual void DispatchEventToExtension(const std::string& extension_id,
                                        scoped_ptr<Event> event);

  
  
  
  
  void DispatchEventWithLazyListener(const std::string& extension_id,
                                     scoped_ptr<Event> event);

  
  void OnEventAck(content::BrowserContext* context,
                  const std::string& extension_id);

 private:
  FRIEND_TEST_ALL_PREFIXES(EventRouterTest, EventRouterObserver);

  
  
  struct ListenerProcess;

  
  
  typedef std::map<std::string, std::set<ListenerProcess> > ListenerMap;

  
  
  typedef std::pair<const content::BrowserContext*, std::string>
      EventDispatchIdentifier;

  
  
  static void NotifyExtensionDispatchObserverOnUIThread(
      void* browser_context_id,
      scoped_ptr<EventDispatchInfo> details);

  
  static void DispatchExtensionMessage(
      IPC::Sender* ipc_sender,
      void* browser_context_id,
      const std::string& extension_id,
      const std::string& event_name,
      base::ListValue* event_args,
      UserGestureState user_gesture,
      const extensions::EventFilteringInfo& info);

  virtual void Observe(int type,
                       const content::NotificationSource& source,
                       const content::NotificationDetails& details) OVERRIDE;

  
  
  
  bool HasEventListenerImpl(const ListenerMap& listeners,
                            const std::string& extension_id,
                            const std::string& event_name);

  
  
  
  void DispatchEventImpl(const std::string& restrict_to_extension_id,
                         const linked_ptr<Event>& event);

  
  
  
  
  void DispatchLazyEvent(const std::string& extension_id,
                         const linked_ptr<Event>& event,
                         std::set<EventDispatchIdentifier>* already_dispatched);

  
  void DispatchEventToProcess(const std::string& extension_id,
                              content::RenderProcessHost* process,
                              const linked_ptr<Event>& event);

  
  
  
  
  bool CanDispatchEventToBrowserContext(content::BrowserContext* context,
                                        const Extension* extension,
                                        const linked_ptr<Event>& event);

  
  
  
  bool MaybeLoadLazyBackgroundPageToDispatchEvent(
      content::BrowserContext* context,
      const Extension* extension,
      const linked_ptr<Event>& event);

  
  void AddFilterToEvent(const std::string& event_name,
                        const std::string& extension_id,
                        const base::DictionaryValue* filter);

  
  void RemoveFilterFromEvent(const std::string& event_name,
                             const std::string& extension_id,
                             const base::DictionaryValue* filter);

  
  
  const base::DictionaryValue* GetFilteredEvents(
      const std::string& extension_id);

  
  
  void IncrementInFlightEvents(content::BrowserContext* context,
                               const Extension* extension);

  
  static void IncrementInFlightEventsOnUI(
      void* browser_context_id,
      const std::string& extension_id);

  void DispatchPendingEvent(const linked_ptr<Event>& event,
                            ExtensionHost* host);

  
  virtual void OnListenerAdded(const EventListener* listener) OVERRIDE;
  virtual void OnListenerRemoved(const EventListener* listener) OVERRIDE;

  content::BrowserContext* browser_context_;

  
  
  ExtensionPrefs* extension_prefs_;

  content::NotificationRegistrar registrar_;

  EventListenerMap listeners_;

  
  typedef base::hash_map<std::string, Observer*> ObserverMap;
  ObserverMap observers_;

  EventDispatchObserver* event_dispatch_observer_;

  DISALLOW_COPY_AND_ASSIGN(EventRouter);
};

struct Event {
  typedef base::Callback<void(content::BrowserContext*,
                              const Extension*,
                              base::ListValue*)> WillDispatchCallback;

  
  std::string event_name;

  
  scoped_ptr<base::ListValue> event_args;

  
  
  
  content::BrowserContext* restrict_to_browser_context;

  
  
  GURL event_url;

  
  EventRouter::UserGestureState user_gesture;

  
  EventFilteringInfo filter_info;

  
  
  
  
  
  WillDispatchCallback will_dispatch_callback;

  Event(const std::string& event_name,
        scoped_ptr<base::ListValue> event_args);

  Event(const std::string& event_name,
        scoped_ptr<base::ListValue> event_args,
        content::BrowserContext* restrict_to_browser_context);

  Event(const std::string& event_name,
        scoped_ptr<base::ListValue> event_args,
        content::BrowserContext* restrict_to_browser_context,
        const GURL& event_url,
        EventRouter::UserGestureState user_gesture,
        const EventFilteringInfo& info);

  ~Event();

  
  
  Event* DeepCopy();
};

struct EventListenerInfo {
  EventListenerInfo(const std::string& event_name,
                    const std::string& extension_id,
                    content::BrowserContext* browser_context);
  
  
  const std::string event_name;

  const std::string extension_id;
  content::BrowserContext* browser_context;
};

struct EventDispatchInfo {
  EventDispatchInfo(const std::string& extension_id,
                    const std::string& event_name,
                    scoped_ptr<ListValue> event_args);
  ~EventDispatchInfo();

  const std::string extension_id;
  const std::string event_name;
  scoped_ptr<ListValue> event_args;
};

}  

#endif  
