// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_SOCKET_STREAM_SOCKET_STREAM_JOB_H_
#define NET_SOCKET_STREAM_SOCKET_STREAM_JOB_H_
#pragma once

#include <string>

#include "base/memory/ref_counted.h"
#include "base/string16.h"
#include "net/socket_stream/socket_stream.h"
#include "net/base/net_export.h"

class GURL;

namespace net {

class NET_EXPORT SocketStreamJob : public base::RefCountedThreadSafe<SocketStreamJob> {
 public:
  
  typedef SocketStreamJob* (ProtocolFactory)(const GURL& url,
                                             SocketStream::Delegate* delegate);

  static ProtocolFactory* RegisterProtocolFactory(const std::string& scheme,
                                                  ProtocolFactory* factory);

  static SocketStreamJob* CreateSocketStreamJob(
      const GURL& url,
      SocketStream::Delegate* delegate,
      const URLRequestContext& context);

  SocketStreamJob();
  void InitSocketStream(SocketStream* socket) {
    socket_ = socket;
  }

  virtual SocketStream::UserData* GetUserData(const void* key) const;
  virtual void SetUserData(const void* key, SocketStream::UserData* data);

  URLRequestContext* context() const {
    return socket_->context();
  }
  void set_context(URLRequestContext* context) {
    socket_->set_context(context);
  }

  virtual void Connect();

  virtual bool SendData(const char* data, int len);

  virtual void Close();

  virtual void RestartWithAuth(const string16& username,
                               const string16& password);

  virtual void DetachDelegate();

 protected:
  friend class WebSocketJobTest;
  friend class base::RefCountedThreadSafe<SocketStreamJob>;
  virtual ~SocketStreamJob();

  scoped_refptr<SocketStream> socket_;

  DISALLOW_COPY_AND_ASSIGN(SocketStreamJob);
};

}  

#endif  
