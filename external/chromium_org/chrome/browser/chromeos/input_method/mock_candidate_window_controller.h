// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_MOCK_CANDIDATE_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_MOCK_CANDIDATE_WINDOW_CONTROLLER_H_

#include "base/basictypes.h"
#include "base/observer_list.h"
#include "chrome/browser/chromeos/input_method/candidate_window_controller.h"

namespace chromeos {
namespace input_method {

class MockCandidateWindowController : public CandidateWindowController {
 public:
  MockCandidateWindowController();
  virtual ~MockCandidateWindowController();

  
  virtual void AddObserver(Observer* observer) OVERRIDE;
  virtual void RemoveObserver(Observer* observer) OVERRIDE;
  virtual void Hide() OVERRIDE;

  
  void NotifyCandidateWindowOpened();
  void NotifyCandidateWindowClosed();

  int add_observer_count_;
  int remove_observer_count_;
  int hide_count_;

 private:
  ObserverList<CandidateWindowController::Observer> observers_;

  DISALLOW_COPY_AND_ASSIGN(MockCandidateWindowController);
};

}  
}  

#endif  