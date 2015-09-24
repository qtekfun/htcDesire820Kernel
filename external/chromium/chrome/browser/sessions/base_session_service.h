// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_BASE_SESSION_SERVICE_H_
#define CHROME_BROWSER_SESSIONS_BASE_SESSION_SERVICE_H_
#pragma once

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "chrome/browser/sessions/session_id.h"
#include "content/browser/cancelable_request.h"

class NavigationEntry;
class Profile;
class SessionBackend;
class SessionCommand;
class TabNavigation;

namespace base {
class Thread;
}

class BaseSessionService : public CancelableRequestProvider,
    public base::RefCountedThreadSafe<BaseSessionService> {
 public:
  
  
  enum SessionType {
    SESSION_RESTORE,
    TAB_RESTORE
  };

  
  
  
  
  
  BaseSessionService(SessionType type,
                     Profile* profile,
                     const FilePath& path);

  Profile* profile() const { return profile_; }

  
  void DeleteLastSession();

  class InternalGetCommandsRequest;

  typedef Callback2<Handle, scoped_refptr<InternalGetCommandsRequest> >::Type
      InternalGetCommandsCallback;

  
  
  class InternalGetCommandsRequest :
      public CancelableRequest<InternalGetCommandsCallback> {
   public:
    explicit InternalGetCommandsRequest(CallbackType* callback)
        : CancelableRequest<InternalGetCommandsCallback>(callback) {
    }

    
    std::vector<SessionCommand*> commands;

   protected:
    virtual ~InternalGetCommandsRequest();

   private:
    DISALLOW_COPY_AND_ASSIGN(InternalGetCommandsRequest);
  };

 protected:
  friend class base::RefCountedThreadSafe<BaseSessionService>;

  virtual ~BaseSessionService();

  
  SessionBackend* backend() const { return backend_; }

  
  base::Thread* backend_thread() const { return backend_thread_; }

  
  
  
  std::vector<SessionCommand*>&  pending_commands() {
    return pending_commands_;
  }

  
  void set_pending_reset(bool value) { pending_reset_ = value; }
  bool pending_reset() const { return pending_reset_; }

  
  int commands_since_reset() const { return commands_since_reset_; }

  
  
  
  virtual void ScheduleCommand(SessionCommand* command);

  
  void StartSaveTimer();

  
  
  virtual void Save();

  
  SessionCommand* CreateUpdateTabNavigationCommand(
      SessionID::id_type command_id,
      SessionID::id_type tab_id,
      int index,
      const NavigationEntry& entry);

  
  SessionCommand* CreateSetTabExtensionAppIDCommand(
      SessionID::id_type command_id,
      SessionID::id_type tab_id,
      const std::string& extension_id);

  
  
  
  bool RestoreUpdateTabNavigationCommand(const SessionCommand& command,
                                         TabNavigation* navigation,
                                         SessionID::id_type* tab_id);

  
  
  
  bool RestoreSetTabExtensionAppIDCommand(
      const SessionCommand& command,
      SessionID::id_type* tab_id,
      std::string* extension_app_id);

  // Returns true if the NavigationEntry should be written to disk.
  bool ShouldTrackEntry(const NavigationEntry& entry);

  // Returns true if the TabNavigationshould be written to disk.
  bool ShouldTrackEntry(const TabNavigation& navigation);

  
  
  Handle ScheduleGetLastSessionCommands(
      InternalGetCommandsRequest* request,
      CancelableRequestConsumerBase* consumer);

  
  
  Handle ScheduleGetCurrentSessionCommands(
      InternalGetCommandsRequest* request,
      CancelableRequestConsumerBase* consumer);

  
  static const int max_persist_navigation_count;

 private:
  
  Profile* profile_;

  
  const FilePath& path_;

  
  scoped_refptr<SessionBackend> backend_;

  
  base::Thread* backend_thread_;

  
  ScopedRunnableMethodFactory<BaseSessionService> save_factory_;

  
  std::vector<SessionCommand*>  pending_commands_;

  
  
  bool pending_reset_;

  
  int commands_since_reset_;

  DISALLOW_COPY_AND_ASSIGN(BaseSessionService);
};

#endif  
