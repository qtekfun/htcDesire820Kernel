// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef NET_COOKIES_COOKIE_MONSTER_STORE_TEST_H_
#define NET_COOKIES_COOKIE_MONSTER_STORE_TEST_H_

#include <map>
#include <string>
#include <utility>
#include <vector>
#include "net/cookies/canonical_cookie.h"
#include "net/cookies/cookie_monster.h"

namespace base {
class Time;
}

namespace net {

class LoadedCallbackTask
    : public base::RefCountedThreadSafe<LoadedCallbackTask> {
 public:
  typedef CookieMonster::PersistentCookieStore::LoadedCallback LoadedCallback;

  LoadedCallbackTask(LoadedCallback loaded_callback,
                     std::vector<CanonicalCookie*> cookies);

  void Run() {
    loaded_callback_.Run(cookies_);
  }

 private:
  friend class base::RefCountedThreadSafe<LoadedCallbackTask>;
  ~LoadedCallbackTask();

  LoadedCallback loaded_callback_;
  std::vector<CanonicalCookie*> cookies_;

  DISALLOW_COPY_AND_ASSIGN(LoadedCallbackTask);
};  

struct CookieStoreCommand {
  enum Type {
    ADD,
    UPDATE_ACCESS_TIME,
    REMOVE,
  };

  CookieStoreCommand(Type type, const CanonicalCookie& cookie)
      : type(type),
        cookie(cookie) {}

  Type type;
  CanonicalCookie cookie;
};

class MockPersistentCookieStore
    : public CookieMonster::PersistentCookieStore {
 public:
  typedef std::vector<CookieStoreCommand> CommandList;

  MockPersistentCookieStore();

  void SetLoadExpectation(
      bool return_value,
      const std::vector<CanonicalCookie*>& result);

  const CommandList& commands() const {
    return commands_;
  }

  virtual void Load(const LoadedCallback& loaded_callback) OVERRIDE;

  virtual void LoadCookiesForKey(const std::string& key,
    const LoadedCallback& loaded_callback) OVERRIDE;

  virtual void AddCookie(const CanonicalCookie& cookie) OVERRIDE;

  virtual void UpdateCookieAccessTime(
      const CanonicalCookie& cookie) OVERRIDE;

  virtual void DeleteCookie(
      const CanonicalCookie& cookie) OVERRIDE;

  virtual void Flush(const base::Closure& callback) OVERRIDE;

  virtual void SetForceKeepSessionState() OVERRIDE;

 protected:
  virtual ~MockPersistentCookieStore();

 private:
  CommandList commands_;

  
  bool load_return_value_;
  std::vector<CanonicalCookie*> load_result_;
  
  
  bool loaded_;

  DISALLOW_COPY_AND_ASSIGN(MockPersistentCookieStore);
};

class MockCookieMonsterDelegate : public CookieMonster::Delegate {
 public:
  typedef std::pair<CanonicalCookie, bool>
      CookieNotification;

  MockCookieMonsterDelegate();

  const std::vector<CookieNotification>& changes() const { return changes_; }

  void reset() { changes_.clear(); }

  virtual void OnCookieChanged(
      const CanonicalCookie& cookie,
      bool removed,
      CookieMonster::Delegate::ChangeCause cause) OVERRIDE;

 private:
  virtual ~MockCookieMonsterDelegate();

  std::vector<CookieNotification> changes_;

  DISALLOW_COPY_AND_ASSIGN(MockCookieMonsterDelegate);
};

CanonicalCookie BuildCanonicalCookie(const std::string& key,
                                     const std::string& cookie_line,
                                     const base::Time& creation_time);

void AddCookieToList(
    const std::string& key,
    const std::string& cookie_line,
    const base::Time& creation_time,
    std::vector<CanonicalCookie*>* out_list);

class MockSimplePersistentCookieStore
    : public CookieMonster::PersistentCookieStore {
 public:
  MockSimplePersistentCookieStore();

  virtual void Load(const LoadedCallback& loaded_callback) OVERRIDE;

  virtual void LoadCookiesForKey(const std::string& key,
      const LoadedCallback& loaded_callback) OVERRIDE;

  virtual void AddCookie(const CanonicalCookie& cookie) OVERRIDE;

  virtual void UpdateCookieAccessTime(const CanonicalCookie& cookie) OVERRIDE;

  virtual void DeleteCookie(const CanonicalCookie& cookie) OVERRIDE;

  virtual void Flush(const base::Closure& callback) OVERRIDE;

  virtual void SetForceKeepSessionState() OVERRIDE;

 protected:
  virtual ~MockSimplePersistentCookieStore();

 private:
  typedef std::map<int64, CanonicalCookie> CanonicalCookieMap;

  CanonicalCookieMap cookies_;

  
  
  bool loaded_;
};

CookieMonster* CreateMonsterFromStoreForGC(
    int num_cookies,
    int num_old_cookies,
    int days_old);

}  

#endif  
