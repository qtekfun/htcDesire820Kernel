// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_DECLARATIVE_RULE_H__
#define CHROME_BROWSER_EXTENSIONS_API_DECLARATIVE_DECLARATIVE_RULE_H__

#include <limits>
#include <set>
#include <string>
#include <vector>

#include "base/callback.h"
#include "base/memory/linked_ptr.h"
#include "base/memory/scoped_vector.h"
#include "base/stl_util.h"
#include "base/time/time.h"
#include "chrome/common/extensions/api/events.h"
#include "components/url_matcher/url_matcher.h"
#include "extensions/common/extension.h"

namespace base {
class Time;
class Value;
}

namespace extensions {

template<typename ConditionT>
class DeclarativeConditionSet {
 public:
  typedef std::vector<linked_ptr<base::Value> > AnyVector;
  typedef std::vector<linked_ptr<const ConditionT> > Conditions;
  typedef typename Conditions::const_iterator const_iterator;

  
  
  
  static scoped_ptr<DeclarativeConditionSet> Create(
      const Extension* extension,
      url_matcher::URLMatcherConditionFactory* url_matcher_condition_factory,
      const AnyVector& conditions,
      std::string* error);

  const Conditions& conditions() const {
    return conditions_;
  }

  const_iterator begin() const { return conditions_.begin(); }
  const_iterator end() const { return conditions_.end(); }

  
  
  
  
  
  bool IsFulfilled(url_matcher::URLMatcherConditionSet::ID url_match_trigger,
                   const typename ConditionT::MatchData& match_data) const;

  
  void GetURLMatcherConditionSets(
      url_matcher::URLMatcherConditionSet::Vector* condition_sets) const;

  
  bool HasConditionsWithoutUrls() const {
    return !conditions_without_urls_.empty();
  }

 private:
  typedef std::map<url_matcher::URLMatcherConditionSet::ID, const ConditionT*>
      URLMatcherIdToCondition;

  DeclarativeConditionSet(
      const Conditions& conditions,
      const URLMatcherIdToCondition& match_id_to_condition,
      const std::vector<const ConditionT*>& conditions_without_urls);

  const URLMatcherIdToCondition match_id_to_condition_;
  const Conditions conditions_;
  const std::vector<const ConditionT*> conditions_without_urls_;

  DISALLOW_COPY_AND_ASSIGN(DeclarativeConditionSet);
};

template<typename ActionT>
class DeclarativeActionSet {
 public:
  typedef std::vector<linked_ptr<base::Value> > AnyVector;
  typedef std::vector<scoped_refptr<const ActionT> > Actions;

  explicit DeclarativeActionSet(const Actions& actions);

  
  
  
  static scoped_ptr<DeclarativeActionSet> Create(const Extension* extension,
                                                 const AnyVector& actions,
                                                 std::string* error,
                                                 bool* bad_message);

  
  void Apply(const std::string& extension_id,
             const base::Time& extension_install_time,
             typename ActionT::ApplyInfo* apply_info) const;

  
  
  
  void Revert(const std::string& extension_id,
              const base::Time& extension_install_time,
              typename ActionT::ApplyInfo* apply_info) const;

  
  
  int GetMinimumPriority() const;

  const Actions& actions() const { return actions_; }

 private:
  const Actions actions_;

  DISALLOW_COPY_AND_ASSIGN(DeclarativeActionSet);
};

template<typename ConditionT, typename ActionT>
class DeclarativeRule {
 public:
  typedef std::string ExtensionId;
  typedef std::string RuleId;
  typedef std::pair<ExtensionId, RuleId> GlobalRuleId;
  typedef int Priority;
  typedef DeclarativeConditionSet<ConditionT> ConditionSet;
  typedef DeclarativeActionSet<ActionT> ActionSet;
  typedef extensions::api::events::Rule JsonRule;
  typedef std::vector<std::string> Tags;

  
  
  typedef base::Callback<bool(const ConditionSet* conditions,
                              const ActionSet* actions,
                              std::string* error)> ConsistencyChecker;

  DeclarativeRule(const GlobalRuleId& id,
                  const Tags& tags,
                  base::Time extension_installation_time,
                  scoped_ptr<ConditionSet> conditions,
                  scoped_ptr<ActionSet> actions,
                  Priority priority);

  
  
  
  
  
  
  
  
  static scoped_ptr<DeclarativeRule> Create(
      url_matcher::URLMatcherConditionFactory* url_matcher_condition_factory,
      const Extension* extension,
      base::Time extension_installation_time,
      linked_ptr<JsonRule> rule,
      ConsistencyChecker check_consistency,
      std::string* error);

  const GlobalRuleId& id() const { return id_; }
  const Tags& tags() const { return tags_; }
  const std::string& extension_id() const { return id_.first; }
  const ConditionSet& conditions() const { return *conditions_; }
  const ActionSet& actions() const { return *actions_; }
  Priority priority() const { return priority_; }

  
  
  
  
  void Apply(typename ActionT::ApplyInfo* apply_info) const;

  
  
  
  Priority GetMinimumPriority() const;

 private:
  GlobalRuleId id_;
  Tags tags_;
  base::Time extension_installation_time_;  
  scoped_ptr<ConditionSet> conditions_;
  scoped_ptr<ActionSet> actions_;
  Priority priority_;

  DISALLOW_COPY_AND_ASSIGN(DeclarativeRule);
};



template<typename ConditionT>
bool DeclarativeConditionSet<ConditionT>::IsFulfilled(
    url_matcher::URLMatcherConditionSet::ID url_match_trigger,
    const typename ConditionT::MatchData& match_data) const {
  if (url_match_trigger == -1) {
    
    
    for (typename std::vector<const ConditionT*>::const_iterator it =
             conditions_without_urls_.begin();
         it != conditions_without_urls_.end(); ++it) {
      if ((*it)->IsFulfilled(match_data))
        return true;
    }
    return false;
  }

  typename URLMatcherIdToCondition::const_iterator triggered =
      match_id_to_condition_.find(url_match_trigger);
  return (triggered != match_id_to_condition_.end() &&
          triggered->second->IsFulfilled(match_data));
}

template<typename ConditionT>
void DeclarativeConditionSet<ConditionT>::GetURLMatcherConditionSets(
    url_matcher::URLMatcherConditionSet::Vector* condition_sets) const {
  for (typename Conditions::const_iterator i = conditions_.begin();
       i != conditions_.end(); ++i) {
    (*i)->GetURLMatcherConditionSets(condition_sets);
  }
}

template<typename ConditionT>
scoped_ptr<DeclarativeConditionSet<ConditionT> >
DeclarativeConditionSet<ConditionT>::Create(
    const Extension* extension,
    url_matcher::URLMatcherConditionFactory* url_matcher_condition_factory,
    const AnyVector& conditions,
    std::string* error) {
  Conditions result;

  for (AnyVector::const_iterator i = conditions.begin();
       i != conditions.end(); ++i) {
    CHECK(i->get());
    scoped_ptr<ConditionT> condition = ConditionT::Create(
        extension, url_matcher_condition_factory, **i, error);
    if (!error->empty())
      return scoped_ptr<DeclarativeConditionSet>();
    result.push_back(make_linked_ptr(condition.release()));
  }

  URLMatcherIdToCondition match_id_to_condition;
  std::vector<const ConditionT*> conditions_without_urls;
  url_matcher::URLMatcherConditionSet::Vector condition_sets;

  for (typename Conditions::const_iterator i = result.begin();
       i != result.end(); ++i) {
    condition_sets.clear();
    (*i)->GetURLMatcherConditionSets(&condition_sets);
    if (condition_sets.empty()) {
      conditions_without_urls.push_back(i->get());
    } else {
      for (url_matcher::URLMatcherConditionSet::Vector::const_iterator
               match_set = condition_sets.begin();
           match_set != condition_sets.end(); ++match_set)
        match_id_to_condition[(*match_set)->id()] = i->get();
    }
  }

  return make_scoped_ptr(new DeclarativeConditionSet(
      result, match_id_to_condition, conditions_without_urls));
}

template<typename ConditionT>
DeclarativeConditionSet<ConditionT>::DeclarativeConditionSet(
    const Conditions& conditions,
    const URLMatcherIdToCondition& match_id_to_condition,
    const std::vector<const ConditionT*>& conditions_without_urls)
    : match_id_to_condition_(match_id_to_condition),
      conditions_(conditions),
      conditions_without_urls_(conditions_without_urls) {}


template<typename ActionT>
DeclarativeActionSet<ActionT>::DeclarativeActionSet(const Actions& actions)
    : actions_(actions) {}

template<typename ActionT>
scoped_ptr<DeclarativeActionSet<ActionT> >
DeclarativeActionSet<ActionT>::Create(
    const Extension* extension,
    const AnyVector& actions,
    std::string* error,
    bool* bad_message) {
  *error = "";
  *bad_message = false;
  Actions result;

  for (AnyVector::const_iterator i = actions.begin();
       i != actions.end(); ++i) {
    CHECK(i->get());
    scoped_refptr<const ActionT> action =
        ActionT::Create(extension, **i, error, bad_message);
    if (!error->empty() || *bad_message)
      return scoped_ptr<DeclarativeActionSet>();
    result.push_back(action);
  }

  return scoped_ptr<DeclarativeActionSet>(new DeclarativeActionSet(result));
}

template<typename ActionT>
void DeclarativeActionSet<ActionT>::Apply(
    const std::string& extension_id,
    const base::Time& extension_install_time,
    typename ActionT::ApplyInfo* apply_info) const {
  for (typename Actions::const_iterator i = actions_.begin();
       i != actions_.end(); ++i)
    (*i)->Apply(extension_id, extension_install_time, apply_info);
}

template<typename ActionT>
void DeclarativeActionSet<ActionT>::Revert(
    const std::string& extension_id,
    const base::Time& extension_install_time,
    typename ActionT::ApplyInfo* apply_info) const {
  for (typename Actions::const_iterator i = actions_.begin();
       i != actions_.end(); ++i)
    (*i)->Revert(extension_id, extension_install_time, apply_info);
}

template<typename ActionT>
int DeclarativeActionSet<ActionT>::GetMinimumPriority() const {
  int minimum_priority = std::numeric_limits<int>::min();
  for (typename Actions::const_iterator i = actions_.begin();
       i != actions_.end(); ++i) {
    minimum_priority = std::max(minimum_priority, (*i)->minimum_priority());
  }
  return minimum_priority;
}


template<typename ConditionT, typename ActionT>
DeclarativeRule<ConditionT, ActionT>::DeclarativeRule(
    const GlobalRuleId& id,
    const Tags& tags,
    base::Time extension_installation_time,
    scoped_ptr<ConditionSet> conditions,
    scoped_ptr<ActionSet> actions,
    Priority priority)
    : id_(id),
      tags_(tags),
      extension_installation_time_(extension_installation_time),
      conditions_(conditions.release()),
      actions_(actions.release()),
      priority_(priority) {
  CHECK(conditions_.get());
  CHECK(actions_.get());
}

template<typename ConditionT, typename ActionT>
scoped_ptr<DeclarativeRule<ConditionT, ActionT> >
DeclarativeRule<ConditionT, ActionT>::Create(
    url_matcher::URLMatcherConditionFactory* url_matcher_condition_factory,
    const Extension* extension,
    base::Time extension_installation_time,
    linked_ptr<JsonRule> rule,
    ConsistencyChecker check_consistency,
    std::string* error) {
  scoped_ptr<DeclarativeRule> error_result;

  scoped_ptr<ConditionSet> conditions = ConditionSet::Create(
      extension, url_matcher_condition_factory, rule->conditions, error);
  if (!error->empty())
    return error_result.Pass();
  CHECK(conditions.get());

  bool bad_message = false;
  scoped_ptr<ActionSet> actions =
      ActionSet::Create(extension, rule->actions, error, &bad_message);
  if (bad_message) {
    
    
    *error = "An action of a rule set had an invalid "
        "structure that should have been caught by the JSON validator.";
    return error_result.Pass();
  }
  if (!error->empty() || bad_message)
    return error_result.Pass();
  CHECK(actions.get());

  if (!check_consistency.is_null() &&
      !check_consistency.Run(conditions.get(), actions.get(), error)) {
    DCHECK(!error->empty());
    return error_result.Pass();
  }

  CHECK(rule->priority.get());
  int priority = *(rule->priority);

  GlobalRuleId rule_id(extension->id(), *(rule->id));
  Tags tags = rule->tags ? *rule->tags : Tags();
  return scoped_ptr<DeclarativeRule>(
      new DeclarativeRule(rule_id, tags, extension_installation_time,
                          conditions.Pass(), actions.Pass(), priority));
}

template<typename ConditionT, typename ActionT>
void DeclarativeRule<ConditionT, ActionT>::Apply(
    typename ActionT::ApplyInfo* apply_info) const {
  return actions_->Apply(extension_id(),
                         extension_installation_time_,
                         apply_info);
}

template<typename ConditionT, typename ActionT>
int DeclarativeRule<ConditionT, ActionT>::GetMinimumPriority() const {
  return actions_->GetMinimumPriority();
}

}  

#endif  
