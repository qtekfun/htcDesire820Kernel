// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_UI_GLOBAL_ERROR_GLOBAL_ERROR_SERVICE_H_
#define CHROME_BROWSER_UI_GLOBAL_ERROR_GLOBAL_ERROR_SERVICE_H_

#include <vector>

#include "base/basictypes.h"
#include "components/browser_context_keyed_service/browser_context_keyed_service.h"

class GlobalError;
class Profile;

class GlobalErrorService : public BrowserContextKeyedService {
 public:
  
  typedef std::vector<GlobalError*> GlobalErrorList;

  
  
  explicit GlobalErrorService(Profile* profile);
  virtual ~GlobalErrorService();

  
  
  
  
  void AddGlobalError(GlobalError* error);

  
  
  void RemoveGlobalError(GlobalError* error);

  
  
  GlobalError* GetGlobalErrorByMenuItemCommandID(int command_id) const;

  
  
  GlobalError* GetHighestSeverityGlobalErrorWithWrenchMenuItem() const;

  
  
  GlobalError* GetFirstGlobalErrorWithBubbleView() const;

  
  const GlobalErrorList& errors() { return errors_; }

  
  
  
  void NotifyErrorsChanged(GlobalError* error);

 private:
  GlobalErrorList errors_;
  Profile* profile_;

  DISALLOW_COPY_AND_ASSIGN(GlobalErrorService);
};

#endif  
