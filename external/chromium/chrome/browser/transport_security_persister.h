// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef CHROME_BROWSER_TRANSPORT_SECURITY_PERSISTER_H_
#define CHROME_BROWSER_TRANSPORT_SECURITY_PERSISTER_H_
#pragma once

#include "base/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/task.h"
#include "net/base/transport_security_state.h"

class TransportSecurityPersister
    : public base::RefCountedThreadSafe<TransportSecurityPersister>,
      public net::TransportSecurityState::Delegate {
 public:
  explicit TransportSecurityPersister(bool readonly);
  void Initialize(net::TransportSecurityState* state,
                  const FilePath& profile_path);

  
  virtual void StateIsDirty(net::TransportSecurityState*);

 private:
  friend class base::RefCountedThreadSafe<TransportSecurityPersister>;

  ~TransportSecurityPersister();

  void Load();
  void CompleteLoad(const std::string& state);

  void Save();
  void CompleteSave(const std::string& state);

  
  ScopedRunnableMethodFactory<TransportSecurityPersister> save_coalescer_;

  scoped_refptr<net::TransportSecurityState>
      transport_security_state_;  

  
  FilePath state_file_;

  
  bool readonly_;
};

#endif  
