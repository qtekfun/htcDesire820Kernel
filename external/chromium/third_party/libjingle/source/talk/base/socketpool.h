/*
 * libjingle
 * Copyright 2004--2005, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice, 
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products 
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TALK_BASE_SOCKETPOOL_H_
#define TALK_BASE_SOCKETPOOL_H_

#include <deque>
#include <list>
#include "talk/base/logging.h"
#include "talk/base/sigslot.h"
#include "talk/base/socketaddress.h"

namespace talk_base {

class AsyncSocket;
class LoggingAdapter;
class SocketFactory;
class SocketStream;
class StreamInterface;


class StreamPool {
public:
  virtual ~StreamPool() { }

  virtual StreamInterface* RequestConnectedStream(const SocketAddress& remote,
                                                  int* err) = 0;
  virtual void ReturnConnectedStream(StreamInterface* stream) = 0;
};


class StreamCache : public StreamPool, public sigslot::has_slots<> {
public:
  StreamCache(StreamPool* pool);
  virtual ~StreamCache();

  
  virtual StreamInterface* RequestConnectedStream(const SocketAddress& remote,
                                                  int* err);
  virtual void ReturnConnectedStream(StreamInterface* stream);

private:
  typedef std::pair<SocketAddress, StreamInterface*> ConnectedStream;
  typedef std::list<ConnectedStream> ConnectedList;

  void OnStreamEvent(StreamInterface* stream, int events, int err);

  
  StreamPool* pool_;
  
  ConnectedList active_;
  
  ConnectedList cached_;
};


class NewSocketPool : public StreamPool {
public:
  NewSocketPool(SocketFactory* factory);
  virtual ~NewSocketPool();
  
  
  virtual StreamInterface* RequestConnectedStream(const SocketAddress& remote,
                                                  int* err);
  virtual void ReturnConnectedStream(StreamInterface* stream);
  
private:
  SocketFactory* factory_;
};


class ReuseSocketPool : public StreamPool, public sigslot::has_slots<> {
public:
  ReuseSocketPool(SocketFactory* factory);
  virtual ~ReuseSocketPool();

  
  virtual StreamInterface* RequestConnectedStream(const SocketAddress& remote,
                                                  int* err);
  virtual void ReturnConnectedStream(StreamInterface* stream);
  
private:
  void OnStreamEvent(StreamInterface* stream, int events, int err);

  SocketFactory* factory_;
  SocketStream* stream_;
  SocketAddress remote_;
  bool checked_out_;  
};


class LoggingPoolAdapter : public StreamPool {
public:
  LoggingPoolAdapter(StreamPool* pool, LoggingSeverity level,
                     const std::string& label, bool binary_mode);
  virtual ~LoggingPoolAdapter();

  
  virtual StreamInterface* RequestConnectedStream(const SocketAddress& remote,
                                                  int* err);
  virtual void ReturnConnectedStream(StreamInterface* stream);

private:
  StreamPool* pool_;
  LoggingSeverity level_;
  std::string label_;
  bool binary_mode_;
  typedef std::deque<LoggingAdapter*> StreamList;
  StreamList recycle_bin_;
};


}  

#endif  
