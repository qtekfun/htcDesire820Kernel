// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_FULLSCREEN_FULLSCREEN_CONTROLLER_STATE_TEST_H_
#define CHROME_BROWSER_UI_FULLSCREEN_FULLSCREEN_CONTROLLER_STATE_TEST_H_

#include <sstream>

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "base/memory/scoped_ptr.h"
#include "build/build_config.h"

class Browser;
class FullscreenController;
class FullscreenNotificationObserver;

#define ENUM_TO_STRING(enum) case enum: return #enum

class FullscreenControllerStateTest {
 public:
  
  enum Event {
    TOGGLE_FULLSCREEN,         
    TOGGLE_FULLSCREEN_CHROME,  
    TAB_FULLSCREEN_TRUE,       
    TAB_FULLSCREEN_FALSE,      
    METRO_SNAP_TRUE,           
    METRO_SNAP_FALSE,          
    BUBBLE_EXIT_LINK,          
    BUBBLE_ALLOW,              
    BUBBLE_DENY,               
    WINDOW_CHANGE,             
    NUM_EVENTS,
    EVENT_INVALID,
  };

  
  
  enum State {
    
    STATE_NORMAL,
    
    STATE_BROWSER_FULLSCREEN_NO_CHROME,
    
    STATE_BROWSER_FULLSCREEN_WITH_CHROME,
    
    
    STATE_METRO_SNAP,
    
    STATE_TAB_FULLSCREEN,
    
    STATE_TAB_BROWSER_FULLSCREEN,
    
    
    STATE_TAB_BROWSER_FULLSCREEN_CHROME,
    
    
    STATE_TO_NORMAL,
    STATE_TO_BROWSER_FULLSCREEN_NO_CHROME,
    STATE_TO_BROWSER_FULLSCREEN_WITH_CHROME,
    STATE_TO_TAB_FULLSCREEN,
    NUM_STATES,
    STATE_INVALID,
  };

  static const int kMaxStateNameLength = 39;
  static const int kMaxEventNameLength = 24;

  FullscreenControllerStateTest();
  virtual ~FullscreenControllerStateTest();

  static const char* GetStateString(State state);
  static const char* GetEventString(Event event);

  
  
  
  static bool IsWindowFullscreenStateChangedReentrant();

  
  
  static bool IsPersistentState(State state);

  
  void TransitionToState(State state);

  
  
  
  bool TransitionAStepTowardState(State destination_state);

  
  
  virtual void ChangeWindowFullscreenState() {}

  
  
  virtual const char* GetWindowStateString();

  
  virtual bool InvokeEvent(Event event);

  
  virtual void VerifyWindowState();

  
  
  void MaybeWaitForNotification();

  
  
  
  
  
  
  
  
  void TestTransitionsForEachState();

  
  std::string GetTransitionTableAsString() const;
  
  std::string GetStateTransitionsAsString() const;

 protected:
  
  
  #define EXPECTATION_ENUM(enum_name, enum_prefix) \
      enum enum_name { \
        enum_prefix##_FALSE, \
        enum_prefix##_TRUE, \
        enum_prefix##_NO_EXPECTATION \
      }
  EXPECTATION_ENUM(FullscreenWithChromeExpectation, FULLSCREEN_WITH_CHROME);
  EXPECTATION_ENUM(FullscreenWithoutChromeExpectation,
                   FULLSCREEN_WITHOUT_CHROME);
  EXPECTATION_ENUM(FullscreenForBrowserExpectation, FULLSCREEN_FOR_BROWSER);
  EXPECTATION_ENUM(FullscreenForTabExpectation, FULLSCREEN_FOR_TAB);
  EXPECTATION_ENUM(InMetroSnapExpectation, IN_METRO_SNAP);

  
  struct StateTransitionInfo {
    StateTransitionInfo()
        : event(EVENT_INVALID),
          state(STATE_INVALID),
          distance(NUM_STATES) {}
    Event event;  
    State state;  
    int distance;  
  };

  
  StateTransitionInfo NextTransitionInShortestPath(State source,
                                                   State destination,
                                                   int search_limit);

  
  
  std::string GetAndClearDebugLog();

  
  virtual bool ShouldSkipStateAndEventPair(State state, Event event);

  
  virtual bool ShouldSkipTest(State state, Event event);

  
  virtual void TestStateAndEvent(State state, Event event);

  
  virtual void VerifyWindowStateExpectations(
      FullscreenWithChromeExpectation fullscreen_with_chrome,
      FullscreenWithoutChromeExpectation fullscreen_without_chrome,
      FullscreenForBrowserExpectation fullscreen_for_browser,
      FullscreenForTabExpectation fullscreen_for_tab,
      InMetroSnapExpectation in_metro_snap);


  virtual Browser* GetBrowser() = 0;
  FullscreenController* GetFullscreenController();

  
  State state() const { return state_; }

 private:
  
  State state_;

  
  
  State last_notification_received_state_;

  
  scoped_ptr<FullscreenNotificationObserver> fullscreen_notification_observer_;

  
  State transition_table_[NUM_STATES][NUM_EVENTS];

  
  
  
  
  StateTransitionInfo state_transitions_[NUM_STATES][NUM_STATES];

  
  std::ostringstream debugging_log_;

  DISALLOW_COPY_AND_ASSIGN(FullscreenControllerStateTest);
};

#endif  
