// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_UTILITY_IMPORTER_IMPORTER_H_
#define CHROME_UTILITY_IMPORTER_IMPORTER_H_

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"

class ImporterBridge;

namespace importer {
struct SourceProfile;
}

class Importer : public base::RefCountedThreadSafe<Importer> {
 public:
  
  
  
  
  virtual void StartImport(const importer::SourceProfile& source_profile,
                           uint16 items,
                           ImporterBridge* bridge) = 0;

  
  virtual void Cancel();

  bool cancelled() const { return cancelled_; }

 protected:
  friend class base::RefCountedThreadSafe<Importer>;

  Importer();
  virtual ~Importer();

  scoped_refptr<ImporterBridge> bridge_;

 private:
  
  bool cancelled_;

  DISALLOW_COPY_AND_ASSIGN(Importer);
};

#endif  
