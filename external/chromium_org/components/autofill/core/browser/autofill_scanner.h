// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_SCANNER_H_
#define COMPONENTS_AUTOFILL_CORE_BROWSER_AUTOFILL_SCANNER_H_

#include <vector>

#include "base/basictypes.h"
#include "base/strings/string16.h"

namespace autofill {

class AutofillField;

class AutofillScanner {
 public:
  explicit AutofillScanner(const std::vector<const AutofillField*>& fields);
  ~AutofillScanner();

  
  void Advance();

  
  
  const AutofillField* Cursor() const;

  
  bool IsEnd() const;

  
  void Rewind();

  
  void RewindTo(size_t index);

  
  
  size_t SaveCursor();

 private:
  
  std::vector<const AutofillField*>::const_iterator cursor_;

  
  std::vector<const AutofillField*>::const_iterator saved_cursor_;

  
  const std::vector<const AutofillField*>::const_iterator begin_;

  
  const std::vector<const AutofillField*>::const_iterator end_;

  DISALLOW_COPY_AND_ASSIGN(AutofillScanner);
};

}  

#endif  
