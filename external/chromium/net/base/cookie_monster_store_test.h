// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include "net/base/cookie_monster.h"

namespace base {
class Time;
}

namespace net {

struct CookieStoreCommand {
  enum Type {
    ADD,
    UPDATE_ACCESS_TIME,
    REMOVE,
  };

  CookieStoreCommand(Type type,
                     const CookieMonster::CanonicalCookie& cookie)
      : type(type),
        cookie(cookie) {}

  Type type;
  CookieMonster::CanonicalCookie cookie;
};

class MockPersistentCookieStore
    : public CookieMonster::PersistentCookieStore {
 public:
  typedef std::vector<CookieStoreCommand> CommandList;

  MockPersistentCookieStore();
  virtual ~MockPersistentCookieStore();

  void SetLoadExpectation(
      bool return_value,
      const std::vector<CookieMonster::CanonicalCookie*>& result);

  const CommandList& commands() const {
    return commands_;
  }

  virtual bool Load(
      std::vector<CookieMonster::CanonicalCookie*>* out_cookies);

  virtual void AddCookie(const CookieMonster::CanonicalCookie& cookie);

  virtual void UpdateCookieAccessTime(
      const CookieMonster::CanonicalCookie& cookie);

  virtual void DeleteCookie(
      const CookieMonster::CanonicalCookie& cookie);

  virtual void Flush(Task* completion_task);

  
  virtual void SetClearLocalStateOnExit(bool clear_local_state);

 private:
  CommandList commands_;

  
  bool load_return_value_;
  std::vector<CookieMonster::CanonicalCookie*> load_result_;

  DISALLOW_COPY_AND_ASSIGN(MockPersistentCookieStore);
};

class MockCookieMonsterDelegate : public CookieMonster::Delegate {
 public:
  typedef std::pair<CookieMonster::CanonicalCookie, bool>
      CookieNotification;

  MockCookieMonsterDelegate();

  const std::vector<CookieNotification>& changes() const { return changes_; }

  void reset() { changes_.clear(); }

  virtual void OnCookieChanged(
      const CookieMonster::CanonicalCookie& cookie,
      bool removed,
      CookieMonster::Delegate::ChangeCause cause);

 private:
  virtual ~MockCookieMonsterDelegate();

  std::vector<CookieNotification> changes_;

  DISALLOW_COPY_AND_ASSIGN(MockCookieMonsterDelegate);
};

void AddCookieToList(
    const std::string& key,
    const std::string& cookie_line,
    const base::Time& creation_time,
    std::vector<CookieMonster::CanonicalCookie*>* out_list);

class MockSimplePersistentCookieStore
    : public CookieMonster::PersistentCookieStore {
 public:
  MockSimplePersistentCookieStore();
  virtual ~MockSimplePersistentCookieStore();

  virtual bool Load(
      std::vector<CookieMonster::CanonicalCookie*>* out_cookies);

  virtual void AddCookie(
      const CookieMonster::CanonicalCookie& cookie);

  virtual void UpdateCookieAccessTime(
      const CookieMonster::CanonicalCookie& cookie);

  virtual void DeleteCookie(
      const CookieMonster::CanonicalCookie& cookie);

  virtual void Flush(Task* completion_task);

  virtual void SetClearLocalStateOnExit(bool clear_local_state);

 private:
  typedef std::map<int64, CookieMonster::CanonicalCookie>
      CanonicalCookieMap;

  CanonicalCookieMap cookies_;
};

CookieMonster* CreateMonsterFromStoreForGC(
    int num_cookies,
    int num_old_cookies,
    int days_old);

}  
