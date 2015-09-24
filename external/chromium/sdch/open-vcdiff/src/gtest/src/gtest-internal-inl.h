// Copyright 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GTEST_SRC_GTEST_INTERNAL_INL_H_
#define GTEST_SRC_GTEST_INTERNAL_INL_H_

#ifndef GTEST_IMPLEMENTATION
#error "gtest-internal-inl.h is part of Google Test's internal implementation."
#error "It must not be included except by Google Test itself."
#endif  

#include <stddef.h>

#include <gtest/internal/gtest-port.h>

#ifdef GTEST_OS_WINDOWS
#include <windows.h>  
#endif  

#include <ostream>  
#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>

namespace testing {

GTEST_DECLARE_bool(break_on_failure);
GTEST_DECLARE_bool(catch_exceptions);
GTEST_DECLARE_string(color);
GTEST_DECLARE_string(filter);
GTEST_DECLARE_bool(list_tests);
GTEST_DECLARE_string(output);
GTEST_DECLARE_int32(repeat);
GTEST_DECLARE_int32(stack_trace_depth);
GTEST_DECLARE_bool(show_internal_stack_frames);

namespace internal {

const char kBreakOnFailureFlag[] = "break_on_failure";
const char kCatchExceptionsFlag[] = "catch_exceptions";
const char kFilterFlag[] = "filter";
const char kListTestsFlag[] = "list_tests";
const char kOutputFlag[] = "output";
const char kColorFlag[] = "color";
const char kRepeatFlag[] = "repeat";

class GTestFlagSaver {
 public:
  
  GTestFlagSaver() {
    break_on_failure_ = GTEST_FLAG(break_on_failure);
    catch_exceptions_ = GTEST_FLAG(catch_exceptions);
    color_ = GTEST_FLAG(color);
    death_test_style_ = GTEST_FLAG(death_test_style);
    filter_ = GTEST_FLAG(filter);
    internal_run_death_test_ = GTEST_FLAG(internal_run_death_test);
    list_tests_ = GTEST_FLAG(list_tests);
    output_ = GTEST_FLAG(output);
    repeat_ = GTEST_FLAG(repeat);
  }

  
  ~GTestFlagSaver() {
    GTEST_FLAG(break_on_failure) = break_on_failure_;
    GTEST_FLAG(catch_exceptions) = catch_exceptions_;
    GTEST_FLAG(color) = color_;
    GTEST_FLAG(death_test_style) = death_test_style_;
    GTEST_FLAG(filter) = filter_;
    GTEST_FLAG(internal_run_death_test) = internal_run_death_test_;
    GTEST_FLAG(list_tests) = list_tests_;
    GTEST_FLAG(output) = output_;
    GTEST_FLAG(repeat) = repeat_;
  }
 private:
  
  bool break_on_failure_;
  bool catch_exceptions_;
  String color_;
  String death_test_style_;
  String filter_;
  String internal_run_death_test_;
  bool list_tests_;
  String output_;
  bool pretty_;
  internal::Int32 repeat_;
} GTEST_ATTRIBUTE_UNUSED;

String ToUtf8String(wchar_t wchar);

size_t GetThreadCount();


template <typename E>  
class List;

template <typename E>  
class ListNode {
  friend class List<E>;

 private:

  E element_;
  ListNode * next_;

  
  
  
  
  
  
  
  
  explicit ListNode(const E & element) : element_(element), next_(NULL) {}

  
  GTEST_DISALLOW_COPY_AND_ASSIGN(ListNode);

 public:

  
  E & element() { return element_; }
  const E & element() const { return element_; }

  
  ListNode * next() { return next_; }
  const ListNode * next() const { return next_; }
};


template <typename E>  
class List {
 public:

  
  List() : head_(NULL), last_(NULL), size_(0) {}

  
  virtual ~List();

  
  void Clear() {
    if ( size_ > 0 ) {
      
      ListNode<E> * node = head_;
      ListNode<E> * next = node->next();
      for ( ; ; ) {
        delete node;
        node = next;
        if ( node == NULL ) break;
        next = node->next();
      }

      
      head_ = last_ = NULL;
      size_ = 0;
    }
  }

  
  int size() const { return size_; }

  
  bool IsEmpty() const { return size() == 0; }

  
  ListNode<E> * Head() { return head_; }
  const ListNode<E> * Head() const { return head_; }

  
  ListNode<E> * Last() { return last_; }
  const ListNode<E> * Last() const { return last_; }

  
  
  
  
  void PushBack(const E & element) {
    ListNode<E> * new_node = new ListNode<E>(element);

    if ( size_ == 0 ) {
      head_ = last_ = new_node;
      size_ = 1;
    } else {
      last_->next_ = new_node;
      last_ = new_node;
      size_++;
    }
  }

  
  void PushFront(const E& element) {
    ListNode<E>* const new_node = new ListNode<E>(element);

    if ( size_ == 0 ) {
      head_ = last_ = new_node;
      size_ = 1;
    } else {
      new_node->next_ = head_;
      head_ = new_node;
      size_++;
    }
  }

  
  
  
  
  bool PopFront(E* result) {
    if (size_ == 0) return false;

    if (result != NULL) {
      *result = head_->element_;
    }

    ListNode<E>* const old_head = head_;
    size_--;
    if (size_ == 0) {
      head_ = last_ = NULL;
    } else {
      head_ = head_->next_;
    }
    delete old_head;

    return true;
  }

  
  
  
  
  ListNode<E>* InsertAfter(ListNode<E>* node, const E& element) {
    if (node == NULL) {
      PushFront(element);
      return Head();
    }

    ListNode<E>* const new_node = new ListNode<E>(element);
    new_node->next_ = node->next_;
    node->next_ = new_node;
    size_++;
    if (node == last_) {
      last_ = new_node;
    }

    return new_node;
  }

  
  
  
  template <typename P>  
  int CountIf(P predicate) const {
    int count = 0;
    for ( const ListNode<E> * node = Head();
          node != NULL;
          node = node->next() ) {
      if ( predicate(node->element()) ) {
        count++;
      }
    }

    return count;
  }

  
  
  
  
  template <typename F>  
  void ForEach(F functor) const {
    for ( const ListNode<E> * node = Head();
          node != NULL;
          node = node->next() ) {
      functor(node->element());
    }
  }

  
  
  
  
  template <typename P>  
  const ListNode<E> * FindIf(P predicate) const {
    for ( const ListNode<E> * node = Head();
          node != NULL;
          node = node->next() ) {
      if ( predicate(node->element()) ) {
        return node;
      }
    }

    return NULL;
  }

  template <typename P>
  ListNode<E> * FindIf(P predicate) {
    for ( ListNode<E> * node = Head();
          node != NULL;
          node = node->next() ) {
      if ( predicate(node->element() ) ) {
        return node;
      }
    }

    return NULL;
  }

 private:
  ListNode<E>* head_;  
  ListNode<E>* last_;  
  int size_;           

  
  GTEST_DISALLOW_COPY_AND_ASSIGN(List);
};

template <typename E>
List<E>::~List() {
  Clear();
}

template <typename T>
static void Delete(T * x) {
  delete x;
}

class TestProperty {
 public:
  
  
  
  TestProperty(const char* key, const char* value) :
    key_(key), value_(value) {
  }

  
  const char* key() const {
    return key_.c_str();
  }

  
  const char* value() const {
    return value_.c_str();
  }

  
  void SetValue(const char* new_value) {
    value_ = new_value;
  }

 private:
  
  String key_;
  
  String value_;
};

class TestPropertyKeyIs {
 public:
  
  
  
  explicit TestPropertyKeyIs(const char* key)
      : key_(key) {}

  
  bool operator()(const TestProperty& test_property) const {
    return String(test_property.key()).Compare(key_) == 0;
  }

 private:
  String key_;
};

class TestResult {
 public:
  
  TestResult();

  
  ~TestResult();

  
  const internal::List<TestPartResult> & test_part_results() const {
    return test_part_results_;
  }

  
  const internal::List<internal::TestProperty> & test_properties() const {
    return test_properties_;
  }

  
  int successful_part_count() const;

  
  int failed_part_count() const;

  
  
  int total_part_count() const;

  
  bool Passed() const { return !Failed(); }

  
  bool Failed() const { return failed_part_count() > 0; }

  
  bool HasFatalFailure() const;

  
  TimeInMillis elapsed_time() const { return elapsed_time_; }

  
  void set_elapsed_time(TimeInMillis elapsed) { elapsed_time_ = elapsed; }

  
  void AddTestPartResult(const TestPartResult& test_part_result);

  
  
  
  
  
  void RecordProperty(const internal::TestProperty& test_property);

  
  
  
  static bool ValidateTestProperty(const internal::TestProperty& test_property);

  
  int death_test_count() const { return death_test_count_; }

  
  int increment_death_test_count() { return ++death_test_count_; }

  
  void Clear();
 private:
  
  
  internal::Mutex test_properites_mutex_;

  
  internal::List<TestPartResult> test_part_results_;
  
  internal::List<internal::TestProperty> test_properties_;
  
  int death_test_count_;
  
  TimeInMillis elapsed_time_;

  
  GTEST_DISALLOW_COPY_AND_ASSIGN(TestResult);
};  

class TestInfoImpl {
 public:
  TestInfoImpl(TestInfo* parent, const char* test_case_name,
               const char* name, TypeId fixture_class_id,
               TestMaker maker);
  ~TestInfoImpl();

  
  bool should_run() const { return should_run_; }

  
  void set_should_run(bool should) { should_run_ = should; }

  
  bool is_disabled() const { return is_disabled_; }

  
  void set_is_disabled(bool is) { is_disabled_ = is; }

  
  const char* test_case_name() const { return test_case_name_.c_str(); }

  
  const char* name() const { return name_.c_str(); }

  
  TypeId fixture_class_id() const { return fixture_class_id_; }

  
  internal::TestResult* result() { return &result_; }
  const internal::TestResult* result() const { return &result_; }

  
  
  void Run();

  
  static void RunTest(TestInfo * test_info) {
    test_info->impl()->Run();
  }

  
  void ClearResult() { result_.Clear(); }

  
  static void ClearTestResult(TestInfo * test_info) {
    test_info->impl()->ClearResult();
  }

 private:
  
  TestInfo* const parent_;         
  const String test_case_name_;    
  const String name_;              
  const TypeId fixture_class_id_;  
  bool should_run_;                
  bool is_disabled_;               
  const TestMaker maker_;          

  
  
  internal::TestResult result_;

  GTEST_DISALLOW_COPY_AND_ASSIGN(TestInfoImpl);
};

}  

class TestCase {
 public:
  
  
  
  
  
  
  
  
  
  
  TestCase(const char* name,
           Test::SetUpTestCaseFunc set_up_tc,
           Test::TearDownTestCaseFunc tear_down_tc);

  
  virtual ~TestCase();

  
  const char* name() const { return name_.c_str(); }

  
  bool should_run() const { return should_run_; }

  
  void set_should_run(bool should) { should_run_ = should; }

  
  internal::List<TestInfo*>& test_info_list() { return *test_info_list_; }

  
  const internal::List<TestInfo *> & test_info_list() const {
    return *test_info_list_;
  }

  
  int successful_test_count() const;

  
  int failed_test_count() const;

  
  int disabled_test_count() const;

  
  int test_to_run_count() const;

  
  int total_test_count() const;

  
  bool Passed() const { return !Failed(); }

  
  bool Failed() const { return failed_test_count() > 0; }

  
  internal::TimeInMillis elapsed_time() const { return elapsed_time_; }

  
  
  void AddTestInfo(TestInfo * test_info);

  
  
  TestInfo* GetTestInfo(const char* test_name);

  
  void ClearResult();

  
  static void ClearTestCaseResult(TestCase* test_case) {
    test_case->ClearResult();
  }

  
  void Run();

  
  static void RunTestCase(TestCase * test_case) { test_case->Run(); }

  
  static bool TestPassed(const TestInfo * test_info) {
    const internal::TestInfoImpl* const impl = test_info->impl();
    return impl->should_run() && impl->result()->Passed();
  }

  
  static bool TestFailed(const TestInfo * test_info) {
    const internal::TestInfoImpl* const impl = test_info->impl();
    return impl->should_run() && impl->result()->Failed();
  }

  
  static bool TestDisabled(const TestInfo * test_info) {
    return test_info->impl()->is_disabled();
  }

  
  static bool ShouldRunTest(const TestInfo *test_info) {
    return test_info->impl()->should_run();
  }

 private:
  
  internal::String name_;
  
  internal::List<TestInfo*>* test_info_list_;
  
  Test::SetUpTestCaseFunc set_up_tc_;
  
  Test::TearDownTestCaseFunc tear_down_tc_;
  
  bool should_run_;
  
  internal::TimeInMillis elapsed_time_;

  
  GTEST_DISALLOW_COPY_AND_ASSIGN(TestCase);
};

namespace internal {

class UnitTestOptions {
 public:
  

  
  static String GetOutputFormat();

  
  
  static String GetOutputFile();

  

  
  
  
  
  
  static bool PatternMatchesString(const char *pattern, const char *str);

  
  
  static bool FilterMatchesTest(const String &test_case_name,
                                const String &test_name);

#ifdef GTEST_OS_WINDOWS
  

  
  
  
  static int GTestShouldProcessSEH(DWORD exception_code);
#endif  
 private:
  
  
  static bool MatchesFilter(const String& name, const char* filter);
};

FilePath GetCurrentExecutableName();

class OsStackTraceGetterInterface {
 public:
  OsStackTraceGetterInterface() {}
  virtual ~OsStackTraceGetterInterface() {}

  
  
  
  
  
  
  virtual String CurrentStackTrace(int max_depth, int skip_count) = 0;

  
  
  
  virtual void UponLeavingGTest() = 0;

 private:
  GTEST_DISALLOW_COPY_AND_ASSIGN(OsStackTraceGetterInterface);
};

class OsStackTraceGetter : public OsStackTraceGetterInterface {
 public:
  OsStackTraceGetter() {}
  virtual String CurrentStackTrace(int max_depth, int skip_count);
  virtual void UponLeavingGTest();

  
  
  static const char* const kElidedFramesMarker;

 private:
  Mutex mutex_;  

  
  
  
  
  void* caller_frame_;

  GTEST_DISALLOW_COPY_AND_ASSIGN(OsStackTraceGetter);
};

struct TraceInfo {
  const char* file;
  int line;
  String message;
};

class UnitTestImpl : public TestPartResultReporterInterface {
 public:
  explicit UnitTestImpl(UnitTest* parent);
  virtual ~UnitTestImpl();

  
  
  virtual void ReportTestPartResult(const TestPartResult& result);

  
  TestPartResultReporterInterface* test_part_result_reporter();

  
  void set_test_part_result_reporter(TestPartResultReporterInterface* reporter);

  
  int successful_test_case_count() const;

  
  int failed_test_case_count() const;

  
  int total_test_case_count() const;

  
  
  int test_case_to_run_count() const;

  
  int successful_test_count() const;

  
  int failed_test_count() const;

  
  int disabled_test_count() const;

  
  int total_test_count() const;

  
  int test_to_run_count() const;

  
  TimeInMillis elapsed_time() const { return elapsed_time_; }

  
  bool Passed() const { return !Failed(); }

  
  
  bool Failed() const {
    return failed_test_case_count() > 0 || ad_hoc_test_result()->Failed();
  }

  
  
  internal::TestResult* current_test_result();

  
  const internal::TestResult* ad_hoc_test_result() const {
    return &ad_hoc_test_result_;
  }

  
  
  
  
  
  void set_result_printer(UnitTestEventListenerInterface * result_printer);

  
  
  
  UnitTestEventListenerInterface* result_printer();

  
  
  
  
  
  void set_os_stack_trace_getter(OsStackTraceGetterInterface* getter);

  
  
  
  OsStackTraceGetterInterface* os_stack_trace_getter();

  
  
  
  
  
  
  
  
  
  
  String CurrentOsStackTraceExceptTop(int skip_count);

  
  
  
  
  
  
  
  
  TestCase* GetTestCase(const char* test_case_name,
                        Test::SetUpTestCaseFunc set_up_tc,
                        Test::TearDownTestCaseFunc tear_down_tc);

  
  
  
  
  
  
  
  void AddTestInfo(Test::SetUpTestCaseFunc set_up_tc,
                   Test::TearDownTestCaseFunc tear_down_tc,
                   TestInfo * test_info) {
    GetTestCase(test_info->test_case_name(),
                set_up_tc,
                tear_down_tc)->AddTestInfo(test_info);
  }

  
  void set_current_test_case(TestCase* current_test_case) {
    current_test_case_ = current_test_case;
  }

  
  
  
  void set_current_test_info(TestInfo* current_test_info) {
    current_test_info_ = current_test_info;
  }

  
  
  
  
  
  
  int RunAllTests();

  
  void ClearResult() {
    test_cases_.ForEach(TestCase::ClearTestCaseResult);
    ad_hoc_test_result_.Clear();
  }

  
  
  
  
  int FilterTests();

  
  void ListAllTests();

  const TestCase* current_test_case() const { return current_test_case_; }
  TestInfo* current_test_info() { return current_test_info_; }
  const TestInfo* current_test_info() const { return current_test_info_; }

  
  
  internal::List<Environment*>* environments() { return &environments_; }
  internal::List<Environment*>* environments_in_reverse_order() {
    return &environments_in_reverse_order_;
  }

  internal::List<TestCase*>* test_cases() { return &test_cases_; }
  const internal::List<TestCase*>* test_cases() const { return &test_cases_; }

  
  internal::List<TraceInfo>* gtest_trace_stack() {
    return gtest_trace_stack_.pointer();
  }
  const internal::List<TraceInfo>* gtest_trace_stack() const {
    return gtest_trace_stack_.pointer();
  }

#ifdef GTEST_HAS_DEATH_TEST
  
  
  
  const InternalRunDeathTestFlag* internal_run_death_test_flag() const {
    return internal_run_death_test_flag_.get();
  }

  
  internal::DeathTestFactory* death_test_factory() {
    return death_test_factory_.get();
  }

  friend class ReplaceDeathTestFactory;
#endif  

 private:
  
  UnitTest* const parent_;

  
  TestPartResultReporterInterface* test_part_result_reporter_;

  
  
  
  internal::List<Environment*> environments_;
  internal::List<Environment*> environments_in_reverse_order_;

  internal::List<TestCase*> test_cases_;  

  
  internal::ListNode<TestCase*>* last_death_test_case_;

  
  
  
  
  TestCase* current_test_case_;

  
  
  
  
  TestInfo* current_test_info_;

  
  
  
  
  
  
  
  
  internal::TestResult ad_hoc_test_result_;

  
  
  
  
  UnitTestEventListenerInterface* result_printer_;

  
  
  
  
  OsStackTraceGetterInterface* os_stack_trace_getter_;

  
  TimeInMillis elapsed_time_;

#ifdef GTEST_HAS_DEATH_TEST
  
  
  internal::scoped_ptr<InternalRunDeathTestFlag> internal_run_death_test_flag_;
  internal::scoped_ptr<internal::DeathTestFactory> death_test_factory_;
#endif  

  
  internal::ThreadLocal<internal::List<TraceInfo> > gtest_trace_stack_;

  GTEST_DISALLOW_COPY_AND_ASSIGN(UnitTestImpl);
};  

inline UnitTestImpl* GetUnitTestImpl() {
  return UnitTest::GetInstance()->impl();
}

}  
}  

#endif  
