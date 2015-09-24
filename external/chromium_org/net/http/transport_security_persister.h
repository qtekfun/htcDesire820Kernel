// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef NET_HTTP_TRANSPORT_SECURITY_PERSISTER_H_
#define NET_HTTP_TRANSPORT_SECURITY_PERSISTER_H_

#include <string>

#include "base/files/file_path.h"
#include "base/files/important_file_writer.h"
#include "base/memory/ref_counted.h"
#include "base/memory/weak_ptr.h"
#include "net/base/net_export.h"
#include "net/http/transport_security_state.h"

namespace base {
class SequencedTaskRunner;
}

namespace net {

class NET_EXPORT TransportSecurityPersister
    : public TransportSecurityState::Delegate,
      public base::ImportantFileWriter::DataSerializer {
 public:
  TransportSecurityPersister(TransportSecurityState* state,
                             const base::FilePath& profile_path,
                             base::SequencedTaskRunner* file_task_runner,
                             bool readonly);
  virtual ~TransportSecurityPersister();

  
  virtual void StateIsDirty(TransportSecurityState*) OVERRIDE;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  virtual bool SerializeData(std::string* data) OVERRIDE;

  
  
  
  
  
  bool LoadEntries(const std::string& serialized, bool* dirty);

 private:
  
  
  
  
  
  static bool Deserialize(const std::string& serialized,
                          bool* dirty,
                          TransportSecurityState* state);

  void CompleteLoad(const std::string& state);

  TransportSecurityState* transport_security_state_;

  
  base::ImportantFileWriter writer_;

  scoped_refptr<base::SequencedTaskRunner> foreground_runner_;
  scoped_refptr<base::SequencedTaskRunner> background_runner_;

  
  const bool readonly_;

  base::WeakPtrFactory<TransportSecurityPersister> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(TransportSecurityPersister);
};

}  

#endif  
