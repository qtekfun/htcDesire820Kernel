// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_PROXY_NACL_MESSAGE_SCANNER_H_
#define PPAPI_PROXY_NACL_MESSAGE_SCANNER_H_

#include <map>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "ppapi/proxy/ppapi_proxy_export.h"

namespace IPC {
class Message;
}

namespace ppapi {
namespace proxy {

class SerializedHandle;

class PPAPI_PROXY_EXPORT NaClMessageScanner {
 public:
  NaClMessageScanner();

  
  
  // rewritten for NaCl, sets |new_msg_ptr| to the new message. If no handles
  
  
  
  
  
  
  
  bool ScanMessage(const IPC::Message& msg,
                   std::vector<SerializedHandle>* handles,
                   scoped_ptr<IPC::Message>* new_msg_ptr);

  
  
  
  
  
  void RegisterSyncMessageForReply(const IPC::Message& msg);

 private:
  
  
  
  typedef std::map<int, uint32> PendingSyncMsgMap;
  PendingSyncMsgMap pending_sync_msgs_;

  DISALLOW_COPY_AND_ASSIGN(NaClMessageScanner);
};

}  
}  

#endif  
