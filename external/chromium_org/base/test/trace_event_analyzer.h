// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef BASE_TEST_TRACE_EVENT_ANALYZER_H_
#define BASE_TEST_TRACE_EVENT_ANALYZER_H_

#include <map>

#include "base/debug/trace_event.h"
#include "base/memory/ref_counted.h"

namespace base {
class Value;
}

namespace trace_analyzer {
class QueryNode;

struct TraceEvent {
  
  struct ProcessThreadID {
    ProcessThreadID() : process_id(0), thread_id(0) {}
    ProcessThreadID(int process_id, int thread_id)
        : process_id(process_id), thread_id(thread_id) {}
    bool operator< (const ProcessThreadID& rhs) const {
      if (process_id != rhs.process_id)
        return process_id < rhs.process_id;
      return thread_id < rhs.thread_id;
    }
    int process_id;
    int thread_id;
  };

  TraceEvent();
  ~TraceEvent();

  bool SetFromJSON(const base::Value* event_value) WARN_UNUSED_RESULT;

  bool operator< (const TraceEvent& rhs) const {
    return timestamp < rhs.timestamp;
  }

  bool has_other_event() const { return other_event; }

  
  
  
  double GetAbsTimeToOtherEvent() const;

  
  bool GetArgAsString(const std::string& name, std::string* arg) const;
  
  bool GetArgAsNumber(const std::string& name, double* arg) const;

  
  bool HasStringArg(const std::string& name) const;
  
  bool HasNumberArg(const std::string& name) const;

  
  
  
  std::string GetKnownArgAsString(const std::string& name) const;
  double GetKnownArgAsDouble(const std::string& name) const;
  int GetKnownArgAsInt(const std::string& name) const;
  bool GetKnownArgAsBool(const std::string& name) const;

  
  ProcessThreadID thread;

  
  
  double timestamp;

  double duration;

  char phase;

  std::string category;

  std::string name;

  std::string id;

  
  
  std::map<std::string, double> arg_numbers;

  std::map<std::string, std::string> arg_strings;

  
  const TraceEvent* other_event;
};

typedef std::vector<const TraceEvent*> TraceEventVector;

class Query {
 public:
  Query(const Query& query);

  ~Query();

  
  

  
  static Query String(const std::string& str);

  
  static Query Double(double num);
  static Query Int(int32 num);
  static Query Uint(uint32 num);

  
  static Query Bool(bool boolean);

  
  static Query Phase(char phase);

  
  
  static Query Pattern(const std::string& pattern);

  
  

  static Query EventPid() { return Query(EVENT_PID); }

  static Query EventTid() { return Query(EVENT_TID); }

  
  static Query EventTime() { return Query(EVENT_TIME); }

  
  
  static Query EventDuration() { return Query(EVENT_DURATION); }

  
  static Query EventCompleteDuration() {
    return Query(EVENT_COMPLETE_DURATION);
  }

  static Query EventPhase() { return Query(EVENT_PHASE); }

  static Query EventCategory() { return Query(EVENT_CATEGORY); }

  static Query EventName() { return Query(EVENT_NAME); }

  static Query EventId() { return Query(EVENT_ID); }

  static Query EventPidIs(int process_id) {
    return Query(EVENT_PID) == Query::Int(process_id);
  }

  static Query EventTidIs(int thread_id) {
    return Query(EVENT_TID) == Query::Int(thread_id);
  }

  static Query EventThreadIs(const TraceEvent::ProcessThreadID& thread) {
    return EventPidIs(thread.process_id) && EventTidIs(thread.thread_id);
  }

  static Query EventTimeIs(double timestamp) {
    return Query(EVENT_TIME) == Query::Double(timestamp);
  }

  static Query EventDurationIs(double duration) {
    return Query(EVENT_DURATION) == Query::Double(duration);
  }

  static Query EventPhaseIs(char phase) {
    return Query(EVENT_PHASE) == Query::Phase(phase);
  }

  static Query EventCategoryIs(const std::string& category) {
    return Query(EVENT_CATEGORY) == Query::String(category);
  }

  static Query EventNameIs(const std::string& name) {
    return Query(EVENT_NAME) == Query::String(name);
  }

  static Query EventIdIs(const std::string& id) {
    return Query(EVENT_ID) == Query::String(id);
  }

  
  static Query EventHasStringArg(const std::string& arg_name) {
    return Query(EVENT_HAS_STRING_ARG, arg_name);
  }

  
  
  static Query EventHasNumberArg(const std::string& arg_name) {
    return Query(EVENT_HAS_NUMBER_ARG, arg_name);
  }

  
  static Query EventArg(const std::string& arg_name) {
    return Query(EVENT_ARG, arg_name);
  }

  
  static Query EventHasOther() { return Query(EVENT_HAS_OTHER); }

  

  static Query OtherPid() { return Query(OTHER_PID); }

  static Query OtherTid() { return Query(OTHER_TID); }

  static Query OtherTime() { return Query(OTHER_TIME); }

  static Query OtherPhase() { return Query(OTHER_PHASE); }

  static Query OtherCategory() { return Query(OTHER_CATEGORY); }

  static Query OtherName() { return Query(OTHER_NAME); }

  static Query OtherId() { return Query(OTHER_ID); }

  static Query OtherPidIs(int process_id) {
    return Query(OTHER_PID) == Query::Int(process_id);
  }

  static Query OtherTidIs(int thread_id) {
    return Query(OTHER_TID) == Query::Int(thread_id);
  }

  static Query OtherThreadIs(const TraceEvent::ProcessThreadID& thread) {
    return OtherPidIs(thread.process_id) && OtherTidIs(thread.thread_id);
  }

  static Query OtherTimeIs(double timestamp) {
    return Query(OTHER_TIME) == Query::Double(timestamp);
  }

  static Query OtherPhaseIs(char phase) {
    return Query(OTHER_PHASE) == Query::Phase(phase);
  }

  static Query OtherCategoryIs(const std::string& category) {
    return Query(OTHER_CATEGORY) == Query::String(category);
  }

  static Query OtherNameIs(const std::string& name) {
    return Query(OTHER_NAME) == Query::String(name);
  }

  static Query OtherIdIs(const std::string& id) {
    return Query(OTHER_ID) == Query::String(id);
  }

  
  static Query OtherHasStringArg(const std::string& arg_name) {
    return Query(OTHER_HAS_STRING_ARG, arg_name);
  }

  
  
  static Query OtherHasNumberArg(const std::string& arg_name) {
    return Query(OTHER_HAS_NUMBER_ARG, arg_name);
  }

  
  static Query OtherArg(const std::string& arg_name) {
    return Query(OTHER_ARG, arg_name);
  }

  
  

  
  static Query MatchBeginWithEnd() {
    return (Query(EVENT_PHASE) == Query::Phase(TRACE_EVENT_PHASE_BEGIN)) &&
           Query(EVENT_HAS_OTHER);
  }

  
  static Query MatchComplete() {
    return (Query(EVENT_PHASE) == Query::Phase(TRACE_EVENT_PHASE_COMPLETE));
  }

  
  static Query MatchAsyncBeginWithNext() {
    return (Query(EVENT_PHASE) ==
            Query::Phase(TRACE_EVENT_PHASE_ASYNC_BEGIN)) &&
           Query(EVENT_HAS_OTHER);
  }

  
  static Query MatchBeginName(const std::string& name) {
    return (Query(EVENT_NAME) == Query(name)) && MatchBeginWithEnd();
  }

  
  static Query MatchCompleteName(const std::string& name) {
    return (Query(EVENT_NAME) == Query(name)) && MatchComplete();
  }

  
  static Query MatchThread(const TraceEvent::ProcessThreadID& thread) {
    return (Query(EVENT_PID) == Query::Int(thread.process_id)) &&
           (Query(EVENT_TID) == Query::Int(thread.thread_id));
  }

  
  static Query MatchCrossThread() {
    return (Query(EVENT_PID) != Query(OTHER_PID)) ||
           (Query(EVENT_TID) != Query(OTHER_TID));
  }

  
  

  
  Query operator==(const Query& rhs) const;
  Query operator!=(const Query& rhs) const;
  Query operator< (const Query& rhs) const;
  Query operator<=(const Query& rhs) const;
  Query operator> (const Query& rhs) const;
  Query operator>=(const Query& rhs) const;
  Query operator&&(const Query& rhs) const;
  Query operator||(const Query& rhs) const;
  Query operator!() const;

  
  
  Query operator+(const Query& rhs) const;
  Query operator-(const Query& rhs) const;
  Query operator*(const Query& rhs) const;
  Query operator/(const Query& rhs) const;
  Query operator-() const;
  
  Query operator%(const Query& rhs) const;

  
  
  bool Evaluate(const TraceEvent& event) const;

 private:
  enum TraceEventMember {
    EVENT_INVALID,
    EVENT_PID,
    EVENT_TID,
    EVENT_TIME,
    EVENT_DURATION,
    EVENT_COMPLETE_DURATION,
    EVENT_PHASE,
    EVENT_CATEGORY,
    EVENT_NAME,
    EVENT_ID,
    EVENT_HAS_STRING_ARG,
    EVENT_HAS_NUMBER_ARG,
    EVENT_ARG,
    EVENT_HAS_OTHER,
    OTHER_PID,
    OTHER_TID,
    OTHER_TIME,
    OTHER_PHASE,
    OTHER_CATEGORY,
    OTHER_NAME,
    OTHER_ID,
    OTHER_HAS_STRING_ARG,
    OTHER_HAS_NUMBER_ARG,
    OTHER_ARG,
  };

  enum Operator {
    OP_INVALID,
    
    OP_EQ,
    OP_NE,
    OP_LT,
    OP_LE,
    OP_GT,
    OP_GE,
    OP_AND,
    OP_OR,
    OP_NOT,
    
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_NEGATE
  };

  enum QueryType {
    QUERY_BOOLEAN_OPERATOR,
    QUERY_ARITHMETIC_OPERATOR,
    QUERY_EVENT_MEMBER,
    QUERY_NUMBER,
    QUERY_STRING
  };

  
  explicit Query(TraceEventMember member);

  
  Query(TraceEventMember member, const std::string& arg_name);

  
  explicit Query(const std::string& str);

  
  explicit Query(double num);

  
  Query(const Query& left, const Query& right, Operator binary_op);

  
  Query(const Query& left, Operator unary_op);

  
  
  
  bool CompareAsDouble(const TraceEvent& event, bool* result) const;

  
  
  
  bool CompareAsString(const TraceEvent& event, bool* result) const;

  
  
  bool GetAsDouble(const TraceEvent& event, double* num) const;

  
  
  bool GetAsString(const TraceEvent& event, std::string* str) const;

  
  bool EvaluateArithmeticOperator(const TraceEvent& event,
                                  double* num) const;

  
  bool GetMemberValueAsDouble(const TraceEvent& event, double* num) const;

  
  bool GetMemberValueAsString(const TraceEvent& event, std::string* num) const;

  
  bool is_value() const { return type_ != QUERY_BOOLEAN_OPERATOR; }

  bool is_unary_operator() const {
    return operator_ == OP_NOT || operator_ == OP_NEGATE;
  }

  bool is_comparison_operator() const {
    return operator_ != OP_INVALID && operator_ < OP_AND;
  }

  const Query& left() const;
  const Query& right() const;

  QueryType type_;
  Operator operator_;
  scoped_refptr<QueryNode> left_;
  scoped_refptr<QueryNode> right_;
  TraceEventMember member_;
  double number_;
  std::string string_;
  bool is_pattern_;
};

class QueryNode : public base::RefCounted<QueryNode> {
 public:
  explicit QueryNode(const Query& query);
  const Query& query() const { return query_; }

 private:
  friend class base::RefCounted<QueryNode>;
  ~QueryNode();

  Query query_;
};

class TraceAnalyzer {
 public:
  ~TraceAnalyzer();

  
  
  static TraceAnalyzer* Create(const std::string& json_events)
                               WARN_UNUSED_RESULT;

  
  
  
  
  
  
  void AssociateBeginEndEvents();

  
  
  
  
  void AssociateAsyncBeginEndEvents();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void AssociateEvents(const Query& first,
                       const Query& second,
                       const Query& match);

  
  // argument name already exists, it will not be overwritten.
  void MergeAssociatedEventArgs();

  
  size_t FindEvents(const Query& query, TraceEventVector* output);

  
  const TraceEvent* FindFirstOf(const Query& query);

  
  const TraceEvent* FindLastOf(const Query& query);

  const std::string& GetThreadName(const TraceEvent::ProcessThreadID& thread);

 private:
  TraceAnalyzer();

  bool SetEvents(const std::string& json_events) WARN_UNUSED_RESULT;

  
  void ParseMetadata();

  std::map<TraceEvent::ProcessThreadID, std::string> thread_names_;
  std::vector<TraceEvent> raw_events_;
  bool allow_assocation_changes_;

  DISALLOW_COPY_AND_ASSIGN(TraceAnalyzer);
};


struct RateStats {
  double min_us;
  double max_us;
  double mean_us;
  double standard_deviation_us;
};

struct RateStatsOptions {
  RateStatsOptions() : trim_min(0u), trim_max(0u) {}
  
  
  
  
  size_t trim_min;  
  size_t trim_max;  
};

bool GetRateStats(const TraceEventVector& events,
                  RateStats* stats,
                  const RateStatsOptions* options);

bool FindFirstOf(const TraceEventVector& events,
                 const Query& query,
                 size_t position,
                 size_t* return_index);

bool FindLastOf(const TraceEventVector& events,
                const Query& query,
                size_t position,
                size_t* return_index);

bool FindClosest(const TraceEventVector& events,
                 const Query& query,
                 size_t position,
                 size_t* return_closest,
                 size_t* return_second_closest);

size_t CountMatches(const TraceEventVector& events,
                    const Query& query,
                    size_t begin_position,
                    size_t end_position);

static inline size_t CountMatches(const TraceEventVector& events,
                                  const Query& query) {
  return CountMatches(events, query, 0u, events.size());
}

}  

#endif  
