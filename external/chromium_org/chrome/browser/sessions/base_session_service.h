// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SESSIONS_BASE_SESSION_SERVICE_H_
#define CHROME_BROWSER_SESSIONS_BASE_SESSION_SERVICE_H_

#include "base/basictypes.h"
#include "base/callback.h"
#include "base/files/file_path.h"
#include "base/gtest_prod_util.h"
#include "base/location.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_vector.h"
#include "base/memory/weak_ptr.h"
#include "base/threading/sequenced_worker_pool.h"
#include "chrome/browser/common/cancelable_request.h"
#include "chrome/browser/sessions/session_id.h"
#include "chrome/common/cancelable_task_tracker.h"
#include "url/gurl.h"

class Profile;
class SessionBackend;
class SessionCommand;

namespace sessions {
class SerializedNavigationEntry;
}

class BaseSessionService : public CancelableRequestProvider {
 public:
  
  
  enum SessionType {
    SESSION_RESTORE,
    TAB_RESTORE
  };

  
  
  
  
  
  BaseSessionService(SessionType type,
                     Profile* profile,
                     const base::FilePath& path);

  Profile* profile() const { return profile_; }

  
  void DeleteLastSession();

  typedef base::Callback<void(ScopedVector<SessionCommand>)>
      InternalGetCommandsCallback;

 protected:
  virtual ~BaseSessionService();

  
  SessionBackend* backend() const { return backend_.get(); }

  
  
  
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
      const sessions::SerializedNavigationEntry& navigation);

  
  SessionCommand* CreateSetTabExtensionAppIDCommand(
      SessionID::id_type command_id,
      SessionID::id_type tab_id,
      const std::string& extension_id);

  
  
  SessionCommand* CreateSetTabUserAgentOverrideCommand(
      SessionID::id_type command_id,
      SessionID::id_type tab_id,
      const std::string& user_agent_override);

  
  SessionCommand* CreateSetWindowAppNameCommand(
      SessionID::id_type command_id,
      SessionID::id_type window_id,
      const std::string& app_name);

  
  
  
  
  bool RestoreUpdateTabNavigationCommand(
      const SessionCommand& command,
      sessions::SerializedNavigationEntry* navigation,
      SessionID::id_type* tab_id);

  
  
  
  bool RestoreSetTabExtensionAppIDCommand(
      const SessionCommand& command,
      SessionID::id_type* tab_id,
      std::string* extension_app_id);

  
  
  bool RestoreSetTabUserAgentOverrideCommand(
      const SessionCommand& command,
      SessionID::id_type* tab_id,
      std::string* user_agent_override);

  
  
  bool RestoreSetWindowAppNameCommand(
      const SessionCommand& command,
      SessionID::id_type* window_id,
      std::string* app_name);

  // Returns true if the entry at specified |url| should be written to disk.
  bool ShouldTrackEntry(const GURL& url);

  
  
  
  CancelableTaskTracker::TaskId ScheduleGetLastSessionCommands(
      const InternalGetCommandsCallback& callback,
      CancelableTaskTracker* tracker);

  
  
  bool RunTaskOnBackendThread(const tracked_objects::Location& from_here,
                              const base::Closure& task);

  
  static const int max_persist_navigation_count;

 private:
  friend class BetterSessionRestoreCrashTest;

  
  Profile* profile_;

  
  scoped_refptr<SessionBackend> backend_;

  
  base::WeakPtrFactory<BaseSessionService> weak_factory_;

  
  std::vector<SessionCommand*>  pending_commands_;

  
  
  bool pending_reset_;

  
  int commands_since_reset_;

  
  base::SequencedWorkerPool::SequenceToken sequence_token_;

  DISALLOW_COPY_AND_ASSIGN(BaseSessionService);
};

#endif  
