// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_CHROMEOS_INPUT_METHOD_CANDIDATE_WINDOW_CONTROLLER_H_
#define CHROME_BROWSER_CHROMEOS_INPUT_METHOD_CANDIDATE_WINDOW_CONTROLLER_H_

#include "base/basictypes.h"

namespace chromeos {
namespace input_method {

class CandidateWindowController {
 public:
  class Observer {
   public:
    virtual ~Observer() {}

    virtual void CandidateClicked(int index) = 0;
    virtual void CandidateWindowOpened() = 0;
    virtual void CandidateWindowClosed() = 0;
  };

  virtual ~CandidateWindowController() {}

  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;
  virtual void Hide() = 0;

  
  
  static CandidateWindowController* CreateCandidateWindowController();
};

}  
}  

#endif  
