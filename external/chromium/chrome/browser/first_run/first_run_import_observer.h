// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_FIRST_RUN_FIRST_RUN_IMPORT_OBSERVER_H_
#define CHROME_BROWSER_FIRST_RUN_FIRST_RUN_IMPORT_OBSERVER_H_
#pragma once

#include "base/basictypes.h"
#include "base/compiler_specific.h"
#include "chrome/browser/importer/importer_observer.h"

class FirstRunImportObserver : public ImporterObserver {
 public:
  FirstRunImportObserver();
  virtual ~FirstRunImportObserver();

  void RunLoop();

  int import_result() const { return import_result_; }

 private:
  void Finish();

  
  virtual void ImportCompleted() OVERRIDE;

  bool loop_running_;
  int import_result_;

  DISALLOW_COPY_AND_ASSIGN(FirstRunImportObserver);
};

#endif  
