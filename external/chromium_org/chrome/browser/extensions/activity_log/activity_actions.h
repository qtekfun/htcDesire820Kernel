// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_ACTIVITY_ACTIONS_H_
#define CHROME_BROWSER_EXTENSIONS_ACTIVITY_LOG_ACTIVITY_ACTIONS_H_

#include <string>
#include <vector>

#include "base/memory/ref_counted_memory.h"
#include "base/time/time.h"
#include "base/values.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/common/extensions/api/activity_log_private.h"
#include "sql/connection.h"
#include "sql/statement.h"
#include "sql/transaction.h"
#include "url/gurl.h"

namespace extensions {

class Action : public base::RefCountedThreadSafe<Action> {
 public:
  
  
  enum ActionType {
    ACTION_API_CALL = 0,
    ACTION_API_EVENT = 1,
    UNUSED_ACTION_API_BLOCKED = 2,  
    ACTION_CONTENT_SCRIPT = 3,
    ACTION_DOM_ACCESS = 4,
    ACTION_DOM_EVENT = 5,
    ACTION_WEB_REQUEST = 6,
    ACTION_ANY = 1001,              
  };

  
  
  typedef std::vector<scoped_refptr<Action> > ActionVector;

  
  
  
  Action(const std::string& extension_id,
         const base::Time& time,
         const ActionType action_type,
         const std::string& api_name);

  
  scoped_refptr<Action> Clone() const;

  
  const std::string& extension_id() const { return extension_id_; }

  
  const base::Time& time() const { return time_; }
  void set_time(const base::Time& time) { time_ = time; }

  
  
  ActionType action_type() const { return action_type_; }

  
  const std::string& api_name() const { return api_name_; }
  void set_api_name(const std::string api_name) { api_name_ = api_name; }

  
  
  
  
  
  const ListValue* args() const { return args_.get(); }
  void set_args(scoped_ptr<ListValue> args);
  ListValue* mutable_args();

  
  const GURL& page_url() const { return page_url_; }
  void set_page_url(const GURL& page_url);

  
  const std::string& page_title() const { return page_title_; }
  void set_page_title(const std::string& title) { page_title_ = title; }

  
  const GURL& arg_url() const { return arg_url_; }
  void set_arg_url(const GURL& arg_url);

  
  
  bool page_incognito() const { return page_incognito_; }
  void set_page_incognito(bool incognito) { page_incognito_ = incognito; }
  bool arg_incognito() const { return arg_incognito_; }
  void set_arg_incognito(bool incognito) { arg_incognito_ = incognito; }

  
  const DictionaryValue* other() const { return other_.get(); }
  void set_other(scoped_ptr<DictionaryValue> other);
  DictionaryValue* mutable_other();

  
  
  
  std::string SerializePageUrl() const;
  void ParsePageUrl(const std::string& url);
  std::string SerializeArgUrl() const;
  void ParseArgUrl(const std::string& url);

  
  int count() const { return count_; }
  void set_count(int count) { count_ = count; }

  
  scoped_ptr<api::activity_log_private::ExtensionActivity>
      ConvertToExtensionActivity();

  
  virtual std::string PrintForDebug() const;

 protected:
  virtual ~Action();

 private:
  friend class base::RefCountedThreadSafe<Action>;

  std::string extension_id_;
  base::Time time_;
  ActionType action_type_;
  std::string api_name_;
  scoped_ptr<ListValue> args_;
  GURL page_url_;
  std::string page_title_;
  bool page_incognito_;
  GURL arg_url_;
  bool arg_incognito_;
  scoped_ptr<DictionaryValue> other_;
  int count_;

  DISALLOW_COPY_AND_ASSIGN(Action);
};

struct ActionComparator {
  
  bool operator()(const scoped_refptr<Action>& lhs,
                  const scoped_refptr<Action>& rhs) const;
};

struct ActionComparatorExcludingTime {
  
  bool operator()(const scoped_refptr<Action>& lhs,
                  const scoped_refptr<Action>& rhs) const;
};

}  

#endif  
